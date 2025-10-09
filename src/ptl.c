#include <ptl.h>

Profile_Tier_Level* profile_tier_level(uint_t profileTierPresentFlag, uint_t MaxNumSubLayerMinus1) {
    Profile_Tier_Level* ptl = initPTL();

    // Parameters
    ptl->profileTierPresentFlag = profileTierPresentFlag;
    ptl->MaxNumSubLayerMinus1 = MaxNumSubLayerMinus1;

    if (profileTierPresentFlag) {
        ptl->general_profile_idc = u(7);
        ptl->general_tier_flag = u(1);
    }
    ptl->general_level_idc = u(8);
    ptl->ptl_frame_only_constraint_flag = u(1);
    ptl->ptl_multilayer_enabled_flag = u(1);
    if (profileTierPresentFlag) {
        ptl->gci = general_constraints_info();
    }
    ptl->ptl_sublayer_level_present_flag = malloc(sizeof(uint_t) * MaxNumSubLayerMinus1);
    for (int i = MaxNumSubLayerMinus1 - 1; i >= 0; i--) {
        ptl->ptl_sublayer_level_present_flag[i] = u(1);
    }
    while (!byte_aligned()) {
        f(1, ptl_reserved_zero_bit);
    }
    ptl->sublayer_level_idc = malloc(sizeof(uint_t) * (MaxNumSubLayerMinus1+1));
    ptl->sublayer_level_idc[MaxNumSubLayerMinus1] = ptl->general_level_idc;
    for (int i = MaxNumSubLayerMinus1 - 1; i >= 0; i--) {
        if (ptl->ptl_sublayer_level_present_flag[i]) {
            ptl->sublayer_level_idc[i] = u(8);
        } else {
            ptl->sublayer_level_idc[i] = ptl->sublayer_level_idc[i+1];
        }
    }
    if (profileTierPresentFlag) {
        ptl->ptl_num_sub_profiles = u(8);
        ptl->general_sub_profile_idc = malloc(sizeof(uint_t) * ptl->ptl_num_sub_profiles);
        for (int i = 0; i < ptl->ptl_num_sub_profiles; i++) {
            ptl->general_sub_profile_idc[i] = u(32);
        }
    }

    return ptl;
}

Profile_Tier_Level* initPTL() {
    Profile_Tier_Level* ptl = malloc(sizeof(Profile_Tier_Level));
    if (ptl == NULL) {
        printf("Memory allocation failed: Profile_Tier_Level\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    ptl->gci = NULL;
    ptl->ptl_sublayer_level_present_flag = NULL;
    ptl->sublayer_level_idc = NULL;
    ptl->general_sub_profile_idc = NULL;
    return ptl;
}

void freePTL(Profile_Tier_Level* ptl) {
    if (ptl) {
        if (ptl->gci) {
            freeGCI(ptl->gci);
        }
        if (ptl->ptl_sublayer_level_present_flag) {
            free(ptl->ptl_sublayer_level_present_flag);
        }
        if (ptl->sublayer_level_idc) {
            free(ptl->sublayer_level_idc);
        }
        if (ptl->general_sub_profile_idc) {
            free(ptl->general_sub_profile_idc);
        }
        free(ptl);
    }
}

void printPTL(Profile_Tier_Level* ptl) {
    incIndent();
    if (ptl == NULL) {
        printIndent();
        printf("Profile Tier Level is NULL\n");
        return;
    }
    printIndent();
    printf("Profile Tier Level:\n");
    if (ptl->profileTierPresentFlag) {
        PRINT_VAR(ptl->general_profile_idc);
        PRINT_VAR(ptl->general_tier_flag);
    }
    PRINT_VAR(ptl->general_level_idc);
    PRINT_VAR(ptl->ptl_frame_only_constraint_flag);
    PRINT_VAR(ptl->ptl_multilayer_enabled_flag);
    if (ptl->profileTierPresentFlag) {
        printGCI(ptl->gci);
    }
    printIndent();
    printf("  ptl_sublayer_level_present_flag: {");
    for (int i = ptl->MaxNumSubLayerMinus1 - 1; i >= 0; i--) {
        if (i < ptl->MaxNumSubLayerMinus1 - 1) {
            printf(",");
        }
        printf("%u", ptl->ptl_sublayer_level_present_flag[i]);
    }
    printf("}\n");
    printIndent();
    printf("  sublayer_level_idc: {");
    for (int i = ptl->MaxNumSubLayerMinus1; i >= 0; i--) {
        if (i < ptl->MaxNumSubLayerMinus1) {
            printf(",");
        }
        printf("%u", ptl->sublayer_level_idc[i]);
    }
    printf("}\n");
    if (ptl->profileTierPresentFlag) {
        PRINT_VAR(ptl->ptl_num_sub_profiles);
        printIndent();
        printf("  general_sub_profile_idc: {");
        for (int i = 0; i < ptl->ptl_num_sub_profiles; i++) {
            if (i > 0) {
                printf(",");
            }
            printf("%u", ptl->general_sub_profile_idc[i]);
        }
    }
    printf("}\n");
    decIndent();
}