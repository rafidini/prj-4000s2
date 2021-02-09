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
/*! \file closeball.c

\brief morphological binary closing by a ball

<B>Usage:</B> closeball in.pgm r [dist] out.pgm

<B>Description:</B>
The closing consists in a dilation (dilatball) followed by an erosion (erosball).
The structuring element for this closing is a ball (or a disc) of radius \b r.
The erosion and dilation are computed by thresholding a distance map.
The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: rounded euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

\warning The input image \b in.pgm must be a binary image. No test is done.

<B>Types supported:</B> binary byte 2D, byte 3D

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2002

<B>Example:</B>

closeball cells 3 cells_closeball

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_closeball.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_closeball</td>
  </tr>
</table>

*/

/*
%TEST closeball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 0 %RESULTS/closeball_b2hebreu_3_0.pgm
%TEST closeball %IMAGES/2dbyte/binary/b2hebreu.pgm 15 2 %RESULTS/closeball_b2hebreu_15_2.pgm
%TEST closeball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 3 %RESULTS/closeball_b2hebreu_3_3.pgm
%TEST closeball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 4 %RESULTS/closeball_b2hebreu_3_4.pgm
%TEST closeball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 8 %RESULTS/closeball_b2hebreu_3_8.pgm
%TEST closeball %IMAGES/3dbyte/binary/b3a.pgm 3 0 %RESULTS/closeball_b3a_3_0.pgm
%TEST closeball %IMAGES/3dbyte/binary/b3a.pgm 15 2 %RESULTS/closeball_b3a_15_2.pgm
%TEST closeball %IMAGES/3dbyte/binary/b3a.pgm 3 3 %RESULTS/closeball_b3a_3_3.pgm
%TEST closeball %IMAGES/3dbyte/binary/b3a.pgm 3 6 %RESULTS/closeball_b3a_3_6.pgm
%TEST closeball %IMAGES/3dbyte/binary/b3a.pgm 3 18 %RESULTS/closeball_b3a_3_18.pgm
%TEST closeball %IMAGES/3dbyte/binary/b3a.pgm 3 26 %RESULTS/closeball_b3a_3_26.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t r, mode;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s f.pgm r [dist] out.pgm \n", argv[0]);
    fprintf(stderr, "dist = [0|2|3|4|8|6|18|26]\n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  r = atoi(argv[2]);

  if (argc == 5) mode = atoi(argv[3]); else mode = 0;
  if ((mode != 0) && (mode != 2) && (mode != 3) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "%s: dist = [0|2|3|4|8|6|18|26] \n", argv[0]);
    exit(1);
  }

  if (! lcloseball(image, r, mode))
  {
    fprintf(stderr, "%s: function lcloseball failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
