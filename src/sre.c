#include <sre.h>

SPS_Range_Extension* sps_range_extension(uint_t sps_transform_skip_enabled_flag) {
    SPS_Range_Extension* sre = initSRE();
    sre->sps_extended_precision_flag = u(1);
    if (sps_transform_skip_enabled_flag) {
        sre->sps_ts_residual_coding_rice_present_in_sh_flag = u(1);
    } else {
        sre->sps_ts_residual_coding_rice_present_in_sh_flag = 0;
    }
    sre->sps_rrc_rice_extension_flag = u(1);
    sre->sps_persistent_rice_adaptation_enabled_flag = u(1);
    sre->sps_reverse_last_sig_coeff_enabled_flag = u(1);
    return sre;
}

SPS_Range_Extension* initSRE() {
    SPS_Range_Extension* sre = malloc(sizeof(SPS_Range_Extension));
    if (sre == NULL) {
        printf("Memory allocation failed: SPS_Range_Extension\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    return sre;
}

void freeSRE(SPS_Range_Extension* sre) {
    if (sre) {
        free(sre);
    }
}

void printSRE(SPS_Range_Extension* sre) {
    incIndent();
    if (sre == NULL) {
        printVar("SPS Range Extension is NULL\n");
        return;
    }
    printVar("SPS Range Extension:\n");
    printVar("  sps_extended_precision_flag: %u\n", sre->sps_extended_precision_flag);
    printVar("  sps_ts_residual_coding_rice_present_in_sh_flag: %u\n", sre->sps_ts_residual_coding_rice_present_in_sh_flag);
    printVar("  sps_rrc_rice_extension_flag: %u\n", sre->sps_rrc_rice_extension_flag);
    printVar("  sps_persistent_rice_adaptation_enabled_flag: %u\n", sre->sps_persistent_rice_adaptation_enabled_flag);
    printVar("  sps_reverse_last_sig_coeff_enabled_flag: %u\n", sre->sps_reverse_last_sig_coeff_enabled_flag);
    decIndent();
}
