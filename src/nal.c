#include <nal.h>

void nal_unit(size_t NumBytesInNalUnit) {
    // nal_unit_header()
    size_t NumBytesInRbsp = 0;
    NAL_Unit* nal_unit = malloc(sizeof(NAL_Unit));
    nal_unit->rbsp_byte = malloc(sizeof(uint8_t) * NumBytesInNalUnit);
    for (size_t i = 0; i < NumBytesInNalUnit; i++) {
        if (i + 2 < NumBytesInNalUnit && next_bits(24) == 0x000003) {
            nal_unit->rbsp_byte[NumBytesInRbsp++] = b();
            nal_unit->rbsp_byte[NumBytesInRbsp++] = b();
            i += 2;
            f(8, emulation_prevention_three_byte);
        } else {
            nal_unit->rbsp_byte[NumBytesInRbsp++] = b();
        }
    }
}