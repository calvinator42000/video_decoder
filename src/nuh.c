#include <nuh.h>

NAL_Unit_Header* nal_unit_header()
{
    NAL_Unit_Header* nuh = malloc(sizeof(NAL_Unit_Header));
    f(1, forbidden_zero_bit);
    f(1, nuh_reserved_zero_bit);
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

void printNUH(NAL_Unit_Header* nuh) {
    if (nuh == NULL) {
        printf("NAL Unit Header is NULL\n");
        return;
    }
    printf("NAL Unit Header:\n");
    printf("  nuh_layer_id: %u\n", nuh->nuh_layer_id);
    printf("  nal_unit_type: %u\n", nuh->nal_unit_type);
    printf("  nuh_temporal_id_plus1: %u\n", nuh->nuh_temporal_id_plus1);
}