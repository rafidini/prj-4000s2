/*
Compute table for the 3D connectivity numbers

Michel Couprie, janvier 2011

As there are 2^26 possible neighborhood configurations, the table is made of 2^26 entries.
Each entry is a 8-bit word, the first four bits encode the T number and the last four bits encode the Tbar number.
*/


#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <mccodimage.h>
#include <mctopo3d.h>

#define VERBOSE

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  uint8_t *conn_numbers;
  int32_t k, u, nbconfig, tablesize, ret, t6b, t26;
  uint32_t config, mask;
  FILE *fd;
  uint8_t F[27], tmp;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s fileout \n", argv[0]);
    exit(1);
  }

  mctopo3d_init_topo3d();

  nbconfig = 1<<26;
  tablesize = nbconfig;
  conn_numbers = (uint8_t *)calloc(tablesize, sizeof(uint8_t));
  assert(conn_numbers != NULL);

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

    mctopo3d_top26(F, 13, 3, 9, 27, &t26, &t6b);
    assert(t26 <= 8); assert(t6b <= 8);
    
    t26 = t26 << 4;
    tmp = t26 | t6b;
    conn_numbers[config] = tmp;
  }

  fd = fopen (argv[argc-1], "w");
  if (fd == NULL)
  {
    fprintf (stderr, "%s : Error while opening file %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

  ret = fwrite(conn_numbers, sizeof(uint8_t), tablesize, fd);
  if (ret != tablesize)
  {
    fprintf(stderr, "%s: only %d items written\n", argv[0], ret);
    exit(1);
  }

  fclose (fd);
  free(conn_numbers);
  mctopo3d_termine_topo3d();

  return 0;
} // main()
