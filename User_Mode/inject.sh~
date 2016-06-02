#!/bin/bash


dd if=/dev/zero of=fdimage bs=512 count=2880
mkfs.ext2 fdimage -F
dd if=FDimage of=fdimage bs=512 count=2880 conv=notrunc
