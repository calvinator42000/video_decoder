#include <utils.h>

extern Context* ctx;

uint8_t readByte(size_t index) {
    if (index >= ctx->size) {
        fprintf(stderr, "Index out of bounds in read_byte: %zu\n", index);
        freeContext();
        exit(EXIT_FAILURE);
    }
    return ctx->data[ctx->byte_offset + index];
}

size_t readBytes(size_t index, size_t length) {
    if (index + length > ctx->size) {
        fprintf(stderr, "Index out of bounds in read_bytes: %zu + %zu\n", index, length);
        freeContext();
        exit(EXIT_FAILURE);
    }
    size_t value = 0;
    for (size_t i = 0; i < length; i++) {
        value = (value << 8) | readByte(index + i);
    }
    return value;
}

void freeContext() {
    if (ctx) {
        if (ctx->data) {
            free(ctx->data);
        }
        free(ctx);
    }
}