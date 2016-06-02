#ifndef kernel_h
#define kernel_h

int do_tswitch();
int do_kfork();
int body();
int kmode(); 
int do_ps();
int chname(char* y);
int loader();

extern PROC proc[NPROC];
extern PROC *running, *freeList, *readyQueue, *sleepList;
extern int nproc;
extern int printQueue(char* name, PROC* queue);
extern PROC *kfork(char* filename);

#endif
