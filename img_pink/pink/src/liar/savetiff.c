/*
 * $Id: savetiff.cxx,v 4.1 2008/10/27 15:34:12 hut66au Exp $
 *
 * Imview, the portable image analysis application
 * http://www.cmis.csiro.au/Hugues.Talbot/imview
 * ----------------------------------------------------------
 *
 *  Imview is an attempt to provide an image display application
 *  suitable for professional image analysis. It was started in
 *  1997 and is mostly the result of the efforts of Hugues Talbot,
 *  Image Analysis Project, CSIRO Mathematical and Information
 *  Sciences, with help from others (see the CREDITS files for
 *  more information)
 *
 *  Imview is Copyrighted (C) 1997-2001 by Hugues Talbot and was
 *  supported in parts by the Australian Commonwealth Science and
 *  Industry Research Organisation. Please see the COPYRIGHT file
 *  for full details. Imview also includes the contributions of
 *  many others. Please see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 * */


/*-------------------------------------------------------------------------
 *
 *      SaveTiff	- A function to write images using the TIFF
 *                        format. Supported compression schemes are
 *                        none (!), packbit  and LZW for any image,
 *			  ccittg3 and ccittg4 for binary images.
 *
 *                        This program makes use of the TIFF
 *                        library by Sam.Leffler@Silicon_Graphics
 *			  based on a program for XLIM by Luc Vincent,
 *                        itself based on a program by Hugues Talbot (91).
 *			  Extended to RGB space.
 *
 *                        This function now allows subimages to be saved.
 *                        subimages are merely appended to an existing file.
 *                        Subimages in multi-subimage TIFF files must
 *                        therefore be written in order.
 *                        Unsupported compression default to no compression
 *
 *	By Hugues Talbot	17 Jun 1997
 *                              Based on Writetiff. Same author.
 *      Modifications:          Hugues Talbot	28 Apr 1998
 *                              Slight changes to make it work with ImView
 *
 *-----------------------------------------------------------------------*/


// still needed even when TIFF is not compiled in
const char *comp_schemes[] = {"NONE", "LZW", "PACKBITS", "CCITTG3",
			      "CCITTG4", "RLE", "RLEW", "THUNDER",
			      "" };

#ifndef HAVE_TIFF_LIB

#include <stio.h>
#include <stdlib.h>
#include <tiffio.h>

#include "liarp.h"
#include "liarwrap.h"
#include "savetiff.h"



int setTifftype(int pixeltype,     /* pixel type */
                int imagetype,     /* image type */
                int *pi,            /* photometric interpretation */
                int *sf,            /* sample format */
                int *spp,           /* samples per pixel (this parameter is I/O) */
                int *bps)           /* bits per sample */
{
    switch (pixeltype) {
      case IM_BINARY:
        *bps = 1;
        *sf = 2;
        break;
      case IM_INT1:
        *bps = 8;
        *sf = 1;  // Should be 2, but we _never_ handle signed char.
        break;
      case IM_UINT1:
        *bps = 8;
        *sf = 1;
        break;
      case IM_INT2:
        *bps = 16;
        *sf = 2;
        break;
      case IM_UINT2:
        *bps = 16;
        *sf = 1;
        break;
      case IM_INT4:
        *bps = 32;
        *sf = 2;
        break;
      case IM_UINT4:
        *bps = 32;
        *sf = 1;
        break;
      case IM_FLOAT:
        *bps = 32;
        *sf = 3;
        break;
      case IM_DOUBLE:
        *bps = 64;
        *sf = 3;
        break;
    }


    /* the photometric interpretation */
    if (*spp == 1 ) {
        if (pixeltype == IM_BINARY)
            *pi = 0; /* min is white */
        else
            *pi = 1; /* min is black */
    }
    /* number of component if not explicitely known */
    if ((imagetype == IM_RGB) ) {
        *spp = 3;
        *pi = 2;
    }
    /* *pi == 3 means CLUT */
    /* *pi == 4 means Masked something ?? */
    if ((*spp > 1) && (*pi != 2))
        *pi = 5; /* basically, this means multispectral image */

    /* 6 is CCIR */
    /* 7 is not supported */
    /* 8 is CIE L*a*b */
    return 0;
}



int save_tiff(void **outbuffp,	   /* output buffer  */
	      int    imageindex,   /* 0 = write, > 0 = append */
	      int    start[2],	   /* dimension start */
	      int    end[2],	   /* dimension end */
	      int    pi,	   /* photometric interpretation */
	      int    sf,	   /* storage format */
	      int    spp,	   /* samples per pixels */
	      int    bps,	   /* bits per pixel */
	      short  colourmap[3], /* colour map (RGB) */
	      long   ncolours,	   /* nb of colour in the LUT */
	      const  char *fname,  /* output file name  */
	      int    compression)  /* compression scheme */
{
    TIFF        *tif;
    uint32	 rowsperstrip;
    int          linebytes, sample;
    uint32	 i, j;
    uint32       nbcols, nbrows;
    uint8	*theBufc, *pc;
    int16	*theBufs, *ps;
    int32	*theBufl, *pl;   /* will serve for float and 32 bit ints */
    double      *theBufd, *pd;   /* will serve for doubles and 64 bit ints */
    int          result=0;

    // this shuts gcc up.
    pc = 0; ps = 0; pl = 0; pd = 0;
    tif = 0;
    pc = 0;

    if (imageindex == 0) {
	if ((tif = TIFFOpen(fname, "w")) == 0) {
	    errprintf("Tiff_save: couldn't open %s for writing", fname);
	    result = 1;
	}
	/* the directory is set, we can start writing data to the TIFF file */
    } else if (imageindex > 0) {
	if ((tif = TIFFOpen(fname, "a")) == 0) {
	    errprintf("Tiff_save: couldn't open %s for appending", fname);
	    result = 2;
	}
	/* I'm not quite sure that the above is sufficient. We'll test this */
    }

    if (result == 0) { 	/* all is well, we can proceed */
	/* write header information */
	nbcols = end[0]-start[0]+1;
	nbrows = end[1]-start[1]+1;

	// indispensable stuff
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, nbcols);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, nbrows);
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16)spp);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16)bps);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, (uint16)pi);
        // for some reason this is NOT optional!
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);
	// optional stuff
	if ((start[0] != 0) ||(start[1] != 0) ) {
	    TIFFSetField(tif, TIFFTAG_XPOSITION, (float)start[0]);
	    TIFFSetField(tif, TIFFTAG_YPOSITION, (float)start[1]);
	}
	// we only output the sample format if bps > 8
	// many tiff readers are put off by sf.
	if (bps > 8)
	    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, (uint16)sf);

	/* compression scheme */
	switch (compression) {
	  case TIFF_CMP_NONE:
	    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	    break;
	  case TIFF_CMP_LZW:
	    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
	    break;
	  case TIFF_CMP_PACKBITS:
	    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);
	    break;
	  case TIFF_CMP_CCITTG3:
	    if (bps == 1)
		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX3);
	    else {
		errprintf("CCITT FAX Group 3 compression scheme applies only to BINARY images");
		return(1);
	    }
	    break;
	  case TIFF_CMP_CCITTG4:
	    if (bps == 1)
		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
	    else {
		errprintf("CCITT FAX Group 4 compression scheme applies only to BINARY images");
		return(1);
	    }
	    break;
	  case TIFF_CMP_RLE:
	    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_CCITTRLE);
	    break;
	  case TIFF_CMP_RLEW:
	    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_CCITTRLEW);
	    break;
	  case TIFF_CMP_THUNDER:
	    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_THUNDERSCAN);
	    break;
	  default:
	    errprintf("Unsupported compression, defaulting to no compression");
	    compression = 0;
	    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	    break;
	}
	/* a little bit of black magic from ras2tiff.c */
	linebytes = ((bps * nbcols + 15)>> 3) &~ 1;
	if (TIFFScanlineSize(tif) > linebytes)
	    theBufc = (uint8 *)_TIFFmalloc(linebytes*sizeof(uint8));
	else
	    theBufc = (uint8 *)_TIFFmalloc(TIFFScanlineSize(tif));
	rowsperstrip = (uint32) (8196/linebytes);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip == 0 ? (uint32)1 :
		     rowsperstrip);
	if (compression != 0) {
	    dbgprintf("TIFFwrite : Writing %d-bit, %d-component TIFF "
		      "file %s: [%d]x[%d], pi=%d, sf=%d, \nwith  %s compression\n",
		      bps, spp, fname, nbcols, nbrows, pi, sf,
		      comp_schemes[compression]);
	} else {
	    dbgprintf("TIFFwrite : Writing %d-bit, %d-component TIFF "
		      "file %s: [%d]x[%d], pi=%d, sf=%d, \nwith no compression\n",
		      bps, spp, fname, nbcols, nbrows, pi, sf);
	}

	if (outbuffp != 0) {
	    for (sample = 0 ; sample < spp ; sample++) {
		/* this switch sets up the input buffer according to type */
		switch (bps) {
		  case 1:
		  case 8:
		    pc = (uint8 *)outbuffp[sample];
		    break;
		  case 16:
		    ps = (int16 *)outbuffp[sample];
		    break;
		  case 32:
		    pl = (int32 *)outbuffp[sample];
		    break;
		  case 64:
		    pd = (double *)outbuffp[sample];
		    break;
		  default:
		    errprintf("save_tiff: unsupported bits per pixel size: %d", bps);
		    result = 10;
		    break;
		}

		/* we have to write the data to the buffer `theBufc' */
		/* it has to be of the right size/format, hence the complication here */
		for (i = 0 ; i < nbrows ; i++) {
		    switch (bps) {
		      case 1:
			for (j = 0 ; j < nbcols ; j++) {
			    if (j%8 == 0) theBufc[j>>3] =0;
			    if (!*pc++) // subtle change here: binary are displayed miniswhite
				theBufc[j>>3] |= 1 <<(7-j%8);
			}
			break;
		      case 8:
			for (j = 0 ; j < nbcols ; j++) {
			    theBufc[j] = (uint8) *pc++;
			}
			break;
		      case 16:
			theBufs = (int16 *)theBufc;
			for (j = 0 ; j < nbcols ; j++) {
			    theBufs[j] = (uint16) *ps++;
			}
			break;
		      case 32:
			theBufl = (int32 *)theBufc;
			for (j = 0 ; j < nbcols ; j++) {
			    theBufl[j] = (uint32) *pl++;
			}
			break;
		      case 64:
			theBufd = (double *)theBufc;
			for (j = 0 ; j < nbcols ; j++) {
			    theBufd[j] = (double) *pd++;
			}
			break;
		      default:
			break;
		    }
		    if (TIFFWriteScanline(tif, theBufc, i, sample) < 0) {
			errprintf("Error writing line %d", i);
			result = 11;
			break;
		    }
		}
	    }
	}
	_TIFFfree(theBufc);
	TIFFClose(tif);
    }


    return result;
}


#else // HAVE_TIFF_LIB

#include <stdio.h>

// stubs
int setTifftype(int pixeltype,     /* pixel type */
                int imagetype,     /* image type */
                int *pi,            /* photometric interpretation */
                int *sf,            /* sample format */
                int *spp,           /* samples per pixel (this parameter is I/O) */
                int *bps)           /* bits per sample */

{
    return 0;
}


int save_tiff(void **outbuffp,	   /* output buffer  */
	      int    imageindex,   /* index to save to in the file being saved */
	      int    start[2],	   /* dimension start */
	      int    end[2],	   /* dimension end */
	      int    pi,	   /* photometric interpretation */
	      int    sf,	   /* storage format */
	      int    spp,	   /* samples per pixels */
	      int    bps,	   /* bits per pixel */
	      short  colourmap[3], /* colour map (RGB) */
	      long   ncolours,	   /* nb of colour in the LUT */
	      const  char *fname,  /* output file name  */
	      int    compression) /* compression scheme */
{

    fprintf(stderr, "This version of Imview cannot save TIFF images\n");
    return 1;
}

#endif // HAVE_TIFF_LIB
