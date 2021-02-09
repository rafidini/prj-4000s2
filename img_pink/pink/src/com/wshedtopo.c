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
/*! \file wshedtopo.c

\brief topological grayscale watershed  (Couprie-Bertrand algorithm)

<B>Usage:</B> wshedtopo in.pgm connex [i] out.pgm

<B>Description:</B>
Topological grayscale watershed as defined in [CB97], preserves the minima - connectivity <B>connex</B>
If the parameter <B>i</B> is given, then the dual operator is applied.

[CB97] M. Couprie and G. Bertrand:
``Topological Grayscale Watershed Transformation'',
SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie

<B>Example:</B>

wshedtopo ur1 4 ur1_wshedtopo

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/ur1.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/ur1_wshedtopo.gif"></td>
  </tr>
  <tr><td align="center">ur1</td>
      <td align="center">ur1_wshedtopo</td>
  </tr>
</table>
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lwshedtopo.h>

/* ==================================== */
static void inverse(struct xvimage * image)
/* ==================================== */
{
  int32_t i, N = rowsize(image) * colsize(image) * depth(image);
  uint8_t *pt;
  for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
    *pt = NDG_MAX - *pt;
} // inverse

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 5) && strcmp(argv[3],"i"))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  
  if (argc == 4) inverse(image);
  if (! lwshedtopo_lwshedtopo(image, connex))
  {
    fprintf(stderr, "%s: lwshedtopo_lwshedtopo failed\n", argv[0]);
    exit(1);
  }
  if (argc == 4) inverse(image);

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



