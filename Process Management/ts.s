#-------------- ts.s file --------------------
.global tswitch, running, scheduler # no underscore prefix


tswitch: 

SAVE:      pushal                   
	   pushfl
           movl   running,%ebx
           movl   %esp, 4(%ebx)  # integers in GCC are 4 bytes

FIND:      call   scheduler

RESUME:    movl   running, %ebx
	   movl   4(%ebx), %esp    
           popfl
           popal
           ret
