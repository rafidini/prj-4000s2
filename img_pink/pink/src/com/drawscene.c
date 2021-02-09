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
/*! \file drawscene.c

\brief draw a 3D scene which is specified by a text file into a 3D pgm image

<B>Usage:</B> drawscene in.pgm scene.3sc out.pgm

<B>Description:</B>
The file \b scene.3sc contains a scene under the 3Dscene format.

<B>Types supported:</B> byte 3D

<B>Category:</B> draw geo
\ingroup  draw geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>
#include <ldraw.h>

double pol3(double *P, double v) { return P[0] + P[1]*v +  P[2]*v*v +  P[3]*v*v*v; }

/* =============================================================== */
static void process_spline(FILE *fd, struct xvimage * image)
/* =============================================================== */
{
  int32_t j, npoints;
  double *x, *y, *z, *t, x1, y1, z1;
  double Px[4], Py[4], Pz[4];
  double *X0, *X1, *X2, *X3, *Y0, *Y1, *Y2, *Y3, *Z0, *Z1, *Z2, *Z3;

  fscanf(fd, "%d", &npoints);
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

    ldrawcubic3d(image, (double *)Px, (double *)Py, (double *)Pz, 10, t[j], t[j+1]);
  }

  free(x); free(y); free(z); free(t); 
  free(X0); free(X1); free(X2); free(X3);
  free(Y0); free(Y1); free(Y2); free(Y3);
  free(Z0); free(Z1); free(Z2); free(Z3);
} // process_spline()

/* =============================================================== */
static void process_closedspline(FILE *fd, struct xvimage * image)
/* =============================================================== */
{
  int32_t j, npoints;
  double *x, *y, *z, *t, x1, y1, z1;
  double Px[4], Py[4], Pz[4];
  double *X0, *X1, *X2, *X3, *Y0, *Y1, *Y2, *Y3, *Z0, *Z1, *Z2, *Z3;

  fscanf(fd, "%d", &npoints);
  x = (double *)calloc(1,(npoints+1)*sizeof(double));
  y = (double *)calloc(1,(npoints+1)*sizeof(double));
  z = (double *)calloc(1,(npoints+1)*sizeof(double));
  t = (double *)calloc(1,(npoints+1)*sizeof(double));
  X0 = (double *)calloc(1,npoints*sizeof(double));
  X1 = (double *)calloc(1,npoints*sizeof(double));
  X2 = (double *)calloc(1,npoints*sizeof(double));
  X3 = (double *)calloc(1,npoints*sizeof(double));
  Y0 = (double *)calloc(1,npoints*sizeof(double));
  Y1 = (double *)calloc(1,npoints*sizeof(double));
  Y2 = (double *)calloc(1,npoints*sizeof(double));
  Y3 = (double *)calloc(1,npoints*sizeof(double));
  Z0 = (double *)calloc(1,npoints*sizeof(double));
  Z1 = (double *)calloc(1,npoints*sizeof(double));
  Z2 = (double *)calloc(1,npoints*sizeof(double));
  Z3 = (double *)calloc(1,npoints*sizeof(double));
  for (j = 0; j < npoints; j++)
  {
    fscanf(fd, "%lf%lf%lf", &x1, &y1, &z1);
    x[j] = x1; y[j] = y1; z[j] = z1; t[j] = (double)j;
  }
  x[npoints] = x[0]; y[npoints] = y[0]; z[npoints] = z[0]; t[npoints] = (double)npoints;

  scn_solveclosedspline(t, x, npoints+1, X0, X1, X2, X3);
  scn_solveclosedspline(t, y, npoints+1, Y0, Y1, Y2, Y3);
  scn_solveclosedspline(t, z, npoints+1, Z0, Z1, Z2, Z3);
    
  for (j = 0; j < npoints; j++)
  {
    Px[0] = X0[j]; Px[1] = X1[j]; Px[2] = X2[j]; Px[3] = X3[j];
    Py[0] = Y0[j]; Py[1] = Y1[j]; Py[2] = Y2[j]; Py[3] = Y3[j];
    Pz[0] = Z0[j]; Pz[1] = Z1[j]; Pz[2] = Z2[j]; Pz[3] = Z3[j];

    ldrawcubic3d(image, (double *)Px, (double *)Py, (double *)Pz, 10, t[j], t[j+1]);
  }

  free(x); free(y); free(z); free(t); 
  free(X0); free(X1); free(X2); free(X3);
  free(Y0); free(Y1); free(Y2); free(Y3);
  free(Z0); free(Z1); free(Z2); free(Z3);
} // process_closedspline()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t i, ret;
  FILE *fd = NULL;
  int32_t nobj;
  char buf[1024];

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm scene.3sc out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
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

  ret = fscanf(fd, "%s", buf);
  if (strncmp(buf, "3Dscene", 7) != 0)
  {
    fprintf(stderr, "%s : bad file format 1 : %s\n", argv[0], buf);
    exit(1);
  }

  ret = fscanf(fd, "%d", &nobj);
  if (ret != 1)
  {
    fprintf(stderr, "%s : bad file format 2\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < nobj; i++)
  {
    ret = fscanf(fd, "%s", buf);
    if (ret != 1)
    {
      fprintf(stderr, "%s : bad file format 3\n", argv[0]);
      exit(1);
    }
    if (strncmp(buf, "spline", 6) == 0) process_spline(fd, image);
    else if (strncmp(buf, "closedspline", 6) == 0) process_closedspline(fd, image);
  } // for (i = 0; i < nobj; i++)

  fclose(fd);

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


