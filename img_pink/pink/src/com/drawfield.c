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
/*! \file drawfield.c

\brief draw line segments that represent a vector field

<B>Usage:</B> drawfield in.pgm [len] out.pgm

<B>Description:</B>
The file \b in.pgm contains a vector field. This operator generates a (binary) image where each vector is represented by a straight line segment of length \b len (default 10).

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/
/*
  Michel Couprie - juillet 2010 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ldraw.h>
#include <lbresen.h>

//#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * field;
  struct xvimage * image;
  int32_t rs, cs, ds;
  double len = 10;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [len] out.pgm \n", argv[0]);
    exit(1);
  }

  field = readimage(argv[1]);
  if (field == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(field);
  cs = colsize(field);
  ds = depth(field);

  if (argc == 4) len = atof(argv[2]);

  if (ds == 1) 
    image = ldrawfield2d(field, len);
  else
    image = ldrawfield3d(field, len);

  writeimage(image, argv[argc-1]);
  freeimage(field);
  freeimage(image);

  return 0;
} /* main */

