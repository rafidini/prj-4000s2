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
/*! \file medianfilter.c

\brief median filter 

<B>Usage:</B> medianfilter in.pgm el.pgm out.pgm

<B>Description:</B>
Let F be the input image, G be the output image, and E the structuring
element. 
then for each pixel p, G[p] is the median element of the sorted list (by 
increasing order) of the pixel values in the set { F[q], q in E[p] }.

The median filter is a particular case of rank filter (see rankfilter.c), 
with rank = (N+1)/2, where N = |E| .

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup morpho

\author Michel Couprie 1997
*/
/* Michel Couprie - decembre 1997 */

/*
%TEST medianfilter %IMAGES/2dbyte/gray/g2gel.pgm %IMAGES/2dbyte/binary/b2_se_5_7.pgm %RESULTS/medianfilter_g2gel_b2_se_5_7.pgm
%TEST medianfilter %IMAGES/3dbyte/gray/g3a.pgm %IMAGES/3dbyte/binary/b3_se_5_5_7.pgm %RESULTS/medianfilter_g3a_b3_se_5_5_7.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltreordre.h>
#include <larith.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * elem;
  index_t x, y, z;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm el.pgm out.pgm \n", argv[0]);
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

  int N = larea(elem);

  if (depth(image) == 1)
  {
    if (! lfiltrerang(image, elem, x, y, (N+1)/2))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lfiltreordre3d(image, elem, x, y, z, 0.5))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
