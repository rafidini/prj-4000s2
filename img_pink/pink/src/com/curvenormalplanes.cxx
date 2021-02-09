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
/*! \file curvenormalplanes.c

\brief computes the normal planes of a digital curve

<B>Usage:</B> curvenormalplanes curve.list [ngauss] normalplanes.list

<B>Description:</B>

Computes the normal planes of a curve which is specified by the ordered list of its points. The output file is a list of normalized normal plane vectors, all with origin 0.

If given, \b ngauss specifies the number of gaussian filtering steps applied to the tangent estimations.

<B>Types supported:</B> curve 2D, curve 3D

<B>Category:</B> geo
\ingroup geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ltangents.h>

#define DEBUG
#define DEBUG_2

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t i;
  FILE *fd = NULL;
  int32_t npoints, ngauss;
  char type;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s curve.list [ngauss] out.list \n", argv[0]);
    exit(1);
  }

  if (argc == 3) ngauss = 0;
  else ngauss = atoi(argv[2]);

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);

  if (type == 'B')
  {
    int32_t *X, *Y, *Z;
    double *Xnp, *Ynp, *Znp;

    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);
    Z = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Z != NULL);
    for (i = 0; i < npoints; i++)
      fscanf(fd, "%d %d %d", &(X[i]), &(Y[i]), &(Z[i]));
    fclose(fd);

    Xnp = (double *)malloc(2 * npoints * sizeof(double)); assert(Xnp != NULL);
    Ynp = (double *)malloc(2 * npoints * sizeof(double)); assert(Ynp != NULL);
    Znp = (double *)malloc(2 * npoints * sizeof(double)); assert(Znp != NULL);

  
    if (!lcurvenormalplanes3D(ngauss, npoints, X, Y, Z, Xnp, Ynp, Znp))
    {
      fprintf(stderr, "%s: procedure lcurvenormalplanes3D failed\n", argv[0]);
      exit(1);
    }

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
      exit(1);
    }
    fprintf(fd, "B %d\n", npoints+npoints);
    for (i = 0; i < npoints+npoints; i++)
      fprintf(fd, "%g %g %g\n", Xnp[i], Ynp[i], Znp[i]);
    fclose(fd);

    free(Xnp);
    free(Ynp);
    free(Znp);
  }
  else
  {
    fprintf(stderr, "usage: %s: bad file format: %c \n", argv[0], type);
    exit(1);
  }

  return 0;
} /* main */
