void chmod_file()
{
INODE *temp;
MINODE *mino;
int dev=root->dev,ino;
ino=getino(dev,pathname);
mino=iget(dev,ino);
temp=&mino->INODE;
__u16 mode=0x0000;
	
sscanf(parameter, "%x", &mode);
printf("a:%x",0xffff&(__u16)mode);
	
printf("\n0x%x:",0xffff&(__u16)(temp->i_mode));
	temp->i_mode = ( (temp->i_mode) & (~0xfff) ) | ( 0xfff & mode );
printf("\n0x%x",0xffff&(__u16)(temp->i_mode));
				mino->dirty=1;
iput(mino);
return;
}

void touch()
{
int ino;
INODE *temp;
MINODE *mino;
time_t t;
int dev=root->dev;
ino=getino(dev,pathname);
mino=iget(dev,ino);
temp=&mino->INODE;
ctime(&t);
temp->i_atime=ctime(&t);
temp->i_ctime=ctime(&t);
printf("\ntime is:%s",temp->i_atime);			
mino->dirty=1;
iput(mino);
}

void mystat()
{
int ino;
INODE *temp;
MINODE *mino;
//stat *st;
int dev=root->dev;
ino=getino(dev,pathname);
mino=iget(dev,ino);
temp=&mino->INODE;
/*st.st_dev=dev;
st.st_ino=ino;
st.st_mode=temp->i_mode;
st.st_uid=temp->i_uid;
st.st_gid=temp->i_gid;
st.st_size=temp->i_size;
st.st_blocks=temp->i_blocks;
st.st_nlink=temp->i_links_count;
st.st_atim=temp->i_atime;
st.st_mtim=temp->i_mtime;
st.st_ctim=temp->i_ctime;
//st(pathname,&st);*/
printf("\ndev:%d",dev);
printf("\nino is:%d",ino);
printf("\nmode is:%d",temp->i_mode);
printf("\nsize is:%d",temp->i_size);
printf("\nblocks is:%d",temp->i_blocks);
printf("\nnlink is:%d",temp->i_links_count);
printf("\ntime is:%d",temp->i_atime);
iput(minode);

}


