#include <ptl.h>

Profile_Tier_Level* profile_tier_level(uint_t profileTierPresentFlag, uint_t MaxNumSubLayerMinus1) {
    Profile_Tier_Level* ptl = malloc(sizeof(Profile_Tier_Level));

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