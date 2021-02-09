/*
 * File:		lferode.hpp
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

/* lferode.hpp */

#ifndef LFERODE_HPP
#define LFERODE_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "liarp.h"
#include "fseries.hpp"


/**
 * \brief erode a 3d image by a 3d rectangle

   NOTE: change the name
*/
template <typename Type>
int lferode_rect(Type *inbuf,       /* pointer to input image */
                 Type *outbuf,      /* pointer to mask image */
                 int ncol, int nrow, int dimx, int dimy)

{
  char LIARstrbuf[1024];

  sprintf(LIARstrbuf, "Erosion by a 3d rect (version char), %d x %d x ", dimx, dimy);
  LIARdebug(LIARstrbuf);

  /* if out buffer is different to in buffer then copy contents */
  if (inbuf != outbuf)
    memcpy(outbuf, inbuf, ncol*nrow);

  /* set each voxel (in outbuf) to min within 3d rect (ie erode) */
  rectminmax(outbuf, ncol, nrow, dimx, dimy, computemin);

  return(0);

} /* end lferode_rect */




/**
 * \brief erode a 3d image by a 3d line
*/
template <typename Type>
int lferode_line(Type *inbuf, Type *outbuf, int ncol, int nrow, int length, int angle, int type)
{
  char LIARstrbuf[1024];
  int LIARerr=0;

  //sprintf(LIARstrbuf, "Erosion by a line, length: %d, dx: %d, dy: %d,  type: %d", length, dx, dy, type);
  LIARdebug(LIARstrbuf);

  if (outbuf != inbuf)
    memcpy(outbuf, inbuf, ncol*nrow);

  /* erode by a bresenham or periodic line */
  if (type != PERIODIC) {
    LIARerr= glineminmax(outbuf, ncol, nrow, length, angle,
		  computemin, computebresen);
  } /* end if */
  else {
    LIARerr = glineminmax(outbuf, ncol, nrow, length, angle,
		  computemin, computeperiod);
  } /* end else */

  return LIARerr;

}


#endif // LFLERODE_HPP
