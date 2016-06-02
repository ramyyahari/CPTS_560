#define PA 9
#include"type.h"

int kcinth()
{  
u16 segment, offset; 
int a, b, c, d, r;
segment = running->uss; offset = running->usp;
/* get syscall parameters from ustack */
a = get_word(segment, offset + 2*PA);
b = get_word(segment, offset + 2*(PA+1));
c = get_word(segment, offset + 2*(PA+2));
d = get_word(segment, offset + 2*(PA+3));
/* route syscall call to kernel functions by call# a */
switch(a){
case 0 : r = getpid();    break;
case 1 : r = do_ps();       break;
case 2 : r = chname(b);  break;
case 3 : r = kfork();    break;
case 4 : r = tswitch();   break;
case 5 : r = wait(b);    break;
case 6 :     kexit(b);    break;
case 8: r=getc();break;
case 9: putc(b); break;
default: printf("invalid syscall %d\n", a); 
}
return r;
}
