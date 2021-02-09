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
/*! \file drawfieldlists.c

\brief draw line segments that represent a vector field (represented by two lists)

<B>Usage:</B> drawfieldlists in1.list in2.list in.pgm [len] out.pgm

<B>Description:</B>
The files \b in1.pgm, \b in2.pgm represent a vector field, with the origins of vectors in \b in1.pgm and their directions in \b in2.pgm. This operator draws in the image \b in.pgm a straight line segment of length \b len (default 10) for each vector.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/
/*
  Michel Couprie - novembre 2010 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ldraw.h>
#include <lbresen.h>

//#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, ds, npoints, n, i; 
  char type;
  int32_t *X, *Y, *Z;
  double *tx, *ty, *tz;
  double len = 10;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in1.list in2.list in.pgm [len] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[3]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);

  if (argc == 6) len = atof(argv[4]);

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type == 'b')
  {
    assert(ds == 1);
    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);

    for (i = 0; i < npoints; i++)
      fscanf(fd, "%d %d", &(X[i]), &(Y[i]));

    fclose(fd);

    fd = fopen(argv[2],"r");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }

    fscanf(fd, "%c", &type);
    assert(type == 'b');

    fscanf(fd, "%d", &n);
    assert(n == npoints);

    tx = (double *)calloc(1,npoints*sizeof(double)); assert(tx != NULL);
    ty = (double *)calloc(1,npoints*sizeof(double)); assert(ty != NULL);

    for (i = 0; i < npoints; i++)
      fscanf(fd, "%lg %lg", &(tx[i]), &(ty[i]));

    ldrawfield2dlist(npoints, X, Y, tx, ty, image, len);

    fclose(fd);
    free(X);
    free(Y);
    free(tx);
    free(ty);
  }  
  else
  {
    assert(type == 'B');
    assert(ds > 1);
    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);
    Z = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Z != NULL);

    for (i = 0; i < npoints; i++)
      fscanf(fd, "%d %d %d", &(X[i]), &(Y[i]), &(Z[i]));

    fclose(fd);

    fd = fopen(argv[2],"r");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }

    fscanf(fd, "%c", &type);
    assert(type == 'B');

    fscanf(fd, "%d", &n);
    assert(n == npoints);

    tx = (double *)calloc(1,npoints*sizeof(double)); assert(tx != NULL);
    ty = (double *)calloc(1,npoints*sizeof(double)); assert(ty != NULL);
    tz = (double *)calloc(1,npoints*sizeof(double)); assert(tz != NULL);

    for (i = 0; i < npoints; i++)
      fscanf(fd, "%lf %lf %lf", &(tx[i]), &(ty[i]), &(tz[i]));

    fclose(fd);

    ldrawfield3dlist(npoints, X, Y, Z, tx, ty, tz, image, len);

    free(X);
    free(Y);
    free(Z);
    free(tx);
    free(ty);
    free(tz);
  }  

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

