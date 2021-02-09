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
/*! \file normalize.c

\brief normalization of grayscale values

<B>Usage:</B> normalize in.pgm [[nmin nmax] out.pgm]

<B>Description:</B>
Grayscale of \b in.pgm values are normalized to span the range of [nmin...nmax].
The parameters \b nmin and \b nmax are optional.
For byte and int32_t images, the default values are \b nmin = 0 and \b nmax = 255.
For float images, the default values are \b nmin = 0 and \b nmax = 1.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> arith
\ingroup  arith

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
  float nmin, nmax;

  if ((argc != 2) && (argc != 3) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm [[nmin nmax] out.pgm] \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    nmin = (float)(atof(argv[2]));
    nmax = (float)(atof(argv[3]));
  }
  else
  {
    nmin = (float)0;
    if (datatype(image1) == VFF_TYP_FLOAT)
      nmax = (float)1;
    else
      nmax = (float)255;
  }

  if (! lnormalize(image1, nmin, nmax))
  {
    fprintf(stderr, "%s: function lnormalize failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */
