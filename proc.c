#ifndef procs
#define procs

#include<stdio.h>

int kexit(int exitValue)
{
	int i, wakeupP1 = 0;
	PROC *p;
	if (running->pid==1&&nproc>0){ // nproc = number of active PROCs
		printf("other procs still exist, P1 can't die yet\n");
		return;
	}
	/* send children (dead or alive) to P1's orphanage */
	for (i = 1; i < NPROC; i++)
	{
		p = &proc[i];
		if (p->status != FREE && p->ppid == running->pid)
		{
			p->ppid = 1;
			p->parent = &proc[1];
			wakeupP1++;
		}
		
	}
	/* record exitValue and become a ZOMBIE */
	running->exitCode = exitValue;
	running->status = ZOMBIE;
	/* wakeup parent and also P1 if necessary */
	if (wakeupP1)
	{
		for (i = 1; i < NPROC; i++)
		{
			p = &proc[i];
			if (p->pid == 1&&p->status==SLEEP)
			{
				p->status=READY;
			}
			if(p->pid==running->ppid&&p->status==SLEEP)
			{
				p->status = READY; // make it ready to run again
				enqueue(&readyQueue, p);
			}
		}	
	}
	tswitch();
	// give up CPU
}

int ksleep(int event)
{
	running->event = event; // record event in PROC.event
	running->status = SLEEP; // change status to SLEEP
	enqueue(&sleepList,running);	
	tswitch();
	// give up CPU
}

int kwakeup(int event)
{
	int i; 
	PROC *p;
	for (i=1; i<NPROC; i++)
	{
		// not applicable to P0
		p = &proc[i];
		if (p->status == SLEEP && p->event==event)
		{
			p->event = 0;
			// cancel PROC’s event
			p->status = READY; // make it ready to run again
			enqueue(&readyQueue, p);	
			}
	}
		
}


int kwait(int status) // wait for ZOMBIE child
{
	PROC *p; 
	int i, hasChild = 0;
	while(1){
	// search PROCs for a child
		for (i=1; i<NPROC; i++){
		// exclude P0
			p = &proc[i];
			if (p->status != FREE && p->ppid == running->pid){
				hasChild = 1;
				// has child flag
				if (p->status == ZOMBIE){
				// lay the dead child to rest
					running->status = p->exitCode;
					// collect its exitCode
					p->status = FREE;
					// free its PROC
					enqueue(&freeList, p); // to freeList
					nproc--;
					// once less processes
					return(p->pid);
					// return its pid
					}
				}
			}
		if (!hasChild) 
			return -1;
		// no child, return ERROR
		ksleep(running->pid); // still has kids alive: sleep on PROC address
	}
}

#endif
