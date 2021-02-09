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
/*! \file rankfilter.c

\brief rank filter 

<B>Usage:</B> rankfilter in.pgm el.pgm r out.pgm

<B>Description:</B>
Let F be the input image, G be the output image, and E the structuring
element. Let n be the number of elements of E, and R be the product n.r,
then for each pixel p, G[p] is the Rth element of the sorted list (by 
increasing order) of the pixel values in the set { F[q], q in E[p] }.

Particular cases are the median filter (r = 0.5), the erosion (r = 0),
and the dilation (r = 1).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup morpho

\author Michel Couprie
*/
/* filtre d'ordre sur un voisinage quelconque */
/* Michel Couprie - decembre 1997 */

/*
%TEST rankfilter %IMAGES/2dbyte/gray/g2gel.pgm %IMAGES/2dbyte/binary/b2_se_5_7.pgm 0.8 %RESULTS/rankfilter_g2gel_b2_se_5_7.pgm
%TEST rankfilter %IMAGES/3dbyte/gray/g3a.pgm %IMAGES/3dbyte/binary/b3_se_5_5_7.pgm 0.8 %RESULTS/rankfilter_g3a_b3_se_5_5_7.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltreordre.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
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
  if (depth(image) == 1)
  {
    if (! lfiltreordre(image, elem, x, y, r))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lfiltreordre3d(image, elem, x, y, z, r))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
