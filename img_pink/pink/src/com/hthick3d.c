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
/* \file hthick3d.c

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
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t nitermax;
  int32_t connex;
  int32_t alpha = 0;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s im.pgm {imcond.pgm|null} connex niter {alpha|delta} fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 26) && (connex != 6))
  {
    fprintf(stderr, "%s : connex = {6|26}\n", argv[0]);
    exit(1);
  }

  nitermax = atoi(argv[4]);

  if (strcmp(argv[5],"alpha") == 0) 
    alpha = 1;
  else
  if (strcmp(argv[5],"delta") != 0) 
  {
    fprintf(stderr, "usage: %s im.pgm {imcond.pgm|null} connex niter {alpha|delta} fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (alpha)
  {
    fprintf(stderr, "%s: alpha NYI \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
    imagecond = NULL;
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lhthickdelta3d(image, imagecond, nitermax, connex))
  {
    fprintf(stderr, "%s: lhthickdelta3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[6]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
