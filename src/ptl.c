#include <ptl.h>

Profile_Tier_Level* profile_tier_level(uint_t profileTierPresentFlag, uint_t MaxNumSubLayerMinus1) {
    Profile_Tier_Level* ptl = malloc(sizeof(Profile_Tier_Level));

    if (profileTierPresentFlag) {
        ptl->general_profile_idc = u(7);
        ptl->general_tier_flag = u(1);
    }
    ptl->general_level_idc = u(8);
    ptl->ptl_frame_only_constraint_flag = u(1);
    ptl->ptl_multilayer_enabled_flag = u(1);

    return ptl;
}

void freePTL(Profile_Tier_Level* ptl) {
    if (ptl) {
        free(ptl);
    }
}

void printPTL(Profile_Tier_Level* ptl) {
    if (ptl == NULL) {
        printf("Profile Tier Level is NULL\n");
        return;
    }
    printf("Profile Tier Level:\n");
}