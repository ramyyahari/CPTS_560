/*********** t.c file for multitasking under Linux ***********/
#include <stdio.h>
#include <stdlib.h> // Linux header files
#include "type.h"   // PROC struct type, same as in MTX4.1

int nproc;

#include "queue.c"  // getproc(),enqueue(),dequeue(); same as in MTX4.1 
#include "proc.c"
#define NPROC 9


int set_vec(u16 vector, u16 handler)
{
	put_word(handler, 0, vector<<2);
     put_word(0x1000,  0, (vector<<2) + 2);
}           

void printQueue(PROC* queue)
{
    PROC* p = queue;
    while(p)
    {	
    printf("%d[%d] --> ", p->pid,p->ppid);
    if(p->pid>8)
	return;    
	p = p->next;
    }
 }

void print()
{
  int i;	
  PROC *p;	
  printf("Running:[%d]", running->pid);         
  printf("\nReady Queue:");    printQueue(readyQueue);
  printf("\nFreeList:");       printQueue(freeList);   
  printf("\nSleepList:");
  for(i=1; i<NPROC; i++)
  {
	// exclude P0
	p = &proc[i];
	if(p->status==SLEEP)
		printf("%d[%d]->",p->pid,p->ppid);		
  }
}

int body()
{ 
char c;
int event;
while(1){
	printf("\n***************************************\n");
	print();
	printf("\nI am task %d My parent=%d\n", running->pid, running->ppid);
	printf("input a char [fork:f|switch:s|exit:q|sleep:z|wakeup:a|wait:w] : ");  
	c = getchar();
	switch(c){
		case 'f': kfork();      break;
		case 's': tswitch();    break;
		case 'q': kexit(0); 	break;
		case 'z': {printf("enter event to put process to sleep");
			  scanf("%d",&event);		
			  ksleep(event);}	
			  break;
		case 'a': {printf("enter event to wake up process");
			  scanf("%d",&event);		
			  kwakeup(event);}	
			  break;
		case 'w': kwait(0); 	break;
		default: printf("invalid char\n"); break;
		}	
	}
return;
}

int init()
{

PROC *p; 
int i;
for (i=0; i<=NPROC; i++)
{
	p = &proc[i];
	p->pid = i;
	p->status = FREE;
	p->priority = 0; 
	str	
	p->next = &proc[i+1];    
	
}
p->next=NULL;

printf("\n");
proc[NPROC-1].next = NULL; // freeList
freeList=&proc[0]; 

readyQueue = NULL;
sleepList=NULL;
/**** create P0 as running ******/
p = get_proc(&freeList); // get PROC from freeList

p->ppid=0;
p->status = READY;
running = p;
nproc=1;
} 

int scheduler()
{ 
if (running->status == READY)
	enqueue(&readyQueue, running);
running = dequeue(&readyQueue);
}

int main()
{
printf("init\n");
init();

printQueue(freeList);
printf("\nkfork\n");
kfork("/bin/u1");   // create P1
printf("\nP0 switch to P1\n");

while(1)
{
	if(readyQueue)
		tswitch();
}
}
