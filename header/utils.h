#ifndef UTILS_H
#define UTILS_H

#include "header.h"

void cpuIdle();
void KillProc(pcb_t *pcb);
void saveArea(state_t* new,state_t* old);
void setProc(state_t* temp, int n);

#endif
