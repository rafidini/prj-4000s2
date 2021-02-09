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
/* \file temp3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Michel Couprie - novembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * f;
  struct xvimage * g;
  int32_t rs, cs, ps, ds, N;
  uint8_t *F;
  int32_t x;
  int32_t t, tb;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  g = allocimage(NULL, 7, 7, 7, VFF_TYP_1_BYTE);
  if (g == NULL)
  {
    fprintf(stderr,"%s : malloc failed\n", argv[0]);
    exit(1);
  }

  f = readimage(argv[1]);  
  if (f == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(f);
  cs = colsize(f);
  ds = depth(f);
  F = UCHARDATA(f);
  ps = rs * cs;
  N = ps * ds;

  printimage(f);
  printf("N = %d\n", N);

  for (x = 0; x < N; x++)
  {
    if (F[x]) 
    {
      t = T3d(f, x % rs, (x % ps) / rs, x / ps, g);
      tb = Tbar3d(f, x % rs, (x % ps) / rs, x / ps, g);
      printf("point %d,%d,%d : T = %d ; Tbar = %d\n", x % rs, (x % ps) / rs, x / ps, t, tb);
    }
  } 
  
  freeimage(g);
  freeimage(f);

  return 0;
} /* main */
