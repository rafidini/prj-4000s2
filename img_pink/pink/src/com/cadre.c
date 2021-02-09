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
/* \file cadre.c

OBSOLETE - voir frame.c

\brief generates an image with a white border and a black interior

<B>Usage:</B> cadre in.pgm out.pgm

<B>Description:</B>
The result out.pgm has the same size as in.pgm. Its border is set to 255, 
all other pixels are set to 0.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  uint8_t *Im;
  int32_t rs, cs, ds, ps, N, x, y, z;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein.pgm fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  N = ps * ds;
  Im = UCHARDATA(image);
  memset(Im, 0, N);

  if (ds > 1)
  {
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      Im[0 * ps + y * rs + x] = NDG_MAX;          /* plan z = 0 */
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      Im[(ds-1) * ps + y * rs + x] = NDG_MAX;     /* plan z = ds-1 */

    for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + 0 * rs + x] = NDG_MAX;          /* plan y = 0 */
    for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + (cs-1) * rs + x] = NDG_MAX;     /* plan y = cs-1 */

    for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + y * rs + 0] = NDG_MAX;          /* plan x = 0 */
    for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + y * rs + (rs-1)] = NDG_MAX;     /* plan x = rs-1 */
  }
  else
  {
    for (x = 0; x < rs; x++) Im[x] = NDG_MAX;
    for (x = 0; x < rs; x++) Im[(cs - 1) * rs + x] = NDG_MAX;

    for (y = 1; y < cs - 1; y++) Im[y * rs] = NDG_MAX;
    for (y = 1; y < cs - 1; y++) Im[y * rs + rs - 1] = NDG_MAX;
  }

  /* ---------------------------------------------------------- */
  /* fabrique le nom de l'image resultat */
  /* ---------------------------------------------------------- */

  image->name = NULL;
  writeimage(image, argv[2]);
  freeimage(image);

  return 0;
} /* main */



