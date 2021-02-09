#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* change les 
int main(argc, argv) 
  int argc; char **argv; 
en
int main(int argc, char **argv)
 */

#define BUFFERSIZE 10000

int main(int argc, char **argv) 
{
  FILE *fdin = NULL;
  FILE *fdout = NULL;
  int ret, stop;
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
  stop = 0;
  do
  {
    read = fgets(buffer, BUFFERSIZE, fdin);
    if (read)
    {
      if (strstr(buffer, "int main(argc, argv)")) stop = 1;
      if (!stop) fputs(buffer, fdout);
      else fputs("int main(int argc, char **argv)\n", fdout);
    }
  } while (read && !stop);

  stop = 0;
  do
  {
    read = fgets(buffer, BUFFERSIZE, fdin);
    if (read)
    {
      if (strstr(buffer, "int argc; char **argv;")) stop = 1;
      if (!stop) fputs(buffer, fdout);
    }
  } while (read && !stop);

  do
  {
    read = fgets(buffer, BUFFERSIZE, fdin);
    if (read)
    {
      fputs(buffer, fdout);
    }
  } while (read);

  fclose(fdin);
  fclose(fdout);
}
