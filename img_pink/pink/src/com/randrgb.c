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
/*! \file randrgb.c

\brief generates random colors from gray levels

<B>Usage:</B> randrgb in.pgm out.pgm

<B>Description:</B>
Generates a color image where each gray level is replaced by a randomly
chosen color

<B>Types supported:</B> byte 2d, int32_t 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/* genere une image en couleurs aleatoires a partir d'une image en gris */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;
  uint8_t r[256];
  uint8_t g[256];
  uint8_t b[256];
  int32_t rs, cs, N;
  int32_t i;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);

  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = in->row_size;
  cs = in->col_size;
  N = rs * cs;

  g1 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  g2 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  g3 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if ((g1 == NULL) || (g2 == NULL) || (g3 == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  r[0] = g[0] = b[0] = 0;
  for (i = 1; i < 256; i++)
  {
    r[i] = rand() % 256;
    g[i] = rand() % 256;
    b[i] = rand() % 256;
  }

  if (datatype(in) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(in);
    uint8_t v;
    for (i = 0; i < N; i++)
    {
      v = F[i];
      (UCHARDATA(g1))[i] = r[v];
      (UCHARDATA(g2))[i] = g[v];
      (UCHARDATA(g3))[i] = b[v];
    }
  }
  else   if (datatype(in) == VFF_TYP_4_BYTE)
  {
    int32_t *F = SLONGDATA(in);
    uint8_t v;
    for (i = 0; i < N; i++)
    {
      v = (uint8_t)(F[i] % 256);
      (UCHARDATA(g1))[i] = r[v];
      (UCHARDATA(g2))[i] = g[v];
      (UCHARDATA(g3))[i] = b[v];
    }
  }
  else   
  {
    fprintf(stderr, "%s: bad data type\n", argv[0]);
    exit(1);
  }

  writergbimage(g1, g2, g3, argv[2]);
  freeimage(in);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  return 0;
} /* main */
