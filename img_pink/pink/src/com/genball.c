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

/*! \file genball.c

\brief generates a binary euclidean ball

<B>Usage:</B> genball radius [dim] out.pgm

<B>Description:</B>
Draws a binary euclidean ball in the new image \b out.pgm.
The parameter <B>radius</B> specifies the radius of the ball. 
The parameter <B>dim</B> specifies the dimension: 2 (default) or 3.
The center of the ball is designated as the origin (for use as structuring element).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldraw.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  double radius;
  int32_t cen, dim=2;
  index_t size;
  
  if ((argc != 4) && (argc != 3))
  {
    fprintf(stderr, "usage: %s radius [dim] out.pgm \n", argv[0]);
    exit(1);
  }

  radius = atof(argv[1]);
  cen = (int32_t)ceil(radius);
  size = 2* cen + 1;

  if (argc == 4) dim = atoi(argv[2]);

  if (dim == 3)
  {
    image = allocimage(NULL, size, size, size, VFF_TYP_1_BYTE); assert(image != NULL);
    ldrawball(image, radius, cen, cen, cen);
  }
  else if (dim == 2)
  {
    image = allocimage(NULL, size, size, 1, VFF_TYP_1_BYTE); assert(image != NULL);
    ldrawdisc(image, radius, cen, cen);
  }
  else
  {
    fprintf(stderr, "usage: %s radius {2|3} out.pgm \n", argv[0]);
    exit(1);
  }

  writese(image, argv[argc-1], cen, cen, cen);
  freeimage(image);
  return 0;
} /* main */

