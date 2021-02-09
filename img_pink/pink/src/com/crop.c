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
/*! \file crop.c

\brief extracts a rectangular (cuboidal in 3d) area from an image

<B>Usage:</B> crop in.pgm [coords.list | x y [z] w h [d]] out.pgm

<B>Description:</B>
For a 2d image, extracts the rectangle with upper left corner (<B>x</B>,<B>y</B>),
of width <B>w</B> and height <B>h</B> from the image in.pgm .
For a 3d image, extracts the cuboid with upper left corner (<B>x</B>,<B>y</B>, <B>z</B>),
of width <B>w</B>, height <B>h</B> and depth <B>d</B> from the image in.pgm .

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.

<B>Category:</B> convert, geo
\ingroup  convert, geo

\author Michel Couprie
*/

/* 
   Michel Couprie - juillet 1996 

   update aout 2000 : int32_t et 3d
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcrop.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * out;
  FILE *fd = NULL;
  index_t x, y, z, w, h, d;
  int n;

  if ((argc != 4) && (argc != 7) && (argc != 9))
  {
    fprintf(stderr, "usage: %s in.pgm [coords.list | x y [z] w h [d]] out.pgm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ((argc == 4)) // coords in file
  {
    fd = fopen(argv[2],"r");
    if (!fd)
    { 
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }
    n = fscanf(fd, "%d %d %d %d %d %d\n", &x, &y, &z, &w, &h, &d);
    if (n == 4)
      out = lcrop(in, x, y, z, w);
    else
      out = lcrop3d(in, x, y, z, w, h, d);

    fclose(fd);
  }
  else if ((argc == 7)) /* 2D case */
  {
    x = atoi(argv[2]);
    y = atoi(argv[3]);
    w = atoi(argv[4]);
    h = atoi(argv[5]);

    out = lcrop(in, x, y, w, h);
  }
  else  /* 3D case */
  {
    x = atoi(argv[2]);
    y = atoi(argv[3]);
    z = atoi(argv[4]);
    w = atoi(argv[5]);
    h = atoi(argv[6]);
    d = atoi(argv[7]);
    out = lcrop3d(in, x, y, z, w, h, d);
  }

  writeimage(out, argv[argc-1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
