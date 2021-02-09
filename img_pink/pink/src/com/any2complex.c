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
/*! \file any2complex.c

\brief converts a "byte", "long" of "float" image (or couple of images) to a "fcomplex" image

<B>Usage:</B> any2complex re.pgm [im.pgm] out.pgm

<B>Description:</B> 

<B>Types supported:</B> byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d.

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - décembre 2010 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * im_complex;
  struct xvimage * im_real;
  struct xvimage * im_imaginary = NULL;
  int32_t x, rs, cs, ds, N;
  fcomplex *C;
  
  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s re.pgm [im.pgm] out.pgm \n", argv[0]);
    exit(1);
  }

  im_real = readimage(argv[1]); 
  if (im_real == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(im_real);
  cs = colsize(im_real);
  ds = depth(im_real);
  N = rs * cs * ds;

  if (argc == 4)
  {
    im_imaginary = readimage(argv[1]); 
    if (im_imaginary == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    assert(rowsize(im_imaginary) == rs);
    assert(colsize(im_imaginary) == cs);
    assert(depth(im_imaginary) == ds);
  }

  im_complex = allocimage(NULL, rs, cs, ds, VFF_TYP_COMPLEX);
  if (im_complex == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  C = COMPLEXDATA(im_complex);
  if (argc == 3) razimage(im_complex);

  if (datatype(im_real) == VFF_TYP_1_BYTE)
  {
    uint8_t *I = UCHARDATA(im_real);
    for (x = 0; x < N; x++) C[x].re = (float)(I[x]);
  }
  else
  if (datatype(im_real) == VFF_TYP_4_BYTE)
  {
    int32_t *I = SLONGDATA(im_real);
    for (x = 0; x < N; x++) C[x].re = (float)I[x];
  }
  else
  if (datatype(im_real) == VFF_TYP_FLOAT)
  {
    float *I = FLOATDATA(im_real);
    for (x = 0; x < N; x++) C[x].re = (float)I[x];
  }
  else
  if (datatype(im_real) == VFF_TYP_DOUBLE)
  {
    double *I = DOUBLEDATA(im_real);
    for (x = 0; x < N; x++) C[x].re = (float)I[x];
  }
  else
  if (datatype(im_real) == VFF_TYP_COMPLEX)
  {
    assert(im_imaginary == NULL);
    copy2image(im_complex, im_real);
  }
  else
  {
    fprintf(stderr, "%s: bad data type: %d\n", argv[0], datatype(im_real));
    exit(1);
  }

  if (im_imaginary != NULL)
  {

    if (datatype(im_imaginary) == VFF_TYP_1_BYTE)
    {
      uint8_t *I = UCHARDATA(im_imaginary);
      for (x = 0; x < N; x++) C[x].im = (float)(I[x]);
    }
    else
    if (datatype(im_imaginary) == VFF_TYP_4_BYTE)
    {
      int32_t *I = SLONGDATA(im_imaginary);
      for (x = 0; x < N; x++) C[x].im = (float)I[x];
    }
    else
    if (datatype(im_imaginary) == VFF_TYP_FLOAT)
    {
      float *I = FLOATDATA(im_imaginary);
      for (x = 0; x < N; x++) C[x].im = (float)I[x];
    }
    else
    if (datatype(im_imaginary) == VFF_TYP_DOUBLE)
    {
      double *I = DOUBLEDATA(im_imaginary);
      for (x = 0; x < N; x++) C[x].im = (float)I[x];
    }
    else
    {
      fprintf(stderr, "%s: bad data type: %d\n", argv[0], datatype(im_imaginary));
      exit(1);
    }
  } // if (im_imaginary != NULL)

  im_complex->xdim = im_real->xdim;
  im_complex->ydim = im_real->ydim;
  im_complex->zdim = im_real->zdim;

  writeimage(im_complex, argv[argc-1]);
  freeimage(im_complex);
  freeimage(im_real);
  if (im_imaginary != NULL) freeimage(im_imaginary);

  return 0;
} /* main */
