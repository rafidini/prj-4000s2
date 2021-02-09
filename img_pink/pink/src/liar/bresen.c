/*
 * File:		bresen.c
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

/* bresen.c,v 1.1.1.1 1996/09/04 07:37:11 talbot Exp */

/*
 * converted into a stand-alone C file by Hugues Talbot	13 Mar 1995
 */

#include <math.h>

#ifndef M_PI
# define M_PI		3.14159265358979323846	/* pi */
#endif /* M_PI */

#include <stdlib.h>

//#include "liarlmts.h"
#include "liarp.h"

long *bresenham(int xin,int yin,int angle,int width,int *period)

{
/**
ARGUMENTS:
	int xin: x starting coordinate
	int yin: y starting corrdinate
	int angle: angle of the line in degrees.
	int width: width of the line; that is, width of image.
	int *period: value placed in here is the period of the line (always
	             1 for the Bresenham line)
RETURN VALUE:
	INT4_TYPE *
DISCRIPTION:
	This function makes a Bresenham line of a given width.
	The points in the line are offsets. The period returned
	is always one (either this function or periodic is called;
	periodic returns a period that may not be one).
HISTORY:
        Written by Ed Breen, modified by Ronald Jones.
TESTS:
        Ran lint and purify.
**/

    long *p,*backp,offset;
    int i,dl,sdl,ixy,x,y,plot,pixposn;
    double angrad,angrad2;

    *period=1;                /* period of a Bresenham line */
    angrad=(double)angle*0.0174533;
    angrad2=(double)(90-angle)*0.0174533;
    /* calloc() args swapped */
    p=(long *)calloc(width+1, sizeof(long));
    backp=p;

    if (abs(angle)<=45)
        dl=(int)(tan(angrad)*(double)width);
    else
        dl=(int)(tan(angrad2)*(double)width);

    ixy=abs(dl);
    sdl=dl>0?1:dl==0?0:-1;
    x=y=0;
    pixposn=xin*yin*width;
    *p++=pixposn;

    if (abs(angle)<=45) {
        for(i=0;i<=width;i++) {
            x+=width;
            y+=ixy;
            plot=0;
            if (x>width) {
                pixposn++;
                x-=width;
                plot=1;
            }
            if (y>width) {
                pixposn+=sdl*(width+1);
                y-=width;
                plot=1;
            }
            if (plot)
                *p++=pixposn;
        }
    }
    else {
        for(i=0;i<=width;i++) {
            y+=width;
            x+=ixy;
            plot=0;
            if (y>width) {
                y-=width;
                plot=1;
            }
            if (x>width) {
                pixposn+=sdl;
                x-=width;
                plot=1;
            }
            if (plot)
                *p++=pixposn;
        }
    }

    if (angle>0) {
        if (angle>45)
            offset=backp[width];
        else
            offset=backp[width]-width;
        for(i=0;i<=width;++i)
            backp[i]-=offset;
    }
    return(backp);
}


long *bresenham2(int xin,int yin,double angle,int imwidth, int imheight, int *ol)
{
/**
ARGUMENTS:
	int xin:      x starting coordinate
	int yin:      y starting corrdinate
	double angle:    angle of the line in degree
	int imwidth:  width of the image
	int imheight: height of the image
	int *ol:      length of the allocated vector
RETURN VALUE:
	INT4_TYPE *
DISCRIPTION:
        This function makes a Bresenham line orientated in
        a given angle (in degree). Any angle is acceptable.
	The points in the line are offsets. The first offset
	is always 0, there are one more offset than there are
	width points.
HISTORY:
        Written by Hugues Talbot	29 Jan 1997
TESTS:
        The core of this program has been written in 1991.
        Many tests including purify.
**/

    INT4_TYPE *p, *rp;
    int       x1, x2, y1, y2, dx, dy;
    int       i, e;
    int       incx, incy, inc1, inc2;
    int       x, y;  /* the actual positions */
    double    angrad,angrad2;

    /* angle is converted to fit between 0 and 360 */
    while (angle < 0)
	angle += 360.0;
    while (angle >= 360.0)
	angle -= 360.0;

    angrad=angle*0.0174533;
    angrad2=(90.0-angle)*0.0174533;
    if ( (angle<=45.0 && angle >= 0.0) ||
	 (angle>=315.0)) {
	dx = imwidth;
	dy = (int)(tan(angrad)*(double)imwidth);
	*ol = imwidth+1;
	p=(long *)calloc(*ol, sizeof(long));
    } else if (angle>=135.0 && angle <= 225.0 ) {
	dx = -imwidth;
	dy = -(int)(tan(angrad)*(double)imwidth);
	*ol = imwidth+1;
	p=(long *)calloc(*ol, sizeof(long));
    } else if (angle>45.0 && angle <135.0) {
	dx=(int)(tan(angrad2)*(double)imheight);
	dy = imheight;
	*ol = imheight+1;
	p=(long *)calloc(*ol, sizeof(long));
    } else {
	dx=-(int)(tan(angrad2)*(double)imheight);
	dy = -imheight;
	*ol = imheight+1;
	p=(long *)calloc(*ol, sizeof(long));
    }

    /* LIARdebug("dx: %d, dy: %d", dx, dy); */
    rp = p;  /* we need to return the starting point of the vector... */
    x1 = xin;
    y1 = yin;
    x2 = x1 + dx;
    y2 = y1 + dy;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    incx = 1;
    if (x2 < x1) incx = -1;
    incy = 1;
    if (y2 < y1) incy = -1;
    /* starting position */
    x = x1;
    y = y1;
    *p++ = y * imwidth + x;


    if (dx > dy) {
	e = 2*dy - dx;
	inc1 = 2*(dy-dx);
	inc2 = 2*dy;
	for (i = 0 ; i < dx ; i++) {
	    if (e >= 0) {
		y += incy;
		e += inc1;
	    }
	    else e += inc2;
	    x += incx;
	    *p++ =  y * imwidth + x;
	}
    }
    else {
	e = 2*dx - dy;
	inc1 = 2*(dx-dy);
	inc2 = 2*dx;
	for (i = 0 ; i < dy ; i++) {
	    if (e >= 0) {
		x += incx;
		e += inc1;
	    }
	    else e += inc2;
	    y += incy;
	    *p++ =  y * imwidth + x;
	}
    }

    return(rp);
}


INT4_TYPE *bresenham_dir(int xin,
                         int yin,
                         double angle,
                         int imwidth,
                         int imheight,
                         int length,
                         int *length2)
{
/**
ARGUMENTS:
	int xin:      x starting coordinate
	int yin:      y starting corrdinate
	double angle:    angle of the line in RADIAN CAREFUL !!!
	int imwidth:  width of the image
	int imheight: height of the image
        int length:   length of the line
	int *length2:      length of the allocated vector
RETURN VALUE:
	INT4_TYPE *
DISCRIPTION:
        This function makes a Bresenham line orientated in
        a given angle (in degree). Any angle is acceptable.
	The points in the line are offsets. The first offset
	is always 0, there are one more offset than there are
	width points.
HISTORY:
        Written by Hugues Talbot/Olena Tankyevych	17 Mars 2008
TESTS:
        The core of this program has been written in 1991.
        Many tests including purify.
**/

    INT4_TYPE *p, *rp;
    int       x1, x2, y1, y2, dx, dy;
    int       i, e, ol;
    int       incx, incy, inc1, inc2;
    int       x, y;  /* the actual positions */

    /* angle is converted to fit between 0 and 2 pi */
    while (angle < 0)
	angle += 2*M_PI;
    while (angle >= 2*M_PI)
	angle -= 2*M_PI;

    if ( ((angle<=M_PI/4) && (angle >= 0.0)) ||
         (angle>=(7*M_PI/4))) {
	dx = imwidth;
	dy = (int)(tan(angle)*(double)imwidth);
	ol = imwidth+1;
	p=(INT4_TYPE *)calloc(ol, sizeof(INT4_TYPE));
        *length2 = (int)(cos(angle)*length);
    } else if ((angle>=(3*M_PI/4)) && (angle <= (5*M_PI/4))) {
	dx = -imwidth;
	dy = -(int)(tan(angle)*(double)imwidth);
	ol = imwidth+1;
	p=(INT4_TYPE *)calloc(ol, sizeof(INT4_TYPE));
        *length2 = (int)(-cos(angle)*length);
    } else if ((angle>M_PI/4) && (angle <(3*M_PI/4))) {
	dx=(int)(tan(M_PI/2-angle)*(double)imheight);
	dy = imheight;
	ol = imheight+1;
	p=(INT4_TYPE *)calloc(ol, sizeof(INT4_TYPE));
        *length2 = (int)(sin(angle)*length);
    } else {
	dx=-(int)(tan(M_PI/2-angle)*(double)imheight);
	dy = -imheight;
	ol = imheight+1;
	p=(INT4_TYPE *)calloc(ol, sizeof(INT4_TYPE));
        *length2 = (int)(-sin(angle)*length);
    }

    /* LIARdebug("dx: %d, dy: %d", dx, dy); */
    rp = p;  /* we need to return the starting point of the vector... */
    x1 = xin;
    y1 = yin;
    x2 = x1 + dx;
    y2 = y1 + dy;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    incx = 1;
    if (x2 < x1) incx = -1;
    incy = 1;
    if (y2 < y1) incy = -1;
    /* starting position */
    x = x1;
    y = y1;
    *p++ = y * imwidth + x;


    if (dx > dy) {
	e = 2*dy - dx;
	inc1 = 2*(dy-dx);
	inc2 = 2*dy;
	for (i = 0 ; (i < dx) && (i < *length2) ; i++) {
	    if (e >= 0) {
		y += incy;
		e += inc1;
	    }
	    else e += inc2;
	    x += incx;
	    *p++ =  y * imwidth + x;
	}
    }
    else {
	e = 2*dx - dy;
	inc1 = 2*(dx-dy);
	inc2 = 2*dx;
	for (i = 0 ; (i < dy) && (i < *length2) ; i++) {
	    if (e >= 0) {
		x += incx;
		e += inc1;
	    }
	    else e += inc2;
	    y += incy;
	    *p++ =  y * imwidth + x;
	}
    }

    return(rp);
}

