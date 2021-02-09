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
/*! \file attribute.c

\brief labels each connected component with its attribute value 

<B>Usage:</B> attribute in.pgm connex seuil typregion attrib out.pgm

<B>Description:</B>

Labels each connected component with its attribute value.

Argument \b connex defines the connectivity.

Argument \b seuil defines a threshold: components with attribute value not higher than \b seuil are filtered out.

Argument \b typregion defines the type of regions:
\li 0: regional maxima
\li 1: regional minima
\li 2: plateaus

Argument \b attrib defines the considered attribute:
\li 0: area
\li 1: perimeter
\li 2: circularity
\li 3: number of holes
\li 4: excentricity
\li 5: orientation
\li 6: horizontal size (of bounding box)
\li 7: vertical size (of bounding box)
\li 8: depth size (of bounding box)
\li 9: max of horizontal, vertical [and depth] sizes (of bounding box)
\li 10: maximum diameter (Euclidean distance between most distant points): not yet implemented

\warning In 3D only the attributes 0 and 6-9 are implemented.

<B>Types supported:</B> byte 2d, byte 3d

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

#define USAGE "usage: %s filein.pgm connex seuil {0=max|1=min|2=pla} {0=area|1=perim|2=circ|3=trous|4=excen|5=orien|6=hdiam|7=vdiam|8=pdiam|9=diam} fileout.pgm\n"

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t connex, attrib, typregion, seuil;
  struct xvimage * image;
  struct xvimage * result;

  if (argc != 7)
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
  seuil = atoi(argv[3]);
  typregion = atoi(argv[4]);
  attrib = atoi(argv[5]);

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lattribute(image, connex, typregion, attrib, seuil, result))
  {
    fprintf(stderr, "%s: lattribute failed\n", argv[0]);
    exit(1);
  }

  writelongimage(result, argv[argc-1]);

  freeimage(result);
  freeimage(image);

  return 0;
} /* main */
