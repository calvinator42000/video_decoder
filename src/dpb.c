#include <dpb.h>

DPB_Parameters* dpb_parameters(uint_t MaxSubLayersMinus1, uint_t subLayerInfoFlag) {
    DPB_Parameters* dpb = initDPB();

    dpb->MaxSubLayersMinus1 = MaxSubLayersMinus1;
    dpb->subLayerInfoFlag = subLayerInfoFlag;

    dpb->dpb_max_dec_pic_buffering_minus1 = malloc(sizeof(uint_t) * (MaxSubLayersMinus1 + 1));
    dpb->dpb_max_num_reorder_pics = malloc(sizeof(uint_t) * (MaxSubLayersMinus1 + 1));
    dpb->dpb_max_latency_increase_plus1 = malloc(sizeof(uint_t) * (MaxSubLayersMinus1 + 1));
    for (int i = (subLayerInfoFlag ? 0 : MaxSubLayersMinus1); i <= MaxSubLayersMinus1; i++) {
        dpb->dpb_max_dec_pic_buffering_minus1[i] = ue();
        dpb->dpb_max_num_reorder_pics[i] = ue();
        dpb->dpb_max_latency_increase_plus1[i] = ue();
    }
    return dpb;
}

DPB_Parameters* initDPB() {
    DPB_Parameters* dpb = malloc(sizeof(DPB_Parameters));
    if (dpb == NULL) {
        printf("Memory allocation failed: DPB_Parameters\n");
        freeContext();
        exit(EXIT_FAILURE);
    }
    dpb->dpb_max_dec_pic_buffering_minus1 = NULL;
    dpb->dpb_max_num_reorder_pics = NULL;
    dpb->dpb_max_latency_increase_plus1 = NULL;
    return dpb;
}

void freeDPB(DPB_Parameters* dpb) {
    if (dpb) {
        if (dpb->dpb_max_dec_pic_buffering_minus1) {
            free(dpb->dpb_max_dec_pic_buffering_minus1);
        }
        if (dpb->dpb_max_num_reorder_pics) {
            free(dpb->dpb_max_num_reorder_pics);
        }
        if (dpb->dpb_max_latency_increase_plus1) {
            free(dpb->dpb_max_latency_increase_plus1);
        }
        free(dpb);
    }
}

void printDPB(DPB_Parameters* dpb) {
    incIndent();
    if (dpb == NULL) {
        printIndent();
        printf("DPB Parameters is NULL\n");
        return;
    }
    printIndent();
    printf("DPB Parameters:\n");
    printIndent();
    printf("  dpb_max_dec_pic_buffering_minus1: {");
    for (int i = (dpb->subLayerInfoFlag ? 0 : dpb->MaxSubLayersMinus1); i <= dpb->MaxSubLayersMinus1; i++) {
        if (i > (dpb->subLayerInfoFlag ? 0 : dpb->MaxSubLayersMinus1)) {
            printf(",");
        }
        printf("%u", dpb->dpb_max_dec_pic_buffering_minus1[i]);
    }
    printf("}\n");
    printIndent();
    printf("  dpb_max_num_reorder_pics: {");
    for (int i = (dpb->subLayerInfoFlag ? 0 : dpb->MaxSubLayersMinus1); i <= dpb->MaxSubLayersMinus1; i++) {
        if (i > (dpb->subLayerInfoFlag ? 0 : dpb->MaxSubLayersMinus1)) {
            printf(",");
        }
        printf("%u", dpb->dpb_max_num_reorder_pics[i]);
    }
    printf("}\n");
    printIndent();
    printf("  dpb_max_latency_increase_plus1: {");
    for (int i = (dpb->subLayerInfoFlag ? 0 : dpb->MaxSubLayersMinus1); i <= dpb->MaxSubLayersMinus1; i++) {
        if (i > (dpb->subLayerInfoFlag ? 0 : dpb->MaxSubLayersMinus1)) {
            printf(",");
        }
        printf("%u", dpb->dpb_max_latency_increase_plus1[i]);
    }
    printf("}\n");
    decIndent();
}