
void list_dir()
{
	int ino;
	INODE *temp,*temp2;
	MINODE *mtemp,*mtemp2;
	int i,dev; 
	char* cp;
        char buf[BLKSIZE];
	if(pathname[0])
	{
		if(pathname[0]=='/')
			dev=root->dev;
		else
			dev = running->cwd->dev;
		ino=getino(dev,pathname);		
		if(!ino)
			printf("\ndirectory doesn't exist");
			
		mtemp=iget(dev,ino);
		temp=&mtemp->INODE;
		//printf("\nPath name is:%s",mtemp->name);		
		if(temp->i_mode&DIR_MODE!=DIR_MODE)
			printf("\nnot a directory");
			
	}	
	else
	{
		temp=&(running->cwd->INODE);
		dev= running->cwd->dev;
		printf("\nPath name is:%s",running->cwd->name);		
		
	}

	for(i=0;i<12;i++)
	{	
		iblock=temp->i_block[i];
		if(!iblock)
		{
			return;
		}
		else
		{        
			get_block(dev,iblock,buf);	
			dp = (DIR *)buf;
			cp = (char *)buf;
			while(dp->rec_len&&*cp<BLKSIZE+buf&&*cp!=NULL)	
			{
				char buf2[256];
				int inodeOffset = (mounttab[0].iblock)*BLOCK_SIZE + (dp->inode-1)*sizeof(INODE);
				pread(root->dev, buf2, sizeof(INODE), inodeOffset);
				temp2 = (INODE*)buf2;
printf("\n0x%x\t%d\t%s",0xffff&(__u16)(temp2->i_mode),dp->inode,dp->name);
				cp+=dp->rec_len; 
				dp =(DIR*)cp;
			}	
		}
	}
}

int check_list(char *parent,char* child)
{
	int ino;
	INODE *temp;
	MINODE *mtemp;
	int i,dev,flag=1; 
	char* cp;
        char buf[BLKSIZE];
	if(parent[0])
	{
		if(parent[0]=='/')
			dev=root->dev;		
		else
			dev = running->cwd->dev;
		ino=getino(dev,parent);		
		if(!ino)
			printf("\ndirectory doesn't exist");	
		mtemp=iget(dev,ino);
		temp=&mtemp->INODE;
	}	
	
	for(i=0;i<12;i++)
	{	
		iblock=temp->i_block[i];
		if(!iblock)
		{
			return 0;
		}
		else
		{        
			get_block(dev,iblock,buf);	
			dp = (DIR *)buf;
			cp = (char *)buf;
			//put_block(dev, iblock, buf);
			while(dp->rec_len&&(cp<BLKSIZE+buf))//&&(*cp!=NULL))	
			{
				if(strstr(dp->name,child))
					flag=0;			
				cp+=dp->rec_len; 
				dp =(DIR*)cp;
			}	
		}
	}

return 0;
}


void make_dir()
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
	printf("\nparent:%s\nchild:%s",parent,child);
	
	if(tem[0]=='/')
	{
		dev=root->dev;
		//parent.substr(1,strlen(parent));	
	}
	else
		dev=running->cwd->dev;
	
	printf("\ndev:%d",dev);	

	ino=getino(dev,parent);

	//i=strcmp(parent,".");
	/*if(ino!=root->ino)
		ino=running->cwd->ino;	
*/
	mtemp=iget(dev,ino);
	temp=&mtemp->INODE;
	/*i=check_list(parent,child);
	if(i)
		printf("\ncan create");		
	else 
		printf("\ncant create");*/		
	if(temp->i_mode&DIR_MODE==DIR_MODE)
	{
	mymkdir(mtemp,child,dev);
	printf("\nmkdir happening");	

	temp->i_links_count++;
	temp->i_atime=ctime(&t);
	}
	mtemp->dirty=1;
	iput(mtemp);	
}



int mymkdir(MINODE *mtemp, char *name,int dev)
{
int ino,bno,i;
MINODE *temp;
INODE *itemp;

char buf[BLKSIZE];
	
	ino = ialloc(dev);    
        bno = balloc(dev);
	if(!ino)
		printf("\nino not allocated");
	
	if(!bno)
		printf("\nbno not allocated");	
	
	printf("\ndev:%d",dev);

	temp = iget(dev, ino);
	itemp=&temp->INODE;

	itemp->i_mode=DIR_MODE;
	itemp->i_uid  = running->uid;	// Owner uid 
  	itemp->i_gid  = running->gid;	// Group Id
  	itemp->i_size = BLKSIZE;		// Size in bytes 
  	itemp->i_links_count = 2;	        // Links count=2 because of . and ..
  	itemp->i_atime =time(0L);
	itemp->i_ctime = time(0L);
	itemp->i_mtime = time(0L);  // set to current time
  	itemp->i_blocks = 2;                	// LINUX: Blocks count in 512-byte chunks 
 	itemp->i_block[0] = bno;             // new DIR has one data block   
	 	for(i=1;i<14;i++)
		itemp->i_block[i] =0; 	
	temp->dirty = 1;               // mark minode dirty
	
	iput(temp);
	
	DIR *dp=(DIR*)buf;
	dp->inode=ino;
	dp->rec_len=12;	
	dp->name_len=1;
	dp->file_type=2;
	strcpy(dp->name,".");
	dp=(DIR*)((int)dp+dp->rec_len);
	dp->inode=mtemp->ino;
	dp->rec_len=1012;
	dp->name_len=2;
	dp->file_type=2;
	strcpy(dp->name,"..");

	put_block(dev,bno,buf);
	
    enter_name(mtemp, ino, name);

return 0;
}

int enter_name(MINODE *pip, int myino, char *myname)
{	
	char buf[BLKSIZE],buf2[BLKSIZE];
	INODE *temp;
	int rem,i=0,bno,bno2,dev,needlen,len,j;
	dev=pip->dev;	
	len=strlen(myname);
	needlen=4*((8 +len+ 3)/4);
		
	//MINODE *t=iget(dev,myino);
	temp=&pip->INODE;

	while(temp->i_block[i]!=0)
	{	
		i++;
	}

	bno=temp->i_block[i-1];
	printf("\nbno:%d",bno);
	printf("\ni:%d",i);
	
	get_block(dev,bno,buf);	

	DIR *dp = (DIR *)buf;
	//printf("\ninode:%d",dp->inode);
	
	if(dp->inode==0)
	{
		dp->inode=myino;				
		dp->rec_len=1024;
		dp->name_len=len;
		strcpy(dp->name,myname);		
		put_block(dev,bno,buf);		
	}	
	else
	{
		while ((dp+dp->rec_len) <buf + BLKSIZE)
		{
			dp = (DIR *)(dp+dp->rec_len);
		} 
		
		int ideal=4*((8 + dp->name_len + 3)/4);
		rem=dp->rec_len-ideal;
		if(rem>=needlen)
		{
			dp->rec_len=ideal;
			dp=(DIR*)((int)dp+dp->rec_len);
			dp->inode=myino;				
			dp->rec_len=rem;
			dp->name_len=strlen(myname);
			strcpy(dp->name,myname);		
			put_block(dev,bno,buf);			
			
		}
		else				
		{

			printf("\nCreating a new block,space in old block=%d",rem-needlen);
			bno2=balloc(dev);						
			get_block(dev,bno2,buf2);
			temp->i_block[i]=bno2;
			dp=(DIR*)buf2;				
			dp->inode=myino;				
			dp->rec_len=1024;
			dp->name_len=strlen(myname);
			strcpy(dp->name,myname);		
			put_block(dev,bno2,buf2);
			temp->i_size+=1024;
			//pip->dirty=1;
			//iput(pip);		
		}
	}
printf("\nentername");	
}

void change_dir()
{
	int ino;
	INODE *temp;
	MINODE *mtemp;
	int i,dev; char* cp;
        char buf[BLKSIZE];
	if(pathname[0])
	{
		if(pathname[0]=='/')
			dev=root->dev;		
		else
			dev = running->cwd->dev;
		ino=getino(dev,pathname);		
		if(!ino)
			printf("\ndirectory doesn't exist");	
		mtemp=iget(dev,ino);
		temp=&mtemp->INODE;	
		running->cwd=mtemp;
	}	
	else
	{
		temp=&(running->cwd->INODE);
		dev= running->cwd->dev;
	}

	if(!strcmp(pathname,".."))
		running->cwd->refCount--;
	else
		running->cwd->refCount++;
	iput(mtemp);
}


void my_rpwd(MINODE *mip) {
	
	if(mip == root) {
		return; 
	} 
	else {
		MINODE *temp;
		//INODE *itemp;		
		temp=iget(mip->dev,mip->ino);
		//itemp=&temp->INODE;	
		my_rpwd(temp);
		
		printf("/%s", temp->name);

		return;
	}

}




void pwd() {
	
	if(running->cwd == root) {
		printf("/");
	} else {
		my_rpwd(running->cwd);
	}
	printf("\n");
}



void rmdir()
{


	int ino,pino;
	INODE *temp;
	MINODE *mtemp,*pip;
	int i,dev=root->dev; 
    char* cp;
    char buf[BLKSIZE],tem[256];

	ino=getino(dev,pathname);
	mtemp=iget(dev,ino);
	temp=&mtemp->INODE;

		
	if(temp->i_mode&DIR_MODE!=DIR_MODE)
	{
		printf("\nnot a directory ");			
		return;
	}
	/*if(running->status == BUSY)
	{
		printf("\nDIRECTORY IN USE");			
		return;
	}*/
		printf("\nchk1 ");			

	for (i=0; i<12; i++)
	{

         if (temp->i_block[i]!=0)
	         bdealloc(mtemp->dev, temp->i_block[i]);
	    	 
		else
		     	break;
		   
	} 

		 idealloc(mtemp->dev, mtemp->ino);
		 iput(mtemp);
	     findino(mtemp, &ino,&pino);


	     pip = iget(dev, pino);

	     rm_child(pip, basename(pathname));

	     pip->INODE.i_atime = pip->INODE.i_mtime = time(0L);

	     pip->dirty = 1;
	     iput(pip);
	     return;

}
	
int rm_child(MINODE * pip, char *myname)
{
	int i=0;
	DIR * dp, *next, *last;
	char * cp, * np;
	int del;
	char name[256],buf[1024];

	if(strncmp(myname, ".", 1)==0 || strncmp(myname, "..", 2)==0)
		return 0;
	
	while(i<12)
	{

		if(pip->INODE.i_block[i] != 0)
		{
			
			get_block(root->dev, pip->INODE.i_block[i], buf);
			dp = (DIR *)buf;
			cp = buf;

			while((cp < buf + BLOCK_SIZE)&&dp->rec_len)
			{
				strcpy(name, dp->name);
				name[dp->name_len] = '\0';

				if(strcmp(dp->name, myname)==0)
				{
						del = dp->rec_len;
						if(del == BLOCK_SIZE)
						{
							bdealloc(root->dev, pip->INODE.i_block[i]);
							pip->INODE.i_size -= BLOCK_SIZE;
							pip->INODE.i_block[i] = 0;
							return 0;
						}
						break;
				}
				last = (DIR *)cp;

				cp += dp->rec_len;
				dp = (DIR *)cp;
				np = cp + dp->rec_len;
				next = (DIR *)np;
			}

			while(dp->rec_len&&(np < buf + BLOCK_SIZE))
			{
				memcpy(dp, np, next->rec_len);
				last = (DIR *)cp;

				cp += dp->rec_len;

				dp = (DIR *)cp;

				np = cp + del;
				next = (DIR *)np;
			}		

			last->rec_len += del;
			put_block(root->dev, pip->INODE.i_block[i], buf);
		}
		i++;
	}
	return 0;
}