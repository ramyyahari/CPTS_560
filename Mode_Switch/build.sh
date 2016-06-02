#!/bin/bash


as86 -o ts.o ts.s
bcc -c -ansi t.c int.c queue.c wait.c kernel.c fork-exec.c 
ld86 -d -o mtx ts.o t.o OBJ/forkexec.o OBJ/wait.o mtxlib /usr/lib/bcc/libc.a

echo "done!"
