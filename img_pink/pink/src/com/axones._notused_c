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
/* rotation et superposition d'images de coupes d'axones  */
/* Michel Couprie - mai 1997, juillet 1998 */

/*
#define DESSINETRAPEZE
#define DESSINETUBULE
#define VERBOSE
*/


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llabelplateaux.h>
#include <lwarp.h>

/* ==================================== */
int32_t laxones(
  struct xvimage * image,     /* image de la coupe en niveaux de gris */
  struct xvimage * imaxes     /* image des extremites des axes des microtubules */
  )
/* ==================================== */
/*
  Identifie les transformations lineaires necessaires pour superposer les microtubules
  Effectue ces transformations (warp) et moyenne les images superposees
*/
{
  int32_t i, j, k, c, npts;
  uint8_t *F;        /* image de travail (centres au debut) */
  uint8_t *G;        /* image de depart */
  char *name1, *name;
  int32_t rs, cs, N;
  double bx;               /* coordonnees du centre global */
  double by;
  double *bxx;             /* coordonnees des points extremites */
  double *byy;
  uint8_t *lab;      /* labels des points extremites */
  uint32_t *CUMUL;
  int32_t *vois;               /* fonction voisin */             
  int32_t *orie;               /* orientation de la paire : 1 pour le premier, 2 pour le second */
  double d, t;
  double mind;
  triangle ts, td;         /* pour le warping */
  int32_t first;
  int32_t nbim;                /* nombre d'images generees */
  double xN, yN;           /* composantes du vecteur normal a AB */
  double cosanglemin;
  double ckx, cky, ux, uy, vx, vy;
  double rayon;

  rs = imaxes->row_size;
  cs = imaxes->col_size;
  if ((rowsize(image) != rs) || (colsize(image) != cs))
  {
    fprintf(stderr, "laxones: incompatible image sizes\n");
    return 0;
  }
  N = rs * cs;
  F = UCHARDATA(imaxes);
  G = UCHARDATA(image);

  CUMUL = (uint32_t *)calloc(1,N * sizeof(int32_t));
  bxx = (double *)calloc(1,18 * sizeof(double));
  byy = (double *)calloc(1,18 * sizeof(double));
  lab = (uint8_t *)calloc(1,18 * sizeof(char));
  vois = (int32_t *)calloc(1,18 * sizeof(int32_t));
  orie = (int32_t *)calloc(1,18 * sizeof(int32_t));
  if ((bxx == NULL) || (byy == NULL) || (lab == NULL) || (vois == NULL) || (orie == NULL))
  {
    fprintf(stderr, "laxones: malloc failed\n");
    exit(0);
  }

  /* ---------------------------------------------------------- */
  /* recuperation des points extremites  */
  /* ---------------------------------------------------------- */

  k = 0;
  for (j = 0; j < cs; j++)
    for (i = 0; i < rs; i++)
    {
      if (F[j * rs + i] != 0)
      {
        bxx[k] = (double)i;
        byy[k] = (double)j;
        lab[k] = F[j * rs + i];
        k += 1;
      }
    }
  if (k != 18) printf("nb. points %d < 18", k);

  /* calcule l'equation du cercle */
  if (identcercle(18, bxx, byy, &bx, &by, &rayon) == 0)
  {
    fprintf(stderr, "laxones: identcercle failed\n");
    exit(0);
  }
#ifdef VERBOSE
  printf("identcercle : bx = %g ; by = %g ; r = %g\n", bx, by, rayon);
#endif

  /* ---------------------------------------------------------- */
  /* determination des voisins, des orientations et de l'angle de cos min.  */
  /* ---------------------------------------------------------- */

  for (c = 0; c < 18; c++)
    for (j = 0; j < 18; j++)
      if ((c != j) && (lab[c] == lab[j]))
        vois[c] = j; 

  cosanglemin = 1.0;
  first = -1;
  for (c = 0; c < 18; c++)
  {
    if (lab[c])
    {
      k = vois[c];
#ifdef VERBOSE
  printf("paire: c = %d (%g,%g) , k = %d (%g,%g)\n", c, bxx[c], byy[c], k, bxx[k], byy[k]);
#endif
      /* reorientation eventuelle de la paire */
      /* on teste le signe du determinant */     
      if (((bxx[c]-bx)*(byy[k]-byy[c]) - (byy[c]-by)*(bxx[k]-bxx[c]))*d < 0)
      {
#ifdef VERBOSE
	printf("reorientation %d <-> %d\n", c, k);
#endif
        t = bxx[c]; bxx[c] = bxx[k]; bxx[k] = t;
        t = byy[c]; byy[c] = byy[k]; byy[k] = t;
        j = lab[c]; lab[c] = lab[k]; lab[k] = j;
      }
      orie[c] = 1;
      orie[k] = 2;
      ckx = (bxx[c] + bxx[k]) / 2;
      cky = (byy[c] + byy[k]) / 2; /* barycentre de c et de k */
      ux = ckx - bx;
      uy = cky - by;
      vx = bxx[k] - bxx[c];
      vy = byy[k] - byy[c];
      t = (ux * vx + uy * vy) / sqrt((ux*ux + uy*uy) * (vx*vx + vy*vy)); /* cos(u,v) */
      if (t < 0.0) t = -t;
      if (t < cosanglemin) { cosanglemin = t; first = c; } 
    }
    lab[c] = lab[k] = 0;    
  }

  /* ---------------------------------------------------------- */
  /* determination des triangles et warps */
  /* ---------------------------------------------------------- */

  for (i = 0; i < N; i++) F[i] = G[i]; /* plus besoin des centres */
  memset(CUMUL, 0, N*sizeof(int32_t));

  c = first;
  k = vois[c];
#ifdef VERBOSE
  printf("first: c = %d (%g,%g) , k = %d (%g,%g)\n", c, bxx[c], byy[c], k, bxx[k], byy[k]);
#endif

{
  double cx = bxx[c] - bx; 
  double cy = byy[c] - by;
  double kx = bxx[k] - bx; 
  double ky = byy[k] - by;
  double bc = sqrt(cx*cx + cy*cy);
  double cost = cx / bc;
  double sint = cy / bc;
  td.xa = bx;  
  td.ya = by;
  td.xb =  cx*cost + cy*sint + bx;
  td.yb = -cx*sint + cy*cost + by;
  td.xc =  kx*cost + ky*sint + bx;
  td.yc = -kx*sint + ky*cost + by;
}
  for (c = 0; c < 18; c++)
  {
    if (orie[c] == 1)
    {
      k = vois[c];
      ts.xa = bx;  
      ts.ya = by;
      ts.xb = bxx[c];  
      ts.yb = byy[c];
      ts.xc = bxx[k];  
      ts.yc = byy[k];
      if (! lwarp(imaxes, &ts, &td, 1))
      {
        fprintf(stderr, "laxones: function lwarp failed\n");
        exit(0);
      }          
      for (i = 0; i < N; i++) CUMUL[i] += F[i]; 
                                      /* accumule pour la moyenne */
      for (i = 0; i < N; i++) F[i] = G[i];
    }
  }

  for (i = 0; i < N; i++) G[i] = (uint8_t)(CUMUL[i]/9);

  /* ---------------------------------------------------------- */
  /* un peu de menage */
  /* ---------------------------------------------------------- */

  free(bxx);
  free(byy);
  free(lab);
  free(vois);
  free(orie);
  free(CUMUL);
  return 1;
} // laxones()

/* ==================================== */
int32_t laxones_sav(
  struct xvimage * image,     /* image de la coupe en niveaux de gris */
  struct xvimage * imcentres, /* image binaire des "centres" des cercles sombres */
  double *xc,
  double *yc,                 /* coord. centre global */
  double *xc1,
  double *yc1,                /* coord. centre tubule 1 */
  double *xc2,
  double *yc2                 /* coord. centre tubule 2 */
  )
/* ==================================== */
/*
  Identifie les transformations lineaires necessaires pour superposer les microtubules
  Effectue ces transformations (warp) et moyenne les images superposees
  Retourne les coordonnees des centres des microtubules et du centre global de la coupe
*/
{
  int32_t i, j, k, c, npts;
  uint8_t *F;        /* image de travail (centres au debut) */
  uint8_t *G;        /* image de depart */
  char *name1, *name;
  int32_t rs, cs, N;
  struct xvimage * lab;
  uint32_t *LABEL;    /* pour les labels des composantes connexes */
  int32_t nblabels;
  uint32_t label;
  double bx;               /* coordonnees du centre global */
  double by;
  double *bxx;             /* pour les tables de barycentres par composante */
  double *byy;
  int32_t *surf;               /* surface des composantes */
  double rayon;            /* rayon de la structure globale */
  double petitrayon =11.0; /* rayon de la structure locale (petit cercle) */
  int32_t *vois;               /* plus proche voisin */             
  int32_t *orie;               /* orientation de la paire : 1 pour le premier, 2 pour le second */
  double d, t;
  double mind;
  triangle ts, td;         /* pour le warping */
  int32_t first;
  int32_t nbim;                /* nombre d'images generees */
  double xA, yA, xB, yB;   /* centres de la superposition */
  double xN, yN;           /* composantes du vecteur normal a AB */
  int32_t note;

  rs = imcentres->row_size;
  cs = imcentres->col_size;
  if ((rowsize(image) != rs) || (colsize(image) != cs))
  {
    fprintf(stderr, "laxones: incompatible image sizes\n");
    return 0;
  }
  N = rs * cs;
  F = UCHARDATA(imcentres);
  G = UCHARDATA(image);

  lab = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  if (lab == NULL)
  {   
    fprintf(stderr, "laxones: allocimage failed\n");
    exit(0);
  }
  LABEL = SLONGDATA(lab);
  if (! llabelplateaux(imcentres, 8, lab, &nblabels))
  {
    fprintf(stderr, "laxones: llabelplateaux failed\n");
    exit(0);
  }

  bxx = (double *)calloc(1,nblabels * sizeof(double));
  byy = (double *)calloc(1,nblabels * sizeof(double));
  surf = (int32_t *)calloc(1,nblabels * sizeof(int32_t));
  vois = (int32_t *)calloc(1,nblabels * sizeof(int32_t));
  orie = (int32_t *)calloc(1,nblabels * sizeof(int32_t));
  if ((bxx == NULL) || (byy == NULL) || (surf == NULL) || (vois == NULL) || (orie == NULL))
  {
    fprintf(stderr, "laxones: malloc failed\n");
    exit(0);
  }

  /* ---------------------------------------------------------- */
  /* calcul des isobarycentres par composante */
  /* ---------------------------------------------------------- */
  
  for (c = 0; c < nblabels; c++)
  {
    bxx[c] = 0.0;
    byy[c] = 0.0;
    surf[c] = 0;
  }

  for (j = 0; j < cs; j++)
    for (i = 0; i < rs; i++)
    {
      label = LABEL[j * rs + i] - 1; 
      /* les valeurs des labels sont entre 1 et nblabels */
      if (F[j * rs + i] != 0)      /* composante connexe */
      {
        surf[label] += 1;
        bxx[label] += (double)i;
        byy[label] += (double)j;
      }
      else 
        surf[label] = -1;          /* marque la composante "fond" */
    }

  /* elimine le(s) point(s) inutile(s) */
  k = 0;
  for (c = 0; c < nblabels; c++)
    if (surf[c] != -1)
    {
      bxx[k] = bxx[c];
      byy[k] = byy[c];
      surf[k] = surf[c];
      k += 1;
    }
  nblabels = k;

  /* moyenne */
  for (c = 0; c < nblabels; c++)
    if (surf[c] != -1)
    {
      bxx[c] = bxx[c] / surf[c];
      byy[c] = byy[c] / surf[c];
    }

  if (identcercle(nblabels, bxx, byy, &bx, &by, &rayon) == 0)
  {
    fprintf(stderr, "laxones: identcercle failed\n");
    exit(0);
  }

#ifdef VERBOSE
  printf("identcercle : bx = %g ; by = %g ; r = %g\n", bx, by, rayon);
#endif

  /* ---------------------------------------------------------- */
  /* elimination des composantes proches ou loin du centre */
  /* ---------------------------------------------------------- */

  for (c = 0; c < nblabels; c++)
  {
    d = dist2(bx, by, bxx[c], byy[c]);
    if (d < (3*rayon)/5 || d > (5*rayon)/4) surf[c] = 0;
  }

  /* elimine le(s) point(s) inutile(s) */
  k = 0;
  for (c = 0; c < nblabels; c++)
    if (surf[c] != 0)
    {
      bxx[k] = bxx[c];
      byy[k] = byy[c];
      surf[k] = surf[c];
      k += 1;
    }
  nblabels = k;      

  /* re-calcule l'equation du cercle */
  if (identcercle(nblabels, bxx, byy, &bx, &by, &rayon) == 0)
  {
    fprintf(stderr, "laxones: identcercle failed\n");
    exit(0);
  }
#ifdef VERBOSE
  printf("identcercle : bx = %g ; by = %g ; r = %g\n", bx, by, rayon);
#endif

  /* ---------------------------------------------------------- */
  /* calcul des plus proches voisins  */
  /* ---------------------------------------------------------- */

  for (c = 0; c < nblabels; c++) vois[c] = -1;
  for (c = 0; c < nblabels; c++)
  {
    mind = 1.0E9;
    for (j = 0; j < nblabels; j++)
    {
      if ((c != j) && (surf[j] > 0))
      {
        d = dist2(bxx[c], byy[c], bxx[j], byy[j]);
        if (d < mind)
        {
          mind = d;
          vois[c] = j; 
        }
      }
    }
  }

#ifdef VERBOSE
  for (c = 0; c < nblabels; c++)
  {
    printf("c=%2d : x=%9g y=%9g s=%3d v=%2d\n",
            c, bxx[c], byy[c], surf[c], vois[c]);
  }
#endif

  *xc = bx;
  *yc = by;
  
  /* ---------------------------------------------------------- */
  /* determination des triangles et warps */
  /* ---------------------------------------------------------- */

  for (i = 0; i < N; i++) F[i] = G[i]; /* plus besoin des centres */
  for (i = 0; i < N; i++) LABEL[i] = (int32_t)G[i];  /* ni des labels */

  first = 1;
  nbim = 1;
  for (c = 0; c < nblabels; c++)
  {
    k = vois[c];
    if (k != -1)
    {
      if (vois[k] == c)
      {
        if (first)
        {
          d = (bxx[c]-bx)*(byy[k]-byy[c]) - (byy[c]-by)*(bxx[k]-bxx[c]);
          /* reorientation eventuelle de la paire */
          /* on teste le signe du determinant */     
          if (d < 0)
          {
            d = -d;
            t = bxx[c]; bxx[c] = bxx[k]; bxx[k] = t;
            t = byy[c]; byy[c] = byy[k]; byy[k] = t;
            j = surf[c]; surf[c] = surf[k]; surf[k] = j;
            j = vois[c]; vois[c] = vois[k]; vois[k] = j;
          }

          td.xa = bx;  
          td.ya = by;
          *xc1 = xA = td.xb = bxx[c];  
          *yc1 = yA = td.yb = byy[c];
          *xc2 = xB = td.xc = bxx[k];  
          *yc2 = yB = td.yc = byy[k];
          first = 0;
        }
        else
        {
          /* reorientation eventuelle de la paire */
          /* on teste le signe du determinant */     
          if (((bxx[c]-bx)*(byy[k]-byy[c]) - (byy[c]-by)*(bxx[k]-bxx[c]))*d < 0)
          {
            t = bxx[c]; bxx[c] = bxx[k]; bxx[k] = t;
            t = byy[c]; byy[c] = byy[k]; byy[k] = t;
            j = surf[c]; surf[c] = surf[k]; surf[k] = j;
            j = vois[c]; vois[c] = vois[k]; vois[k] = j;
          }
          ts.xa = bx;  
          ts.ya = by;
          ts.xb = bxx[c];  
          ts.yb = byy[c];
          ts.xc = bxx[k];  
          ts.yc = byy[k];
          if (! lwarp(imcentres, &ts, &td, 1))
          {
            fprintf(stderr, "laxones: function lwarp failed\n");
            exit(0);
          }          
          for (i = 0; i < N; i++) LABEL[i] += F[i]; 
                                            /* accumule pour la moyenne */
          for (i = 0; i < N; i++) F[i] = G[i];
          nbim += 1;
        }
        vois[k] = -1;
        vois[c] = -1;
      }
    }
  }

  /* printf("nombre de paires trouvees = %d\n", nbim); */
  if (nbim != 9) printf("nbim %d ", nbim);

  for (i = 0; i < N; i++) G[i] = (uint8_t)(LABEL[i]/nbim);

#ifdef NOTE
  /* ---------------------------------------------------------- */
  /* determination de trois points sur la structure */
  /* ---------------------------------------------------------- */

  t = (xB - xA) / (yB - yA);
  xN = 1 / sqrt(1 + t * t);
  yN = -t * xN;

  ts.xa = xA + petitrayon * xN;
  ts.ya = yA + petitrayon * yN;
  ts.xb = xA - petitrayon * xN;
  ts.yb = yA - petitrayon * yN;
  ts.xc = xB + petitrayon * xN;
  ts.yc = yB + petitrayon * yN;

  td.xa = 10.0;
  td.ya = 10.0;
  td.xb = 10.0;
  td.yb = 50.0;
  td.xc = 30.0;
  td.yc = 10.0;

  if (! lwarp(image, &ts, &td, 1))
  {
    fprintf(stderr, "laxones: function lwarp failed\n");
    exit(0);
  }          

  /* coordonnees de 2 triangles determines a partir de l'image _m */
  ts.xa = 46.0;
  ts.ya = 18.0;
  ts.xb = 50.0;
  ts.yb = 12.0;
  ts.xc = 67.0;
  ts.yc = 20.0;

  td.xa = 46.0;
  td.ya = 18.0;
  td.xb = 58.0;
  td.yb = 31.0;
  td.xc = 67.0;
  td.yc = 20.0;
  
  if ((inittriangle(&ts) == 0) || (inittriangle(&td) == 0))
  {
    fprintf(stderr, "laxones: mauvais triangle\n");
    return 0;
  }
  
  note = 0;
  for (i = 0; i < rs; i++)
    for (j = 0; j < cs; j++)
      if (danstriangle(&td, (double)i, (double)j) || danstriangle(&ts, (double)i, (double)j))
        note += G[j * rs + i];

  printf("note = %d\n", note);
#endif

  /* ---------------------------------------------------------- */
  /* un peu de menage */
  /* ---------------------------------------------------------- */

  free(bxx);
  free(byy);
  free(surf);
  free(vois);
  freeimage(lab);
} // laxones_sav()

/* ==================================== */
int32_t ltrapeze(
  struct xvimage * image,
  double xc1,
  double yc1,
  double xc2,
  double yc2,
  double xc,
  double yc,
  double * mndg)
/* ==================================== */
/* calcule la moyenne des niveaux de gris dans le trapeze */
{ 
  uint8_t *F;
  int32_t rs, cs, N;
  triangle ts, t, t1;
  int32_t x, y, k, n; 
  mat33 m;                           /* matrice de travail */
  mat33 a;                           /* transformation affine dest -> source */
  vec3 b;
  vec3 sol;
  double X1, X2, X3, X4;
  double Y1, Y2, Y3, Y4;
  double cumulndg;

  /* ---------------------------------------------------------- */

  rs = rowsize(image);
  cs = colsize(image);
  N = rs * cs;
  F = UCHARDATA(image);

  ts.xa = 0.0; ts.xb = 1.0; ts.xc = 0.0;
  ts.ya = 0.0; ts.yb = 0.0; ts.yc = 1.0;

  if (inittriangle(&ts) == 0)
    {
      fprintf(stderr, "ltrapeze: mauvais triangle source\n");
      return 0;
    }

  /* ------------------------------------------------------------------ */
  /* calcule la matrice pour passer du repere ts au repere c1,c2,c      */
  /* ------------------------------------------------------------------ */

  m[0][0] = ts.xa;     m[0][1] = ts.ya;     m[0][2] = 1.0;
  m[1][0] = ts.xb;     m[1][1] = ts.yb;     m[1][2] = 1.0;
  m[2][0] = ts.xc;     m[2][1] = ts.yc;     m[2][2] = 1.0;

  b[0] = xc1;
  b[1] = xc2;
  b[2] = xc;
  
  if (solsyst3(m, b, sol) == 0)
    fprintf(stderr, "ltrapeze: determinant nul\n");

  a[0][0] = sol[0];     a[0][1] = sol[1];     a[0][2] = sol[2];

  b[0] = yc1;
  b[1] = yc2;
  b[2] = yc;
  
  if (solsyst3(m, b, sol) == 0)
    fprintf(stderr, "ltrapeze: determinant nul\n");

  a[1][0] = sol[0];     a[1][1] = sol[1];     a[1][2] = sol[2];
  a[2][0] = 0.0;        a[2][1] = 0.0;        a[2][2] = 1.0;

  /* ------------------------------------------------------------------ */
  /* calcule les coord. du trapeze dans le nouveau repere */
  /* ------------------------------------------------------------------ */

  b[0] = 1.10845 + 0.2;
  b[1] = 0.352063;
  b[2] = 1.0;
  multmat3vec3(a, b, sol);
  X1 = sol[0];
  Y1 = sol[1];
  /*  printf("x1 = %g   y1 = %g\n",X1, Y1); */
  
  b[0] = 1.7787 + 0.2;
  b[1] = 0.431633;
  b[2] = 1.0;
  multmat3vec3(a, b, sol);
  X2 = sol[0];
  Y2 = sol[1];
  /* printf("x2 = %g   y2 = %g\n",X2, Y2); */
	
  b[0] = 1.04727 + 0.2;
  b[1] = 0.535594;
  b[2] = 1.0;
  multmat3vec3(a, b, sol);
  X3 = sol[0];
  Y3 = sol[1];
  /*  printf("x3 = %g   y3 = %g\n",X3, Y3); */
  
  b[0] = 1.37678 + 0.2;
  b[1] = 0.553513;
  b[2] = 1.0;
  multmat3vec3(a, b, sol);
  X4 = sol[0];
  Y4 = sol[1];
  /*  printf("x4 = %g   y4 = %g\n",X4, Y4); */
  
  t.xa = X1; t.ya = Y1;
  t.xb = X2; t.yb = Y2;
  t.xc = X3; t.yc = Y3;
  
  t1.xa = X2; t1.ya = Y2;
  t1.xb = X4; t1.yb = Y4;
  t1.xc = X3; t1.yc = Y3;

  /* ------------------------------------------------------------------ */
  /* calcule la moyenne des niveaux de gris dans le trapeze */
  /* ------------------------------------------------------------------ */

  if (inittriangle (&t) == 0)
    {
      fprintf(stderr, "ltrapeze: mauvais triangle\n");
      return 0;
    }
  
  if (inittriangle (&t1) == 0)
    {
      fprintf(stderr, "ltrapeze: mauvais triangle\n");
      return 0;
    }

  n = 0;
  cumulndg = 0.0;
  for (x = 0; x <rs ; x++)      /* balaye toute l'image : bourrin */
    for (y = 0; y < cs; y += 1)
      if ((danstriangle (&t , (double)x, (double)y) != 0) || 
          (danstriangle (&t1 , (double)x, (double)y) != 0))
      {
        n++;
        cumulndg += (double)F[y * rs + x];
#ifdef DESSINETRAPEZE
        F[y * rs + x] = NDG_MAX;
#endif
      }

  *mndg = cumulndg / n;

  return 1;

}


/* ==================================== */
double ltubule(
  struct xvimage * image,
  double xc,
  double yc,
  double r
)
/* ==================================== */
/* calcule la moyenne des niveaux de gris sur un cercle de rayon r dans le tubule */
{ 
  uint8_t *F = UCHARDATA(image);
  int32_t rs = rowsize(image); 
  int32_t cs = colsize(image); 
  int32_t x, y, n; 
  double cumulndg;

  /* ---------------------------------------------------------- */

  n = 0;
  cumulndg = 0.0;
  for (x = max(0,xc-r); x < min(xc+r+1,rs); x++) 
    for (y = max(0,yc-r); y < min(yc+r+1,cs); y++) 
      if ((int32_t)sqrt(((x - xc) * (x - xc)) + ((y - yc) * (y - yc))) == r)
      {
        n++;
        cumulndg += F[y * rs + x];
#ifdef DESSINETUBULE
        F[y * rs + x] = NDG_MAX;
#endif
      }
  return cumulndg / n;
}


/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imcentres;
  int32_t i;
  uint8_t *pt;
  int32_t N;
  int32_t d3 = 0;
  double mndg;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm centres.pgm out.pgm \n", argv[0]);
    exit(0);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "axones: readimage failed\n");
    exit(0);
  }

  imcentres = readimage(argv[2]);
  if (imcentres == NULL)
  {
    fprintf(stderr, "axones: readimage failed\n");
    exit(0);
  }

  if (! laxones(image, imcentres))
  {
    fprintf(stderr, "axones: function laxones failed\n");
    exit(0);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(imcentres);

  return 0;
} /* main */

