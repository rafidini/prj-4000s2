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
/*! \file amont.c

\brief amont of a binary marker in a grayscale image

<B>Usage:</B> amont in.pgm mark.pgm connex [s] out.pgm

<B>Description:</B>
Let F be the function defined by \b in.pgm, 
let X be the set defined by \b mark.pgm.
The amont of X in F is defined as the set of points x such that there exists
an ascending path from X to x for F with connectivity <B>connex</B>.
If the parameter <B>s</B> is given, then only strictly ascending paths are 
considered.

<B>Types supported:</B> byte 2d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeodesic.h>
#include <lwshedtopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * marker;
  int32_t connex;
  int32_t strict = 0;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm mark.pgm connex [s] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && strcmp(argv[4],"s"))
  {
    fprintf(stderr, "usage: %s filein.pgm mark.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }
  if (argc == 6) strict = 1;

  image = readimage(argv[1]);
  marker = readimage(argv[2]);
  if ((image == NULL) || (marker == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  
  if (! lamont(marker, image, connex, strict))
  {
    fprintf(stderr, "%s: lamont failed\n", argv[0]);
    exit(1);
  }

  writeimage(marker, argv[argc - 1]);
  freeimage(image);
  freeimage(marker);

  return 0;
} /* main */



