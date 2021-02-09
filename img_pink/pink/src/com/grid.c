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
/* \file grid.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* surimpressionne une grille a l'image en ndg */

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
  struct xvimage * temp1;
  struct xvimage * temp2;
  int32_t x;
  uint8_t *I;
  uint8_t *T1;
  uint8_t *T2;
  int32_t rs, cs, N;
  int32_t gridstep;
  int32_t step5;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm gridstep out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "grid: readimage failed\n");
    exit(1);
  }
  
  gridstep = atoi(argv[2]);
  step5 = 5 * gridstep;
  temp1 = copyimage(in);
  temp2 = copyimage(in);
  if ((temp1 == NULL) || (temp2 == NULL))
  {
    fprintf(stderr, "grid: copyimage failed\n");
    exit(1);
  }
  
  rs = in->row_size;
  cs = in->col_size;
  N = rs * cs;
  T1 = UCHARDATA(temp1);
  T2 = UCHARDATA(temp2);
  I = UCHARDATA(in);
  for (x = 0; x < N; x++)
  {
    if (((x / rs) % gridstep  == gridstep - 1) || ((x % rs) % gridstep == gridstep - 1))
    {
      I[x] = NDG_MIN;
      T1[x] = NDG_MIN;
      T2[x] = NDG_MAX;           /* petites divisions en bleu */
    }    
    if (((x / rs) % step5  == step5 - 1) || ((x % rs) % step5 == step5 - 1))
    {
      I[x] = NDG_MIN;
      T1[x] = NDG_MAX;           /* grandes divisions en vert */
      T2[x] = NDG_MIN;
    }
  }

  writergbimage(in, temp1, temp2, argv[3]);
  freeimage(in);
  freeimage(temp1);
  freeimage(temp2);

  return 0;
} /* main */
