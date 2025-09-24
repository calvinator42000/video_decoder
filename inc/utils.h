#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t* data;
    size_t byte_offset;
    size_t bit_offset;
    size_t size;
} Context;

uint8_t readByte(size_t index);

#define leading_zero_8bits 0x00
#define zero_byte 0x00

void freeContext();

#endif // UTILS_H
