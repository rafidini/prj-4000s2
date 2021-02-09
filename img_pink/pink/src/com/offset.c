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
/*! \file offset.c

\brief image translation 

<B>Usage:</B> in.pgm ox oy oz [mode] out.ppm

<B>Description:</B>
Translates the image by the offsets \b ox, \b oy, \b oz in the directions
x, y, z respectively. Offsets may be negative or positive integers.
If \b mode is 0, the points that get out of the image frame are lost. 
If \b mode is 1, a torical space is simulated in all dimensions. 
Default mode is 0.

<B>Types supported:</B> byte 2d, byte3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <loffset.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t ox, oy, oz, mode = 0;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm ox oy oz [mode] out.ppm \n", argv[0]);
    fprintf(stderr, "       mode = 0 (loss) or 1 (torical) \n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  ox = atoi(argv[2]);
  oy = atoi(argv[3]);
  oz = atoi(argv[4]);
  if (argc == 7) mode = atoi(argv[5]);

  if (! loffset(image, ox, oy, oz, mode))
  {
    fprintf(stderr, "%s: function loffset failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
