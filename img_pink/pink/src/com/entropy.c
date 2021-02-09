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
/*! \file entropy.c

\brief computes the Shannon entropy of an image or a region

<B>Usage:</B> entropy in.pgm [mask.pgm] out.list

<B>Description:</B>
Calculates the Shannon entropy of \b im.pgm (masked by the binary image
\b mask.pgm, if given) and saves it in file \b out.list .

Let H(i), i = 0...B-1, denote the histogram of \b im.pgm, where B is the number of bins.
Let P(i) be the frequency of the level i in the image, that is, P(i) = H(i)/N, where N is the number of pixels of \b im.pgm .
Then, the Shannon entropy of \b im.pgm is defined by: E = - SUM P(i) log_2 P(i), for all i in {0...B-1}.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhisto.h>
#include <larith.h>

#define INDEX_FIRST
/*
*/

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask = NULL;
  FILE *fd = NULL;
  double E;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask.pgm] out.list\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    mask = readimage(argv[2]);
    if (mask == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  fd = fopen(argv[argc-1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

  E = lentropy(image, mask);

  fprintf(fd, "e %d\n", 1); 
  fprintf(fd, "%g\n", E); 
  fclose(fd);
  freeimage(image);
  if (mask) freeimage(mask);

  return 0;
} /* main */
