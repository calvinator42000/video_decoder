#include <nal.h>

extern Context* ctx;

NAL_Unit* nal_unit(size_t NumBytesInNalUnit) {
    NAL_Unit* nal = malloc(sizeof(NAL_Unit));
    nal->nuh = nal_unit_header();
    size_t NumBytesInRbsp = 0;
    uint8_t* rbsp_byte = malloc(sizeof(uint8_t) * NumBytesInNalUnit - 2);
    for (size_t i = 2; i < NumBytesInNalUnit; i++) {
        if (i + 2 < NumBytesInNalUnit && next_bits(24) == 0x000003) {
            rbsp_byte[NumBytesInRbsp++] = b();
            rbsp_byte[NumBytesInRbsp++] = b();
            i += 2;
            f(8, emulation_prevention_three_byte);
        } else {
            rbsp_byte[NumBytesInRbsp++] = b();
        }
    }
    rbsp_byte = realloc(rbsp_byte, sizeof(uint8_t) * NumBytesInRbsp);

    // Save current buffer and switch to RBSP buffer
    if (ctx->rbsp) {
        freeDataBuffer(ctx->rbsp); // Free buffer just to be sure
    }
    ctx->rbsp = malloc(sizeof(Data_Buffer));
    ctx->rbsp->data = rbsp_byte;
    ctx->rbsp->size = NumBytesInRbsp;
    ctx->rbsp->bit_offset = 0;
    ctx->mode = RBSP_MODE;

    switch (nal->nuh->nal_unit_type) {
        case AUD_NUT:
            nal->payload.aud = access_unit_delimiter_rbsp();
            break;
        case SPS_NUT:
            nal->payload.sps = seq_parameter_set_rbsp();
            break;
        default:
            break;
    }

    // Restore original buffer
    ctx->mode = BYTE_STREAM_MODE;

    return nal;
}

void freeNALUnit(NAL_Unit* nal_unit) {
    if (nal_unit) {
        freeNUH(nal_unit->nuh);
        switch (nal_unit->nuh->nal_unit_type) {
            case AUD_NUT:
                freeAUD(nal_unit->payload.aud);
                break;
            case SPS_NUT:
                freeSPS(nal_unit->payload.sps);
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
    printf("RBSP Bytes (%zu bytes):\n", ctx->rbsp->data ? ctx->rbsp->size : 0);
    if (ctx->rbsp->data) {
        for (size_t i = 0; i < ctx->rbsp->size; i++) {
            printf("%02X ", ctx->rbsp->data[i]);
            if ((i + 1) % 16 == 0) {
                printf("\n");
            }
        }
        if (ctx->rbsp->size % 16 != 0) {
            printf("\n");
        }
    }
    switch (nal_unit->nuh->nal_unit_type) {
        case AUD_NUT:
            printAUD(nal_unit->payload.aud);
            break;
        case SPS_NUT:
            printSPS(nal_unit->payload.sps);
            break;
        default:
            break;
    }
}