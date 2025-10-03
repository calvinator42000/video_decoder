#ifndef GCI_H
#define GCI_H

#include <utils.h>

// 7.3.3.2 General constraints information syntax - Rec. ITU-T H.266 (V3) (09/2023)
typedef struct {

} General_Constraints_Info;

General_Constraints_Info* general_constraints_info();

void freeGCI(General_Constraints_Info* gci);
void printGCI(General_Constraints_Info* gci);

#endif // GCI_H