#ifndef VUI_PAR_H
#define VUI_PAR_H

#include <utils.h>

// 7.2 VUI parameters syntax - Rec. ITU-T H.274 (V3) (09/2023)
typedef struct {
    // Parameters
    uint_t ChromaFormatIdc;

    uint_t vui_progressive_source_flag;
    uint_t vui_interlaced_source_flag;
    uint_t vui_non_packed_constraint_flag;
    uint_t vui_non_projected_constraint_flag;
    uint_t vui_aspect_ratio_info_present_flag;
    uint_t vui_aspect_ratio_constant_flag;
    uint_t vui_aspect_ratio_idc;
    uint_t vui_sar_width;
    uint_t vui_sar_height;
    uint_t vui_overscan_info_present_flag;
    uint_t vui_overscan_appropriate_flag;
    uint_t vui_colour_description_present_flag;
    uint_t vui_colour_primaries;
    uint_t vui_transfer_characteristics;
    uint_t vui_matrix_coeffs;
    uint_t vui_full_range_flag;
    uint_t vui_chroma_loc_info_present_flag;
    uint_t vui_chroma_sample_loc_type_frame;
    uint_t vui_chroma_sample_loc_type_top_field;
    uint_t vui_chroma_sample_loc_type_bottom_field;
} VUI_Parameters;

VUI_Parameters* vui_parameters(uint_t payloadSize, uint_t ChromaFormatIdc);

VUI_Parameters* initVUI_Par();
void freeVUI_Par(VUI_Parameters* vui_par);
void printVUI_Par(VUI_Parameters* vui_par);

#endif // VUI_PAR_H