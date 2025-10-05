#include <sps.h>

Sequence_Parameter_Set* seq_parameter_set_rbsp() {
    Sequence_Parameter_Set* sps = initSPS();
    sps->sps_seq_parameter_set_id = u(4);
    sps->sps_video_parameter_set_id = u(4);
    sps->sps_max_sublayers_minus1 = u(3);
    sps->sps_chroma_format_idc = u(2);
    sps->sps_log2_ctu_size_minus5 = u(2);
    sps->sps_ptl_dpb_hrd_params_present_flag = u(1);
    if (sps->sps_ptl_dpb_hrd_params_present_flag) {
        sps->ptl = profile_tier_level(1, sps->sps_max_sublayers_minus1);
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
    return sps;
}

void freeSPS(Sequence_Parameter_Set* sps) {
    if (sps) {
        if (sps->ptl) {
            freePTL(sps->ptl);
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
    decIndent();
}