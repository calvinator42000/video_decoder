#include <sps.h>

Sequence_Parameter_Set* seq_parameter_set_rbsp() {
    Sequence_Parameter_Set* sps = malloc(sizeof(Sequence_Parameter_Set));

    sps->sps_seq_parameter_set_id = u(4);
    sps->sps_video_parameter_set_id = u(4);
    sps->sps_max_sublayers_minus1 = u(3);
    sps->sps_chroma_format_idc = u(2);
    sps->sps_log2_ctu_size_minus5 = u(2);
    sps->sps_ptl_dpb_hrd_params_present_flag = u(1);
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        // profile_tier_level
    }

    return sps;
}

void freeSPS(Sequence_Parameter_Set* sps) {
    if (sps) {
        free(sps);
    }
}

void printSPS(Sequence_Parameter_Set* sps) {
    if (sps == NULL) {
        printf("Sequence Parameter Set is NULL\n");
        return;
    }
    printf("Sequence Parameter Set:\n");
}