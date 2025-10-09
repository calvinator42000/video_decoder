#ifndef SLH_H
#define SLH_H

#include <utils.h>
#include <gth.h>

struct General_Timing_Hrd_Parameters_struct;
typedef struct General_Timing_Hrd_Parameters_struct General_Timing_Hrd_Parameters;

// 7.3.5.3 Sublayer HRD parameters syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {
    // Parameters
    General_Timing_Hrd_Parameters* gth;

    uint_t* bit_rate_value_minus1;
    uint_t* cpb_size_value_minus1;
    uint_t* cpb_size_du_value_minus1;
    uint_t* bit_rate_du_value_minus1;
    uint_t* cbr_flag;
} Sublayer_HRD_Parameters;

Sublayer_HRD_Parameters* sublayer_hrd_parameters(General_Timing_Hrd_Parameters* gth);

Sublayer_HRD_Parameters* initSLH();
void freeSLH(Sublayer_HRD_Parameters* slh);
void printSLH(Sublayer_HRD_Parameters* slh);

#endif // SLH_H