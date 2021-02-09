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
/*! \file crestsegment.c

\brief measures the height of crests in a thinned grayscale image

<B>Usage:</B> crestsegment skel.pgm connex niter base incr out.pgm

<B>Description:</B>
Let F be the input image (parameter <B>skel.pgm</B>), and
let R be the output image (parameter <B>out.pgm</B>).

TO BE WRITTEN

Reference:<BR> 
"Topological operators for the detection of
curvilinar structures in grayscale images", in preparation, 2003.

<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclifo.h>
#include <mctopo.h>
#include <mcindic.h>
#include <mcutil.h>

/*
   principe de l'encodage du alpha-: 
   la valeur alpha- de x est placee dans les bits 24 a 31 de l'int32_t contenant x
                                                      24
   il reste donc 24 bits pour coder les pixels, soit 2   = 16 megapixels.
   (ex: image 4k x 4k)

   une valeur -1 pour nitermax signifie de laisser calculer jusqu'a stabilite (en fait
   2000000000 iterations) 
*/

#define ENCODE(y,a) (y|(a<<24))
#define DECODEX(y) (y&0x00ffffff)
#define DECODEA(y) ((y>>24)&0xff)

/* ==================================== */
int32_t lcrestsegment(struct xvimage *image, int32_t connex, int32_t nitermax, int32_t base, int32_t incr)
/* ==================================== */
#undef F_NAME
#define F_NAME "lcrestsegment"
#define EN_LIFO 0
#define ROBUSTE 1
{ 
  int32_t i, k, a, h;
  int32_t x, y;                 /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t niter;                   /* nombre d'iterations effectuees */
  Lifo * LIFO1;
  Lifo * LIFO2;
  int32_t incr_vois;
  int32_t *mu;

#ifdef VERBOSE
  fprintf(stderr, "%s: nitermax = %d\n", F_NAME, nitermax);
#endif

  mu = (int32_t *)calloc(1,nitermax * sizeof(int32_t));
  if (mu == NULL) 
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    return(0);
  }
  for (i = 0; i < nitermax; i++) mu[i] = base + i * incr;

  if (connex == 4) incr_vois = 2; else incr_vois = 1;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    return(0);
  }

  if (N > (1<<24)) 
  {
    fprintf(stderr, "%s: image trop grande (limite 16 M)\n", F_NAME);
    return(0);
  }

  IndicsInit(N);

  LIFO1 = CreeLifoVide(N);
  LIFO2 = CreeLifoVide(N);
  if ((LIFO1 == NULL) || (LIFO2 == NULL))
  {   fprintf(stderr, "%s() : CreeLifoVide failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DE LA LIFO: empile tous les points destructibles avec leur delta-  */
  /* ========================================================= */

  if (connex == 4)
  {
    for (x = 0; x < N; x++) 
      if (pdestr4(F,x,rs,N)) LifoPush(LIFO1, ENCODE(x,delta4m(F,x,rs,N)));
  }
  else
  {
    fprintf(stderr, "%s() : connex 8 NYI\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT ITERATIONS                */
  /* ================================================ */

  niter = 0;

  if (connex == 4)
  {
    while (!LifoVide(LIFO1) && (niter < nitermax))
    {
      niter++;

#ifdef VERBOSE
  fprintf(stderr, "%s: niter = %d\n", F_NAME, niter);
#endif

  /* --------------------------------------------------------- */
  /* 1ere demi iteration : on abaisse les points destructibles */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO1))
      {
        y = LifoPop(LIFO1);
        x = DECODEX(y);
        a = DECODEA(y);
        UnSet(x,EN_LIFO);

        if (pdestr4(F, x, rs, N) && !separant4(F, x, rs, N))
	{
          F[x] = mcmax(delta4m(F, x, rs, N),a);
          LifoPush(LIFO2, x);
        } /* if (pdestr4(F, x, rs, N)) */

      } /* while (!LifoVide(LIFO1)) */

  /* --------------------------------------------------------- */
  /* 2eme demi iteration : on empile les voisins */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO2))
      {
        x = LifoPop(LIFO2);
        if ((! IsSet(x,EN_LIFO)) && (pdestr4(F,x,rs,N)))
        {
          LifoPush(LIFO1, ENCODE(x,delta4m(F,x,rs,N)));
          Set(x,EN_LIFO);
	}
        for (k = 0; k < 8; k += 1)             /* parcourt les voisins */
        {                                      /* pour empiler les voisins */
          y = voisin(x, k, rs, N);             /* non deja empiles */
          if ((y != -1) && (! IsSet(y,EN_LIFO)) && (pdestr4(F,y,rs,N)))
          {
            LifoPush(LIFO1, ENCODE(y,delta4m(F,y,rs,N)));
            Set(y,EN_LIFO);
          } /* if y */
        } /* for k */      
      } /* while (!LifoVide(LIFO2)) */

      /* --------------------------------------------------------- */
      /* experimental : on etiquete les points separants p 
         tq F[p]-alpha-(p) >= mu[niter] */
      /* --------------------------------------------------------- */
      for (x = 0; x < N; x++) 
        if ((h = separant4(F, x, rs, N)) && 
            ((F[x] - halpha8m(F, x, h, rs, N)) >= mu[niter-1]))
          Set(x,ROBUSTE);

    } /* while (! (LifoVide(LIFO1) ...)) */
  } /* if (connex == 4) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  for (x = 0; x < N; x++) 
    if (IsSet(x,ROBUSTE)) F[x] = 255; else F[x] = 0;

  IndicsTermine();
  LifoTermine(LIFO1);
  LifoTermine(LIFO2);
  return(1);
} /* lcrestsegment() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, niter, base, incr;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s image.pgm connex niter base incr out.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }
  niter = atoi(argv[3]);
  base = atoi(argv[4]);
  incr = atoi(argv[5]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lcrestsegment(image, connex, niter, base, incr))
  {
    fprintf(stderr, "%s: lcrestsegment failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
