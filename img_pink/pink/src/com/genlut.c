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
/*! \file genlut.c

\brief generates a lookup table

<B>Usage:</B> genlut ncol minhue maxhue minlum maxlum minsat maxsat  out.lut

<B>Description:</B> Generates a lookup table with a continuous color gradient.

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
#include <mccolors.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * lut_r;
  struct xvimage * lut_g;
  struct xvimage * lut_b;
  int32_t i, ncol;
  double minhue, maxhue, minlum, maxlum, minsat, maxsat;
  double r, g, b, hue, lum, sat;
  uint8_t *R, *G, *B;
  int32_t rint, gint, bint;

  if (argc != 9)
  {
    fprintf(stderr, "usage: %s ncol minhue maxhue minlum maxlum minsat maxsat  out.lut\n", argv[0]);
    exit(1);
  }

  ncol = atoi(argv[1]);
  minhue = atof(argv[2]);
  maxhue = atof(argv[3]);
  minlum = atof(argv[4]);
  maxlum = atof(argv[5]);
  minsat = atof(argv[6]);
  maxsat = atof(argv[7]);
  lut_r = allocimage(NULL, ncol, 1, 1, VFF_TYP_1_BYTE);
  lut_g = allocimage(NULL, ncol, 1, 1, VFF_TYP_1_BYTE);
  lut_b = allocimage(NULL, ncol, 1, 1, VFF_TYP_1_BYTE);
  if ((lut_r == NULL) || (lut_g == NULL) || (lut_b == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  R = UCHARDATA(lut_r);
  G = UCHARDATA(lut_g);
  B = UCHARDATA(lut_b);

  for (i = 0; i < ncol; i++)
  {
    hue = (double)(minhue + ((maxhue-minhue)*i) / (double)ncol);
    while (hue < 0.0) hue += 360.0;
    while (hue >= 360.0) hue -= 360.0;
    lum = (double)(minlum + ((maxlum-minlum)*i) / (double)ncol);
    sat = (double)(minsat + ((maxsat-minsat)*i) / (double)ncol);
    hls2rgb(&r, &g, &b, hue, lum, sat);
    rint = (int32_t)(r*256);
    gint = (int32_t)(g*256);
    bint = (int32_t)(b*256);
    if (rint>255) R[i]=255; else R[i]=(uint8_t)rint;
    if (gint>255) G[i]=255; else G[i]=(uint8_t)gint;
    if (bint>255) B[i]=255; else B[i]=(uint8_t)bint;
  }

  writergbimage(lut_r, lut_g, lut_b, argv[argc-1]);
  freeimage(lut_r);
  freeimage(lut_g);
  freeimage(lut_b);

  return 0;
} /* main */
