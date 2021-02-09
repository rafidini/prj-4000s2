/*
 * File:		lfdilate_poly_generic.h
 *
 * Written by:		Image Analysis Group staff,
 * 			CSIRO Mathematical and Information Sciences.
 *
 * Date:		March 2001
 *
 *
 * CSIRO Mathematical and Information Sciences is the owner of all
 * copyright subsisting in the software contained in this file. It may
 * not be disclosed to, or used by, anyone without the prior approval
 * of the Chief, CSIRO Mathematical and Information Sciences.
 *
*/

#ifndef LFERODE_POLY_HPP
#define LFERODE_POLY_HPP

//#include "liarlmts.h"
//#include "liarp.h"
#include "liarp.h"
//#include "fseries.hpp"

template <typename Type>
int lferode_poly (const Type *inbuf, Type *outbuf, int ncol, int nrow, int radius, int type, int sides)
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
        LIARerr = circleminmax(outbuf, ncol, nrow, radius, computemin, computeperiod, sides/2);
    } else {
        LIARerr = circleminmax(outbuf, ncol, nrow, radius, computemin, computebresen, sides/2);
    }
    return 0; // means OK
}


#endif // LFLERODE_POLY_HPP











