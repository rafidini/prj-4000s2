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
/* \file histscal.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* transformation multi-lineaire de l'echelle de gris d' une image */
/* Michel Couprie - juillet 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhistscal.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t a, A, b, B, c, C;

  if ((argc != 7) && (argc != 9))
  {
    fprintf(stderr, "usage: %s in.pgm a A b B [c C] out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "histscal: readimage failed\n");
    exit(1);
  }
  
  a = atoi(argv[2]);
  A = atoi(argv[3]);
  b = atoi(argv[4]);
  B = atoi(argv[5]);
  if (argc == 9)
  {
    c = atoi(argv[6]);
    C = atoi(argv[7]);
    if (! lhistscal3(image, a, A, b, B, c, C))
    {
      fprintf(stderr, "histscal: function lhistscal3 failed\n");
      exit(1);
    }
  }
  else
    if (! lhistscal(image, a, A, b, B))
    {
      fprintf(stderr, "histscal: function lhistscal failed\n");
      exit(1);
    }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
