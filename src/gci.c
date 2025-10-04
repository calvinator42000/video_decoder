#include <gci.h>

General_Constraints_Info* general_constraints_info() {
    General_Constraints_Info* gci = initGCI();

    gci->gci_present_flag = u(1);
    if (gci->gci_present_flag) {
        /* general */
        gci->gci_intra_only_constraint_flag = u(1);
        gci->gci_all_layers_independent_constraint_flag = u(1);
        gci->gci_one_au_only_constraint_flag = u(1);
        /* picture format */
        gci->gci_sixteen_minus_max_bitdepth_constraint_idc = u(4);
        gci->gci_three_minus_max_chroma_format_constraint_idc = u(2);
        /* NAL unit type related */
        gci->gci_no_mixed_nalu_types_in_pic_constraint_flag = u(1);
        gci->gci_no_trail_constraint_flag = u(1);
        gci->gci_no_stsa_constraint_flag = u(1);
        gci->gci_no_rasl_constraint_flag = u(1);
        gci->gci_no_radl_constraint_flag = u(1);
        gci->gci_no_idr_constraint_flag = u(1);
        gci->gci_no_cra_constraint_flag = u(1);
        gci->gci_no_gdr_constraint_flag = u(1);
        gci->gci_no_aps_constraint_flag = u(1);
        gci->gci_no_idr_rpl_constraint_flag = u(1);
        /* tile, slice, subpicture partitioning */
        gci->gci_one_tile_per_pic_constraint_flag = u(1);
        gci->gci_pic_header_in_slice_header_constraint_flag = u(1);
        gci->gci_one_slice_per_pic_constraint_flag = u(1);
        gci->gci_no_rectangular_slice_constraint_flag = u(1);
        gci->gci_one_slice_per_subpic_constraint_flag = u(1);
        gci->gci_no_subpic_info_constraint_flag = u(1);
        /* CTU and block partitioning */
        gci->gci_three_minus_max_log2_ctu_size_constraint_idc = u(2);
        gci->gci_no_partition_constraints_override_constraint_flag = u(1);
        gci->gci_no_mtt_constraint_flag = u(1);
        gci->gci_no_qtbtt_dual_tree_intra_constraint_flag = u(1);
        /* intra */
        gci->gci_no_palette_constraint_flag = u(1);
        gci->gci_no_ibc_constraint_flag = u(1);
        gci->gci_no_isp_constraint_flag = u(1);
        gci->gci_no_mrl_constraint_flag = u(1);
        gci->gci_no_mip_constraint_flag = u(1);
        gci->gci_no_cclm_constraint_flag = u(1);
        /* inter */
        gci->gci_no_ref_pic_resampling_constraint_flag = u(1);
        gci->gci_no_res_change_in_clvs_constraint_flag = u(1);
        gci->gci_no_weighted_prediction_constraint_flag = u(1);
        gci->gci_no_ref_wraparound_constraint_flag = u(1);
        gci->gci_no_temporal_mvp_constraint_flag = u(1);
        gci->gci_no_sbtmvp_constraint_flag = u(1);
        gci->gci_no_amvr_constraint_flag = u(1);
        gci->gci_no_bdof_constraint_flag = u(1);
        gci->gci_no_smvd_constraint_flag = u(1);
        gci->gci_no_dmvr_constraint_flag = u(1);
        gci->gci_no_mmvd_constraint_flag = u(1);
        gci->gci_no_affine_motion_constraint_flag = u(1);
        gci->gci_no_prof_constraint_flag = u(1);
        gci->gci_no_bcw_constraint_flag = u(1);
        gci->gci_no_ciip_constraint_flag = u(1);
        gci->gci_no_gpm_constraint_flag = u(1);
        /* transform, quantization, residual */
        gci->gci_no_luma_transform_size_64_constraint_flag = u(1);
        gci->gci_no_transform_skip_constraint_flag = u(1);
        gci->gci_no_bdpcm_constraint_flag = u(1);
        gci->gci_no_mts_constraint_flag = u(1);
        gci->gci_no_lfnst_constraint_flag = u(1);
        gci->gci_no_joint_cbcr_constraint_flag = u(1);
        gci->gci_no_sbt_constraint_flag = u(1);
        gci->gci_no_act_constraint_flag = u(1);
        gci->gci_no_explicit_scaling_list_constraint_flag = u(1);
        gci->gci_no_dep_quant_constraint_flag = u(1);
        gci->gci_no_sign_data_hiding_constraint_flag = u(1);
        gci->gci_no_cu_qp_delta_constraint_flag = u(1);
        gci->gci_no_chroma_qp_offset_constraint_flag = u(1);
        /* loop filter */
        gci->gci_no_sao_constraint_flag = u(1);
        gci->gci_no_alf_constraint_flag = u(1);
        gci->gci_no_ccalf_constraint_flag = u(1);
        gci->gci_no_lmcs_constraint_flag = u(1);
        gci->gci_no_ladf_constraint_flag = u(1);
        gci->gci_no_virtual_boundaries_constraint_flag = u(1);
        gci->gci_num_additional_bits =-u(8);
        size_t numAdditionalBitsUsed;
        if (gci->gci_num_additional_bits > 5) {
            gci->gci_all_rap_pictures_constraint_flag = u(1);
            gci->gci_no_extended_precision_processing_constraint_flag = u(1);
            gci->gci_no_ts_residual_coding_rice_constraint_flag = u(1);
            gci->gci_no_rrc_rice_extension_constraint_flag = u(1);
            gci->gci_no_persistent_rice_adaptation_constraint_flag = u(1);
            gci->gci_no_reverse_last_sig_coeff_constraint_flag = u(1);
            numAdditionalBitsUsed = 6;
        } else {
            numAdditionalBitsUsed = 0;
        }
        gci->gci_reserved_bit = malloc(sizeof(uint_t) * (gci->gci_num_additional_bits - numAdditionalBitsUsed));
        for (size_t i = 0; i < gci->gci_num_additional_bits - numAdditionalBitsUsed; i++) {
            gci->gci_reserved_bit[i] = u(1);
        }
    }

    while(!byte_aligned()) {
        f(1, gci_alignment_zero_bit);
    }

    return gci;
}

General_Constraints_Info* initGCI() {
    General_Constraints_Info* gci = malloc(sizeof(General_Constraints_Info));
    if (gci == NULL) {
        printf("Memory allocation failed: General_Constraints_Info\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    gci->gci_reserved_bit = NULL;
    return gci;
}

void freeGCI(General_Constraints_Info* gci) {
    if (gci) {
        if (gci->gci_reserved_bit) {
            free(gci->gci_reserved_bit);
        }
        free(gci);
    }
}

void printGCI(General_Constraints_Info* gci) {
    if (gci == NULL) {
        printf("General Constraints Info is NULL\n");
        return;
    }
    printf("General Constraints Info:\n");
    printf("  gci_present_flag: %u\n", gci->gci_present_flag);
    if (gci->gci_present_flag) {
        /* general */
        printf("  gci_intra_only_constraint_flag: %u\n", gci->gci_intra_only_constraint_flag);
        printf("  gci_all_layers_independent_constraint_flag: %u\n", gci->gci_all_layers_independent_constraint_flag);
        printf("  gci_one_au_only_constraint_flag: %u\n", gci->gci_one_au_only_constraint_flag);
        /* picture format*/
        printf("  gci_sixteen_minus_max_bitdepth_constraint_idc: %u\n", gci->gci_sixteen_minus_max_bitdepth_constraint_idc);
        printf("  gci_three_minus_max_chroma_format_constraint_idc: %u\n", gci->gci_three_minus_max_chroma_format_constraint_idc);
        /* NAL unit type related */
        printf("  gci_no_mixed_nalu_types_in_pic_constraint_flag: %u\n", gci->gci_no_mixed_nalu_types_in_pic_constraint_flag);
        printf("  gci_no_trail_constraint_flag: %u\n", gci->gci_no_trail_constraint_flag);
        printf("  gci_no_stsa_constraint_flag: %u\n", gci->gci_no_stsa_constraint_flag);
        printf("  gci_no_rasl_constraint_flag: %u\n", gci->gci_no_rasl_constraint_flag);
        printf("  gci_no_radl_constraint_flag: %u\n", gci->gci_no_radl_constraint_flag);
        printf("  gci_no_idr_constraint_flag: %u\n", gci->gci_no_idr_constraint_flag);
        printf("  gci_no_cra_constraint_flag: %u\n", gci->gci_no_cra_constraint_flag);
        printf("  gci_no_gdr_constraint_flag: %u\n", gci->gci_no_gdr_constraint_flag);
        printf("  gci_no_aps_constraint_flag: %u\n", gci->gci_no_aps_constraint_flag);
        printf("  gci_no_idr_rpl_constraint_flag: %u\n", gci->gci_no_idr_rpl_constraint_flag);
        /* tile, slice, subpicture partitioning */
        printf("  gci_one_tile_per_pic_constraint_flag: %u\n", gci->gci_one_tile_per_pic_constraint_flag);
        printf("  gci_pic_header_in_slice_header_constraint_flag: %u\n", gci->gci_pic_header_in_slice_header_constraint_flag);
        printf("  gci_one_slice_per_pic_constraint_flag: %u\n", gci->gci_one_slice_per_pic_constraint_flag);
        printf("  gci_no_rectangular_slice_constraint_flag: %u\n", gci->gci_no_rectangular_slice_constraint_flag);
        printf("  gci_one_slice_per_subpic_constraint_flag: %u\n", gci->gci_one_slice_per_subpic_constraint_flag);
        printf("  gci_no_subpic_info_constraint_flag: %u\n", gci->gci_no_subpic_info_constraint_flag);
        /* CTU and block partitioning */
        printf("  gci_three_minus_max_log2_ctu_size_constraint_idc: %u\n", gci->gci_three_minus_max_log2_ctu_size_constraint_idc);
        printf("  gci_no_partition_constraints_override_constraint_flag: %u\n", gci->gci_no_partition_constraints_override_constraint_flag);
        printf("  gci_no_mtt_constraint_flag: %u\n", gci->gci_no_mtt_constraint_flag);
        printf("  gci_no_qtbtt_dual_tree_intra_constraint_flag: %u\n", gci->gci_no_qtbtt_dual_tree_intra_constraint_flag);
        /* intra */
        printf("  gci_no_palette_constraint_flag: %u\n", gci->gci_no_palette_constraint_flag);
        printf("  gci_no_ibc_constraint_flag: %u\n", gci->gci_no_ibc_constraint_flag);
        printf("  gci_no_isp_constraint_flag: %u\n", gci->gci_no_isp_constraint_flag);
        printf("  gci_no_mrl_constraint_flag: %u\n", gci->gci_no_mrl_constraint_flag);
        printf("  gci_no_mip_constraint_flag: %u\n", gci->gci_no_mip_constraint_flag);
        printf("  gci_no_cclm_constraint_flag: %u\n", gci->gci_no_cclm_constraint_flag);
        /* inter */
        printf("  gci_no_ref_pic_resampling_constraint_flag: %u\n", gci->gci_no_ref_pic_resampling_constraint_flag);
        printf("  gci_no_res_change_in_clvs_constraint_flag: %u\n", gci->gci_no_res_change_in_clvs_constraint_flag);
        printf("  gci_no_weighted_prediction_constraint_flag: %u\n", gci->gci_no_weighted_prediction_constraint_flag);
        printf("  gci_no_ref_wraparound_constraint_flag: %u\n", gci->gci_no_ref_wraparound_constraint_flag);
        printf("  gci_no_temporal_mvp_constraint_flag: %u\n", gci->gci_no_temporal_mvp_constraint_flag);
        printf("  gci_no_sbtmvp_constraint_flag: %u\n", gci->gci_no_sbtmvp_constraint_flag);
        printf("  gci_no_amvr_constraint_flag: %u\n", gci->gci_no_amvr_constraint_flag);
        printf("  gci_no_bdof_constraint_flag: %u\n", gci->gci_no_bdof_constraint_flag);
        printf("  gci_no_smvd_constraint_flag: %u\n", gci->gci_no_smvd_constraint_flag);
        printf("  gci_no_dmvr_constraint_flag: %u\n", gci->gci_no_dmvr_constraint_flag);
        printf("  gci_no_mmvd_constraint_flag: %u\n", gci->gci_no_mmvd_constraint_flag);
        printf("  gci_no_affine_motion_constraint_flag: %u\n", gci->gci_no_affine_motion_constraint_flag);
        printf("  gci_no_prof_constraint_flag: %u\n", gci->gci_no_prof_constraint_flag);
        printf("  gci_no_bcw_constraint_flag: %u\n", gci->gci_no_bcw_constraint_flag);
        printf("  gci_no_ciip_constraint_flag: %u\n", gci->gci_no_ciip_constraint_flag);
        printf("  gci_no_gpm_constraint_flag: %u\n", gci->gci_no_gpm_constraint_flag);
        /* transform, quantization, residual */
        printf("  gci_no_luma_transform_size_64_constraint_flag: %u\n", gci->gci_no_luma_transform_size_64_constraint_flag);
        printf("  gci_no_transform_skip_constraint_flag: %u\n", gci->gci_no_transform_skip_constraint_flag);
        printf("  gci_no_bdpcm_constraint_flag: %u\n", gci->gci_no_bdpcm_constraint_flag);
        printf("  gci_no_mts_constraint_flag: %u\n", gci->gci_no_mts_constraint_flag);
        printf("  gci_no_lfnst_constraint_flag: %u\n", gci->gci_no_lfnst_constraint_flag);
        printf("  gci_no_joint_cbcr_constraint_flag: %u\n", gci->gci_no_joint_cbcr_constraint_flag);
        printf("  gci_no_sbt_constraint_flag: %u\n", gci->gci_no_sbt_constraint_flag);
        printf("  gci_no_act_constraint_flag: %u\n", gci->gci_no_act_constraint_flag);
        printf("  gci_no_explicit_scaling_list_constraint_flag: %u\n", gci->gci_no_explicit_scaling_list_constraint_flag);
        printf("  gci_no_dep_quant_constraint_flag: %u\n", gci->gci_no_dep_quant_constraint_flag);
        printf("  gci_no_sign_data_hiding_constraint_flag: %u\n", gci->gci_no_sign_data_hiding_constraint_flag);
        printf("  gci_no_cu_qp_delta_constraint_flag: %u\n", gci->gci_no_cu_qp_delta_constraint_flag);
        printf("  gci_no_chroma_qp_offset_constraint_flag: %u\n", gci->gci_no_chroma_qp_offset_constraint_flag);
        /* loop filter */
        printf("  gci_no_sao_constraint_flag: %u\n", gci->gci_no_sao_constraint_flag);
        printf("  gci_no_alf_constraint_flag: %u\n", gci->gci_no_alf_constraint_flag);
        printf("  gci_no_ccalf_constraint_flag: %u\n", gci->gci_no_ccalf_constraint_flag);
        printf("  gci_no_lmcs_constraint_flag: %u\n", gci->gci_no_lmcs_constraint_flag);
        printf("  gci_no_ladf_constraint_flag: %u\n", gci->gci_no_ladf_constraint_flag);
        printf("  gci_no_virtual_boundaries_constraint_flag: %u\n", gci->gci_no_virtual_boundaries_constraint_flag);
        printf("  gci_num_additional_bits: %u\n", gci->gci_num_additional_bits);
        size_t numAdditionalBitsUsed;
        if (gci->gci_num_additional_bits > 5) {
            printf("  gci_all_rap_pictures_constraint_flag: %u\n", gci->gci_all_rap_pictures_constraint_flag);
            printf("  gci_no_extended_precision_processing_constraint_flag: %u\n", gci->gci_no_extended_precision_processing_constraint_flag);
            printf("  gci_no_ts_residual_coding_rice_constraint_flag: %u\n", gci->gci_no_ts_residual_coding_rice_constraint_flag);
            printf("  gci_no_rrc_rice_extension_constraint_flag: %u\n", gci->gci_no_rrc_rice_extension_constraint_flag);
            printf("  gci_no_persistent_rice_adaptation_constraint_flag: %u\n", gci->gci_no_persistent_rice_adaptation_constraint_flag);
            printf("  gci_no_reverse_last_sig_coeff_constraint_flag: %u\n", gci->gci_no_reverse_last_sig_coeff_constraint_flag);
            numAdditionalBitsUsed = 6;
        } else {
            numAdditionalBitsUsed = 0;
        }
        printf("  gci_reserved_bit: {");
        for (size_t i = 0; i < gci->gci_num_additional_bits - numAdditionalBitsUsed; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", gci->gci_reserved_bit[i]);
        }
        printf("\n");
    }
}