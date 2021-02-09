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
/*! \file points2spline.c

\brief converts a sequence of control points into a spline

<B>Usage:</B> points2spline points.txt spline.txt

<B>Description:</B>

Let S be the sequence of control points in <B>points.txt</B>. This program computes
a parametric curve P defined by two (in 2D) or three (in 3D) splines 
that interpolates the points of the sequence S.

If the last point equals the first point, then a closed spline is generated.

The result is given in the form of the list of control points, followed by
the set of coefficients for each spline segment. 

The input file \b points.txt contains a list of points under the format:

b nbpoints<br>  
x1 y1<br>
x2 y2<br>
x3 y3<br>
...<br>

or:

B nbpoints<br>  
x1 y1 z1<br>
x2 y2 z2<br>
x3 y3 z3<br>
...<br>

The output file format is the following for 2D:

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

The ith segment (starting from i=0) of the parametric curve P is then defined by:

x(t) = C3Xi * t^3 + C2Xi * t^2 + C1Xi * t + C0Xi<br>
y(t) = C3Yi * t^3 + C2Yi * t^2 + C1Yi * t + C0Yi<br>
z(t) = C3Zi * t^3 + C2Zi * t^2 + C1Zi * t + C0Zi<br>
with t in [i,i+1]

<B>Types supported:</B> list 2D, list 3D

<B>Category:</B> draw geo
\ingroup  draw geo

\author Michel Couprie
*/

/*
%TEST points2spline %IMAGES/2dlist/binary/l2points1.list %RESULTS/points2spline_l2points1.spline
%TEST points2spline %IMAGES/3dlist/binary/l3points1.list %RESULTS/points2spline_l3points1.spline
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t i, npoints;
  char type;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s points.txt spline.txt \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }
  fscanf(fd, "%c", &type);
  if ((type != 'b') && (type != 'B'))
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", argv[0], type);
    exit(1);
  }

  if (type == 'b') // cas 2D
  {
    double *X, *Y, *t;
    double x, y;
    double *C0, *C1, *C2, *C3;
    double *D0, *D1, *D2, *D3;
    fscanf(fd, "%d", &npoints);

    if (npoints < 2)
    {
      fprintf(stderr, "%s: not enough points\n", argv[0]);
      exit(1);
    }

    X = (double *)calloc(1,npoints*sizeof(double));
    Y = (double *)calloc(1,npoints*sizeof(double));
    t = (double *)calloc(1,npoints*sizeof(double));
    C0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    C3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    D3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    if ((X == NULL) || (Y == NULL) || (t == NULL) || 
        (C0 == NULL) || (C1 == NULL) || (C2 == NULL) || (C3 == NULL) ||
        (D0 == NULL) || (D1 == NULL) || (D2 == NULL) || (D3 == NULL))
    {
      fprintf(stderr, "%s: malloc failed\n", argv[0]);
      exit(0);
    }

    for (i = 0; i < npoints; i++)
    {
      fscanf(fd, "%lf %lf", &x, &y); 
      X[i] = x; 
      Y[i] = y; 
      t[i] = (double)i; 
    }
    fclose(fd);

    if (npoints == 2)
    {
      C0[0] = X[0]; C1[0] = X[1] - X[0]; C2[0] = C3[0] = 0.0; 
      D0[0] = Y[0]; D1[0] = Y[1] - Y[0]; D2[0] = D3[0] = 0.0; 
    }
    else
    {
      if ((X[0] != X[npoints-1]) || (Y[0] != Y[npoints-1]))
      {
	scn_solvespline(t, X, npoints, C0, C1, C2, C3);
	scn_solvespline(t, Y, npoints, D0, D1, D2, D3);
      }
      else
      {
	scn_solveclosedspline(t, X, npoints, C0, C1, C2, C3);
	scn_solveclosedspline(t, Y, npoints, D0, D1, D2, D3);
      }
    }

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }
  
    fprintf(fd, "c %d\n", npoints);
    for (i = 0; i < npoints; i++) fprintf(fd, "%d %d\n", (int32_t)X[i], (int32_t)Y[i]);
    for (i = 0; i < npoints-1; i++) fprintf(fd, "%g %g %g %g %g %g %g %g\n", 
                                          C0[i], D0[i], C1[i], D1[i], C2[i], D2[i], C3[i], D3[i]);

    fclose(fd);
    free(X); free(Y); free(t);
    free(C0); free(C1); free(C2); free(C3);
    free(D0); free(D1); free(D2); free(D3);
  }
  else if (type == 'B')  // cas 3D
  {
    double *X, *Y, *Z, *t;
    double x, y, z;
    double *C0, *C1, *C2, *C3;
    double *D0, *D1, *D2, *D3;
    double *E0, *E1, *E2, *E3;
    fscanf(fd, "%d", &npoints);

    if (npoints < 2)
    {
      fprintf(stderr, "%s: not enough points\n", argv[0]);
      exit(1);
    }

    X = (double *)calloc(1,npoints*sizeof(double));
    Y = (double *)calloc(1,npoints*sizeof(double));
    Z = (double *)calloc(1,npoints*sizeof(double));
    t = (double *)calloc(1,npoints*sizeof(double));
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
    if ((X == NULL) || (Y == NULL) || (Z == NULL) || (t == NULL) || 
        (C0 == NULL) || (C1 == NULL) || (C2 == NULL) || (C3 == NULL) ||
        (D0 == NULL) || (D1 == NULL) || (D2 == NULL) || (D3 == NULL) ||
        (E0 == NULL) || (E1 == NULL) || (E2 == NULL) || (E3 == NULL))
    {
      fprintf(stderr, "%s: malloc failed\n", argv[0]);
      exit(0);
    }

    for (i = 0; i < npoints; i++)
    {
      fscanf(fd, "%lf %lf %lf", &x, &y, &z);
      X[i] = x; 
      Y[i] = y; 
      Z[i] = z; 
      t[i] = (double)i; 
    }
    fclose(fd);

    if (npoints == 2)
    {
      C0[0] = X[0]; C1[0] = X[1] - X[0]; C2[0] = C3[0] = 0.0; 
      D0[0] = Y[0]; D1[0] = Y[1] - Y[0]; D2[0] = D3[0] = 0.0; 
      E0[0] = Z[0]; E1[0] = Z[1] - Z[0]; E2[0] = E3[0] = 0.0; 
    }
    else
    {
      scn_solvespline(t, X, npoints, C0, C1, C2, C3);
      scn_solvespline(t, Y, npoints, D0, D1, D2, D3);
      scn_solvespline(t, Z, npoints, E0, E1, E2, E3);
    }

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }
  
    fprintf(fd, "C %d\n", npoints);
    for (i = 0; i < npoints; i++) fprintf(fd, "%d %d %d\n", (int32_t)X[i], (int32_t)Y[i], (int32_t)Z[i]);
    for (i = 0; i < npoints-1; i++) fprintf(fd, "%g %g %g %g %g %g %g %g %g %g %g %g\n", 
                                          C0[i], D0[i], E0[i], C1[i], D1[i], E1[i], C2[i], D2[i], E2[i], C3[i], D3[i], E3[i]);

    fclose(fd);
    free(X); free(Y); free(Z); free(t);
    free(C0); free(C1); free(C2); free(C3);
    free(D0); free(D1); free(D2); free(D3);
    free(E0); free(E1); free(E2); free(E3);
  }
  return 0;
} /* main */


