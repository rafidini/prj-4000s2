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
/*! \file lengthspline.c

\brief computes the length of a spline which is specified by its control points in a text file

<B>Usage:</B> lengthspline spline.txt [out.list]

<B>Description:</B>
Computes the length of a spline which is specified by its control points in a text file.
The file format for \b spline.txt is the following for 2D:

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

<B>Types supported:</B> spline 2D, spline 3D

<B>Category:</B> geo
\ingroup geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>
#include <mcutil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t j;
  FILE *fd = NULL;
  int32_t npoints;
  double *x, *y, *z, *t, L;
  double xx, yy, zz;
  double *X0, *X1, *X2, *X3, *Y0, *Y1, *Y2, *Y3, *Z0, *Z1, *Z2, *Z3;
  char type;

  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s spline.txt [out.list] \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type == 's') || (type == 'S'))
    fprintf(stderr, "%s: WARNING: use tags c and C for single splines, not %c\n", argv[0], type);

  if ((type != 'c') && (type != 'C') && (type != 's') && (type != 'S'))
  {
    fprintf(stderr, "%s: bad file format : %c \n", argv[0], type);
    exit(1);
  }
  fscanf(fd, "%d", &npoints);

  if (npoints < 2)
  {
    fprintf(stderr, "%s: not enough points\n", argv[0]);
    exit(1);
  }

  if ((type == 'c') || (type == 's'))
  {
    double p0, p1, p2, p3, q0, q1, q2, q3;
    x = (double *)calloc(1,npoints*sizeof(double));
    y = (double *)calloc(1,npoints*sizeof(double));
    t = (double *)calloc(1,npoints*sizeof(double));
    X0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y3 = (double *)calloc(1,(npoints-1)*sizeof(double));

    for (j = 0; j < npoints; j++)
    {
      fscanf(fd, "%lf%lf", &xx, &yy);
      x[j] = xx; y[j] = yy; t[j] = (double)j;
    }
    for (j = 0; j < npoints-1; j++)
    {
      fscanf(fd, "%lf%lf%lf%lf%lf%lf%lf%lf", &p0, &q0, &p1, &q1, &p2, &q2, &p3, &q3);
      X0[j] = p0; X1[j] = p1; X2[j] = p2; X3[j] = p3; 
      Y0[j] = q0; Y1[j] = q1; Y2[j] = q2; Y3[j] = q3; 
    }
    fclose(fd);

    L = scn_lengthspline(X0, X1, X2, X3, Y0, Y1, Y2, Y3, npoints);

    if (argc == 3)
    {
      fd = fopen(argv[argc - 1],"w");
      if (!fd)
      {
	fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
	exit(1);
      }
      fprintf(fd, "e %d\n", 1); 
      fprintf(fd, "%g\n", L); 
      fclose(fd);
    }
    else printf("%g\n", L); 

    free(x); free(y); free(t); 
    free(X0); free(X1); free(X2); free(X3);
    free(Y0); free(Y1); free(Y2); free(Y3);
  }
  else //3D
  {
    double p0, p1, p2, p3, q0, q1, q2, q3, r0, r1, r2, r3;
    x = (double *)calloc(1,npoints*sizeof(double));
    y = (double *)calloc(1,npoints*sizeof(double));
    z = (double *)calloc(1,npoints*sizeof(double));
    t = (double *)calloc(1,npoints*sizeof(double));
    X0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Z0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Z1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Z2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Z3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    for (j = 0; j < npoints; j++)
    {
      fscanf(fd, "%lf%lf%lf", &xx, &yy, &zz);
      x[j] = xx; y[j] = yy; z[j] = zz; t[j] = (double)j;
    }
    for (j = 0; j < npoints-1; j++)
    {
      fscanf(fd, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", &p0, &q0, &r0, &p1, &q1, &r1, &p2, &q2, &r2, &p3, &q3, &r3);
      X0[j] = p0; X1[j] = p1; X2[j] = p2; X3[j] = p3; 
      Y0[j] = q0; Y1[j] = q1; Y2[j] = q2; Y3[j] = q3; 
      Z0[j] = r0; Z1[j] = r1; Z2[j] = r2; Z3[j] = r3; 
    }
    fclose(fd);

    L = scn_lengthspline3d(X0, X1, X2, X3, Y0, Y1, Y2, Y3, Z0, Z1, Z2, Z3, npoints);

    if (argc == 3)
    {
      fd = fopen(argv[argc - 1],"w");
      if (!fd)
      {
	fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
	exit(1);
      }
      fprintf(fd, "e %d\n", 1); 
      fprintf(fd, "%g\n", L); 
      fclose(fd);
    }
    else printf("%g\n", L); 

    free(x); free(y); free(z); free(t); 
    free(X0); free(X1); free(X2); free(X3);
    free(Y0); free(Y1); free(Y2); free(Y3);
    free(Z0); free(Z1); free(Z2); free(Z3);
  }

  return 0;
} /* main */


