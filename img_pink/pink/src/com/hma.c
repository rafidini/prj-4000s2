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
/*!
\brief Higher-resolution Medial Axis (HMA) extraction

<B>Usage:</B> hma in.pgm [mode] out.pgm

<B>Description:</B>
Higher-resolution Medial Axis (HMA) of the binary image \b Xh, where Xh has
been obtained by H(X), X being the original object and H, the H-transform.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Andre Vital Saude
*/

/* update 2/4/6 MC : simplification */

//#define VERBOSE

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcchrono.h>
#include <avscrop.h>
#include <avsimage.h>
#include <llut.h>
#include <lhma.h>
#include <lcrop.h>
#include <l2dkhalimsky.h>
#include <l3dkhalimsky.h>

//#define CHRONO

/* =============================================================== */
void hma_print_usage(char **argv)
/* =============================================================== */
{
  fprintf(stderr, "\nusage: %s filein.pgm fileout.pgm\n", argv[0]);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * X = NULL;
  struct xvimage * aux = NULL;
  struct xvimage * res = NULL;
  struct xvimage * resaux = NULL;
  int32_t rs, cs, ds;
#ifdef CHRONO
  chrono Chrono1;
#endif

  //arguments
  if (argc != 3) { hma_print_usage(argv); exit(1); }

  X = readimage(argv[1]);
  if (X == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(X);
  cs = colsize(X);
  ds = depth(X); 

  aux = lenframe(X, 0, 1);
#ifdef CHRONO
  start_chrono(&Chrono1);
#endif
  res = lhma(aux);  // do it
#ifdef CHRONO
  printf("nb de usec ecoulees = %d\n", read_chrono(&Chrono1));
#endif
  if (!res) {
    fprintf(stderr, "%s: lhma failed\n", argv[0]);
    exit(1);
  }
  if (ds == 1) 
    resaux = lcrop(res, 1, 1, rs, cs);
  else
    resaux = lcrop3d(res, 1, 1, 1, rs, cs, ds);

  if (!resaux) {
    fprintf(stderr, "%s: lcrop failed\n", argv[0]);
    exit(1);
  }

  writeimage(resaux, argv[argc - 1]);
  freeimage(X);
  freeimage(res);
  freeimage(aux);
  freeimage(resaux);

  return 0;
} /* main */

