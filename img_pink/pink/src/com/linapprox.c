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
/* \file linapprox.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>linapprox</B> - 

SYNOPSIS

<B>linaprox</B> in.list epsilon out.list

DESCRIPTION

    From: "Optimum Uniform Piecewise Linear Approximation of Planar Curves", J.G. Dunham, 
    IEEE PAMI v8 n1 1986

CLASS 

geo

*/

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llinapprox.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  double *x;
  double *y; 
  int32_t i, n;
  double eps;
  FILE *fd = NULL;
  double xx, yy;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.list epsilon out.list \n", argv[0]);
    exit(1);
  }

  eps = atof(argv[2]);

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'b')
  {
    fprintf(stderr, "usage: %s : bat file format : %c \n", type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  x= (double *)calloc(1,n * sizeof(double));
  y= (double *)calloc(1,n * sizeof(double));

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf\n", &xx, &yy);
    x[i] = xx;
    y[i] = yy;
  }

  fclose(fd);

  SJCurveAproximate(x,y,n,eps);

  free(x);
  free(y);
  

  return 0;
} /* main */


