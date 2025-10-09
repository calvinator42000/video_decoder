#ifndef GTH_H
#define GTH_H

#include <utils.h>

// 7.3.5.1 General timing and HRD parameters syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct General_Timing_Hrd_Parameters_struct {
    uint_t num_units_in_tick;
    uint_t time_scale;
    uint_t general_nal_hrd_params_present_flag;
    uint_t general_vcl_hrd_params_present_flag;
    uint_t general_same_pic_timing_in_all_ols_flag;
    uint_t general_du_hrd_params_present_flag;
    uint_t tick_divisor_minus2;
    uint_t bit_rate_scale;
    uint_t cpb_size_scale;
    uint_t cpb_size_du_scale;
    uint_t hrd_cpb_cnt_minus1;
} General_Timing_Hrd_Parameters;

General_Timing_Hrd_Parameters* general_timing_hrd_parameters();

General_Timing_Hrd_Parameters* initGTH();
void freeGTH(General_Timing_Hrd_Parameters* gth);
void printGTH(General_Timing_Hrd_Parameters* gth);

#endif // GTH_H