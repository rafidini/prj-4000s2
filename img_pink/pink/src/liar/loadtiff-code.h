/*
 * $Id: loadtiff-code.h,v 4.2 2004/06/21 08:18:29 hut66au Exp $
 *
 * Imview, the portable image analysis application
 * http://imview.sf.net
 * ----------------------------------------------------------
 *
 *  Imview is an attempt to provide an image display application
 *  suitable for professional image analysis. It was started in
 *  1997 and is mostly the result of the efforts of Hugues Talbot,
 *  with help from others (see the CREDITS files for
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

/*------------------------------------------------------------------------
 *
 * Decodes the TIFF format.
 * Should be able to read any data format. Will deal with non-char
 * data
 *
 * Based on `loadtiff.c' by the same author
 *
 * Hugues Talbot	10 Jan 1998
 *      
 *-----------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>
#include <stdarg.h>
#include "loadtiff.h"

/* useful function for decoding TIFF image type */

int getTiffType(int pi,  /* photometric interpretation */
		int sf,  /* sample format */
		int spp, /* samples per pixel */
		int bps, /* bits per sample */
		pixtype *pixeltype,
		imgtype *imagetype)
{

    int tys[][3] = {
	{IM_BINARY,IM_BINARY, -1,},
	{IM_UINT1, IM_UINT1,  -1,},   /* we _never_ handle signed char */
	{IM_INT2, IM_UINT2, -1,},
	{IM_INT4, IM_UINT4, IM_FLOAT,},
	{IM_INT8, IM_UINT8, IM_DOUBLE,}
	};

    int idx1, idx2;
	
    *imagetype = IM_UNSPEC;  /*-- default imagetype is 'unspecified'--*/
    
    if(spp == 3 && bps == 8 && pi == 2) {
        *pixeltype = IM_UINT1;
	*imagetype  = IM_RGB; /*--assume RGB--*/
	return 0;
    }

    switch(bps) {
    case 1: idx1 = 0; break;
    case 4: idx1 = 1; break;
    case 8: idx1 = 1; break;
    case 12:idx1 = 2; break;
    case 16:idx1 = 2; break;
    case 32:idx1 = 3; break;
    case 64:idx1 = 4; break;
    default:
        *imagetype = IM_ERROR;
	return 1; /* error unknown image type */
    }

    if(sf == 1 || sf == 4)
	idx2 = 1;
    else if(sf == 2)
	idx2 = 0;
    else if(sf == 3)
	idx2 = 2;
    else
        {
	*imagetype = IM_ERROR;
	return 1; /* error unknown image type */ 
        }
	
    *pixeltype = (pixtype)(tys[idx1][idx2]);
    if(*pixeltype == -1)
        {
        *imagetype = IM_ERROR;
	return 1;   /* error unknown image type */
        }
    else
        {
        *imagetype = IM_SPECTRUM; /* default to single component image */
	return 0;
        }
}


/* this is the unalterated function from the LIAR. */
/* Hugues Talbot	10 Jan 1998 */

int load_tiff(const char          *fname,	 /* file name  */
	      int                  imageindex,     /* image index in the file to load */
	      int                  start[3],	 /* dimensions start */
	      int                  end[3],         /* dimensions end */
	      int                 *pi,            /* photometric interpretation */
	      int                 *sf,            /* storage format */
	      int                 *spp,	         /* samples per pixel */
	      int                 *bps,           /* bits per pixel */
	      unsigned short      *colourmap[3],  /* RGB lut */
	      long                *ncolours,      /* nb of colours in the lut */      
	      void               **inbuffp)	 /* data buffer for this image */
{
/** Loads a TIFF image of any type.

RETURN VALUE:	int 

DESCRIPTION: Loads a TIFF image. Always allocate a 2D buffer. 

HISTORY: by Hugues Talbot and Ed Breen  9 Dec 1996

TESTS:

REFERENCES:

KEYWORDS: input/output

**/

    TIFF        *tif;
    void        **buffp;
    uint8       *bufIn, *cp, *cpi;
    uint16      *sp, *spi, photometric, sampleformat;
    uint16      *red_cm, *green_cm, *blue_cm;
    uint32      *lp, *lpi;
    double      *dp, *dpi;
    tsize_t     tf_bytesperrow;
    uint16      config;
    uint32      imWidth, imLength;
    uint16	samplesperpixel;
    uint16	bitspersample;
    float       imXposition, imYposition;
    unsigned int 	i, j, jj, k, l;
    int         n, range, nbcols;

    /* first open the TIFF file */
    
    if ((tif = TIFFOpen(fname, "r")) == NULL) {
	LIARerror("Cannot open file %s", fname);
	return(1);
    }

    /* then start by reading the correct subimage */
    if (!TIFFSetDirectory(tif, imageindex)) {
	/* this is quite legitimate to try to read beyond the last index */
	TIFFClose(tif);
	return(2);
    }
    
    /* get information on the image */
    
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLESPERPIXEL,
                          &samplesperpixel);
    
    tf_bytesperrow = TIFFScanlineSize(tif);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imWidth);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imLength);
    /* although as we use it, the X and Y positions are always
       integer, they are stored as float in the Tiff Lib */
    if (!TIFFGetField(tif, TIFFTAG_XPOSITION, &imXposition))
	imXposition = 0.0;
    if (!TIFFGetField(tif, TIFFTAG_YPOSITION, &imYposition))
	imYposition = 0.0;
    if (!TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sampleformat)) {
	/* NOTE: if this field is not present then the data is integer... */
	if (bitspersample > 16)
	    sampleformat = 2; /* signed for ints and doubles */
	else
	    sampleformat = 1; /* unsigned for shorts, chars and lower */
    }
    TIFFGetFieldDefaulted(tif, TIFFTAG_PHOTOMETRIC, &photometric);

    /* check for colourmap images (Pseudocolor) */
    range = 256L; /* old-style 8 bit palettes */
    if (photometric == PHOTOMETRIC_PALETTE) {
	TIFFGetField(tif, TIFFTAG_COLORMAP,
		     &red_cm, &green_cm, &blue_cm);
	nbcols = 1 << ((bitspersample < 8) ? 8 : bitspersample);
	for (n = 0; n < (int) nbcols; n++)
	    if ((red_cm[n] >= 256) || (green_cm[n] >= 256) ||
		(blue_cm[n] >= 256)) {
		range=65535L;
		break;
	    }
	
    }
	    
    
    /* fill what is expected of us... */
    start[0] = (int)imXposition;
    end[0] = (int)imXposition + imWidth - 1;
    start[1] = (int)imYposition;
    end[1] = (int)imYposition + imLength - 1;
    start[2] = 0;
    end[2] = 0; /* preparing for a 3d extention to TIFF */
    *spp = samplesperpixel;
    *bps = bitspersample;
    *sf = sampleformat;
    *pi = photometric;
    
    buffp = (void **) malloc(samplesperpixel * sizeof(void *));
    
    /* allocate the input buffer (must be known to the TIFF system) */
    if ((bufIn = (uint8 *)_TIFFmalloc((tf_bytesperrow+1) * sizeof(uint8))) == NULL) {
	LIARerror("Error(TIFFRead): Not enough memory");
	TIFFClose(tif);
	return(2);
    }

    for (i = 0 ; i < samplesperpixel ; i++) {
	/* prepare output image buffer */
	switch (bitspersample) {
             /* smaller than 8-bit must be expanded */
	  case 1:
	  case 4:
	    if ((buffp[i] = malloc(imLength * imWidth *  sizeof(uint8))) == NULL) {
		LIARerror("Error(TIFFRead): Not enough memory");
		_TIFFfree(bufIn);
		TIFFClose(tif);
		return(3);
	    }
	    break;
            /* these must be expanded to 16 bits */
        case 12:
            if ((buffp[i] = malloc(imLength * imWidth *  sizeof(uint16))) == NULL) {
		LIARerror("Error(TIFFRead): Not enough memory");
		_TIFFfree(bufIn);
		TIFFClose(tif);
		return(3);
	    }
	    break;
	  case 8:
	  case 16:
	  case 32:
	  case 64:
	    if ((buffp[i] = malloc(imLength * tf_bytesperrow)) == NULL) {
		LIARerror("Error(TIFFRead): Not enough memory");
		_TIFFfree(bufIn);
		TIFFClose(tif);
		return(4);
	    }
	    break;
	  
	  default:
	    LIARerror("Error(TIFFRead): unsupported word length");
	    _TIFFfree(bufIn);
	    TIFFClose(tif);
	    return(8);
	} 
    }

    /* read can be performed now */
    LIARdebug("Reading TIFF file %s ; [%d]x[%d], %d bps, %d spp, %d pi, %d sf\n",
	      fname,
	      imWidth,
	      imLength,
	      bitspersample,
	      samplesperpixel,
	      photometric,
	      sampleformat);

     /* get planar arrangement */
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);

    switch (config) {
      case PLANARCONFIG_CONTIG :
	/* all the data is arranged like this: RGBRGBRGB... */
	LIARdebug("Contig config");
	for (l = 0 ; l < imLength ; l++) {
	    if (TIFFReadScanline(tif, bufIn, l, 0) < 0) {
		LIARerror("(load_tiff): Error while reading line %d", l);
		for (i = 0 ; i < samplesperpixel ; i++) 
		    free(buffp[i]);
		_TIFFfree(bufIn);
		TIFFClose(tif);
		return 13;
	    }
	    switch (bitspersample) {
	      case 1:
		for (k = 0 ; k < samplesperpixel ; k++) {
		    cp = (uint8*)(buffp[k]);
		    for (j = 0 ; j < imWidth ; j++) {
			jj = k + j*samplesperpixel; 
			cp[j + l*imWidth] = (uint8) ((uint8)(bufIn[jj>>3] & (1 << (7-jj%8))) > 0);
		    }
		}
		break;
	      case 4:
		for (k = 0 ; k < samplesperpixel ; k++) {
		    cp = (uint8*)(buffp[k]);
		    for (j = 0 ; j < imWidth ; j++) {
			jj = k + j*samplesperpixel; 
			cp[j + l*imWidth] = (uint8) ((uint8)(bufIn[jj>>1] << 4));
		    }
		}
		break;
	      case 8:
		for (k = 0 ; k < samplesperpixel ; k++) {
		    cp = (uint8*)(buffp[k]) + l*imWidth;
		    cpi = bufIn + k;
		    for (j = 0 ; j < imWidth ; j++) {
			*cp++ = *cpi;
			cpi += samplesperpixel;
		    }
		}
		break;
            case 12: {
                /* assuming 2*12bits packed in 3*8-bits variables */
                int linelength = samplesperpixel * imWidth;
                int b, p,  byteindex;
                uint16 msb, lsb, val;

                for (b = 0, p = 0 ; b < linelength ; ++b) {
                    byteindex = (b * 3) / 2;
                    if ((b % 2) == 0) {
                        msb = bufIn[byteindex];
                        lsb = bufIn[byteindex+1];
                        val = (msb << 4) + (lsb >> 4);
                    } else {
                        msb = bufIn[byteindex] & 0x0f;
                        lsb = bufIn[byteindex+1];
                        val = (msb << 8) + lsb;
                    }
                    /* find component */
                    k = b % samplesperpixel; /* should work when samplesperpixel == 1 */
                    sp = (uint16*)(buffp[k]) + l*imWidth;
                    sp[p++] = val;
                }
            }
                break;
                
	      case 16:
		for (k = 0 ; k < samplesperpixel ; k++) {
		    sp = (uint16*)(buffp[k]) + l*imWidth;
		    spi = (uint16*)bufIn + k;
		    for (j = 0 ; j < imWidth ; j++) {
			*sp++ = *spi;
			spi += samplesperpixel;
		    }
		}
		break;
	      case 32:
		for (k = 0 ; k < samplesperpixel ; k++) {
		    lp = (uint32*)(buffp[k]) + l*imWidth;
		    lpi = (uint32*)bufIn + k;
		    for (j = 0 ; j < imWidth ; j++) {
			*lp++ = *lpi;
			lpi += samplesperpixel;
		    }
		}
		break;
	      case 64:
		for (k = 0 ; k < samplesperpixel ; k++) {
		    dp = (double*)(buffp[k]) + l*imWidth;
		    dpi = (double*)bufIn + k;
		    for (j = 0 ; j < imWidth ; j++) {
			*dp++ = *dpi;
			dpi += samplesperpixel;
		    }
		}
		break;
	    }
	}

	break;

      case PLANARCONFIG_SEPARATE:
	/* this is a little easier:
	   RRRR..., then GGG..., then BBB..., etc
	   */
	LIARdebug("Separate config");
	for (k = 0 ; k < samplesperpixel ; k++) {
	    for (l = 0 ; l < imLength ; l++) {
		/* read component k in line l */
		if (TIFFReadScanline(tif, bufIn, l, k) < 0) {
		    LIARerror("(load_tiff): Error while reading component %d in line %d", k, l);
		    for (i = 0 ; i < samplesperpixel ; i++) 
			free(buffp[i]);
		    _TIFFfree(bufIn);
		    TIFFClose(tif);
		    return 13;
		}
		/* write this line out */
		switch (bitspersample) {
                case 1:
		    cp = (uint8*)(buffp[k]);
		    for (j = 0 ; j < imWidth ; j++) {
			cp[j + l*imWidth] = (uint8) ((uint8)(bufIn[j>>3] & (1 << (7-j%8))) > 0);
		    }
		    break;
                case 4:
		    cp = (uint8*)(buffp[k]);
		    for (j = 0 ; j < imWidth ; j++) {
			cp[j + l*imWidth] = (uint8) ((uint8)(bufIn[j>>1] << 4));
		    }
		    break;
                case 8:
		    cp = (uint8*)(buffp[k]) + l*imWidth;
		    cpi = bufIn;
		    for (j = 0 ; j < imWidth ; j++)
			*cp++ = *cpi++;
		    break;

                case 12: {
                    /* assuming 2*12bits packed in 3*8-bits variables */
                    int linelength = imWidth;
                    int b, p, byteindex;
                    uint16 msb, lsb, val;
                    
                    for (b = 0, p = 0 ; b < linelength ; ++b) {
                        byteindex = (b * 3) / 2;
                        if ((b % 2) == 0) {
                            msb = bufIn[byteindex];
                            lsb = bufIn[byteindex+1];
                            val = (msb << 4) + (lsb >> 4);
                        } else {
                            msb = bufIn[byteindex] & 0x0f;
                            lsb = bufIn[byteindex+1];
                            val = (msb << 8) + lsb;
                        }
                        /* find component */
                        sp = (uint16*)(buffp[k]) + l*imWidth;
                        sp[p++] = val;
                    }
                }
                    break;
                case 16:
		    sp = (uint16*)(buffp[k]) + l*imWidth;
		    spi = (uint16*)bufIn;
		    for (j = 0 ; j < imWidth ; j++)
			*sp++ = *spi++;
		    break;
                case 32:
		    lp = (uint32*)(buffp[k]) + l*imWidth;
		    lpi = (uint32*)bufIn;
		    for (j = 0 ; j < imWidth ; j++)
			*lp++ = *lpi++;
		    break;
                case 64:
		    dp = (double*)(buffp[k]) + l*imWidth;
		    dpi = (double*)bufIn;
		    for (j = 0 ; j < imWidth ; j++)
			*dp++ = *dpi++;
		    break;
		}
	    }
	}
	break;

      default:
	LIARerror("Unrecognized pixel configuration");
	for (i = 0 ; i < samplesperpixel ; i++) 
	    free(buffp[i]);
	_TIFFfree(bufIn);
	TIFFClose(tif);
	return(16);
    }    

    /* apply the palette if needs be */
    /* reduce everything to an 8bit RGB image. Too hard otherwise */
    if ((photometric == PHOTOMETRIC_PALETTE) &&
	(*bps == 8) && (*spp == 1)) {
	uint8 *pR, *pG, *pB, *q;
	uint8 **outbuffp;
	
	outbuffp = (uint8 **) malloc(3 * sizeof(void *));
	pR = outbuffp[0] = (uint8 *)malloc(imLength*imWidth*sizeof(uint8));
	pG = outbuffp[1] = (uint8 *)malloc(imLength*imWidth*sizeof(uint8));
	pB = outbuffp[2] = (uint8 *)malloc(imLength*imWidth*sizeof(uint8));
	q = (uint8*)(buffp[0]);
	for (i = 0 ; i < imLength * imWidth ; i++) {
	    (*pR) = (255*red_cm[*q])/range;
	    (*pG) = (255*green_cm[*q])/range;
	    (*pB) = (255*blue_cm[*q])/range;
	    pR++; pG++; pB++;
	    q++;
	}
	free(buffp[0]);
	free(buffp);
	/* change the interpretation */
	*spp = 3; /* RGB */
	*pi = PHOTOMETRIC_RGB; /* same */
	*inbuffp = outbuffp;
    } else {
	/* return the allocated buffer */
	*inbuffp = buffp;
    }
	
    /* at the moment, no CLUT Hugues Talbot	11 Dec 1996 */
    *ncolours = 0;
    colourmap[0] = colourmap[1] = colourmap[2] = 0;

    TIFFClose(tif);
    _TIFFfree(bufIn);
    return 0;
}



