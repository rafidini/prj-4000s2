/*
 * File:		circlmm.c
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

/* circlmm.c,v 1.1.1.1 1996/09/04 07:37:11 talbot Exp */

/*
 * converted into a stand-alone C file Hugues Talbot	13 Mar 1995
 * Minor changes by Son Truong to incorporate old style and generic form 1/12/2000
 */

#include <stdio.h>
#include <math.h>
//#include "liarlmts.h"
#include "liarp.h"


template <typename Type>
int circleminmax(Type *f, int nx, int ny, int radius,bool usemin, bool lineop, int n)
{

    /**void (*operation)(): genfmax for dilation, genfmin for erosion =usemin
	int *(*line)(): type of line to use (Bresenham or periodic) = lineop*/

    float degrees,th;
    int k;


    k = (int)(((M_PI * (float)radius)/(float)n) + 0.5);
    degrees = (180.0/(float)n);

    if(usemin)
        for(th = -90.0+degrees;th <= 90.0; th += degrees)
            glineminmax(f,nx,ny,k,(int)th,usemin,lineop);
    else
        for(th = 90.0;th > -90.0; th -= degrees)
            glineminmax(f,nx,ny,k,(int)th,usemin,lineop);

    return 0;
}

//void circleminmax_INT4(INT4_TYPE *f, int nx, int ny, int radius,void
//		  (*operation)(),INT4_TYPE *(*line)(), int n)
//{
//    float degrees,th;
//    int k;
//    extern void genfmax_int4();
//    extern void genfmax_INT4();
//
//    k = (int)(((M_PI * (float)radius)/(float)n) + 0.5);
//    degrees = (180.0/(float)n);
//
//    if((operation != genfmax_int4) && (operation != genfmax_INT4))
//	for(th = -90.0+degrees;th <= 90.0; th += degrees)
//	    glineminmax_int4(f,nx,ny,k,(int)th,operation,line);
//    else
//	for(th = 90.0;th > -90.0; th -= degrees)
//	    glineminmax_int4(f,nx,ny,k,(int)th,operation,line);
//}
//void circleminmax_DBL(DBL_TYPE *f, int nx, int ny, int radius,void
//		  (*operation)(),INT4_TYPE *(*line)(), int n)
//{
//    float degrees,th;
//    int k;
//    extern void genfmax_dbl();
//    extern void genfmax_DBL();
//
//    k = (int)(((M_PI * (float)radius)/(float)n) + 0.5);
//    degrees = (180.0/(float)n);
//
//    if((operation != genfmax_dbl) && (operation != genfmax_DBL))
//	for(th = -90.0+degrees;th <= 90.0; th += degrees)
//	    glineminmax_dbl(f,nx,ny,k,(int)th,operation,line);
//    else
//	for(th = 90.0;th > -90.0; th -= degrees)
//	    glineminmax_dbl(f,nx,ny,k,(int)th,operation,line);
//}
//
//
//






