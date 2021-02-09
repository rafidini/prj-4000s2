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
/*! \file crestrestoration.c

\brief crest restoration algorithm 

<B>Usage:</B> crestrestoration im.pgm {imcond.pgm|null} connex niter fileout.pgm [condout.pgm]

<B>Description:</B>
Crest restoration algorithm, as described in ref. CBB01.
The input image \b im.pgm must be a "thin" grayscale image, as
the output of the operator \b hthin.
The parameter \b connex indicates the connectivity used for the minimal 
regions.
The parameter \b niter gives the number of iterations.
The optional parameter \b imcond.pgm is a binary image (a set C) which indicates the points
in the neighborhood of which the extensible points will be searched.
The points which are modified by the algorithm will be dynamically added to C.
The optional parameter \b condout.pgm is an output file containing the final state of the 
set C.

Reference:<BR> 
[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
grayscale image processing"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.

<B>Types supported:</B> byte 2D

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t nitermax;
  int32_t connex;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s im.pgm {imcond.pgm|null} connex niter fileout.pgm [condout.pgm]\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  nitermax = atoi(argv[4]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
  {
    if (argc == 7)
    {
      fprintf(stderr, "%s: cannot generate condout.pgm when imcond is null\n", argv[0]);
      exit(1);
    }
    imagecond = NULL;
  }
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lcrestrestoration(image, imagecond, nitermax, connex))
  {
    fprintf(stderr, "crestrestoration: lcrestrestoration failed\n");
    exit(1);
  }

  if (argc == 6) 
    writeimage(image, argv[argc - 1]);
  else
  {
    writeimage(imagecond, argv[argc - 1]);
    writeimage(image, argv[argc - 2]);
  }
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
