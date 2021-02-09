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
/*! \file toposhrink.c

\brief topologically controled binary shrinking guided by a priority image

<B>Usage:</B> toposhrink in.pgm prio.pgm connex tmin tmax tbmin tbmax [inhibit] out.pgm

<B>Description:</B>
Topologically controled binary shrinking guided by a priority image.
The lowest values of the priority image correspond to the highest priority.

The parameter \b prio is an image (byte or int32_t).

The parameter \b connex indicates the connectivity of the binary object.
Possible choices are 4, 8 in 2D and 6, 18, 26 in 3D.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image (set Y) will be left unchanged. 

Let X be the set of points of the binary image \b in.pgm .

\verbatim
Repeat until stability: 
    select a point p of X \ Y such that
        tmin <= T(p) <= tmax and tbmin <= Tb(p) <= tbmax 
        and with the lowest possible priority value
    X := X \ { p }
\endverbatim

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lskeletons.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * prio;
  struct xvimage * prio2;
  int32_t connex;
  struct xvimage * inhibimage = NULL;
  int32_t tmin, tmax, tbmin, tbmax;

  if ((argc != 9) && (argc != 10))
  {
    fprintf(stderr, "usage: %s in.pgm prio.pgm connex tmin tmax tbmin tbmax [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  prio2 = readimage(argv[2]);
  if (prio2 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (datatype(prio2) == VFF_TYP_1_BYTE)
  {
    int32_t rs = rowsize(prio2);
    int32_t cs = colsize(prio2);
    int32_t ds = depth(prio2);
    int32_t N = rs * cs * ds;
    uint8_t *B = UCHARDATA(prio2);
    int32_t *L;
    int32_t x;
    prio = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    if (prio == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    L = SLONGDATA(prio);
    for (x = 0; x < N; x++) L[x] = (int32_t)B[x];
    freeimage(prio2);
  }
  else if (datatype(prio2) == VFF_TYP_4_BYTE)
  {
    prio = prio2;
  }
  else
  {
    fprintf(stderr, "%s: bad datatype for prio\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  tmin = atoi(argv[4]);
  tmax = atoi(argv[5]);
  tbmin = atoi(argv[6]);
  tbmax = atoi(argv[7]);

  if (argc == 10) 
  {
    inhibimage = readimage(argv[8]);
    if (inhibimage == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (depth(image) == 1)
  {
    if (! ltoposhrink(image, prio, connex, tmin, tmax, tbmin, tbmax, inhibimage))
    {
      fprintf(stderr, "%s: ltoposhrink failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! ltoposhrink3d(image, prio, connex, tmin, tmax, tbmin, tbmax, inhibimage))
    {
      fprintf(stderr, "%s: ltoposhrink3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(prio);
  if (inhibimage) freeimage(inhibimage);

  return 0;
} /* main */
