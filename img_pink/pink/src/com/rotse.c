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
/*! \file rotse.c

\brief rotation of a structuring element

<B>Usage:</B> rotse in.pgm angle out.pgm

<B>Description:</B>
Rotation of a structuring element, by an angle of 0, 90, 180 or 270 degrees.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/*
   Michel Couprie  -  Janvier 2005
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsym.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  index_t x, y, z, i1, j1, i2, j2;
  index_t rs, cs, ds;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm angle out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readse(argv[1], &x, &y, &z);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readse failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image1);      /* taille ligne */
  cs = colsize(image1);      /* taille colonne */
  ds = depth(image1);        /* nb plans */

  if (ds != 1)
  {
    fprintf(stderr, "%s: NYI for 3D images\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2], "0") == 0) writese(image1, argv[argc - 1], x, y, z);
  else if (strcmp(argv[2], "180") == 0) 
  {
    if (! lsym(image1, 'c'))
    {
      fprintf(stderr, "%s: function lsym failed\n", argv[0]);
      exit(1);
    }
    writese(image1, argv[argc - 1], rs - 1 - x, cs - 1 - y, 1);
  }
  else if ((strcmp(argv[2], "90") == 0) || (strcmp(argv[2], "270") == 0))
  {
    struct xvimage * image2;
    uint8_t *I1, *I2;
    image2 = allocimage(NULL, cs, rs, 1, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    I1 = UCHARDATA(image1);
    I2 = UCHARDATA(image2);
    for (j2 = i1 = 0; i1 < rs; j2++, i1++)
      for (i2 = cs-1, j1 = 0; i2 >= 0; i2--, j1++)
	I2[j2*cs + i2] = I1[j1*rs + i1];
    if (strcmp(argv[2], "90") == 0) 
      writese(image2, argv[argc - 1], cs - 1 - y, x, 1);
    else
    {
      if (! lsym(image2, 'c'))
      {
	fprintf(stderr, "%s: function lsym failed\n", argv[0]);
	exit(1);
      }
      writese(image2, argv[argc - 1], y, rs - 1 - x, 1);
    }
    freeimage(image2);
  }
  else
  {
    fprintf(stderr, "%s: bad angle value (must be 0, 90, 180, 270) : %s\n", argv[0], argv[2]);
    exit(1);
  }
  freeimage(image1);
  return 0;
} /* main */


