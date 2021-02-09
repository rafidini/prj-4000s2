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
/*! \file erosion.c

\brief morphological erosion by a plane structuring element

<B>Usage:</B> erosion in.pgm se.pgm out.pgm

<B>Description:</B>
The (plane) structuring element is given by the non-null values in \b se.pgm, 
its origin (wrt the point (0,0) of \b se.pgm ) is given by a comment line in the file <B>se.pgm</B>. 

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2002

<B>Example:</B>

erosion cells ~/Pink/masks/carre5.pgm cells_eros

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_eros.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_eros</td>
  </tr>
</table>

*/

/*
%TEST erosion %IMAGES/2dbyte/binary/b2hebreu.pgm %IMAGES/2dbyte/binary/b2_se_5_7.pgm %RESULTS/erosion_b2hebreu_b2_se_5_7.pgm
%TEST erosion %IMAGES/2dbyte/gray/g2fish1.pgm %IMAGES/2dbyte/binary/b2_se_5_7.pgm %RESULTS/erosion_g2fish1_b2_se_5_7.pgm
%TEST erosion %IMAGES/3dbyte/binary/b3a.pgm %IMAGES/3dbyte/binary/b3_se_5_5_7.pgm %RESULTS/erosion_b3a_b3_se_5_5_7.pgm
%TEST erosion %IMAGES/3dbyte/gray/g3a.pgm %IMAGES/3dbyte/binary/b3_se_5_5_7.pgm %RESULTS/erosion_g3a_b3_se_5_5_7.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>
#include <ldilateros3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * elem;
  index_t x, y, z;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s f.pgm el.pgm out.pgm \n", argv[0]);
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

  if (depth(image) == 1)
  {
    if (! ldilateros_leros(image, elem, x, y))
    {
      fprintf(stderr, "%s: function leros failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! leros3d(image, elem, x, y, z))
    {
      fprintf(stderr, "%s: function leros3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(elem);

  return 0;
} /* main */
