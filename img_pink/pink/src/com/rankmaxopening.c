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
/*! \file rankmaxopening.c

\brief rank-max opening 

<B>Usage:</B> rankmaxopening in.pgm el.pgm r out.pgm

<B>Description:</B>
Let F be the input image, G be the output image, and E the structuring
element.

The rank-max opening [Ron86]
of structuring element $E$ and parameter \b r in [0...1]
may be defined by G = min(F,dilation(rankfilter(F,E,1-r),E))

[Ron86] C. Ronse:
"Erosion of narrow image features by combination of local low rank and max filters",
Proc. 2nd Int. Conf. on Image Processing and int32_t Applications,
pages 77-81, 1986.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup morpho

\author Michel Couprie
*/
/* Michel Couprie - janvier 2005 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltreordre.h>
#include <ldilateros.h>
#include <ldilateros3d.h>
#include <larith.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * orig;
  struct xvimage * elem;
  index_t x, y, z;
  double r;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm el.pgm r out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  elem = readse(argv[2], &x, &y, &z);
  if (elem == NULL)
  {
    fprintf(stderr, "%s: readse failed\n", argv[0]);
    exit(1);
  }
  
  r = atof(argv[3]);
  if ((r < 0.0) || (r > 1.0))
  {
    fprintf(stderr, "%s: r = %g ; on doit avoir 0 <= r <= 1\n", argv[0], r);
    exit(1);
  }

  orig = copyimage(image);

  if (depth(image) == 1)
  {
    if (! lfiltreordre(image, elem, x, y, 1.0 - r))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
    if (! ldilateros_ldilat(image, elem, x, y))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
    (void)lmin(image, orig);
  }
  else
  {
    if (! lfiltreordre3d(image, elem, x, y, z, 1.0 - r))
    {
      fprintf(stderr, "%s: function lfiltreordre3d failed\n", argv[0]);
      exit(1);
    }
    if (! ldilat3d(image, elem, x, y, z))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
    (void)lmin(image, orig);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(orig);

  return 0;
} /* main */
