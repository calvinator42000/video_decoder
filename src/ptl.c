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
    // TODO: finish implementing this

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
        free(ptl);
    }
}

void printPTL(Profile_Tier_Level* ptl) {
    if (ptl == NULL) {
        printf("Profile Tier Level is NULL\n");
        return;
    }
    printf("Profile Tier Level:\n");
    if (ptl->profileTierPresentFlag) {
        printf("  general_profile_idc: %u\n", ptl->general_profile_idc);
        printf("  general_tier_flag: %u\n", ptl->general_tier_flag);
    }
    printf("  general_level_idc: %u\n", ptl->general_level_idc);
    printf("  ptl_frame_only_constraint_flag: %u\n", ptl->ptl_frame_only_constraint_flag);
    printf("  ptl_multilayer_enabled_flag: %u\n", ptl->ptl_multilayer_enabled_flag);
    if (ptl->profileTierPresentFlag) {
        printGCI(ptl->gci);
    }
    printf("  ptl_sublayer_level_present_flag: {");
    for (int i = ptl->MaxNumSubLayerMinus1 - 1; i >= 0; i--) {
        ptl->ptl_sublayer_level_present_flag[i] = u(1);
        if (i < ptl->MaxNumSubLayerMinus1 - 1) {
            printf(",");
        }
        printf("%u", ptl->ptl_sublayer_level_present_flag[i]);
    }
    printf("}\n");
}