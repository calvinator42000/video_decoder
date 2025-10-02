#include <aud.h>

Access_Unit_Delimiter* access_unit_delimiter_rbsp() {
    Access_Unit_Delimiter* aud = malloc(sizeof(Access_Unit_Delimiter));
    aud->aud_irap_or_gdr_flag = u(1);
    aud->aud_pic_type = u(3);
    rbsp_trailing_bits();
    return aud;
}

void freeAUD(Access_Unit_Delimiter* aud) {
    if (aud) {
        free(aud);
    }
}

void printAUD(Access_Unit_Delimiter* aud) {
    if (aud == NULL) {
        printf("Access Unit Delimiter is NULL\n");
        return;
    }
    printf("Access Unit Delimiter:\n");
    printf("  aud_irap_or_gdr_flag: %u\n", aud->aud_irap_or_gdr_flag);
    printf("  aud_pic_type: %u\n", aud->aud_pic_type);
}