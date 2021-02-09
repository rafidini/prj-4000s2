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
/*! \file fitcircle.c

\brief fits a circle which locally minimizes gray levels

<B>Usage:</B> fitcircle in.pgm x y r [translate] out.pgm

<B>Description:</B>
Identifies the circle C which locally minimizes the mean gray level 
of the image \b in.pgm in the vincinity of the circle
of parameters \b x , \b y and \b r.
If the parameter \b translate is added, then the minimization is
done only for dimensions x and y.
Draws this circle in \b out.pgm .

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/* 
  Michel Couprie - avril 2002 
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <lbresen.h>
#include <lgeo.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * image;
  uint8_t *Im;
  int32_t rs, cs, d, N;
  double x, y, r;
  cercle *C;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm x y r [translate] out.pgm \n", argv[0]);
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

  x = atof(argv[2]);
  y = atof(argv[3]);
  r = atof(argv[4]);

  if (argc == 6)
    C = AjusteCercle(image, x, y, r);
  else
    C = AjusteCercle2(image, x, y, r);

  printf("%g %g %g\n", C->x0, C->y0, C->r);

  memset(Im, 0, N);
  lellipse(Im, rs, cs, 0, arrondi(C->r), arrondi(C->r), 0, 
           arrondi(C->x0), arrondi(C->y0));

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

