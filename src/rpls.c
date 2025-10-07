#include <rpls.h>

Reference_Picture_List_Structure* ref_pic_list_struct(uint_t listIdx, uint_t rplsIdx, Sequence_Parameter_Set* sps) {
    Reference_Picture_List_Structure* rpls = initRPLS();
    rpls->sps = sps;
    rpls->listIdx = listIdx;
    rpls->rplsIdx = rplsIdx;
    rpls->num_ref_entries = ue();
    if (sps->sps_long_term_ref_pics_flag && rplsIdx < sps->sps_num_ref_pic_lists[listIdx] && rpls->num_ref_entries > 0) {
        rpls->ltrp_in_header_flag = u(1);
    } else if (sps->sps_long_term_ref_pics_flag && rplsIdx == sps->sps_num_ref_pic_lists[listIdx]){
        rpls->ltrp_in_header_flag = 1;
    }
    int j = 0;
    rpls->inter_layer_ref_pic_flag = malloc(sizeof(uint_t) * rpls->num_ref_entries);
    rpls->st_ref_pic_flag = malloc(sizeof(uint_t) * rpls->num_ref_entries);
    rpls->abs_delta_poc_st = malloc(sizeof(uint_t) * rpls->num_ref_entries);
    rpls->AbsDeltaPocSt = malloc(sizeof(uint_t) * rpls->num_ref_entries);
    rpls->strp_entry_sign_flag = malloc(sizeof(uint_t) * rpls->num_ref_entries);
    rpls->rpls_poc_lsb_lt = malloc(sizeof(uint_t) * rpls->num_ref_entries);
    rpls->ilrp_idx = malloc(sizeof(uint_t) * rpls->num_ref_entries);
    for (int i = 0; i < rpls->num_ref_entries; i++) {
        if (sps->sps_inter_layer_prediction_enabled_flag) {
            rpls->inter_layer_ref_pic_flag[i] = u(1);
        } else {
            rpls->inter_layer_ref_pic_flag[i] = 0;
        }
        if (!rpls->inter_layer_ref_pic_flag[i]) {
            if (sps->sps_long_term_ref_pics_flag) {
                rpls->st_ref_pic_flag[i] = u(1);
            } else {
                rpls->st_ref_pic_flag[i] = 1;
            }
            if (rpls->st_ref_pic_flag[i]) {
                rpls->abs_delta_poc_st[i] = ue();
                if ((sps->sps_weighted_pred_flag || sps->sps_weighted_bipred_flag) && i != 0) {
                    rpls->AbsDeltaPocSt[i] = rpls->abs_delta_poc_st[i];
                } else {
                    rpls->AbsDeltaPocSt[i] = rpls->abs_delta_poc_st[i] + 1;
                }
                if (rpls->AbsDeltaPocSt[i] > 0) {
                    rpls->strp_entry_sign_flag[i] = u(1);
                } else {
                    rpls->strp_entry_sign_flag[i] = 0;
                }
            } else if (!rpls->ltrp_in_header_flag) {
                rpls->rpls_poc_lsb_lt[j++] = u(sps->sps_log2_max_pic_order_cnt_lsb_minus4 + 4);
            } else {
                rpls->strp_entry_sign_flag[i] = 0;
            }
        } else {
            rpls->ilrp_idx[i] = ue();
            rpls->strp_entry_sign_flag[i] = 0;
        }
    }
    return rpls;
}

Reference_Picture_List_Structure* initRPLS() {
    Reference_Picture_List_Structure* rpls = malloc(sizeof(Reference_Picture_List_Structure));
    rpls->inter_layer_ref_pic_flag = NULL;
    rpls->st_ref_pic_flag = NULL;
    rpls->abs_delta_poc_st = NULL;
    rpls->AbsDeltaPocSt = NULL;
    rpls->strp_entry_sign_flag = NULL;
    rpls->rpls_poc_lsb_lt = NULL;
    rpls->ilrp_idx = NULL;
    return rpls;
}

void freeRPLS(Reference_Picture_List_Structure* rpls) {
    if (rpls) {
        if (rpls->inter_layer_ref_pic_flag) {
            free(rpls->inter_layer_ref_pic_flag);
        }
        if (rpls->st_ref_pic_flag) {
            free(rpls->st_ref_pic_flag);
        }
        if (rpls->abs_delta_poc_st) {
            free(rpls->abs_delta_poc_st);
        }
        if (rpls->AbsDeltaPocSt) {
            free(rpls->AbsDeltaPocSt);
        }
        if (rpls->strp_entry_sign_flag) {
            free(rpls->strp_entry_sign_flag);
        }
        if (rpls->rpls_poc_lsb_lt) {
            free(rpls->rpls_poc_lsb_lt);
        }
        if (rpls->ilrp_idx) {
            free(rpls->ilrp_idx);
        }
        free(rpls);
    }
}

void printRPLS(Reference_Picture_List_Structure* rpls) {
    incIndent();
    if (rpls == NULL) {
        printVar("Reference Picture List Structure is NULL\n");
        return;
    }
    printVar("Reference Picture List Structure:\n");
    printVar("  num_ref_entries: %u\n", rpls->num_ref_entries);
    if (rpls->sps->sps_long_term_ref_pics_flag && ((rpls->rplsIdx < rpls->sps->sps_num_ref_pic_lists[rpls->listIdx] && rpls->num_ref_entries > 0) || (rpls->rplsIdx == rpls->sps->sps_num_ref_pic_lists[rpls->listIdx]))) {
        printVar("  ltrp_in_header_flag: %u\n", rpls->ltrp_in_header_flag);
    }
    printVar("  inter_layer_ref_pic_flag: {");
    for (int i = 0; i < rpls->num_ref_entries; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%u", rpls->inter_layer_ref_pic_flag[i]);
    }
    printf("}\n");
    printVar("  st_ref_pic_flag: {");
    for (int i = 0; i < rpls->num_ref_entries; i++) {
        if (i > 0) {
            printf(",");
        }
        if (!rpls->inter_layer_ref_pic_flag[i]) {
            printf("%u", rpls->st_ref_pic_flag[i]);
        } else {
            printf("x");
        }
    }
    printf("}\n");
    printVar("  abs_delta_poc_st: {");
    for (int i = 0; i < rpls->num_ref_entries; i++) {
        if (i > 0) {
            printf(",");
        }
        if (!rpls->inter_layer_ref_pic_flag[i] && rpls->st_ref_pic_flag[i]) {
            printf("%u", rpls->abs_delta_poc_st[i]);
        } else {
            printf("x");
        }
    }
    printf("}\n");
    printVar("  rpls_poc_lsb_lt: {");
    int j = 0;
    for (int i = 0; i < rpls->num_ref_entries; i++) {
        if (!rpls->inter_layer_ref_pic_flag[i] && !rpls->st_ref_pic_flag[i] && !rpls->ltrp_in_header_flag) {
            if (j > 0) {
                printf(",");
            }
            printf("%u", rpls->rpls_poc_lsb_lt[j]);
            j++;
        }
    }
    printf("}\n");
    printVar("  ilrp_idx: {");
    for (int i = 0; i < rpls->num_ref_entries; i++) {
        if (i > 0) {
            printf(",");
        }
        if (rpls->inter_layer_ref_pic_flag[i]) {
            printf("%u", rpls->ilrp_idx[i]);
        } else {
            printf("x");
        }
    }
    printf("}\n");
    printVar("  strp_entry_sign_flag: {");
    for (int i = 0; i < rpls->num_ref_entries; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%u", rpls->strp_entry_sign_flag[i]);
    }
    printf("}\n");
    decIndent();
}