/*
 * File:		periodic.c
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

/* periodic.c,v 1.1.1.1 1996/09/04 07:37:13 talbot Exp */

/*
 * converted into a stand-alone C file by Hugues Talbot	13 Mar 1995
 *
 */

#include <math.h>
#include <stdlib.h>

#include "liarp.h"

void getdxdy(int angle, int *dx, int *dy)
{
    int division;
    int ang1,anga,dd;

    anga=abs(angle);
    if (anga<=45)
	ang1=anga;
    else
	ang1=90-anga;

    division=abs(ang1)/5;
    switch(division) {
          case 0:
            *dx=1;
            *dy=0;
            break;
          case 1:
            *dx=11;
            *dy=1;
            break;
          case 2:
            *dx=6;
            *dy=1;
            break;
          case 3:
            *dx=4;
            *dy=1;
            break;
          case 4:
            *dx=3;
            *dy=1;
            break;
          case 5:
            *dx=2;
            *dy=1;
            break;
          case 6:
            *dx=2;
            *dy=1;
            break;
          case 7:
            *dx=3;
            *dy=2;
            break;
          case 8:
            *dx=4;
            *dy=3;
            break;
          case 9:
            *dx=1;
            *dy=1;
            break;
        }
        if (anga>45) {
            dd=*dx;
            *dx=*dy;
            *dy=dd;
        }
        if (angle<0)
            *dy=-*dy;
}


long *periodic(int x, int y, int angle, int width, int *period)
{
    long *p,offset;
    int dx,dy,lx,ly,nshft,i;

    width++;
    /* calloc() args swapped */
    p=(long *)calloc(width, sizeof(long));
    getdxdy(angle,&dx,&dy);            /* find dx and dy for the given angle */

    if (abs(dx)>=abs(dy)) {
        *period=abs(dx);
        lx=width-(*period)+1;         /* extent of the line minus the period */
        nshft=lx/(*period);
        offset=0;
        for(i=0;i<nshft;++i,offset+=dx+dy*width)
            p[i]=offset;
        if (dy>0) {               /* offset the line to sit off the image */
            offset=p[nshft-1]-lx+1;
            for(i=0;i<nshft;++i)
            p[i]-=offset;
        }
    }
    else {
        *period=abs(dy);
        ly=width-(*period)+1;         /* extent of the line minus the period */
        nshft=ly/(*period);
        offset=0;
        if (dy<0)
            dx=-dx;
        for(i=0;i<nshft;++i,offset+=dx)
            p[i]=offset;
        if (dx>0) {                /* offset the line to sit off the image */
            offset=p[nshft-1];
            for(i=0;i<nshft;++i)
            p[i]-=offset;
        }
    }
    return(p);
}


