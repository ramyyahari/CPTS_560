#!/bin/bash


dd if=/dev/zero of=mtximage2 bs=512 count=2880
mkfs.ext2 fdimage -F
dd if=mtximage of=mtximage2 bs=512 count=2880 conv=notrunc
