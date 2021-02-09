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
/*! \file samplespline.c

\brief samples a cubic spline according to its curvilinear abcissa

<B>Usage:</B> samplespline in.txt nsamples out.txt

<B>Description:</B>

This program computes a regular sampling of the spline given in <B>in.txt</B>.
The result is a list of <B>nsamples</B> points, which are stored in <B>out.txt</B>.

<B>Types supported:</B> list 2D, list 3D

<B>Category:</B> draw geo
\ingroup  draw geo

\author Michel Couprie
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
  int32_t i, nsamples;
  FILE *fd = NULL;
  int32_t npoints;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.txt nsamples out.txt \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }
  nsamples = atoi(argv[2]);
  fscanf(fd, "%c", &type);
  if ((type != 's') && (type != 'S') && (type != 'b') && (type != 'B'))
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", argv[0], type);
    exit(1);
  }

  if ((type == 's') || (type == 'b')) // cas 2D
  {
    double *x, *y, *X, *Y;
    fscanf(fd, "%d", &npoints);
    x = (double *)calloc(1,npoints*sizeof(double));
    y = (double *)calloc(1,npoints*sizeof(double));
    X = (double *)calloc(1,nsamples*sizeof(double));
    Y = (double *)calloc(1,nsamples*sizeof(double));
    for (i = 0; i < npoints; i++)
    {
      fscanf(fd, "%lf %lf", &(x[i]), &(y[i]));
    }
    fclose(fd);

    scn_samplespline(x, y, npoints, nsamples, X, Y);

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }
  
    fprintf(fd, "b %d\n", nsamples);
    for (i = 0; i < nsamples; i++) fprintf(fd, "%g %g\n", X[i], Y[i]);
    fclose(fd);

    free(x); free(y);
    free(X); free(Y);
  }
  else if ((type == 'S') || (type == 'B')) // cas 3D
  {
    double *x, *y, *z, *X, *Y, *Z;
    fscanf(fd, "%d", &npoints);
    x = (double *)calloc(1,npoints*sizeof(double));
    y = (double *)calloc(1,npoints*sizeof(double));
    z = (double *)calloc(1,npoints*sizeof(double));
    X = (double *)calloc(1,nsamples*sizeof(double));
    Y = (double *)calloc(1,nsamples*sizeof(double));
    Z = (double *)calloc(1,nsamples*sizeof(double));
    for (i = 0; i < npoints; i++)
    {
      fscanf(fd, "%lf %lf %lf", &(x[i]), &(y[i]), &(z[i]));
    }
    fclose(fd);

    scn_samplespline3d(x, y, z, npoints, nsamples, X, Y, Z);

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }
  
    fprintf(fd, "B %d\n", nsamples);
    for (i = 0; i < nsamples; i++) fprintf(fd, "%g %g %g\n", X[i], Y[i], Z[i]);
    fclose(fd);

    free(x); free(y); free(z);
    free(X); free(Y); free(Z);
  }
  return 0;
} /* main */


