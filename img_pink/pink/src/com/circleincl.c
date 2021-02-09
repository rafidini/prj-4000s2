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
/*! \file circleincl.c

\brief circle identification and drawing from spare points

<B>Usage:</B> circleincl in.pgm pts.list filled out.pgm

<B>Description:</B>

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author John Chaussard
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcutil.h>
#include <ldraw.h>
#include <lborder.h>
#include <lballincl.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * circle;
  uint8_t *Im;
  uint8_t *El;
  int32_t rs, cs, d, N;
  FILE *fd = NULL;
  int32_t i, n;
  char type;
  double *points, xx, yy, R, X, Y;
  int32_t filled;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm pts.list filled out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  if (d > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }
  N = rs * cs;
  Im = UCHARDATA(image);

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'b')
  {
    fprintf(stderr, "usage: %s : bat file format : %c \n", type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  points = (double *)calloc(1, 2 * n * sizeof(double));
  if (points == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf\n", &xx, &yy);
    points[i+i] = xx;
    points[i+i+1] = yy;
  }
  fclose(fd);

  filled = atoi(argv[3]);

  if (!compute_min_disk_with_border_constraint(points, n, NULL, 0, &X, &Y, &R))
  {
    fprintf(stderr, "%s: compute_min_disk_with_border_constraint failed\n", argv[0]);
    exit(1);
  }

#ifdef VERBOSE
  printf("center %g,%g radius %g\n", X, Y, R);
#endif

  free(points);

  circle = copyimage(image);
  El = UCHARDATA(circle);
  ldrawball(circle, R, X, Y, 0);
  if (!filled) 
  {
    if (! mctopo3d_lborder(circle, 8))
    {
      fprintf(stderr, "lcircleincl: mctopo3d_lborder failed\n");
      exit(1);
    }
  }
  for (i = 0; i < N; i++) Im[i] = mcmax(Im[i],El[i]);
  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(circle);

  return 0;
} /* main */

