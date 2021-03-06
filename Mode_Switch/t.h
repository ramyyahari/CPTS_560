#ifndef t_h
#define t_h

#include "kernel.c"
#include "int.c"

extern PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;
extern int body();

int init();
int scheduler();
int int80h();

int set_vec(u16 vector, u16 addr);

#endif
