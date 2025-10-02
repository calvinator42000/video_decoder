#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// B.2.2 - Rec. ITU-T H.266 (V3) (09/2023)
#define leading_zero_8bits 0x00
#define zero_byte 0x00
#define start_code_prefix_one_3bytes 0x000001
#define trailing_zero_8bits 0x00

// 7.4.3.16 RBSP trailing bits semantics - Rec. ITU-T H.266 (V3) (09/2023)
#define rbsp_stop_one_bit 1
#define rbsp_alignment_zero_bit 0

typedef unsigned int uint_t;

typedef struct {
    uint8_t* data;
    size_t bit_offset;
    size_t size;
} Data_Buffer;

typedef enum {
    BYTE_STREAM_MODE,
    RBSP_MODE
} BufferMode;

typedef struct {
    BufferMode mode;
    Data_Buffer* byte_stream;
    Data_Buffer* rbsp;
} Context;

size_t readBytes(size_t index, size_t length);

// 7.2 - Rec. ITU-T H.266 (V3) (09/2023)
int byte_aligned();
int more_data_in_byte_stream();
size_t next_bits(size_t n);
size_t read_bits(size_t n);
uint8_t b();
void f(size_t n, size_t val);
uint_t u(size_t n);

void freeDataBuffer(Data_Buffer* data_buffer);
void freeContext();

// 7.3.2.16 RBSP trailing bits syntax - Rec. ITU-T H.266 (V3) (09/2023)
void rbsp_trailing_bits();

Data_Buffer* getDataBuffer();

#endif // UTILS_H
