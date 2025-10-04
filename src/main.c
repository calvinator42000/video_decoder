#include <main.h>

Context* ctx;

size_t get_NumBytesInNALUnit() {
    size_t initial_offset = ctx->byte_stream->bit_offset;
    size_t num_bytes = 0;
    while (1) {
        if (ctx->byte_stream->bit_offset / 8 + 2 == ctx->byte_stream->size) {
            num_bytes += 2;
            ctx->byte_stream->bit_offset += 16;
            break;
        }
        if (!(next_bits(24) == 0x000001 || next_bits(24) == 0x000000)) {
            num_bytes++;
            ctx->byte_stream->bit_offset += 8;
        } else {
            break;
        }
    }
    ctx->byte_stream->bit_offset = initial_offset;
    return num_bytes;
}

size_t byte_stream_nal_unit() {
    while(next_bits(24) != 0x000001 && next_bits(32) != 0x00000001) {
        f(8, leading_zero_8bits);
    }
    if (next_bits(24) != 0x000001) {
        f(8, zero_byte);
    }
    f(24, start_code_prefix_one_3bytes);
    size_t NumBytesInNalUnit = get_NumBytesInNALUnit();
    printf("NAL Unit found at byte offset %zu, size: %zu bytes\n", ctx->byte_stream->bit_offset / 8, NumBytesInNalUnit);
    size_t next_nal_start_offset = ctx->byte_stream->bit_offset + NumBytesInNalUnit * 8; // Saving this so parsing future nal units isn't reliant on the parsing of the previous one
    NAL_Unit* nal = nal_unit(NumBytesInNalUnit);
    printNAL(nal);
    freeNAL(nal);
    ctx->byte_stream->bit_offset = next_nal_start_offset;
    while (more_data_in_byte_stream() && next_bits(24) != 0x000001 && next_bits(32) != 0x00000001) {
        f(8, trailing_zero_8bits);
    }
    return NumBytesInNalUnit;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Usage: %s <input_vvc_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* input_file_name = argv[1];

    FILE* fptr;
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

    ctx = malloc(sizeof(Context));
    if (ctx == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    ctx->byte_stream = NULL;
    ctx->rbsp = NULL;

    // Create and initialize data buffer
    ctx->byte_stream = malloc(sizeof(Data_Buffer));
    if (ctx->byte_stream == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    ctx->byte_stream->data = malloc(sizeof(uint8_t) * filesize);
    if (ctx->byte_stream->data == NULL) {
        printf("Memory allocation failed!\n");
        freeContext();
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    ctx->byte_stream->size = fread(ctx->byte_stream->data, sizeof(uint8_t), filesize, fptr);
    if (ctx->byte_stream->size != filesize) {
        printf("Error reading file!\n");
        freeContext();
        fclose(fptr);
        exit(EXIT_FAILURE);
    }
    fclose(fptr);
    ctx->byte_stream->bit_offset = 0;
    ctx->mode = BYTE_STREAM_MODE;

    // Process NAL units
    while ((ctx->byte_stream->bit_offset + 7) / 8 < ctx->byte_stream->size) {
        byte_stream_nal_unit();
    }

    freeContext();
    return 0;
}
