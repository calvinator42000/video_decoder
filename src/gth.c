#include <gth.h>

General_Timing_Hrd_Parameters* general_timing_hrd_parameters() {
    General_Timing_Hrd_Parameters* gth = initGTH();
    gth->num_units_in_tick = u(32);
    gth->time_scale = u(32);
    gth->general_nal_hrd_params_present_flag = u(1);
    gth->general_vcl_hrd_params_present_flag = u(1);
    if (gth->general_nal_hrd_params_present_flag || gth->general_vcl_hrd_params_present_flag) {
        gth->general_same_pic_timing_in_all_ols_flag = u(1);
        gth->general_du_hrd_params_present_flag = u(1);
        if (gth->general_du_hrd_params_present_flag) {
            gth->tick_divisor_minus2 = u(8);
        }
        gth->bit_rate_scale = u(4);
        gth->cpb_size_scale = u(4);
        if (gth->general_du_hrd_params_present_flag) {
            gth->cpb_size_du_scale = u(4);
        }
        gth->hrd_cpb_cnt_minus1 = ue();
    } else {
        gth->general_du_hrd_params_present_flag = 0;
    }
    return gth;
}

General_Timing_Hrd_Parameters* initGTH() {
    General_Timing_Hrd_Parameters* gth = malloc(sizeof(General_Timing_Hrd_Parameters));
    if (gth == NULL) {
        printf("Memory allocation failed: General_Timing_Hrd_Parameters\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    return gth;
}

void freeGTH(General_Timing_Hrd_Parameters* gth) {
    if (gth) {
        free(gth);
    }
}

void printGTH(General_Timing_Hrd_Parameters* gth) {
    incIndent();
    if (gth == NULL) {
        printVar("General Timing Hrd Parameters is NULL\n");
        return;
    }
    printVar("General Timing Hrd Parameters:\n");
    printVar("  num_units_in_tick: %u\n", gth->num_units_in_tick);
    printVar("  time_scale: %u\n", gth->time_scale);
    printVar("  general_nal_hrd_params_present_flag: %u\n", gth->general_nal_hrd_params_present_flag);
    printVar("  general_vcl_hrd_params_present_flag: %u\n", gth->general_vcl_hrd_params_present_flag);
    if (gth->general_nal_hrd_params_present_flag || gth->general_vcl_hrd_params_present_flag) {
        printVar("  general_same_pic_timing_in_all_ols_flag: %u\n", gth->general_same_pic_timing_in_all_ols_flag);
    }
    printVar("  general_du_hrd_params_present_flag: %u\n", gth->general_du_hrd_params_present_flag);
    if (gth->general_nal_hrd_params_present_flag || gth->general_vcl_hrd_params_present_flag) {
        printVar("  general_same_pic_timing_in_all_ols_flag: %u\n", gth->general_same_pic_timing_in_all_ols_flag);
        if (gth->general_du_hrd_params_present_flag) {
            printVar("  tick_divisor_minus2: %u\n", gth->tick_divisor_minus2);
        }
        printVar("  bit_rate_scale: %u\n", gth->bit_rate_scale);
        printVar("  cpb_size_scale: %u\n", gth->cpb_size_scale);
        if (gth->general_du_hrd_params_present_flag) {
            printVar("  cpb_size_du_scale: %u\n", gth->cpb_size_du_scale);
        }
        printVar("  hrd_cpb_cnt_minus1: %u\n", gth->hrd_cpb_cnt_minus1);
    }
    decIndent();
}