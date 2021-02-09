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
/*! \file point.c

\brief modifies the value of a given point 

<B>Usage:</B> point in.pgm x y z v out.pgm

<B>Description:</B>
The value of the point (<B>x</B>,<B>y</B>,<B>z</B>)
is changed to <B>v</B>.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lpoint.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  index_t x, y, z;
  float v;

  if ((argc != 7) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm [x y z | i] v out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  x = atoi(argv[2]);
  if (argc == 7)
  {
    y = atoi(argv[3]);
    z = atoi(argv[4]);
    v = atof(argv[5]);
  }
  if (argc == 5)
  {
    index_t rs = rowsize(image1);
    index_t ps = rs * colsize(image1);
    v = atof(argv[3]);
    z = x / ps;
    y = (x % ps) / rs;
    x = x % rs;
  }

  if (! lpoint(image1, x, y, z, v))
  {
    fprintf(stderr, "%s: function lpoint failed\n", argv[0]);
    exit(1);
  }
  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  return 0;
} /* main */


