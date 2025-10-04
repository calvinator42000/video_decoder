#ifndef NAL_H
#define NAL_H

#include <utils.h>
#include <nuh.h>
#include <aud.h>
#include <sps.h>

// 7.4.2.2 NAL unit header semantics - Rec. ITU-T H.266 (V3) (09/2023)
enum nal_unit_type {
    TRAIL_NUT = 0,
    STSA_NUT = 1,
    RADL_NUT = 2,
    RASL_NUT = 3,
    RSV_VCL_4 = 4,
    RSV_VCL_5 = 5,
    RSV_VCL_6 = 6,
    IDR_W_RADL = 7,
    IDR_N_LP = 8,
    CRA_NUT = 9,
    GDR_NUT = 10,
    RSV_IRAP_11 = 11,
    OPI_NUT = 12,
    DCI_NUT = 13,
    VPS_NUT = 14,
    SPS_NUT = 15,
    PPS_NUT = 16,
    PREFIX_APS_NUT = 17,
    SUFFIX_APS_NUT = 18,
    PH_NUT = 19,
    AUD_NUT = 20,
    EOS_NUT = 21,
    EOB_NUT = 22,
    PREFIX_SEI_NUT = 23,
    SUFFIX_SEI_NUT = 24,
    FD_NUT = 25,
    RSV_NVCL_26 = 26,
    RSV_NVCL_27 = 27,
    UNSPEC_28 = 28,
    UNSPEC_29 = 29,
    UNSPEC_30 = 30,
    UNSPEC_31 = 31
};

// 7.4.2.1 General NAL unit semantics - Rec. ITU-T H.266 (V3) (09/2023)
#define emulation_prevention_three_byte 0x03

// 7.3.1.1 General NAL unit syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {
    NAL_Unit_Header* nuh;
    union {
        Access_Unit_Delimiter* aud;
        Sequence_Parameter_Set* sps;
    } payload;
} NAL_Unit;

NAL_Unit* nal_unit(size_t NumBytesInNalUnit);

NAL_Unit* initNAL();
void freeNAL(NAL_Unit* nal);
void printNAL(NAL_Unit* nal);

#endif // NAL_H