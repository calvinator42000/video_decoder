#include <slh.h>

Sublayer_HRD_Parameters* sublayer_hrd_parameters(General_Timing_Hrd_Parameters* gth) {
    Sublayer_HRD_Parameters* slh = initSLH();
    slh->bit_rate_value_minus1 = malloc(sizeof(uint_t) * gth->hrd_cpb_cnt_minus1);
    slh->cpb_size_value_minus1 = malloc(sizeof(uint_t) * gth->hrd_cpb_cnt_minus1);
    slh->cpb_size_du_value_minus1 = malloc(sizeof(uint_t) * gth->hrd_cpb_cnt_minus1);
    slh->bit_rate_du_value_minus1 = malloc(sizeof(uint_t) * gth->hrd_cpb_cnt_minus1);
    slh->cbr_flag = malloc(sizeof(uint_t) * gth->hrd_cpb_cnt_minus1);
    for (int j = 0; j <= gth->hrd_cpb_cnt_minus1; j++) {
        slh->bit_rate_value_minus1[j] = ue();
        slh->cpb_size_value_minus1[j] = ue();
        if (gth->general_du_hrd_params_present_flag) {
            slh->cpb_size_du_value_minus1[j] = ue();
            slh->bit_rate_du_value_minus1[j] = ue();
        }
        slh->cbr_flag[j] = u(1);
    }
    return slh;
}

Sublayer_HRD_Parameters* initSLH() {
    Sublayer_HRD_Parameters* slh = malloc(sizeof(Sublayer_HRD_Parameters));
    if (slh == NULL) {
        printf("Memory allocation failed: Sublayer_HRD_Parameters\n");
        freeContext();
        exit(EXIT_FAILURE);
    }

    slh->bit_rate_value_minus1 = NULL;
    slh->cpb_size_value_minus1 = NULL;
    slh->cpb_size_du_value_minus1 = NULL;
    slh->bit_rate_du_value_minus1 = NULL;
    slh->cbr_flag = NULL;
    return slh;
}

void freeSLH(Sublayer_HRD_Parameters* slh) {
    if (slh) {
        if (slh->bit_rate_value_minus1) {
            free(slh->bit_rate_value_minus1);
        }
        if (slh->cpb_size_value_minus1) {
            free(slh->cpb_size_value_minus1);
        }
        if (slh->cpb_size_du_value_minus1) {
            free(slh->cpb_size_du_value_minus1);
        }
        if (slh->bit_rate_du_value_minus1) {
            free(slh->bit_rate_du_value_minus1);
        }
        if (slh->cbr_flag) {
            free(slh->cbr_flag);
        }
        free(slh);
    }
}

void printSLH(Sublayer_HRD_Parameters* slh) {
    incIndent();
    if (slh == NULL) {
        printVar("Sublayer HRD Paramters is NULL\n");
        return;
    }
    printVar("Sublayer HRD Paramters:\n");
    printVar("  bit_rate_value_minus1: {");
    for (int j = 0; j <= slh->gth->hrd_cpb_cnt_minus1; j++) {
        if (j == 0) {
            printf(",");
        }
        printf("%u", slh->bit_rate_value_minus1[j]);
    }
    printf("}\n");
    printVar("  cpb_size_value_minus1: {");
    for (int j = 0; j <= slh->gth->hrd_cpb_cnt_minus1; j++) {
        if (j == 0) {
            printf(",");
        }
        printf("%u", slh->cpb_size_value_minus1[j]);
    }
    printf("}\n");
    printVar("  cpb_size_du_value_minus1: {");
    for (int j = 0; j <= slh->gth->hrd_cpb_cnt_minus1; j++) {
        if (j == 0) {
            printf(",");
        }
        printf("%u", slh->cpb_size_du_value_minus1[j]);
    }
    printf("}\n");
    printVar("  bit_rate_du_value_minus1: {");
    for (int j = 0; j <= slh->gth->hrd_cpb_cnt_minus1; j++) {
        if (j == 0) {
            printf(",");
        }
        printf("%u", slh->bit_rate_du_value_minus1[j]);
    }
    printf("}\n");
    printVar("  cbr_flag: {");
    for (int j = 0; j <= slh->gth->hrd_cpb_cnt_minus1; j++) {
        if (j == 0) {
            printf(",");
        }
        printf("%u", slh->cbr_flag[j]);
    }
    printf("}\n");
    decIndent();
}