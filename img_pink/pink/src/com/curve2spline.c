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
/*! \file curve2spline.c

\brief converts a discrete open curve into a spline

<B>Usage:</B> curve2spline curve.list [tolerance] out.spline

<B>Description:</B>

Let C be the discrete open curve described in <B>curve.list</B>. This program finds a approximation of C
in the form of a parametric curve P defined by two (in 2D) or three (in 3D) splines 
such that the maximal distance between P and C is beyond the given <B>tolerance</B>.
The default value for <B>tolerance</B> is 2.

The result is given in the form of the list of control points for the splines, followed by
the set of coefficients for each spline segment. 
The file format is the following for 2D:

c n+1 (where n+1 denotes the number of control points)<br>
x1 y1<br>
...<br>
xn+1 yn+1<br>
C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1<br>
...<br>
C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn<br>

and in the 3D case:

C n+1 (where n+1 denotes the number of control points)<br>
x1 y1 z1<br>
...<br>
xn+1 yn+1 zn+1<br>
C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1<br>
...<br>
C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn<br>

The ith segment (starting with i=0) of the parametric curve P is then defined by:

x(t) = C3Xi * t^3 + C2Xi * t^2 + C1Xi * t + C0Xi<br>
y(t) = C3Yi * t^3 + C2Yi * t^2 + C1Yi * t + C0Yi<br>
z(t) = C3Zi * t^3 + C2Zi * t^2 + C1Zi * t + C0Zi<br>
with t in [i,i+1]

\warning Closed curves are not allowed.

Reference:<BR> 
[Cou10] M. Couprie: <A HREF="http://www.esiee.fr/~coupriem/Pdf/splines.pdf">"Short note on natural cubic splines and discrete curves"</A>, internal report, 2010.<BR>

<B>Types supported:</B> list 2D, list 3D

<B>Category:</B> draw geo
\ingroup  draw geo

\author Michel Couprie
*/

/*
%TEST curve2spline %IMAGES/2dlist/binary/l2curve1.list %RESULTS/curve2spline_l2curve1.spline
%TEST curve2spline %IMAGES/3dlist/binary/l3curve1.list %RESULTS/curve2spline_l3curve1.spline
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>

#define DEFAULT_TOLERANCE 2

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t i, n;
  FILE *fd = NULL;
  int32_t npoints;
  double tolerance;
  char type;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s curve.list [tolerance] out.spline \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }
  if (argc == 4)
    tolerance = atof(argv[2]);
  else
    tolerance = DEFAULT_TOLERANCE;

  fscanf(fd, "%c", &type);
  if ((type != 'b') && (type != 'B'))
  {
    fprintf(stderr, "usage: %s: bad file format: %c \n", argv[0], type);
    exit(1);
  }

  if (type == 'b') // cas 2D
  {
    int32_t *X, *Y, *W;
    double *C0, *C1, *C2, *C3;
    double *D0, *D1, *D2, *D3;
    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t));
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t));
    W = (int32_t *)calloc(1,npoints*sizeof(int32_t));
    C0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    for (i = 0; i < npoints; i++)
    {
      fscanf(fd, "%d %d", &(X[i]), &(Y[i]));
    }
    fclose(fd);

    scn_approxcurve(X, Y, npoints, tolerance, W, &n, C0, C1, C2, C3, D0, D1, D2, D3);

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }
  
    fprintf(fd, "c %d\n", n);
    for (i = 0; i < n; i++) fprintf(fd, "%d %d\n", X[W[i]], Y[W[i]]);
    for (i = 0; i < n-1; i++) fprintf(fd, "%g %g %g %g %g %g %g %g\n", 
                                          C0[i], D0[i], C1[i], D1[i], C2[i], D2[i], C3[i], D3[i]);

    fclose(fd);
    free(X); free(Y); free(W);
    free(C0); free(C1); free(C2); free(C3);
    free(D0); free(D1); free(D2); free(D3);
  }
  else // cas 3D
  {
    int32_t *X, *Y, *Z, *W;
    double *C0, *C1, *C2, *C3;
    double *D0, *D1, *D2, *D3;
    double *E0, *E1, *E2, *E3;
    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t));
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t));
    Z = (int32_t *)calloc(1,npoints*sizeof(int32_t));
    W = (int32_t *)calloc(1,npoints*sizeof(int32_t));
    C0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    E0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    E1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    E2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    E3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    for (i = 0; i < npoints; i++)
    {
      fscanf(fd, "%d %d %d", &(X[i]), &(Y[i]), &(Z[i]));
    }
    fclose(fd);

    scn_approxcurve3d(X, Y, Z, npoints, tolerance, W, &n, C0, C1, C2, C3, D0, D1, D2, D3, E0, E1, E2, E3);

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }
  
    fprintf(fd, "C %d\n", n);
    for (i = 0; i < n; i++) fprintf(fd, "%d %d %d\n", X[W[i]], Y[W[i]], Z[W[i]]);
    for (i = 0; i < n-1; i++) fprintf(fd, "%g %g %g %g %g %g %g %g %g %g %g %g\n", 
                                          C0[i], D0[i], E0[i], C1[i], D1[i], E1[i], C2[i], D2[i], E2[i], C3[i], D3[i], E3[i]);

    fclose(fd);
    free(X); free(Y); free(Z); free(W);
    free(C0); free(C1); free(C2); free(C3);
    free(D0); free(D1); free(D2); free(D3);
    free(E0); free(E1); free(E2); free(E3);
  }
  return 0;
} /* main */


