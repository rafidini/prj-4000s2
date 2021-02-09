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
/*! \file propgeo.c

\brief geodesic propagation of image values

<B>Usage:</B> propgeo in.pgm mask.pgm connex mode out.pgm

<B>Description:</B>
Propagates certain values of the original image <B>in.pgm</B>, 
geodesicaly wrt the connected components of the binary image <B>mask.pgm</B>.
The connexity is specified by the parameter <B>connex</B>.
The <B>mode</B> is one of the following ones:
\li 0 | min   : propagates the minimal value of the component
\li 1 | max   : propagates the maximal value of the component
\li 2 | moy   : propagates the mean grayscale value of the component
\li 3 | min1  : selects one point the value of which is equal to the min
\li 4 | max1  : selects one point the value of which is equal to the max
\li 5 | moy1  : selects one point the value of which is nearest to the mean
\li 6 | minb  : propagates the minimal value of the external border of the component
\li 7 | maxb  : propagates the maximal value of the external border of the component
\li 8 | moyb  : propagates the mean value of the external border of the component
\li 9 | randb : fills the component with random values, with a normal distribution centered around the value computed as for moyb

Only modes min, max and moy are available for int32_t images.

<B>Types supported:</B> byte 2d, int32_t 2d byte 3d, int32_t 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lpropgeo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * masque;
  int32_t connex; 
  int32_t function;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s filein.pgm mask.pgm connex {min1|min|minb|max1|max|maxb|moy1|moy|moyb|randb} fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  masque = readimage(argv[2]);
  if (masque == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);  

  if (strcmp(argv[4], "min1") == 0) function = MIN1; else
  if (strcmp(argv[4], "min") == 0) function = MIN; else
  if (strcmp(argv[4], "minb") == 0) function = MINB; else
  if (strcmp(argv[4], "max1") == 0) function = MAX1; else
  if (strcmp(argv[4], "max") == 0) function = MAX; else
  if (strcmp(argv[4], "maxb") == 0) function = MAXB; else
  if (strcmp(argv[4], "moy1") == 0) function = MOY1; else
  if (strcmp(argv[4], "moy") == 0) function = MOY; else
  if (strcmp(argv[4], "moyb") == 0) function = MOYB; else
  if (strcmp(argv[4], "randb") == 0) function = RANDB; else
    function = atoi(argv[4]);

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    if (! lpropgeo(image, masque, connex, function))
    {
      fprintf(stderr, "%s: lpropgeo failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lpropgeolong(image, masque, connex, function))
    {
      fprintf(stderr, "%s: lpropgeolong failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[5]);
  freeimage(image);
  freeimage(masque);

  return 0;
} /* main */
