#include "ucode.c"

int main0(char* s)
{
  int i,argc=0;
  char *cp=strtok(s," ");
  while (cp){
           argv[argc++] = cp;         
	   cp++;
  }
  argv[argc] = 0;
  main(argc, argv);
}


