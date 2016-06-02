#ifndef que
#define que

#include <stdio.h>
#include"type.h"



PROC* get_proc(PROC **list)
{
	PROC* elt=*list;
	*list=(*list)->next;	
	return elt;

}


void enqueue(PROC **queue,PROC *p)
{		

	if(*queue==NULL)
	{
		PROC *top=(*queue);
		*queue=p;
		p->next=top;
		return;
	}
	else
	{	
		PROC *x=(*queue);
		PROC *y=x->next;
		while(y!=NULL)
		{ 
			x=x->next;		
			y=y->next;
		}
		x->next=p;	
		p->next=y;	
		return;
	}

}
PROC* dequeue(PROC **queue)
{
	PROC* top=(*queue);
	*queue=(*queue)->next;
	return top;

}

#endif
