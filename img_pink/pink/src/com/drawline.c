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
/*! \file drawline.c

\brief draw a straight line segment in a 2D image or a 3D image

<B>Usage:</B> drawline in.pgm x1 y1 [z1] x2 y2 [z2] out.pgm

<B>Description:</B>

Draws a straight line segment between (x1,y1[,z1]) and (x2,y2[,z2])

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/

/*
%TEST drawline %IMAGES/2dbyte/binary/b2empty_30_40.pgm 0 0 29 39 %RESULTS/drawline_b2empty_30_40.pgm
%TEST drawline %IMAGES/3dbyte/binary/b3empty_20_30_40.pgm 0 0 0 19 29 39 %RESULTS/drawline_b3empty_20_30_40.pgm
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
  int32_t x1, y1, z1, x2, y2, z2;

  if ((argc != 7) && (argc != 9))
  {
    fprintf(stderr, "usage: %s in.pgm x1 y1 [z1] x2 y2 [z2] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 7)
  {
    if (depth(image) > 1)
    {
      fprintf(stderr, "%s: image must be 2D\n", argv[0]);
      exit(1);
    }
    x1 = atoi(argv[2]);
    y1 = atoi(argv[3]);
    x2 = atoi(argv[4]);
    y2 = atoi(argv[5]);
    ldrawline(image, x1, y1, x2, y2);
  }

  if (argc == 9)
  {
    x1 = atoi(argv[2]);
    y1 = atoi(argv[3]);
    z1 = atoi(argv[4]);
    x2 = atoi(argv[5]);
    y2 = atoi(argv[6]);
    z2 = atoi(argv[7]);
    ldrawline3d(image, x1, y1, z1, x2, y2, z2);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

