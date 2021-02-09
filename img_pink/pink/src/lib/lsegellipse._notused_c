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
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mclifo.h>
#include <mcutil.h>
#include <lbresen.h>
#include <llabelextrema.h>
#include <lsegellipse.h>

/****************************************************************
*
* Routine Name: lsegellipse - library call for segellipse
*
* Purpose: segmentation d'objets de forme elliptique
* Input:
* Output:
* Written By: Micllhel Couprie - novembre 1998
*
* facteur d'ellipticite:
*   a = aire de la composante
*   ni = nombre de pixels de la composante dans l'ellipse de la composante
*   f = ni / a
*
*   ce facteur vaut 1 pour un objet entierement inclus dans son ellipse
*   (mais pas forcement de forme elliptique)
*
****************************************************************/

#define DEBUG
/*
#define VERBOSE
#define DEBUGEXCEN
#define DEBUGORIEN
*/

#define EPSILON 1E-6

/* ==================================== */
int32_t lsegellipse(
        struct xvimage *img,    /* image de depart */
        int32_t connex,             /* 4, 8  */
        int32_t minimum)            /* booleen */
/* ==================================== */
{
  int32_t k, l;
  int32_t w, x, y, z;
  uint8_t *SOURCE = UCHARDATA(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  struct xvimage * labels;
  uint32_t *LABEL;
  uint32_t label;
  Lifo * LIFO;
  int32_t area;
  double mx1, my1, mx2, my2, mxy2;
  double Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;
  int32_t incr_vois;
  double xx, yy, xc, yc, a;
  double x1, y1, x2, y2;
  int32_t sign;
  double theta;
  ellipse *ell;
  uint8_t ellipticite;
  int32_t ni;

  if (depth(img) != 1) 
  {
    fprintf(stderr, "lsegellipse: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "lsegellipse: mauvaise connexite: %d\n", connex);
      return 0;
  } /* switch (connex) */

  labels = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "lsegellipse: allocimage failed\n");
    return 0;
  }
  LABEL = SLONGDATA(labels);

  if (!llabelextrema(img, connex, minimum, labels, &n))
  {
    fprintf(stderr, "lsegellipse: llabelextrema failed\n");
    return 0;
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lattribute() : CreeLifoVide failed\n");
      return(0);
  }

  ell = AllocEllipseList(rs, cs);

  for (x = 0; x < N; x++)
  {
    if (LABEL[x])
    {
#ifdef DEBUG
printf("point %d %d : nouvelle composante\n", x%rs, x/rs);
#endif
      label = LABEL[x];
      area = 0; mx1 = my1 = mx2 = my2 = mxy2 = 0.0;
      LifoPush(LIFO, x);         /* on va parcourir 2x la comp. a laquelle appartient x */
      while (! LifoVide(LIFO))   /* premier parcours */
      {
        w = LifoPop(LIFO);
        LABEL[w] = n+1; /* marquage provisoire */
        area++; mx1 += w%rs; my1 += w/rs; mxy2 += (w%rs) * (w/rs);
        mx2 += (w%rs) * (w%rs); my2 += (w/rs) * (w/rs);
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == label)) LifoPush(LIFO, y);
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

      /* segellipse centres d'ordre 2 */
      Mx2 = mx2 - mx1 * mx1 / area;  
      My2 = my2 - my1 * my1 / area;  
      Mxy2 = mxy2 - mx1 * my1 / area;

      /* calcul des longueurs des demi-axes */
      delta = (Mx2 - My2) * (Mx2 - My2) + 4 * Mxy2 * Mxy2;
      lambda1 = sqrt(2 * (Mx2 + My2 + sqrt(delta)) / area);
      lambda2 = sqrt(2 * (Mx2 + My2 - sqrt(delta)) / area);
#ifdef DEBUG
printf("Mx2 = %g ; My2 = %g ; Mxy2 = %g ; delta = %g\n", Mx2, My2, Mxy2, delta);
printf("lambda1 = %g ; lambda2 = %g\n", lambda1, lambda2);
#endif
      if (lambda2 < 0.0)
      {
        fprintf(stderr, "lsegellipse : valeur propre negative : %g\n", lambda2);
        return 0;
      }
      if (abs(lambda1 - Mx2) < EPSILON) 
        theta = 0.0;
      else
      {
        a = Mxy2 / (lambda1 - Mx2);
        if (a < 0) sign = -1; else sign = 1; 
        a = a * a;
        xx = sqrt(a / (1 + a));
        yy = sign * sqrt(1 / (1 + a));
#ifdef DEBUG
printf("xx = %g ; yy = %g ; atan(yy/xx) = %g\n", xx, yy, atan(yy/xx));
#endif
        theta = atan(yy/xx);
        if (theta < 0) theta += M_PI;
      }
      xc = mx1/area;
      yc = my1/area;

      x1 = lambda1 * cos(theta);
      y1 = lambda1 * sin(theta);
      x2 = lambda1 * excen * cos(theta + M_PI/2);
      y3 = lambda1 * excen * sin(theta + M_PI/2);
      lellipselist(ell, rs, cs, x1, y1, x2, y2, xc, yc);
      ni = 0;

      LABEL[x] = n+2;            /* nouveau label */
      LifoPush(LIFO, x);         /* on re-parcourt la composante */
      while (! LifoVide(LIFO))   /* et on calcule le facteur d'ellipticite */
      {
        w = LifoPop(LIFO);
        if (InEllipse(ell)) ni++;
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n + 1)) 
          {
            LABEL[y] = n + 2;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */          
      } /* while (! LifoVide(LIFO)) */

      ellipticite = (uint8_t)((255 * ni) / area);

      LABEL[x] = 0;              /* label nul */
      LifoPush(LIFO, x);         /* on re-parcourt la composante */
      while (! LifoVide(LIFO))   /* pour la demarquer et stocker l'ellipticite dans img */
      {
        w = LifoPop(LIFO);
        SOURCE[w] = ellipticite;        
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n + 2)) 
          {
            LABEL[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

    } /* if (LABEL[x]) */
  } /* for (x = 0; x < N; x++) */


  FreeEllipseList(ell);
  LifoTermine(LIFO);
  freeimage(labels);

  return(1);
}

