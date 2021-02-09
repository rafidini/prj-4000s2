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
/*! \file selectcomp.c

\brief selects a connected component

<B>Usage:</B> selectcomp in.pgm connex x y z out.pgm

<B>Description:</B>
The connected component of the binary image <B>in.pgm</B>
(according to the connectivity <B>connex</B>)
which contains the point (<B>x</B>,<B>y</B>,<B>z</B>)
is preserved. All other points are deleted.
Possible values for \b connex are 4, 8 (2D), 6, 18, 26, 60, 260 (3D).
Values 60 and 260 correspond to restrictions of 6 and 26 connectivities to the 
current xy plane.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
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
  struct xvimage * image;
  int32_t x, y, z, connex;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm connex x y z out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "selectcomp: readimage failed\n");
    exit(1);
  }
  connex = atoi(argv[2]);
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  z = atoi(argv[5]);
  
  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    if (! lselectcomp(image, connex, x, y, z))
    {
      fprintf(stderr, "selectcomp: function lselectcomp failed\n");
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "selectcomp: bad data type\n");
    exit(1);
  }
  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */


