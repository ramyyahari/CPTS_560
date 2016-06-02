
typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define NULL     0
#define NPROC    17
#define SSIZE 1024

//******* PROC status ********
#define FREE     0
#define READY    1
#define RUNNING  2
#define STOPPED  3
#define SLEEP    4
#define ZOMBIE   5

typedef struct proc{
    struct proc *next;
    int    *ksp;

    int    uss, usp;
    int    pid;                // add pid for identify the proc
    int    status;             // status = FREE|READY|RUNNING|SLEEP|ZOMBIE    
    int    ppid;               // parent pid
    struct proc *parent;
    int    priority;
    int    event;
    int    exitCode;

    char   name[32];
    int    kstack[SSIZE];      // per proc stack area
}PROC;

PROC proc[NPROC], *running, *freeList, *readyQueue, *sleepList;

int kcinth();
int kps();
PROC *kfork(char *filename);
int do_ps();
int chname(char* b);
int printQueue(char* name, PROC* queue);
PROC *get_proc();
enqueue(PROC **queue, PROC *ptr);
PROC *dequeue(PROC **queue);

int sleep(int event);
int wakeup(int event);
int exit(int exitValue);
int wait(int *status);




