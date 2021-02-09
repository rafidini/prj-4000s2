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
/*! \file 2dskelck.c

\brief homotopic thinning based on critical kernels

<B>Usage:</B> 2dskelck in.pgm mode niter [inhibit] out.pgm

<B>Description:</B>

Homotopic thinning of a 2d Khalimsky image.
The maximum number of steps is given by <B>niter</B>.
If the value given for \b niter equals -1, the thinning is continued
until stability.

The parameter \b mode specifies the algorithm used for thinning.
Possible choices are:
\li 0: ultimate thinning, symmetrical
\li 1: curve thinning, symmetrical

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

\warning Input must be a complex with no point on the image border.

References:<BR> 

[Ber07] G. Bertrand: <A HREF="http://www.esiee.fr/~info/ck/CK_biblio.html#Ber07">"On critical kernels"</A>, Comptes-rendus de l'Académie des Sciences, série math., Vol. I, Nb. 345, pp. 363-367, 2007.<BR>

[BC09] G. Bertrand and M. Couprie: <A HREF="http://www.esiee.fr/~info/ck/CK_biblio.html#BC09">"On parallel thinning algorithms: minimal non-simple sets, P-simple points and critical kernels"</A>, Journal of Mathematical Imaging and Vision, Vol. 35, Nb. 1, pp. 23-35, 2009.<BR>

<B>Types supported:</B> byte 2d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mcimage.h>
#include "mcskel2d.h"

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * k;
  struct xvimage * inhi = NULL;
  int nsteps, mode;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in mode nsteps [inhibit] out\n", argv[0]);
    fprintf(stderr, "  0: ultimate thinning, symmetrical\n");
    fprintf(stderr, "  1: curve thinning, symmetrical\n");
    exit(0);
  }

  k = readimage(argv[1]);
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(0);
  }

  mode = atoi(argv[2]);
  nsteps = atoi(argv[3]);

  if (argc == 6)
  {
    inhi = readimage(argv[4]);
    if (inhi == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(0);
    }
  }

  l2dskelck(k, mode, nsteps, inhi);
  writeimage(k, argv[argc-1]);

  freeimage(k);
  if (inhi) freeimage(inhi);
  return 0;
} /* main() */
