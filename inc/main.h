#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

// B.3.3 - Rec. ITU-T H.266 (V3) (09/2023)
size_t get_NumBytesInNALUnit();

// B.2.1 Byte stream NAL unit syntax - Rec. ITU-T H.266 (V3) (09/2023)
byte_stream_nal_unit(size_t NumBytesInNalUnit) {
