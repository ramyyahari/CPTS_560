#include "type.h"

/************************ globals *****************************/
MINODE *root; 
char pathname[128], parameter[128], *name[128], cwdname[128];
char names[128][256];

int  nnames;
char *rootdev = "disk", *slash = "/", *dot = ".";
int iblock;
int fd;
int imap, bmap;  // IMAP and BMAP block number
int ninodes, nblocks, nfreeInodes, nfreeBlocks;

MINODE minode[NMINODES];
MOUNT  mounttab[NMOUNT];
PROC   proc[NPROC], *running;
OFT    oft[NOFT];

#include "a_d.c"   
#include "dir.c"
#include "file.c"
#include "other.c"
#include "link.c"

MOUNT *getmountp();

char *cmds[] = { "mkdir", "rmdir","ls",  "cd", "pwd", "creat", "link", "unlink", "symlink", "stat", "chmod", "touch","quit","rm"}; 

int DEBUG=0;
int nproc=0;

int findCmd(char *name)
{
   int i=0;   
   char *p=cmds[0];

   while (p){
         if (strcmp(p, name)==0)
            return i;
         i++;  
         p = cmds[i];
   } 
   return(-1);
}


mountroot()   /* mount root file system */
{
  int i, ino, fd, dev;
  MOUNT *mp;
  SUPER *sp;
  MINODE *ip;

  char line[64], buf[BLOCK_SIZE], *rootdev;
  
  printf("enter rootdev name (RETURN for disk) : ");
  gets(line);
  rootdev = "disk";

  if (line[0] != 0)
     rootdev = line;

  dev = open(rootdev, O_RDWR);
  if (dev < 0){
     printf("panic : can't open root device\n");
     exit(1);
  }

  /* get super block of rootdev */
  get_block(dev, 1, buf);
  sp = (SUPER *)buf;

  /* check magic number */
  printf("SUPER magic=0x%x  ", sp->s_magic);
  if (sp->s_magic != SUPER_MAGIC){
     printf("super magic=%x : %s is not a valid Ext2 filesys\n",
             sp->s_magic, rootdev);
     exit(0);
  }

  mp = &mounttab[0];      /* use mounttab[0] */

  /* copy super block info to mounttab[0] */
  ninodes = mp->ninodes = sp->s_inodes_count;
  nblocks = mp->nblocks = sp->s_blocks_count;
  
  nfreeBlocks = sp->s_free_blocks_count;
  nfreeInodes = sp->s_free_inodes_count;

  get_block(dev, 2, buf);
  gp = (GD *)buf;

  mp->dev = dev;         
  mp->busy = BUSY;

  bmap= mp->bmap = gp->bg_block_bitmap;
  imap= mp->imap = gp->bg_inode_bitmap;
  mp->iblock = gp->bg_inode_table;

  strcpy(mp->name, rootdev);
  strcpy(mp->mount_name, "/");


  printf("bmap=%d  ",   gp->bg_block_bitmap);
  printf("imap=%d  ",   gp->bg_inode_bitmap);
  printf("iblock=%d\n", gp->bg_inode_table);  


  /***** call iget(), which inc the Minode's refCount ****/

  root = iget(dev, 2);          /* get root inode */
  mp->mounted_inode = root;
  root->mountptr = mp;

  printf("mount : %s  mounted on / \n", rootdev);
  printf("nblocks=%d  bfree=%d   ninodes=%d  ifree=%d\n",
	  nblocks, nfreeBlocks, ninodes, nfreeInodes);

  return(0);
} 

init()
{
  int i, j;
  PROC *p;

  for (i=0; i<NMINODES; i++)
      minode[i].refCount = 0;

  for (i=0; i<NMOUNT; i++)
      mounttab[i].busy = 0;

  for (i=0; i<NPROC; i++){
      proc[i].status = FREE;
      for (j=0; j<NFD; j++)
          proc[i].fd[j] = 0;
      proc[i].next = &proc[i+1];
  }

  for (i=0; i<NOFT; i++)
      oft[i].refCount = 0;

  printf("mounting root\n");
    mountroot();
  printf("mounted root\n");

  printf("creating P0, P1\n");
  p = running = &proc[0];
  p->status = BUSY;
  p->uid = 0; 
  p->pid = p->ppid = p->gid = 0;
  p->parent = p->sibling = p;
  p->child = 0;
  p->cwd = root;
  p->cwd->refCount++;

  p = &proc[1];
  p->next = &proc[0];
  p->status = BUSY;
  p->uid = 2; 
  p->pid = 1;
  p->ppid = p->gid = 0;
  p->cwd = root;
  p->cwd->refCount++;
  
  nproc = 2;
}

int quit()
{
  // write YOUR quit function here
   exit(0);
} 

main(int argc, char *argv[ ]) 
{
  int i,cmd; 
  char line[128], cname[64];

  if (argc>1){
    if (strcmp(argv[1], "-d")==0)
        DEBUG = 1;
  }

  init();

  while(1){
      printf("\nP%d running: ", running->pid);
      /* zero out pathname, parameter */
      for (i=0; i<64; i++){
          pathname[i]=parameter[i] = 0;
      }      
      /* these do the same; set the strings to 0 */
      memset(pathname, 0, 64);
      memset(parameter,0, 64);

      printf("\ninput command : ");
      gets(line);
	if (line[0]==0) continue;

      sscanf(line, "%s %s %64c", cname, pathname, parameter);      
	cmd = findCmd(cname);
	switch(cmd){
           case 0 : make_dir();               break;//
           case 1 : rmdir();	                break;//
           case 2 : list_dir();               break;//
           case 3 : change_dir();             break;//
           case 4 : pwd();		                break;
           case 5 : creat_file();             break;//
           case 6 : link();                   break;//
           case 7 : unlink();                 break;//
           case 8 : symlink();                break;//
           case 9 : mystat();                 break;//
           case 10: chmod_file();             break;//
	         case 11: touch(); 		              break;//	
           case 12: quit();	 	                break;//   
           case 13: unlink();                 break;
	         default: printf("invalid command\n");
           break;
      }
 } 
} /* end main */

// NOTE: you MUST use a function pointer table






