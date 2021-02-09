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
/*! \file maxima.c

\brief regional maxima 

<B>Usage:</B> in.pgm connex out.pgm

<B>Description:</B> 
Selects the regional maxima of a grayscale image with connexity <B>connex</B>.

<B>Types supported:</B> byte 2d, int32_t 2d, byte 3d, int32_t 3d

<B>Category:</B> connect, topogray
\ingroup  connect, topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llabelextrema.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t nblabels, connex, i;
  struct xvimage * image;
  struct xvimage * result;
  uint8_t * I;
  int32_t * IL;
  int32_t N;
  int32_t * R;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm (connex = 0[max. absolus], 4,8[2D], 6,18,26[3D]) \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "maxima: readimage failed\n");
    exit(1);
  }
  N = rowsize(image) * colsize(image) * depth(image);

  connex = atoi(argv[2]);

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  R = SLONGDATA(result);

  if (connex == 0)
  {
    if (datatype(image) == VFF_TYP_1_BYTE)
    {   
      uint8_t absmax;
      I = UCHARDATA(image);
      absmax = I[0];
      for (i = 1; i < N; i++) if (I[i] > absmax) absmax = I[i];
      for (i = 0; i < N; i++) if (I[i] == absmax) I[i] = NDG_MAX; else I[i] = NDG_MIN;
    }
    else if (datatype(image) == VFF_TYP_4_BYTE) 
    {   
      int32_t absmax;
      IL = SLONGDATA(image);
      absmax = IL[0];
      for (i = 1; i < N; i++) if (IL[i] > absmax) absmax = IL[i];
      for (i = 0; i < N; i++) if (IL[i] == absmax) IL[i] = NDG_MAX; else IL[i] = NDG_MIN;
    }
  }
  else
  {
    if (! llabelextrema(image, connex, LABMAX, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }

    printf("%s : NOMBRE DE MAXIMA : %d\n", argv[0], nblabels-1);

    if (datatype(image) == VFF_TYP_1_BYTE)
    {   
      I = UCHARDATA(image);
      for (i = 0; i < N; i++)
        if (R[i]) I[i] = NDG_MAX; else I[i] = NDG_MIN;
    }
    else if (datatype(image) == VFF_TYP_4_BYTE) 
    {   
      IL = SLONGDATA(image);
      for (i = 0; i < N; i++)
        if (R[i]) IL[i] = (int32_t)NDG_MAX; else IL[i] = (int32_t)NDG_MIN;
    }
  }
  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    struct xvimage *im2;
    uint8_t *I2;
    im2 = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
    if (im2 == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    I2 = UCHARDATA(im2);
    for (i = 0; i < N; i++) I2[i] = (uint8_t)IL[i];
    writeimage(im2, argv[3]);
  }
  else
    writeimage(image, argv[3]);
  freeimage(result);
  freeimage(image);

  return 0;
} /* main */


