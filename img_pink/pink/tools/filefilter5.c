#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  elimine les lignes contenant "/* $Id:"

#define BUFFERSIZE 10000

int main(int argc, char **argv) 
{
  FILE *fdin = NULL;
  FILE *fdout = NULL;
  int ret;
  char *read;
  char buffer[BUFFERSIZE];
 
  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in out \n", argv[0]);
    exit(1);
  }

  fdin = fopen(argv[1],"r");
  if (!fdin) 
  { 
    fprintf(stderr, "Error : cannot open file %s\n", argv[1]);
    exit(0);
  }

  fdout = fopen(argv[2],"w");
  if (!fdout) 
  { 
    fprintf(stderr, "Error : cannot open file %s\n", argv[2]);
    exit(0);
  }

  do
  {
    read = fgets(buffer, BUFFERSIZE, fdin);
    if (read)
    {
      if (!strstr(buffer, "/* $Id:")) fputs(buffer, fdout);
    }
  } while (read);

  fclose(fdin);
  fclose(fdout);
}
