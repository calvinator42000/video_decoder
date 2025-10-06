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
    }
    sps->sps_subpic_info_present_flag = u(1);
    if (sps->sps_subpic_info_present_flag) {
        sps->sps_num_subpics_minus1 = ue();
        if (sps->sps_num_subpics_minus1 > 0) {
            sps->sps_independent_subpics_flag = u(1);
            sps->sps_subpic_same_size_flag = u(1);
        }
        sps->sps_subpic_ctu_top_left_x = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_subpic_ctu_top_left_y = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_subpic_width_minus1 = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_subpic_height_minus1 = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_subpic_treated_as_pic_flag = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        sps->sps_loop_filter_across_subpic_enabled_flag = malloc(sizeof(uint_t) * (sps->sps_num_subpics_minus1 + 1));
        for (int i = 0; sps->sps_num_subpics_minus1 > 0 && i <= sps->sps_num_subpics_minus1; i++) {
            if (!sps->sps_subpic_same_size_flag || i == 0) {
                if (i > 0 && sps->sps_pic_width_max_in_luma_samples > sps->CtbSizeY) {
                    sps->sps_subpic_ctu_top_left_x[i] = u(ceil(log2(tmpWidthVal)));
                }
                if (i > 0 && sps->sps_pic_height_max_in_luma_samples > sps->CtbSizeY) {
                    sps->sps_subpic_ctu_top_left_y[i] = u(ceil(log2(tmpHeightVal)));
                }
                if (i < sps->sps_num_subpics_minus1 && sps->sps_pic_width_max_in_luma_samples > sps->CtbSizeY) {
                    sps->sps_subpic_width_minus1[i] = u(ceil(log2(tmpWidthVal)));
                }
                if (i < sps->sps_num_subpics_minus1 && sps->sps_pic_height_max_in_luma_samples > sps->CtbSizeY) {
                    sps->sps_subpic_height_minus1[i] = u(ceil(log2(tmpHeightVal)));
                }
            }
            if (!sps->sps_independent_subpics_flag) {
                sps->sps_subpic_treated_as_pic_flag[i] = u(1);
                sps->sps_loop_filter_across_subpic_enabled_flag[i] = u(1);
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
        }
        sps->dpb = dpb_parameters(sps->sps_max_sublayers_minus1, sps->sps_sublayer_dpb_params_flag);
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
    if (sps->sps_ref_pic_resampling_enabled_flag) {
        printVar("  sps_res_change_in_clvs_allowed_flag: %u\n", sps->sps_res_change_in_clvs_allowed_flag);
    }
    printVar("  sps_pic_width_max_in_luma_samples: %u\n", sps->sps_pic_width_max_in_luma_samples);
    printVar("  sps_pic_height_max_in_luma_samples: %u\n", sps->sps_pic_height_max_in_luma_samples);
    printVar("  sps_conformance_window_flag: %u\n", sps->sps_conformance_window_flag);
    if (sps->sps_conformance_window_flag) {
        printVar("  sps_conf_win_left_offset: %u\n", sps->sps_conf_win_left_offset);
        printVar("  sps_conf_win_right_offset: %u\n", sps->sps_conf_win_right_offset);
        printVar("  sps_conf_win_top_offset: %u\n", sps->sps_conf_win_top_offset);
        printVar("  sps_conf_win_bottom_offset: %u\n", sps->sps_conf_win_bottom_offset);
    }
    printVar("  sps_subpic_info_present_flag: %u\n", sps->sps_subpic_info_present_flag);
    if (sps->sps_subpic_info_present_flag) {
        printVar("  sps_num_subpics_minus1: %u\n", sps->sps_num_subpics_minus1);
        if (sps->sps_num_subpics_minus1 > 0) {
            printVar("  sps_independent_subpics_flag: %u\n", sps->sps_independent_subpics_flag);
            printVar("  sps_subpic_same_size_flag: %u\n", sps->sps_subpic_same_size_flag);
        }
        for (int i = 0; sps->sps_num_subpics_minus1 > 0 && i <= sps->sps_num_subpics_minus1; i++) {
            if (!sps->sps_subpic_same_size_flag || i == 0) {
                if (i > 0 && sps->sps_pic_width_max_in_luma_samples > sps->CtbSizeY) {
                    printVar("  sps_subpic_ctu_top_left_x[%u]: %u\n", i, sps->sps_subpic_ctu_top_left_x[i]);
                }
                if (i > 0 && sps->sps_pic_height_max_in_luma_samples > sps->CtbSizeY) {
                    printVar("  sps_subpic_ctu_top_left_y[%u]: %u\n", i, sps->sps_subpic_ctu_top_left_y[i]);
                }
                if (i < sps->sps_num_subpics_minus1 && sps->sps_pic_width_max_in_luma_samples > sps->CtbSizeY) {
                    printVar("  sps_subpic_width_minus1[%u]: %u\n", i, sps->sps_subpic_width_minus1[i]);
                }
                if (i < sps->sps_num_subpics_minus1 && sps->sps_pic_height_max_in_luma_samples > sps->CtbSizeY) {
                    printVar("  sps_subpic_height_minus1[%u]: %u\n", i, sps->sps_subpic_height_minus1[i]);
                }
            }
            if (!sps->sps_independent_subpics_flag) {
                printVar("  sps_subpic_treated_as_pic_flag[%u]: %u\n", i, sps->sps_subpic_treated_as_pic_flag);
                printVar("  sps_loop_filter_across_subpic_enabled_flag[%u]: %u\n", i, sps->sps_loop_filter_across_subpic_enabled_flag);
            }
        }
        sps->sps_subpic_id_len_minus1 = ue();
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
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        if (sps->sps_max_sublayers_minus1 > 0) {
            printVar("  sps_sublayer_dpb_params_flag: %u\n", sps->sps_sublayer_dpb_params_flag);
        }
        printDPB(sps->dpb);
    }
    decIndent();
}