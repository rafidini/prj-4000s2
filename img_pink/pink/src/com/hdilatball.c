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
/*! \file hdilatball.c

\brief topologically controlled dilation

<B>Usage:</B> hdilatball in.pgm radius dist connex out.pgm

<B>Description:</B>
Performs a topologically controlled dilation, that is, a homotopic thickening 
constrained by the dilation of the input object.

The parameter \b radius gives the radius of the ball which is 
used as structuring element for the dilation.

The parameter \b dist is a numerical code
indicating the distance which is used to compute the dilation.
The possible choices are:
\li 0: approximate euclidean distance
\li 1: approximate quadratic euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

The parameter \b connex indicates the connectivity of the binary object.
Possible choices are 4, 8 in 2d and 6, 26 in 3d.

Let X be the set corresponding to the input image \b in.pgm, and let
Xbar denote its complementary set. The algorithm is the following:

\verbatim
I = dilatball(X, dist, r)
Repeat:
    Select a point x in [Xbar inter I] such that dist(x,X) is minimal
    If x is simple for X then
        X = X union {x}
	I = I \ {x}
Until [Xbar inter I] = emptyset
Result: X
\endverbatim

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie

*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lskeletons.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * prio;
  struct xvimage * inhibimage;
  int32_t connex, dist, i, N;
  uint8_t *F;
  int32_t *P;
  uint8_t *I;
  double radius;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in.pgm radius dist connex out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  radius = atof(argv[2]);
  dist = atoi(argv[3]);
  connex = atoi(argv[4]);

  prio = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  inhibimage = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
  if ((prio == NULL) || (inhibimage == NULL))
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  N = rowsize(image) * colsize(image) * depth(image);
  F = UCHARDATA(image);
  P = SLONGDATA(prio);
  I = UCHARDATA(inhibimage);
  
  if (dist == 0)
  {
    if (depth(image) == 1)
    {
      if (! ldisteuc(image, prio))
      {
	fprintf(stderr, "%s: ldisteuc failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      if (! ldisteuc3d(image, prio))
      {
	fprintf(stderr, "%s: ldisteuc3d failed\n", argv[0]);
	exit(1);
      }
    }
  }
  else
  if (dist == 1)
  {
    if (depth(image) == 1)
    {
      if (! ldistquad(image, prio))
      {
	fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      if (! ldistquad3d(image, prio))
      {
	fprintf(stderr, "%s: ldistquad3d failed\n", argv[0]);
	exit(1);
      }
    }
  }
  else
  if (dist == 2)
  {
    if (! lchamfrein(image, prio))
    {
      fprintf(stderr, "%s: lchamfrein failed\n", argv[0]);
      exit(1);
    }
  }
  else
  if (dist == 3)
  {
    if (! lsedt_meijster(image, prio))
    {
      fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! ldist(image, dist, prio))
    {
      fprintf(stderr, "%s: ldist failed\n", argv[0]);
      exit(1);
    }
  }
  
  for (i = 0; i < N; i++) // calcule la dilation inversee
    if (P[i] <= radius) I[i] = 0; else I[i] = NDG_MAX;

  for (i = 0; i < N; i++) // inverse l'image
    if (F[i]) F[i] = 0; else F[i] = NDG_MAX;

  if (depth(image) == 1)
  {
    if (! lskelubp2(image, prio, connex, inhibimage))
    {
      fprintf(stderr, "%s: lskelubp2 failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lskelubp3d2(image, prio, connex, inhibimage))
    {
      fprintf(stderr, "%s: lskelubp3d2 failed\n", argv[0]);
      exit(1);
    }
  }
  for (i = 0; i < N; i++) // re-inverse l'image
    if (F[i]) F[i] = 0; else F[i] = NDG_MAX;

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(prio);
  freeimage(inhibimage);

  return 0;
} /* main */
