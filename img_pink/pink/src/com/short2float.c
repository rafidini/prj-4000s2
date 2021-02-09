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
/*! \file short2float.c

\brief converts a "short" image to a "float" image

<B>Usage:</B> short2float in [out]

<B>Description:</B> 

For each pixel x, out[x] = (float)in[x]

If the last argument \b out is omitted, then out = in.

<B>Types supported:</B> short 2d, short 3d.

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - mai 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * imagefloat;
  struct xvimage * imageshort;
  float *F;
  uint16_t *S;
  index_t x, rs, cs, ds, N;

  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s in1.pgm [out.pgm] \n", argv[0]);
    exit(1);
  }

  imageshort = readimage(argv[1]); 
  if (imageshort == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(imageshort) != VFF_TYP_2_BYTE)
  {
    fprintf(stderr, "%s: image type must be uint16_t\n", argv[0]);
    exit(1);
  }

  rs = rowsize(imageshort);
  cs = colsize(imageshort);
  ds = depth(imageshort);
  N = rs * cs * ds;
  S = USHORTDATA(imageshort);
  
  imagefloat = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  F = FLOATDATA(imagefloat);
  imagefloat->xdim = imageshort->xdim;
  imagefloat->ydim = imageshort->ydim;
  imagefloat->zdim = imageshort->zdim;

  for (x = 0; x < N; x++) F[x] = (float)S[x];

  writeimage(imagefloat, argv[argc-1]);
  freeimage(imagefloat);
  freeimage(imageshort);

  return 0;
} /* main */

