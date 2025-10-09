#include <utils.h>

extern Context* ctx;

size_t readBytes(size_t index, size_t length) {
    Data_Buffer* buffer = getDataBuffer();
    if (index + length > buffer->size) {
        fprintf(stderr, "ERROR: Index out of bounds in read_bytes: %zu + %zu, size: %zu\n", index, length, buffer->size);
        freeContext();
        exit(EXIT_FAILURE);
    }
    size_t value = 0;
    for (size_t i = 0; i < length; i++) {
        value = (value << 8) | buffer->data[index + i];
    }
    return value;
}

int byte_aligned() {
    Data_Buffer* buffer = getDataBuffer();
    return buffer->bit_offset % 8 == 0;
}

int more_data_in_byte_stream() {
    Data_Buffer* buffer = getDataBuffer();
    return buffer->bit_offset < buffer->size * 8;
}

int more_data_in_payload(size_t initOffset, uint_t payloadSize) {
    if (byte_aligned() && (getDataBuffer()->bit_offset - initOffset) == 8 * payloadSize) {
        return 0;
    }
    return 1;
}

int more_rbsp_data() {
    if (!more_data_in_byte_stream()) {
        return 0;
    }
    Data_Buffer* buffer = ctx->rbsp;
    size_t byte_offset = buffer->size - 1;
    size_t lastByte = readBytes(byte_offset, 1);
    // Find last byte in rbsp that is not 0 as that will contain the rbsp_stop_one_bit
    while (lastByte == 0) {
        byte_offset--;
        lastByte = readBytes(byte_offset, 1);
    }
    uint_t numShifts = 0;
    while (lastByte != 0) {
        lastByte <<= lastByte;
        numShifts++;
    }
    if (buffer->bit_offset < byte_offset * 8 + numShifts - 1) {
        return 1;
    }
    return 0;
}

size_t next_bits(size_t n) {
    Data_Buffer* buffer = getDataBuffer();
    size_t value;
    if (n == 0) {
        return 0;
    }
    // Read all bytes that are fully covered by the bits to read
    size_t num_bytes = ((buffer->bit_offset % 8) + n + 7) / 8;
    value = readBytes(buffer->bit_offset / 8, num_bytes);
    // Shift right to discard bits after the n bits we want
    value >>= (num_bytes * 8 - n - (buffer->bit_offset % 8));
    // Mask to keep only n bits
    value &= (1ULL << n) - 1;
    return value;
}

int payload_extension_present(size_t initOffset, uint_t payloadSize) {
    if (!(next_bits(1) == 1 && (getDataBuffer()->bit_offset - initOffset) < 8 * payloadSize)) {
        return 1;
    }
    return 0;
}

size_t read_bits(size_t n) {
    Data_Buffer* buffer = getDataBuffer();
    size_t value = next_bits(n);
    buffer->bit_offset += n;
    return value;
}

uint8_t b() {
    return (uint8_t)read_bits(8);
}

void f(size_t n, size_t val) {
    Data_Buffer* buffer = getDataBuffer();
    size_t initial_offset = buffer->bit_offset;
    size_t val_read = read_bits(n);
    if (val_read != val) {
        fprintf(stderr, "Expected %zu bits with value 0x%zX, but got 0x%zX at byte index %zu (bit index %zu)\n", n, val, val_read, initial_offset / 8, initial_offset % 8);
        freeContext();
        exit(EXIT_FAILURE);
    }
}

int se() {
    uint_t codeNum = expGolombCode(0);
    return (int)(pow(-1, codeNum+1) * ceil((double)codeNum / 2.0));
}

uint_t u(size_t n) {
    return (uint_t)read_bits(n);
}

uint_t ue() {
    return expGolombCode(0);
}

void rbsp_trailing_bits() {
    f(1, rbsp_stop_one_bit);
    while (!byte_aligned()) {
        f(1, rbsp_alignment_zero_bit);
    }
}

uint_t expGolombCode(uint_t k) {
    int leadingZeroBits = -1;
    for (uint_t b = 0; !b; leadingZeroBits++) {
        b = read_bits(1);
    }
    uint_t codeNum = (pow(2, leadingZeroBits) - 1) * pow(2, k) + read_bits(leadingZeroBits + k);
    return codeNum;
}

Data_Buffer* getDataBuffer() {
    switch (ctx->mode) {
        case BYTE_STREAM_MODE:
            return ctx->byte_stream;
        case RBSP_MODE:
            return ctx->rbsp;
        default:
            fprintf(stderr, "Invalid BufferMode in getDataBuffer\n");
            return NULL;
    }
}

Context* getContext() {
    return ctx;
}

void freeDataBuffer(Data_Buffer* data_buffer) {
    if (data_buffer) {
        if (data_buffer->data) {
            free(data_buffer->data);
        }
        free(data_buffer);
    }
}

void freeContext() {
    if (ctx) {
        if (ctx->byte_stream) {
            freeDataBuffer(ctx->byte_stream);
        }
        if (ctx->rbsp) {
            freeDataBuffer(ctx->rbsp);
        }
        free(ctx);
    }
}

void incIndent() {
    if (strlen(ctx->print_indent) > 30) {
        printf("ERROR: incIndent() called with indent length greater than allowed space: 30\n");
    }
    char indent[] = "  ";
    strcat(ctx->print_indent, indent);
}

void decIndent() {
    if (strlen(ctx->print_indent) < 2) {
        printf("ERROR: decIndent() called with indent length less than 2\n");
    }
    ctx->print_indent[strlen(ctx->print_indent)-2] = '\0';
}

void printIndent() {
    printf(ctx->print_indent);
}

void printVar(char* fmt_str, ...) {
    va_list args;
    va_start(args, fmt_str);
    printf(ctx->print_indent);
    vprintf(fmt_str, args);
    va_end(args);
}