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
/*! \file average1.c

\brief return the average of the pixel values of an image

<B>Usage:</B> average1 in.pgm [mask.pgm] out.list

<B>Description:</B>
This function returns (in the list <B>out.list</B>) 
the average of the pixel values of the image \b in.pgm .
If the optional parameter \b mask.pgm is given, then only the 
values which correspond to non-null points of mask are averaged.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> signal stats
\ingroup  signal stats

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lstat.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask = NULL;
  double averageval;
  FILE *fd = NULL;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [mask.pgm] out.list\n", argv[0]);
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
    averageval = laverage2(image, mask);
  }
  else
    averageval = laverage1(image);

#ifdef VERBOSE
    printf("averageval: %g\n", averageval);
#endif

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "e %d\n", 1); 
  fprintf(fd, "%g\n", averageval); 
  fclose(fd);

  freeimage(image);
  if (mask) freeimage(mask);

  return 0;
} /* main */
