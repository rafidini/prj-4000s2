/*
 * File:		lfopen.hpp
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
same conditions as regards security.h

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.

 *
*/

/* lfopen.hpp */
/* opening by 3 dimensional structuring elements */

#ifndef LFOPEN_HPP
#define LFOPEN_HPP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "liarp.h"
#include "fseries.hpp"


template <typename Type>
int lfopen_rect(Type *inbuf, Type *outbuf,int ncol, int nrow, int dimx, int dimy)
{
  char LIARstrbuf[1024];

  sprintf(LIARstrbuf, "Opening by a rect (char version), %d x %d x ", dimx, dimy);
  LIARdebug(LIARstrbuf);

  /* if not writing to same buffer then copy contents of original */
  if (outbuf != inbuf)
    memcpy(outbuf, inbuf, ncol*nrow);

  /* erode then dilate */
  rectminmax(outbuf, ncol, nrow, dimx, dimy, computemin); // compute minimum
  rectminmax(outbuf, ncol, nrow, dimx, dimy, computemax); // compute maximum

  /* all done */
  return(0);

} /* end lfopen_rect */


template <typename Type>
int lfopen_line(Type *inbuf, Type *outbuf, int ncol, int nrow, int length, int angle, int type)
{
  char LIARstrbuf[1024];
  int LIARerr;

  //sprintf(LIARstrbuf, "Opening by a line, length: %d, dx: %d, dy: %d,  type: %d", length, dx, dy, type);
  LIARdebug(LIARstrbuf);

  if (outbuf != inbuf)
    memcpy(outbuf, inbuf, ncol*nrow);

  if (type != PERIODIC) {
    LIARerr = glineminmax(outbuf, ncol, nrow, length, angle,
		  computemin, computebresen);
    LIARerr = glineminmax(outbuf, ncol, nrow, length, angle,
		  computemax, computebresen);
  }
  else {
    LIARerr = glineminmax(outbuf, ncol, nrow, length, angle,
		  computemin, computeperiod);
    LIARerr = glineminmax(outbuf, ncol, nrow, length, angle,
		  computemax, computeperiod);
  }

  return(LIARerr);

} /* end lfopen_line */



#endif // LFLOPEN_HPP

