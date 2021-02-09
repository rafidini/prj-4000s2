/*
 * File:		glinemm.c
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

/* $Id: glinemm_generic.h,v 1.7 2003/04/17 08:26:22 kcheong Exp $ */
/* changed for pthread use  and as a generic 22/05/2002 Yves Leveque, Richard Beare, CSIRO.
* it must be used with glinemm.c
 * converted into a stand-alone C file by Hugues Talbot	13 Mar 1995
 */

#ifndef GLINEMM_GENERIC_HPP
#define GLINENN_GENERIC_HPP

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

//#include "liarlmts.h"
#include "liarp.h"
#include "mcutil.h"
#include "genfmin.hpp"
#include "genfmax.hpp"
#include "lgrank_generic.hpp"

// #ifndef max
// #define max(a,b) ((a)<(b)) ? (b):(a)
// #endif


template <typename Type>
int glineminmax(Type *f,int nx,int ny,int k,int theta,
		 bool usemin, bool lineop)
{
/**
ARGUMENTS:
	I_TYPE *f: the input image - here the type actually corresponds to the appropriate generic instantiation
	int nx: number of columns
	int ny: number of rows
	int k: length of structuring element
	int theta: angle of structuring element
	void (*operation)(): genfmax for dilation, genfmin for erosion
	int *(*line)(): type of line to use (Bresenham or periodic)
RETURN VALUE:
	void
DESCRIPTION:
	Computes dilation or erosion (and may be generalised to other
	functions aswell) along a line at given angle in the image.
	Cascading this function can be used to generate dilations and
	erosions over rectangles and other polygons.
	depending on the way it has been comp[iled (LPTHREAD), it uses a single processor (LPTHREAD_NONE) or several ones (LPTHREAD_FIXED or LPTHREAD_DYNAMIC)
HISTORY:
	Written by Ed Breen and modified by Ronald Jones. CSIRO DMS.
	Minor changes by Hugues Talbot	20 Mar 1995

	pthread adds: 22/05/02 by Yves Leveque and Richard Beare .CSIRO.

TESTS: Linted and purified, tested with experience.
**/


    int nshft,totpix,offset;
    long *p,*pn;
    int i,cont,start,end,period;
    double m,thetrad=M_PI/180.0;

//#if (LPTHREAD_STYLE == LP_DYNAMIC)
//    const int LPTHREAD_COUNT=lget_pthread_count();
//#endif

    m = std::max(fabs(cos(thetrad*(double)theta)),fabs(sin(thetrad*(double)theta)));
    k=(int)((double)k*m+0.5);/* +0.5 is for having the nearest integer */

    if (abs(theta)<=45) {     /* set up offsets to scan image top to bottom */
        if (lineop){
            p = bresenham(0,0,theta,nx-1,&period);/* period refers to bresenham or periodic lines */
        }
        else {
            p = periodic(0,0,theta,nx-1,&period);/* period refers to bresenham or periodic lines */
        }
        pn=NULL;
        nshft=((nx-period+1)/period);/*size of each shift */
        totpix=nx*ny-period+1; /* total number of pixel for the buffer */
    }
    else {                    /* set up offsets to scan image left to right */
        if (lineop){
            p = bresenham(0,0,theta,ny-1,&period);/* period refers to bresenham or periodic lines */
            pn=bresenham(0,0,theta,ny-1,&period);
        }
        else {
            p = periodic(0,0,theta,ny-1,&period);/* period refers to bresenham or periodic lines */
            pn=periodic(0,0,theta,ny-1,&period);
        }
        nshft=((ny-period+1)/period);
        for (i=0;i<nshft;++i)
            pn[i]+=i*period*nx;
        totpix=nx-period+1;
    }

    k=(int)(k/period + 0.5);/* length of the structuring element divided by the epriod */

    if (theta<=0)
        start=end=0;
    else
        start=end=nshft-1;
    /* initialisations */
    offset=0;
    cont=1;

    {
      /* needed for taking into account the different #if.. */

//#if (LPTHREAD_STYLE == LP_NONE) /* computing with one processor */

      /*****************************************/
       Type *g=(Type *)calloc((unsigned int)nshft, sizeof(Type));
       Type *h=(Type *)calloc((unsigned int)nshft, sizeof(Type));
       int limit;
      /*****************************************/

    while (cont) {                        /* first compute the start */
        limit=totpix+offset;
        if ((start>=0)&&(p[start]<limit)&&(p[start]>=offset)) {
          while ((start>=0)&&(p[start]<limit)&&(p[start]>=offset)) {
            start--;
          }
          start++;
        } else {
          while ((start<nshft)&&((p[start]<offset)||(p[start]>=limit))) {
            start++;
          }
        }
        if (start<nshft) {
            if ((end<nshft)&&(p[end]>=offset)&&(p[end]<limit)) {
              while ((end<nshft)&&(p[end]>=offset)&&(p[end]<limit)) {
                end++;
              }
              end--;
            } else {
              while ((end>start)&&((p[end]<offset)||(p[end]>=limit))) {
                end--;
              }
            }

            if (abs(theta)<=45) {
              if (start<=end) {
                for (i=0;i<period;++i) {
                    if (usemin) {
                        genfmin(f+i-offset,g,h,p+start,end-start+1,k);
                    } else {
                       genfmax(f+i-offset,g,h,p+start,end-start+1,k);
                    }
                }
              }
            } else {
              if (start<=end) {
                for (i=0;i<period;++i) {
                    if (usemin) {
                        genfmin(f+i*nx-offset,g,h,pn+start,end-start+1,k);
                    } else {
                       genfmax(f+i*nx-offset,g,h,pn+start,end-start+1,k);
                    }
                }
              }
            }

            if (abs(theta)<=45)
                offset-=nx;
            else
                offset--;
        } else {
          cont=0;
        }
    }
    free(g);
    free(h);

//#else /*calls functions for using multiprocessors *****************/
//
//    lpthread_struct_t LPS=lcreate_pthread_struct();
//    int i,l;
//    I_TYPE **g, **h;
//    argument_fserie *argarray;
//
//
//    g=(Type **)calloc(LPTHREAD_COUNT, sizeof(Type*));
//    h=(Type **)calloc(LPTHREAD_COUNT, sizeof(Type*));
//    argarray=calloc(LPTHREAD_COUNT, sizeof(argument_fserie ));
//
//  /****************initialisation of argarrays*******/
// for (i=0;i<LPTHREAD_COUNT;i++)
//    {
//
//      g[i]=(Type *)calloc((unsigned int)nshft, sizeof(Type));
//      h[i]=(Type *)calloc((unsigned int)nshft, sizeof(Type));
//      //argarray[i].operation=*operation;
//      argarray[i].f=f;
//      argarray[i].g=g[i];
//      argarray[i].h=h[i];
//      argarray[i].p=p;
//      argarray[i].pn=pn;
//      argarray[i].start=start;
//      argarray[i].end=end;
//      argarray[i].i = i;
//      argarray[i].nx = nx;
//      argarray[i].totpix = totpix;
//      argarray[i].period = period;
//      argarray[i].theta = theta;
//      argarray[i].nshft = nshft;
//      argarray[i].k = k;
//    }
//  /* loops and starts threads */
//  for (i=0;i<LPTHREAD_COUNT;i++) {
//
//    if (pthread_create(&(LPS.Threads[i]), LPS.attr_default,
//		       (void *) XCAT2(fseriep, I_NAME), (void *) &argarray[i])) {
//        LIARerror("Failed starting thread\n");
//       }
//  }
// /* wait for join */
//  for (l=0;l<LPTHREAD_COUNT;l++) {
//    int status;
//    if (pthread_join(LPS.Threads[l], (void **)&status)) {
//      LIARerror("Failed joining thread\n");
//    }
//    if (status) {
//      LIARerror("Threaded LIAR function returned a bad value,status=%d\n",status);
//
//    }
//  }
//
//  free(argarray);
//  free(g);
//  free(h);
//
//  ldestroy_pthread_struct(LPS);
//
//#endif

    }


    free(p);
    if (abs(theta)>45)   /* only free this if it has been assigned */
	free(pn);

	return 0;
}

#endif // GLINEMM_GENERIC_HPP
