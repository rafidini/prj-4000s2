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
/* \file filtreordre.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* filtre d'ordre sur un voisinage quelconque */
/* Michel Couprie - decembre 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltreordre.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * elem;
  int32_t x, y, z;
  double r;

  if ((argc != 7) && (argc != 8))
  {
    fprintf(stderr, "usage: %s f.pgm el.pgm x y [z] r out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem = readimage(argv[2]);
  if ((image == NULL) || (elem == NULL))
  {
    fprintf(stderr, "filtreordre: readimage failed\n");
    exit(1);
  }
  
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  if (argc == 7)
    r = atof(argv[5]);
  else
  {
    z = atof(argv[5]);
    r = atof(argv[6]);
  }
  if ((r < 0.0) || (r > 1.0))
  {
    fprintf(stderr, "filtreordre: r = %g ; on doit avoir 0 <= r <= 1\n", r);
    exit(1);
  }
  if (argc == 7)
  {
    if (! lfiltreordre(image, elem, x, y, r))
    {
      fprintf(stderr, "filtreordre: function lfiltreordre failed\n");
      exit(1);
    }
  }
  else
  {
    if (! lfiltreordre3d(image, elem, x, y, z, r))
    {
      fprintf(stderr, "filtreordre: function lfiltreordre failed\n");
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
