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
/*! \file pgm2curve.c

\brief extracts a curve from a binary image

<B>Usage:</B> pgm2curve image.pgm connex [x y [z]] out.curve

<B>Description:</B>
Extracts a curve from a binary image. 
The parameter \b connex is the connexity of the curve.
It may be equal to 4 or 8 in 2D, and to 6, 18 or 26 in 3D.
If given, the point <B>(x, y, z)</B> (2D) or <B>(x, y, z)</B> (3D) 
is the beginning of the curve, and must be an end point. 
The output is the text file \b out.curve, with the following format:<br>
b nbpoints<br>
x1 y1<br>
x2 y2<br>
...<br>
or (3D): 
B nbpoints<br>
x1 y1 z1<br>
x2 y2 z2<br>
...<br>

The points of the curve may also be valued. This is must be indicated by 
a value of 40, 80, 60, 180 or 260 for the parameter \b connex, instead
of 4, 8, 6, 18 or 26 respectively. In this case,
the output is the text file \b out.curve, with the following format:<br>
n nbpoints<br>
x1 y1 v1<br>
x2 y2 v2<br>
...<br>
or (3D): 
N nbpoints<br>
x1 y1 z1 v1<br>
x2 y2 z2 v2<br>
...<br>

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> convert geo
\ingroup  convert geo

\author Michel Couprie
*/

/*
  update Oct 27, 2010 : output formats are now b, B, n, N 
  (list formats) instead of c, V, cv, CV
 */

/*
%TEST pgm2curve %IMAGES/2dbyte/binary/b2curve1.pgm 8 %RESULTS/pgm2curve_b2curve1.list
%TEST pgm2curve %IMAGES/3dbyte/binary/b3curve1.pgm 26 %RESULTS/pgm2curve_b3curve1.list
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <lcurves.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, ds, ps, N, x, y, z, p, n, connex, val;
  uint8_t *F;
  int32_t *X, *Y, *Z = NULL;

  if ((argc != 7) && (argc != 6) && (argc != 4))
  {
    fprintf(stderr, "usage: %s image.pgm connex [x y [z]] out.curve \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  connex = atoi(argv[2]);
  if (connex >= 40)
  {
    val = 1;
    connex = connex / 10;
  }
  else val = 0;

  if (argc > 4)
  { 
    x = atoi(argv[3]);
    y = atoi(argv[4]);
  }
  if (argc > 6)
    z = atoi(argv[5]);

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  N = ps * ds;
  F = UCHARDATA(image);

  if (argc > 4)
  {   
    if (argc > 6) p = z*ps + y*rs + x; else p = y*rs + x;

    if (!F[p])
    {
      fprintf(stderr, "%s: bad starting point\n", argv[0]);
      exit(1);
    }

    if ((connex == 4) && (nbvois4(F, p, rs, N) != 1))
    {
      fprintf(stderr, "%s: (x,y) not end\n", argv[0]);
      exit(1);
    }
    else if ((connex == 8) && (nbvois8(F, p, rs, N) != 1))
    {
      fprintf(stderr, "%s: (x,y) not end\n", argv[0]);
      exit(1);
    }
    else if ((connex == 6) && (mctopo3d_nbvoiso6(F, p, rs, ps, N) != 1))
    {
      fprintf(stderr, "%s: (x,y,z) not end\n", argv[0]);
      exit(1);
    }
    else if ((connex == 18) && (mctopo3d_nbvoiso18(F, p, rs, ps, N) != 1))
    {
      fprintf(stderr, "%s: (x,y,z) not end\n", argv[0]);
      exit(1);
    }
    else if ((connex == 26) && (mctopo3d_nbvoiso26(F, p, rs, ps, N) != 1))
    {
      fprintf(stderr, "%s: (x,y,z) not end\n", argv[0]);
      exit(1);
    }
  }
  else // no given begin point
  {
    p = -1;
    if (connex == 4)
    { for (x = 0; x < N; x++) if ((F[x]) && (nbvois4(F, x, rs, N) == 1)) { p = x; break; } }
    else if (connex == 8)
    { for (x = 0; x < N; x++) if ((F[x]) && (nbvois8(F, x, rs, N) == 1)) { p = x; break; } }
    else if (connex == 6)
    { for (x = 0; x < N; x++) if ((F[x]) && (mctopo3d_nbvoiso6(F, x, rs, ps, N) == 1)) { p = x; break; } }
    else if (connex == 18)
    { for (x = 0; x < N; x++) if ((F[x]) && (mctopo3d_nbvoiso18(F, x, rs, ps, N) == 1)) { p = x; break; } }
    else if (connex == 26)
    { for (x = 0; x < N; x++) if ((F[x]) && (mctopo3d_nbvoiso26(F, x, rs, ps, N) == 1)) { p = x; break; } }
    if (p == -1)
    {
      fprintf(stderr, "%s: no end point\n", argv[0]);
      exit(1);
    }
  }

  if ((connex == 4) || (connex == 8))
    (void)lcurves_extractcurve(F, p, rs, N, connex, &X, &Y, &n);
  else
    (void)lcurves_extractcurve3d(F, p, rs, ps, N, connex, &X, &Y, &Z, &n);

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

  if (val)
  {
    if ((connex == 4) || (connex == 8))
    {
      fprintf(fd, "n %d\n", n); 
      for (p = 0; p < n; p++)
	fprintf(fd, "%d %d %d\n", X[p], Y[p], F[Y[p]*rs + X[p]]);
    }
    else
    {
      fprintf(fd, "N %d\n", n); 
      for (p = 0; p < n; p++) 
	fprintf(fd, "%d %d %d %d\n", X[p], Y[p], Z[p], F[Z[p]*ps + Y[p]*rs + X[p]]);
    }
  }
  else
  {
    if ((connex == 4) || (connex == 8))
    {
      fprintf(fd, "b %d\n", n); 
      for (p = 0; p < n; p++)
	fprintf(fd, "%d %d\n", X[p], Y[p]);
    }
    else
    {
      fprintf(fd, "B %d\n", n); 
      for (p = 0; p < n; p++) 
	fprintf(fd, "%d %d %d\n", X[p], Y[p], Z[p]);
    }
  }

  fclose(fd);
  freeimage(image);
  free(X);
  free(Y);
  if (Z != NULL) free(Z);
  return 0;
}
