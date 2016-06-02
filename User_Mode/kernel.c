#include"type.h"

PROC * kfork(char *filename)
{
	int i=1;
	PROC *p = get_proc(&freeList);
	u16 segment;
	if (!p)
	{
		printf("no more PROC, kfork() failed\n");	
		return 0;
	}

	/* initialize the new proc and kstack */
	p->status = READY;
	p->ppid = running->pid;
	p->priority = 1;                     // priority = 1
	p->kstack[SSIZE-1] = (int)body;      // start to run body()  
	p->ksp = &(p->kstack[SSIZE-9]);    // SEE REQUIREMENTS BELOW
	enqueue(&readyQueue,p);

	segment=0x2000+(p->pid-1)*0x0800;

	load("/bin/u1",segment);
	printf("Loaded segment");  

	while(i<9)
	{
		u16 store;
		if(i==1)
			store=0x0200;
		if(i==2||i==7||i==8)
			store=segment;
		else 
			store=0;
	}
	put_word(store,segment,-(i*2));
	i++;
	}
	p->uss=segment;
	p->usp=-8*2;
	return(p->pid);
}
int body()
{
  char c;
  printf("proc %d resumes to body()\n", running->pid);
  while(1){
    printf("-----------------------------------------\n");
    print freeList
    print readyQueue
    printf("-----------------------------------------\n");

    printf("proc %d running: parent = %d  enter a char [s|f|w|q|u] : ", 
	   running->pid, running->ppid);
    c = getc(); printf("%c\n", c);
    switch(c){
       //----------------------------------------
       case 's' : tswitch();   break;
       case 'f' : kfork();     break;
       case 'w' : wait();      break;
       case 'q' : exit();      break;
    //--- above commands are same as in LAB#2. The 'u' command is NEW -------

       case 'u' : goUmode();      break; // <=== go back to Umode
    }
  }
}

int do_ps()
{
 int i=0;	
 PROC* p = &proc[i];

	for (i = 0; i < NPROC; ++i)
	{
			
	printf("pid=%d ppid=%d status=", p->pid, p->ppid);
	switch(p->status) 
	{
		case RUNNING: printf("RUNNING");             break;
		case READY:   printf("READY");               break;
		case SLEEP:   printf("SLEEP on %d", p->event); break;
		case ZOMBIE:  printf("ZOMBIE");              break;
		case FREE:    printf("FREE");                break;
		default:      printf("ERROR");               break;
	}
		
	printf(" PName%s\n", p->name);
}
	
printf("\n");
return 1;

}
int chname(char *newname)
{
	for (i = 0; i < 32; ++i)  // size of name
	{
		running->name[i] = get_byte(running->uss, newname+i);
		if (running->name[i] == '\0')
			break;
		else 
			printf("%c", running->name[i]);
	}
	running->name[i]='\0';


}


