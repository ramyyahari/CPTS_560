/*************** type.h file *****************/
#define NPROC    9
#define SSIZE 1024
#define FREE     0
#define READY    1
#define RUNNING  2     // for clarity only, not needed or used
#define STOPPED  3
#define SLEEP    4
#define ZOMBIE   5
typedef struct proc{
struct proc *next;
int    *ksp;
int    pid;            // process ID number
int    status;         // status = FREE|READY|RUNNING|SLEEP|ZOMBIE  
int    ppid;           // parent pid
struct proc *parent;   // pointer to parent PROC
int    priority;
int    event;          // sleep event
int    exitCode;       // exit code
int    kstack[SSIZE];      
}PROC;


PROC proc[NPROC], *running, *freeList, *readyQueue,*sleepList;


int body(void);
PROC* get_proc(PROC **list);
void enqueue(PROC **queue,PROC *p);
PROC* dequeue(PROC **queue);

int kexit(int exitValue);
int ksleep(int event);
int kwakeup(int event);
int kwait(int status);
