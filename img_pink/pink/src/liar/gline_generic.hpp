/**
 * generic 1D median along an arbitrary line
 *
 */

#ifndef GLINE_GENERIC_HPP
#define GLINE_GENERIC_HPP

#include "liarp.h"

/* What follows is the exact copy of gline.c. No trick in there */
static void bres2dxy(int x1,int y1,
		     int x2,int y2,
		     int *xcoor, int *ycoor)
{
    int dx, dy,ix,iy,plotx,ploty,x,y,i,inc,plot,sdx,sdy;
  
    dx = x2 - x1;
    dy = y2 -y1;
    ix = abs(dx);
    iy = abs(dy);
    inc = ix > iy ? ix : iy;
    sdy = dy > 0 ? 1: dy == 0? 0: -1;
    sdx = dx > 0 ? 1: dx == 0? 0: -1;
  
  
    plotx = x1;
    ploty = y1;
  
    x = y = 0;
  
    *xcoor++ = plotx;
    *ycoor++ = ploty;
  
    for(i=0;i<=inc; i++) {
	x += ix;
	y += iy;
	plot = 0;
	if(x > inc) {
	    plot = 1;
	    x -= inc;
	    plotx +=  sdx;
	}
	if(y > inc) {
	    plot = 1;
	    y -= inc;
	    ploty += sdy;
	}
	if(plot){
	    *xcoor++ = plotx;
	    *ycoor++ = ploty;
	}
    }
}

#ifndef liarmax
#define liarmax(x,y)       ((x)>(y)?(x):(y))
#endif

#ifndef degree2rad
#define degree2rad(x)  ((x)*0.017453293)
#endif

template <typename Type>
void gline(Type *IN,      /**< [in] input image */
	   Type *OUT,     /**< [out] output image */
	   int nx,        /**< [in] number of columns */
	   int ny,        /**< [in] number of lines */
	   int k,         /**< [in] horizontal length of the line */
	   double th,     /**< [in] the angle of the line */
	   double rank    /**< [in] the rank */)
{
    /*
      Code taken from gline.c and slightly modified written by Edmond J. Breen.
      Header for gline1.c (This generised function works for short, double and       int images).
     */		   
    
    int *xcoor, *ycoor;
    int incx, incy;
    int *p,kth;
    int xp, yp, nshft;
    int i, j;
    int sx,sy,ex,ey;		/* start and end points */
    
    
    if(th < -90.0 || th > 90.0)
        return;
    

    kth = (k * liarmax(fabs(cos(degree2rad(th))),fabs(sin(degree2rad(th)))) +0.5);
    kth += !(kth%2); /* add one if kth is even */

    /* kth = ((int)(dbl/2 +0.5) * 2 + 1); */
    
    /* fprintf(stderr, "angle:%g kth = %d\n", th, kth); */
    
    if (fabs(th) <= 45.0){	/* set up offsets to scan image top to bottom */
	incx = 0;
	incy = 1;
	nshft = nx;
        /* calloc() args swapped */
	xcoor = (int*)calloc(nx, sizeof(int));
	ycoor = (int*)calloc(nx, sizeof(int));
	yp = ((double)(nx-1)*tan(th*0.0174533));

	bres2dxy(0, 0, nx-1, yp, xcoor, ycoor);
	if (yp > 0){
	    for (i=0; i<nx; i++)
		ycoor[i] -= yp;
	}
    }
    else {			/* set up offsets to scan image left to right */
	incx = -1;
	incy = 0;
	nshft = ny;
        /* calloc() args swapped */
	xcoor = (int*)calloc(nshft, sizeof(int));

	ycoor = (int*)calloc(nshft, sizeof(int));
	xp = ((double)(ny-1)*tan((90.0-fabs(th))*0.0174533));
	yp = th < 0 ? (-ny+1):(ny-1);
	bres2dxy(0, 0, xp, yp, xcoor, ycoor);

	if (yp > 0){		/* translate the line to the end of the x axis */
	    for (i=0; i<ny; i++)
		xcoor[i] += (nx-1);
	}
	else {	
	    for(i=0; i<ny; i++){
		xcoor[i] += (nx-1);
		ycoor[i] += (ny-1);
	    }
	}
    }
    
    /* calloc() args swapped */
    p = (int*)calloc(nshft, sizeof(int));

    for (i=0; i<nshft; i++)
	p[i] = xcoor[i] + ycoor[i]*nx;

    sx = 0;
    sy = 0;
    ex = nx;
    ey = ny;
    while (1){
	int sp=0;			/* start point */

        for (j=i=0; i<nshft; i++){ /* determine length and start */
	    /* position */
            if (xcoor[i]>=sx && xcoor[i]<ex 
		&& ycoor[i]>=sy && ycoor[i]<ey){
		if(!j++)
		    sp = i;
            }
        }
        if (j==0)
            break;
	
        generic_running_rank(IN,		/* input array. Where 1st member is *(f+p[0]) and */
			     /* 2nd member is = *(f+p[1]) etc. */
			     OUT,
			     &p[sp],		/* beginning of array of offsets */
			     j,			/* number of offsets */
			     kth, rank);       	/* kernel length and rank */
	
	if(incx != 0) {
	    sx ++;
	    ex ++;
	    IN--;
	    OUT--;
	} else {
	    sy -= incy;
	    ey -= incy;
	    IN += nx;
	    OUT +=nx;
	    
	}
	    
    }
    
    free(xcoor);
    free(ycoor);
    free(p);
    
    
}


#endif // GLINE_GENERIC_HPP
