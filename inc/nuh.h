#ifndef NUH_H
#define NUH_H

#include <utils.h>

// 7.3.1.2 - NAL unit header syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {
    uint_t nuh_layer_id;
    uint_t nal_unit_type;
    uint_t nuh_temporal_id_plus1;
} NAL_Unit_Header;

NAL_Unit_Header* nal_unit_header();

void freeNUH(NAL_Unit_Header* nuh);

#endif // NUH_H