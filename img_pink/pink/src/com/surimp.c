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
/*! \file surimp.c

\brief superimposition of a binary image with a grayscale image

<B>Usage:</B> surimp i1.ndg i2.bin out.ppm

<B>Description:</B> Superimposes the binary image <b>i2.bin</b> (in red) to the 
grayscale image <b>i1.bin</b>. The result is stored as a color image <b>out.ppm</b>.

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  struct xvimage * tmp;
  struct xvimage * tmp3;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s i1.ndg i2.bin out.ppm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {   fprintf(stderr,"%s: readimage failed\n", argv[0]);
      exit(1);
  }

  tmp = copyimage(image2);
  linverse(tmp);
  tmp3 = copyimage(image1);
  lmin(tmp3,tmp);
  copy2image(tmp,image2);
  ladd(tmp,tmp3);
  writergbimage(tmp, tmp3, tmp3, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);
  freeimage(tmp);
  freeimage(tmp3);

  return 0;
} /* main */
