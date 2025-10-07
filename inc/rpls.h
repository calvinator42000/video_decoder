#ifndef RPLS_H
#define RPLS_H

#include <utils.h>
#include <sps.h>

struct Sequence_Parameter_Set_struct;
typedef struct Sequence_Parameter_Set_struct Sequence_Parameter_Set;

// 7.3.10 Reference picture list structure syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct Reference_Picture_List_Structure_struct{
    // Parameters
    Sequence_Parameter_Set* sps;
    uint_t listIdx;
    uint_t rplsIdx;
    uint_t* AbsDeltaPocSt;

    uint_t num_ref_entries;
    uint_t ltrp_in_header_flag;
    uint_t* inter_layer_ref_pic_flag;
    uint_t* st_ref_pic_flag;
    uint_t* abs_delta_poc_st;
    uint_t* strp_entry_sign_flag;
    uint_t* rpls_poc_lsb_lt;
    uint_t* ilrp_idx;
} Reference_Picture_List_Structure;

Reference_Picture_List_Structure* ref_pic_list_struct(uint_t listIdx, uint_t rplsIdx, Sequence_Parameter_Set* sps);

Reference_Picture_List_Structure* initRPLS();
void freeRPLS(Reference_Picture_List_Structure* rpls);
void printRPLS(Reference_Picture_List_Structure* rpls);

#endif // RPLS_H