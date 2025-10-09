#ifndef VUI_PAY_H
#define VUI_PAY_H

#include <utils.h>
#include <vui_par.h>

// 7.4.3.21 VUI payload semantics - Rec. ITU-T H.266 (V3) (09/2023)
#define vui_payload_bit_equal_to_one 1
#define vui_payload_bit_equal_to_zero 0

// 7.3.2.21 VUI payload syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {
    VUI_Parameters* vui_par;
    uint_t vui_reserved_payload_extension_data;
} VUI_Payload;

VUI_Payload* vui_payload(uint_t payloadSize, uint_t ChromaFormatIdc);

VUI_Payload* initVUI_Pay();
void freeVUI_Pay(VUI_Payload* vui_pay);
void printVUI_Pay(VUI_Payload* vui_pay);


#endif // VUI_PAY_H