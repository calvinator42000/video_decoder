#include <nuh.h>

NAL_Unit_Header* nal_unit_header()
{
    NAL_Unit_Header* nuh = malloc(sizeof(NAL_Unit_Header));
    f(1, 0); // forbidden_zero_bit
    f(1, 0); // nuh_reserved_zero_bit
    nuh->nuh_layer_id = u(6);
    nuh->nal_unit_type = u(5);
    nuh->nuh_temporal_id_plus1 = u(3);
    return nuh;
}

void freeNUH(NAL_Unit_Header* nuh) {
    if (nuh) {
        free(nuh);
    }
}