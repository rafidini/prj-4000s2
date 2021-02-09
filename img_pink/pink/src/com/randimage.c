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
/*! \file randimage.c

\brief random image generation

<B>Usage:</B> randimage [in.pgm | rowsize colsize depth] valmax [seed] out.pgm

<B>Description:</B> Generates an image with pixel values uniformly randomly
distributed between 0 and \b valmax (included). 
If given, the parameter \b seed is used to 
initialize the random generator, otherwise the clock is used instead.

<B>Types supported:</B> byte 2D, byte 3D, int32_t 2D, int32_t 3D

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie
*/
/* genere une image aleatoire - bruit blanc */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t i;
  int32_t rs, cs, ds, N, valmax;

  if ((argc < 4) || (argc > 7))
  {
    fprintf(stderr, "usage: %s [in.pgm | rowsize colsize depth] valmax [seed] out.pgm \n", argv[0]);
    exit(1);
  }

  if ((argc == 4) || (argc == 5))
  {
    image = readheader(argv[1]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    rs = rowsize(image);
    cs = colsize(image);
    ds = depth(image);
    valmax = atoi(argv[2]);
    if (argc == 5) srand(atoi(argv[3])); else srand(time(NULL));
    freeimage(image);
  }
  else // 6 or 7
  {
    rs = atoi(argv[1]);
    cs = atoi(argv[2]);
    ds = atoi(argv[3]);
    valmax = atoi(argv[4]);
    if (argc == 7) srand(atoi(argv[5])); else srand(time(NULL));
  }

  if (valmax <= 255)
  {
    uint8_t *Im;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }

    Im = UCHARDATA(image);
    N = rs * cs * ds;
    for (i = 0; i < N; i++)
    {
      Im[i] = (int32_t) ((valmax + 1.0)*rand() / (RAND_MAX + 1.0));
    }
  }
  else
  {
    int32_t *Im;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }

    Im = SLONGDATA(image);
    N = rs * cs * ds;
    for (i = 0; i < N; i++)
    {
      Im[i] = (int32_t) ((valmax + 1.0)*rand() / (RAND_MAX + 1.0));
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
