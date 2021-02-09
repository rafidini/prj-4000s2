/**
 * non-generic 1D median along an arbitrary line
 * 
 * written by Ed. Breen 1994
 *
 *
*/

#ifndef LGRANK_GENERIC_HPP
#define LGRANK_GENERIC_HPP

#include "liarp.h"                   

template <typename Type>
void gline( Type *IN,      /**< [in] input image */
            Type *OUT,     /**< [out] output image */
            int nx,        /**< [in] number of columns */
            int ny,        /**< [in] number of lines */
            int k,         /**< [in] horizontal length of the line */
            double th,     /**< [in] the angle of the line */
            double rank    /**< [in] the rank */);


/* 
 *
 *  A 1D running median/rank routine written by Edmond J. Breen 17/5/94
 *  Developed from Huang et-al. (1979) PAMI 13-18.
 *   modifications: (1) Works upto the border of the input.
 *                  (2) Is a rank filter rather than just
 *                        the median. 
 *
 */

static INT4_TYPE *_GhIsT=0, _GrEy_ScAlEs=0;
static double rank;

static void open_running_rank(INT4_TYPE no_of_grey_scales)
{
    /* calloc() args swapped */
    _GhIsT = (INT4_TYPE *)calloc(no_of_grey_scales,sizeof(INT4_TYPE));
    _GrEy_ScAlEs = no_of_grey_scales;
}

static void close_running_rank(void)
{
    free(_GhIsT);
}

static void set_rank(double r)
{
    rank = r;
}


template <typename Type>
void generic_running_rank(Type * in, 
			  Type * out,
			  INT4_TYPE * offset,
			  int nx, 
			  int nk,
			  double rank
			  )
{
    /* in and out: input and output arrays.
     * offset:  array used for indirect indexing.
     * nx: length of in and out offset arrays.
     * nk  extent of filter.
     * rank:
     */
    INT4_TYPE j,i,hk,tk,tsum,hsum,lsum;

    Type mdn=0,g1,g2;
    hk = (nk>>1) < nx? (nk>>1) : nx -1;
    tk = nx - hk;

    /* set up histogram */
    memset(_GhIsT,0,_GrEy_ScAlEs * sizeof(INT4_TYPE));


     for(j=0;j<=hk;j++)  
	_GhIsT[*(in + offset[j])]++;

    /* do left border pixels */
    /* do first pixel */
    tsum = hk + 1;
    hsum = (tsum-1)*rank;
    for(lsum=i=0;i<_GrEy_ScAlEs;i++) {
	lsum += _GhIsT[i];
	if((lsum >= hsum) && lsum) {
	    mdn = (Type)i;
	    lsum -= _GhIsT[i];
	    break;
	}
    }

    out[offset[0]] = mdn;
    /* do remainder of left border pixels */
    for(i=1;i<=hk;i++,j++) {
	if(j<nx) {
	    g2 = *(in + offset[j]);	/* right border */
	    _GhIsT[g2]++;
	    tsum++;
	    hsum = (tsum-1)*rank;
	    if(g2 < mdn) 
		lsum++;
	    if(lsum > hsum) 
		do {
		    mdn--;
		    lsum -= _GhIsT[mdn];
		}while(lsum > hsum);
	    
	    else while(lsum + _GhIsT[mdn] <= hsum) {
		lsum += _GhIsT[mdn];
		mdn++;
	    }
	}
	out[offset[i]] = mdn;
    }


    /* do middle pixels */
    for(j=0;i<tk;i++,j++) {
	if(j+nk < nx) {
	    g1 = *(in + offset[j]);	/* left border */
	    g2 = *(in + offset[j+nk]); /* right border */
	    _GhIsT[g1]--;
	    if(g1 < mdn)
		lsum--;
	    
	    _GhIsT[g2]++;
	    if(g2 < mdn)
		lsum++;
	    
	    if(lsum > hsum) 
		do {
		    mdn--;
		    lsum -= _GhIsT[mdn];
		}while(lsum > hsum);
	    
	    else while(lsum + _GhIsT[mdn] <= hsum) {
		lsum += _GhIsT[mdn];
		mdn++;
	    }
	}
	out[offset[i]] = mdn;

    }

    /* do right pixels */
    for(;i<nx;i++,j++) {
	if(j < nx) {
	    g1 = *(in + offset[j]);
	    tsum--;
	    hsum = (tsum-1)*rank;
	    _GhIsT[g1]--;
	    if(g1 < mdn)
		lsum--;

	    if(lsum > hsum) 
		do {
		    mdn--;
		    lsum -= _GhIsT[mdn];
		}while(lsum > hsum);
	
	    else while(lsum + _GhIsT[mdn] <= hsum) {
		lsum += _GhIsT[mdn];
		mdn++;
	    }
	}
	out[offset[i]] = mdn;
    }
}


/** \brief Compute a rank filter along an arbitrary 1D line 
 *
 */
template <typename Type>
void gsrank(Type *IN,	/**< [in] pointer to input image */
	    Type *OUT,	/**< [out] pointer to output image */
	    int       nx,	/**< [in] no. of pixels in input image */
	    int       ny,	/**< [in] no. of lines in input image */
	    int       nk,	/**< [in] no. of pixels in template */
	    double    angle,	/**< [in] angle of line */
	    double    rank)	/**< [in] rank */
{
    open_running_rank(256);
    set_rank(rank);

    /* angle has to be between -90 and +90 for some reason */
    while (angle >= 90 )
	angle -= 180;
    while (angle < -90)
	angle += 180;
    
    gline(IN,OUT,nx,ny,nk, angle,rank);

    close_running_rank();

}


#endif // LGRANK_GENERIC_HPP
