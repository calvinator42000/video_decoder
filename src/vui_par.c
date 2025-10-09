#include <vui_par.h>

VUI_Parameters* vui_parameters(uint_t payloadSize, uint_t ChromaFormatIdc) {
    VUI_Parameters* vui_par = initVUI_Par();

    vui_par->ChromaFormatIdc = ChromaFormatIdc;

    vui_par->vui_progressive_source_flag = u(1);
    vui_par->vui_interlaced_source_flag = u(1);
    vui_par->vui_non_packed_constraint_flag = u(1);
    vui_par->vui_non_projected_constraint_flag = u(1);
    vui_par->vui_aspect_ratio_info_present_flag = u(1);
    if (vui_par->vui_aspect_ratio_info_present_flag) {
        vui_par->vui_aspect_ratio_constant_flag = u(1);
        vui_par->vui_aspect_ratio_idc = u(8);
        if (vui_par->vui_aspect_ratio_idc == 255) {
            vui_par->vui_sar_width = u(16);
            vui_par->vui_sar_height = u(16);
        }
    } else {
        vui_par->vui_aspect_ratio_constant_flag = 0;
        vui_par->vui_aspect_ratio_idc = 0;
    }
    vui_par->vui_overscan_info_present_flag = u(1);
    if (vui_par->vui_overscan_info_present_flag) {
        vui_par->vui_overscan_appropriate_flag = u(1);
    }
    vui_par->vui_colour_description_present_flag = u(1);
    if (vui_par->vui_colour_description_present_flag) {
        vui_par->vui_colour_primaries = u(8);
        vui_par->vui_transfer_characteristics = u(8);
        vui_par->vui_matrix_coeffs = u(8);
        vui_par->vui_full_range_flag = u(1);
    } else {
        vui_par->vui_colour_primaries = 2;
        vui_par->vui_transfer_characteristics = 2;
        vui_par->vui_matrix_coeffs = 2;
        vui_par->vui_full_range_flag = 0;
    }
    vui_par->vui_chroma_loc_info_present_flag = u(1);
    if (vui_par->vui_chroma_loc_info_present_flag) {
        if (vui_par->vui_progressive_source_flag && !vui_par->vui_interlaced_source_flag) {
            vui_par->vui_chroma_sample_loc_type_frame = ue();
            vui_par->vui_chroma_sample_loc_type_top_field = vui_par->vui_chroma_sample_loc_type_frame;
            vui_par->vui_chroma_sample_loc_type_bottom_field = vui_par->vui_chroma_sample_loc_type_frame;
        } else {
            vui_par->vui_chroma_sample_loc_type_top_field = ue();
            vui_par->vui_chroma_sample_loc_type_bottom_field = ue();
        }
    } else if (ChromaFormatIdc == 1) {
        vui_par->vui_chroma_sample_loc_type_frame = 6;
    }
    return vui_par;
}

VUI_Parameters* initVUI_Par() {
    VUI_Parameters* vui_par = malloc(sizeof(VUI_Parameters));
    if (vui_par == NULL) {
        printf("Memory allocation failed: VUI_Parameters\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    return vui_par;
}

void freeVUI_Par(VUI_Parameters* vui_par) {
    if (vui_par) {
        free(vui_par);
    }
}

void printVUI_Par(VUI_Parameters* vui_par) {
    incIndent();
    if (vui_par == NULL) {
        printVar("VUI Parameters is NULL\n");
        return;
    }
    printVar("VUI Parameters:\n");
    printVar("  vui_progressive_source_flag: %u\n", vui_par->vui_progressive_source_flag);
    printVar("  vui_interlaced_source_flag: %u\n", vui_par->vui_interlaced_source_flag);
    printVar("  vui_non_packed_constraint_flag: %u\n", vui_par->vui_non_packed_constraint_flag);
    printVar("  vui_non_projected_constraint_flag: %u\n", vui_par->vui_non_projected_constraint_flag);
    printVar("  vui_aspect_ratio_info_present_flag: %u\n", vui_par->vui_aspect_ratio_info_present_flag);
    printVar("  vui_aspect_ratio_constant_flag: %u\n", vui_par->vui_aspect_ratio_constant_flag);
    printVar("  vui_aspect_ratio_idc: %u\n", vui_par->vui_aspect_ratio_idc);
    if (vui_par->vui_aspect_ratio_info_present_flag && vui_par->vui_aspect_ratio_idc == 255) {
        printVar("  vui_sar_width: %u\n", vui_par->vui_sar_width);
        printVar("  vui_sar_height: %u\n", vui_par->vui_sar_height);
    }
    printVar("  vui_overscan_info_present_flag: %u\n", vui_par->vui_overscan_info_present_flag);
    if (vui_par->vui_overscan_info_present_flag) {
        printVar("  vui_overscan_appropriate_flag: %u\n", vui_par->vui_overscan_appropriate_flag);
    }
    printVar("  vui_colour_description_present_flag: %u\n", vui_par->vui_colour_description_present_flag);
    printVar("  vui_colour_primaries: %u\n", vui_par->vui_colour_primaries);
    printVar("  vui_transfer_characteristics: %u\n", vui_par->vui_transfer_characteristics);
    printVar("  vui_matrix_coeffs: %u\n", vui_par->vui_matrix_coeffs);
    printVar("  vui_full_range_flag: %u\n", vui_par->vui_full_range_flag);
    printVar("  vui_chroma_loc_info_present_flag: %u\n", vui_par->vui_chroma_loc_info_present_flag);
    if (vui_par->vui_chroma_loc_info_present_flag) {
        if (vui_par->vui_progressive_source_flag && !vui_par->vui_interlaced_source_flag) {
            printVar("  vui_chroma_sample_loc_type_frame: %u\n", vui_par->vui_chroma_sample_loc_type_frame);
        }
        printVar("  vui_chroma_sample_loc_type_top_field: %u\n", vui_par->vui_chroma_sample_loc_type_top_field);
        printVar("  vui_chroma_sample_loc_type_bottom_field: %u\n", vui_par->vui_chroma_sample_loc_type_bottom_field);
    } else if (vui_par->ChromaFormatIdc == 1) {
        printVar("  vui_chroma_sample_loc_type_frame: %u\n", vui_par->vui_chroma_sample_loc_type_frame);
        printVar("  vui_chroma_sample_loc_type_top_field: %u\n", vui_par->vui_chroma_sample_loc_type_top_field);
        printVar("  vui_chroma_sample_loc_type_bottom_field: %u\n", vui_par->vui_chroma_sample_loc_type_bottom_field);
    }
    decIndent();
}
