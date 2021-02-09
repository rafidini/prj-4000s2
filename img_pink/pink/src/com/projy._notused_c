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
/* \file projy.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* projette une image 3d binaire suivant une perspective cavaliere */
/* Michel Couprie - juillet 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image3d;
  struct xvimage * image;
  int32_t i, j, k;
  uint8_t *Im3d;
  uint8_t *Im;
  int32_t rs, cs, d, n, N;
  float dx, dy;
  float x0, y0;
  float dndg, ndg;
  int32_t x, y, z, dzx, dzy, nplans, nrs, ncs, NI;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in3d.pgm dx dy dzx dzy out.pgm \n", argv[0]);
    exit(1);
  }

  image3d = readimage(argv[1]);

  rs = rowsize(image3d);   /* taille ligne */
  cs = colsize(image3d);   /* taille colonne */
  d = depth(image3d);      /* nb plans */
  n = rs * cs;             /* taille plan */
  N = n * d;               /* taille image */
  Im3d = UCHARDATA(image3d);

  dx = atof(argv[2]);
  dy = atof(argv[3]);
  dz = atoi(argv[4]);

  nplans = d * dz;
  nrs = (int32_t)ceil(rs + (dx * nplans));
  ncs = (int32_t)ceil(cs + (dy * nplans));
  NI = nrs * ncs;
  dndg = (float)(7 * NDG_MAX) / (float)(8 * nplans);

  image = allocimage(NULL, nrs, ncs, 1, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "proj3d: allocimage failed\n");
    exit(1);
  }
  Im = UCHARDATA(image);
  for (i = 0; i < NI; i++) Im[i] = 0;

  ndg = (float)NDG_MAX / 8;
  for (k = d-1; k >= 0; k--)
  {
    for (z = dz-1; z >= 0; z--)
    {
      x0 = (nplans - ((k*dz)+z)) * dx;
      y0 = (nplans - ((k*dz)+z)) * dy;
      for (j = 0; j < cs; j++)
        for (i = 0; i < rs; i++)
	{
          x = (int32_t)(x0 + i);
          y = (int32_t)(y0 + j);
          if (Im3d[k * n + j * rs + i]) 
            Im[y * nrs + x] = (uint8_t)ndg;
	}
      ndg += dndg;
    } /* for (z = 0; z < dz; z++) */
  } /* for (k = d-1; k >= 0; k--) */

  writeimage(image, argv[5]);
  freeimage(image3d);
  freeimage(image);

  return 0;
} /* main */

