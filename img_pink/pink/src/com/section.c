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
/*! \file section.c

\brief computes a 2d cross section from a 3d image

<B>Usage:</B> section in.pgm x0 y0 z0 x1 y1 z1 x2 y2 z2 out.pgm

<B>Description:</B> 
The points P0 ( \b x0, \b y0, \b z0 ), P1 ( \b x1, \b y1, \b z1 ) and
P2 ( \b x2, \b y2, \b z2 ) form a basis for a plane in the 3d space. 
The intersection of this plane with the domain of the 3d image \b in.pgm
form the domain of the 2d image \b out.pgm .
The grayscale values of \b out.pgm are obtained by trilinear interpolation from
the grayscale values of \b in.pgm .
The coordinates of the basis for the plane are expressed in "real world" dimensions.

<B>Types supported:</B> byte 3d, int32_t 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - decembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * out;
  double x0, y0, z0, x1, y1, z1, x2, y2, z2;
  int i0, j0;

  if (argc != 12)
  {
    fprintf(stderr, "usage: %s in.pgm x0 y0 z0 x1 y1 z1 x2 y2 z2 out.pgm\n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  x0 = atof(argv[2]);
  y0 = atof(argv[3]);
  z0 = atof(argv[4]);
  x1 = atof(argv[5]);
  y1 = atof(argv[6]);
  z1 = atof(argv[7]);
  x2 = atof(argv[8]);
  y2 = atof(argv[9]);
  z2 = atof(argv[10]);

  out = lsection(in, x0, y0, z0, x1, y1, z1, x2, y2, z2, &i0, &j0);
  if (out == NULL)
  {
    fprintf(stderr, "%s: lsection failed\n", argv[0]);
    exit(1);
  }

  writese(out, argv[argc-1], i0, j0, 0);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
