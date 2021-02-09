/*
 * File:		rectmm_generic.h
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

/* rectmm.c,v 1.1.1.1 1996/09/04 07:37:13 talbot Exp */
/* changed for pthread use and as a generic 22/05/2002 Yves Leveque, Richard Beare, CSIRO.
* it must be used with rectmm.c
*
 * converted to a standalone C file by Hugues Talbot	13 Mar 1995
 * Added int4 and dbl by Carolyn Evans sometime in 1997
 */

#ifndef RECTMM_HPP
#define RECTMM_HPP

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include "liarlmts.h"
#include "liarp.h"

//#ifndef max
//#define max(a,b) ((a)<(b)) ? (b):(a)
//#endif
template <typename Type>
void rectminmax(Type *in, int nx, int ny,int w, int b,bool usemin)
{
/**
ARGUMENTS:
	I_TYPE*f: the input image - here the type actually corresponds to the appropriate generic instantiation
	int nx: number of columns
	int ny: number of rows
	int w: length of one side of the  structuring element
	int b: length of the other side of the  structuring element
	void (*operation)(): genfmax for dilation, genfmin for erosion
RETURN VALUE:
	void
DESCRIPTION:
	Computes dilation or erosion (and may be generalised to other
	functions aswell) by a rectangle.
	Depending on the way it has been comp[iled (LPTHREAD), it uses a single processor (LPTHREAD_NONE) or several ones (LPTHREAD_FIXED or LPTHREAD_DYNAMIC).
HISTORY:
	Written by Ed Breen and modified by Ronald Jones. CSIRO DMS.
	Minor changes by Hugues Talbot	20 Mar 1995

	pthread adds: 22/05/02 by Yves Leveque and Richard Beare .CSIRO.

TESTS: Linted and purified, tested with experience.
**/
    int i;
    Type *row, *col;
//#if (LPTHREAD_STYLE == LP_DYNAMIC)
//    const int LPTHREAD_COUNT=lget_pthread_count();
//#endif
    row = col = in; /* initialisation of buffers */
    /***********************************************/
    {  /* needed for taking into account the different #if.. */


//#if (LPTHREAD_STYLE == LP_NONE) /* computing with one processor */

      /***********************************************/
      Type *g = (Type *)calloc(nx<ny?ny:nx, sizeof(Type));
      Type *h = (Type *)calloc(nx<ny?ny:nx, sizeof(Type));
      long *p = (long *)calloc(nx<ny?ny:nx, sizeof(long));
      /***********************************************/


    if(w > 1) { /* do rows */
        for(p[0] = 0,i=1;i<nx;i++)
            p[i] = 1 + p[i-1];
        if (usemin) {
            for(i=0;i<ny;i++,row+=nx)
                genfmin(row,g,h,p,nx,w);
        } else {
            for(i=0;i<ny;i++,row+=nx)
                genfmax(row,g,h,p,nx,w);
        }
    }

    if(b > 1) {    /* do columns */
        for(p[0] = 0,i=1;i<ny;i++)
            p[i] = nx + p[i-1];

        if (usemin) {
            for(i=0;i<nx;i++,col++)
                genfmin(col,g,h,p,ny,b);
        } else {
            for(i=0;i<nx;i++,col++)
                genfmax(col,g,h,p,ny,b);
        }

    }

    free(g);
    free(h);
//#else /*calls functions with multiprocessors *****************/
//
//    lpthread_struct_t LPS=lcreate_pthread_struct();
//    Type **g, **h;
//    long **p;
//    argument_fserie *argarray;
//    int l,open;
//
//    p=(long **)calloc(LPTHREAD_COUNT, sizeof(long*));
//    g=(Type **)calloc(LPTHREAD_COUNT, sizeof(Type*));
//    h=(Type **)calloc(LPTHREAD_COUNT, sizeof(Type*));
//    argarray=calloc(LPTHREAD_COUNT, sizeof(argument_fserie ));
//
//     /****************initialisation of argarrays*******/
// for (i=0;i<LPTHREAD_COUNT;i++)
//    {
//
//      g[i]=(Type *)calloc(nx<ny?ny:nx, sizeof(Type));
//      h[i]=(Type *)calloc(nx<ny?ny:nx, sizeof(Type));
//      p[i] = (long *)calloc(nx<ny?ny:nx, sizeof(long));
//      argarray[i].operation=*operation;
//      argarray[i].f=row;
//      argarray[i].g=g[i];
//      argarray[i].h=h[i];
//      argarray[i].p=p[i];
//      argarray[i].pn=col;
//      argarray[i].start=w;
//      argarray[i].end=b;
//      argarray[i].i = i;
//      argarray[i].nx = nx;
//      argarray[i].totpix = ny;
//      argarray[i].k = 1;/* k=1 means rows will be computed first */
//
//    }
//
//  /* loops and start threads */
//
// /*threads for rows (k=1) */
//  for (i=0;i<LPTHREAD_COUNT;i++) {
//
//    open = (pthread_create(&(LPS.Threads[i]), LPS.attr_default,
//			   (void *) XCAT2(fserierectp, I_NAME), (void *) &argarray[i]));
// 	if (open != 0) {
// 	LIARerror("Failed starting thread\n");
//        }
//  }
//
// /* wait for join */
//  for (l=0;l<LPTHREAD_COUNT;l++) {
//    int status;
//    if (pthread_join(LPS.Threads[l], (void **)&status)) {
//       LIARerror("Failed joining thread\n");
//     }
//     if (status) {
//      LIARerror("Threaded LIAR function returned a bad value,status=%d\n",status);
//    }
//  }
//
//
// /*threads for columns (k=2)*/
//  for (i=0;i<LPTHREAD_COUNT;i++) {
//    argarray[i].k = 2;
//    if (pthread_create(&(LPS.Threads[i]), LPS.attr_default,
//		       (void *) XCAT2(fserierectp, I_NAME), (void *) &argarray[i])) {
//      LIARerror("Failed starting thread\n");
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
//   for (i=0;i<LPTHREAD_COUNT;i++) {
//   free (g[i]);
//   free (h[i]);
//   free (p[i]);
//  }
//  free(argarray);
//  free(g);
//  free(h);
//
//  ldestroy_pthread_struct(LPS);
//
//#endif

    free(p);
    }
}

#endif // RECT3DMM_HPP

