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
/*! \file distsets.c

\brief distance between sets

<B>Usage:</B> distsets in1.pgm in1.pgm [mode [cut] out.list]

<B>Description:</B>

Computes the distance between the object X defined by the binary image
\b in1.pgm and the object Y defined by the binary image \b in2.pgm .

Stores the result (a number) in the file \b out.list .
If only two images are given as arguments, then mode 3 is assumed as the default and the result is printed in stdout.

The used pointwise distance is the exact Euclidean distance (float).

The definition of the set distance used depends on the parameter \b mode :
\li 0: Hausdorff
\li 1: Baddeley, order 1
\li 2: Baddeley, order 2
\li 3: Dubuisson-Jain

The optional parameter \b cut is required only for Baddeley distances. 

\warning The input images \b in1.pgm and \b in2.pgm must be binary images. No test is done.

<B>Types supported:</B> byte 2d,  byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t mode;
  struct xvimage * image1;
  struct xvimage * image2;
  float result, cut = 0;
  FILE *fd = NULL;

  if ((argc != 3) && (argc != 4) && (argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm [mode [cut] [out.list]]\n", argv[0]);
    fprintf(stderr, "       mode = 0 (Hausdorff), 1 (Baddeley 1), 2 (Baddeley 2),\n");
    fprintf(stderr, "              3 (mean pointwise distance, default)\n");
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc != 3) mode = atoi(argv[3]); else mode = 3;

  if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3))
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm mode [cut] out.list\n", argv[0]);
    fprintf(stderr, "       mode = 0 (Hausdorff), 1 (Baddeley 1), 2 (Baddeley 2),\n");
    fprintf(stderr, "              3 (Dubuisson-Jain)\n");
    exit(1);
  }

  if ((mode == 1) || (mode == 2))
  {
    if (argc == 6)
      cut = (float)atof(argv[4]);
    else
    {
      fprintf(stderr, "%s: cut parameter needed for Baddeley distances\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (argc == 6)
    {
      fprintf(stderr, "%s: cut parameter not needed\n", argv[0]);
      exit(1);
    }
  }
  result = ldistsets(image1, image2, mode, cut);
  if (result < 0)
  {
    fprintf(stderr, "%s: ldistset failed\n", argv[0]);
    exit(1);
  }

  if (argc > 4)  
  {
    fd = fopen(argv[argc - 1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
      exit(1);
    }
    fprintf(fd, "e %d\n", 1); 
    fprintf(fd, "%g\n", result); 
    fclose(fd);
  }
  else
  {
    printf("%g\n", result); 
  }
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


