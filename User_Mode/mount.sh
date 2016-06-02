#!/bin/bash

sudo mount -o loop fdimage /mnt
sudo cp mtx /mnt/bin/mtx
sudo umount /mnt

