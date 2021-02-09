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
/*! \file enframe3d.c

\brief adds a border with a given gray value to an image

<B>Usage:</B> enframe3d in.pgm [grayvalue [width [out.pgm]]]

<B>Description:</B>
Adds a border to the input image, filled with the value \b grayvalue (default 0). 
The width of the border may be given as parameter \b width, 
otherwise its value is 1.

If \b out.pgm is not specified, then out.pgm = in.pgm.

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.

<B>Category:</B> convert, geo
\ingroup  convert, geo

\author Michel Couprie
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
  struct xvimage * image;
  struct xvimage * imageout;
  int32_t grayval;
  int32_t width;  

  if ((argc != 2) && (argc != 3) && (argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm [grayvalue [width [out.pgm]]]\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if (argc > 2) grayval = atoi(argv[2]); else grayval = 0;
  if (argc > 3) width = atoi(argv[3]); else width = 1;
  imageout = lenframe3d(image, grayval, width);
  if (imageout == NULL)
  {
    fprintf(stderr, "%s: lenframe3d failed\n", argv[0]);
    exit(1);
  }

  if (argc > 4)  
    writeimage(imageout, argv[argc-1]);
  else
    writeimage(imageout, argv[1]);
  freeimage(imageout);
  freeimage(image);

  return 0;
} /* main */



