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
/*! \file float2long.c

\brief converts a "float" image to a "int32_t" image

<B>Usage:</B> float2long in.pgm offset factor out.pgm

<B>Description:</B>

Applies the following tranformation to each value x in \b in.pgm :

X = round(offset + factor * x)

<B>Types supported:</B> float 2d, float 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/*
   Michel Couprie - novembre 2005
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <lhisto.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * imagefloat;
  struct xvimage * imagelong;
  float *L;
  int32_t *B;
  int32_t x;
  int32_t rs, cs, d, N;
  double T, offset, factor;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in1.pgm offset factor out.pgm \n", argv[0]);
    exit(1);
  }

  imagefloat = readimage(argv[1]);  
  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(imagefloat) != VFF_TYP_FLOAT)
  {
    fprintf(stderr, "%s: image type must be float\n", argv[0]);
    exit(1);
  }

  offset = atof(argv[2]);
  factor = atof(argv[3]);

  //  printf("%g %g\n", offset, factor);

  rs = rowsize(imagefloat);
  cs = colsize(imagefloat);
  d = depth(imagefloat);
  N = rs * cs * d;
  L = FLOATDATA(imagefloat);
  
  imagelong = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  B = (int32_t *)(UCHARDATA(imagelong));
  imagelong->xdim = imagefloat->xdim;
  imagelong->ydim = imagefloat->ydim;
  imagelong->zdim = imagefloat->zdim;

  for (x = 0; x < N; x++)
  {
    T = offset + (factor * L[x]);
    B[x] = (int32_t)arrondi(T);
  }

  writeimage(imagelong, argv[argc-1]);
  freeimage(imagefloat);
  freeimage(imagelong);

  return 0;
} /* main */
