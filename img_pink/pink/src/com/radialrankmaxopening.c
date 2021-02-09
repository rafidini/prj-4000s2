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
/*! \file radialrankmaxopening.c

\brief radial rank-max opening by line

<B>Usage:</B> radialrankmaxopening in.pgm length rank [angle in degrees] [dilatmask.pgm] out.pgm

<B>Description:</B> Radial rank-max opening by line (or dilated
line). Max between the rank-max openings (with parameter \b rank) of
\b in.pgm by a linear structuring element rotated by angle until 360
degrees and dilated by dilatmask.pgm. The angle is normalized to an
exact division of 360 by an integer.

<B>Types supported:</B> byte 2d

<B>Category:</B> morpho
\ingroup  morpho

\author André Vital Saude, Michel Couprie jan 2005

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>
#include <ldilateros.h>
#include <ldraw.h>
#include <lsym.h>
#include <lfiltreordre.h>

//#define DEBUG 
//#define VERBOSE

#define DEFAULT_ANGLE 30.0

/* =============================================================== */
void drawline(struct xvimage *mask, int32_t length, double angle) 
/* =============================================================== */
{
  int32_t dx, dy, x1, y1, x2, y2;
  double rangle;

  rangle = (angle * M_PI) / 180;

  dx = (length/2) * cos(rangle);
  dy = (length/2) * sin(rangle);
  
  x1 = (length/2) - dx;
  y1 = (length/2) - dy;
  x2 = (length/2) + dx;
  y2 = (length/2) + dy;

  ldrawline(mask, x1, y1, x2, y2);

#ifdef VERBOSE
  printf("length=%d; angle=%f; rangle=%f; dx=%d; dy=%d; x1=%d; y1=%d; x2=%d; y2=%d \n", length, angle, rangle, dx, dy, x1, y1, x2, y2);
#endif

}

/* =============================================================== */
void rankmax_open_image(struct xvimage * img, struct xvimage * org_img, struct xvimage * mask, double r) 
/* rank_max open the org_image and write the result into img */
/* Adapter, that creates a copy of the original image */
/* and executes a dilation after an erosion on the copy */
/* Gets: a result_image, the original image, and a mask */
/* =============================================================== */
{	
#undef F_NAME
#define F_NAME "rankmax_open_image"
  // Copy org_img to img
  copy2image(img, org_img);

  //Rankfilter
  if (! lfiltreordre(img, mask, rowsize(mask)/2, colsize(mask)/2, 1.0 - r))
  {
    fprintf(stderr, "%s: function lfiltreordre failed\n", F_NAME);
    exit(1);
  }
  
  //As mask is a line dilated by an ordinary structuring element,
  //it is necessary:
  if (! lsym(mask, 'c')) {
    fprintf(stderr, "%s: function lsym failed\n", F_NAME);
    exit(1);
  }

  //Dilation
  if (! ldilateros_ldilat(img, mask, rowsize(mask)/2, colsize(mask)/2)) {
    fprintf(stderr, "%s: function ldilateros_ldilat failed\n", F_NAME);
    exit(1);
  }
  
  (void)lmin(img, org_img);

  return;
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image; 
  struct xvimage * opened_image; 
  struct xvimage * result_image;
  struct xvimage * mask;
  struct xvimage * dilatmask=NULL;

  index_t i, rs, cs, ds, N, Nmask;
  index_t sex, sey, sez;
  int32_t length, nangles; 
  uint8_t * mask_data;
  double rank;

  double angle=DEFAULT_ANGLE;

  if ((argc < 5) || (argc > 7))
  {
    fprintf(stderr, "usage: %s in.pgm length rank [angle in degrees] [dilatmask.pgm] out.pgm\n    0 <= rank <= 1 ; default angle: %f\n", argv[0], DEFAULT_ANGLE);
    exit(1);
  }

  // read parameters
  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  length = atoi(argv[2]);	// length of structuring element
  rank = atof(argv[3]);
  if ((rank < 0.0) || (rank > 1.0))
  {
    fprintf(stderr, "%s: rank = %g ; on doit avoir 0 <= rank <= 1\n", argv[0], rank);
    exit(1);
  }

  if (argc > 5)
  {
    angle = atof(argv[4]);
  }

  if (argc > 6)
  {
    dilatmask = readse(argv[5], &sex, &sey, &sez);
    if (dilatmask == NULL) {
      fprintf(stderr, "%s: readse failed\n", argv[0]);
      exit(1);
    }
  }

  rs = rowsize(image);      /* taille ligne */
  cs = colsize(image);      /* taille colonne */
  ds = depth(image);        /* nombre plans */
  N = rs * cs * ds;         /* taille image */

  length = (length/2) * 2 +1; 	// has to be 2*x+1 (ungerade)
  Nmask = length*length;
  printf("Length is: %i\n", length);
 
  mask = allocimage(NULL, length, length, ds, datatype(image));
  mask_data = UCHARDATA(mask);
  opened_image = allocimage(NULL, rs, cs, ds, datatype(image));
  result_image = allocimage(NULL, rs, cs, ds, datatype(image));

  nangles = (int32_t)(360.0/angle);
  angle = 360.0 / (double)nangles;

#ifdef VERBOSE
  printf("angle=%f; nangles=%d\n", angle, nangles);
#endif

  // For every angle
  for(i=0; i<nangles; i++){
    //reset mask_data
    memset(mask_data, 0, Nmask);
#ifdef VERBOSE
    printf("%d: ", i);
#endif
    drawline(mask, length, angle*(double)i);
    if (dilatmask != NULL) {
      ldilateros_ldilat(mask, dilatmask, sex, sey);
    }
    rankmax_open_image(opened_image, image, mask, rank);
    if (! lmax(result_image, opened_image)) {
      fprintf(stderr, "%s: function lmax failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(result_image, argv[argc-1]);
#ifdef DEBUG
  writeimage(mask, "/tmp/mask.pgm");
#endif
  freeimage(mask);
  freeimage(opened_image);
  freeimage(result_image);
  freeimage(image);
  
  return 0;
} /* main */
