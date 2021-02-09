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
/*! \file genplane.c

\brief generates a plane normal to one of the directions x,y,z

<B>Usage:</B> genplane rs cs ds dir n out.pgm

<B>Description:</B>
The size of the result image <B>out.pgm</B> is given by the parameters 
<B>rs</B>, <B>cs</B>, <B>ds</B>.
The parameter <B>dir</B> is set to <B>x</B>, <B>y</B>, or <B>z</B>.
The parameter <B>n</B> is an integer.
The output binary image <B>out.pgm</B> contains the plane defined by <B>dir=n</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - decembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
  int32_t x, y, z, n;
  char axe;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s rs cs ds {x|y|z} n out.pgm\n", argv[0]);
    exit(1);
  }

  rs = atoi(argv[1]);
  cs = atoi(argv[2]);
  ds = atoi(argv[3]);
  ps = rs * cs;
  N = ps * ds;
  axe = argv[4][0];
  n = atoi(argv[5]);
  image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  Im = UCHARDATA(image);
  memset(Im, 0, N);

  switch(axe)
  {
    case 'x': 
      if ((n < 0) || (n >= rs))
      {
        fprintf(stderr, "%s: bad n = %d\n", argv[0], n);
        exit(1);
      }
      for (z = 0; z < ds; z++)
        for (y = 0; y < cs; y++)
          Im[z * ps + y * rs + n] = NDG_MAX;
      break;
    case 'y': 
      if ((n < 0) || (n >= cs))
      {
        fprintf(stderr, "%s: bad n = %d\n", argv[0], n);
        exit(1);
      }
      for (z = 0; z < ds; z++)
        for (x = 0; x < rs; x++)
          Im[z * ps + n * rs + x] = NDG_MAX;
      break;
    case 'z': 
      if ((n < 0) || (n >= ds))
      {
        fprintf(stderr, "%s: bad n = %d\n", argv[0], n);
        exit(1);
      }
      for (y = 0; y < cs; y++)
        for (x = 0; x < rs; x++)
          Im[n * ps + y * rs + x] = NDG_MAX;
      break;
    default:
      fprintf(stderr, "usage: %s rs cs ds {x|y|z} n out.pgm\n", argv[0]);
      exit(1);
  } /* switch */

  writeimage(image, argv[6]);
  freeimage(image);
  return 0;
} /* main */

