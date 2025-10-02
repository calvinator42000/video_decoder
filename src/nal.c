#include <nal.h>

extern Context* ctx;

NAL_Unit* nal_unit(size_t NumBytesInNalUnit) {
    NAL_Unit* nal = malloc(sizeof(NAL_Unit));
    nal->nuh = nal_unit_header();
    nal->NumBytesInRbsp = 0;
    nal->rbsp_byte = malloc(sizeof(uint8_t) * NumBytesInNalUnit - 2);
    for (size_t i = 2; i < NumBytesInNalUnit; i++) {
        if (i + 2 < NumBytesInNalUnit && next_bits(24) == 0x000003) {
            nal->rbsp_byte[nal->NumBytesInRbsp++] = b();
            nal->rbsp_byte[nal->NumBytesInRbsp++] = b();
            i += 2;
            f(8, emulation_prevention_three_byte);
        } else {
            nal->rbsp_byte[nal->NumBytesInRbsp++] = b();
        }
    }
    nal->rbsp_byte = realloc(nal->rbsp_byte, sizeof(uint8_t) * nal->NumBytesInRbsp);

    // Save current context and switch to RBSP context
    Context* saved_ctx = ctx;
    Context* nal_ctx;
    nal_ctx = malloc(sizeof(Context));
    nal_ctx->data = nal->rbsp_byte;
    nal_ctx->size = nal->NumBytesInRbsp;
    nal_ctx->bit_offset = 0;
    ctx = nal_ctx;

    switch (nal->nuh->nal_unit_type) {
        case AUD_NUT:
            nal->payload.aud = access_unit_delimiter_rbsp();
            break;
        default:
            break;
    }

    // Restore original context
    ctx->data = NULL;
    free(ctx);
    ctx = saved_ctx;

    return nal;
}

void freeNALUnit(NAL_Unit* nal_unit) {
    if (nal_unit) {
        freeNUH(nal_unit->nuh);
        if (nal_unit->rbsp_byte) {
            free(nal_unit->rbsp_byte);
        }
        switch (nal_unit->nuh->nal_unit_type) {
            case AUD_NUT:
                freeAUD(nal_unit->payload.aud);
                break;
            default:
                break;
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
    switch (nal_unit->nuh->nal_unit_type) {
        case AUD_NUT:
            printAUD(nal_unit->payload.aud);
            break;
        default:
            break;
    }
}