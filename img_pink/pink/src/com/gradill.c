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
/* \file gradill.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>gradill</B> - introduces an illumination gradient in an image

SYNOPSIS

<B>gradill</B> in.pgm alpha out.pgm

DESCRIPTION

The parameter <B>alpha</B> must be between 0.0 and 1.0.
The first row of <B>out.pgm</B> is equal to the first row of <B>in.pgm</B>.
The last row of <B>out.pgm</B> is equal to the last row of <B>in.pgm</B>
multiplied by a factor (1-alpha).
The other rows are multiplied by factors ranging linearly between 
1 and (1-alpha).

Types supported: byte 2d.

CLASS 

gene

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgradill.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  double alpha;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm alpha out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "gradill: readimage failed\n");
    exit(1);
  }

  alpha = atof(argv[2]);
  if ((alpha <= 0) || (alpha > 1))
  {
    fprintf(stderr, "gradill: le parametre doit etre entre 0 et 1\n");
    exit(1);
  }

  if (! lgradill(image1, alpha))
  {
    fprintf(stderr, "gradill: function lgradill failed\n");
    exit(1);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */


