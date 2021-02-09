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
/*! \file redt.c

\brief reverse euclidean distance transform

<B>Usage:</B> redt in.pgm out.pgm

<B>Description:</B>
Reverse euclidean distance transform, by the Coeurjolly's algorithm [1].
The input \b in.pgm must be a quadratic euclidean distance map, or 
an image of type int32_t where each non-null value correspond to the quadratic
radius of a disc or ball. The output is a binary image.

[1] D. Coeurjolly,
"d-Dimensional Reverse Euclidean Distance Transformation
and Euclidean Medial Axis Extraction in Optimal Time", 
DGCI 2003, LNCS 2886, pp. 327-337, 2003.

<B>Types supported:</B> int32_t 2d, int32_t 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2004, Jean Cousty 2005 (3d)
*/

/*
%TEST redt %IMAGES/2dlong/l2fish1distc3.pgm %RESULTS/redt_l2fish1distc3.pgm
%TEST redt %IMAGES/2dlong/l2fish1distc3subset.pgm %RESULTS/redt_l2fish1distc3subset.pgm
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

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);

  if (depth(image) == 1) // 2D
  {
    if (!convertlong(&image))
    {
      fprintf(stderr, "%s: function convertlong failed\n", argv[0]);
      exit(1);
    }
    res = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
    if (res == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    if (!lredt2d(image, res))
    {
      fprintf(stderr, "%s: function lredt2d failed\n", argv[0]);
      exit(1);
    }
  }
  else // 3D
  {
    if (!convertlong(&image))
    {
      fprintf(stderr, "%s: function convertlong failed\n", argv[0]);
      exit(1);
    }
    res = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
    if (res == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    if (!lredt3d(image, res))
    {
      fprintf(stderr, "%s: function lredt2d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(res, argv[argc-1]);
  freeimage(res);
  freeimage(image);
  return 0;
} /* main */
