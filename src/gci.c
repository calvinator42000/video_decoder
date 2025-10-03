#include <gci.h>

General_Constraints_Info* general_constraints_info() {
    General_Constraints_Info* gci = malloc(sizeof(General_Constraints_Info));

    return gci;
}

void freeGCI(General_Constraints_Info* gci) {
    if (gci) {
        free(gci);
    }
}

void printGCI(General_Constraints_Info* gci) {
    if (gci == NULL) {
        printf("General Constraints Info is NULL\n");
        return;
    }
    printf("General Constraints Info:\n");
}