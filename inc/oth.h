#ifndef OTH_H
#define OTH_H

#include <utils.h>
#include <gth.h>
#include <slh.h>

struct General_Timing_Hrd_Parameters_struct;
typedef struct General_Timing_Hrd_Parameters_struct General_Timing_Hrd_Parameters;

// 7.3.5.2 OLS timing and HRD parameters syntax
typedef struct OLS_Timing_and_HRD_Parameters_struct {
    // Parameters
    uint_t firstSubLayer;
    uint_t MaxSubLayersVal;
    General_Timing_Hrd_Parameters* gth;

    uint_t* fixed_pic_rate_general_flag;
    uint_t* fixed_pic_rate_within_cvs_flag;
    uint_t* elemental_duration_in_tc_minus1;
    uint_t* low_delay_hrd_flag;
    Sublayer_HRD_Parameters** slh;
} OLS_Timing_and_HRD_Parameters;

OLS_Timing_and_HRD_Parameters* ols_timing_hrd_parameters(uint_t firstSubLayer, uint_t MaxSubLayersVal, General_Timing_Hrd_Parameters* gth);

OLS_Timing_and_HRD_Parameters* initOTH();
void freeOTH(OLS_Timing_and_HRD_Parameters* oth);
void printOTH(OLS_Timing_and_HRD_Parameters* oth);

#endif // OTH_H