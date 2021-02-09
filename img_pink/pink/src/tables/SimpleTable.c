/*
Compute table for the 3D simple points

Michel Couprie, août 2006
*/


#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mctopo3d.h>

#define SetSimple(x) simplepoint[x/8]|=(1<<(x%8))
#define IsSimple(x) (simplepoint[x/8]&(1<<(x%8)))

#define VERBOSE

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  uint8_t *simplepoint;
  int32_t k, u, nbconfig, tablesize, ret;
  uint32_t config, mask;
  FILE *fd;
  uint8_t F[27];

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s fileout \n", argv[0]);
    exit(1);
  }

  init_topo3d();

  nbconfig = 1<<26;
  tablesize = nbconfig / 8;
  simplepoint = (uint8_t *)calloc(tablesize, sizeof(uint8_t));
  assert(simplepoint != NULL);

  for (config = 0; config < nbconfig; config++)
  {
#ifdef VERBOSE
    if ((config % 100000) == 0) printf("computing config %d\n", config);
#endif
    mask = config;
    for (u = 0; u < 13; u++)
    {
      k = voisin26(13, u, 3, 9, 27);
      if (mask % 2) F[k] = 1; else F[k] = 0;
      mask = mask >> 1;
    }
    F[13] = 1; 
    for (u = 13; u < 26; u++)
    {
      k = voisin26(13, u, 3, 9, 27);
      if (mask % 2) F[k] = 1; else F[k] = 0;
      mask = mask >> 1;
    }
    if (simple26(F, 13, 3, 9, 27)) SetSimple(config);
  }

  fd = fopen (argv[argc-1], "w");
  if (fd == NULL)
  {
    fprintf (stderr, "%s : Error while opening file %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

  ret = fwrite(simplepoint, sizeof(char), tablesize, fd);
  if (ret != tablesize)
  {
    fprintf(stderr, "%s: only %d items written\n", argv[0], ret);
    exit(1);
  }
  fclose (fd);
  free(simplepoint);
  termine_topo3d();

  return 0;
} // main()
