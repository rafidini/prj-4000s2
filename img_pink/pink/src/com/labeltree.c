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
/*! \file labeltree.c

\brief branch distance

<B>Usage:</B> labeltree in.pgm connex out.pgm

<B>Description:</B>
Level of the branch section defined by his level in the tree. Returns the level
max of the branches.

The distance d used depends on the parameter \b connex :
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> topo
\ingroup   topo

\author Cédric Allène
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llabeltree.h>

/* =============================================================== */
int main(int argc, char **argv)
/* /include/llabeltree.h */
/* /src/lib/llabeltree.c */
/* /src/com/labeltree.c */
/* =============================================================== */
{
  int32_t connex;
  struct xvimage * image;
  struct xvimage * result;
  
  int32_t d_max = 0;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm connex out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8) && (connex != 6) && (connex != 18) && (connex != 26))
  {
    fprintf(stderr, "usage: %s in.pgm {4|8|6|18|26} fileout.pgm\n", argv[0]);
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  d_max = llabeltree(image, connex, result);
  if (! d_max)
  {
    fprintf(stderr, "%s: llabeltree failed\n", argv[0]);
    exit(1);
  }
  
  printf ("%d\n", d_max);
  
  result->name = NULL;
  writeimage(result, argv[argc-1]);
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */


