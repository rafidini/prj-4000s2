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
/*! \file ellipsefind.c

\brief ellipse identification and drawing from spare points

<B>Usage:</B> ellipsefind pts.list out.list [in.pgm out.pgm]

<B>Description:</B>
Identifies the ellipse E which minimizes the sum of the (approximate) distances
between the points in \b pts.list and E. 
The results are written into the file \b out.list , in the form:
If the parameters \b in.pgm and \b out.pgm are given, the ellipse is drawn into the image \b in.pgm and the result is saved as \b out.pgm .

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/* 
  Michel Couprie - june 2017
*/

//#define CHRONO

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcutil.h>
#include <lbresen.h>
#include <lmoments.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  uint8_t *Im;
  int32_t rs, cs, d;
  FILE *fd = NULL;
  int32_t i, n;
  char type;
  double *x, *y, xx, yy, x0, y0, xa, ya, xb, yb;
  Indicateur indic;

  if ((argc != 3) && (argc != 5))
  {
    fprintf(stderr, "usage: %s pts.list out.list [in.pgm out.pgm]\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
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

  if (!llistemoments2d(x, y, n, &indic))
  {
    fprintf(stderr, "%s: llistemoments2d failed\n", argv[0]);
    exit(1);
  }

  free(x);
  free(y);

  x0 = indic.xcentre;  
  y0 = indic.ycentre; 
  xb = indic.gdiam * cos(indic.orien); 
  yb = indic.gdiam * sin(indic.orien); 
  xa = (indic.gdiam * indic.excen) * sin(indic.orien); 
  ya = (indic.gdiam * indic.excen) * cos(indic.orien); 

  if (argc == 5)
  {
    image = readimage(argv[3]);
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

    Im = UCHARDATA(image);
    lellipse(Im, rs, cs, arrondi(xa), arrondi(ya), arrondi(xb), arrondi(yb), arrondi(x0), arrondi(y0));
    writeimage(image, argv[argc-1]);
    freeimage(image);
  }

  return 0;
} /* main */

