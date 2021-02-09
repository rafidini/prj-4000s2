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
/*! \file ptjunction.c

\brief detects junction points in 2d or 3d binary images

<B>Usage:</B> ptjunction in.pgm connex out.pgm

<B>Description:</B>
Detects junction points in the 2d or 3d binary image \b in.pgm, which is supposed to contain a skeleton.
A junction point is a white point x such that #(Nn[x] inter X) > 2, where Nn[x] stands for the n-neighborhood of x (excluding x), and n = 4, 8 in 2D or n = 6, 18, 26 in 3D, as set by the parameter \b connex.

When the type of \b in.pgm is 4_BYTE, the image is treated as a label image, where each label is processed as a separate binary image (all other labels are considered as background).

<B>Types supported:</B> byte 2D, byte 3D, long 3D

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2010
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseltopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    struct xvimage * res = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
    if (res == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    if (! lptjunctionlab(image, connex, res))
    {
      fprintf(stderr, "%s: function lptjunctionlab failed\n", argv[0]);
      exit(1);
    }
    writeimage(res, argv[argc-1]);
    freeimage(res);
  }
  else 
  {
    if (! lptjunction(image, connex))
    {
      fprintf(stderr, "%s: function lptjunction failed\n", argv[0]);
      exit(1);
    }
    writeimage(image, argv[argc-1]);
  }

  freeimage(image);

  return 0;
} /* main */
