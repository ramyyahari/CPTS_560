symlink()
{

int ino,oino,nino,dev=root->dev,tino;
MINODE *omip,*nmip,*pmip,*tmip;
INODE *otemp,*ptemp,*ttemp;
char *newfile=parameter;
char* cp,*parent,*child;
char buf[BLKSIZE],tem[256];


oino=getino(dev,pathname);
omip=iget(dev,oino);
otemp=&omip->INODE;

/*
if(temp->i_mode&FILE_MODE!=FILE_MODE)
{
	printf("\nNot a file");
	return;
}
*/

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
	ptemp=&pmip->INODE;
	/*i=check_list(parent,child);
	if(i)
		printf("\ncan create");		
	else 
		printf("\ncant create");		
*/printf("\ncreat happening");

		enter_name(pmip, omip->ino, child);
		ino=getino(dev,parameter);
		pmip=iget(dev,ino);
		ptemp->i_mode=0xA000;
		get_block(dev,ptemp->i_block[0],buf);
		DIR *dp=(DIR*)buf;
		dp->inode=ino;			
		dp->rec_len=1024;
		dp->file_type=7;
		dp->name_len=strlen(child);		/* Name length */
		strcpy(dp->name,child);
		put_block(dev,ptemp->i_block[0],buf);
		/*	
		tino=getino(dev,child);
		tmip=iget(dev,tino);	
		ttemp=&tmip->INODE;
		ttemp->i_mode=0xA000;
		*/
	otemp->i_links_count++;
	
	//tmip->dirty=1;
	pmip->dirty=1;	
	omip->dirty=1;
	iput(omip);
	iput(pmip);
	//iput(tmip);
return 0;

}
