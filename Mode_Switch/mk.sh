
as86 -o ts.o ts.s
bcc -c -ansi t.c 
ld86 -d -o mtx ts.o t.o OBJ/forkexec.o OBJ/wait.o mtxlib /usr/lib/bcc/libc.a

echo "done build 2!"

sudo mount -o loop fdimage /mnt
sudo cp mtx /mnt/boot
sudo umount /mnt
sudo rm mtx

echo "done copy!" 

(cd User;sh mkallu.sh)


echo "done and done!"

