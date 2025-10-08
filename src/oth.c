#include <oth.h>

OLS_Timing_and_HRD_Parameters* ols_timing_hrd_parameters(uint_t firstSubLayer, uint_t MaxSubLayersVal, General_Timing_Hrd_Parameters* gth) {
    OLS_Timing_and_HRD_Parameters* oth = initOTH();

    oth->firstSubLayer = firstSubLayer;
    oth->MaxSubLayersVal = MaxSubLayersVal;
    oth->gth = gth;

    oth->fixed_pic_rate_general_flag = malloc(sizeof(uint_t) * (MaxSubLayersVal + 1));
    oth->fixed_pic_rate_within_cvs_flag = malloc(sizeof(uint_t) * (MaxSubLayersVal + 1));
    oth->elemental_duration_in_tc_minus1 = malloc(sizeof(uint_t) * (MaxSubLayersVal + 1));
    oth->low_delay_hrd_flag = malloc(sizeof(uint_t) * (MaxSubLayersVal + 1));
    oth->slh = malloc(sizeof(Sublayer_HRD_Parameters*) * (MaxSubLayersVal + 1));
    for (uint_t i = firstSubLayer; i <= MaxSubLayersVal; i++) {
        oth->fixed_pic_rate_general_flag[i] = u(1);
        if (!oth->fixed_pic_rate_general_flag[i]) {
            oth->fixed_pic_rate_within_cvs_flag[i] = u(1);
        } else {
            oth->fixed_pic_rate_within_cvs_flag[i] = 1;
        }
        if (oth->fixed_pic_rate_within_cvs_flag[i]) {
            oth->elemental_duration_in_tc_minus1[i] = ue();
        } else if ((gth->general_nal_hrd_params_present_flag || gth->general_vcl_hrd_params_present_flag) && gth->hrd_cpb_cnt_minus1 == 0) {
            oth->low_delay_hrd_flag[i] = u(1);
        }
        if (gth->general_nal_hrd_params_present_flag) {
            oth->slh[i] = sublayer_hrd_parameters(gth);
        }
        if (gth->general_vcl_hrd_params_present_flag) {
            oth->slh[i] = sublayer_hrd_parameters(gth);
        }
        if (!(gth->general_nal_hrd_params_present_flag || gth->general_vcl_hrd_params_present_flag)) {
            oth->slh[i] = NULL;
        }
    }

    return oth;
}

OLS_Timing_and_HRD_Parameters* initOTH() {
    OLS_Timing_and_HRD_Parameters* oth = malloc(sizeof(OLS_Timing_and_HRD_Parameters));
    if (oth == NULL) {
        printf("Memory allocation failed: OLS_Timing_and_HRD_Parameters\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    oth->fixed_pic_rate_general_flag = NULL;
    oth->fixed_pic_rate_within_cvs_flag = NULL;
    oth->elemental_duration_in_tc_minus1 = NULL;
    oth->low_delay_hrd_flag = NULL;
    oth->slh = NULL;
    return oth;
}

void freeOTH(OLS_Timing_and_HRD_Parameters* oth) {
    if (oth) {
        if (oth->fixed_pic_rate_general_flag) {
            free(oth->fixed_pic_rate_general_flag);
        }
        if (oth->fixed_pic_rate_within_cvs_flag) {
            free(oth->fixed_pic_rate_within_cvs_flag);
        }
        if (oth->elemental_duration_in_tc_minus1) {
            free(oth->elemental_duration_in_tc_minus1);
        }
        if (oth->low_delay_hrd_flag) {
            free(oth->low_delay_hrd_flag);
        }
        if (oth->slh) {
            for (uint_t i = oth->firstSubLayer; i <= oth->MaxSubLayersVal; i++) {
                if (oth->slh[i]) {
                    freeSLH(oth->slh[i]);
                }
            }
            free(oth->slh);
        }
        free(oth);
    }
}

void printOTH(OLS_Timing_and_HRD_Parameters* oth) {
    incIndent();
    if (oth == NULL) {
        printVar("OLS Timing and HRD Parameters is NULL\n");
        return;
    }
    printVar("OLS Timing and HRD Parameters:\n");
    printVar("  fixed_pic_rate_general_flag: {");
    for (uint_t i = oth->firstSubLayer; i <= oth->MaxSubLayersVal; i++) {
        if (i == oth->firstSubLayer) {
            printf(",");
        }
        printf("%u", oth->fixed_pic_rate_general_flag[i]);
    }
    printf("}\n");
    printVar("  fixed_pic_rate_within_cvs_flag: {");
    for (uint_t i = oth->firstSubLayer; i <= oth->MaxSubLayersVal; i++) {
        if (i == oth->firstSubLayer) {
            printf(",");
        }
        printf("%u", oth->fixed_pic_rate_within_cvs_flag[i]);
    }
    printf("}\n");
    printVar("  elemental_duration_in_tc_minus1: {");
    for (uint_t i = oth->firstSubLayer; i <= oth->MaxSubLayersVal; i++) {
        if (i == oth->firstSubLayer) {
            printf(",");
        }
        if (oth->fixed_pic_rate_within_cvs_flag[i]) {
            printf("%u", oth->elemental_duration_in_tc_minus1[i]);
        } else {
            printf("x");
        }
    }
    printf("}\n");
    printVar("  low_delay_hrd_flag: {");
    for (uint_t i = oth->firstSubLayer; i <= oth->MaxSubLayersVal; i++) {
        if (i == oth->firstSubLayer) {
            printf(",");
        }
        if (!oth->fixed_pic_rate_within_cvs_flag[i] && (oth->gth->general_nal_hrd_params_present_flag || oth->gth->general_vcl_hrd_params_present_flag) && oth->gth->hrd_cpb_cnt_minus1 == 0)
        printf("%u", oth->low_delay_hrd_flag[i]);
    }
    printf("}\n");
    for (uint_t i = oth->firstSubLayer; i <= oth->MaxSubLayersVal; i++) {
        if (oth->gth->general_nal_hrd_params_present_flag || oth->gth->general_vcl_hrd_params_present_flag) {
            printSLH(oth->slh[i]);
        }
    }
    decIndent();
}