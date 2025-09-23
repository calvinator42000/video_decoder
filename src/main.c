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

    printf("Read %zu bytes from file.\n", read_size);

    printf("First 10 bytes of the file:\n");
    for (size_t i = 0; i < 10 && i < read_size; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
    free(buffer);
    fclose(fptr);
    return 0;
}