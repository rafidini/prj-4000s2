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
/*! \file asfr.c

\brief alternate sequential filter by reconstruction

<B>Usage:</B> asfr in.pgm [rmin] rmax out.pgm

<B>Description:</B>
Alternate sequential filter by reconstruction. 
Disc-shaped structuring elements of increasing radius,
ranging from 1 (or <B>rmin</B> if this parameter is specified) to <B>rmax</B>,
are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.
Let I0 = <B>in.pgm</B>, the ith intermediate result Ii is obtained by the closure 
by reconstruction of Ii-1 by the structuring element Di. 
The result <B>out.pgm</B> contains the final result In.

<B>Types supported:</B> byte 2d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>
#include <lgeodesic.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * copy;
  int32_t rs, cs, ds, ps, N;
  int32_t rayonmin, rayonmax, rayon;
  int32_t d, xc, yc, x, y, nptb;
  int32_t *tab_es_x, *tab_es_y;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm [rayonmin] rayonmax out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);     /* taille ligne */
  cs = colsize(image);     /* taille colonne */
  ps = rs * cs;            /* taille plan */
  ds = depth(image);       /* nombre plans */
  N = ds * ps;             /* taille image */
  if (argc == 4) 
  {
    rayonmin = 1;
    rayonmax = atoi(argv[2]);
  }
  else
  {
    rayonmin = atoi(argv[2]);
    rayonmax = atoi(argv[3]);
  }

  d = 2 * rayonmax + 1;
  tab_es_x = (int32_t *)calloc(1,d * d * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,d * d * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  copy = copyimage(image);
  if (copy == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", argv[0]);
    exit(1);
  }

  for (rayon = rayonmin; rayon <= rayonmax; rayon++)
  {
#ifdef VERBOSE
    fprintf(stderr, "%s: rayon = %d\n", argv[0], rayon);      
#endif
    /* genere l'element structurant circulaire sous forme de liste des coord. des points */
    yc = xc = rayon; 
    d = 2 * rayon + 1;
    nptb = 0;
    for (x = 0; x < d; x++)
      for (y = 0; y < d; y++)
        if (((x - xc) * (x - xc) + (y - yc) * (y - yc)) <= (rayon * rayon))
	{
          tab_es_x[nptb] = x;
          tab_es_y[nptb] = y;
          nptb++;
	}

#ifdef DILATATION_D_ABORD
    if (! ldilat2(image, nptb, tab_es_x, tab_es_y, xc, yc))
    {
      fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
      exit(1);
    }

    if (! lgeoeros(image, copy, 4, -1))
    {
      fprintf(stderr, "%s: function lgeoeros failed\n", argv[0]);
      exit(1);
    }

    memcpy(UCHARDATA(copy), UCHARDATA(image), N);    

    if (! leros2(image, nptb, tab_es_x, tab_es_y, xc, yc))
    {
      fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
      exit(1);
    }

    if (! lgeodilat(image, copy, 4, -1))
    {
      fprintf(stderr, "%s: function lgeodilat failed\n", argv[0]);
      exit(1);
    }

    if (rayon < rayonmax) memcpy(UCHARDATA(copy), UCHARDATA(image), N);
#else
    if (! leros2(image, nptb, tab_es_x, tab_es_y, xc, yc))
    {
      fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
      exit(1);
    }

    if (! lgeodilat(image, copy, 4, -1))
    {
      fprintf(stderr, "%s: function lgeodilat failed\n", argv[0]);
      exit(1);
    }

    memcpy(UCHARDATA(copy), UCHARDATA(image), N);    

    if (! ldilat2(image, nptb, tab_es_x, tab_es_y, xc, yc))
    {
      fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
      exit(1);
    }

    if (! lgeoeros(image, copy, 4, -1))
    {
      fprintf(stderr, "%s: function lgeoeros failed\n", argv[0]);
      exit(1);
    }

    if (rayon < rayonmax) memcpy(UCHARDATA(copy), UCHARDATA(image), N);
#endif

  } /* for (rayon = 1; rayon <= rayonmax; rayon++) */

  freeimage(copy);

  writeimage(image, argv[argc-1]);
  freeimage(image);
  free(tab_es_x);
  free(tab_es_y);

  return 0;
} /* main */
