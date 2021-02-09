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
/*! \file holeclosing.c

\brief 3d hole closing

<B>Usage:</B> holeclosing in connex holesize [guide] out

<B>Description:</B> 
Hole closing in 3d binary images. 
The parameter \b connex gives the connectivity used for the object;
possible choices are 6 and 26.
Holes which have a "size" greater (strictly) than \b holesize are let open 
(where -1 is used as a symbol for infinity).

Let X be the set of points of the binary image \b in, let Y be a full enclosing box. 
The algorithm is the following:

\verbatim
Repeat until stability:
    Select a point p of Y \ X such that Tb(p,Y) = 1
        or such that Tb(p,Y) = 2 and d(p,X) > holesize
        which is at the greatest distance from X
    Y := Y \ {p}
Result: Y
\endverbatim

If the optional parameter \b guide is given, then replace 'greatest distance from X' by 
'greatest distance from Y' in the algorithm, Y being the set of non-null points of image \b guide.

\warning There must be no object point on the border of the image (a test is done).

Reference: 
Z. Aktouf, G. Bertrand, L.Perroton: 
"A three-dimensional holes closing algorithm",
<I>Pattern Recognition Letters</I>, No.23, pp.523-531, 2002.

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/
/* Operateur de fermeture de trous en 3d */
/* Michel Couprie - decembre 1999 */

/*
%TEST holeclosing %IMAGES/3dbyte/binary/b3tores.pgm 26 -1 %RESULTS/holeclosing_b3tores_26__1.pgm
%TEST holeclosing %IMAGES/3dbyte/binary/b3tores.pgm 26 2 %RESULTS/holeclosing_b3tores_26_2.pgm
%TEST holeclosing %IMAGES/3dbyte/binary/b3tores.pgm 6 -1 %RESULTS/holeclosing_b3tores_6__1.pgm
%TEST holeclosing %IMAGES/3dbyte/binary/b3tores.pgm 6 2 %RESULTS/holeclosing_b3tores_6_2.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfermetrous3d.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * image;      /* image (binaire) de depart */
  struct xvimage * guide;      /* image (long) */
  int32_t connex;
  int32_t tailletrous;
  
  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm connex holesize [guide] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  connex = atoi(argv[2]);
  tailletrous = atoi(argv[3]);

  if (argc == 6)
  {
    guide = readimage(argv[4]);  
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
	
    if(rowsize(image)!=rowsize(guide) || colsize(image)!=colsize(guide) || 
       depth(image)!=depth(guide))
      {
  	fprintf(stderr, "%s: different size of input image and guide image\n", argv[0]);
  	exit(1);
    }

    if (! lfermetrous3dbin2(image, guide, connex, tailletrous))
    {
      fprintf(stderr, "%s: function lfermetrous3dbin2 failed\n", argv[0]);
      exit(1);
    }
    freeimage(guide);
  }
  else 
  {
    if (! lfermetrous3dbin(image, connex, tailletrous))
    {
      fprintf(stderr, "%s: function lfermetrous3dbin failed\n", argv[0]);
      exit(1);
    }
  }
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
