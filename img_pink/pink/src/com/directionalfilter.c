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
/*! \file directionalfilter.c

\brief directional filter for curvilinear feature extraction

<B>Usage:</B> directionalfilter.c in.pgm width length ndir out.pgm

<B>Description:</B>
Let F be the original image from \b in.pgm .
This operator computes the supremum of the convolutions of F
by a series of kernels K0, ... Kn where n = \b ndir - 1, which are defined
by, for each (x,y) and each i in [0...n]: 

\verbatim
sigma = 1 / (2*width*width);
lambda = 1 / (2*length*length);
theta = i * PI / n;
xr = cos(theta) * x - sin(theta) * y;
yr = sin(theta) * x + cos(theta) * y;
Ki(x,y) = exp(-lambda*yr*yr) *
          (4*sigma*sigma*xr*xr - 2*sigma) * 
          exp(-sigma*xr*xr)
\endverbatim

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie 2003
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lconvol.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  double width, length;  
  int32_t ndir;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in.pgm width length ndir out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  width = atof(argv[2]);
  length = atof(argv[3]);
  ndir = atoi(argv[4]);

  if (! convertfloat(&image))
  {
    fprintf(stderr, "%s: function convertfloat failed\n", argv[0]);
    exit(1);
  }
  
  if (! ldirectionalfilter(image, width, length, ndir))
  {
    fprintf(stderr, "%s: function ldirectionalfilter failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return 0;
} /* main */

