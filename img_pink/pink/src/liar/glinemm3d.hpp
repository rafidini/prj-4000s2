/*
 * File:		glinemm3d.c
 *
Hugues Talbot	 7 Dec 2010

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

/* glinemm3d.hpp */


#ifndef GLINEMM3D_HPP
#define GLINEMM3D_HPP



#include <stdlib.h>
#include <math.h>

#include "liarp.h"


/**
 * \brief 3d implementation of glineminmax function

    This function performs erosion or dilation along a line parallel to a given
    direction vector (dx,dy,dz).  Whether an erosion or a dilation is performed
    is specified by the (*operation)() argument, this being either genfmin for
    erosion or genfmax for dilation.  The *(*lineop)() argument is then used to
    specify whether this operation uses a periodic or a bresenham line (by
    passing either periodic3d or bresenham3d).  As with the two dimensional
    case, cascading this function can be used to generate other erosions and
    dilations.

	Tested on a number of images

 * \param *f:             input image
 * \param nx:             number of columns in input image
 * \param ny:             number of rows in input image
 * \param nz:             number of slices in input image
 * \param k:              length of structuring element
 * \param dx:             x dimension of SE direction vector
 * \param dy:             y dimension of SE direction vector
 * \param dz:             z dimension of SE direction vector
 * \param (*operation)(): min or max operation (genfmin/genfmax)
 * \param *(*lineop)():   line operation to use (bresenham3d or periodic3d)

 * \return 0 	indicates success
   <br>    1 	failure because SE vector does not fit inside input image,
				that is (nx<dx) or (ny<dy) or (nz<dz)
 * \author Ian Sowden
 * <br> 7 Jan 1998: Modified by Ian Sowden to incorporate periodic lines
 * \date 5 Jan 1998
*/
template <typename Type>
int glineminmax3d(Type *f, int nx, int ny, int nz, int k,
		   int dx, int dy, int dz,
		   bool usemin, bool lineop )
{

  Type        *g, *h;                    /* forwards and backwards arrays */
  int         totpix, slicepix;          /* pixel counts                  */
  long        *line;                     /* the line to perform operation */
  int         ol;                        /* length of the line            */
  long        start, end;                /* start and end of line segs    */
  Type        offset;                    /* image offset                  */
  int         row_current, row_prev;     /* current and previous rows     */
  int         col_current, col_prev;     /* current and previous cols     */
  int         upper_limit, lower_limit;  /* image limits (with offset)    */
  int         maindir_cutoff;            /* cutoff for value of end       */
  int         dir_y, dir_z;              /* direction of the line +-1     */
  int         i, j, l;                   /* indexing vars                 */
  int         badx, bady, badz, good_previous; /* has x/y/z violation occurred in
					    this loop? in the previous loop ? */
  int         period;                    /* the period of the line        */
  int         nshift;
  double      xinc, yinc;

  /* set number of pixels in image, slice */
  slicepix = nx*ny;
  totpix = slicepix*nz;

  /* generate line */
  if (lineop){
    line = bresenham3d(dx,dy,dz, nx,ny,nz, &ol, &period);
  }
  else {
     line = periodic3d(dx,dy,dz, nx,ny,nz, &ol, &period);
  }
  /* unable to allocate line  */
  /* this happens when  */
  if (line == NULL) return 1;

  /* allocate memory for forward and backward min/max arrays */
  g = (Type *)calloc(ol, sizeof(Type));
  h = (Type *)calloc(ol, sizeof(Type));

  /* reflect -dx vectors (makes things easier later) */
  if (dx<0) {
    dx=-dx;
    dy=-dy;
    dz=-dz;
  } /* end if */

  /* figure out the xy and xz slopes */
  dir_y=(dy<0)?-1:1;
  dir_z=(dz<0)?-1:1;

  /* remainder of function depends on the which of dx, dy and dz is the */
  /* largest */

  if ( (abs(dx) >= abs(dy)) && (abs(dx) >= abs(dz)) ) {
    /* then the vector is closest to the x axis.  Thus the image will be */
    /* scanned by taking increments in both the y and z dimensions.      */
    /* start and end will be x coordinates                               */

    if (period > 1)
      yinc = dir_y*(abs(dy) - 0.5);
    else
      yinc = dir_y*(0.5);

    /*    nshift = (nx-period+1)/period;*/
    nshift = nx/period-1;
    maindir_cutoff = nx - nx%period;

    for(i=0; i<nz; ++i) {

      for (j=0; j<ny; ++j) {

	for (l=0; l<period; ++l) {

	  /* start from the left of each row and from the front(back) of  */
	  /* each slice depending on whether dz is pos(neg)               */
	  start = end = 0;
	  offset = i*slicepix*dir_z + j*nx*dir_y + l
	    + ((1-dir_z)/2)*slicepix*(nz-1)
	    + ((1-dir_y)/2)*nx*(ny-1);

	  /* continue until we reach the end of the row */
	  while (start<=nshift) {

	    /* increment end until the point moves outside of either */
	    /* the y or z ranges of the image */

	    bady=badz=0;
	    good_previous=0;

	    /* note that 2 slices are added to the position to remove any */
	    /* problems around 0 (eg -1/nx = 1/nx = 0)                    */
	    row_current = (line[end]+offset+2*slicepix)/nx;
	    do {
	      ++end;

	      row_prev=row_current;
	      row_current=(line[end]+offset+2*slicepix)/nx;

	      /* the following should detect every single y violation */
	      if ((int)((row_current%ny)-(row_prev%ny)-(yinc))!=0)
		bady=1;

	      /* now check for z violations */
	      if (dir_z==1) {
		if (row_current >= ((ny*nz)+((bady*dir_y+2)*ny)))
		  badz=1;
	      } /* end if dir_z==1 */
	      else {  /* dir_z==-1 */
		if ( row_current < ((bady*dir_y)+(2*ny)) )
		  badz=1;
	      } /* end else dir_z == -1 */

	      /* also check whether the point is within x range*/
	      if  ((!bady) && (!badz) && (end*period<(maindir_cutoff-l)) )
		good_previous=1;
	    } while  ((!bady) && (!badz) && (end*period<(maindir_cutoff-l)) );

	    --end;

	    /* perform the actual min/max operation */
	    /* but only if this value of end is acceptable */
	    if (good_previous){
            if (usemin) {
                genfmin(f+offset, g, h, line+start, end-start+1,(k-period+1)/period);
            } else {
               genfmax(f+offset, g, h, line+start, end-start+1,(k-period+1)/period);
            }
	    }
	    /* set the start (and end) to the next point */
	    start = ++end;

	    /* update the offset so that starting point is valid */
	    offset -= (bady*dir_y*slicepix) + (badz*dir_z*totpix);

	  } /* end while start<nx */

	} /* end for each col in period */

      } /* end for each row */

    } /* end for each slice */


  } /* end if the line is closest to the x axis */
  else if ( (abs(dy)>=abs(dx))&&(abs(dy)>=abs(dz)) ) {
    /* in this case the vector is closest to the y axis. As such the image */
    /* will be scanned in the x and z dimensions.  start and end will both */
    /* be y values */

    if (period > 1)
      xinc = (abs(dx) - 0.5);
    else
      xinc = 0.5;

    nshift= ny/period-1;
    maindir_cutoff = ny - ny%period;

    for(i=0; i<nz; ++i) {

      for (j=0; j<nx; ++j) {

	for (l=0; l<period; ++l) {

	  /* start from the top(bottom) of each column for pos(neg) y slopes */
	  /* and the front(back) for pos(neg) z slopes */
	  start=end=0;
	  offset = i*slicepix*dir_z + j + l*nx*(dir_y)
	    + nx*(ny-1)*((1-dir_y)/2) + slicepix*(nz-1)*((1-dir_z)/2);

	  while (start<=nshift) {

	    /* set location bounds */
	    lower_limit = -offset;
	    upper_limit = totpix-offset;

	    /* find last acceptable end value */
	    badx=badz=0;
	    good_previous=0;

	    col_current = (line[end]+offset+2*slicepix)%nx;
	    do {
	      ++end;

	      col_prev=col_current;
	      col_current=(line[end]+offset+2*slicepix)%nx;

	      /* the following should detect any x violations */
	      if ((int)(col_current-col_prev-(xinc))!=0)
		badx=1;

	      /* now just check for z violations */
	      /* in this case z violations should be all values outside the */
	      /* the image boundaries. It should not be possible to have an */
	      /* x violation moving outside the boundaries.  Likewise an x  */
	      /* violation shouldn't cancel a z */
	      if ((line[end]<lower_limit)||(line[end]>=upper_limit))
		badz=1;

	      if ((!badx) && (!badz) && (end*period<maindir_cutoff-l))
		good_previous = 1;
	    } while ((!badx) && (!badz) && (end*period<maindir_cutoff-l) );

	    --end;

	    /* perform the actual min/max operation */
	    /* but only if this value of end is acceptable */
	    if (good_previous)
            if (usemin) {
                genfmin(f+offset, g, h, line+start, end-start+1,
                        (k-period+1)/period);
            } else {
               genfmax(f+offset, g, h, line+start, end-start+1,
                        (k-period+1)/period);
            }


	    /* set the next start (and end) to the next point along */
	    start = ++end;

	    /* update the offset based on any violations that occured */
	    offset -= (badx*nx) + (badz*dir_z*totpix);

	  } /* end while start<nshift */

	} /* end for each row in period */

      } /* end for each column */

    } /* end for each slice */


  } /* end else if closest to the y axis */
  else {
    /* finally, the vector must be closest to the z axis.  Thus we scan in  */
    /* both x and y dimensions.  start and end will be z values             */

    if (period > 1)
      xinc = abs(dx) - 0.5;
    else
      xinc = 0.5;

    if (period > 1)
      yinc = dir_y*(abs(dy) - 0.5);
    else
      yinc = dir_y*(0.5);

    nshift = nz/period-1;
    maindir_cutoff = nz - nz%period;

    for (i=0; i<ny; ++i) {

      for (j=0; j<nx; ++j) {

	for (l=0; l<period; ++l) {

	  /* start from the front(back) of each slice if dz is pos(neg) */
	  /* and the top(bottom) of each col if dy is pos(neg)          */
	  start=end=0;
	  offset = i*nx*dir_y + j + l*slicepix*dir_z
	    + (slicepix*(nz-1)*((1-dir_z)/2))+ ((1-dir_y)/2)*nx*(ny-1);

	  /* continue until we reach the end of the slice */
	  while (start<=nshift) {

	    lower_limit = -offset;
	    upper_limit = totpix-offset;

	    /* increment end until no longer acceptable */

	    badx=bady=0;
	    good_previous=0;

	    col_current = (line[end]+offset+2*slicepix)%nx;
	    row_current = ((line[end]+offset+2*slicepix)/nx)%ny;
	    do {
	      ++end;

	      col_prev=col_current;
	      col_current=(line[end]+offset+2*slicepix)%nx;
	      row_prev=row_current;
	      row_current=((line[end]+offset+2*slicepix)/nx)%ny;

	      /* find any x violations - should find all */
	      if ((int)(col_current-col_prev-(xinc))!=0)
		badx=1;

	      /* now check for y violations */
	      if ((int)(row_current-row_prev-(yinc)-badx)!=0)
		bady=1;

	      /* finally one special case - this needs improvement!! */
	      if ( badx && (row_current==0)) {
		if (row_prev==0)
		  bady=1;
		if ((row_prev==(ny-1)) || (row_prev==(ny-2)))
		  bady=0;
	      } /* end if */

	      if ((!badx) && (!bady) && (end*period<maindir_cutoff-l) )
		good_previous = 1;
	    } while( (!badx) && (!bady) && (end*period<maindir_cutoff-l) );
	    --end;

	    /* perform the actual op */
	    /* but only if this value of end is acceptable */
	    if (good_previous)
            if (usemin) {
                genfmin(f+offset, g, h, line+start, end-start+1,
                            (k-period+1)/period);
            } else {
                genfmax(f+offset, g, h, line+start, end-start+1,
                            (k-period+1)/period);
            }


	    /* update start and end positions */
	    start = ++end;

	    /* update offset */
	    offset -= ((bady*dir_y*slicepix) + (badx*nx));

	  } /* end while start<nshift */

	} /* end for each slice in period */

      } /* end for each column */

    } /* end for each row */


  } /* end else closest to the z axis */

  /* clean up */
  free(g);
  free(h);
  free(line);

  return 0;

} /* end of glineminmax3d */




#endif // GLINEMM3D_HPP




