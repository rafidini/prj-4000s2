/*
Table for the end skeletons

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SetEnd(x)   endpoint[x/8]|=(1<<(x%8))

#define VERBOSE

typedef unsigned long ulong;

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int x, y, L1, nb;
  unsigned char *endpoint;
  int tablesize, ret;
  ulong config;
  FILE *fd;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein fileout \n", argv[0]);
    exit(1);
  }

  tablesize = 1<<24;
  endpoint = calloc(tablesize, 1);
  if (! endpoint)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  fd = fopen (argv[1], "r");
  if (fd == NULL)
  {
    fprintf (stderr, "%s : Error while opening file %s\n", argv[0], argv[1]);
    exit(1);
  }
  nb = 0;
  do
  {
    ret = fscanf(fd, "%x", &config);
    if (ret == 1) SetEnd(config);
#ifdef VERBOSE
    nb++;
    if (nb % 100000 == 0)
      fprintf(stderr, "%d conf. traitees\n", nb);
#endif
  } while(ret == 1);
  fclose (fd);  

  fd = fopen (argv[argc-1], "w");
  if (fd == NULL)
  {
    fprintf (stderr, "%s : Error while opening file %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

#ifdef VERIF
#define IsEnd(x) (endpoint[x/8]&(1<<(x%8)))
{ int i, s = 1 << 27;
  for (i = 0; i < s; i++)
    if (IsEnd(i)) printf("Is End: %d\n", i);
}
#endif

  ret = fwrite(endpoint, sizeof(char), tablesize, fd);
  if (ret != tablesize)
  {
    fprintf(stderr, "%s: only %d items written\n", argv[0], ret);
    exit(1);
  }
  

  fclose (fd);
  free(endpoint);

  return 0;
} // main()
