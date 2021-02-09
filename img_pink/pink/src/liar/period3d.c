/*
 * File:		period3d.c
 *
 Hugues Talbot	 7 Dec 2010

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
 *
*/

/* period3d.c */
/* generates a 3 dimensional periodic line */
#include <math.h>
#include <stdlib.h>

#include "liarp.h"


/**
 * \brief generates a 3 dimensional periodic line

    This function creates a periodic line with an aspect ratio as specified
    by the x, y and z dimensions (dx, dy and dz).  The line returned is of
    length 'ol' equal to the number of periods required to cover the image in
    the dominant dimension of the direction vector.  All values within the
    returned line vector are in the form of offsets for the original image,
    with the first such offset being set to 0.  All lines are returned heading
    right from the origin.
    Note that the vector 2 2 2 will not return the same result as 1 1 1 since
    the period of the first will be two whilst that of the latter will be 1.

 * \param dx:        x dimension of the line
 * \param dy:        y dimension of the line
 * \param dz:        z dimension of the line
 * \param imwidth:   height of the image (x dimension)
 * \param imheight:  length of the image (y dimension)
 * \param imdepth:   depth of the image (z dimension)
 * \param *ol:       length of the allocated vector
 * \param *period:   the period of the line

 * \return line offsets
 * \author Ian Sowden
 * <br> Modified by Annick LeHegarat, 16 July 2004. Increased length of line by one
    to better suit glineminmax3d(). The last value p[ol-1] gets read within algorithm
    even though the operation is not performed for that value of end.
 * \date 7 Jan 1997
*/
long *periodic3d(int dx, int dy, int dz,
		      int imwidth, int imheight, int imdepth,
		      int *ol, int *period)
{
  long      *p, offset;
  int       i;
  int       absdx, absdy, absdz;
  int       nshift;

  /* set absolute values */
  absdx = abs(dx);
  absdy = abs(dy);
  absdz = abs(dz);

  /* if dx is negative then reflect vector through origin */
  if (dx<0) {
    dx = -dx;
    dy = -dy;
    dz = -dz;
  } /* end if */

  if ( (absdx >= absdy) && (absdx >= absdz) ) {

    /* line repeats after each dx */
    *period = absdx;

    if (*period == 0)
      return NULL;

    /* determine number of dx's required to cover image width */
    nshift=(imwidth)/(*period);

  } /* end if vector closest to the x axis */
  else if ( (absdy >= absdx) && (absdy >= absdz) ) {

    /* line repeats after each dy */
    *period = absdy;

    if (*period == 0)
      return NULL;

    /* determine number of dy's required to cover image height */
    nshift=(imheight)/(*period);

  } /* end if closest to the y axis */
  else { /* vector must be closest to the z axis */

    /* line repeats after each dz */
    *period = absdz;

    if (*period == 0)
      return NULL;

    /* determine number of dz's required to cover image depth */
    nshift=(imdepth)/(*period);

  } /* end else closest to the z axis */

  /* set the length of the line */
  *ol = nshift+1;

  /* allocate mem */
  p = (INT4_TYPE*)calloc(*ol, sizeof(INT4_TYPE));
  if (p == NULL) {
    LIARerror("periodic3d: Unable to allocate memory for line offsets");
    return(NULL);
  }

  /* update p with the offset for each dx/dy/dz (as applicable) */
  offset=0;
  for(i=0; i<*ol; ++i, offset+=(dz*imwidth*imheight)+(dy*imwidth)+dx)
    p[i]=offset;

  /* return offsets */
  return(p);

} /* end periodic3d */


