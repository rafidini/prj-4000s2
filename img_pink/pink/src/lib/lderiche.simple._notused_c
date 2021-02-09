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
/****************************************************************
*
* Routine Name: lderiche - library call for deriche
*
* Purpose:     Filtre lineaire general recursif de Deriche
*
* Input:       Image en niveau de gris
* Output:      Image en niveau de gris
* Written By:  Michel Couprie - janvier 1998
*
****************************************************************/

/* 
le filtre lineaire recursif general de Deriche est defini par :
   x : le signal a filtrer
   y : le signal resultat
   y1, y2, r : resultats intermediaires
   alpha : parametre donnant la "taille" du filtre
   a1..8 : parametres calcules a partir de alpha, differents selon les applications
   b1..2 : parametres calcules a partir de alpha, differents selon les applications

   y1[m,n] = a1 x[m,n] + a2 x[m,n-1] + b1 y1[m,n-1] + b2 y1[m,n-2]
     x[m,-1] = 0
     y1[m,-2] = y1[m,-1] = 0
   y2[m,n] = a3 x[m,n+1] + a4 x[m,n+2] + b1 y2[m,n+1] + b2 y2[m,n+2]
     x[m,N] = x[m,N+1] = 0
     y2[m,N] = y2[m,N+1] = 0
   r[m,n] = y1[m,n] + y2[m,n]

   y1[m,n] = a5 r[m,n] + a6 r[m-1,n] + b1 y1[m-1,n] + b2 y1[m-2,n]
     r[-1,n] = 0
     y1[-2,n] = y1[-1,n] = 0
   y2[m,n] = a7 r[m+1,n] + a8 r[m+2,n] + b1 y2[m+1,n] + b2 y2[m+2,n]
     r[M,n] = r[M+1,n] = 0
     y2[M,n] = y2[M+1,n] = 0
   y[m,n] = y1[m,n] + y2[m,n]
*/

#include <mccodimage.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mcutil.h>
/*
#define DEBUG
*/

#define EPSILON 1E-20

#define abs(X) (X>=0?X:-(X))

/* ==================================== */
void derichegen(double *x,               /* image a traiter */
                int32_t M,                   /* taille ligne */
                int32_t N,                   /* taille colonne */
                double *y1, double *y2, double *y,
                double a1, double a2, double a3, double a4, 
                double a5, double a6, double a7, double a8, 
                double b1, double b2)
/* ==================================== */
{
  int32_t n, m;

  for (m = 0; m < M; m++)
  {
    y1[0] = a1 * x[m+M*0];
    y1[1] = a1 * x[m+M*1] + a2 * x[m+M*0] + b1 * y1[0];
    for (n = 2; n < N; n++)
      y1[n] = a1*x[m+M*n] + a2*x[m+M*(n-1)] + b1*y1[n-1] + b2*y1[n-2];

    y2[N-1] = 0;
    y2[N-2] = a3 * x[m+M*(N-1)] + b1 * y2[N-1];
    for (n = N-3; n >= 0; n--)
      y2[n] = a3*x[m+M*(n+1)] + a4*x[m+M*(n+2)] + b1*y2[n+1] + b2*y2[n+2];

    for (n = 0; n < N; n++)
      y[m+M*n] = y1[n] + y2[n];
  }

  for (n = 0; n < N; n++) 
  {
    y1[0] = a5 * y[0+M*n];
    y1[1] = a5 * y[1+M*n] + a6 * y[0+M*n] + b1 * y1[0];
    for (m = 2; m < M; m++)
      y1[m] = a5 * y[m+M*n] + a6 * y[m-1+M*n] + b1 * y1[m-1] + b2 * y1[m-2];

    y2[M-1] = 0;
    y2[M-2] = a7 * y[M-1+M*n] + b1 * y2[M-1];
    for (m = M-3; m >= 0; m--)
      y2[m] = a7 * y[m+1+M*n] + a8 * y[m+2+M*n] + b1 * y2[m+1] + b2 * y2[m+2];

    for (m = 0; m < M; m++)
      y[m+M*n] = y1[m] + y2[m];
  }
#ifdef DEBUG
printf("derichegen\n");
/*
for (n = 0; n < N; n++) printf("");
printf("\n");
*/
#endif

} /* derichegen() */

/* ==================================== */
int32_t lderiche(struct xvimage *image, double alpha)
/* ==================================== */
/*
    alpha : parametre (1/taille) du filtre
*/
{ 
  int32_t i, j;
  uint8_t *ima = UCHARDATA(image);
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t N = rs * cs;
  double *Im1;    /* image intermediaire */
  double *Im2;    /* image intermediaire */
  double *Imd;    /* image intermediaire */
  double *buf1;   /* buffer ligne ou colonne */
  double *buf2;   /* buffer ligne ou colonne */
  double k;       /* constante de normalisation pour le lisseur */
  double kp;      /* constante de normalisation pour le derivateur */
  double e_a;     /* stocke exp(-alpha) */
  double e_2a;    /* stocke exp(-2alpha) */
  double a1, a2, a3, a4, a5, a6, a7, a8, b1, b2;
  double t1, t2;
  double lmax, lmin;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lderiche: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  Im1 = (double *)calloc(1,N * sizeof(double));
  Im2 = (double *)calloc(1,N * sizeof(double));
  Imd = (double *)calloc(1,N * sizeof(double));
  buf1 = (double *)calloc(1,max(rs, cs) * sizeof(double));
  buf2 = (double *)calloc(1,max(rs, cs) * sizeof(double));
  if ((Im1==NULL) || (Im2==NULL) || (Imd==NULL) || (buf1==NULL) || (buf2==NULL))
  {   printf("lderiche() : malloc failed\n");
      return(0);
  }

  for (i = 0; i < N; i++) Imd[i] = (double)ima[i];

  e_a = exp(- alpha);
  e_2a = exp(- 2.0 * alpha);
  k = 1.0 - e_a;
  k = - k * k / e_a;
  kp = 1.0 - e_a;
  kp = kp * kp / (1.0 + (2 * alpha * e_a) - e_2a);

#ifdef DEBUG
printf("alpha = %g , e_a = %g , e_2a = %g , k = %g\n", alpha, e_a, e_2a, k);
#endif

  /* valeurs de parametres pour filtre lisseur-derivateur */
  a1 = k;
  a2 = k * e_a * (alpha - 1.0);
  a3 = k * e_a * (alpha + 1.0);
  a4 = - k * e_2a;
  a5 = 0.0;
  a6 = kp * e_a;
  a7 = - kp * e_a;
  a8 = 0;
  b1 = 2 * e_a;
  b2 = - e_2a;

  derichegen(Imd, rs, cs, buf1, buf2, Im1,
             a1, a2, a3, a4, a5, a6, a7, a8, b1, b2);
  derichegen(Imd, rs, cs, buf1, buf2, Im2,
             a5, a6, a7, a8, a1, a2, a3, a4, b1, b2);

  for (i = 0; i < N; i++)
  {
    t1 = Im1[i];
    t2 = Im2[i];
    t2 = sqrt((t1 * t1) + (t2 * t2));
    if (t2 <= 255.0)
      ima[i] = (uint8_t)t2;
    else
      ima[i] = 255;
  }

  free(Im1);
  free(Im2);
  free(Imd);
  free(buf1);
  free(buf2);

  return 1;
}

