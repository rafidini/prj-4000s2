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
/*! \file crestheight.c

\brief measures the height of crests in a thinned grayscale image

<B>Usage:</B> crestheight skel.pgm connex out.pgm

<B>Description:</B>
Let F be the input image (parameter <B>skel.pgm</B>), and
let R be the output image (parameter <B>out.pgm</B>).
For each point p which is separating for F (connexity
given by the parameter <B>connex</B>), the output value R(p) is
equal to calpha8m(F,S,p).

References:<BR> 
"Topological operators for the detection of
curvilinar structures in grayscale images", in preparation, 2003.

<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>

#define NORMALIZE

/* ==================================== */
int32_t lcrestheight(
  struct xvimage *skel, 
  int32_t connex)
/* ==================================== */
#undef F_NAME
#define F_NAME "lcrestheight"
{ 
  int32_t p;
  int32_t rs = rowsize(skel);     /* taille ligne */
  int32_t cs = colsize(skel);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *S = UCHARDATA(skel);      /* l'image de squelette */
  struct xvimage *sp;
  uint8_t *SP;
  struct xvimage *r;
  uint8_t *R;
  uint8_t vmax = 0;

  if (depth(skel) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    return(0);
  }

  sp = copyimage(skel);
  r = copyimage(skel);
  if ((sp == NULL) || (r == NULL))
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return(0);
  }
  SP = UCHARDATA(sp);
  R = UCHARDATA(r);

  for (p = 0; p < N; p++) if (separant4(S, p, rs, N)) SP[p] = 255; else SP[p] = 0;

  for (p = 0; p < N; p++)
  {
    if (SP[p])
    {
      R[p] = S[p] - calpha8m(S, SP, p, rs, N);
#ifdef NORMALIZE
      if (R[p] > vmax) vmax = R[p];
#endif
    }
    else
      R[p] = 0;
  }

#ifdef NORMALIZE
  for (p = 0; p < N; p++)
    R[p] = (255 * R[p]) / vmax;
#endif

  memcpy(S, R, N);

  freeimage(sp);
  freeimage(r);
  return(1);
} /* lcrestheight() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * skel;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s skel.pgm connex out.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  skel = readimage(argv[1]);
  if (skel == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lcrestheight(skel, connex))
  {
    fprintf(stderr, "%s: lcrestheight failed\n", argv[0]);
    exit(1);
  }

  writeimage(skel, argv[argc-1]);
  freeimage(skel);

  return 0;
} /* main */
