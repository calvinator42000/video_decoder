#include <main.h>

Context* ctx;

size_t get_NumBytesInNALUnit() {
    size_t initial_offset = ctx->bit_offset;
    size_t num_bytes = 0;
    while (!(next_bits(24) == 0x000001 || next_bits(24) == 0x000000)) {
        num_bytes++;
        ctx->bit_offset += 8;
    }
    ctx->bit_offset = initial_offset;
    return num_bytes;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Usage: %s <input_vvc_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* input_file_name = argv[1];

    FILE *fptr;
    size_t filesize;

    fptr = fopen(input_file_name, "rb");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    // Get file size
    if (fseek(fptr, 0L, SEEK_END) != 0) {
        printf("Error seeking to end of file!\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    filesize = ftell(fptr);
    if (filesize == -1) {
        printf("Error determining file size!\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    rewind(fptr);
    printf("File size: %ld bytes\n", filesize);

    // Create and initialize context
    ctx = malloc(sizeof(Context));
    if (ctx == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    ctx->data = malloc(sizeof(uint8_t) * filesize);
    if (ctx->data == NULL) {
        printf("Memory allocation failed!\n");
        freeContext();
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    ctx->size = fread(ctx->data, sizeof(uint8_t), filesize, fptr);
    if (ctx->size != filesize) {
        printf("Error reading file!\n");
        freeContext();
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    fclose(fptr);
    ctx->bit_offset = 0;

    // B.2.1 Byte stream NAL unit syntax - Rec. ITU-T H.266 (V3) (09/2023)
    while ((ctx->bit_offset + 7) / 8 < ctx->size) {
        while(next_bits(24) != 0x000001 && next_bits(32) != 0x00000001) {
            f(8, leading_zero_8bits);
        }
        if (next_bits(24) != 0x000001) {
            f(8, zero_byte);
        }
        f(24, start_code_prefix_one_3bytes);
        size_t NumBytesInNalUnit = get_NumBytesInNALUnit();
        printf("NAL Unit found at byte offset %zu, size: %zu bytes\n", ctx->bit_offset / 8 - 4, NumBytesInNalUnit);
        // nal_unit()
        ctx->bit_offset += NumBytesInNalUnit * 8;
        while (more_data_in_byte_stream() && next_bits(24) != 0x000001 && next_bits(32) != 0x00000001) {
            f(8, trailing_zero_8bits);
        }
    }

    freeContext(ctx);
    return 0;
}
