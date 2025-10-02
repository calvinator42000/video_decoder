#include <nal.h>

NAL_Unit* nal_unit(size_t NumBytesInNalUnit) {
    NAL_Unit* nal_unit = malloc(sizeof(NAL_Unit));
    nal_unit->nuh = nal_unit_header();
    nal_unit->NumBytesInRbsp = 0;
    nal_unit->rbsp_byte = malloc(sizeof(uint8_t) * NumBytesInNalUnit - 2);
    for (size_t i = 2; i < NumBytesInNalUnit; i++) {
        if (i + 2 < NumBytesInNalUnit && next_bits(24) == 0x000003) {
            nal_unit->rbsp_byte[nal_unit->NumBytesInRbsp++] = b();
            nal_unit->rbsp_byte[nal_unit->NumBytesInRbsp++] = b();
            i += 2;
            f(8, emulation_prevention_three_byte);
        } else {
            nal_unit->rbsp_byte[nal_unit->NumBytesInRbsp++] = b();
        }
    }
    nal_unit->rbsp_byte = realloc(nal_unit->rbsp_byte, sizeof(uint8_t) * nal_unit->NumBytesInRbsp);
    return nal_unit;
}

void freeNALUnit(NAL_Unit* nal_unit) {
    if (nal_unit) {
        freeNUH(nal_unit->nuh);
        if (nal_unit->rbsp_byte) {
            free(nal_unit->rbsp_byte);
        }
        free(nal_unit);
    }
}

void printNALUnit(NAL_Unit* nal_unit) {
    if (nal_unit == NULL) {
        printf("NAL Unit is NULL\n");
        return;
    }
    printNUH(nal_unit->nuh);
    printf("RBSP Bytes (%zu bytes):\n", nal_unit->rbsp_byte ? nal_unit->NumBytesInRbsp : 0);
    if (nal_unit->rbsp_byte) {
        for (size_t i = 0; i < nal_unit->NumBytesInRbsp; i++) {
            printf("%02X ", nal_unit->rbsp_byte[i]);
            if ((i + 1) % 16 == 0) {
                printf("\n");
            }
        }
        if (nal_unit->NumBytesInRbsp % 16 != 0) {
            printf("\n");
        }
    }
}

void rbsp_trailing_bits() {
    f(1, 1); // rbsp_stop_one_bit
    while (!byte_aligned()) {
        f(1, 0); // rbsp_alignment_zero_bit
    }
}