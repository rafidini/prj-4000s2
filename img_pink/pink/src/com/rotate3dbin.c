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
/*! \file rotate3dbin.c

\brief rotation of an image

<B>Usage:</B> rotate3dbin in.pgm theta axis [c1 c2] out.pgm

<B>Description:</B>
Method: truncated real rotation.
Rotates the image \b in.pgm of the angle \b theta (in degrees), 
around the straight line defined by parameters \b axis, \b c1 and \b c2, parallel to one of the main axes x, y, z.<br>
Parameter \b axis may be either x, y or z.<br>
If \b axis = x, then the rotation axis is defined by y = \b c1, z = \b c2.<br>
If \b axis = y, then the rotation axis is defined by x = \b c1, z = \b c2.<br>
If \b axis = z, then the rotation axis is defined by x = \b c1, y = \b c2.<br>

If \b in.pgm is not a structuring element, and if parameters \b c1 and \b c2 are omitted then the default values 0, 0 are assumed and the resulting image size is computed such that no loss of information occur. Otherwise, no resize is made.

If \b in.pgm is a structuring element, then parameters \b c1 and \b c2 are ignored and the coordinates of the rotation axis are taken among the ones of the origin of the structuring element.

<B>Types supported:</B> byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/*
%TEST rotate3dbin %IMAGES/3dbyte/binary/b3_se_5_7_9.pgm 25 x %RESULTS/rotate3dbin_b3_se_5_7_9_x_25.pgm
%TEST rotate3dbin %IMAGES/3dbyte/binary/b3_se_5_7_9.pgm 25 y %RESULTS/rotate3dbin_b3_se_5_7_9_y_25.pgm
%TEST rotate3dbin %IMAGES/3dbyte/binary/b3_se_5_7_9.pgm 25 z %RESULTS/rotate3dbin_b3_se_5_7_9_z_25.pgm
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
  double theta, c1, c2, newc1, newc2;
  index_t x, y, z;
  int32_t se, resize;
  char axis;

  if ((argc != 5) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm theta axis [c1 c2] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readse(argv[1], &x, &y, &z);
  if (image == NULL)
  {
    se = 0;
    image = readimage(argv[1]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }
  else se = 1;

  theta = atof(argv[2]);
  theta = (theta * M_PI) / 180;

  axis = argv[3][0];

  resize=1;
  if (argc == 7)
  {
    if (!se) resize=0;
    c1 = atof(argv[4]);
    c2 = atof(argv[5]);
    if (se) fprintf(stderr, "%s: warning ignored parameters %g,%g (structuring element)\n", argv[0], c1, c2);
  }
  else
  {
    if (se)
      switch(axis)
      {
      case 'x': c1 = (double)y; c2 = (double)z; break;
      case 'y': c1 = (double)x; c2 = (double)z; break;
      case 'z': c1 = (double)x; c2 = (double)y; break;
      }
    else  
      c1 = c2 = 0.0;
  }  

  switch(axis)
  {
  case 'x':
    image2 = lrotationRT3Dx(image, theta, c1, c2, &newc1, &newc2, resize); break;
  case 'y':
    image2 = lrotationRT3Dy(image, theta, c1, c2, &newc1, &newc2, resize); break;
  case 'z':
    image2 = lrotationRT3Dz(image, theta, c1, c2, &newc1, &newc2, resize); break;
  default:
    fprintf(stderr, "%s: bad value for axis: %c\n", argv[0], axis);
    exit(1);
  }

  if (! image2)
  {
    fprintf(stderr, "%s: function lrotationRT3D? failed\n", argv[0]);
    exit(1);
  }

  if (se) 
    switch(axis)
    {
    case 'x': writese(image2, argv[argc-1], x, (index_t)newc1, (index_t)newc2); break;
    case 'y': writese(image2, argv[argc-1], (index_t)newc1, y, (index_t)newc2); break;
    case 'z': writese(image2, argv[argc-1], (index_t)newc1, (index_t)newc2, z); break;
    }
  else
    writeimage(image2, argv[argc-1]);
  freeimage(image);
  freeimage(image2);
  return 0;
} /* main */


