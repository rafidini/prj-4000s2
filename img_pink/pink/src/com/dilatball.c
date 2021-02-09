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
/*! \file dilatball.c

\brief morphological binary dilation by a ball

<B>Usage:</B> dilatball in.pgm r [dist] out.pgm

<B>Description:</B>
Morphological binary dilation by a metric ball.
If the parameter \b r is a positive integer, then
the structuring element for this dilation is a ball (or a disc) of radius \b r.
In this case the result of the dilation is defined by Y = {x; d(x,X) <= r} where X
represents the original point set (non-null pixels in the original image).
If the parameter \b r is -1, then 
the structuring element for each point x is a ball (or a disc) of radius \b F(x),
where F is the greyscale image \b in.pgm .

The dilation is computed by thresholding a distance map.
The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: rounded Euclidean distance
\li 1: approximate quadratic Euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic Euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

\warning Except for the case r=-1, the input image \b in.pgm must be a binary image. No test is done.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2002

<B>Example:</B>

dilatball cells 4 cells_dilatball

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_dilatball.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_dilatball</td>
  </tr>
</table>

*/

/*
%TEST dilatball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 0 %RESULTS/dilatball_b2hebreu_3_0.pgm
%TEST dilatball %IMAGES/2dbyte/binary/b2hebreu.pgm 15 2 %RESULTS/dilatball_b2hebreu_15_2.pgm
%TEST dilatball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 3 %RESULTS/dilatball_b2hebreu_3_3.pgm
%TEST dilatball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 4 %RESULTS/dilatball_b2hebreu_3_4.pgm
%TEST dilatball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 8 %RESULTS/dilatball_b2hebreu_3_8.pgm
%TEST dilatball %IMAGES/2dlong/l2fish1ma3.pgm -1 3 %RESULTS/dilatball_l2fish1ma3__1_3.pgm
%TEST dilatball %IMAGES/2dlong/l2fish1ma4.pgm -1 4 %RESULTS/dilatball_l2fish1ma4__1_4.pgm
%TEST dilatball %IMAGES/2dlong/l2fish1ma8.pgm -1 8 %RESULTS/dilatball_l2fish1ma8__1_8.pgm

%TEST dilatball %IMAGES/3dbyte/binary/b3a.pgm 3 0 %RESULTS/dilatball_b3a_3_0.pgm
%TEST dilatball %IMAGES/3dbyte/binary/b3a.pgm 15 2 %RESULTS/dilatball_b3a_15_2.pgm
%TEST dilatball %IMAGES/3dbyte/binary/b3a.pgm 3 3 %RESULTS/dilatball_b3a_3_3.pgm
%TEST dilatball %IMAGES/3dbyte/binary/b3a.pgm 3 6 %RESULTS/dilatball_b3a_3_6.pgm
%TEST dilatball %IMAGES/3dbyte/binary/b3a.pgm 3 18 %RESULTS/dilatball_b3a_3_18.pgm
%TEST dilatball %IMAGES/3dbyte/binary/b3a.pgm 3 26 %RESULTS/dilatball_b3a_3_26.pgm
%TEST dilatball %IMAGES/3dlong/l3mortierma3.pgm -1 3 %RESULTS/dilatball_l3mortierma3__1_3.pgm
%TEST dilatball %IMAGES/3dlong/l3mortierma6.pgm -1 6 %RESULTS/dilatball_l3mortierma6__1_6.pgm
%TEST dilatball %IMAGES/3dlong/l3mortierma26.pgm -1 26 %RESULTS/dilatball_l3mortierma26__1_26.pgm

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
  struct xvimage * res;
  int32_t r;
  int32_t mode;

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
  if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "%s: dist = [0|1|2|3|4|8|6|18|26] \n", argv[0]);
    exit(1);
  }

  if (r >= 0)
  {
    if (! ldilatball(image, r, mode))
    {
      fprintf(stderr, "%s: function ldilatball failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    res = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
    if (res == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    if (mode == 3)
    {
      if (!convertlong(&image))
      {
	fprintf(stderr, "%s: function convertlong failed\n", argv[0]);
	exit(1);
      }
      if (depth(image) == 1)
      {
	if (!lredt2d(image, res))
	{
	  fprintf(stderr, "%s: function lredt2d failed\n", argv[0]);
	  exit(1);
	}
      }
      else
      {
	if (!lredt3d(image, res))
	{
	  fprintf(stderr, "%s: function lredt3d failed\n", argv[0]);
	  exit(1);
	}
      }
    }
    else
    {
      if (!ldilatballloc(image, res, mode))
      {
	fprintf(stderr, "%s: function ldilatballloc failed\n", argv[0]);
	exit(1);
      }
    }
  }

  if (r >= 0) 
  { 
    writeimage(image, argv[argc-1]); 
  }
  else 
  { 
    writeimage(res, argv[argc-1]);
    freeimage(res);
  }
  freeimage(image);
  return 0;
} /* main */
