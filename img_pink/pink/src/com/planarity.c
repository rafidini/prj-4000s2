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
/*! \file planarity.c

\brief computes a planarity measure for each connected component

<B>Usage:</B> planarity in.pgm connex out.pgm

<B>Description:</B>
Labels each connected component with its planarity value.
Argument \b connex defines the connectivity.
The result is a floating point image.

<B>Types supported:</B> byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribute.h>

//#define VERBOSE
#define USAGE "usage: %s filein.pgm connex fileout.pgm\n"

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t connex;
  struct xvimage * image;
  struct xvimage * result;

  if (argc != 4)
  {
    fprintf(stderr, USAGE, argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_FLOAT);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (depth(image) == 1)
  {
    fprintf(stderr, "%s: for 3D images only\n", argv[0]);
    exit(1);
  }
  else
  {
    if (! lplanarity(image, connex, result))
    {
      fprintf(stderr, "%s: lplanarity failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(result, argv[argc-1]);

  freeimage(result);
  freeimage(image);

  return 0;
} /* main */
