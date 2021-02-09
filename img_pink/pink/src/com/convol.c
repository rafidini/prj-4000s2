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
/*! \file convol.c

\brief convolution

<B>Usage:</B> convol in.pgm kernel.pgm [mode] out.pgm

<B>Description:</B>
Convolution of \b in.pgm by <B>kernel.pgm</B>.
The result is a float image.
Depending on the value given for the (optional) parameter <B>mode</B>:
\li   <B>mode</B> = 0 (default) : naive algorithm. 
      The image \b in.pgm is considered as null out of its support. 
\li   <B>mode</B> = 1 : naive algorithm. 
      The boundary of image \b in.pgm is extended outside its support.
\li   <B>mode</B> = 2 : convolution using the FFT.
      The image \b in.pgm is considered as null out of its support. 
\li   <B>mode</B> = 3 : convolution using the FFT.
      The boundary of image \b in.pgm is extended outside its support.

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d

<B>Category:</B> signal
\ingroup  signal

\warning
Naive convolution algorithm is in O(|in|*|kernel|). 
For large kernels, use FFT version which is in 
O(n log n) where n = max(|in|,|kernel|)

\author Michel Couprie
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
  struct xvimage * mask;
  int32_t mode = 0;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm kernel.pgm [mode] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  mask = readimage(argv[2]);
  if ((image == NULL) || (mask == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5) mode = atoi(argv[3]);

  if (! convertfloat(&mask))
  {
    fprintf(stderr, "%s: function convertfloat failed\n", argv[0]);
    exit(1);
  }

  if (! convertgen(&image, &mask))
  {
    fprintf(stderr, "%s: function convertgen failed\n", argv[0]);
    exit(1);
  }
  
  if (! lconvol(image, mask, mode))
  {
    fprintf(stderr, "%s: function lconvol failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(mask);
  return 0;
} /* main */

