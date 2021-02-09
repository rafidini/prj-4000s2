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
/*! \file medialaxis.c

\brief medial axis transform

<B>Usage:</B> medialaxis in.pgm distance out.pgm

<B>Description:</B>
Medial axis of the binary image \b X. If x is the center of a maximal ball
included in X, then the result R(x) is equal to the smallest distance between x 
and a point outside X, otherwise it is equal to 0 .
The distance is indicated by the parameter <B>distance</B> :
\li 0: approximate euclidean distance (Meyer's algorithm)
\li 1: exact quadratic Euclidean distance (Saito-Toriwaki's skeleton)
\li 2: exact quadratic Euclidean distance (Coeurjolly's reduced axis)
\li 3: exact quadratic Euclidean distance (Rémy-Thiel)
\li 4: 4-distance in 2d
\li 6: 6-distance in 3d
\li 8: 8-distance in 2d
\li 26: 26-distance in 3d

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

/*
%TEST medialaxis %IMAGES/2dbyte/binary/b2hebreu.pgm 0 %RESULTS/medialaxis_b2hebreu_0.pgm
%TEST medialaxis %IMAGES/2dbyte/binary/b2hebreu.pgm 3 %RESULTS/medialaxis_b2hebreu_3.pgm
%TEST medialaxis %IMAGES/2dbyte/binary/b2hebreu.pgm 4 %RESULTS/medialaxis_b2hebreu_4.pgm
%TEST medialaxis %IMAGES/2dbyte/binary/b2hebreu.pgm 8 %RESULTS/medialaxis_b2hebreu_8.pgm
%TEST medialaxis %IMAGES/3dbyte/binary/b3a.pgm 0 %RESULTS/medialaxis_b3a_0.pgm
%TEST medialaxis %IMAGES/3dbyte/binary/b3a.pgm 3 %RESULTS/medialaxis_b3a_3.pgm
%TEST medialaxis %IMAGES/3dbyte/binary/b3a.pgm 6 %RESULTS/medialaxis_b3a_6.pgm
%TEST medialaxis %IMAGES/3dbyte/binary/b3a.pgm 26 %RESULTS/medialaxis_b3a_26.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * res;
  int32_t distance;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm distance fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  distance = atoi(argv[2]);

  res = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (res == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (!lmedialaxis_lmedialaxis(image, distance, res))
  {
    fprintf(stderr, "%s: lmedialaxis_lmedialaxis failed\n", argv[0]);
    exit(1);
  }

  writeimage(res, argv[argc - 1]);
  freeimage(image);
  freeimage(res);

  return 0;
} /* main */
