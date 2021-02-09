/*
 * File:		bresen3d.c
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



/* bresen3d.c */


#include <math.h>
#include <stdlib.h>

#include "liarp.h"


/**
 * \brief 3 dimensional bresneham line function

    This function creates a Bresenham line with an aspect ratio as specified
    by the x, y and z dimensions (dx, dy and dz).  The line returned is of
    length 'ol' equal to either the width, length or depth of the image plus
    one (depending on the aspect ratio).  All values within the returned line
    vector are in the form of offsets for the original image.  The first
    offset is always set to 0.  All lines are returned heading right from the
    origin.

 * \param dx x dimension of the line
 * \param dy y dimension of the line
 * \param dz z dimension of the line
 * \param imwidth height of the image (x dimension)
 * \param imheight length of the image (y dimension)
 * \param imdepth depth of the image (z dimension)
 * \param *ol length of the allocated vector
 * \param *period the period of the line (always 1)
 * \return line offsets
 * \author Ian Sowden
 * <br> based on the bresenham2 function by Hugues Talbot
 * \date 30/12/97
*/
long *bresenham3d(int dx, int dy, int dz,
		       int imwidth, int imheight, int imdepth,
		       int *ol, int *period)
{

  int        i;             /* indexing variable */
  long       *p;            /* the actual line */
  int        x, y, z;       /* position coordinates */
  int        xa, ya, za;    /* translated coordinates */
  int        e1, e2;        /* decision parameters - two planes, two params */
  int        inc1a, inc1b;  /* e1 increments */
  int        inc2a, inc2b;  /* e2 increments */
  int        absdx, absdy, absdz;  /* absolute values */

  /* set the initial coords */
  x = y = z = 0;

  /* since this is not a periodic line the period will always be 1 */
  *period = 1;

  /* set absolute values */
  absdx = abs(dx);
  absdy = abs(dy);
  absdz = abs(dz);

  /* if dx is negative then reflect through origin  */
  /* ie. we want all lines heading left from origin */
  if (dx < 0) {
    dx = -dx;
    dy = -dy;
    dz = -dz;
  } /* end if dx < 0 */


  if ( (absdx >= absdy) && (absdx >= absdz) ) {
    /* the vector is closest to the x axis.  The bresenham line is thus
       calculated by taking projections onto the xy and xz planes */

    /* set vector length and alocate mem */
    *ol = imwidth + 1;
    p = (long*) calloc(*ol, sizeof(long));
    if (p == NULL) {
      LIARerror("bresenham3d: Unable to allocate memory for line offsets");
      return(NULL);
    }

    /* calculate out initial decision params and increments */
    /* xy plane */
    inc1a = 2*absdy;           /* 2dy */
    inc1b = inc1a - 2*absdx;   /* 2dy - 2dx */
    e1 = inc1a - absdx;        /* 2dy - dx  */
    /* xz plane */
    inc2a = 2*absdz;           /* 2dz */
    inc2b = inc2a - 2*absdx;   /* 2dz - 2dx */
    e2 = inc2a - absdx;        /* 2dz - dx  */

    /* set first point */
    p[0] = z*(imwidth*imheight) + y*imwidth + x;

    /* complete the line */
    for (i=0; i<imwidth; ++i) {

      /* increment x value */
      ++x;

      /* deal with the xy plane first */

      /* if xy decision paramater is greater than zero then inc y */
      if (e1 > 0) {
	y++;
	e1 += inc1b;
      } /* end if */
      else /* leave y value unchanged */
	e1 += inc1a;

      /* now deal with the xz plane */

      /* if xz decision paramater is greater than zero then inc z */
      if (e2 > 0) {
	z++;
	e2 += inc2b;
      } /* end if */
      else /* leave 2 value unchanged */
	e2 += inc2a;

      /* translate point to the correct quadrant */
      xa = x;
      ya = (dy<0)?-y:y;
      za = (dz<0)?-z:z;

      /* add point to the line */
      p[i+1] = za*(imwidth*imheight) + ya*(imwidth) + xa;

    } /* end for */

  } /* end if dx is largest */
  else if ( (absdy >= absdx) && (absdy >= absdz) ) {
    /* in this case the vector is closest to the y axis.  The bresenham
       line is thus calculated by taking projections onto the yx and yz
       planes */

    /* set vector length and alocate mem */
    *ol = imheight + 1;
    p = (long*) calloc(*ol, sizeof(long));
    if (p == NULL) {
      LIARerror("bresenham3d: Unable to allocate memory for line offsets");
      return(NULL);
    }

    /* calculate out initial decision params and increments */
    /* yx plane */
    inc1a = 2*absdx;           /* 2dx */
    inc1b = inc1a - 2*absdy;   /* 2dx - 2dy */
    e1 = inc1a - absdy;        /* 2dx - dy  */
    /* yz plane */
    inc2a = 2*absdz;           /* 2dz */
    inc2b = inc2a - 2*absdy;   /* 2dz - 2dy */
    e2 = inc2a - absdy;        /* 2dz - dy  */

    /* set first point */
    p[0] = z*(imwidth*imheight) + y*imwidth + x;

    /* complete the line */
    for (i=0; i<imheight; ++i) {

      /* increment y value */
      ++y;

      /* deal with the yx plane first */

      /* if yx decision paramater is greater than zero then inc x */
      if (e1 > 0) {
	x++;
	e1 += inc1b;
      } /* end if */
      else /* leave x value unchanged */
	e1 += inc1a;

      /* now deal with the yz plane */

      /* if yz decision paramater is greater than zero then inc z */
      if (e2 > 0) {
	z++;
	e2 += inc2b;
      } /* end if */
      else /* leave 2 value unchanged */
	e2 += inc2a;

      /* translate point to the correct quadrant */
      xa = x;
      ya = (dy<0)?-y:y;
      za = (dz<0)?-z:z;

      /* add point to the line */
      p[i+1] = za*(imwidth*imheight) + ya*(imwidth) + xa;

    } /* end for */

  } /* end else if dy is largest */
  else {
    /* finally, the vector is closest to the z axis.  The bresenham line
       is thus calculated by taking projections onto the zx and zy planes */

    /* set vector length and alocate mem */
    *ol = imdepth + 1;
    p = (long*) calloc(*ol, sizeof(long));
    if (p == NULL) {
      LIARerror("bresenham3d: Unable to allocate memory for line offsets");
      return(NULL);
    }

    /* calculate out initial decision params and increments */
    /* zx plane */
    inc1a = 2*absdx;           /* 2dx */
    inc1b = inc1a - 2*absdz;   /* 2dx - 2dz */
    e1 = inc1a - absdz;        /* 2dx - dz  */
    /* zy plane */
    inc2a = 2*absdy;           /* 2dy */
    inc2b = inc2a - 2*absdz;   /* 2dy - 2dz */
    e2 = inc2a - absdz;        /* 2dy - dz  */

    /* set first point */
    p[0] = z*(imwidth*imheight) + y*imwidth + x;

    /* complete the line */
    for (i=0; i<imdepth; ++i) {

      /* increment z value */
      ++z;

      /* deal with the zx plane first */

      /* if zx decision paramater is greater than zero then inc x */
      if (e1 > 0) {
	x++;
	e1 += inc1b;
      } /* end if */
      else /* leave x value unchanged */
	e1 += inc1a;

      /* now deal with the zy plane */

      /* if zy decision paramater is greater than zero then inc y */
      if (e2 > 0) {
	y++;
	e2 += inc2b;
      } /* end if */
      else /* leave y value unchanged */
	e2 += inc2a;

      /* translate point to the correct quadrant */
      xa = x;
      ya = (dy<0)?-y:y;
      za = (dz<0)?-z:z;

      /* add point to the line */
      p[i+1] = za*(imwidth*imheight) + ya*(imwidth) + xa;

    } /* end for */

  } /* end else dz is largest */


  return(p);

} /* end of bresneham3d function */



/**
 * \brief 3 dimensional bresenham directional line function

    This function creates a Bresenham line with an aspect ratio as specified
    by the x, y and z dimensions (dx, dy and dz).  The line returned is of
    desired length 'length' but of actual returned length 'ol', which is also
    the length of the returned vector,
    depending on the aspect ratio.  All values within the returned line
    vector are in the form of offsets for the original image.  The first
    offset is always set to 0.  All lines are returned heading right from the
    origin.

 * \param dx x dimension of the line
 * \param dy y dimension of the line
 * \param dz z dimension of the line
 * \param imwidth height of the image (x dimension)
 * \param imheight length of the image (y dimension)
 * \param imdepth depth of the image (z dimension)
 * \param *ol length of the allocated vector
 * \param *period the period of the line (always 1)
 * \return line offsets
 * \author Ian Sowden
 * <br> based (loosely) on the bresenham2 function by Hugues Talbot
 * \date 30/12/97
*/
INT4_TYPE *bresenham3d_dir(int dx, int dy, int dz,
                           int imwidth, int imheight, int imdepth,
                           int length,
                           int *ol, int *period)
{

    int        i;             /* indexing variable */
    INT4_TYPE  *p;            /* the actual line */
    int        x, y, z;       /* position coordinates */
    int        xa, ya, za;    /* translated coordinates */
    int        e1, e2;        /* decision parameters - two planes, two params */
    int        inc1a, inc1b;  /* e1 increments */
    int        inc2a, inc2b;  /* e2 increments */
    int        absdx, absdy, absdz;  /* absolute values */

    /* set the initial coords */
    x = y = z = 0;

    /* since this is not a periodic line the period will always be 1 */
    *period = 1;

    /* set absolute values */
    absdx = abs(dx);
    absdy = abs(dy);
    absdz = abs(dz);

    /* if dx is negative then reflect through origin  */
    /* ie. we want all lines heading left from origin */
    if (dx < 0) {
        dx = -dx;
        dy = -dy;
        dz = -dz;
    } /* end if dx < 0 */


    if ( (absdx >= absdy) && (absdx >= absdz) ) {
        /* the vector is closest to the x axis.  The bresenham line is thus
           calculated by taking projections onto the xy and xz planes */

        /* set vector length and alocate mem */
        *ol = liarmin(imwidth,length);
        p = (INT4_TYPE*) calloc(*ol+1, sizeof(INT4_TYPE));
        if (p == NULL) {
            LIARerror("bresenham3d: Unable to allocate memory for line offsets");
            return(NULL);
        }

        /* calculate out initial decision params and increments */
        /* xy plane */
        inc1a = 2*absdy;           /* 2dy */
        inc1b = inc1a - 2*absdx;   /* 2dy - 2dx */
        e1 = inc1a - absdx;        /* 2dy - dx  */
        /* xz plane */
        inc2a = 2*absdz;           /* 2dz */
        inc2b = inc2a - 2*absdx;   /* 2dz - 2dx */
        e2 = inc2a - absdx;        /* 2dz - dx  */

        /* set first point */
        p[0] = z*(imwidth*imheight) + y*imwidth + x;

        /* complete the line */
        for (i=0; i< *ol; ++i) {

            /* increment x value */
            ++x;

            /* deal with the xy plane first */

            /* if xy decision paramater is greater than zero then inc y */
            if (e1 > 0) {
                y++;
                e1 += inc1b;
            } /* end if */
            else /* leave y value unchanged */
                e1 += inc1a;

            /* now deal with the xz plane */

            /* if xz decision paramater is greater than zero then inc z */
            if (e2 > 0) {
                z++;
                e2 += inc2b;
            } /* end if */
            else /* leave 2 value unchanged */
                e2 += inc2a;

            /* translate point to the correct quadrant */
            xa = x;
            ya = (dy<0)?-y:y;
            za = (dz<0)?-z:z;

            /* add point to the line */
            p[i+1] = za*(imwidth*imheight) + ya*(imwidth) + xa;

        } /* end for */

    } /* end if dx is largest */
    else if ( (absdy >= absdx) && (absdy >= absdz) ) {
        /* in this case the vector is closest to the y axis.  The bresenham
           line is thus calculated by taking projections onto the yx and yz
           planes */

        /* set vector length and alocate mem */
        *ol = liarmin(imheight,length);
        p = (INT4_TYPE*) calloc(*ol+1, sizeof(INT4_TYPE));
        if (p == NULL) {
            LIARerror("bresenham3d: Unable to allocate memory for line offsets");
            return(NULL);
        }

        /* calculate out initial decision params and increments */
        /* yx plane */
        inc1a = 2*absdx;           /* 2dx */
        inc1b = inc1a - 2*absdy;   /* 2dx - 2dy */
        e1 = inc1a - absdy;        /* 2dx - dy  */
        /* yz plane */
        inc2a = 2*absdz;           /* 2dz */
        inc2b = inc2a - 2*absdy;   /* 2dz - 2dy */
        e2 = inc2a - absdy;        /* 2dz - dy  */

        /* set first point */
        p[0] = z*(imwidth*imheight) + y*imwidth + x;

        /* complete the line */
        for (i=0; i<*ol; ++i) {

            /* increment y value */
            ++y;

            /* deal with the yx plane first */

            /* if yx decision paramater is greater than zero then inc x */
            if (e1 > 0) {
                x++;
                e1 += inc1b;
            } /* end if */
            else /* leave x value unchanged */
                e1 += inc1a;

            /* now deal with the yz plane */

            /* if yz decision paramater is greater than zero then inc z */
            if (e2 > 0) {
                z++;
                e2 += inc2b;
            } /* end if */
            else /* leave 2 value unchanged */
                e2 += inc2a;

            /* translate point to the correct quadrant */
            xa = x;
            ya = (dy<0)?-y:y;
            za = (dz<0)?-z:z;

            /* add point to the line */
            p[i+1] = za*(imwidth*imheight) + ya*(imwidth) + xa;

        } /* end for */

    } /* end else if dy is largest */
    else {
        /* finally, the vector is closest to the z axis.  The bresenham line
           is thus calculated by taking projections onto the zx and zy planes */

        /* set vector length and alocate mem */
        *ol = liarmin(imdepth,length);
        p = (INT4_TYPE*) calloc(*ol+1, sizeof(INT4_TYPE));
        if (p == NULL) {
            LIARerror("bresenham3d: Unable to allocate memory for line offsets");
            return(NULL);
        }

        /* calculate out initial decision params and increments */
        /* zx plane */
        inc1a = 2*absdx;           /* 2dx */
        inc1b = inc1a - 2*absdz;   /* 2dx - 2dz */
        e1 = inc1a - absdz;        /* 2dx - dz  */
        /* zy plane */
        inc2a = 2*absdy;           /* 2dy */
        inc2b = inc2a - 2*absdz;   /* 2dy - 2dz */
        e2 = inc2a - absdz;        /* 2dy - dz  */

        /* set first point */
        p[0] = z*(imwidth*imheight) + y*imwidth + x;

        /* complete the line */
        for (i=0; i<*ol; ++i) {

            /* increment z value */
            ++z;

            /* deal with the zx plane first */

            /* if zx decision paramater is greater than zero then inc x */
            if (e1 > 0) {
                x++;
                e1 += inc1b;
            } /* end if */
            else /* leave x value unchanged */
                e1 += inc1a;

            /* now deal with the zy plane */

            /* if zy decision paramater is greater than zero then inc y */
            if (e2 > 0) {
                y++;
                e2 += inc2b;
            } /* end if */
            else /* leave y value unchanged */
                e2 += inc2a;

            /* translate point to the correct quadrant */
            xa = x;
            ya = (dy<0)?-y:y;
            za = (dz<0)?-z:z;

            /* add point to the line */
            p[i+1] = za*(imwidth*imheight) + ya*(imwidth) + xa;

        } /* end for */

    } /* end else dz is largest */


    return(p);

} /* end of bresneham3d function */

