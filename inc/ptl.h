#ifndef PTL_H
#define PTL_H

#include <utils.h>

// 7.3.3.1 General profile, tier, and level status - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {

} Profile_Tier_Level;

Profile_Tier_Level* profile_tier_level(uint_t profileTierPresentFlag, uint_t MaxNumSubLayerMinus1);

void freePTL(Profile_Tier_Level* ptl);
void printPTL(Profile_Tier_Level* ptl);

#endif // PTL_H