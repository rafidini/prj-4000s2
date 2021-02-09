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
/*! \file surf.c

\brief topographic view of a 2D image

<B>Usage:</B> surf in.pgm mode out.pgm

<B>Description:</B>
Creates a gray-scale image
with shading view 2D gray-scale image as a
topographical surface. Useful to view
distance transforms and results of morphological
operators.

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Roberto Lotufo
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* ==================================== */
int32_t lsurf(struct xvimage * image, int32_t mode)
/* ==================================== */
#undef F_NAME
#define F_NAME "lsurf"
{
  int32_t i, j;
  int32_t rs, cs, N;
  double  vx;             /*+ In: illumination direction, x direction      +*/
  double  vy;             /*+ In: illumination direction, y direction      +*/
  double  vz;             /*+ In: illumination direction, z direction      +*/
  double *img_gv, *img_gh, *img_gz;

  if (depth(image) != 1)
  {
    fprintf(stderr, "%s: only 2D images\n", F_NAME);
    return 0;
  }

  rs = rowsize(image);
  cs = colsize(image);
  N = rs * cs;
  
  vz = 1.0;
  switch (mode)
  {
    case 0: vx = 1.0; vy = 0.0; break;
    case 1: vx = 1.0; vy = -1.0; break;
    case 2: vx = 0.0; vy = -1.0; break;
    case 3: vx = -1.0; vy = -1.0; break;
    case 4: vx = -1.0; vy = 0.0; break;
    case 5: vx = -1.0; vy = 1.0; break;
    case 6: vx = 0.0; vy = 1.0; break;
    case 7: vx = 1.0; vy = 1.0; break;
  } // switch (mode)

  img_gv = (double *)calloc(1,N * sizeof(double));
  img_gh = (double *)calloc(1,N * sizeof(double));
  img_gz = (double *)calloc(1,N * sizeof(double));
  if ((img_gv == NULL) || (img_gh == NULL) || (img_gz == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    return 0;
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */  

  /* img_gv is normalized vertical gradient */ ???????????????????
  /* img_gh is normalized horizontal gradient */
  /* determine gz = 1/(gv^2 + gh^2 + 1) */
  /* gv = gv*gz */
  /* gh = gh*gz */
  v = sqrt(vx*vx + vy*vy + vz*vz);
  vx = vx/v; vy = vy/v; vz = vz/v;
  /* gz = vz*gz + vx*gv + vy*gh */
  /* clip on negative values */ 
  /* gz > 0 */
  /* convert to uint8 */

  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(image);
    double tmp, mx;
    mx = -1;
    for (j = 0; j < cs; j++)
    {
      for (i = 1; i < rs; i++)
      {
	tmp = (double)F[j*rs + i] - (double)F[j*rs + i-1];
	tmp = abs(tmp);
	if (tmp > mx) mx = tmp; 
	img_gh[j*rs + i-1] = tmp;
      }
      img_gh[j*rs + i-1] = 0.0;
    }

















  }
  else
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  free(img_gv);
  free(img_gh);
  free(img_gz);
  return 1;
} /* lsurf() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t mode;
  struct xvimage * image;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm mode fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[2]);
  if ((mode < 0) || (mode > 7))
  {
    fprintf(stderr, "usage: %s filein.pgm mode fileout.pgm\n", argv[0]);
    fprintf(stderr, "       mode = 0 (E), 1 (NE), 2 (N), 3 (NW), 4 (W), 5 (SW), 6 (S), 7 (SE)\n");
    exit(1);
  }

  if (! lsurf(image, mode))
  {
    fprintf(stderr, "%s: lsurf failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


