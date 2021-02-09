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
/*! \file fft.c

\brief fast Fourier transform

<B>Usage:</B> fft in.pgm [dir] out.pgm

<B>Description:</B>
Computes the 2 dimensional Fast Fourier Transform of an image.

This program will compute either a forward or inverse FFT, depending on
the direction requested with the \b dir option.  A \b dir value of 0
will result in a forward FFT, and a \b dir value of 1 will result
in an inverse FFT. 

The arguments are described as follows:

\b in.pgm specifies the input image, which must be of data type COMPLEX.

\b out.pgm output image, which will be an image of data type COMPLEX. 
If row size or column size of in.pgm is not an integral power of 2, then input data is padded and dimensions of out.pgm may be different from those of in.pgm . 

\b dir (optional)
specifies the FFT direction.  A \b dir of 0 (default) will result in a forward FFT, 
and a \b dir of 1 will result in an inverse FFT.

This particular implementation of the DFT uses the transform pair defined as follows:

Let there be two complex arrays each with n rows and m columns.

Index them as 

f(x,y):    0 <= x <= m - 1,  0 <= y <= n - 1

F(u,v):    -m/2 <= u <= m/2 - 1,  -n/2 <= v <= n/2 - 1

Then the forward and inverse transforms are related as follows.

Forward:

F(u,v) = \sum_{x=0}^{m-1} \sum_{y=0}^{n-1} f(x,y) \exp{-2\pi i (ux/m + vy/n)}

Inverse:

f(x,y) = 1/(mn) \sum_{u=-m/2}^{m/2-1} \sum_{v=-n/2}^{n/2-1} F(u,v) \exp{2\pi i (ux/m + vy/n)}

Therefore, the transforms have these properties:

1.  \sum_x \sum_y  f(x,y) = F(0,0)

2.  m n \sum_x \sum_y |f(x,y)|^2 = \sum_u \sum_v |F(u,v)|^2

<B>Types supported:</B> complex 2d

<B>Category:</B> signal
\ingroup signal

\author Stefan Gustavson (stegu@itn.liu.se) 2003-10-20
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcrop.h>
#include <lfft.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * image2;
  int32_t dir = 0;
  int32_t rs, cs, cs2, rs2;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [dir] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);

  if ((datatype(image) != VFF_TYP_COMPLEX) || (depth(image) != 1))
  {
    fprintf(stderr,"%s: input image type must be complex 2D\n", argv[0]);
    exit(1);
  }

  if (argc == 4) dir = atoi(argv[2]);

  rs2 = cs2 = 1;
  while (rs2 < rs) rs2 = rs2 << 1;
  while (cs2 < cs) cs2 = cs2 << 1;
  if ((rs2 != rs) || (cs2 != cs))
  {
    image2 = allocimage(NULL, rs2, cs2, 1, VFF_TYP_COMPLEX);
    assert(image2 != NULL);
    image2->xdim = image->xdim;
    image2->ydim = image->ydim;
    image2->zdim = image->zdim;
    razimage(image2);
    if (!linsert(image, image2, 0, 0, 0))
    {
      fprintf(stderr, "%s: function linsert failed\n", argv[0]);
      exit(1);
    }
    freeimage(image);
    image = image2;
  }

  if (! lfft(image, dir))
  {
    fprintf(stderr, "%s: function lfft failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


