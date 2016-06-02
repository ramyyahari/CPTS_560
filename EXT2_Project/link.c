void link() 
{

int ino,oino,nino,dev=root->dev;
MINODE *omip,*nmip,*pmip;
INODE *temp,*ntemp;
char *newfile=parameter;
char* cp,*parent,*child;
char buf[BLKSIZE],tem[256];


oino=getino(dev,pathname);
omip=iget(dev,oino);
temp=&omip->INODE;

if(temp->i_mode&FILE_MODE!=FILE_MODE)
{
	printf("\nNot a file");
	return;
}

nino=getino(dev,newfile);
if(nino)
{
printf("\nFile already exists");
	return;
}
	strcpy(tem,newfile);

	parent=dirname(newfile);
	child=basename(tem);	
			
	if(parameter[0]=='/')
	{
		dev=root->dev;
			
	}
	else
		dev=running->cwd->dev;
		
		
	ino=getino(dev,parent);
	pmip=iget(dev,ino);
	ntemp=&pmip->INODE;
	/*i=check_list(parent,child);
	if(i)
		printf("\ncan create");		
	else 
		printf("\ncant create");		
*/printf("\ncreat happening");
	if(ntemp->i_mode&DIR_MODE==DIR_MODE)
	{
		enter_name(pmip, omip->ino, child);
	}	
	temp->i_links_count++;
	omip->dirty=1;
	iput(omip);

return 0;
} 

symlink()
{

int ino,nino,dev=root->dev,tino;
MINODE *mip,*pmip;
INODE *temp,*ptemp;
char *newfile=parameter;
char* cp,*parent,*child;
char buf[BLKSIZE],tem[256];


ino=getino(dev,pathname);
mip=iget(dev,ino);
temp=&mip->INODE;

if((temp->i_mode&FILE_MODE!=FILE_MODE)&&(temp->i_mode&DIR_MODE!=DIR_MODE))
{
	printf("\nNot a file or directory");
	return;
}

nino=getino(dev,newfile);
if(nino)
{
printf("\nFile already exists");
	return;
}
	strcpy(pathname,parameter);
	creat_file();
	nino=getino(dev,newfile);
	pmip=iget(dev,nino);
	ptemp=&pmip->INODE;
	ptemp->i_mode=0xA000;
	
	temp->i_links_count++;
	
	mip->dirty=1;
	pmip->dirty=1;	
	iput(mip);
	iput(pmip);
	return 0;

}


unlink()
{

int ino,dev=root->dev,i;
MINODE *mip,*pip;
INODE *temp;
char *cp,*parent,*child;
char buf[BLKSIZE],tem[256];
strcpy(tem,pathname);

ino=getino(dev,pathname);
mip=iget(dev,ino);
temp=&mip->INODE;
strcpy(tem,pathname);

	if(temp->i_mode != DIR_MODE)
	{
		temp->i_links_count--;

		if(temp->i_links_count == 0)
		{
			
			while(i<12)
			{
				if(temp->i_block[i] != 0)
				{
					bdealloc(dev, temp->i_block[i]);
				}
				else 
					break;
				i++;
			}

			idealloc(dev, ino);
		}
		mip->dirty=1;
		iput(mip);

		ino = getino(dev,dirname(tem));
		pip = iget(dev,ino);
		rm_child(pip,basename(pathname));
		//pip->dirty=1;
		//iput(pip);
	}
	return 0;
}



