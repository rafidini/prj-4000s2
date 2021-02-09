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
/*! \file threshold.c

\brief simple threshold

<B>Usage:</B> threshold in.pgm n [n2] out.pgm

<B>Description:</B>
If n2 is not present, for each pixel x, out[x] = if (in[x] < n) then 0 else 255

If n2 is present, for each pixel x, out[x] = if (n <= in[x] < n2) then 255 else 0

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie 2010
*/

/*
En remplacement de l'opérateur seuil, conservé pour raison de compatibilité
TEST: voir seuil
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseuil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  double seuil, seuil2;
  struct xvimage * image;
  struct xvimage * imagebin;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm seuil [seuil2] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  seuil = atof(argv[2]);
  if (argc==5)
    seuil2 = atof(argv[3]);
  else seuil2 = 0;

  if (argc == 4)
  {
    if (! lseuil(image, seuil))
      {
	fprintf(stderr, "%s: function lseuil failed\n", argv[0]);
	exit(1);
      }
  }
  else
  {
    if (! lseuil3(image, seuil, seuil2))
      {
	fprintf(stderr, "%s: function lseuil3 failed\n", argv[0]);
	exit(1);
      }
  }

  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    index_t rs, cs, ds, N, x;
    uint8_t *F;
    rs = rowsize(image); cs = colsize(image); ds = depth(image); N = rs * cs * ds; 
    imagebin = allocimage(image->name, rs, cs, ds, VFF_TYP_1_BYTE);
    if (imagebin == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    F = UCHARDATA(imagebin); 

    if (datatype(image) == VFF_TYP_4_BYTE)
    {
      int32_t *FL = SLONGDATA(image);
      for (x = 0; x < N; x++) F[x] = (uint8_t)FL[x];
    }
    else if (datatype(image) == VFF_TYP_2_BYTE)
    {
      int16_t *FL = SSHORTDATA(image);
      for (x = 0; x < N; x++) F[x] = (uint8_t)FL[x];
    }
    else if (datatype(image) == VFF_TYP_FLOAT)
    {
      float *FL = FLOATDATA(image);
      for (x = 0; x < N; x++) if (FL[x] == 0.0) F[x] = NDG_MIN; else F[x] = NDG_MAX;
    }
    else if (datatype(image) == VFF_TYP_DOUBLE)
    {
      double *FL = DOUBLEDATA(image);
      for (x = 0; x < N; x++) if (FL[x] == 0.0) F[x] = NDG_MIN; else F[x] = NDG_MAX;
    }
    else
    {
      fprintf(stderr, "%s: bad data type %d\n", argv[0], datatype(image));
      exit(1);
    }
    writeimage(imagebin, argv[argc-1]);
    freeimage(imagebin);
  }
  else
  {
    writeimage(image, argv[argc-1]);
  }

  freeimage(image);
  return 0;
} /* main */
