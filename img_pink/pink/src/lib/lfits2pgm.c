/*
Copyright ESIEE (2009)

m.couprie@esiee.fr

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
/*! \file fits2pgm.c

\brief converts from fits format into pgm format

<B>Usage:</B> fits2pgm in.fits out.pgm

<B>Description:</B> Converts from fits format into pgm format.

Parameters:
\li \b in.fits : source file in fits format

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - janvier 2000 */

#include <stdio.h>

#ifdef HAVE_FITS_LIB

#  include <stdint.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <stdlib.h>
#  include <assert.h>
#  include <mcimage.h>
#  include <mccodimage.h>
#  include <mcutil.h>
#  include <lfits2pgm.h>
#  include "fitsio.h"


/* =============================================================== */
struct xvimage * lfits2pgm(char *filename)
/* =============================================================== */

{

    struct xvimage *image;
    int status, anynul, datatype=0;
    fitsfile *fptr; /* pointer to the FITS file; defined in fitsio.h */
    //char comment[FLEN_COMMENT];
    long nelements, firstelem, naxes[2];
    int   kwvBITPIX, numAxis;
    double bscale = 1.0, bzero = 0.0;


    status=0;  /* initialize status before calling fitsio routines */
    firstelem=1;
    anynul=0;

 	/* Opening the fits file */
    fits_open_file (&fptr, filename, READONLY, &status);
    fits_report_error(stderr,status);

  	/* Display the data type in the image. */
  	fits_get_img_type(fptr, &kwvBITPIX, &status);
  	fits_report_error(stderr,status);
  	switch(kwvBITPIX) {
    	case BYTE_IMG   : fprintf(stderr, "Image Type:  8-bit byte pixels, 0 - 255\n");
    	    datatype=TBYTE;
    	    break;
    	case SHORT_IMG  : fprintf(stderr, "Image Type: 16 bit integer pixels\n");
    	    datatype = TSHORT;
    	    break;
    	case LONG_IMG   : fprintf(stderr, "Image Type: 32-bit integer pixels\n");
    	    datatype = TINT;
    	    break;
    	case FLOAT_IMG  : fprintf(stderr, "Image Type: 32-bit floating point pixels\n");
    	    datatype = TFLOAT;
    	    break;
    	case DOUBLE_IMG : fprintf(stderr, "Image Type: 64-bit floating point pixels\n");
    	    datatype= TDOUBLE;
    	    break;
    	default         : fprintf(stderr, "Image Type: UNKNOWN\n");                      break;
  	}

    fits_get_img_dim(fptr, &numAxis,  &status);
    fits_report_error(stderr, status); /* print out any error messages */
	fprintf(stderr, "Number of axis: %d\n", numAxis);

	/* Find the x/y-axis dimensions and the color dimension if it exists. */
  	if(numAxis < 2) {
    	fprintf(stderr, "Too few axes to be a real image!\n");
    	exit(1);
  	} else if(numAxis > 3) {
    	fprintf(stderr, "Too many axes to be a real image!\n");
    	exit(2);
  	}


    fits_get_img_size(fptr, 3, naxes, &status);
    fits_report_error(stderr, status); /* print out any error messages */
 	nelements=naxes[0]*naxes[1]; /* number of pixels to read */
    fprintf(stderr, "naxes[0]=%ld, naxes[1]=%ld\n",naxes[0],naxes[1]);

    /* turn off any scaling so that we copy the raw pixel values */
    fits_set_bscale(fptr, bscale, bzero, &status);
    fits_report_error(stderr, status); /* print out any error messages */

    /* Read the file */
    switch (datatype) {
        case TBYTE:
        image = allocimage(NULL, naxes[0], naxes[1], 1, VFF_TYP_1_BYTE);
        fits_read_img(fptr, datatype,  firstelem,  nelements, 0, image->image_data, &anynul, &status);
        break;

        case TSHORT:
        {
            // we need to convert from signed int to unsigned int (only type supported by Pink)
            fprintf(stderr, "Conversion short -> unsigned short\n");
            short *tempbuf = malloc(nelements*sizeof(short));
            short *p;
            unsigned char b1, b2;
            unsigned short *q;
            int i;
            if (tempbuf) {
                image = allocimage(NULL, naxes[0], naxes[1], 1, VFF_TYP_2_BYTE);
                if (image) {
                    fits_read_img(fptr, datatype,  firstelem,  nelements, 0, tempbuf, &anynul, &status);
                    for (i = 0, p = tempbuf, q = image->image_data ; i < nelements ; ++i,++p,++q) {
                        *q = (int)(*p) + 32768;
                    }
                } else {
                    fprintf(stderr, "Image allocation failed\n");
                }
                // this is no longer necessary as PINK now has a standard 16-bit PNM writer.
//                for (i = 0, p = tempbuf, q = image->image_data ; i < nelements ; ++i,++p,++q) {
//                    *q = (int)(*p) + 32768;
//                    // we need to swap bytes too
//                    b1 = *q & 0xFF;
//                    b2 = (*q & 0xFF00) >> 8;
//                    *q = b1 << 8 | b2 ;
//                }
            //}
                free(tempbuf);
            }
        }
        break;

        case TINT:
        image = allocimage(NULL, naxes[0], naxes[1], 1, VFF_TYP_4_BYTE);
        fits_read_img(fptr, datatype,  firstelem,  nelements, 0, image->image_data, &anynul, &status);
        break;

        case TFLOAT:
        image = allocimage(NULL, naxes[0], naxes[1], 1, VFF_TYP_FLOAT);
        fits_read_img(fptr, datatype,  firstelem,  nelements, 0, image->image_data, &anynul, &status);
        break;

        case 8:
        image = allocimage(NULL, naxes[0], naxes[1], 1, VFF_TYP_DOUBLE);
        fits_read_img(fptr, datatype,  firstelem,  nelements, 0, image->image_data, &anynul, &status);
        break;

        default:
        fprintf(stderr, "Datatype = %d not supported\n", datatype);
        break;
        }


	/* Write the file into a pgm image */
//    if (status == 0) {
//        // int fd = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 00700);
//        //if (fd > 0) {
//        //    int nbwritten = write(fd, image->image_data, nelements*nbytes);
//        //    if (nbwritten < nelements*nbytes)
//        //        fprintf(stderr, "Incomplete write\n");
//        //}
//        writeimage(image, filename);
//        freeimage(image);
//    } else {
//        fits_report_error(stderr, status); /* print out any error messages */
//        exit(3);
//    }

 	fits_close_file(fptr, &status);    /* close the file */

    return (image);
} // main

#else  // NOT HAVE_FITS_LIB

struct xvimage * lfits2pgm(char *filename)
{
    fprintf(stderr,"Sorry, FITS library not found at compile time\n");

    return 0;
}
#endif // NOT HAVE_FITS_LIB



