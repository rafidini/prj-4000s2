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
/* \file seuilhistolocal.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  seuillage base sur l'histogramme
  on veut obtenir une proportion p de pixels blancs

  Michel Couprie - avril 1998

  Modif fevrier 2000 :
    histogramme calcule localement (fenetre glissante)  
    
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

#define WINRAD 25         /* fenetre de taille 2 * WINRAD + 1 */ 

#define DEBUG

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * result;
  struct xvimage * masque;
  uint32_t * histo;
  int32_t i, j, k, h, seuil;
  int32_t nbpts, pnbpts, n, x, y;
  int32_t rs, cs, d, N;
  uint8_t *I;
  uint8_t *R;
  uint8_t *M;
  double p;

#ifdef DEBUG
fprintf(stderr,"seuilhistolocal : 1\n");
#endif

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm p [mask.pgm] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  p = 1.0 - atof(argv[2]);
  
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;
  I = UCHARDATA(image);

  result = copyimage(image);
  R = UCHARDATA(result);
  memset(R, 0, N);

  histo = (uint32_t *)calloc((NDG_MAX - NDG_MIN + 1), sizeof(int32_t));
  if (histo == NULL)
  {
    fprintf(stderr, "%s: calloc failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {

#ifdef DEBUG
fprintf(stderr,"seuilhistolocal : 2\n");
#endif

    masque = readimage(argv[3]);
    if (masque == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    M = UCHARDATA(masque);
    for (y = WINRAD; y < cs - WINRAD; y++)
    {
#ifdef DEBUG
fprintf(stderr,"y = %d\n", y);
#endif
      for (x = WINRAD; x < rs - WINRAD; x++)
      {
        if (M[y * rs + x])
        {
          /* initialise l'histo */
          memset(histo, 0, (NDG_MAX - NDG_MIN + 1) * sizeof(int32_t));
          nbpts = 0;
          for (j = -WINRAD; j <= WINRAD; j++) 
            for (i = -WINRAD; i <= WINRAD; i++) 
            {
              k = (y+j)*rs + x+i;
              if (M[k]) { histo[I[k]]++; nbpts++; }
  	    }
          pnbpts = (int32_t)(p * nbpts); n = 0;
          for (h = NDG_MIN; h <= NDG_MAX; h++) 
            { n += histo[h]; if (n >= pnbpts) { seuil = h; break; } }
          if (I[k] >= seuil) R[k] = NDG_MAX; else R[k] = NDG_MIN;
	} /* if (M[y * rs + x]) */
      } /* for (x = WINRAD; x < rs - WINRAD; x++) */
    } /* for (y = WINRAD; y < cs - WINRAD; y++) */
  }
  else
  {
    nbpts = 2 * WINRAD + 1;
    nbpts = nbpts * nbpts;
    pnbpts = (int32_t)(p * nbpts);

    for (y = WINRAD; y < cs - WINRAD; y++)
    {
#ifdef DEBUG
fprintf(stderr,"y = %d\n", y);
#endif
      for (x = WINRAD; x < rs - WINRAD; x++)
      {
        /* initialise l'histo */
        memset(histo, 0, (NDG_MAX - NDG_MIN + 1) * sizeof(int32_t));
        for (j = -WINRAD; j <= WINRAD; j++) 
          for (i = -WINRAD; i <= WINRAD; i++) 
          {
            k = (y+j)*rs + x+i;
            histo[I[k]]++;
  	  }
        n = 0;
        for (h = NDG_MIN; h <= NDG_MAX; h++) 
          { n += histo[h]; if (n >= pnbpts) { seuil = h; break; } }
        if (I[k] >= seuil) R[k] = NDG_MAX; else R[k] = NDG_MIN;
      } /* for (x = WINRAD; x < rs - WINRAD; x++) */
    } /* for (y = WINRAD; y < cs - WINRAD; y++) */
  }

  memcpy(I, R, N);
  writeimage(image, argv[argc-1]);
  freeimage(result);
  free(histo);
  freeimage(image);
  if (argc == 5) freeimage(masque);

  return 0;
} /* main */












