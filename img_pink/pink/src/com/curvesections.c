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
/*! \file curvesections.c

\brief computes a series of normal 2d cross sections from a 3d curve and image

<B>Usage:</B> curvesections in.pgm curve.list tolerance n_samples clip_rad out_rs out_cs out_prefix

<B>Description:</B> 
Computes a series of normal 2d cross sections from a 3d curve \b curve.list and image \b in.pgm.

Method:

1/ computes a cubic spline from the curve, with tolerance given by parameter \b tolerance .

2/ samples the spline to obtain \b n_samples points, and computes the tangents at these points

3/ at each sample point, extract a 2D section from \b in.pgm in the plane normal to the tangent. Delete the pixels at distance larger than \b clip_rad from the basis origin.

<B>Types supported:</B> byte 3d, int32_t 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - decembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>
#include <mclin.h>
#include <lgeo.h>
#include <lcrop.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * out;
  struct xvimage * out_image;
  char type;
  FILE *fd = NULL;
  double tolerance;
  int32_t npoints, n_samples, clip_rad;
  int32_t i, n; // where n+1 denotes the number of control points
  int32_t *X, *Y, *Z, *W;
  double *C0, *C1, *C2, *C3;
  double *D0, *D1, *D2, *D3;
  double *E0, *E1, *E2, *E3;
  double *sk, *xk, *yk, *zk, *txk, *tyk, *tzk, norm;
  double v[3], n1[3], n2[3];
  char bufname[1024];
  int32_t namelen, nl;
  int32_t out_rs, out_cs;

  if (argc != 9 )
  {
    fprintf(stderr, "usage: %s in.pgm curve.list tolerance n_samples clip_rad out_rs out_cs out_prefix\n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[2], "r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'B')
  {
    fprintf(stderr, "usage: %s: bad file format: %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d", &npoints);
  X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
  Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);
  Z = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);

  for (i = 0; i < npoints; i++)
    fscanf(fd, "%d %d %d", &(X[i]), &(Y[i]), &(Z[i]));
  fclose(fd);

  tolerance = atof(argv[3]);
  n_samples = atoi(argv[4]) + 1;
  clip_rad  = atoi(argv[5]);
  out_rs  = atoi(argv[6]);
  out_cs  = atoi(argv[7]);

  out_image = allocimage(NULL, out_rs, out_cs, 1, VFF_TYP_1_BYTE);
  if (out_image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  
  strcpy(bufname, argv[argc - 1]);
  namelen = strlen(argv[argc - 1]);

  // 1. computes a cubic spline from the curve, with tolerance given by parameter \b tolerance .

  W = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(W != NULL);
  C0 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(C0 != NULL);
  C1 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(C1 != NULL);
  C2 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(C2 != NULL);
  C3 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(C3 != NULL);
  D0 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(D0 != NULL);
  D1 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(D1 != NULL);
  D2 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(D2 != NULL);
  D3 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(D3 != NULL);
  E0 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(E0 != NULL);
  E1 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(E1 != NULL);
  E2 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(E2 != NULL);
  E3 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(E3 != NULL);

  scn_approxcurve3d(X, Y, Z, npoints, tolerance, W, &n, C0, C1, C2, C3, D0, D1, D2, D3, E0, E1, E2, E3);
  
  // 2. samples the spline to obtain \b n_samples points, and computes the tangents at these points
  
  sk = (double *)calloc(1,n_samples*sizeof(double)); assert(sk != NULL);
  xk = (double *)calloc(1,n_samples*sizeof(double)); assert(xk != NULL);
  yk = (double *)calloc(1,n_samples*sizeof(double)); assert(yk != NULL);
  zk = (double *)calloc(1,n_samples*sizeof(double)); assert(zk != NULL);
  txk = (double *)calloc(1,n_samples*sizeof(double)); assert(txk != NULL);
  tyk = (double *)calloc(1,n_samples*sizeof(double)); assert(tyk != NULL);
  tzk = (double *)calloc(1,n_samples*sizeof(double)); assert(tzk != NULL);

  scn_tangents3d(C0, C1, C2, C3, D0, D1, D2, D3, E0, E1, E2, E3, 
		 n, n_samples, sk, xk, yk, zk, txk, tyk, tzk);
    
  // 3. at each sample point, extract a 2D section from \b in.pgm in the plane normal to the tangent. Delete the pixels at distance larger than \b clip_rad from the basis origin.

  for (i = 1; i < n_samples; i++)
  {
    int xc, yc;
    v[0] = txk[i]; v[1] = tyk[i]; v[2] = tzk[i];
    norm = lin_norme2(v, 3, 1);
    lin_multscal(v, 1/norm, 3, 1);
    lin_createbase(v, n1, n2);

    //    printf("i=%d p=%g %g %g v=%g %g %g n1=%g %g %g n2=%g %g %g\n", i, xk[i], yk[i], zk[i], v[0], v[1], v[2], n1[0], n1[1], n1[2], n2[0], n2[1], n2[2] );

    out = lsection(in, 
		   xk[i], yk[i], zk[i], 
		   xk[i]+n1[0], yk[i]+n1[1], zk[i]+n1[2], 
		   xk[i]+n2[0], yk[i]+n2[1], zk[i]+n2[2], 
		   &xc, &yc);
    assert(out != NULL);
    
    int32_t rs = rowsize(out);
    int32_t cs = colsize(out);
    uint8_t *O = UCHARDATA(out);
    int x, y;

    for (y=0; y < cs; y++) // clipping
      for (x=0; x < rs; x++)
      {
	v[0] = x - xc;
	v[1] = y - yc;
	if (lin_norme2(v, 2, 1) > clip_rad) 
	  O[y*rs + x] = NDG_MIN;
      }

    razimage(out_image);
    if (!linsert(out, out_image, out_rs/2 - (int)xc, out_cs/2 - (int)yc, 0))
    {
      fprintf(stderr, "%s: linsert failed\n", argv[0]);
      exit(1);
    }

    nl = namelen;
    bufname[nl++] = '_';
    bufname[nl++] = '0' + (i / 1000) % 10;
    bufname[nl++] = '0' + (i / 100) % 10;
    bufname[nl++] = '0' + (i / 10) % 10;
    bufname[nl++] = '0' + (i / 1) % 10;
    bufname[nl++] = '.';
    bufname[nl++] = 'p';
    bufname[nl++] = 'g';
    bufname[nl++] = 'm';
    bufname[nl++] = '\0';

    writese(out_image, bufname, out_rs - (int)xc, out_cs - (int)yc, 0);
    freeimage(out);

  } // for (i = 0; i < n_samples; i++)


  freeimage(in);

  free(X); free(Y); free(Z); free(W);
  free(C0); free(C1); free(C2); free(C3);
  free(D0); free(D1); free(D2); free(D3);
  free(E0); free(E1); free(E2); free(E3);
  free(sk);
  free(xk); free(yk); free(zk);
  free(txk); free(tyk); free(tzk);

  return 0;
} /* main */
