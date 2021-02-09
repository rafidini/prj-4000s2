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
/*! \file ombre.c

\brief binary shadow of a grayscale function

<B>Usage:</B> ombre in.pgm out.pgm

<B>Description:</B>
The shadow of a 1D grayscale image is a 2D binary image.
The shadow of a 2D grayscale image is a 3D binary image.

<B>Types supported:</B> byte 1d, byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * ombre;
  int32_t rs, cs, ds, N, i, j, k;
  uint8_t *I;
  uint8_t *O;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: bad data type\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  if (depth(image) != 1)
  {
    fprintf(stderr, "%s: 3D images not allowed\n", argv[0]);
    exit(0);
  }
  N = rs * cs;

  if (cs == 1) { cs = 256; ds = 1; }
  else ds = 256;
  ombre = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (ombre == NULL)
  {   fprintf(stderr,"%s : allocimage failed\n", argv[0]);
      exit(0);
  }
  I = UCHARDATA(image);
  O = UCHARDATA(ombre);
  memset(O, NDG_MIN, rs*cs*ds);

  if (ds == 1)
    for (i = 0; i < rs; i++)
      for (j = 0; j <= I[i]; j++)
        O[(cs-j-1)*rs + i] = NDG_MAX;
  else
    for (j = 0; j < cs; j++)
      for (i = 0; i < rs; i++)
        for (k = 0; k < I[j*rs + i]; k++)
          O[(ds-k-1)*N + j*rs + i] = NDG_MAX;
    
  writeimage(ombre, argv[argc-1]);
  freeimage(image);
  freeimage(ombre);
  return 0;
} /* main */
