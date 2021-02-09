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
/*! \file drawellipse.c

\brief Draws an ellipse

<B>Usage:</B> drawellipse in.pgm x1 y1 x2 y2 X1 Y1 X2 Y2 [filled] out.pgm

<B>Description:</B>
Draws an  ellipse specified by four points, e.g. the extremities of the small axis 
(<B>x1</B>, <B>y1</B>, <B>x2</B>, <B>y2</B>) and the extremities of the big axis 
(<B>X1</B>, <B>Y1</B>, <B>X2</B>, <B>Y2</B>).
The ellipse is surimposed to the contents of <B>in.pgm</B>.
If the (optional) int32_t parameter \b filled is set (1), then the ellipse is filled.
Otherwise it is empty (default).

<B>Types supported:</B> byte 2d

<B>Category:</B> draw geo
\ingroup  draw geo

\author Michel Couprie
*/

/* Michel Couprie - novembre 2005 */

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
#include <lellipseincl.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * ellipse;
  uint8_t *Im;
  uint8_t *El;
  int32_t rs, cs, d, N;
  int32_t i;
  double R, S, T, U, V, Z;
  double *x, *y;
  int32_t filled;

  if ((argc != 11) && (argc != 12))
  {
    fprintf(stderr, "usage: %s in.pgm x1 y1 x2 y2 X1 Y1 X2 Y2 [filled] out.pgm\n", argv[0]);
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

  x = (double *)calloc(1,4 * sizeof(double));
  y = (double *)calloc(1,4 * sizeof(double));
  if ((x == NULL) || (y == NULL))
  {
    fprintf(stderr, "%s: calloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < 4; i++)
  {
    x[i] = atof(argv[2*i + 2]);
    y[i] = atof(argv[2*i + 3]);
  }

  if (argc == 12) filled = atoi(argv[10]); else filled = 0;

  if (!lellipseincl(x, y, 4, &R, &S, &T, &U, &V, &Z))
  {
    fprintf(stderr, "%s: lellipseincl failed\n", argv[0]);
    exit(1);
  }

  //printf("%g %g %g %g %g %g\n", R, S, T, U, V, Z);

  free(x);
  free(y);

  ellipse = copyimage(image);
  El = UCHARDATA(ellipse);
  ldrawfilledellipse(ellipse, R, S, T, U, V, Z);
  if (!filled) 
  {
    if (! mctopo3d_lborder(ellipse, 8))
    {
      fprintf(stderr, "lellipseincl: lborder failed\n");
      exit(1);
    }
  }
  for (i = 0; i < N; i++) Im[i] = mcmax(Im[i],El[i]);
  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(ellipse);

  return 0;
} /* main */

