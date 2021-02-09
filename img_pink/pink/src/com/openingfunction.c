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
/*! \file openingfunction.c

\brief opening function

<B>Usage:</B> openingfunction in.pgm [dist] out.pgm

<B>Description:</B>
This operator associates, to each point x of the input object X, 
the radius of the biggest ball included in X that includes x.
The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: Euclidean distance (truncated)
\li 1: approximate quadratic Euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic Euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

\warning The input image \b in.pgm must be a binary image. No test is done.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2009

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * result;
  int32_t mode;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s f.pgm [dist] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4) mode = atoi(argv[2]); else mode = 0;
  if ((mode != 0) && (mode != 2) && (mode != 3) &&  (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "%s: dist = [0|2|3|4|8|6|18|26] \n", argv[0]);
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (!lmedialaxis_openingfunction(image, mode, result))
  {
    fprintf(stderr, "%s: function lmedialaxis_openingfunction failed\n", argv[0]);
    exit(1);
  }

  writeimage(result, argv[argc-1]);
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */
