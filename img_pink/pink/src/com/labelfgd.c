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
/*! \file labelfgd.c

\brief labeling of the foreground components of a binary image

<B>Usage:</B> labelfgd in.pgm connex out.pgm

<B>Description:</B>
Each connected component of \b in.pgm is labeled with a unique integer, starting from 1. The background points are labeled by 0.
The argument \b connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).
The output image \b out.pgm has the type "int32_t".

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> connect
\ingroup  connect

\author Michel Couprie
*/

/*
%TEST labelfgd %IMAGES/2dbyte/binary/b2hebreu.pgm 8 %RESULTS/labelfgd_b2hebreu_8.pgm
%TEST labelfgd %IMAGES/2dbyte/binary/b2hebreu.pgm 4 %RESULTS/labelfgd_b2hebreu_4.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>
#include <llabelextrema.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t connex;
  struct xvimage * image;
  struct xvimage * result;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! llabelfgd(image, connex, result))
  {
    fprintf(stderr, "%s: llabelfgd failed\n", argv[0]);
    exit(1);
  }    

  writeimage(result, argv[argc-1]);
  freeimage(result);
  freeimage(image);

  return 0;
} /* main */


