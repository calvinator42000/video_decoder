#include <sps.h>

Sequence_Parameter_Set* seq_parameter_set_rbsp() {
    Sequence_Parameter_Set* sps = initSPS();
    sps->sps_seq_parameter_set_id = u(4);
    sps->sps_video_parameter_set_id = u(4);
    sps->sps_max_sublayers_minus1 = u(3);
    sps->sps_chroma_format_idc = u(2);
    sps->sps_log2_ctu_size_minus5 = u(2);

    // 7.4.3.4 Sequence parameter set RBSP semantics
    uint_t CtbLog2SizeY = sps->sps_log2_ctu_size_minus5 + 5;
    sps->CtbSizeY = 1 << CtbLog2SizeY;

    sps->sps_ptl_dpb_hrd_params_present_flag = u(1);
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        sps->ptl = profile_tier_level(1, sps->sps_max_sublayers_minus1);
    }
    sps->sps_gdr_enabled_flag = u(1);
    sps->sps_ref_pic_resampling_enabled_flag = u(1);
    if (sps->sps_ref_pic_resampling_enabled_flag) {
        sps->sps_res_change_in_clvs_allowed_flag = u(1);
    } else {
        sps->sps_res_change_in_clvs_allowed_flag = 0;
    }
    sps->sps_pic_width_max_in_luma_samples = ue();
    sps->sps_pic_height_max_in_luma_samples = ue();

    // 7.4.3.4 Sequence parameter set RBSP semantics
    double tmpWidthVal = (double)(sps->sps_pic_width_max_in_luma_samples + sps->CtbSizeY - 1) / (double)(sps->CtbSizeY);
    double tmpHeightVal = (double)(sps->sps_pic_height_max_in_luma_samples + sps->CtbSizeY - 1) / (double)(sps->CtbSizeY);

    sps->sps_conformance_window_flag = u(1);
    if (sps->sps_conformance_window_flag) {
        sps->sps_conf_win_left_offset = ue();
        sps->sps_conf_win_right_offset = ue();
        sps->sps_conf_win_top_offset = ue();
        sps->sps_conf_win_bottom_offset = ue();
    } else {
        sps->sps_conf_win_left_offset = 0;
        sps->sps_conf_win_right_offset = 0;
        sps->sps_conf_win_top_offset = 0;
        sps->sps_conf_win_bottom_offset = 0;
    }
    sps->sps_subpic_info_present_flag = u(1);
    if (sps->sps_subpic_info_present_flag) {
        sps->sps_num_subpics_minus1 = ue();
        if (sps->sps_num_subpics_minus1 > 0) {
            sps->sps_independent_subpics_flag = u(1);
            sps->sps_subpic_same_size_flag = u(1);
        } else {
            sps->sps_independent_subpics_flag = 1;
            sps->sps_subpic_same_size_flag = 0;
        }
        sps->sps_subpic_ctu_top_left_x = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_subpic_ctu_top_left_y = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_subpic_width_minus1 = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_subpic_height_minus1 = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_subpic_treated_as_pic_flag = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_loop_filter_across_subpic_enabled_flag = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        uint_t numSubpicCols;
        for (int i = 0; sps->sps_num_subpics_minus1 > 0 && i <= sps->sps_num_subpics_minus1; i++) {
            if (!sps->sps_subpic_same_size_flag || i == 0) {
                if (i > 0 && sps->sps_pic_width_max_in_luma_samples > sps->CtbSizeY) {
                    sps->sps_subpic_ctu_top_left_x[i] = u(ceil(log2(tmpWidthVal)));
                } else {
                    sps->sps_subpic_ctu_top_left_x[i] = 0;
                }
                if (i > 0 && sps->sps_pic_height_max_in_luma_samples > sps->CtbSizeY) {
                    sps->sps_subpic_ctu_top_left_y[i] = u(ceil(log2(tmpHeightVal)));
                } else {
                    sps->sps_subpic_ctu_top_left_y[i] = 0;
                }
                if (i < sps->sps_num_subpics_minus1 && sps->sps_pic_width_max_in_luma_samples > sps->CtbSizeY) {
                    sps->sps_subpic_width_minus1[i] = u(ceil(log2(tmpWidthVal)));
                } else {
                    sps->sps_subpic_width_minus1[i] = tmpWidthVal - sps->sps_subpic_ctu_top_left_x[i] - 1;
                }
                if (i < sps->sps_num_subpics_minus1 && sps->sps_pic_height_max_in_luma_samples > sps->CtbSizeY) {
                    sps->sps_subpic_height_minus1[i] = u(ceil(log2(tmpHeightVal)));
                } else {
                    sps->sps_subpic_height_minus1[i] = tmpHeightVal - sps->sps_subpic_ctu_top_left_y[i] - 1;
                }
            } else {
                sps->sps_subpic_ctu_top_left_x[i] = (i % numSubpicCols) * (sps->sps_subpic_width_minus1[0] + 1);
                sps->sps_subpic_ctu_top_left_y[i] = (i / numSubpicCols) * (sps->sps_subpic_height_minus1[0] + 1);
                sps->sps_subpic_width_minus1[i] = sps->sps_subpic_width_minus1[0];
                sps->sps_subpic_height_minus1[i] = sps->sps_subpic_height_minus1[0];
            }
            if (sps->sps_subpic_same_size_flag && i == 0) {
                numSubpicCols = tmpWidthVal / (sps->sps_subpic_width_minus1[0] + 1);
            }
            if (!sps->sps_independent_subpics_flag) {
                sps->sps_subpic_treated_as_pic_flag[i] = u(1);
                sps->sps_loop_filter_across_subpic_enabled_flag[i] = u(1);
            } else {
                sps->sps_subpic_treated_as_pic_flag[i] = 1;
                sps->sps_loop_filter_across_subpic_enabled_flag[i] = 0;
            }
        }
        sps->sps_subpic_id_len_minus1 = ue();
        sps->sps_subpic_id_mapping_explicitly_signalled_flag = u(1);
        if (sps->sps_subpic_id_mapping_explicitly_signalled_flag) {
            sps->sps_subpic_id_mapping_present_flag = u(1);
            if (sps->sps_subpic_id_mapping_present_flag) {
                sps->sps_subpic_id = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
                for (int i = 0; i <= sps->sps_num_subpics_minus1; i++) {
                    sps->sps_subpic_id[i] = u(sps->sps_subpic_id_len_minus1 + 1);
                }
            }
        }
    } else {
        sps->sps_num_subpics_minus1 = 0;
        sps->sps_independent_subpics_flag = 1;
        sps->sps_subpic_same_size_flag = 0;
        sps->sps_subpic_id_mapping_explicitly_signalled_flag = 0;
    }
    sps->sps_bitdepth_minus8 = ue();
    sps->sps_entropy_coding_sync_enabled_flag = u(1);
    sps->sps_entry_point_offsets_present_flag = u(1);
    sps->sps_log2_max_pic_order_cnt_lsb_minus4 = u(4);
    sps->sps_poc_msb_cycle_flag = u(1);
    if (sps->sps_poc_msb_cycle_flag) {
        sps->sps_poc_msb_cycle_len_minus1 = ue();
    }
    sps->sps_num_extra_ph_bytes = u(2);
    sps->sps_extra_ph_bit_present_flag = malloc(sizeof(uint_t) * (sps->sps_num_extra_ph_bytes * 8));
    for (int i = 0; i < (sps->sps_num_extra_ph_bytes * 8); i++) {
        sps->sps_extra_ph_bit_present_flag[i] = u(1);
    }
    sps->sps_num_extra_sh_bytes = u(2);
    sps->sps_extra_sh_bit_present_flag = malloc(sizeof(uint_t) * (sps->sps_num_extra_sh_bytes * 8));
    for (int i = 0; i < (sps->sps_num_extra_sh_bytes * 8); i++) {
        sps->sps_extra_sh_bit_present_flag[i] = u(1);
    }
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        if (sps->sps_max_sublayers_minus1 > 0) {
            sps->sps_sublayer_dpb_params_flag = u(1);
        } else {
            sps->sps_sublayer_dpb_params_flag = 0;
        }
        sps->dpb = dpb_parameters(sps->sps_max_sublayers_minus1, sps->sps_sublayer_dpb_params_flag);
    } else {
        sps->sps_sublayer_dpb_params_flag = 0;
    }
    sps->sps_log2_min_luma_coding_block_size_minus2 = ue();
    sps->sps_partition_constraints_override_enabled_flag = u(1);
    sps->sps_log2_diff_min_qt_min_cb_intra_slice_luma = ue();
    sps->sps_max_mtt_hierarchy_depth_intra_slice_luma = ue();
    if (sps->sps_max_mtt_hierarchy_depth_intra_slice_luma != 0) {
        sps->sps_log2_diff_max_bt_min_qt_intra_slice_luma = ue();
        sps->sps_log2_diff_max_tt_min_qt_intra_slice_luma = ue();
    } else {
        sps->sps_log2_diff_max_bt_min_qt_intra_slice_luma = 0;
        sps->sps_log2_diff_max_tt_min_qt_intra_slice_luma = 0;
    }
    if (sps->sps_chroma_format_idc != 0) {
        sps->sps_qtbtt_dual_tree_intra_flag = u(1);
    } else {
        sps->sps_qtbtt_dual_tree_intra_flag = 0;
    }
    if (sps->sps_qtbtt_dual_tree_intra_flag) {
        sps->sps_log2_diff_min_qt_min_cb_intra_slice_chroma = ue();
        sps->sps_max_mtt_hierarchy_depth_intra_slice_chroma = ue();
        if (sps->sps_max_mtt_hierarchy_depth_intra_slice_chroma != 0) {
            sps->sps_log2_diff_max_bt_min_qt_intra_slice_chroma = ue();
            sps->sps_log2_diff_max_tt_min_qt_intra_slice_chroma = ue();
        } else {
            sps->sps_log2_diff_max_bt_min_qt_intra_slice_chroma = 0;
            sps->sps_log2_diff_max_tt_min_qt_intra_slice_chroma = 0;
        }
    } else {
        sps->sps_log2_diff_min_qt_min_cb_intra_slice_chroma = 0;
        sps->sps_max_mtt_hierarchy_depth_intra_slice_chroma = 0;
        sps->sps_log2_diff_max_bt_min_qt_intra_slice_chroma = 0;
        sps->sps_log2_diff_max_tt_min_qt_intra_slice_chroma = 0;
    }
    sps->sps_log2_diff_min_qt_min_cb_inter_slice = ue();
    sps->sps_max_mtt_hierarchy_depth_inter_slice = ue();
    if (sps->sps_max_mtt_hierarchy_depth_inter_slice != 0) {
        sps->sps_log2_diff_max_bt_min_qt_inter_slice = ue();
        sps->sps_log2_diff_max_tt_min_qt_inter_slice = ue();
    } else {
        sps->sps_log2_diff_max_bt_min_qt_inter_slice = 0;
        sps->sps_log2_diff_max_tt_min_qt_inter_slice = 0;
    }
    if (sps->CtbSizeY > 32) {
        sps->sps_max_luma_transform_size_64_flag = u(1);
    } else {
        sps->sps_max_luma_transform_size_64_flag = 0;
    }
    sps->sps_transform_skip_enabled_flag = u(1);
    if (sps->sps_transform_skip_enabled_flag) {
        sps->sps_log2_transform_skip_max_size_minus2 = ue();
        sps->sps_bdpcm_enabled_flag = u(1);
    } else {
        sps->sps_bdpcm_enabled_flag = 0;
    }
    sps->sps_mts_enabled_flag = u(1);
    if (sps->sps_mts_enabled_flag) {
        sps->sps_explicit_mts_intra_enabled_flag = u(1);
        sps->sps_explicit_mts_inter_enabled_flag = u(1);
    } else {
        sps->sps_explicit_mts_intra_enabled_flag = 0;
        sps->sps_explicit_mts_inter_enabled_flag = 0;
    }
    sps->sps_lfnst_enabled_flag = u(1);
    if (sps->sps_chroma_format_idc != 0) {
        sps->sps_joint_cbcr_enabled_flag = u(1);
        sps->sps_same_qp_table_for_chroma_flag = u(1);
        sps->numQpTables = sps->sps_same_qp_table_for_chroma_flag ? 1 : (sps->sps_joint_cbcr_enabled_flag ? 3 : 2);
        sps->sps_qp_table_start_minus26 = malloc(sizeof(int) * sps->numQpTables);
        sps->sps_num_points_in_qp_table_minus1 = malloc(sizeof(uint_t) * sps->numQpTables);
        sps->sps_delta_qp_in_val_minus1 = malloc(sizeof(uint_t*) * sps->numQpTables);
        sps->sps_delta_qp_diff_val = malloc(sizeof(uint_t*) * sps->numQpTables);
        for (int i = 0; i < sps->numQpTables; i++) {
            sps->sps_qp_table_start_minus26[i] = se();
            sps->sps_num_points_in_qp_table_minus1[i] = ue();
            sps->sps_delta_qp_in_val_minus1[i] = malloc(sizeof(uint_t) * (sps->sps_num_points_in_qp_table_minus1[i] + 1));
            sps->sps_delta_qp_diff_val[i] = malloc(sizeof(uint_t) * (sps->sps_num_points_in_qp_table_minus1[i] + 1));
            for (int j = 0; j <= sps->sps_num_points_in_qp_table_minus1[i]; j++) {
                sps->sps_delta_qp_in_val_minus1[i][j] = ue();
                sps->sps_delta_qp_diff_val[i][j] = ue();
            }
        }
    } else {
        sps->sps_joint_cbcr_enabled_flag = 0;
        sps->sps_same_qp_table_for_chroma_flag = 1;
        sps->sps_qp_table_start_minus26 = malloc(sizeof(int) * 1);
        sps->sps_qp_table_start_minus26[0] = 0;
        sps->sps_num_points_in_qp_table_minus1 = malloc(sizeof(uint_t) * 1);
        sps->sps_num_points_in_qp_table_minus1[0] = 0;
        sps->sps_delta_qp_in_val_minus1 = malloc(sizeof(uint_t*) * 1);
        sps->sps_delta_qp_in_val_minus1[0] = malloc(sizeof(uint_t) * 1);
        sps->sps_delta_qp_in_val_minus1[0][0] = 0;
    }
    sps->sps_sao_enabled_flag = u(1);
    sps->sps_alf_enabled_flag = u(1);
    if (sps->sps_alf_enabled_flag && sps->sps_chroma_format_idc != 0) {
        sps->sps_ccalf_enabled_flag = u(1);
    } else {
        sps->sps_ccalf_enabled_flag = 0;
    }
    sps->sps_lmcs_enabled_flag = u(1);
    sps->sps_weighted_pred_flag = u(1);
    sps->sps_weighted_bipred_flag = u(1);
    sps->sps_long_term_ref_pics_flag = u(1);
    if (sps->sps_video_parameter_set_id > 0) {
        sps->sps_inter_layer_prediction_enabled_flag = u(1);
    } else {
        sps->sps_inter_layer_prediction_enabled_flag = 0;
    }
    sps->sps_idr_rpl_present_flag = u(1);
    sps->sps_rpl1_same_as_rpl0_flag = u(1);
    sps->sps_num_ref_pic_lists = malloc(sizeof(uint_t) * 2);
    sps->rpls = malloc(sizeof(Reference_Picture_List_Structure**) * 2);
    for (int i = 0; i < (sps->sps_rpl1_same_as_rpl0_flag ? 1 : 2); i++) {
        sps->sps_num_ref_pic_lists[i] = ue();
        if (sps->sps_rpl1_same_as_rpl0_flag) {
            sps->sps_num_ref_pic_lists[1] = sps->sps_num_ref_pic_lists[0];
        }
        sps->rpls[i] = malloc(sizeof(Reference_Picture_List_Structure*) * sps->sps_num_ref_pic_lists[i]);
        for (int j = 0; j < sps->sps_num_ref_pic_lists[i]; j++) {
            sps->rpls[i][j] = ref_pic_list_struct(i, j, sps);
            if (sps->sps_rpl1_same_as_rpl0_flag) {
                sps->rpls[1][j] = sps->rpls[0][j];
            }
        }
    }
    sps->sps_ref_wraparound_enabled_flag = u(1);
    sps->sps_temporal_mvp_enabled_flag = u(1);
    if (sps->sps_temporal_mvp_enabled_flag) {
        sps->sps_sbtmvp_enabled_flag = u(1);
    } else {
        sps->sps_sbtmvp_enabled_flag = 0;
    }
    sps->sps_amvr_enabled_flag = u(1);
    sps->sps_bdof_enabled_flag = u(1);
    if (sps->sps_bdof_enabled_flag) {
        sps->sps_bdof_control_present_in_ph_flag = u(1);
    } else {
        sps->sps_bdof_control_present_in_ph_flag = 0;
    }
    sps->sps_smvd_enabled_flag = u(1);
    sps->sps_dmvr_enabled_flag = u(1);
    if (sps->sps_dmvr_enabled_flag) {
        sps->sps_dmvr_control_present_in_ph_flag = u(1);
    } else {
        sps->sps_dmvr_control_present_in_ph_flag = 0;
    }
    sps->sps_mmvd_enabled_flag = u(1);
    if (sps->sps_mmvd_enabled_flag) {
        sps->sps_mmvd_fullpel_only_enabled_flag = u(1);
    } else {
        sps->sps_mmvd_fullpel_only_enabled_flag = 0;
    }
    sps->sps_six_minus_max_num_merge_cand = ue();
    sps->MaxNumMergeCand = 6 - sps->sps_six_minus_max_num_merge_cand;
    sps->sps_sbt_enabled_flag = u(1);
    sps->sps_affine_enabled_flag = u(1);
    if (sps->sps_affine_enabled_flag) {
        sps->sps_five_minus_max_num_subblock_merge_cand = ue();
        sps->sps_6param_affine_enabled_flag = u(1);
        if (sps->sps_amvr_enabled_flag) {
            sps->sps_affine_amvr_enabled_flag = u(1);
        } else {
            sps->sps_affine_amvr_enabled_flag = 0;
        }
        sps->sps_affine_prof_enabled_flag = u(1);
        if (sps->sps_affine_prof_enabled_flag) {
            sps->sps_prof_control_present_in_ph_flag = u(1);
        } else {
            sps->sps_prof_control_present_in_ph_flag = 0;
        }
    } else {
        sps->sps_6param_affine_enabled_flag = 0;
        sps->sps_affine_amvr_enabled_flag = 0;
        sps->sps_affine_prof_enabled_flag = 0;
        sps->sps_prof_control_present_in_ph_flag = 0;
    }
    sps->sps_bcw_enabled_flag = u(1);
    sps->sps_ciip_enabled_flag = u(1);
    if (sps->MaxNumMergeCand >= 2) {
        sps->sps_gpm_enabled_flag = u(1);
        if (sps->sps_gpm_enabled_flag && sps->MaxNumMergeCand >= 3) {
            sps->sps_max_num_merge_cand_minus_max_num_gpm_cand = ue();
        }
    } else {
        sps->sps_gpm_enabled_flag = 0;
    }
    sps->sps_log2_parallel_merge_level_minus2 = ue();
    sps->sps_isp_enabled_flag = u(1);
    sps->sps_mrl_enabled_flag = u(1);
    sps->sps_mip_enabled_flag = u(1);
    if (sps->sps_chroma_format_idc != 0) {
        sps->sps_cclm_enabled_flag = u(1);
    } else {
        sps->sps_cclm_enabled_flag = 0;
    }
    if (sps->sps_chroma_format_idc == 1) {
        sps->sps_chroma_horizontal_collocated_flag = u(1);
        sps->sps_chroma_vertical_collocated_flag = u(1);
    } else {
        sps->sps_chroma_horizontal_collocated_flag = 1;
        sps->sps_chroma_vertical_collocated_flag = 1;
    }
    sps->sps_palette_enabled_flag = u(1);
    if (sps->sps_chroma_format_idc == 3 && !sps->sps_max_luma_transform_size_64_flag) {
        sps->sps_act_enabled_flag = u(1);
    } else {
        sps->sps_act_enabled_flag = 0;
    }
    if (sps->sps_transform_skip_enabled_flag || sps->sps_palette_enabled_flag) {
        sps->sps_min_qp_prime_ts = ue();
    }
    sps->sps_ibc_enabled_flag = u(1);
    if (sps->sps_ibc_enabled_flag) {
        sps->sps_six_minus_max_num_ibc_merge_cand = ue();
    }
    sps->sps_ladf_enabled_flag = u(1);
    if (sps->sps_ladf_enabled_flag) {
        sps->sps_num_ladf_intervals_minus2 = u(2);
        sps->sps_ladf_lowest_interval_qp_offset = se();
        sps->sps_ladf_qp_offset = malloc(sizeof(int) * (sps->sps_num_ladf_intervals_minus2 + 1));
        sps->sps_ladf_delta_threshold_minus1 = malloc(sizeof(int) * (sps->sps_num_ladf_intervals_minus2 + 1));
        for (int i = 0; i < sps->sps_num_ladf_intervals_minus2 + 1; i++) {
            sps->sps_ladf_qp_offset[i] = se();
            sps->sps_ladf_delta_threshold_minus1[i] = ue();
        }
    }
    sps->sps_explicit_scaling_list_enabled_flag = u(1);
    if (sps->sps_lfnst_enabled_flag && sps->sps_explicit_scaling_list_enabled_flag) {
        sps->sps_scaling_matrix_for_lfnst_disabled_flag = u(1);
    }
    if (sps->sps_act_enabled_flag && sps->sps_explicit_scaling_list_enabled_flag) {
        sps->sps_scaling_matrix_for_alternative_colour_space_disabled_flag = u(1);
    } else {
        sps->sps_scaling_matrix_for_alternative_colour_space_disabled_flag = 0;
    }
    if (sps->sps_scaling_matrix_for_alternative_colour_space_disabled_flag) {
        sps->sps_scaling_matrix_designated_colour_space_flag = u(1);
    }
    sps->sps_dep_quant_enabled_flag = u(1);
    sps->sps_sign_data_hiding_enabled_flag = u(1);
    sps->sps_virtual_boundaries_enabled_flag = u(1);
    if (sps->sps_virtual_boundaries_enabled_flag) {
        sps->sps_virtual_boundaries_present_flag = u(1);
        if (sps->sps_virtual_boundaries_present_flag) {
            sps->sps_num_ver_virtual_boundaries = ue();
            sps->sps_virtual_boundary_pos_x_minus1 = malloc(sizeof(uint_t) * sps->sps_num_ver_virtual_boundaries);
            for (int i = 0; i < sps->sps_num_ver_virtual_boundaries; i++) {
                sps->sps_virtual_boundary_pos_x_minus1[i] = ue();
            }
            sps->sps_num_hor_virtual_boundaries = ue();
            sps->sps_virtual_boundary_pos_y_minus1 = malloc(sizeof(uint_t) * sps->sps_num_hor_virtual_boundaries);
            for (int i = 0; i < sps->sps_num_hor_virtual_boundaries; i++) {
                sps->sps_virtual_boundary_pos_y_minus1[i] = ue();
            }
        } else {
            sps->sps_num_ver_virtual_boundaries = 0;
            sps->sps_num_hor_virtual_boundaries = 0;
        }
    } else {
        sps->sps_virtual_boundaries_present_flag = 0;
        sps->sps_num_ver_virtual_boundaries = 0;
        sps->sps_num_hor_virtual_boundaries = 0;
    }
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        sps->sps_timing_hrd_params_present_flag = u(1);
        if (sps->sps_timing_hrd_params_present_flag) {
            sps->gth = general_timing_hrd_parameters();
            if (sps->sps_max_sublayers_minus1 > 0) {
                sps->sps_sublayer_cpb_params_present_flag = u(1);
            } else {
                sps->sps_sublayer_cpb_params_present_flag = 0;
            }
            uint_t firstSubLayer = sps->sps_sublayer_cpb_params_present_flag ? 0 : sps->sps_max_sublayers_minus1;
            sps->oth = ols_timing_hrd_parameters(firstSubLayer, sps->sps_max_sublayers_minus1, sps->gth);
        }
    }
    if (sps->sps_max_sublayers_minus1 == 0) {
        sps->sps_sublayer_cpb_params_present_flag = 0;
    }
    sps->sps_field_seq_flag = u(1);
    sps->sps_vui_parameters_present_flag = u(1);
    if (sps->sps_vui_parameters_present_flag) {
        sps->sps_vui_payload_size_minus1 = ue();
        while(!byte_aligned()) {
            f(1, sps_vui_alignment_zero_bit);
        }
        // vui_payload(sps->sps_vui_payload_size_minus1 + 1);
    }
    // TODO: finish implementing this
    return sps;
}

Sequence_Parameter_Set* initSPS() {
    Sequence_Parameter_Set* sps = malloc(sizeof(Sequence_Parameter_Set));
    if (sps == NULL) {
        printf("Memory allocation failed: Sequence_Parameter_Set\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    sps->ptl = NULL;
    sps->sps_subpic_ctu_top_left_x = NULL;
    sps->sps_subpic_ctu_top_left_y = NULL;
    sps->sps_subpic_width_minus1 = NULL;
    sps->sps_subpic_height_minus1 = NULL;
    sps->sps_subpic_treated_as_pic_flag = NULL;
    sps->sps_loop_filter_across_subpic_enabled_flag = NULL;
    sps->sps_subpic_id = NULL;
    sps->sps_extra_ph_bit_present_flag = NULL;
    sps->sps_extra_sh_bit_present_flag = NULL;
    sps->dpb = NULL;
    sps->sps_qp_table_start_minus26 = NULL;
    sps->sps_num_points_in_qp_table_minus1 = NULL;
    sps->sps_delta_qp_in_val_minus1 = NULL;
    sps->sps_delta_qp_diff_val = NULL;
    sps->sps_num_ref_pic_lists = NULL;
    sps->rpls = NULL;
    sps->sps_ladf_qp_offset = NULL;
    sps->sps_ladf_delta_threshold_minus1 = NULL;
    sps->sps_virtual_boundary_pos_x_minus1 = NULL;
    sps->sps_virtual_boundary_pos_y_minus1 = NULL;
    sps->gth = NULL;
    sps->oth = NULL;
    return sps;
}

void freeSPS(Sequence_Parameter_Set* sps) {
    if (sps) {
        if (sps->ptl) {
            freePTL(sps->ptl);
        }
        if (sps->sps_subpic_ctu_top_left_x) {
            free(sps->sps_subpic_ctu_top_left_x);
        }
        if (sps->sps_subpic_ctu_top_left_y) {
            free(sps->sps_subpic_ctu_top_left_y);
        }
        if (sps->sps_subpic_width_minus1) {
            free(sps->sps_subpic_width_minus1);
        }
        if (sps->sps_subpic_height_minus1) {
            free(sps->sps_subpic_height_minus1);
        }
        if (sps->sps_subpic_treated_as_pic_flag) {
            free(sps->sps_subpic_treated_as_pic_flag);
        }
        if (sps->sps_loop_filter_across_subpic_enabled_flag) {
            free(sps->sps_loop_filter_across_subpic_enabled_flag);
        }
        if (sps->sps_subpic_id) {
            free(sps->sps_subpic_id);
        }
        if (sps->sps_extra_ph_bit_present_flag) {
            free(sps->sps_extra_ph_bit_present_flag);
        }
        if (sps->sps_extra_sh_bit_present_flag) {
            free(sps->sps_extra_sh_bit_present_flag);
        }
        if (sps->dpb) {
            freeDPB(sps->dpb);
        }
        if (sps->sps_qp_table_start_minus26) {
            free(sps->sps_qp_table_start_minus26);
        }
        if (sps->sps_num_points_in_qp_table_minus1) {
            if (sps->sps_delta_qp_in_val_minus1) {
                for (int i = 0; i < sps->numQpTables; i++) {
                    if (sps->sps_delta_qp_in_val_minus1[i]) {
                        free(sps->sps_delta_qp_in_val_minus1[i]);
                    }
                }
                free(sps->sps_delta_qp_in_val_minus1);
            }
            if (sps->sps_delta_qp_diff_val) {
                for (int i = 0; i < sps->numQpTables; i++) {
                    if (sps->sps_delta_qp_diff_val[i]) {
                        free(sps->sps_delta_qp_diff_val[i]);
                    }
                }
                free(sps->sps_delta_qp_diff_val);
            }
            free(sps->sps_num_points_in_qp_table_minus1);
        }
        if (sps->sps_num_ref_pic_lists) {
            if (sps->rpls) {
                for (int i = 0; i < 2; i++) {
                    if (sps->rpls[i]) {
                        for (int j = 0; j < sps->sps_num_ref_pic_lists[i]; j++) {
                            if (sps->rpls[i][j]) {
                                freeRPLS(sps->rpls[i][j]);
                            }
                        }
                        free(sps->rpls[i]);
                    }
                }
                free(sps->rpls);
            }
            free(sps->sps_num_ref_pic_lists);
        }
        if (sps->sps_ladf_qp_offset) {
            free(sps->sps_ladf_qp_offset);
        }
        if (sps->sps_ladf_delta_threshold_minus1) {
            free(sps->sps_ladf_delta_threshold_minus1);
        }
        if (sps->sps_virtual_boundary_pos_x_minus1) {
            free(sps->sps_virtual_boundary_pos_x_minus1);
        }
        if (sps->sps_virtual_boundary_pos_y_minus1) {
            free(sps->sps_virtual_boundary_pos_y_minus1);
        }
        if (sps->gth) {
            freeGTH(sps->gth);
        }
        if (sps->oth) {
            freeOTH(sps->oth);
        }
        free(sps);
    }
}

void printSPS(Sequence_Parameter_Set* sps) {
    incIndent();
    if (sps == NULL) {
        printVar("Sequence Parameter Set is NULL\n");
        return;
    }
    printVar("Sequence Parameter Set:\n");
    printVar("  sps_seq_parameter_set_id: %u\n", sps->sps_seq_parameter_set_id);
    printVar("  sps_video_parameter_set_id: %u\n", sps->sps_video_parameter_set_id);
    printVar("  sps_max_sublayers_minus1: %u\n", sps->sps_max_sublayers_minus1);
    printVar("  sps_chroma_format_idc: %u\n", sps->sps_chroma_format_idc);
    printVar("  sps_log2_ctu_size_minus5: %u\n", sps->sps_log2_ctu_size_minus5);
    printVar("  sps_ptl_dpb_hrd_params_present_flag: %u\n", sps->sps_ptl_dpb_hrd_params_present_flag);
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        printPTL(sps->ptl);
    }
    printVar("  sps_gdr_enabled_flag: %u\n", sps->sps_gdr_enabled_flag);
    printVar("  sps_ref_pic_resampling_enabled_flag: %u\n", sps->sps_ref_pic_resampling_enabled_flag);
    printVar("  sps_res_change_in_clvs_allowed_flag: %u\n", sps->sps_res_change_in_clvs_allowed_flag);
    printVar("  sps_pic_width_max_in_luma_samples: %u\n", sps->sps_pic_width_max_in_luma_samples);
    printVar("  sps_pic_height_max_in_luma_samples: %u\n", sps->sps_pic_height_max_in_luma_samples);
    printVar("  sps_conformance_window_flag: %u\n", sps->sps_conformance_window_flag);
    printVar("  sps_conf_win_left_offset: %u\n", sps->sps_conf_win_left_offset);
    printVar("  sps_conf_win_right_offset: %u\n", sps->sps_conf_win_right_offset);
    printVar("  sps_conf_win_top_offset: %u\n", sps->sps_conf_win_top_offset);
    printVar("  sps_conf_win_bottom_offset: %u\n", sps->sps_conf_win_bottom_offset);
    printVar("  sps_subpic_info_present_flag: %u\n", sps->sps_subpic_info_present_flag);
    printVar("  sps_num_subpics_minus1: %u\n", sps->sps_num_subpics_minus1);
    printVar("  sps_independent_subpics_flag: %u\n", sps->sps_independent_subpics_flag);
    printVar("  sps_subpic_same_size_flag: %u\n", sps->sps_subpic_same_size_flag);
    if (sps->sps_subpic_info_present_flag) {
        printVar("  sps_subpic_ctu_top_left_x: {");
        for (int i = 0; i <= sps->sps_num_subpics_minus1; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_subpic_ctu_top_left_x[i]);
        }
        printf("}\n");
        printVar("  sps_subpic_ctu_top_left_y: {");
        for (int i = 0; i <= sps->sps_num_subpics_minus1; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_subpic_ctu_top_left_y[i]);
        }
        printf("}\n");
        printVar("  sps_subpic_width_minus1: {");
        for (int i = 0; i <= sps->sps_num_subpics_minus1; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_subpic_width_minus1[i]);
        }
        printf("}\n");
        printVar("  sps_subpic_height_minus1: {");
        for (int i = 0; i <= sps->sps_num_subpics_minus1; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_subpic_height_minus1[i]);
        }
        printf("}\n");
        printVar("  sps_subpic_treated_as_pic_flag: {");
        for (int i = 0; i <= sps->sps_num_subpics_minus1; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_subpic_treated_as_pic_flag[i]);
        }
        printf("}\n");
        printVar("  sps_loop_filter_across_subpic_enabled_flag: {");
        for (int i = 0; i <= sps->sps_num_subpics_minus1; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_loop_filter_across_subpic_enabled_flag[i]);
        }
        printf("}\n");
        printVar("  sps_subpic_id_len_minus1: %u\n", sps->sps_subpic_id_len_minus1);
        printVar("  sps_subpic_id_mapping_explicitly_signalled_flag: %u\n", sps->sps_subpic_id_mapping_explicitly_signalled_flag);
        if (sps->sps_subpic_id_mapping_explicitly_signalled_flag) {
            printVar("  sps_subpic_id_mapping_present_flag: %u\n", sps->sps_subpic_id_mapping_present_flag);
            if (sps->sps_subpic_id_mapping_present_flag) {
                printVar("  sps_subpic_id: {");
                for (int i = 0; i <= sps->sps_num_subpics_minus1; i++) {
                    if (i > 0) {
                        printf(",");
                    }
                    printf("%u", sps->sps_subpic_id[i]);
                }
                printf("}\n");
            }
        }
    } else {
        printVar("  sps_subpic_id_mapping_explicitly_signalled_flag: %u\n", sps->sps_subpic_id_mapping_explicitly_signalled_flag);
    }
    printVar("  sps_bitdepth_minus8: %u\n", sps->sps_bitdepth_minus8);
    printVar("  sps_entropy_coding_sync_enabled_flag: %u\n", sps->sps_entropy_coding_sync_enabled_flag);
    printVar("  sps_entry_point_offsets_present_flag: %u\n", sps->sps_entry_point_offsets_present_flag);
    printVar("  sps_log2_max_pic_order_cnt_lsb_minus4: %u\n", sps->sps_log2_max_pic_order_cnt_lsb_minus4);
    printVar("  sps_poc_msb_cycle_flag: %u\n", sps->sps_poc_msb_cycle_flag);
    if (sps->sps_poc_msb_cycle_flag) {
        printVar("  sps_poc_msb_cycle_len_minus1: %u\n", sps->sps_poc_msb_cycle_len_minus1);
    }
    printVar("  sps_num_extra_ph_bytes: %u\n", sps->sps_num_extra_ph_bytes);
    printVar("  sps_extra_ph_bit_present_flag: {");
    for (int i = 0; i < (sps->sps_num_extra_ph_bytes * 8); i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%u", sps->sps_extra_ph_bit_present_flag[i]);
    }
    printf("}\n");
    printVar("  sps_num_extra_sh_bytes: %u\n", sps->sps_num_extra_sh_bytes);
    printVar("  sps_extra_sh_bit_present_flag: {");
    for (int i = 0; i < (sps->sps_num_extra_sh_bytes * 8); i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%u", sps->sps_extra_sh_bit_present_flag[i]);
    }
    printf("}\n");
    printVar("  sps_sublayer_dpb_params_flag: %u\n", sps->sps_sublayer_dpb_params_flag);
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        printDPB(sps->dpb);
    }
    printVar("  sps_log2_min_luma_coding_block_size_minus2: %u\n", sps->sps_log2_min_luma_coding_block_size_minus2);
    printVar("  sps_partition_constraints_override_enabled_flag: %u\n", sps->sps_partition_constraints_override_enabled_flag);
    printVar("  sps_log2_diff_min_qt_min_cb_intra_slice_luma: %u\n", sps->sps_log2_diff_min_qt_min_cb_intra_slice_luma);
    sps->sps_log2_diff_min_qt_min_cb_intra_slice_luma = ue();
    printVar("  sps_max_mtt_hierarchy_depth_intra_slice_luma: %u\n", sps->sps_max_mtt_hierarchy_depth_intra_slice_luma);
    printVar("  sps_log2_diff_max_bt_min_qt_intra_slice_luma: %u\n", sps->sps_log2_diff_max_bt_min_qt_intra_slice_luma);
    printVar("  sps_log2_diff_max_tt_min_qt_intra_slice_luma: %u\n", sps->sps_log2_diff_max_tt_min_qt_intra_slice_luma);
    printVar("  sps_qtbtt_dual_tree_intra_flag: %u\n", sps->sps_qtbtt_dual_tree_intra_flag);
    printVar("  sps_log2_diff_min_qt_min_cb_intra_slice_chroma: %u\n", sps->sps_log2_diff_min_qt_min_cb_intra_slice_chroma);
    printVar("  sps_max_mtt_hierarchy_depth_intra_slice_chroma: %u\n", sps->sps_max_mtt_hierarchy_depth_intra_slice_chroma);
    printVar("  sps_log2_diff_max_bt_min_qt_intra_slice_chroma: %u\n", sps->sps_log2_diff_max_bt_min_qt_intra_slice_chroma);
    printVar("  sps_log2_diff_max_tt_min_qt_intra_slice_chroma: %u\n", sps->sps_log2_diff_max_tt_min_qt_intra_slice_chroma);
    printVar("  sps_log2_diff_min_qt_min_cb_inter_slice: %u\n", sps->sps_log2_diff_min_qt_min_cb_inter_slice);
    printVar("  sps_max_mtt_hierarchy_depth_inter_slice: %u\n", sps->sps_max_mtt_hierarchy_depth_inter_slice);
    printVar("  sps_log2_diff_max_bt_min_qt_inter_slice: %u\n", sps->sps_log2_diff_max_bt_min_qt_inter_slice);
    printVar("  sps_log2_diff_max_tt_min_qt_inter_slice: %u\n", sps->sps_log2_diff_max_tt_min_qt_inter_slice);
    printVar("  sps_max_luma_transform_size_64_flag: %u\n", sps->sps_max_luma_transform_size_64_flag);
    printVar("  sps_transform_skip_enabled_flag: %u\n", sps->sps_transform_skip_enabled_flag);
    if (sps->sps_transform_skip_enabled_flag) {
        printVar("  sps_log2_transform_skip_max_size_minus2: %u\n", sps->sps_log2_transform_skip_max_size_minus2);
    }
    printVar("  sps_bdpcm_enabled_flag: %u\n", sps->sps_bdpcm_enabled_flag);
    printVar("  sps_mts_enabled_flag: %u\n", sps->sps_mts_enabled_flag);
    printVar("  sps_explicit_mts_intra_enabled_flag: %u\n", sps->sps_explicit_mts_intra_enabled_flag);
    printVar("  sps_explicit_mts_inter_enabled_flag: %u\n", sps->sps_explicit_mts_inter_enabled_flag);
    printVar("  sps_lfnst_enabled_flag: %u\n", sps->sps_lfnst_enabled_flag);
    printVar("  sps_joint_cbcr_enabled_flag: %u\n", sps->sps_joint_cbcr_enabled_flag);
    printVar("  sps_same_qp_table_for_chroma_flag: %u\n", sps->sps_same_qp_table_for_chroma_flag);
    printVar("  sps_qp_table_start_minus26: {");
    for (int i = 0; i < sps->numQpTables; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%i", sps->sps_qp_table_start_minus26[i]);
    }
    printf("}\n");
    printVar("  sps_num_points_in_qp_table_minus1: {");
    for (int i = 0; i < sps->numQpTables; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%u", sps->sps_num_points_in_qp_table_minus1[i]);
    }
    printf("}\n");
    printVar("  sps_delta_qp_in_val_minus1: {");
    for (int i = 0; i < sps->numQpTables; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("{");
        for (int j = 0; j <= sps->sps_num_points_in_qp_table_minus1[i]; j++) {
            if (j > 0) {
                printf(",");
            }
            printf("%u", sps->sps_delta_qp_in_val_minus1[i][j]);
        }
        printf("}");
    }
    printf("}\n");
    if (sps->sps_chroma_format_idc != 0) {
        printVar("  sps_delta_qp_diff_val: {");
        for (int i = 0; i < sps->numQpTables; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("{");
            for (int j = 0; j <= sps->sps_num_points_in_qp_table_minus1[i]; j++) {
                if (j > 0) {
                    printf(",");
                }
                printf("%u", sps->sps_delta_qp_diff_val[i][j]);
            }
            printf("}");
        }
        printf("}\n");
    }
    printVar("  sps_sao_enabled_flag: %u\n", sps->sps_sao_enabled_flag);
    printVar("  sps_alf_enabled_flag: %u\n", sps->sps_alf_enabled_flag);
    printVar("  sps_ccalf_enabled_flag: %u\n", sps->sps_ccalf_enabled_flag);
    printVar("  sps_lmcs_enabled_flag: %u\n", sps->sps_lmcs_enabled_flag);
    printVar("  sps_weighted_pred_flag: %u\n", sps->sps_weighted_pred_flag);
    printVar("  sps_weighted_bipred_flag: %u\n", sps->sps_weighted_bipred_flag);
    printVar("  sps_long_term_ref_pics_flag: %u\n", sps->sps_long_term_ref_pics_flag);
    printVar("  sps_inter_layer_prediction_enabled_flag: %u\n", sps->sps_inter_layer_prediction_enabled_flag);
    printVar("  sps_idr_rpl_present_flag: %u\n", sps->sps_idr_rpl_present_flag);
    printVar("  sps_rpl1_same_as_rpl0_flag: %u\n", sps->sps_rpl1_same_as_rpl0_flag);
    printVar("  sps_num_ref_pic_lists: {");
    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%u", sps->sps_num_ref_pic_lists[i]);
    }
    printf("}\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < sps->sps_num_ref_pic_lists[i]; j++) {
            printRPLS(sps->rpls[i][j]);
        }
    }
    printVar("  sps_ref_wraparound_enabled_flag: %u\n", sps->sps_ref_wraparound_enabled_flag);
    printVar("  sps_temporal_mvp_enabled_flag: %u\n", sps->sps_temporal_mvp_enabled_flag);
    printVar("  sps_sbtmvp_enabled_flag: %u\n", sps->sps_sbtmvp_enabled_flag);
    printVar("  sps_amvr_enabled_flag: %u\n", sps->sps_amvr_enabled_flag);
    printVar("  sps_bdof_enabled_flag: %u\n", sps->sps_bdof_enabled_flag);
    printVar("  sps_bdof_control_present_in_ph_flag: %u\n", sps->sps_bdof_control_present_in_ph_flag);
    printVar("  sps_smvd_enabled_flag: %u\n", sps->sps_smvd_enabled_flag);
    printVar("  sps_dmvr_enabled_flag: %u\n", sps->sps_dmvr_enabled_flag);
    printVar("  sps_dmvr_control_present_in_ph_flag: %u\n", sps->sps_dmvr_control_present_in_ph_flag);
    printVar("  sps_mmvd_enabled_flag: %u\n", sps->sps_mmvd_enabled_flag);
    printVar("  sps_mmvd_fullpel_only_enabled_flag: %u\n", sps->sps_mmvd_fullpel_only_enabled_flag);
    printVar("  sps_six_minus_max_num_merge_cand: %u\n", sps->sps_six_minus_max_num_merge_cand);
    printVar("  sps_sbt_enabled_flag: %u\n", sps->sps_sbt_enabled_flag);
    printVar("  sps_affine_enabled_flag: %u\n", sps->sps_affine_enabled_flag);
    if (sps->sps_affine_enabled_flag) {
        printVar("  sps_five_minus_max_num_subblock_merge_cand: %u\n", sps->sps_five_minus_max_num_subblock_merge_cand);
    }
    printVar("  sps_6param_affine_enabled_flag: %u\n", sps->sps_6param_affine_enabled_flag);
    printVar("  sps_affine_amvr_enabled_flag: %u\n", sps->sps_affine_amvr_enabled_flag);
    printVar("  sps_affine_prof_enabled_flag: %u\n", sps->sps_affine_prof_enabled_flag);
    printVar("  sps_prof_control_present_in_ph_flag: %u\n", sps->sps_prof_control_present_in_ph_flag);
    printVar("  sps_bcw_enabled_flag: %u\n", sps->sps_bcw_enabled_flag);
    printVar("  sps_ciip_enabled_flag: %u\n", sps->sps_ciip_enabled_flag);
    printVar("  sps_gpm_enabled_flag: %u\n", sps->sps_gpm_enabled_flag);
    if (sps->sps_gpm_enabled_flag && sps->MaxNumMergeCand >=3) {
        printVar("  sps_max_num_merge_cand_minus_max_num_gpm_cand: %u\n", sps->sps_max_num_merge_cand_minus_max_num_gpm_cand);
    }
    printVar("  sps_log2_parallel_merge_level_minus2: %u\n", sps->sps_log2_parallel_merge_level_minus2);
    printVar("  sps_isp_enabled_flag: %u\n", sps->sps_isp_enabled_flag);
    printVar("  sps_mrl_enabled_flag: %u\n", sps->sps_mrl_enabled_flag);
    printVar("  sps_mip_enabled_flag: %u\n", sps->sps_mip_enabled_flag);
    printVar("  sps_cclm_enabled_flag: %u\n", sps->sps_cclm_enabled_flag);
    printVar("  sps_chroma_horizontal_collocated_flag: %u\n", sps->sps_chroma_horizontal_collocated_flag);
    printVar("  sps_chroma_vertical_collocated_flag: %u\n", sps->sps_chroma_vertical_collocated_flag);
    printVar("  sps_palette_enabled_flag: %u\n", sps->sps_palette_enabled_flag);
    printVar("  sps_act_enabled_flag: %u\n", sps->sps_act_enabled_flag);
    if (sps->sps_transform_skip_enabled_flag || sps->sps_palette_enabled_flag) {
        printVar("  sps_min_qp_prime_ts: %u\n", sps->sps_min_qp_prime_ts);
    }
    printVar("  sps_ibc_enabled_flag: %u\n", sps->sps_ibc_enabled_flag);
    if (sps->sps_ibc_enabled_flag) {
        printVar("  sps_six_minus_max_num_ibc_merge_cand: %u\n", sps->sps_six_minus_max_num_ibc_merge_cand);
    }
    printVar("  sps_ladf_enabled_flag: %u\n", sps->sps_ladf_enabled_flag);
    if (sps->sps_ladf_enabled_flag) {
        printVar("  sps_num_ladf_intervals_minus2: %u\n", sps->sps_num_ladf_intervals_minus2);
        printVar("  sps_ladf_lowest_interval_qp_offset: %i\n", sps->sps_ladf_lowest_interval_qp_offset);
        printVar("  sps_ladf_qp_offset: {");
        for (int i = 0; i < sps->sps_num_ladf_intervals_minus2 + 1; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%i", sps->sps_ladf_qp_offset[i]);
        }
        printf("}\n");
        printVar("  sps_ladf_delta_threshold_minus1: {");
        for (int i = 0; i < sps->sps_num_ladf_intervals_minus2 + 1; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_ladf_delta_threshold_minus1[i]);
        }
        printf("}\n");
    }
    printVar("  sps_explicit_scaling_list_enabled_flag: %u\n", sps->sps_explicit_scaling_list_enabled_flag);
    if (sps->sps_lfnst_enabled_flag && sps->sps_explicit_scaling_list_enabled_flag) {
        printVar("  sps_scaling_matrix_for_lfnst_disabled_flag: %u\n", sps->sps_scaling_matrix_for_lfnst_disabled_flag);
    }
    printVar("  sps_scaling_matrix_for_alternative_colour_space_disabled_flag: %u\n", sps->sps_scaling_matrix_for_alternative_colour_space_disabled_flag);
    if (sps->sps_scaling_matrix_for_alternative_colour_space_disabled_flag) {
        printVar("  sps_scaling_matrix_designated_colour_space_flag: %u\n", sps->sps_scaling_matrix_designated_colour_space_flag);
    }
    printVar("  sps_dep_quant_enabled_flag: %u\n", sps->sps_dep_quant_enabled_flag);
    printVar("  sps_sign_data_hiding_enabled_flag: %u\n", sps->sps_sign_data_hiding_enabled_flag);
    printVar("  sps_virtual_boundaries_present_flag: %u\n", sps->sps_virtual_boundaries_present_flag);
    printVar("  sps_num_ver_virtual_boundaries: %u\n", sps->sps_num_ver_virtual_boundaries);
    if (sps->sps_virtual_boundaries_enabled_flag && sps->sps_virtual_boundaries_present_flag) {
        printVar("  sps_virtual_boundary_pos_x_minus1: {");
        for (int i = 0; i < sps->sps_num_ver_virtual_boundaries; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_virtual_boundary_pos_x_minus1[i]);
        }
        printf("}\n");
    }
    printVar("  sps_num_hor_virtual_boundaries: %u\n", sps->sps_num_hor_virtual_boundaries);
    if (sps->sps_virtual_boundaries_enabled_flag && sps->sps_virtual_boundaries_present_flag) {
        printVar("  sps_virtual_boundary_pos_y_minus1: {");
        for (int i = 0; i < sps->sps_num_hor_virtual_boundaries; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", sps->sps_virtual_boundary_pos_y_minus1[i]);
        }
        printf("}\n");
    }
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        printVar("  sps_timing_hrd_params_present_flag: %u\n", sps->sps_timing_hrd_params_present_flag);
        if (sps->sps_timing_hrd_params_present_flag) {
            printGTH(sps->gth);
        }
    }
    printVar("  sps_sublayer_cpb_params_present_flag: %u\n", sps->sps_sublayer_cpb_params_present_flag);
    if (sps->sps_ptl_dpb_hrd_params_present_flag && sps->sps_timing_hrd_params_present_flag) {
        printOTH(sps->oth);
    }
    printVar("  sps_field_seq_flag: %u\n", sps->sps_field_seq_flag);
    printVar("  sps_vui_parameters_present_flag: %u\n", sps->sps_vui_parameters_present_flag);
    if (sps->sps_vui_parameters_present_flag) {
        printVar("  sps_vui_payload_size_minus1: %u\n", sps->sps_vui_payload_size_minus1);
    }
    // printVar("  : %u\n", sps->);
    // printVar("  : %u\n", sps->);
    // printVar("  : %u\n", sps->);
    // printVar("  : %u\n", sps->);
    // printVar("  : %u\n", sps->);
    decIndent();
}