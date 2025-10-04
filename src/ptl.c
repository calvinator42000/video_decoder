#include <ptl.h>

Profile_Tier_Level* profile_tier_level(uint_t profileTierPresentFlag, uint_t MaxNumSubLayerMinus1) {
    Profile_Tier_Level* ptl = initPTL();
    if (profileTierPresentFlag) {
        ptl->general_profile_idc = u(7);
        ptl->general_tier_flag = u(1);
    }
    ptl->general_level_idc = u(8);
    ptl->ptl_frame_only_constraint_flag = u(1);
    ptl->ptl_multilayer_enabled_flag = u(1);
    ptl->gci = general_constraints_info();
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
    return ptl;
}

void freePTL(Profile_Tier_Level* ptl) {
    if (ptl) {
        if (ptl->gci) {
            freeGCI(ptl->gci);
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
    printf("  general_profile_idc: %u\n", ptl->general_profile_idc);
    printf("  general_tier_flag: %u\n", ptl->general_tier_flag);
    printf("  general_level_idc: %u\n", ptl->general_level_idc);
    printf("  ptl_frame_only_constraint_flag: %u\n", ptl->ptl_frame_only_constraint_flag);
    printf("  ptl_multilayer_enabled_flag: %u\n", ptl->ptl_multilayer_enabled_flag);
    printGCI(ptl->gci);
}