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
/*! \file seamcarving.c

\brief applies the seam carving method to shrink an image

<B>Usage:</B> seamcarving in.ppm energy.pgm w h out.ppm

<B>Description:</B>

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* 
   Michel Couprie - juillet 2012
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseamcarving.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * en;
  struct xvimage * out;
  index_t w, h;
  int32_t ret;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in.ppm energy.pgm w h out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  en = readimage(argv[2]);
  if (en == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  w = atoi(argv[3]);
  h = atoi(argv[4]);
  if (h != colsize(in))
  {
    h = colsize(in);
    printf("WARNING: change of h not yet implemented, h forced to %d\n", h);
  }
  if (w >= rowsize(in))
  {
    printf("WARNING: increase of w not yet implemented, nothing done\n");
    return 0;
  }

  out = allocmultimage(NULL, w, h, 1, 1, 3, VFF_TYP_1_BYTE);
  if (out == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  ret = lseamcarving(in, en, w, out);

  writeimage(out, argv[argc-1]);
  freeimage(in);
  freeimage(en);
  freeimage(out);

  return 0;
} /* main */
