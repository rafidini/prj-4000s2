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
/* \file worms.c

\brief draw random non-intersecting spline segments

<B>Usage:</B> worms rs cs ds nmax lenmin lenmax bendmin bendmax dmin dilatse out.pgm

Draw random non-intersecting spline segments.

Parameters: 
\li \b rs, \b cs, \b ds Image dimensions
\li \b nmax Maximum number of segments 
\li \b lenmin Minimum length for a segment
\li \b lenmax Maximum length for a segment
\li \b bendmin Minimum bending factor for a segment (range: [0,0.5])
\li \b bendmax Maximum bending factor for a segment (range: [0,0.5])
\li \b dmin Minimum distance between segments
\li \b dilatse Structuring element or ball radius for dilation
\li \b out.pgm Output image

<B>Types supported:</B> byte 3D

<B>Category:</B> draw geo experimental
\ingroup  draw geo experimental

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>
#include <mcprobas.h>
#include <mcgeo.h>
#include <ldist.h>
#include <ldraw.h>
#include <lcrop.h>
#include <ldilateros3d.h>
#include <lrotations.h>

//#define DEBUG

#define VERBOSE
#define NDG_DIL 127
#define AVOIDFRAME

/* ==================================== */
static double scalarprod(double x1, double y1, double z1, double x2, double y2, double z2)
/* ==================================== */
{
  return (x1 * x2) + (y1 * y2) + (z1 * z2);
}

/* ==================================== */
static double norm(double x, double y, double z)
/* ==================================== */
{
  return sqrt((x * x) + (y * y) + (z * z));
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * img;
  struct xvimage * tmp;
  struct xvimage * dis;
  struct xvimage * dil;
  struct xvimage * dse;
  uint8_t *I, *T, *L;
  uint32_t *D;
  index_t i, j, rs, cs, ds, N;
  int32_t n, nmax, nt, ntmax, radius, ret;
  const int32_t npoints = 3;
  double x[npoints], y[npoints], z[npoints], t[npoints];
  double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, xc, yc, zc;
  double ux, uy, uz, vx, vy, vz;
  double Px[4], Py[4], Pz[4];
  double X0[2], X1[2], X2[2], X3[2];  
  double Y0[2], Y1[2], Y2[2], Y3[2];
  double Z0[2], Z1[2], Z2[2], Z3[2];
  double len, nor, scal, theta, phi;
  double lenmin, lenmax, bendmin, bendmax;
  int32_t dmin;

  if (argc != 12)
  {
    fprintf(stderr, "usage: %s rs cs ds nmax lenmin lenmax bendmin bendmax dmin dilatse out.pgm \n", argv[0]);
    exit(1);
  }

  rs = atoi(argv[1]);
  cs = atoi(argv[2]);
  ds = atoi(argv[3]);
  nmax = atoi(argv[4]);
  lenmin = atof(argv[5]);
  lenmax = atof(argv[6]);
  bendmin = atof(argv[7]);
  bendmax = atof(argv[8]);
  assert(bendmax <= 0.5);
  assert(bendmin >= 0.0);
  assert(bendmax >= bendmin);
  dmin = (int32_t)atof(argv[9]);
  ntmax = nmax * 20;
  N = rs * cs * ds;
  img = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE); assert(img != NULL);
  tmp = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE); assert(tmp != NULL);
  dis = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE); assert(dis != NULL);
  razimage(img);
  I = UCHARDATA(img);
  T = UCHARDATA(tmp);
  D = ULONGDATA(dis);

  if (nmax > 255) 
    fprintf(stderr, "%s: warning: more than 255 segments (several segments will have the same tag)\n", argv[0]);

  ret = sscanf(argv[10], "%d", &radius);
  if (ret == 0) // structuring element : image 
  {
    index_t x, y, z;
    dse = readse(argv[10], &x, &y, &z); 
    assert(dse != NULL); 
    xc = (double)x; yc = (double)y; zc = (double)z;
  }
  else  // structuring element : Euclidean ball (radius given)
    dse = NULL;

#ifdef AVOIDFRAME
  assert(rs-dmin-1 > dmin); assert(cs-dmin-1 > dmin); assert(ds-dmin-1 > dmin);
#else
  assert(rs > 0); assert(cs > 0); assert(ds > 0);
#endif

  n = 0; nt = 0;
  while ((n < nmax) && (nt < ntmax))
  {
#ifdef AVOIDFRAME
    x1 = (double)Random(dmin, rs-dmin-1); y1 = (double)Random(dmin, cs-dmin-1); z1 = (double)Random(dmin, ds-dmin-1);
    x3 = (double)Random(dmin, rs-dmin-1); y3 = (double)Random(dmin, cs-dmin-1); z3 = (double)Random(dmin, ds-dmin-1);
#else
    x1 = (double)Random(0, rs-1); y1 = (double)Random(0, cs-1); z1 = (double)Random(0, ds-1);
    x3 = (double)Random(0, rs-1); y3 = (double)Random(0, cs-1); z3 = (double)Random(0, ds-1);
#endif

    len = dist3(x1, y1, z1, x3, y3, z3);

    if ((len >= lenmin) && (len <= lenmax))
    {
      razimage(tmp);
      x2 = (x1 + x3) / 2; y2 = (y1 + y3) / 2; z2 = (z1 + z3) / 2;
      do 
      {
	x4 = (double)Random(0, rs-1); y4 = (double)Random(0, cs-1); z4 = (double)Random(0, ds-1);
      } while (dist3(x2, y2, z2, x4, y4, z4) < 1.0);

      ux = x3 - x2; uy = y3 - y2; uz = z3 - z2;
      vx = x4 - x2; vy = y4 - y2; vz = z4 - z2;
      // normalize u
      nor = norm(ux, uy, uz); assert(nor > 0.0);
      ux /= nor; uy /= nor; uz /= nor;
      // then v.u gives the magnitude of the orthogonal projection of v on u
      scal = scalarprod(vx, vy, vz, ux, uy, uz);
      // let u = proj(v,u)
      ux *= scal; uy *= scal; uz *= scal;
      // then v - u is a vector orthogonal to u in the plane u,v
      vx = vx - ux; vy = vy - uy; vz = vz - uz; 
      // normalize v
      nor = norm(vx, vy, vz); assert(nor > 0.0);
      vx /= nor; vy /= nor; vz /= nor;
      // multiply by the bending factor 
      scal = Uniform(bendmin, bendmax) * len;
      vx *= scal; vy *= scal; vz *= scal;
      // add to x2
      x2 += vx; y2 += vy; z2 += vz;

      x[0] = x1; y[0] = y1; z[0] = z1; t[0] = 0.0;
      x[1] = x2; y[1] = y2; z[1] = z2; t[1] = 1.0;
      x[2] = x3; y[2] = y3; z[2] = z3; t[2] = 2.0;

#ifdef DEBUG
      printf("p1: %g,%g,%g\n", x1, y1, z1);
      printf("p2: %g,%g,%g\n", x2, y2, z2);
      printf("p3: %g,%g,%g\n", x3, y3, z3);
#endif

      scn_solvespline((double *)t, (double *)x, npoints, (double *)X0, (double *)X1, (double *)X2, (double *)X3);
      scn_solvespline((double *)t, (double *)y, npoints, (double *)Y0, (double *)Y1, (double *)Y2, (double *)Y3);
      scn_solvespline((double *)t, (double *)z, npoints, (double *)Z0, (double *)Z1, (double *)Z2, (double *)Z3);
    
      for (j = 0; j < npoints-1; j++)
      {
	Px[0] = X0[j]; Px[1] = X1[j]; Px[2] = X2[j]; Px[3] = X3[j];
	Py[0] = Y0[j]; Py[1] = Y1[j]; Py[2] = Y2[j]; Py[3] = Y3[j];
	Pz[0] = Z0[j]; Pz[1] = Z1[j]; Pz[2] = Z2[j]; Pz[3] = Z3[j];
	ldrawcubic3d(tmp, (double *)Px, (double *)Py, (double *)Pz, 10, t[j], t[j+1]);
      }

      if (n > 0)
      {
	if (! ldisteuc3d(img, dis))
	{
	  fprintf(stderr, "%s: ldisteuc3d failed\n", argv[0]);
	  exit(1);
	}

	for (i = 0; i < N; i++) if (T[i] && (D[i] <= dmin)) break;
	if (i == N)
	{
	  n++;
#ifdef DEBUG
	printf("nb fibres generees: %d\n", n);
#endif
	  for (i = 0; i < N; i++) if (T[i]) I[i] = n;
	}
      } // if (n > 1)
      else
      {
	n++;
#ifdef DEBUG
	printf("INIT : nb fibres generees: %d\n", n);
#endif
	for (i = 0; i < N; i++) if (T[i]) I[i] = n;
      }

      nt++;
    } // if ((len >= lenmin) && (len <= lenmax)))
  } // while ((n < nmax) && (nt < ntmax))
#ifdef VERBOSE
  printf("n = %d segments generated; %d trials\n", n, nt);
  writeimage(img, "_img");
  writeimage(tmp, "_tmp");
#endif

  if ((dse == NULL) && (radius > 0))
  {
    dil = copyimage(img);
    L = UCHARDATA(dil);
    ret = ldilatball(dil, radius, 0); assert(ret != 0);
    for (i = 0; i < N; i++) 
      if (I[i]) I[i] = NDG_MAX;
      else if (L[i]) I[i] = NDG_DIL;
    freeimage(dil);
  }

  if (dse != NULL)
  {
    struct xvimage * rot1;
    struct xvimage * rot2;
    struct xvimage * dil1;
    uint8_t *L1;
    double yc1, zc1, xc2, zc2;
    dil = copyimage(img);
    L = UCHARDATA(dil);
    razimage(dil);
    dil1 = copyimage(img);
    L1 = UCHARDATA(dil1);
    for (j = 1; j <= n; j++) 
    {
      for (i = 0; i < N; i++) 
	if (I[i] == j) L1[i] = NDG_MAX; else L1[i] = 0;
      theta = Uniform(0.0, 90.0);
      phi = Uniform(0.0, 90.0);
      rot1 = lrotationRT3Dx(dse, theta, yc, zc, &yc1, &zc1, 1);
      rot2 = lrotationRT3Dy(rot1, phi, xc, zc1, &xc2, &zc2, 1);
      ret = ldilat3d(dil1, rot2, xc2, yc1, zc2); assert(ret != 0);
      for (i = 0; i < N; i++) if (L1[i]) L[i] = NDG_MAX;
    }
    for (i = 0; i < N; i++) 
      if (I[i]) I[i] = NDG_MAX;
      else if (L[i]) I[i] = NDG_DIL;

    freeimage(dse);
    freeimage(dil);
    freeimage(dil1);
    freeimage(rot1);
    freeimage(rot2);
  }

  writeimage(img, argv[argc-1]);
  freeimage(img);
  freeimage(tmp);
  freeimage(dis);

  return 0;
} /* main */
