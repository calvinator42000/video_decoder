#ifndef GCI_H
#define GCI_H

#include <utils.h>

// 7.4.4.2 General constraints information semantics - Rec. ITU-T H.266 (V3) (09/2023)
#define gci_alignment_zero_bit 0

// 7.3.3.2 General constraints information syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {
    uint_t gci_present_flag;
    /* general */
    uint_t gci_intra_only_constraint_flag;
    uint_t gci_all_layers_independent_constraint_flag;
    uint_t gci_one_au_only_constraint_flag;
    /* picture format */
    uint_t gci_sixteen_minus_max_bitdepth_constraint_idc;
    uint_t gci_three_minus_max_chroma_format_constraint_idc;
    /* NAL unit type related */
    uint_t gci_no_mixed_nalu_types_in_pic_constraint_flag;
    uint_t gci_no_trail_constraint_flag;
    uint_t gci_no_stsa_constraint_flag;
    uint_t gci_no_rasl_constraint_flag;
    uint_t gci_no_radl_constraint_flag;
    uint_t gci_no_idr_constraint_flag;
    uint_t gci_no_cra_constraint_flag;
    uint_t gci_no_gdr_constraint_flag;
    uint_t gci_no_aps_constraint_flag;
    uint_t gci_no_idr_rpl_constraint_flag;
    /* tile, slice, subpicture partitioning */
    uint_t gci_one_tile_per_pic_constraint_flag;
    uint_t gci_pic_header_in_slice_header_constraint_flag;
    uint_t gci_one_slice_per_pic_constraint_flag;
    uint_t gci_no_rectangular_slice_constraint_flag;
    uint_t gci_one_slice_per_subpic_constraint_flag;
    uint_t gci_no_subpic_info_constraint_flag;
    /* CTU and block partitioning */
    uint_t gci_three_minus_max_log2_ctu_size_constraint_idc;
    uint_t gci_no_partition_constraints_override_constraint_flag;
    uint_t gci_no_mtt_constraint_flag;
    uint_t gci_no_qtbtt_dual_tree_intra_constraint_flag;
    /* intra */
    uint_t gci_no_palette_constraint_flag;
    uint_t gci_no_ibc_constraint_flag;
    uint_t gci_no_isp_constraint_flag;
    uint_t gci_no_mrl_constraint_flag;
    uint_t gci_no_mip_constraint_flag;
    uint_t gci_no_cclm_constraint_flag;
    /* inter */
    uint_t gci_no_ref_pic_resampling_constraint_flag;
    uint_t gci_no_res_change_in_clvs_constraint_flag;
    uint_t gci_no_weighted_prediction_constraint_flag;
    uint_t gci_no_ref_wraparound_constraint_flag;
    uint_t gci_no_temporal_mvp_constraint_flag;
    uint_t gci_no_sbtmvp_constraint_flag;
    uint_t gci_no_amvr_constraint_flag;
    uint_t gci_no_bdof_constraint_flag;
    uint_t gci_no_smvd_constraint_flag;
    uint_t gci_no_dmvr_constraint_flag;
    uint_t gci_no_mmvd_constraint_flag;
    uint_t gci_no_affine_motion_constraint_flag;
    uint_t gci_no_prof_constraint_flag;
    uint_t gci_no_bcw_constraint_flag;
    uint_t gci_no_ciip_constraint_flag;
    uint_t gci_no_gpm_constraint_flag;
    /* transform, quantization, residual */
    uint_t gci_no_luma_transform_size_64_constraint_flag;
    uint_t gci_no_transform_skip_constraint_flag;
    uint_t gci_no_bdpcm_constraint_flag;
    uint_t gci_no_mts_constraint_flag;
    uint_t gci_no_lfnst_constraint_flag;
    uint_t gci_no_joint_cbcr_constraint_flag;
    uint_t gci_no_sbt_constraint_flag;
    uint_t gci_no_act_constraint_flag;
    uint_t gci_no_explicit_scaling_list_constraint_flag;
    uint_t gci_no_dep_quant_constraint_flag;
    uint_t gci_no_sign_data_hiding_constraint_flag;
    uint_t gci_no_cu_qp_delta_constraint_flag;
    uint_t gci_no_chroma_qp_offset_constraint_flag;
    /* loop filter */
    uint_t gci_no_sao_constraint_flag;
    uint_t gci_no_alf_constraint_flag;
    uint_t gci_no_ccalf_constraint_flag;
    uint_t gci_no_lmcs_constraint_flag;
    uint_t gci_no_ladf_constraint_flag;
    uint_t gci_no_virtual_boundaries_constraint_flag;
    uint_t gci_num_additional_bits;
    uint_t gci_all_rap_pictures_constraint_flag;
    uint_t gci_no_extended_precision_processing_constraint_flag;
    uint_t gci_no_ts_residual_coding_rice_constraint_flag;
    uint_t gci_no_rrc_rice_extension_constraint_flag;
    uint_t gci_no_persistent_rice_adaptation_constraint_flag;
    uint_t gci_no_reverse_last_sig_coeff_constraint_flag;
    uint_t* gci_reserved_bit;
} General_Constraints_Info;

General_Constraints_Info* general_constraints_info();

General_Constraints_Info* initGCI();
void freeGCI(General_Constraints_Info* gci);
void printGCI(General_Constraints_Info* gci);

#endif // GCI_H