/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file colorize.c

\brief generates a color image from a grayscale image and a lookup table

<B>Usage:</B> colorize in.pgm lut.ppm out.ppm

<B>Description:</B>
Generates a color image from a grayscale image and a lookup table (see genlut.c).

<B>Types supported:</B> byte 2D, long 2D, byte 3D, long 3D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;
  uint8_t *r; /* la LookUp Table */
  uint8_t *g;
  uint8_t *b;
  int32_t rs, cs, N, i, lutsize;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm lut.ppm out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);

  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(in);
  cs = colsize(in);
  N = rs * cs;
  assert(depth(in) == 1);

  // Charge la LUT
  if (!readrgbimage(argv[2], &g1, &g2, &g3))
  {
    fprintf(stderr, "%s: readrgbimage failed\n", argv[0]);
    exit(1);
  }
  assert(colsize(g1) == 1);
  assert(depth(g1) == 1);
  assert(datatype(g1) == VFF_TYP_1_BYTE);

  lutsize = rowsize(g1);
  r = (uint8_t *)malloc(lutsize * sizeof(uint8_t)); assert(r != NULL);
  g = (uint8_t *)malloc(lutsize * sizeof(uint8_t)); assert(g != NULL);
  b = (uint8_t *)malloc(lutsize * sizeof(uint8_t)); assert(b != NULL);

  for (i = 0; i < lutsize; i++)
  {  
    r[i] = (UCHARDATA(g1))[i];
    g[i] = (UCHARDATA(g2))[i];
    b[i] = (UCHARDATA(g3))[i];
  }

  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  g1 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); assert(g1 != NULL);
  g2 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); assert(g2 != NULL);
  g3 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); assert(g3 != NULL);

  // Applique la LUT
  if (datatype(in) == VFF_TYP_1_BYTE)
  {
    uint8_t v;
    for (i = 0; i < N; i++)
    {
      v = ((UCHARDATA(in))[i])%lutsize;
      (UCHARDATA(g1))[i] = r[v];
      (UCHARDATA(g2))[i] = g[v];
      (UCHARDATA(g3))[i] = b[v];
    }
  }
  else if (datatype(in) == VFF_TYP_4_BYTE)
  {
    uint32_t v;
    for (i = 0; i < N; i++)
    {
      v = ((ULONGDATA(in))[i])%lutsize;
      (UCHARDATA(g1))[i] = r[v];
      (UCHARDATA(g2))[i] = g[v];
      (UCHARDATA(g3))[i] = b[v];
    }
  }
  else 
  {
    fprintf(stderr, "%s: bad data type\n", argv[0]);
    exit(1);
  }

  writergbimage(g1, g2, g3, argv[argc-1]);
  freeimage(in);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);
  free(r); free(g); free(b);

  return 0;
} /* main */
