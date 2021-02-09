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
/*! \file quasishear.c

\brief quasi shear rotation

<B>Usage:</B> quasishear in.pgm theta [x y] out.pgm

<B>Description:</B>
rotates the image of the angle \b theta (in degrees).
If a point (x,y) is given, then the result is croped to the original
image size, otherwise a new image with sufficient size is created.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

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
#include <lrotations.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * image2;
  double theta, x, y;

  if ((argc != 4) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm theta [x y] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  theta = atof(argv[2]);
  theta = (theta * M_PI) / 180;

  if (argc == 6)
  {
    x = atof(argv[3]);
    y = atof(argv[4]);
    if (!lquasishear(image, theta, x, y))
    {
      fprintf(stderr, "%s: function lquasishear failed\n", argv[0]);
      exit(1);
    }
    writeimage(image, argv[argc-1]);
    freeimage(image);
    return 0;
  }
  else
  {
    image2 = lquasishear2(image, theta);
    if (! image2)
    {
      fprintf(stderr, "%s: function lquasishear2 failed\n", argv[0]);
      exit(1);
    }
    writeimage(image2, argv[argc-1]);
    freeimage(image);
    freeimage(image2);
    return 0;
  }

} /* main */

