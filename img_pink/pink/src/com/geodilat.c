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
/*! \file geodilat.c

\brief geodesic (grayscale or binary) dilation

<B>Usage:</B> geodilat g.pgm f.pgm connex niter [cond] out.pgm

<B>Description:</B>
Geodesic dilation of <B>g.pgm</B> under <B>f.pgm</B>.
Let G and F be the two input images. If G is not under F, then G is replaced initially by min(G,F).
The structuring element is specified by the value of the parameter <B>connex</B>, 
which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.
The parameter \b niter sets the number of iterations. If \b niter = -1,
then the iterations continue until stability.
If given, the parameter \b cond is a binary image (region of interest) that restricts the action of the operator.

<B>Types supported:</B> byte 2d, byte 3d, short 2d, short 3d, long 2d, long 3d.

<B>Category:</B> connect, morpho
\ingroup  connect, morpho

\author Michel Couprie
*/

/* Michel Couprie - juillet 1996 */

/*
%TEST geodilat %IMAGES/2dbyte/binary/b2hebreu_er.pgm %IMAGES/2dbyte/binary/b2hebreu.pgm 4 -1 %RESULTS/geodilat_b2hebreu_er_b2hebreu_4__1.pgm
%TEST geodilat %IMAGES/2dbyte/binary/b2hebreu_er.pgm %IMAGES/2dbyte/binary/b2hebreu.pgm 8 -1 %RESULTS/geodilat_b2hebreu_er_b2hebreu_8__1.pgm
%TEST geodilat %IMAGES/3dbyte/binary/b3mortier_er.pgm %IMAGES/3dbyte/binary/b3mortier.pgm 6 -1 %RESULTS/geodilat_b3mortier_er_b3mortier_6__1.pgm
%TEST geodilat %IMAGES/3dbyte/binary/b3mortier_er.pgm %IMAGES/3dbyte/binary/b3mortier.pgm 26 -1 %RESULTS/geodilat_b3mortier_er_b3mortier_26__1.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeodesic.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  struct xvimage * image3;
  int32_t connex;
  int32_t niter;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s g.pgm f.pgm connex niter [cond] out.pgm \n", argv[0]);
    fprintf(stderr, "       connex = [4|8|6|18|26]\n");
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  connex = atoi(argv[3]);
  if ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26))
  {
    fprintf(stderr, "%s: bad connexity - use one of the following:\n", argv[0]);
    fprintf(stderr, "4, 8 (in 2d), 6, 18, 26 (in 3d)\n");
    exit(1);
  }

  niter = atoi(argv[4]);

  if (argc == 6)
  {
    if (! lgeodilat(image1, image2, connex, niter))
    {
      fprintf(stderr, "%s: function lgeodilat failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    image3 = readimage(argv[5]);
    if (image3 == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    if (! lgeodilatcond(image1, image2, image3, connex, niter))
    {
      fprintf(stderr, "%s: function lgeodilatcond failed\n", argv[0]);
      exit(1);
    }
    freeimage(image3);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */
