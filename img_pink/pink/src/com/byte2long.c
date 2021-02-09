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
/*! \file byte2long.c

\brief converts a "byte" image to a "int32_t" image

<B>Usage:</B> byte2long in [out]

<B>Description:</B> 

For each pixel x, out[x] = (int32_t)in[x].

If the last argument \b out is omitted, then out = in.

<B>Types supported:</B> byte 2d, byte 3d.

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
  struct xvimage * imagelong;
  struct xvimage * imagebyte;
  int32_t *L;
  uint8_t *B;
  index_t x, rs, cs, ds, N;

  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s in1.pgm [out.pgm] \n", argv[0]);
    exit(1);
  }

  imagebyte = readimage(argv[1]); 
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(imagebyte);
  cs = colsize(imagebyte);
  ds = depth(imagebyte);
  N = rs * cs * ds;
  B = UCHARDATA(imagebyte);
  
  imagelong = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  L = SLONGDATA(imagelong);
  imagelong->xdim = imagebyte->xdim;
  imagelong->ydim = imagebyte->ydim;
  imagelong->zdim = imagebyte->zdim;

  for (x = 0; x < N; x++) L[x] = (int32_t)B[x];

  writeimage(imagelong, argv[argc-1]);
  freeimage(imagelong);
  freeimage(imagebyte);

  return 0;
} /* main */

