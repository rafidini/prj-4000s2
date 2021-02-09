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
/* \file regul.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/

#define SATUR

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lregul.h>
#include <llvkern.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  int32_t mode;
  double alpha;
  int32_t nbreg;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in1.pgm mode [0:basse, 1:haute, 2: les deux] alpha out.pgm \n", argv[0]);
    exit(1);
  }
  mode = atoi(argv[2]);
  if ((mode < 0) || (mode > 2))
  {
    fprintf(stderr, "usage: %s in1.pgm mode [0:basse, 1:haute, 2: les deux] alpha out.pgm \n", argv[0]);
    exit(1);
  }
  alpha = atof(argv[3]);

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

#ifdef SATUR
  do
  {
#endif
    if (! lregul(image1, mode, alpha, &nbreg))
    {
      fprintf(stderr, "%s: function lregul failed\n", argv[0]);
      exit(1);
    }
    printf("regul : %d REGULARISATIONS\n", nbreg);
    {
      if (! llvkern(image1, 0, 4))
      {
        fprintf(stderr, "%s: function llvkern failed\n", argv[0]);
        exit(1);
      }
    }
#ifdef SATUR
  }
  while (nbreg);
#endif

  writeimage(image1, argv[4]);
  freeimage(image1);

  return 0;
} /* main */


