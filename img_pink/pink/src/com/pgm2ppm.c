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
/*! \file pgm2ppm.c

\brief constructs a ppm color image from 3 pgm grayscale image or from a pgm image and a lookup table

<B>Usage:</B> pgm2ppm {r.pgm g.pgm b.pgm|in.pgm in.lut} out.ppm

<B>Description:</B>
1st mode (3 arguments): combines three grayscale images: <B>r.pgm</B>, <B>g.pgm</B> and
<B>b.pgm</B> as the red, green and blue color planes of the color image <B>out.ppm</B>

2nd mode (2 arguments): generates a color ppm image from the image <B>in.pgm</B> and the 
lookup table <B>in.lut</B>

A lookup table is represented by a special ppm image with 1 row. 

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s {r.pgm g.pgm b.pgm|in.pgm in.lut} out.ppm \n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    g1 = readimage(argv[1]);
    g2 = readimage(argv[2]);
    g3 = readimage(argv[3]);
    if ((g1 == NULL) || (g2 == NULL) || (g3 == NULL))
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (argc == 4)
  {
    struct xvimage * lut_r;
    struct xvimage * lut_g;
    struct xvimage * lut_b;
    int32_t ncol, i, N;
    uint8_t *G1, *G2, *G3, *R, *G, *B, c;
    g1 = readimage(argv[1]);
    g2 = copyimage(g1);
    g3 = copyimage(g1);
    readrgbimage(argv[2], &lut_r, &lut_g, &lut_b);
    if ((g1 == NULL) || (g2 == NULL) || (g3 == NULL) || 
        (lut_r == NULL) || (lut_g == NULL) || (lut_b == NULL))
    {
      fprintf(stderr, "%s: readimage or copyimage failed\n", argv[0]);
      exit(1);
    }
    if (colsize(lut_r) != 1)
    {
      fprintf(stderr, "%s: bad lut format\n", argv[0]);
      exit(1);
    }
    ncol = rowsize(lut_r);
    N = rowsize(g1) * colsize(g1);
    G1 = UCHARDATA(g1);
    G2 = UCHARDATA(g2);
    G3 = UCHARDATA(g3);
    R = UCHARDATA(lut_r);
    G = UCHARDATA(lut_g);
    B = UCHARDATA(lut_b);
    for (i = 0; i < N; i++)
    {
      c = G1[i];
      if (c >= ncol)
      {
        fprintf(stderr, "%s: color code incompatible with lut\n", argv[0]);
        exit(1);
      }
      G1[i] = R[c];
      G2[i] = G[c];
      G3[i] = B[c];
    }
    freeimage(lut_r);
    freeimage(lut_g);
    freeimage(lut_b);
  }

  writergbimage(g1, g2, g3, argv[argc-1]);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  return 0;
} /* main */
