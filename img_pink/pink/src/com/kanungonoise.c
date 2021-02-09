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
/*! \file bruite.c

\brief adds kanungo noise to an image

<B>Usage:</B> kanungonoise in.pgm eta alpha0 alpha beta0 beta k out.pgm

<B>Description:</B>
Adds kanungo noise to the image in.pgm .
 
\reference 
T. Kanungo, R.M. Haralick, H.S. Baird, W. Stuetzle, D. Madigan, A statistical, nonparametric methodology for document degradation model validation, IEEE Trans. on Pattern Analysis and Machine Intelligence 22 (2000) 1209–1223. 

<B>Types supported:</B> byte 2d

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie
*/

/* Michel Couprie - mai 2017 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcprobas.h>
#include <ldist.h>

//#define RANDOMIZE
#ifdef RANDOMIZE
#include <mcchrono.h>
#endif

void usage(int argc, char **argv)
{
    fprintf(stderr, "usage: %s in.pgm eta alpha0 alpha beta0 beta k out.pgm\n", argv[0]);
    exit(1);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * dist;
  struct xvimage * distc;
  int32_t i;
  uint8_t *I;
  uint32_t *D, *DC;
  
  int32_t rs, cs, ds, N;
  double eta, alpha0, alpha, beta0, beta, k, k2 ;

  if (argc != 9)
    usage(argc, argv);

#ifdef RANDOMIZE
  ResetProb(usecs());
#endif
  
  eta = atof(argv[2]);
  alpha0 = atof(argv[3]);
  alpha = atof(argv[4]);
  beta0 = atof(argv[5]);
  beta = atof(argv[6]);
  k = atof(argv[7]);
  k2 = k * k;

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  N = rs * cs * ds;
  I = UCHARDATA(image);

  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "byte images only\n");
    exit(1);
  }

  dist = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  assert(dist != NULL);
  distc = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  assert(distc != NULL);

  if (! ldistquad(image, dist)) // calcule les distances au carré à l'objet
  {
    fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
    exit(1);
  }
  for (i = 0; i < N; i++) if (I[i]) I[i] = 0; else I[i] = NDG_MAX; // inverse l'image
  if (! ldistquad(image, distc)) // calcule les distances au carré au complémentaire
  {
    fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
    exit(1);
  }
  for (i = 0; i < N; i++) if (I[i]) I[i] = 0; else I[i] = NDG_MAX; // inverse l'image
  D = ULONGDATA(dist);
  DC = ULONGDATA(distc);
  for (i = 0; i < N; i++) D[i] += DC[i];

  for (i = 0; i < N; i++) 
  {
    if (I[i])
    {
      if (Prob() < (alpha0 * exp(-alpha*DC[i]) + eta)) I[i] = 0;
    }
    else // I[i] == 0
    {
      if (Prob() < (beta0 * exp(-beta*D[i]) + eta)) I[i] = NDG_MAX;
    }
  } // for (i = 0; i < N; i++) 
  

  for (i = 0; i < N; i++) if (I[i]) I[i] = 0; else I[i] = NDG_MAX; // inversion 

  // dilation by a disk of radius k
  if (! ldistquad(image, dist)) // calcule les distances au carré à l'objet
  {
    fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
    exit(1);
  }
  for (i = 0; i < N; i++) if (D[i] <= k2) I[i] = 0; else I[i] = NDG_MAX; // seuil et inversion

  // dilation by a disk of radius k
  if (! ldistquad(image, dist)) // calcule les distances au carré à l'objet
  {
    fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
    exit(1);
  }
  for (i = 0; i < N; i++) if (D[i] <= k2) I[i] = NDG_MAX; else I[i] = 0; // seuil

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

