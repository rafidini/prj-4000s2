#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 10000

int searchstring(char *filename, char *string)
{
  FILE *fd = NULL;
  char buffer[BUFFERSIZE];
  char *read;
  int ret = 0;
  fd = fopen(filename, "r");
  if (!fd) 
  { 
    fprintf(stderr, "Error : cannot open file %s\n", filename);
    exit(0);
  }
  
  do
  {
    read = fgets(buffer, BUFFERSIZE, fd);
    if (read && strstr(buffer, string)) ret = 1;
  } while (read && !ret);
  
  fclose(fd);
  return ret;
}

int main(int argc, char **argv) 
{
  FILE *fdin = NULL;
  FILE *fdout = NULL;
  int ret1, ret2, ret3, ret4, stop;
  char *read;
  char buffer[BUFFERSIZE];
 
  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in out \n", argv[0]);
    exit(1);
  }

  ret1 = searchstring(argv[1], "stdio.h");
  ret2 = searchstring(argv[1], "stdlib.h");
  ret3 = searchstring(argv[1], "string.h");
  ret4 = searchstring(argv[1], "strcmp");

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
      if (strstr(buffer, "#include")) stop = 1;
      fputs(buffer, fdout);
    }
  } while (read && !stop);
  
  if (!ret1) fputs("#include <stdio.h>\n", fdout);
  if (!ret2) fputs("#include <stdlib.h>\n", fdout);
  if (!ret3 && ret4) fputs("#include <string.h>\n", fdout);

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
