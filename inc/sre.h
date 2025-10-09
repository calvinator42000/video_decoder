#ifndef SRE_H
#define SRE_H

#include <utils.h>

typedef struct {
    uint_t sps_extended_precision_flag;
    uint_t sps_ts_residual_coding_rice_present_in_sh_flag;
    uint_t sps_rrc_rice_extension_flag;
    uint_t sps_persistent_rice_adaptation_enabled_flag;
    uint_t sps_reverse_last_sig_coeff_enabled_flag;
} SPS_Range_Extension;

SPS_Range_Extension* sps_range_extension(uint_t sps_transform_skip_enabled_flag);

SPS_Range_Extension* initSRE();
void freeSRE(SPS_Range_Extension* sre);
void printSRE(SPS_Range_Extension* sre);

#endif // SRE_H