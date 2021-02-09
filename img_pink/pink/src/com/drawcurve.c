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
/*! \file drawcurve.c

\brief draw line segments which are specified by a text file

<B>Usage:</B> drawcurve in.pgm curve.txt out.pgm

<B>Description:</B>
The file \b curve.txt contains a list of points under the format:<br>
c nb_points<br>
x1 y1<br>
x2 y2<br>
x3 y3<br>
...<br>

<B>Types supported:</B> byte 2D

<B>Category:</B> draw
\ingroup draw

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ldraw.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t i;
  double x1, y1, x2, y2;
  int32_t X1, Y1, X2, Y2;
  FILE *fd = NULL;
  int32_t npoints;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm curve.txt out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (depth(image) > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }
  
  fscanf(fd, "%c", &type);
  if ((type != 'c') && (type != 'b'))
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", argv[0], type);
    exit(1);
  }
  fscanf(fd, "%d", &npoints);
  fscanf(fd, "%lf%lf", &x1, &y1);
  X1 = arrondi(x1);
  Y1 = arrondi(y1);

  for (i = 1; i < npoints; i++)
  {
    fscanf(fd, "%lf%lf", &x2, &y2);
    X2 = arrondi(x2);
    Y2 = arrondi(y2);
    ldrawline(image, X1, Y1, X2, Y2);
    X1 = X2; Y1 = Y2;
  }
  fclose(fd);
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

