#ifndef PTL_H
#define PTL_H

#include <utils.h>

// 7.3.3.1 General profile, tier, and level status - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {
    uint_t general_profile_idc;
    uint_t general_tier_flag;
    uint_t general_level_idc;
    uint_t ptl_frame_only_constraint_flag;
    uint_t ptl_multilayer_enabled_flag;
    // general_constraints_info
    uint_t* ptl_sublayer_level_present_flag;
    uint_t ptl_reserved_zero_bit;
    uint_t* sublayer_level_idc;
    uint_t ptl_num_sub_profiles;
    uint_t* general_sub_profile_idc;
} Profile_Tier_Level;

Profile_Tier_Level* profile_tier_level(uint_t profileTierPresentFlag, uint_t MaxNumSubLayerMinus1);

void freePTL(Profile_Tier_Level* ptl);
void printPTL(Profile_Tier_Level* ptl);

#endif // PTL_H