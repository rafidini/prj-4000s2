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
/*! \file skelcurvfilter.c

\brief thresholds a skeleton map (hierarchy) to obtain a maximal curve skeleton without multiple points

<B>Usage:</B> skelcurvfilter in.pgm out.pgm

<B>Description:</B>
Thresholds a skeleton map (hierarchy) to obtain a maximal curve skeleton without junction points.
The pseudo-code is given below:

\verbatim
Input: F
Result: X
LMAX = 0
For all x in E
  Let f1,...,fk be the values of F for x and its k neighbors sorted in decreasing order
  If f4 > LMAX Then LMAX = f4
X = { x in E | F[x] > LMAX)
\endverbatim

<B>Types supported:</B> float 2d, float 3d

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
#include <mcgeo.h>
#include <ldist.h>
#include <lskeletons.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagebin;
  index_t rs, cs, ds, N, x;
  uint8_t *F;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lskelcurvfilter(image))
  {
    fprintf(stderr, "%s: lskelcurvfilter failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image); cs = colsize(image); ds = depth(image); N = rs * cs * ds; 
  imagebin = allocimage(image->name, rs, cs, ds, VFF_TYP_1_BYTE);
  if (imagebin == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  F = UCHARDATA(imagebin); 

  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    int32_t *FL = SLONGDATA(image);
    for (x = 0; x < N; x++) if (FL[x] == 0) F[x] = NDG_MIN; else F[x] = NDG_MAX;
  }
  else if (datatype(image) == VFF_TYP_2_BYTE)
  {
    int16_t *FL = SSHORTDATA(image);
    for (x = 0; x < N; x++) if (FL[x] == 0) F[x] = NDG_MIN; else F[x] = NDG_MAX;
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float *FL = FLOATDATA(image);
    for (x = 0; x < N; x++) if (FL[x] == 0.0) F[x] = NDG_MIN; else F[x] = NDG_MAX;
  }

  writeimage(imagebin, argv[argc-1]);
  freeimage(imagebin);
  freeimage(image);

  return 0;
} /* main */
