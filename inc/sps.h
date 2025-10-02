#ifndef SPS_H
#define SPS_H

#include <utils.h>

typedef struct {

} Sequence_Parameter_Set;

Sequence_Parameter_Set* seq_parameter_set_rbsp();

void freeSPS(Sequence_Parameter_Set* sps);
void printSPS(Sequence_Parameter_Set* sps);

#endif // SPS_H