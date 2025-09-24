#include <main.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Usage: %s <input_vvc_file>\n", argv[0]);
        exit(1);
    }

    char* input_file_name = argv[1];

    FILE *fptr;
    long filesize;
    unsigned char *buffer;
    BufferContext* ctx = malloc(sizeof(BufferContext));

    fptr = fopen(input_file_name, "rb");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    if (fseek(fptr, 0L, SEEK_END) != 0) {
        printf("Error seeking to end of file!\n");
        fclose(fptr);
        exit(1);
    }

    filesize = ftell(fptr);
    if (filesize == -1) {
        printf("Error determining file size!\n");
        fclose(fptr);
        exit(1);
    }
    rewind(fptr);
    printf("File size: %ld bytes\n", filesize);

    buffer = malloc(sizeof(unsigned char) * filesize);
    if (buffer == NULL) {
        printf("Memory allocation failed!\n");
        fclose(fptr);
        exit(1);
    }
    size_t read_size = fread(buffer, sizeof(unsigned char), filesize, fptr);
    if (read_size != filesize) {
        printf("Error reading file!\n");
        free(buffer);
        fclose(fptr);
        exit(1);
    }

    ctx->data = buffer;
    ctx->bit_offset = 0;
    ctx->byte_offset = 0;
    ctx->size = read_size;

    // B.3 - Rec. ITU-T H.266 (V3) (09/2023)
    while ((ctx->byte_offset < ctx->size) &&
           !(ctx->data[ctx->byte_offset] == 0x00 &&
             ctx->data[ctx->byte_offset + 1] == 0x00 &&
             ctx->data[ctx->byte_offset + 2] == 0x00 &&
             ctx->data[ctx->byte_offset + 3] == 0x01)) {
        if (ctx->byte_offset != leading_zero_8bits) {
            printf("Expected leading_zero_8bits at byte offset %zu, found 0x%02X\n",
                   ctx->byte_offset, ctx->data[ctx->byte_offset]);
            free(buffer);
            fclose(fptr);
            exit(1);
        }
        ctx->byte_offset += 1;
    }
    printf("Found start code at byte offset: %zu\n", ctx->byte_offset);

    free(buffer);
    fclose(fptr);
    return 0;
}
