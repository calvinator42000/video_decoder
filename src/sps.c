#include <sps.h>

Sequence_Parameter_Set* seq_parameter_set_rbsp() {
    Sequence_Parameter_Set* sps = malloc(sizeof(Sequence_Parameter_Set));
    return sps;
}

void freeSPS(Sequence_Parameter_Set* sps) {
    if (sps) {
        free(sps);
    }
}

void printSPS(Sequence_Parameter_Set* sps) {
    if (sps == NULL) {
        printf("Sequence Parameter Set is NULL\n");
        return;
    }
    printf("Sequence Parameter Set:\n");
}