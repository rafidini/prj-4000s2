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
/*! \file drawquadrangle.c

\brief draw a filled quadrangle in a 2D image

<B>Usage:</B> drawquadrangle in.pgm x1 y1 x2 y2 x3 y3 out.pgm

<B>Description:</B>

Draws a filled quadrangle segment with vertices  (x1,y1), (x2,y2) and (x3,y3)

<B>Types supported:</B> byte 2D

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/

/*
%TEST drawquadrangle %IMAGES/2dbyte/binary/b2empty_30_40.pgm 0 0 4 5 29 39 10 30 %RESULTS/drawquadrangle_b2empty_30_40.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldraw.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t x1, y1, x2, y2, x3, y3, x4, y4;

  if (argc != 11)
  {
    fprintf(stderr, "usage: %s in.pgm x1 y1 x2 y2 x3 y3 x4 y4 out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (depth(image) > 1)
  {
    fprintf(stderr, "%s: image must be 2D\n", argv[0]);
    exit(1);
  }

  x1 = atoi(argv[2]);
  y1 = atoi(argv[3]);
  x2 = atoi(argv[4]);
  y2 = atoi(argv[5]);
  x3 = atoi(argv[6]);
  y3 = atoi(argv[7]);
  x4 = atoi(argv[8]);
  y4 = atoi(argv[9]);
  ldrawfilledquadrangle(image, x1, y1, x2, y2, x3, y3, x4, y4);

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

