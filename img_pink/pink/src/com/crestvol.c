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
/*! \file crestvol.c

\brief enhances linear structures in a grayscale image

<B>Usage:</B> crestvol in.pgm connex radius out.pgm

<B>Description:</B>
Enhances linear structures in a grayscale image.
The algorithm is the following:

\verbatim
Let F be the original image ("in.pgm")
Let O be the final result ("out.pgm")
Let r be the value of parameter "radius"
Compute k iterations of homotopic thinning, result S
Lower all separating points p in S down to alpha8m(S, p), result Sp 
For each point x of S which is separating
  Let Dx be the image with D(x) = F(x), and D(y) = 0 for all y != x
  Let Fx be the geodesic reconstruction of Dx under F 
  O(x) = volume((Fx-Sp).Br(x)) 
  where Br(x) is the disc centered on x with radius r
EndFor
\endverbatim

References:<BR> 
"Topological operators for the detection of
curvilinar structures in grayscale images", in preparation.

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
#include <lgeodesic.h>
#include <lhtkern.h>

/* ==================================== */
int32_t lcrestvol(
  struct xvimage *skel, 
  struct xvimage *orig, 
  int32_t connex, 
  struct xvimage *es, 
  int32_t es_x, int32_t es_y)
/* ==================================== */
#undef F_NAME
#define F_NAME "lcrestvol"
{ 
  int32_t p, x, y, xx, yy, xf, yf, i, j;
  int32_t rs = rowsize(skel);     /* taille ligne */
  int32_t cs = colsize(skel);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *S = UCHARDATA(skel);      /* l'image de squelette */
  uint8_t *F = UCHARDATA(orig);      /* l'image originale */
  uint32_t *V, maxvol;
  int32_t rs_es = rowsize(es);
  int32_t cs_es = colsize(es);
  int32_t N_es = rs_es * cs_es;
  uint8_t *E = UCHARDATA(es);
  struct xvimage *t1, *t2; 
  uint8_t *T1, *T2;
  struct xvimage *sp;
  uint8_t *SP;

  if (depth(skel) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    return(0);
  }

  if ((rowsize(orig) != rs) || (colsize(orig) != cs) || (depth(orig) != 1))
  {
    fprintf(stderr, "%s: tailles skel et orig incompatibles\n", F_NAME);
    return(0);
  }

  V = (uint32_t *) calloc(N, sizeof(int32_t));
  if (V == NULL)
  {
    fprintf(stderr, "%s: calloc failed\n", F_NAME);
    return(0);
  }

  sp = copyimage(skel);
  if (sp == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return(0);
  }
  SP = UCHARDATA(sp);
  for (p = 0; p < N; p++)
    if (separant4(S, p, rs, N))
      SP[p] = alpha8m(S, p, rs, N);

  t1 = allocimage(NULL, rs_es, cs_es, 1, VFF_TYP_1_BYTE);
  t2 = allocimage(NULL, rs_es, cs_es, 1, VFF_TYP_1_BYTE);
  if ((t1 == NULL) || (t2 == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return(0);
  }
  T1 = UCHARDATA(t1);
  T2 = UCHARDATA(t2);


  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */


  maxvol = 0;
  if (connex == 4)
  {
    for (p = 0; p < N; p++)
    {
      if (separant4(S, p, rs, N))
      {
        uint32_t vol = 0;
        x = p % rs; y = p / rs;

        // recopie le voisinage de p pour F dans T1

        for (xx = 0; xx < rs_es; xx++)
          for (yy = 0; yy < cs_es; yy++)
            if (E[yy*rs_es + xx])
            {
              xf = xx - es_x + x;
              yf = yy - es_y + y;
              if ((xf >= 0) && (xf < rs) && (yf >= 0) && (yf < cs))
	      {
                T1[yy*rs_es + xx] = F[yf*rs + xf];
	      }
	    }

        // T2 = pic en p, 0 ailleurs

        memset(T2, 0, N_es);
        T2[es_y*rs_es + es_x] = F[p]; 

        // reconstruction de T2 sous T1 (resultat dans T2)

        if (!lgeodilat(t2, t1, 8, -1))
        {
          fprintf(stderr, "%s: lgeodilat failed\n", F_NAME);
          return(0);
        }

        // recopie le voisinage de p pour SP dans T1

        memset(T1, 0, N_es);
        for (xx = 0; xx < rs_es; xx++)
          for (yy = 0; yy < cs_es; yy++)
	  {
            i = yy*rs_es + xx;
            if (E[i])
            {
              xf = xx - es_x + x;
              yf = yy - es_y + y;
              if ((xf >= 0) && (xf < rs) && (yf >= 0) && (yf < cs))
	      {
                j = yf*rs+xf;
                T1[i] = SP[j];
                if (T2[i] < T1[i])
                  T1[i] = T2[i];
                // volume de (T2 - T1) . E
                vol += (T2[i] - T1[i]);
	      }
	    }
	  }
        
        V[p] = vol;
        if (vol > maxvol) maxvol = vol;
      }
    } // for (p = 0; p < N; p++)
  } // if (connex == 4)
  else
  {
    fprintf(stderr, "%s: connex 8 NYI\n", F_NAME);
    return(0);
  }

  printf("maxvol = %ld\n", (long int)maxvol);

  for (p = 0; p < N; p++)
    S[p] = (uint8_t)((V[p] * 255) / maxvol);

  free(V);  
  freeimage(sp);
  freeimage(t1);
  freeimage(t2);
  return(1);
} /* lcrestvol() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * orig;
  struct xvimage * skel;
  int32_t radius;
  int32_t connex;
  int32_t x, y, xc, yc, r2, rs_es, cs_es;
  struct xvimage * es;
  uint8_t *E;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm connex radius out.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  radius = atoi(argv[3]);

  orig = readimage(argv[1]);
  if (orig == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  skel = copyimage(orig);
  if (skel == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", argv[0]);
    exit(1);
  }

  // generation de l'ES
  r2 = radius * radius;
  cs_es = rs_es = 2*radius + 1;  
  xc = yc = radius;
  es = allocimage(NULL, rs_es, cs_es, 1, VFF_TYP_1_BYTE);
  if (es == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  E = UCHARDATA(es);
  for (x = 0; x < rs_es; x++)
    for (y = 0; y < cs_es; y++)
      if (((x-xc)*(x-xc) + (y-yc)*(y-yc)) <= r2)
        E[y*rs_es + x] = 255;
      else
        E[y*rs_es + x] = 0;

  if (! lhthindelta(skel, NULL, radius, connex))
  {
    fprintf(stderr, "%s: lhthin failed\n", argv[0]);
    exit(1);
  }

  if (! lcrestvol(skel, orig, connex, es, xc, yc))
  {
    fprintf(stderr, "%s: lcrestvol failed\n", argv[0]);
    exit(1);
  }

  writeimage(skel, argv[argc-1]);
  freeimage(skel);
  freeimage(orig);
  freeimage(es);

  return 0;
} /* main */
