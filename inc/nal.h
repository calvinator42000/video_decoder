#ifndef NAL_H
#define NAL_H

#include <utils.h>

typedef struct {
    // nal_unit_header()
    uint8_t* rbsp_byte;
} NAL_Unit;

// 7.3.1.1 General NAL unit syntax - Rec. ITU-T H.266 (V3) (09/2023)
void nal_unit(size_t NumBytesInNalUnit);

// 7.4.2.1 General NAL unit semantics - Rec. ITU-T H.266 (V3) (09/2023)
#define emulation_prevention_three_byte 0x03

#endif // NAL_H