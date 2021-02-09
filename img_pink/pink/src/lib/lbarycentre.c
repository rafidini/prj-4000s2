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
/* 
   lbarycentre : 
     calcule les isobarycentres des composantes connexes d'une image binaire.

   lbarycentrelab : 
     calcule les isobarycentres des regions etiquetees.

   Michel Couprie - avril 1997 
*/

//#define DEBUG

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <llabelextrema.h>

/* ==================================== */
int32_t lbarycentrelab(struct xvimage * imagelab)
/* ==================================== */
#undef F_NAME
#define F_NAME "lbarycentrelab"
{
  index_t i, j, k;
  int32_t *F;
  index_t rs, cs, ds, ps, N;
  int32_t nblabels;
  double *bxx, *byy, *bzz; /* pour les tables de barycentres par composantes */
  int32_t *surf;
  int32_t lab;

  ACCEPTED_TYPES1(imagelab, VFF_TYP_4_BYTE);

  rs = rowsize(imagelab);
  cs = colsize(imagelab);
  ds = depth(imagelab);
  ps = rs * cs;
  N = ps * ds;
  F = SLONGDATA(imagelab);

  nblabels = 0;
  for (j = 0; j < N; j++) if (F[j] > nblabels ) nblabels = F[j];

#ifdef DEBUG
printf("%d\n", nblabels);
#endif

  bxx = (double *)calloc(1,nblabels * sizeof(double)); assert(bxx != NULL);
  byy = (double *)calloc(1,nblabels * sizeof(double)); assert(byy != NULL);
  bzz = (double *)calloc(1,nblabels * sizeof(double)); assert(bzz != NULL);
  surf = (int32_t *)calloc(1,nblabels * sizeof(int32_t)); assert(surf != NULL);

  /* ---------------------------------------------------------- */
  /* calcul des isobarycentres par region (sauf fond) */
  /* ---------------------------------------------------------- */
  
  for (i = 0; i < nblabels; i++)
  {
    bxx[i] = 0.0;
    byy[i] = 0.0;
    bzz[i] = 0.0;
    surf[i] = 0;
  }

  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    if (F[k*ps + j*rs + i] != 0)
    {
      lab = F[k*ps + j*rs + i] - 1; /* les valeurs des labels sont entre 1 et nblabels */
      surf[lab] += 1;
      bxx[lab] += (double)i;
      byy[lab] += (double)j;
      bzz[lab] += (double)k;
    }
  }

  for (i = 0; i < nblabels; i++)
    if (surf[i]) 
    {
      bxx[i] = bxx[i] / surf[i];
      byy[i] = byy[i] / surf[i];
      bzz[i] = bzz[i] / surf[i];
#ifdef DEBUG
printf("%g %g %g\n", bxx[i], byy[i], bzz[i]);
#endif
    }

  /* ---------------------------------------------------------- */
  /* marque l'emplacement approximatif des barycentres dans l'image */
  /* ---------------------------------------------------------- */

  for (j = 0; j < N; j++) F[j] = 0;

  for (i = 0; i < nblabels; i++)
    if (surf[i])  
      F[(int32_t)(arrondi(bzz[i])) * ps + 
	(int32_t)(arrondi(byy[i])) * rs + 
	(int32_t)(arrondi(bxx[i]))
       ] = i+1;

  free(bxx);
  free(byy);
  free(bzz);
  free(surf);
  return 1;
} /* lbarycentrelab() */

/* ==================================== */
int32_t lbarycentre(struct xvimage * image1, int32_t connex)
/* ==================================== */
#undef F_NAME
#define F_NAME "lbarycentre"
{
  index_t i, j, k;
  uint8_t *F;
  index_t rs, cs, ds, ps, N;
  struct xvimage *label;
  int32_t *LABEL;   /* pour les labels des composantes connexes */
  int32_t nblabels;
  double *bxx, *byy, *bzz; /* pour les tables de barycentres par composantes */
  int32_t *surf;
  int32_t lab;

  ACCEPTED_TYPES1(image1, VFF_TYP_1_BYTE);

  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);
  ps = rs * cs;
  N = ps * ds;
  F = UCHARDATA(image1);

  label = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE); assert(label != NULL);
  LABEL = SLONGDATA(label);

  if (! llabelextrema(image1, connex, LABMAX, label, &nblabels))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", F_NAME);
    return 0;
  }

  bxx = (double *)calloc(1,(nblabels+1) * sizeof(double)); assert(bxx != NULL);
  byy = (double *)calloc(1,(nblabels+1) * sizeof(double)); assert(byy != NULL);
  bzz = (double *)calloc(1,(nblabels+1) * sizeof(double)); assert(bzz != NULL);
  surf = (int32_t *)calloc(1,(nblabels+1) * sizeof(int32_t)); assert(surf != NULL);

  /* ---------------------------------------------------------- */
  /* calcul des isobarycentres par composante */
  /* ---------------------------------------------------------- */
  
  for (i = 0; i <= nblabels; i++)
  {
    bxx[i] = 0.0;
    byy[i] = 0.0;
    bzz[i] = 0.0;
    surf[i] = 0;
  }

  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    if (F[k*ps + j*rs + i] != 0)
    {
      lab = LABEL[k*ps + j*rs + i] - 1; /* les valeurs des labels sont entre 1 et nblabels */
      surf[lab] += 1;
      bxx[lab] += (double)i;
      byy[lab] += (double)j;
      bzz[lab] += (double)k;
    }
  }

#ifdef DEBUG
printf("nlabels = %d\n", nblabels);
#endif

  for (i = 0; i < nblabels-1; i++)
  {
    bxx[i] = bxx[i] / surf[i];
    byy[i] = byy[i] / surf[i];
    bzz[i] = bzz[i] / surf[i];
#ifdef DEBUG
    printf("i = %d %g %g %g\n", i, bxx[i], byy[i], bzz[i]);
#endif
  }

  /* ---------------------------------------------------------- */
  /* marque l'emplacement approximatif des barycentres dans l'image */
  /* ---------------------------------------------------------- */

  for (j = 0; j < N; j++) F[j] = 0;

  for (i = 0; i < nblabels-1; i++)
  {
    F[(int32_t)(arrondi(bzz[i])) * ps + 
      (int32_t)(arrondi(byy[i])) * rs + 
      (int32_t)(arrondi(bxx[i]))
     ] = NDG_MAX;
  }
  
  freeimage(label);
  free(bxx);
  free(byy);
  free(bzz);
  free(surf);
  return 1;
} /* lbarycentre() */
