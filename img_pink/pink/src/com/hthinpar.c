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
/*! \file hthinpar.c

\brief parallel grayscale homotopic thinning

<B>Usage:</B> hthinpar in.pgm {imcond.pgm|null} connex niter out.pgm

<B>Description:</B>
Parallel grayscale homotopic thinning (refs. [BEC97, CBB01]).
The parameter \b connex gives the connectivity used for the minima;
possible choices are 4, 8 in 2D and 6, 26 in 3D.
Let F be the function corresponding to the input image \b in.pgm.
Let G be the function corresponding to the input image 
\b imcond.pgm, or the null function if the keyword \b null is used.

If \b niter is set to -1, then the operator iterates until stability.

References:

to appear

<B>Types supported:</B> byte 2D.

<B>Category:</B> topogray
\ingroup  topogray

\author Nivando Bezerra
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lskelpar.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t nitermax;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm {imcond.pgm|null} niter fileout.pgm\n", argv[0]);
    exit(1);
  }

  nitermax = atoi(argv[3]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
    imagecond = NULL;
  else
  {
    fprintf(stderr, "%s: imagecond not yet implemented\n", argv[0]);
    exit(1);
    /*    
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    */
  }

  //  if (! lhthinpar(image, nitermax))
  if (! lhthinpar_asymmetric(image, nitermax))
  {
    fprintf(stderr, "%s: lhthinpar_asymmetric failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
