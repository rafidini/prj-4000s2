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
/*! \file identifyparabola2.c

\brief identification of a best matching parabola from a set of 2D points

<B>Usage:</B> identifyparabola2 in.list out.list

<B>Description:</B>
Identifies the parameters (a,b) of the equation of the 2D parabola:
ax^2+b=y that minimizes the least square error between this parabola 
and the given points. Method: basic linear regression.

<B>Types supported:</B> list 1D, list 2D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/*
%TEST identifyparabola2 %IMAGES/2dlist/binary/l2parabola1.list %RESULTS/identifyparabola2_l2parabola1.list
%TEST identifyparabola2 %IMAGES/2dlist/binary/l2parabola2.list %RESULTS/identifyparabola2_l2parabola2.list
*/

/* 
  Michel Couprie - octobre 2009
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mclin.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t n, i;
  char type;
  double *pbx, *pby, a, b;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.list out.list\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type != 's') && (type != 'b'))
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  pbx = (double *)malloc(n * sizeof(double));
  pby = (double *)malloc(n * sizeof(double));

  if ((pbx == NULL) || (pby == NULL))
  {
    fprintf(stderr, "usage: %s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
    fscanf(fd, "%lf %lf\n", pbx+i, pby+i);

  fclose(fd);

  if (!lidentifyparabola2(pbx, pby, n, &a, &b))
  {
    fprintf(stderr, "%s: lidentifyparabola2 failed\n", argv[0]);
    exit(1);
  }

#ifdef VERBOSE
  printf("a = %g, b = %g\n", a, b);
#endif

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "e %d\n", 2); 
  fprintf(fd, "%g %g\n", a, b); 
  fclose(fd);

  free(pbx);
  free(pby);

  return 0;
}

