#ifndef AUD_H
#define AUD_H

#include <utils.h>

typedef struct {
    uint_t aud_irap_or_gdr_flag;
    uint_t aud_pic_type;
} Access_Unit_Delimiter;

Access_Unit_Delimiter* access_unit_delimiter_rbsp();

void freeAUD(Access_Unit_Delimiter* aud);
void printAUD(Access_Unit_Delimiter* aud);

#endif // AUD_H