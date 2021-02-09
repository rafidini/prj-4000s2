/*
 * File:		lfdilate_poly_generic.h
 *
 * C version orginally from CSIRO MIS, 2001.
 * 
 * C++ generic version:
 * Hugues Talbot, LIGM, Université Paris Est
 * March 2012.
 *
*/

#ifndef LFDILATE_POLY_HPP
#define LFDILATE_POLY_HPP

#include "liarp.h"

/** \brief Fast flat dilation by a polygon, approximating a disk
 *
 * This computes a dilation by a polygon with a specified number of sides
 * allowing for a good approximation of disks.
 * The computation is in constant time irrespective of the diameter of the disk,
 * Given a fixed number of sides. If the number of sides is zero, then the function
 * adapts the number of sides of the polygon to the radius, up to a maximum of 12 sides.
 *
 * Hugues Talbot 2012-03-02
 * 
 */

template <typename Type>
int lfdilate_poly (const Type *inbuf, /**< [in] Input buffer */
		   Type *outbuf, /**< [out] result buffer (can be same as input) */
		   int ncol, /**< [in] number of columns (x dimension) of the input image */
		   int nrow, /**< [in] number of lines (y dimension) of the input */
		   int radius, /**< [in] radius of the polygon */
		   int type,  /**< [in] type of line used : 0 = periodic ; 1 = Bresenham */
		   int sides) /**< [in] number of sides of the polygon. May be zero */
{
    char LIARstrbuf[1024];
    int LIARerr;

    if (sides == 0) {
	if (radius<=3)
	    sides=4;
	else
	    if (radius<=8)
		sides=8;
	    else
		if (type)
		    sides = 8;
		else
		    sides = 12;
    }

    sprintf(LIARstrbuf, "Dilation by a polygone, %d sides, radius= %d, type= %d", sides, radius, type);
    LIARdebug(LIARstrbuf);


    if (inbuf != outbuf)
        memcpy(outbuf, inbuf, ncol*nrow);

    if (type == PERIODIC) {
	LIARerr = circleminmax(outbuf, ncol, nrow, radius, computemax, computeperiod, sides/2);
    } else {
	LIARerr =circleminmax(outbuf, ncol, nrow, radius, computemax, computebresen, sides/2);
    }
    return 0; // means OK
}


#endif // LFLDILATE_POLY_HPP











