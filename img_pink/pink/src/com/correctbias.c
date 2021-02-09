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
/*! \file correctbias.c

\brief correction of a luminosity bias expressed by a linear function

<B>Usage:</B> correctbias in.pgm factor type [xc yc] out.pgm

<B>Description:</B>
Let \b a be the value of parameter \b factor.
The bias \b type can be:
\li 0 radial (center \b xc, \b yc), linear (add -ax to the value of the element of abcissa x).
\li 1 radial (center \b xc, \b yc), parabolic (add -a^2 x to the value of the element of abcissa x).

<B>Types supported:</B> byte 2D

<B>Category:</B> arith
\ingroup arith

\author Michel Couprie
*/

/* 
  Michel Couprie - juin 2009
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  char type;
  double xc, yc, a, T, R;
  int32_t rs, cs, N, x, y;
  uint8_t *I;

  if ((argc != 5) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm a type [xc yc] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  I = UCHARDATA(image);
  rs = rowsize(image);
  cs = colsize(image);
  N = rs * cs;

  a = atof(argv[2]);
  type = argv[3][0];
  if (argc == 7)
  {
    xc = atof(argv[4]);
    yc = atof(argv[5]);
  }

  if (type == '0')
  {
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++)
    {
      R = sqrt((xc-x)*(xc-x) + (yc-y)*(yc-y));
      T = (double)(I[y*rs + x]) - (a * R);
      if (T > 255) T = 255; 
      if (T < 0) T = 0; 
      I[y*rs + x] = arrondi(T);
    }
  } 
  else if (type == '1')
  {
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++)
    {
      R = sqrt((xc-x)*(xc-x) + (yc-y)*(yc-y));
      T = (double)(I[y*rs + x]) - (a * R * R);
      if (T > 255) T = 255; 
      if (T < 0) T = 0; 
      I[y*rs + x] = arrondi(T);
    }
  } 
  else
  {
    fprintf(stderr, "%s: bad type: '%c'\n", argv[0], type);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);


  return 0;
}

