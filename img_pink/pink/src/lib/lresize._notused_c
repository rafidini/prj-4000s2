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
/* zoom par un facteur rationnel < 1 */
/* Michel Couprie - decembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lzoom.h>

/* ==================================== */
int32_t lzoom(
  struct xvimage * in,
  struct xvimage ** out,
  double z)
/* ==================================== */
{
  int32_t x, y, x1, y1, xn, yn, xx, yy;
  uint8_t *ptin;
  uint8_t *ptout;
  int32_t rs, cs;
  int32_t rs2, cs2;
  double k, tmp, d, dx1, dxn, dy1, dyn, sigmad;

  if (depth(in) != 1) 
  {
    fprintf(stderr, "lzoom: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  rs = in->row_size;
  cs = in->col_size;
  ptin = UCHARDATA(in);

  if ((z >= 1.0) || (z <= 0.0))
  {   fprintf(stderr,"lzoom() : bad zoom factor : %g - must be < 1\n", z);
      return 0;
  }

  rs2 = (int32_t)(rs * z);
  cs2 = (int32_t)(cs * z);
  k = 1.0 / z;
  return 1;
}
