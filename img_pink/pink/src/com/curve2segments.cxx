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
/*! \file curve2segments.cxx

\brief computes a covering of a digital curve by digital straigh line segments

<B>Usage:</B> curve2segments curve.list [tolerance] segments.list

<B>Description:</B>

Computes a covering of a digital curve by digital straight line segments. 
The input file contains a curve which is specified by the ordered list of its points.
The output file is an ordered list of points, which are the extremities of the segments.

Parameter \b tolerance gives the thickness of the digital straight line segments. Default value is 1.

\warning Curves must be 8-curves in 2D of 26-curves in 3D. Greedy algorithm.

<B>Types supported:</B> curve 2D, curve 3D

<B>Category:</B> geo
\ingroup geo

\author Michel Couprie
*/

/*
%TEST curve2segments %IMAGES/2dlist/binary/l2curve1.list %RESULTS/curve2segments_l2curve1.list
%TEST curve2segments %IMAGES/2dlist/binary/l2curve2.list %RESULTS/curve2segments_l2curve2.list
%TEST curve2segments %IMAGES/2dlist/binary/l2curve2.list 2 %RESULTS/curve2segments_l2curve2_2.list
%TEST curve2segments %IMAGES/3dlist/binary/l3curve1.list %RESULTS/curve2segments_l3curve1.list
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
  int32_t npoints;
  double tolerance;
  char type;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s curve.list [tolerance] out.list \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  if (argc == 3) tolerance = 1;
  else tolerance = atof(argv[2]);
  
  fscanf(fd, "%c", &type);
  if (type == 'b')
  {
    int32_t *X, *Y, nseg;

    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);

    for (i = 0; i < npoints; i++)
      fscanf(fd, "%d %d", &(X[i]), &(Y[i]));
    fclose(fd);

    nseg = CoverByDSSs(npoints, X, Y, tolerance);

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
      exit(1);
    }
    fprintf(fd, "b %d\n", nseg);
    for (i = 0; i < nseg; i++)
      fprintf(fd, "%d %d\n", X[i], Y[i]);
    fclose(fd);

    free(X); free(Y);
  }
  else if (type == 'B')
  {
    int32_t *X, *Y, *Z, nseg;

    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);
    Z = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);

    for (i = 0; i < npoints; i++)
      fscanf(fd, "%d %d %d", &(X[i]), &(Y[i]), &(Z[i]));
    fclose(fd);

    nseg = CoverByDSSs3D(npoints, X, Y, Z, tolerance);

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
      exit(1);
    }
    fprintf(fd, "B %d\n", nseg);
    for (i = 0; i < nseg; i++)
      fprintf(fd, "%d %d %d\n", X[i], Y[i], Z[i]);
    fclose(fd);

    free(X); free(Y); free(Z);
  }
  else
  {
    fprintf(stderr, "usage: %s: bad file format: %c \n", argv[0], type);
    exit(1);
  }

  return 0;
} /* main */
