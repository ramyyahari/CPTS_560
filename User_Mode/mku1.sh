#!/bin/bash

as86 -o u.o u.s
bcc -c -ansi u1.c
ld86 -o u1 u.o u1.o mtxlib /usr/lib/bcc/libc.a

sudo mount -o loop mtximage /mnt
sudo cp u1 /mnt/bin/u1
sudo umount /mnt

rm *.o u1
echo "done u1"

