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
/*! \file rgb2hls.c

\brief converts a ppm color file to hls representation

<B>Usage:</B> in.ppm [mode] h.pgm l.pgm s.pgm

<B>Description:</B>
Constructs 3 pgm files from 1 ppm file:
\li <B>h.pgm</B> : Hue (int32_t - 0 to 359)
\li <B>h.pgm</B> : Luminance (byte)
\li <B>h.pgm</B> : Saturation (byte)

Different modes are available [default mode is 0]:
\li mode = 0: classical HLS coding, based on [1].
\li mode = 1: L1 norm (NYI) (see [2])
\li mode = 2: L1 norm with gamma correction (NYI) (see [2])

[1] Foley, Van Damm & al: "Computer Graphics", 2nd ed., p. 595

[2] J. Angulo and J. Serra. "Traitements des images de couleur 
en représentation luminance/saturation/teinte par norme L_1" (in French). 
Traitement du Signal, Vol. 21, No. 6, pp 583-604, December 2004.

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/*
** Michel Couprie  -  avril 1999
**
** Quelques valeurs de HUE:
**     0  rouge
**    60  jaune
**   120  vert
**   180  cyan
**   240  bleu
**   300  magenta
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
  double th, tl, ts;
  int32_t mode;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.ppm [mode] h.pgm l.pgm s.pgm\n", argv[0]);
    exit(1);
  }

  if (argc == 6) mode = atoi(argv[2]); else mode = 0;

  if (readrgbimage(argv[1], &R, &G, &B) == 0)
  {
    fprintf(stderr, "%s: readrgbimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(R);
  cs = colsize(R);
  N = rs * cs;

  H = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  L = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  S = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if ((H == NULL) || (L == NULL) || (S == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  r = UCHARDATA(R);
  g = UCHARDATA(G);
  b = UCHARDATA(B);
  h = SLONGDATA(H);
  l = UCHARDATA(L);
  s = UCHARDATA(S);

  if (mode == 0)
  {
    for (x = 0; x < N; x++) 
    {
      rgb2hls(((double)(r[x]))/255, ((double)(g[x]))/255, ((double)(b[x]))/255, &th, &tl, &ts);
      h[x] = (int32_t)th;
      l[x] = (uint8_t)(tl * 255);
      s[x] = (uint8_t)(ts * 255);
    }
  } 
  else
  {
    fprintf(stderr, "%s: mode %d not implemented\n", argv[0], mode);
    exit(1);
  } 

  writeimage(H, argv[argc-3]);
  writeimage(L, argv[argc-2]);
  writeimage(S, argv[argc-1]);
  freeimage(R);
  freeimage(G);
  freeimage(B);
  freeimage(H);
  freeimage(L);
  freeimage(S);
  return 0;
}
