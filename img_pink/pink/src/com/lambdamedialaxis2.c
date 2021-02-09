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
/*! \file lambdamedialaxis2.c

\brief discrete lambda-medial axis transform

<B>Usage:</B> lambdamedialaxis2 in.pgm auxlambda lambda out.pgm

<B>Description:</B>

Experimental

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * distimage;
  struct xvimage * lambdaimage;
  struct xvimage * opening;	
  double lambda, lowlambda;
  uint32_t rs, cs, ds, N, i;
  float max, *L, *O;;
  uint8_t *I;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s filein.pgm lowlambda lambda opening.pgm fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  opening = readimage(argv[4]);
	
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  N = rs * cs * ds;

  lambdaimage = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
  if (lambdaimage == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  distimage = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if (distimage == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lsedt_meijster(image, distimage))
  {
    fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
    exit(1);
  }

  if (!llambdamedialaxis(distimage, lambdaimage))
  {
    fprintf(stderr, "%s: llambdamedialaxis failed\n", argv[0]);
    exit(1);
  }

  I = UCHARDATA(image);
  L = FLOATDATA(lambdaimage);
  O = FLOATDATA(opening);
  max=0;
  for (i = 0; i < N; i++)
  {
    if (O[i] > max) max=O[i];	  
  }
  lambda = atof(argv[2]);
  lambda = (lambda*M_PI_4)/100;
  lowlambda = atof(argv[3]);
	  
  for (i = 0; i < N; i++)
  {
    //if (L[i] <= ((lowlambda/L[i])+((O[i]*lambda)/max))) // michal
    //if (L[i]*L[i] <= ((O[i]*lambda)/max))
    if (L[i] <  O[i]*tan(lambda))
    //if (L[i]*L[i] <  (O[i]*tan(lambda)*O[i]*tan(lambda)))
    //if (L[i] < (O[i] - (max - ((lambda*max)/M_PI_4)))) pas bon
    {
      I[i] = NDG_MIN;
    }
  }

  if(lowlambda>-1)
  {
    writeimage(image, argv[argc - 1]);
  }
  else
    writeimage(lambdaimage, argv[argc - 1]);

  freeimage(lambdaimage);
  freeimage(distimage);
  freeimage(image);
  freeimage(opening);

  return 0;
} /* main */


