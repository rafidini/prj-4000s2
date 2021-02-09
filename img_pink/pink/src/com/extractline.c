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
/*! \file extractline.c

\brief extracts a line between two given points from a 3D image

<B>Usage:</B> extractline in.pgm x1 y1 x2 y2 out.pgm

<B>Description:</B>
The original 2D image is <B>in.pgm</B>. 
The output 1D image <B>out.pgm</B> contains the line between 
points (x1,y1) and (x2,y2) extracted from <B>in.pgm</B>.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#define PARANO

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lbresen.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imgres;
  int32_t i, n, nn, rs, cs, x1, y1, x2, y2;
  uint8_t *I, *R;
  int32_t *lx, *ly;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm x1 y1 x2 y2 out.pgm\n", argv[0]);
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
  if (depth(image) != 1)
  {
    fprintf(stderr, "%s: only 2D images allowed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  I = UCHARDATA(image);

  x1 = atoi(argv[2]);
  y1 = atoi(argv[3]);
  x2 = atoi(argv[4]);
  y2 = atoi(argv[5]);

  /* ---------------------------------------------------------- */
  /* extraction d'une ligne */
  /* ---------------------------------------------------------- */

  n = mcmax(mcabs(x2-x1),(y2-y1))+1;
  lx = (int32_t *)calloc(n, sizeof(int32_t));
  ly = (int32_t *)calloc(n, sizeof(int32_t));
  if (image == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  nn = n;
  lbresenlist(x1, y1, x2, y2, lx, ly, &nn);
#ifdef PARANO
  if (nn != n)
  {
    fprintf(stderr, "%s: WARNING: unexpected number of points returned by lbresenlist()\n", argv[0]);
    fprintf(stderr, "expected : %d ; got : %d\n", n, nn);
  }
#endif
  imgres = allocimage(NULL, n, 1, 1, datatype(image));
  R = UCHARDATA(imgres);
  for (i = 0; i < n; i++) R[i] = I[ly[i]*rs + lx[i]];    

  writeimage(imgres, argv[argc-1]);
  freeimage(imgres);
  freeimage(image);
  free(lx);
  free(ly);
  return 0;
} /* main */



