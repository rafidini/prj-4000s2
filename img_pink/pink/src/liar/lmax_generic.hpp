/**
 * trivial pixel-wise minimum 
 *
 */

#ifndef MAX_GENERIC_H
#define MAX_GENERIC_H

#include "liarp.h"

/** \brief takes the infinum of two images 
 *
 */
template <typename Type>
int li_max(Type *in1,	 /**< [in] pointer to first input image */
	   Type *in2,	 /**< [in] pointer to second input image */
	   Type *out,	 /**< [out] pointer to output image */
	   long nbpix)	 /**< [in] size of input & output images (in pixels) */

{
    Type 	*pi1, *pi2, *po, *pend;

    pi1 = in1;
    pi2 = in2;
    po = out;
    pend = po+nbpix;
    while (po < pend) {
      *po++ = (*pi1 < *pi2) ? *pi2:*pi1;
      pi1++;
      pi2++;
    }
    
    return 0;
}

#endif // MAX_GENERIC_H

