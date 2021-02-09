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
/*! \file meanfilter.c

\brief Mean filter operator

<B>Usage:</B> meanfilter in.pgm roi.pgm niter [inhibit.pgm] out.pgm

<B>Description:</B>
Let F be the function stored in image \b in.pgm.
Let R be the function stored in image \b roi.pgm (region of interest).
Let I be the function stored in image \b inhibit.pgm (if any).
The algorithm is the following:

\verbatim
Repeat niter times
  G = F
  For each image point x
    If R[x] and not I[x] then
      S = G[x]; C = 1;
      For each neighbour y of x
        If R[y] then S = S + G[y]; C = C + 1;
      F[x] = S / C;
Result: F
\endverbatim

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d

<B>Category:</B> signal
\ingroup  signal

\author Marcin Janaszewski, Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmeanfilter.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * roi;
  struct xvimage * inhibit = NULL;
  int32_t niter;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm roi.pgm niter [inhibit.pgm] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  roi = readimage(argv[2]);
  if ((image == NULL) || (roi == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  niter = atoi(argv[3]);

  if (argc == 6) 
  {
    inhibit = readimage(argv[4]);
    if (inhibit == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }    
  }

  if (! convertfloat(&image))
  {
    fprintf(stderr, "%s: function convertfloat failed\n", argv[0]);
    exit(1);
  }
  
  if (! lmeanfilter(image, roi, inhibit, niter))
  {
    fprintf(stderr, "%s: function lmeanfilter failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(roi);
  if (inhibit) freeimage(inhibit);
  return 0;
} /* main */


