#ifndef NAL_H
#define NAL_H

#include <utils.h>
#include <nuh.h>

// 7.3.1.1 General NAL unit syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {
    NAL_Unit_Header* nuh;
    uint8_t* rbsp_byte;
    size_t NumBytesInRbsp;
} NAL_Unit;

NAL_Unit* nal_unit(size_t NumBytesInNalUnit);

void freeNALUnit(NAL_Unit* nal_unit);
void printNALUnit(NAL_Unit* nal_unit);

// 7.4.2.1 General NAL unit semantics - Rec. ITU-T H.266 (V3) (09/2023)
#define emulation_prevention_three_byte 0x03

#endif // NAL_H