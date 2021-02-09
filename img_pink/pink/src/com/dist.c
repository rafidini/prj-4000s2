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
/*! \file dist.c

\brief distance transform (external)

<B>Usage:</B> dist in.pgm mode out.pgm

<B>Description:</B>
Distance to the object X defined by the binary image \b in.pgm .
The result function DX(x) is defined by: DX(x) = min {d(x,y), y in X}.

The distance d used depends on the parameter \b mode:
\li 0: euclidean distance (rounded to the nearest int32)
\li 1: approximate quadratic euclidean distance (Danielsson)
\li 2: chamfer distance ([5,7] in 2D; [4,5,6] in 3D)
\li 3: exact quadratic euclidean distance (int32)
\li 4: 4-distance in 2d
\li 5: exact euclidean distance (float)
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d
\li 40: 4-distance in 2d (byte coded ouput)
\li 80: 8-distance in 2d (byte coded ouput)
\li 60: 6-distance in 3d (byte coded ouput)
\li 180: 18-distance in 3d (byte coded ouput)
\li 260: 26-distance in 3d (byte coded ouput)

The output \b out.pgm is of type int32_t for modes < 40, of type byte for other modes.

<B>Types supported:</B> byte 2d,  byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie, Xavier Daragon
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <mcutil.h>
#include <ldist.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t mode;
  struct xvimage * image;
  struct xvimage * result;
  index_t N, i;
  uint8_t *F;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm mode fileout.pgm\n", argv[0]);
    fprintf(stderr, "       mode = 0 (dist. eucl. trunc), 1 (dist. eucl. quad.), 2 (chamfrein),\n");
    fprintf(stderr, "              3 (exact eucl. quad.), 5 (exact eucl.), 4, 8 (2D), 6, 18, 26 (3D)\n");
    fprintf(stderr, "                40, 80 (2D), 60, 180, 260 (3D)\n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[2]);
  if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 5) &&
      (mode != 4) && (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26) &&
      (mode != 40) && (mode != 80) && (mode != 60) && (mode != 180) && (mode != 260))
  {
    fprintf(stderr, "usage: %s filein.pgm mode fileout.pgm\n", argv[0]);
    fprintf(stderr, "       mode = 0 (dist. eucl. trunc), 1 (dist. eucl. quad.), 2 (chamfrein),\n");
    fprintf(stderr, "              3 (exact eucl. quad.), 5 (exact eucl.), 4, 8 (2D), 6, 18, 26 (3D)\n");
    fprintf(stderr, "                40, 80 (2D), 60, 180, 260 (3D)\n");
    exit(1);
  }

  if (mode < 40)
    result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  else
    result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  N = rowsize(image) * colsize(image) * depth(image);
  F = UCHARDATA(image);;

  if (mode == 1)
  {
    if (depth(image) == 1)
    {
      if (! ldistquad(image, result))
      {
        fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
        exit(1);
      }
    }
    else
    {
      if (! ldistquad3d(image, result))
      {
        fprintf(stderr, "%s: ldistquad3d failed\n", argv[0]);
        exit(1);
      }
    }
  }
  else if (mode == 2)
  {
    if (! lchamfrein(image, result))
    {
      fprintf(stderr, "%s: lchamfrein failed\n", argv[0]);
      exit(1);
    }
  }
  else if ((mode == 0) || (mode == 3) || (mode == 5))
  {
    for (i = 0; i < N; i++) // inverse l'image
      if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
    if (! lsedt_meijster(image, result))
    {
      fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
      exit(1);
    }
    if (mode == 0)
    {
      double d;
      uint32_t *R = ULONGDATA(result);
      for (i = 0; i < N; i++) 
      {
	
	d = sqrt((double)(R[i]));
	R[i] = (uint32_t)arrondi(d);
      }
    }
    else if (mode == 5)
    {
      float *D;
      convertfloat(&result);
      D = FLOATDATA(result);
      for (i = 0; i < N; i++) D[i] = (float)sqrt(D[i]);
    }
  }
  else if (mode < 40)
  {
    if (! ldist(image, mode, result))
    {
      fprintf(stderr, "%s: ldist failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! ldistbyte(image, mode, result))
    {
      fprintf(stderr, "%s: ldist failed\n", argv[0]);
      exit(1);
    }
  }
  writeimage(result, argv[3]);
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */


