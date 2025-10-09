#include <vui_pay.h>

VUI_Payload* vui_payload(uint_t payloadSize, uint_t ChromaFormatIdc) {
    VUI_Payload* vui_pay = initVUI_Pay();
    uint_t VuiExtensionBitsPresentFlag = 0;
    Data_Buffer* buffer = getDataBuffer();
    size_t initOffset = buffer->bit_offset;
    vui_pay->vui_par = vui_parameters(payloadSize, ChromaFormatIdc);
    if (VuiExtensionBitsPresentFlag || more_data_in_payload(initOffset, payloadSize)) {
        if (payload_extension_present(initOffset, payloadSize)) {
            uint_t nEarlierBits = buffer->bit_offset - initOffset;
            // Little hack to get the number of vui_paylaod_bit_equal_to_zero bits
            size_t lastByte = readBytes((initOffset / 8) + payloadSize, 1);
            uint_t numShifts = 0;
            while (lastByte != 0) {
                lastByte <<= lastByte;
                numShifts++;
            }
            uint_t nPayloadZeroBits = 8 - numShifts;

            vui_pay->vui_reserved_payload_extension_data = u(8 * payloadSize - nEarlierBits - nPayloadZeroBits - 1);
        }
        f(1, vui_payload_bit_equal_to_one);
        while (!byte_aligned()) {
            f(1, vui_payload_bit_equal_to_zero);
        }
    }
    return vui_pay;
}

VUI_Payload* initVUI_Pay() {
    VUI_Payload* vui_pay = malloc(sizeof(VUI_Payload));
    if (vui_pay == NULL) {
        printf("Memory allocation failed: VUI_Payload\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    vui_pay->vui_par = NULL;
    return vui_pay;
}

void freeVUI_Pay(VUI_Payload* vui_pay) {
    if (vui_pay) {
        if (vui_pay->vui_par) {
            freeVUI_Par(vui_pay->vui_par);
        }
        free(vui_pay);
    }
}

void printVUI_Pay(VUI_Payload* vui_pay) {
    incIndent();
    if (vui_pay == NULL) {
        printVar("VUI Payload is NULL\n");
        return;
    }
    printVar("VUI Payload:\n");
    printVUI_Par(vui_pay->vui_par);
    printVar("  vui_reserved_payload_extension_data: %u\n", vui_pay->vui_reserved_payload_extension_data);
    decIndent();
}
