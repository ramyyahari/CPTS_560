

int tst_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; 
  j=bit%8;
  if (buf[i] & (1 << j))
     return 1;
  return 0;
}

int set_bit(char *buf, int bit)
{
  int i, j;

  i = bit/8; j=bit%8;
  buf[i] |= (1 << j);
}

int clr_bit(char *buf, int bit)
{
  int i, j;
  i = bit/8; j=bit%8;
  buf[i] &= ~(1 << j);
}

int decFreeInodes(int dev)
{
  char buf[BLKSIZE];

  // dec free inodes count in SUPER and GD
  get_block(dev, 1, buf);
  sp = (SUPER *)buf;
  sp->s_free_inodes_count--;
  put_block(dev, 1, buf);

  get_block(dev, 2, buf);
  gp = (GD *)buf;
  gp->bg_free_inodes_count--;
  put_block(dev, 2, buf);
}


int incFreeInodes(int dev)
{
  char buf[BLKSIZE];

  // dec free inodes count in SUPER and GD
  get_block(dev, 1, buf);
  sp = (SUPER *)buf;
  sp->s_free_inodes_count++;
  put_block(dev, 1, buf);

  get_block(dev, 2, buf);
  gp = (GD *)buf;
  gp->bg_free_inodes_count++;
  put_block(dev, 2, buf);
}


int decFreeBlocks(int dev)
{
  char buf[BLKSIZE];

  // dec free inodes count in SUPER and GD
  get_block(dev, 1, buf);
  sp = (SUPER *)buf;
  sp->s_free_blocks_count--;
  put_block(dev, 1, buf);

  get_block(dev, 2, buf);
  gp = (GD *)buf;
  gp->bg_free_blocks_count--;
  put_block(dev, 2, buf);
}


int ialloc(int dev)
{
  int  i,imap,ninodes;
  char buf[BLKSIZE];
    //imap = root->mountptr->imap;
    ninodes = root->mountptr->ninodes;
		
  // read inode_bitmap block
  get_block(dev, 9, buf);

  for (i=0; i < ninodes; i++){
    if (tst_bit(buf, i)==0){
       set_bit(buf,i);
       decFreeInodes(dev);
	printf("ino allocated is:%d\n",i+1);

       put_block(dev,9, buf);

       return i+1;
    }
  }
  printf("ialloc(): no more free inodes\n");
  return 0;
}

int balloc(int dev)
{
  int  i,nblocks;
  char buf[BLKSIZE];
  // read inode_bitmap block
 
   // bmap = root->mountptr->bmap;
    nblocks = root->mountptr->nblocks;

  get_block(dev,8, buf);

  for (i=0; i < nblocks; i++){
    if (tst_bit(buf, i)==0)
	{
       set_bit(buf,i);
       decFreeBlocks(dev);
	printf("bno allocated is:%d\n",i+1);
       put_block(dev,8, buf);

       return i+1;
    }
  }
  printf("balloc(): no more free inodes\n");
  return 0;
}

int bdealloc(int dev,int ino)
{
  
  char buf[BLKSIZE];
  get_block(dev, 8, buf);

       set_bit(buf,ino);
       incFreeInodes(dev);
       put_block(dev, 8, buf);
  return 0;
}

int idealloc(int dev,int ino)
{
  
  char buf[BLKSIZE];
  get_block(dev,9, buf);

       set_bit(buf,ino);
       incFreeInodes(dev);
       put_block(dev, 9, buf);
  return 0;
}

int midealloc(MINODE *mip)
{
  mip->refCount = 0;
}

MINODE* mialloc()
{
  int  i;
  MINODE* temp;	

  for (i=0; i < NMINODES; i++){
	temp=&minode[i];
	if (temp->refCount==0){
		temp->refCount++;
		return temp;       	
	}

	}
  printf("mialloc(): no more free inodes\n");
  return 0;

}

