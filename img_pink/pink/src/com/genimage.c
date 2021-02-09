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
/*! \file genimage.c

\brief generates a monocolor image of given size

<B>Usage:</B> genimage {in.pgm|rs cs ds} col out.pgm

<B>Description:</B>
The size of the result image <B>out.pgm</B> is taken from image
<B>in.pgm</B>, or given by the parameters <B>rs</B>, <B>cs</B>, <B>ds</B>. 
For a 2D image, the parameter <B>ds</B> (depth size) is set to 1.
The color of the result image is given by the parameter <B>col</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> arith, geo
\ingroup  arith, geo

\author Michel Couprie
*/

/* 
   Michel Couprie - mai 1998 
   revision decembre 1999 : 3D
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * header;
  struct xvimage * image;
  uint8_t *Im;
  int32_t rs, cs, ds, N;
  uint8_t col;
  double xdim, ydim, zdim;

  if ((argc != 4) && (argc != 6))
  {
    fprintf(stderr, "usage: %s {in.pgm|rowsize colsize depth} color out.pgm \n", argv[0]);
    exit(1);
  }

  if (argc == 6)
  {
    rs = atoi(argv[1]);
    cs = atoi(argv[2]);
    ds = atoi(argv[3]);
    col = (uint8_t)atoi(argv[4]);
    xdim = ydim = zdim = 1.0;
  }
  else
  {
    header = readheader(argv[1]);
    rs = rowsize(header);
    cs = colsize(header);
    ds = depth(header);
    col = (uint8_t)atoi(argv[2]);
    xdim = header->xdim;
    ydim = header->ydim;
    zdim = header->zdim;
  }
  image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  Im = UCHARDATA(image);
  N = rs * cs * ds;
  memset(Im, col, N);
  image->xdim = xdim;
  image->ydim = ydim;
  image->zdim = zdim;

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


