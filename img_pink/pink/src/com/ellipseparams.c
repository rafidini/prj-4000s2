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
/*! \file ellipseparams.c

\brief ellipse identification and main parameter computation

<B>Usage:</B> ellipseparams pts.list out.list

<B>Description:</B>
Identifies the ellipse E which minimizes the sum of the (approximate) distances
between the points in \b pts.list and E. 
The output is a list containing 6 numbers: a1, a2, r1, r2, r3, r4
where a1 is the half of the big axis length, a2 is the
the half of the little axis length, and each ri is the mean "distance",
for quadrant i, between intput points and the computed ellipse 
(the expected value for points forming a perfect ellipse is 0)

Ref: 
Andrew W. Fitzgibbon, Maurizio Pilu, and Robert B. Fisher
Direct least-squares fitting of ellipses,
IEEE Transactions on Pattern Analysis and Machine Intelligence, 21(5), 476--480, May 1999

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/* 
  Michel Couprie - mars 2006
*/

//#define CHRONO

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcutil.h>
#include <lbresen.h>
#include <lellipsefit.h>

#ifdef CHRONO
#include <mcchrono.h>
#endif

/* ==================================== */
static double dist2(double x1, double y1, double x2, double y2)
/* ==================================== */
{
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t i, n;
  char type;
  double *x, *y, xx, yy, x0, y0, xa, ya, xb, yb;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s pts.list out.list\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'b')
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  x = (double *)calloc(1,n * sizeof(double));
  y = (double *)calloc(1,n * sizeof(double));
  if ((x == NULL) || (y == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf\n", &xx, &yy);
    x[i] = xx;
    y[i] = yy;
  }
  fclose(fd);


#ifdef CHRONO
  chrono Chrono1;
  start_chrono(&Chrono1);
#endif
  if (!lellipsefit(x, y, n, &x0, &y0, &xa, &ya, &xb, &yb))
  {
    fprintf(stderr, "%s: lellipsefit failed\n", argv[0]);
    exit(1);
  }
#ifdef CHRONO
  printf("nb points = %d ; nb de usec ecoulees = %d\n", n, read_chrono(&Chrono1));
#endif

  fd = fopen(argv[argc-1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

  //printf("%g %g %g %g %g %g\n", x0, y0, xa, ya, xb, yb);
  fprintf(fd, "e 6\n");

  // longueur demi-grand axe et demi-petit axe
  fprintf(fd, "%g %g\n", dist2(x0, y0, xa, ya), dist2(x0, y0, xb, yb));

  // calcule la matrice N de changement de base pour exprimer les points d'entrée 
  // dans le repère lié à l'ellipse
  {
    double M[4], N[4], P[2], Q[2];
    M[0] = xa-x0; M[1] = ya-y0; M[2] = xb-x0; M[3] = yb-y0;
    if (!lin_inverseLUP((double *)M, (double *)N, 2))
    {
      fprintf(stderr, "%s: lin_inverseLUP failed\n", argv[0]);
      exit(1);
    }
    //passe les points dans le nouveau repère
    for (i = 0; i < n; i++)
    {
      P[0] = x[i]-x0; P[1] = y[i]-y0;
      lin_mult(N, P, Q, 2, 2, 1);
      x[i] = Q[0]; y[i] = Q[1];
    }
  }
  // calcule le rayon moyen par quandrant dans le nouveau repère (cas idéal : 1)
  {
    double r1, r2, r3, r4;
    int n1, n2, n3, n4;
    r1 = r2 = r3 = r4 = 0.0;
    n1 = n2 = n3 = n4 = 0;
    for (i = 0; i < n; i++)
    {
      if (x[i] >= 0)
      {
	if (y[i] >= 0)
	{
	  r1 += x[i]*x[i] + y[i]*y[i]; n1++;
	}
	else
	{
	  r2 += x[i]*x[i] + y[i]*y[i]; n2++;
	}
      }
      else
      {
	if (y[i] >= 0)
	{
	  r3 += x[i]*x[i] + y[i]*y[i]; n3++;	  
	}
	else
	{
	  r4 += x[i]*x[i] + y[i]*y[i]; n4++;
	}
      }
    }    
    fprintf(fd, "%g %g %g %g\n", 1.0 - r1/n1, 1.0 - r2/n2, 1.0 - r3/n3, 1.0 - r4/n4);
  }

  fclose(fd);
  free(x);
  free(y);

  return 0;
} /* main */

