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

\brief adds noise to an image

<B>Usage:</B> bruite in.pgm n alpha p out.pgm

<B>Description:</B>
Adds a uniform or gaussian noise, of mean 0 and standard deviation <B>alpha</B>, 
to a proportion <B>p</B> of the pixels of the image in.pgm .

  OPTIONS
    <B>n</B> = 0  uniform noise
    <B>n</B> = 1  gaussian noise

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie
*/

/* Michel Couprie - mai 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcprobas.h>

//#define RANDOMIZE
#ifdef RANDOMIZE
#include <mcchrono.h>
#endif

static double bruite(double f, int32_t type, double alpha, double p) 
{
  double tmp;

  switch(type)
  {
    case 0:
      tmp = Prob();
      if (tmp <= p) {
	tmp = ((double)f) + Uniform(-2*alpha,2*alpha);
	//if (tmp>255) printf("tmp = %lg\n", tmp);
      }
      else 
	tmp = f;
      break;
    case 1:
      tmp = Prob();
      if (tmp <= p)
	tmp = ((double)f) + Normal(0.0, alpha);
      else 
	tmp = f;
      break;
    default:
      exit(1);
  }
  return tmp;
}

void usage(int argc, char **argv)
{
    fprintf(stderr, "usage: %s in.pgm n alpha p out.pgm\n", argv[0]);
    fprintf(stderr, "       n = 0 (uniforme) | 1 (gaussien) \n");
    fprintf(stderr, "       alpha = ecart-type ; p = proportion de pixels a changer (0 < p <= 1) \n");
    fprintf(stderr, "Note that for ULONG image, the pixel value is limited to %d\n", UINT_MAX);
    exit(1);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t i;
  uint8_t *ImUC;
  int32_t *ImUL;
  
  int32_t rs, cs, d, N;
  int32_t n;
  double alpha, tmp, p;

  if (argc != 6)
    usage(argc, argv);

#ifdef RANDOMIZE
  ResetProb(usecs());
#endif

  n = atoi(argv[2]);
  if ((n!=0) && (n!=1))
    usage(argc, argv);
  
  alpha = atof(argv[3]);
  p = atof(argv[4]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;

  switch(datatype(image)) {
  case VFF_TYP_1_BYTE:
    ImUC = UCHARDATA(image);
    for (i = 0; i < N; i++) {
      tmp = bruite((double)(ImUC[i]), n, alpha, p);
      ImUC[i] = (uint8_t)mcmax(NDG_MIN,mcmin(NDG_MAX,tmp));
    }
    break;
  case VFF_TYP_4_BYTE:
    ImUL = SLONGDATA(image);
    for (i = 0; i < N; i++) {
      tmp = bruite((double)(ImUL[i]), n, alpha, p);
      ImUL[i] = (int32_t)mcmax(INT32_MIN,mcmin(INT32_MAX,tmp));
    }
    break;
  default:
    fprintf(stderr, "Type of image not implemented\n");
    exit(1);
  }
  image->name = NULL;
  writeimage(image, argv[5]);
  freeimage(image);

  return 0;
} /* main */

