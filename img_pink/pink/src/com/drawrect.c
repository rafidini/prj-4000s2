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
/*! \file drawrect.c

\brief Draws a rectangle with sides parallel to the main axes

<B>Usage:</B> drawrect in.pgm x1 y1 z1 x2 y2 z2 out.pgm

<B>Description:</B>
Draws a rectangle (a box in 3D) with sides parallel to the main axes. 
Two diagonally opposed corners are specified by 
<B>x1</B>, <B>y1</B>, <B>z1</B> and <B>x2</B>, <B>y2</B>, <B>z2</B>.
The rectangle is surimposed to the contents of <B>in.pgm</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> draw
\ingroup  draw

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
  uint8_t *Im;
  int32_t rs, cs, ds, ps, N;
  int32_t x, y, z, x1, y1, z1, x2, y2, z2;

  if (argc != 9)
  {
    fprintf(stderr, "usage: %s in.pgm x1 y1 z1 x2 y2 z2 out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  Im = UCHARDATA(image);
  rs = image->row_size;
  cs = image->col_size;
  ps = rs * cs;
  ds = depth(image);
  N = ds * ps;

  x1 = atoi(argv[2]);
  y1 = atoi(argv[3]);
  z1 = atoi(argv[4]);

  x2 = atoi(argv[5]);
  y2 = atoi(argv[6]);
  z2 = atoi(argv[7]);

  if ((x1 < 0) || (x1 >= rs) || (x2 < 0) || (x2 >= rs) ||
      (y1 < 0) || (y1 >= cs) || (y2 < 0) || (y2 >= cs) ||
      (z1 < 0) || (z1 >= ds) || (z2 < 0) || (z2 >= ds)
      )
  {
    fprintf(stderr, "%s: bad coordinates\n", argv[0]);
    exit(1);
  }

  if (x1 > x2) {x = x1; x1 = x2; x2 = x;} 
  if (y1 > y2) {y = y1; y1 = y2; y2 = y;} 
  if (z1 > z2) {z = z1; z1 = z2; z2 = z;} 
  for (z = z1; z <= z2; z++)
    for (y = y1; y <= y2; y++)
      for (x = x1; x <= x2; x++)
	Im[z * ps + y * rs + x] = NDG_MAX;

  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */

