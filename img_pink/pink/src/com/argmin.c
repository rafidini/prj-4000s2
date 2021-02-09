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
/*! \file argmin.c

\brief return the coordinates of a pixel having the minimal value

<B>Usage:</B> argmin in.pgm [out.list]

<B>Description:</B>
This function returns (in the list <B>out.list</B>) 
coordinates of a pixel having the minimal value in the image \b in.pgm .

If the parameter \b out.list is ommitted, the result is printed on the standard output.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rs, ps;
  index_t arg;
  FILE *fd = NULL;
  
  if ((argc != 3) && (argc != 2))
  {
    fprintf(stderr, "usage: %s in.pgm [out.list]\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  ps = rs * colsize(image);

  arg = largmin(image);
 
  if (argc == 3)
  {
    fd = fopen(argv[argc - 1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
      exit(1);
    }
    fprintf(fd, "e %d\n", 3); 
    fprintf(fd, "%d %d %d\n", arg % rs, (arg % ps) / rs, arg/ps); 
    fclose(fd);
  }
  else
    printf("%d %d %d\n", arg % rs, (arg % ps) / rs, arg/ps); 

  freeimage(image);

  return 0;
} /* main */
