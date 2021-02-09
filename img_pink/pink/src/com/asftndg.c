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
/*! \file asftndg.c

\brief alternate sequential filter controled by topology

<B>Usage:</B> asftndg in.pgm {c.pgm cc.pgm|null null} connex rmax out.pgm

<B>Description:</B>
Alternate sequential filter controled by topology for multilevel images.
Disc- or Ball-shaped structuring elements of increasing radius,
ranging from 1 to <B>rmax</B>,
are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.
Let I(0) = <B>in.pgm</B>, the ith intermediate result I(i) is obtained by the 
homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) by the structuring element Di. 
Two images \b c.pgm and \b cc.pgm can be given to serve as constraints. 
The image \b c.pgm must be 0 except for the points x that are to be preserved,
which can be set to 255 or to I[x].
The image \b cc.pgm must be 255 except for the points x that are to be preserved,
which can be set to 0 or to I[x].
The result <B>out.pgm</B> contains the final result I(n).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho, topogray
\ingroup  morpho, topogray

\author Michel Couprie
*/
/* M. Couprie -- juillet 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lasft.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagec = NULL;
  struct xvimage * imagecc = NULL;
  int32_t rayonmax;
  int32_t connex;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm {c.pgm cc.pgm|null null} connex rmax out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ((strcmp(argv[2],"null") != 0) && (strcmp(argv[3],"null") != 0))
  {
    imagec = readimage(argv[2]);
    imagecc = readimage(argv[3]);
    if ((imagec == NULL) || (imagecc == NULL))
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  connex = atoi(argv[4]);
  rayonmax = atoi(argv[5]);

  if (depth(image) == 1)
  {
    if (! lasft_ndg(image, imagec, imagecc, connex, rayonmax))
    {
      fprintf(stderr, "%s: lasft_ndg failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lasft_ndg3d(image, imagec, imagecc, connex, rayonmax))
    {
      fprintf(stderr, "%s: lasft_ndg3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  if (imagec) freeimage(imagec);
  if (imagecc) freeimage(imagecc);

  return 0;
} /* main */
