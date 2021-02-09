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
/* \file concat3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>concat3d</B> - constructs a 3d image by catenating two images

SYNOPSIS

<B>concat3d</B> in1.pgm in2.pgm out.pgm

DESCRIPTION

The first planes of out.pgm are those of in1.pgm, 
the last planes of out.pgm are those of in2.pgm.
The images in1.pgm and in2.pgm must have the same rowsize
and colsize.

Types supported: byte 2d, byte 3d.

CLASS 

gene

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  struct xvimage * image;
  int32_t i, n, N1, N2, rs, cs, d1, d2;
  uint8_t *img1;
  uint8_t *img2;
  uint8_t *img;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm fileout.pgm\n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL) 
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  image2 = readimage(argv[2]);
  if (image2 == NULL) 
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image1);
  cs = colsize(image1);
  if ((rowsize(image2) != rs) || (colsize(image2) != cs))
  {
    fprintf(stderr, "%s, %s: incompatible sizes\n", argv[1], argv[2]);
    exit(1);
  }
  d1 = depth(image1);
  d2 = depth(image2);
  img1 = UCHARDATA(image1);
  img2 = UCHARDATA(image2);
  n = rs * cs;
  N1 = d1 * n;
  N2 = d2 * n;

  image = allocimage(NULL, rs, cs, d1+d2, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  img = UCHARDATA(image);

  for (i = 0; i < N1; i++) img[i] = img1[i];    
  for (i = 0; i < N2; i++) img[N1+i] = img2[i];    

  image->name = NULL;
  writeimage(image, argv[3]);
  freeimage(image1);
  freeimage(image2);
  freeimage(image);

  return 0;
} /* main */



