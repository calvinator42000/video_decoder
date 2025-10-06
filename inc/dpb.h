#ifndef DPB_H
#define DPB_Hs

#include <utils.h>

// 7.3.4 DPB parameters syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {
    // Parameters
    uint_t MaxSubLayersMinus1;
    uint_t subLayerInfoFlag;

    uint_t* dpb_max_dec_pic_buffering_minus1;
    uint_t* dpb_max_num_reorder_pics;
    uint_t* dpb_max_latency_increase_plus1;
} DPB_Parameters;

DPB_Parameters* dpb_parameters(uint_t MaxSubLayersMinus1, uint_t subLayerInfoFlag);

DPB_Parameters* initDPB();
void freeDPB(DPB_Parameters* dpb);
void printDPB(DPB_Parameters* dpb);

#endif // DPB_H