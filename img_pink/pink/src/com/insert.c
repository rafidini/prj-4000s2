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
/*!\file insert.c

\brief inserts an image into another image

<B>Usage:</B> insert a.pgm b.pgm x y z out.ppm

<B>Description:</B> Inserts image \b a.pgm into image \b b.pgm at position \b x,y,z

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.

<B>Category:</B> convert, geo
\ingroup convert, geo

\author Michel Couprie
*/
/* 
   Michel Couprie - octobre 1997 
   Revision decembre 1999 (3D)
*/


#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcrop.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * a;
  struct xvimage * b;
  int32_t x, y, z;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s a.pgm b.pgm x y z out.ppm \n", argv[0]);
    exit(1);
  }

  a = readimage(argv[1]);
  if (a == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  b = readimage(argv[2]);
  if (b == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  z = atoi(argv[5]);

  if (! convertgen(&a, &b))
  {
    fprintf(stderr, "%s: function convertgen failed\n", argv[0]);
    exit(1);
  }

  if (!linsert(a, b, x, y, z))
  {
    fprintf(stderr, "%s: linsert failed\n", argv[0]);
    exit(1);
  }
 
  writeimage(b, argv[argc-1]);
  freeimage(a);
  freeimage(b);

  return 0;
} /* main */
