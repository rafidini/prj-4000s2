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
/*! \file hls2rgb.c

\brief converts from hls to rgb color representation

<B>Usage:</B> hls2rgb H.pgm L.pgm S.pgm out.ppm

<B>Description:</B>

Constructs 1 ppm from 3 pgm files:
   <B>H.pgm</B> : Hue (int32_t - 0 to 359)
   <B>L.pgm</B> : Luminance (byte)
   <B>S.pgm</B> : Saturation (byte)
Some particular values of HUE:
     0  red
    60  yellow
   120  green
   180  cyan
   240  blue
   300  magenta
Based on Foley, Van Damm & al: "Computer Graphics", 2nd ed., p. 595

Types supported: int32_t 2D (hue), byte 2D (others)

<B>Types supported:</B> byte 2D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/*
 Michel Couprie  -  avril 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <mccolors.h>

int main(int argc, char **argv)
{
  struct xvimage * R;
  struct xvimage * G;
  struct xvimage * B;
  struct xvimage * H;
  struct xvimage * L;
  struct xvimage * S;
  uint8_t *r, *g, *b;
  int32_t *h;
  uint8_t *l, *s;
  int32_t rs, cs, N;
  int32_t x;
  double tr, tg, tb;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s h.pgm l.pgm s.pgm out.ppm\n", argv[0]);
    exit(1);
  }

  H = readimage(argv[1]);
  L = readimage(argv[2]);
  S = readimage(argv[3]);
  if ((H == NULL) || (L == NULL) || (S == NULL))
  {
    fprintf(stderr, "hls2rgb: readrgbimage failed\n");
    exit(1);
  }
  if (datatype(H) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "hls2rgb: h must be int32_t\n");
    exit(1);
  }

  rs = rowsize(H);
  cs = colsize(H);
  N = rs * cs;

  R = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  G = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  B = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if ((R == NULL) || (G == NULL) || (B == NULL))
  {
    fprintf(stderr, "hls2rgb: allocimage failed\n");
    exit(1);
  }

  r = UCHARDATA(R);
  g = UCHARDATA(G);
  b = UCHARDATA(B);
  h = SLONGDATA(H);
  l = UCHARDATA(L);
  s = UCHARDATA(S);

  for (x = 0; x < N; x++) 
  {
    hls2rgb(&tr, &tg, &tb, (double)h[x], ((double)l[x])/255, ((double)s[x])/255);
    r[x] = (uint8_t)(tr * 255);
    g[x] = (uint8_t)(tg * 255);
    b[x] = (uint8_t)(tb * 255);
  }

  writergbimage(R, G, B, argv[4]);

  freeimage(R);
  freeimage(G);
  freeimage(B);
  freeimage(H);
  freeimage(L);
  freeimage(S);
  return 0;
}
