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
/*! \file filtrerang.c

\brief rank filter 

<B>Usage:</B> filtrerang in.pgm el.pgm R out.pgm

<B>Description:</B>
Let F be the input image, G be the output image, and E the structuring
element. 
For each pixel p, G[p] is the Rth element of the sorted list (by 
increasing order) of the pixel values in the set { F[q], q in E[p] }.

Let n be the number of elements of E,
particular cases are the median filter (R = n/2), the erosion (R = 0),
and the dilation (R = n).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup morpho

\author Michel Couprie
*/
/* filtre d'ordre sur un voisinage quelconque */
/* Michel Couprie - decembre 1997 */

/*
%TEST filtrerang %IMAGES/2dbyte/gray/g2gel.pgm %IMAGES/2dbyte/binary/b2_se_5_7.pgm 0.8 %RESULTS/filtrerang_g2gel_b2_se_5_7.pgm
%TEST filtrerang %IMAGES/3dbyte/gray/g3a.pgm %IMAGES/3dbyte/binary/b3_se_5_5_7.pgm 0.8 %RESULTS/filtrerang_g3a_b3_se_5_5_7.pgm
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
  int r;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm el.pgm R out.pgm \n", argv[0]);
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
  
  r = atoi(argv[3]);

  if (depth(image) == 1)
  {
    if (! lfiltrerang(image, elem, x, y, r))
    {
      fprintf(stderr, "%s: function lfiltrerang failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "%s: function lfiltrerang3d not implemented\n", argv[0]);
    /*    if (! lfiltrerang3d(image, elem, x, y, z, r))
    {
      fprintf(stderr, "%s: function lfiltrerang3d failed\n", argv[0]);
      exit(1);
    }
    */
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
