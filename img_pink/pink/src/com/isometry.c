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
/*! \file isometry.c

\brief basic isometries in 3D 

<B>Usage:</B> isometry in MODE out

<B>Description:</B>
Basic isometries in 2D and 3D.
Available modes: xzy, yxz, yzx, zxy, zyx, (3D), and
0: identity,
1: rotation 90 degrees clockwise,
2: rotation 180 degrees clockwise,
3: rotation 270 degrees clockwise,
4: symmetry / vertical axis,
5: symmetry / horizontal axis.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/*
   Michel Couprie  -  Novembre 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lisometry.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in mode out \n", argv[0]);
    fprintf(stderr, "available modes: xzy, yxz, yzx, zxy, zyx, 0, 1, 2, 3, 4, 5\n");
    exit(1);
  }

  if ((strcmp(argv[2], "xzy") != 0) &&
      (strcmp(argv[2], "yxz") != 0) &&
      (strcmp(argv[2], "yzx") != 0) &&
      (strcmp(argv[2], "zxy") != 0) &&
      (strcmp(argv[2], "zyx") != 0) &&
      (strcmp(argv[2], "0") != 0) &&
      (strcmp(argv[2], "1") != 0) &&
      (strcmp(argv[2], "2") != 0) &&
      (strcmp(argv[2], "3") != 0) &&
      (strcmp(argv[2], "4") != 0) &&
      (strcmp(argv[2], "5") != 0)
     )
  {   
    fprintf(stderr, "%s() : bad mode : %s\n", argv[0], argv[2]);
    fprintf(stderr, "available modes: xzy, yxz, yzx, zxy, zyx, 0, 1, 2, 3, 4, 5\n");
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lisometry(image1, argv[2], &image2))
  {
    fprintf(stderr, "%s: function lisometry failed\n", argv[0]);
    exit(1);
  }

  writeimage(image2, argv[3]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


