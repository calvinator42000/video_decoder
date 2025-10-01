#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t* data;
    size_t bit_offset;
    size_t size;
} Context;

typedef unsigned int uint_t;

size_t readBytes(size_t index, size_t length);

// 7.2 - Rec. ITU-T H.266 (V3) (09/2023)
int more_data_in_byte_stream();
size_t next_bits(size_t n);
size_t read_bits(size_t n);
uint8_t b();
void f(size_t n, size_t val);
uint_t u(size_t n);

// B.2.2 - Rec. ITU-T H.266 (V3) (09/2023)
#define leading_zero_8bits 0x00
#define zero_byte 0x00
#define start_code_prefix_one_3bytes 0x000001
#define trailing_zero_8bits 0x00

void freeContext();

#endif // UTILS_H
