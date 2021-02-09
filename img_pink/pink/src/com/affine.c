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
/*! \file affine.c

\brief affine transformation of a 2D image

<B>Usage:</B> affine in.pgm {trans.lst | hx hy theta tx ty} out.pgm

<B>Description:</B>

Applies to in.pgm an affine tranformation composed of a zoom 
(factors \b hx, \b hy), a rotation (angle \b theta in degrees, 
center 0,0), and a translation (vector \b tx, \b ty). 
Alternatively, the transformation parameters may be given 
in a type "e" list file (see doc/formats.txt).

Method: interpolation.

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
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcgeo.h>
#include <lrotations.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * image2;
  double theta, hx, hy, tx, ty;

  if ((argc != 4) && (argc != 8))
  {
    fprintf(stderr, "usage: %s in.pgm {trans.lst | hx hy theta tx ty} out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 8)
  {
    hx = atof(argv[2]);
    hy = atof(argv[3]);
    theta = atof(argv[4]);
    theta = (theta * M_PI) / 180;
    tx = atof(argv[5]);
    ty = atof(argv[6]);
  }
  else
  {
    FILE * fd = fopen(argv[2],"r");
    char type;
    int32_t n;

    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }

    fscanf(fd, "%c", &type);
    if (type != 'e')
    {
      fprintf(stderr, "usage: %s: bad file format for %s: %c \n", argv[0], argv[2], type);
      exit(1);
    }

    fscanf(fd, "%d\n", &n);
    if (n != 5)
    {
      fprintf(stderr, "usage: %s: wrong number of parameters in %s: %d \n", argv[0], argv[2], n);
      exit(1);
    }

    fscanf(fd, "%lf%lf%lf%lf%lf", &hx, &hy, &theta, &tx, &ty);
    theta = (theta * M_PI) / 180;
  } // else

  image2 = copyimage(image);
  if (! image2)
  {
    fprintf(stderr, "%s: copyimage failed\n", argv[0]);
    exit(1);
  }

  if (! laffinetransformation(image, hx, hy, theta, tx, ty, image2))
  {
    fprintf(stderr, "%s: function laffinetransformation failed\n", argv[0]);
    exit(1);
  }
  writeimage(image2, argv[argc-1]);
  freeimage(image);
  freeimage(image2);
  return 0;
} /* main */


