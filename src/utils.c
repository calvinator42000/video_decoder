#include <utils.h>

extern Context* ctx;

size_t readBytes(size_t index, size_t length) {
    if (index + length > ctx->size) {
        fprintf(stderr, "Index out of bounds in read_bytes: %zu + %zu\n", index, length);
        freeContext();
        exit(EXIT_FAILURE);
    }
    size_t value = 0;
    for (size_t i = 0; i < length; i++) {
        value = (value << 8) | ctx->data[index + i];
    }
    return value;
}

int byte_aligned() {
    return ctx->bit_offset % 8 == 0;
}

int more_data_in_byte_stream() {
    return ctx->bit_offset < ctx->size * 8;
}

size_t next_bits(size_t n) {
    size_t value;
    if (n == 0) {
        return 0;
    }
    // Read all bytes that are fully covered by the bits to read
    size_t num_bytes = ((ctx->bit_offset % 8) + n + 7) / 8;
    value = readBytes(ctx->bit_offset / 8, num_bytes);
    // Shift right to discard bits after the n bits we want
    value >>= (num_bytes * 8 - n - (ctx->bit_offset % 8));
    // Mask to keep only n bits
    value &= (1ULL << n) - 1;
    return value;
}

size_t read_bits(size_t n) {
    size_t value = next_bits(n);
    ctx->bit_offset += n;
    return value;
}

uint8_t b() {
    return (uint8_t)read_bits(8);
}

void f(size_t n, size_t val) {
    size_t initial_offset = ctx->bit_offset;
    size_t val_read = read_bits(n);
    if (val_read != val) {
        fprintf(stderr, "Expected %zu bits with value 0x%zX, but got 0x%zX at byte index %zu (bit index %zu)\n", n, val, val_read, initial_offset / 8, initial_offset % 8);
        freeContext();
        exit(EXIT_FAILURE);
    }
}

uint_t u(size_t n) {
    return (uint_t)read_bits(n);
}

void freeContext() {
    if (ctx) {
        if (ctx->data) {
            free(ctx->data);
        }
        free(ctx);
    }
}

void rbsp_trailing_bits() {
    f(1, 1); // rbsp_stop_one_bit
    while (!byte_aligned()) {
        f(1, 0); // rbsp_alignment_zero_bit
    }
}