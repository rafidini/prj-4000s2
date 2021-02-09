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

OBSOLETE - VOIR DRAWBALL

/* \file ball.c

\brief generates a binary euclidean ball

<B>Usage:</B> ball radius x_size y_size z_size out.pgm

<B>Description:</B>
Generates a binary euclidean ball.
The parameter <B>radius</B> specifies the radius of the ball. 
The parameters <B>x_size</B>, <B>y_size</B> and <B>z_size</B>
indicate the dimensions of the voxel in the "real" world.

<B>Types supported:</B> byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - decembre 1999 */

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
  struct xvimage * image;
  int32_t i, j, k, xc, yc, zc;
  uint8_t *Im;
  int32_t rs, cs, ds, ps, N;
  char name[512];
  double x_size, y_size, z_size, x, y, z;
  double radius, r2;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s radius x_size y_size z_size out.pgm \n", argv[0]);
    exit(1);
  }

  radius = atof(argv[1]);
  x_size = atof(argv[2]);
  y_size = atof(argv[3]);
  z_size = atof(argv[4]);
  xc = (int32_t)ceil(radius / x_size);
  yc = (int32_t)ceil(radius / y_size);
  zc = (int32_t)ceil(radius / z_size);
  rs = 2 * xc + 1;
  cs = 2 * yc + 1;
  ds = 2 * zc + 1;
  ps = rs * cs;
  N = ps * ds;
  sprintf(name, "ball(%lf,%lf,%lf,%lf)", radius, x_size, y_size, z_size);
  image = allocimage(name, rs, cs, ds, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "ball: allocimage failed\n");
    exit(1);
  }
  Im = UCHARDATA(image);
  memset(Im, NDG_MIN, N);

  r2 = radius * radius;
  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    x = abs(i-xc) * x_size;
    y = abs(j-yc) * y_size;
    z = abs(k-zc) * z_size;
    if (x * x + y * y + z * z <= r2)
      Im[k * ps + j * rs + i] = NDG_MAX;
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */

