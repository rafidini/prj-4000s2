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
/*! \file integermedialaxis.c

\brief integer medial axis transform

<B>Usage:</B> integermedialaxis in.pgm [gamma] out.pgm

<B>Description:</B>
Integer medial axis, as defined in [HR05],
of the binary image \b X contained in \b in.pgm.

References:<BR>
[HR05] "Euclidean Skeletons of 3D Data Sets in Linear Time
by the Integer Medial Axis Transform",
W.H. Hesselink and B.T.M. Roerdink,
Computational Imaging and Vision, Vol. 30,
Mathematical Morphology: 40 Years On, Springer, 2005, pp. 259-268

<B>Types supported:</B> byte 2d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>
#include "lvoronoilabelling.h"

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * resf;
  double gamma;
  uint32_t i, N;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.pgm [gamma] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  resf = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_FLOAT);
  if (resf == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (!lmedax_Hesselink(image, resf))
  {
    fprintf(stderr, "%s: lmedialaxis failed\n", argv[0]);
    exit(1);
  }

  if(argc==3)
  {
  	writeimage(resf, argv[argc - 1]);
  }
  else
  {
  	gamma = atof(argv[2]);
	if (gamma < 1)
	{
		fprintf(stderr, "%s: gamma must be greater than 1\n", argv[0]);
		exit(1);
	}

	N = rowsize(image) * colsize(image) * depth(image);
	for(i=0; i<N; i++)
	{
		if(FLOATDATA(resf)[i] < gamma)
			UCHARDATA(image)[i] = NDG_MIN;
	}

	writeimage(image, argv[argc - 1]);
  }

  freeimage(image);
  freeimage(resf);

  return 0;
} /* main */
