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
/*! \file drawsplinesorient.c

\brief draw spline orientations in a vector field (multiple spline version)

<B>Usage:</B> drawsplinesorient in.pgm in.splines out.pgm

<B>Description:</B>
Draws spline orientations in a vector field. 
The spline is specified by its control points in a text file.
The parameter \b in.pgm gives a vector field into which the spline is to be drawn.

The file \b splines.txt contains a list of splines under the format:<br>
d nb_splines<br>
nb_points_spline_1  x11 y11  x12 y12 ...<br>
nb_points_spline_2  x21 y21  x22 y22 ...<br>
nb_points_spline_3  x31 y31  x32 y32 ...<br>
...<br>
or, in 3D:<br>
D nb_splines<br>
nb_points_spline_1  x11 y11 z11  x12 y12 z12 ...<br>
nb_points_spline_2  x21 y21 z21  x22 y22 z22 ...<br>
nb_points_spline_3  x31 y31 z31  x32 y32 z32 ...<br>
...<br>

<B>Types supported:</B> spline 2D, spline 3D

<B>Category:</B> draw geo
\ingroup  draw geo

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
#include <mcutil.h>
#include <ldraw.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * field;
  int32_t j;
  FILE *fd = NULL;
  int32_t npoints, nsplines, i;
  double *x, *y, *z, *t, x1, y1, z1;
  double Px[4], Py[4], Pz[4];
  double *X0, *X1, *X2, *X3, *Y0, *Y1, *Y2, *Y3, *Z0, *Z1, *Z2, *Z3;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm splines.txt out.pgm \n", argv[0]);
    exit(1);
  }

  field = readimage(argv[1]);
  if (field == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }
  
  fscanf(fd, "%c %d", &type, &nsplines);
  if ((type != 'd') && (type != 'D'))
  {
    fprintf(stderr, "%s : bad file format : %c \n", argv[0], type);
    exit(1);
  }
#ifdef VERBOSE
  printf("%s: %d splines\n", argv[0], nsplines);
#endif

  if (type == 'd')
  {
    assert(depth(field) == 1);
    for (i = 0; i < nsplines; i++)
    {
      fscanf(fd, "%d", &npoints);
#ifdef VERBOSE
      printf("%s: spline %d nbpoints %d\n", argv[0], i, npoints);
#endif

      if (npoints < 2)
      {
	fprintf(stderr, "%s: not enough points in spline %d\n", argv[0], i);
	exit(1);
      }

      x = (double *)calloc(1,npoints*sizeof(double)); assert(x != NULL);
      y = (double *)calloc(1,npoints*sizeof(double)); assert(y != NULL);
      t = (double *)calloc(1,npoints*sizeof(double)); assert(t != NULL);
      X0 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(X0 != NULL);
      X1 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(X1 != NULL);
      X2 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(X2 != NULL);
      X3 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(X3 != NULL);
      Y0 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Y0 != NULL);
      Y1 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Y1 != NULL);
      Y2 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Y2 != NULL);
      Y3 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Y3 != NULL);
      for (j = 0; j < npoints; j++)
      {
	fscanf(fd, "%lf%lf", &x1, &y1);
	x[j] = x1; y[j] = y1; t[j] = (double)j;
      }
      scn_solvespline(t, x, npoints, X0, X1, X2, X3);
      scn_solvespline(t, y, npoints, Y0, Y1, Y2, Y3);
    
      for (j = 0; j < npoints-1; j++)
      {
	Px[0] = X0[j]; Px[1] = X1[j]; Px[2] = X2[j]; Px[3] = X3[j];
	Py[0] = Y0[j]; Py[1] = Y1[j]; Py[2] = Y2[j]; Py[3] = Y3[j];

	ldrawtangents2d(field, (double *)Px, (double *)Py, 10, t[j], t[j+1]);
      }

      free(x); free(y); free(t); 
      free(X0); free(X1); free(X2); free(X3);
      free(Y0); free(Y1); free(Y2); free(Y3);
    }
  }
  else // 3D
  {
    for (i = 0; i < nsplines; i++)
    {
      fscanf(fd, "%d", &npoints);
#ifdef VERBOSE
      printf("%s: spline %d nbpoints %d\n", argv[0], i, npoints);
#endif

      if (npoints < 2)
      {
	fprintf(stderr, "%s: not enough points in spline %d\n", argv[0], i);
	exit(1);
      }

      x = (double *)calloc(1,npoints*sizeof(double)); assert(x != NULL);
      y = (double *)calloc(1,npoints*sizeof(double)); assert(y != NULL);
      z = (double *)calloc(1,npoints*sizeof(double)); assert(z != NULL);
      t = (double *)calloc(1,npoints*sizeof(double)); assert(t != NULL);
      X0 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(X0 != NULL);
      X1 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(X1 != NULL);
      X2 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(X2 != NULL);
      X3 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(X3 != NULL);
      Y0 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Y0 != NULL);
      Y1 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Y1 != NULL);
      Y2 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Y2 != NULL);
      Y3 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Y3 != NULL);
      Z0 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Z0 != NULL);
      Z1 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Z1 != NULL);
      Z2 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Z2 != NULL);
      Z3 = (double *)calloc(1,(npoints-1)*sizeof(double)); assert(Z3 != NULL);
      for (j = 0; j < npoints; j++)
      {
	fscanf(fd, "%lf%lf%lf", &x1, &y1, &z1);
	x[j] = x1; y[j] = y1; z[j] = z1; t[j] = (double)j;
      }
      scn_solvespline(t, x, npoints, X0, X1, X2, X3);
      scn_solvespline(t, y, npoints, Y0, Y1, Y2, Y3);
      scn_solvespline(t, z, npoints, Z0, Z1, Z2, Z3);
    
      for (j = 0; j < npoints-1; j++)
      {
	Px[0] = X0[j]; Px[1] = X1[j]; Px[2] = X2[j]; Px[3] = X3[j];
	Py[0] = Y0[j]; Py[1] = Y1[j]; Py[2] = Y2[j]; Py[3] = Y3[j];
	Pz[0] = Z0[j]; Pz[1] = Z1[j]; Pz[2] = Z2[j]; Pz[3] = Z3[j];

	ldrawtangents3d(field, (double *)Px, (double *)Py, (double *)Pz, 10, t[j], t[j+1]);
      }

      free(x); free(y); free(z); free(t); 
      free(X0); free(X1); free(X2); free(X3);
      free(Y0); free(Y1); free(Y2); free(Y3);
      free(Z0); free(Z1); free(Z2); free(Z3);
    }
  }

  fclose(fd);

  writeimage(field, argv[argc-1]);
  freeimage(field);

  return 0;
} /* main */
