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
/*! \file privlist2pgm.c

\brief converts from point list representation to pgm

<B>Usage:</B> privlist2pgm in.list scale out.pgm

<B>Description:</B>

Reads the file <B>in.list</B>. This file must have the following format:
<pre>  
  N &lt;n&gt;    
  x1 y1 z1 v1
  z2 x2 y2 v2
  ...
  zn xn yn vn
</pre>

The parameter \b scale allows to scale the coordinates.

<B>Types supported:</B> list 3D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - septembre 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, ds, ps, N, x, y, z, n, i, margin=2, xmin, ymin, zmin, xmax, ymax, zmax;
  double xx, yy, zz, vv, scale;
  uint8_t *F;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.list scale out.pgm\n", argv[0]);
    exit(1);
  }

  scale = atof(argv[argc-2]);

  // 1st pass - to get image dimensions
  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'N')
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  xmin = ymin = zmin = INT_MAX;
  xmax = ymax = zmax = INT_MIN;
  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf %lf %lf\n", &xx, &yy, &zz, &vv);
    xx *= scale;
    yy *= scale;
    zz *= scale;
    x = (int32_t)floor(xx);
    y = (int32_t)floor(yy);
    z = (int32_t)floor(zz);
    if (x < xmin) xmin = x; if (y < ymin) ymin = y; if (z < zmin) zmin = z;
    if (x > xmax) xmax = x; if (y > ymax) ymax = y; if (z > zmax) zmax = z;
  }
  fclose(fd);

  // allocate image
  rs = xmax - xmin + margin + margin;
  cs = ymax - ymin + margin + margin;
  ds = zmax - zmin + margin + margin;
  ps = rs * cs;
  N = ps * ds;

  //  printf("xmin=%ld, ymin=%ld, zmin=%ld\n", xmin, ymin, zmin);
  //  printf("xmax=%ld, ymax=%ld, zmax=%ld\n", xmax, ymax, zmax);
  printf("rs=%ld, cs=%ld, ds=%ld\n", (long int)rs, (long int)cs, (long int)ds);

  image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE); assert(image != NULL);
  F = UCHARDATA(image);
  razimage(image);

  // 2nd pass - read values and create image
  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  fscanf(fd, "%d\n", &n);

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf %lf %lf\n", &xx, &yy, &zz, &vv);
    xx *= scale;
    yy *= scale;
    zz *= scale;
    x = (int32_t)floor(xx);
    y = (int32_t)floor(yy);
    z = (int32_t)floor(zz);
    x = x - xmin + margin;
    y = y - ymin + margin;
    z = z - zmin + margin;
    F[z * ps + y * rs + x] = (int32_t)floor(vv);
  }
  fclose(fd);

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
}

