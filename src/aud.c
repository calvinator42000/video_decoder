#include <aud.h>

Access_Unit_Delimiter* access_unit_delimiter_rbsp() {
    Access_Unit_Delimiter* aud = initAUD();
    aud->aud_irap_or_gdr_flag = u(1);
    aud->aud_pic_type = u(3);
    rbsp_trailing_bits();
    return aud;
}

Access_Unit_Delimiter* initAUD() {
    Access_Unit_Delimiter* aud = malloc(sizeof(Access_Unit_Delimiter));
    if (aud == NULL) {
        printf("Memory allocation failed: Access_Unit_Delimiter\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    return aud;
}

void freeAUD(Access_Unit_Delimiter* aud) {
    if (aud) {
        free(aud);
    }
}

void printAUD(Access_Unit_Delimiter* aud) {
    incIndent();
    if (aud == NULL) {
        printIndent();
        printf("Access Unit Delimiter is NULL\n");
        return;
    }
    printIndent();
    printf("Access Unit Delimiter:\n");
    PRINT_VAR(aud->aud_irap_or_gdr_flag);
    PRINT_VAR(aud->aud_pic_type);
    decIndent();
}