
int creat_file()
{
	int ino;
	INODE *temp;
	MINODE *mtemp;
	time_t t;
	int i=1,dev; 
	char* cp,*parent,*child;
    char buf[BLKSIZE],tem[256];
	strcpy(tem,pathname);

	parent=dirname(pathname);
	child=basename(tem);	
		
	if(pathname[0]=='/')
	{
		dev=root->dev;
			
	}
	else
		dev=running->cwd->dev;
		
	printf("\nparent:%s\nchild:%s",parent,child);
		
	ino=getino(dev,parent);
	mtemp=iget(dev,ino);
	temp=&mtemp->INODE;
	/*i=check_list(parent,child);
	if(i)
		printf("\ncan create");		
	else 
		printf("\ncant create");		
*/
	if((temp->i_mode&DIR_MODE==DIR_MODE)&&i)
		my_creat(mtemp,child,dev);
	
	temp->i_links_count++;
	temp->i_atime=ctime(&t);
	mtemp->dirty=1;
	iput(mtemp);
return 0;
} 


int my_creat(MINODE *mtemp, char *name,int dev)
{
int ino,bno,i;
MINODE *temp;
INODE *itemp;

char buf[BLKSIZE];
	
	ino = ialloc(dev);    
        bno = balloc(dev);
		printf("\nino1:%d",ino);

		printf("\nbno1:%d",bno);
	if(!ino)
		printf("ino not allocated");
	
	if(!bno)
		printf("bno not allocated");	
	temp = iget(dev, ino);
	itemp=&temp->INODE;

	itemp->i_mode=0X81A4;
	itemp->i_uid  = running->uid;	 
  	itemp->i_gid  = running->gid;	
  	itemp->i_size = 0;		 
  	itemp->i_links_count = 1;
  	itemp->i_atime =time(0L);
	itemp->i_ctime = time(0L);
	itemp->i_mtime = time(0L);  // set to current time
  	itemp->i_blocks = 2;                	// LINUX: Blocks count in 512-byte chunks 
  	for(i=1;i<15;i++)
		itemp->i_block[i] =0; 	
	itemp->i_block[0] = bno;             // new DIR has one data block   
	
  	temp->dirty = 1;               // mark minode dirty
	
	iput(temp);

	DIR *dp=(DIR*)buf;
	dp->inode=ino;
	dp->rec_len=12;	
	dp->name_len=1;
	dp->file_type=1;
	
	put_block(dev,bno,buf);
    enter_name(mtemp, ino, name);
return 0;
}  

