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
/* \file temp3dthin.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
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
  struct xvimage * o;
  struct xvimage * k;
  struct xvimage * kp;
  int32_t nsteps;
  int32_t stablealpha, stablebeta;
  int32_t i, x, y, z;
  int32_t rs, cs, ps, d, N;
  uint8_t * K;
  uint8_t * KP;
  
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm n out.pgm \n", argv[0]);
    exit(1);
  }


  o = readimage(argv[1]);  
  if (o == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  nsteps = atoi(argv[2]);

  fprintf(stderr, "Debut traitement\n");

  k = Khalimskize3d(o);

  fprintf(stderr, "Khalimskisation terminee\n");

  if (k == NULL)
  {
    fprintf(stderr, "%s: Khalimskize3d failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(k);
  cs = colsize(k);
  d = depth(k);
  ps = rs * cs;
  N = ps * d;
  K = UCHARDATA(k);

  Connex6Obj3d(k);

  fprintf(stderr, "Connex6 termine\n");

  InitPileGrilles3d();

  kp = copyimage(k);
  KP = UCHARDATA(kp);

  for (i = 1; i <= nsteps; i++)
  {
    fprintf(stderr, "step %d\n", i);
    if (i % 2)
    {
      stablealpha = 1;
      for (z = 0; z < d; z++)
      for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if (K[z * ps + y * rs + x] && Alpha3Simple3d(k, x, y, z))
          { KP[z * ps + y * rs + x] = 0; stablealpha = 0; }
      memcpy(K, KP, N);
    }
    else
    {
      stablebeta = 1;
      for (z = 0; z < d; z++)
      for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if (K[z * ps + y * rs + x] && Beta3Simple3d(k, x, y, z))
          { KP[z * ps + y * rs + x] = 0; stablebeta = 0; }
      memcpy(K, KP, N);
    }
    if (stablealpha && stablebeta) break;
  }

  TerminePileGrilles3d();
  
  ColorieKh3d(k);
  writeimage(k, "tmp");

  DeKhalimskize3d(k, o);
  writeimage(o, argv[3]);

  freeimage(o);
  freeimage(k);
  freeimage(kp);

  return 0;
} /* main */
