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
/*! \file car2pol.c

\brief converts a 2d image from cartesian to polar coordinates

<B>Usage:</B> in x_center y_center out 

<B>Description:</B>
Converts a 2d image from cartesian to polar coordinates.

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/*
Michel Couprie  -  decembre 2002
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <lgeo.h>

int main(int argc, char **argv)
{
  struct xvimage * image1;
  struct xvimage * image2;
  double x, y; 

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in x_center y_center out \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  x = atof(argv[2]);
  y = atof(argv[3]);

  image2 = lcar2pol(image1, x, y);
  if (! image2)
  {
    fprintf(stderr, "%s: function lcar2pol failed\n", argv[0]);
    exit(1);
  }

  writeimage(image2, argv[argc-1]);
  freeimage(image2);
  freeimage(image1);

  return 0;

}
