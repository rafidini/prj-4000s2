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
/*! \file hitormiss.c

\brief morphological hit or miss transformation for binay images

<B>Usage:</B> hitormiss in.pgm e1.pgm e2.pgm out.pgm

<B>Description:</B>
Let X be the input set, E1, E2 be the two structuring elements, the result Y is given by:<br>
Y = (X - E1) inter (Xb - E2)<br>
where '-' denotes the erosion operator, and 'Xb' denotes the complementary of X.  

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, long3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2003

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>
#include <ldilateros3d.h>
#include <larith.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * tmp;
  struct xvimage * elem1;
  struct xvimage * elem2;
  index_t x1, y1, z1;
  index_t x2, y2, z2;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s f.pgm e1.pgm e2.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem1 = readse(argv[2], &x1, &y1, &z1);
  elem2 = readse(argv[3], &x2, &y2, &z2);
  if ((image == NULL) || (elem1 == NULL) || (elem2 == NULL))
  {
    fprintf(stderr, "%s: readimage or readse failed\n", argv[0]);
    exit(1);
  }

  tmp = copyimage(image);
  linverse(tmp);
  if (depth(image) == 1)
  {
    if (! ldilateros_leros(image, elem1, x1, y1))
    {
      fprintf(stderr, "%s: function leros failed\n", argv[0]);
      exit(1);
    }
    if (! ldilateros_leros(tmp, elem2, x2, y2))
    {
      fprintf(stderr, "%s: function leros failed\n", argv[0]);
      exit(1);
    }
    lmin(image,tmp);
  }
  else
  {
    if (! leros3d(image, elem1, x1, y1, z1))
    {
      fprintf(stderr, "%s: function leros3d failed\n", argv[0]);
      exit(1);
    }
    if (! leros3d(tmp, elem2, x2, y2, z2))
    {
      fprintf(stderr, "%s: function leros3d failed\n", argv[0]);
      exit(1);
    }
    lmin(image,tmp);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(elem1);
  freeimage(elem2);
  freeimage(tmp);

  return 0;
} /* main */
