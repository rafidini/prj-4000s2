/*
 * $Id: savetiff.hxx,v 4.0 2003/04/28 14:44:37 hut66au Exp $
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

/*------------------------------------------------------------------------
 *
 * The save tiff include file. 
*      
*-----------------------------------------------------------------------*/

#ifndef SAVETIFF_H
#define SAVETIFF_H

/* compression schemes */
enum {
    TIFF_CMP_NONE       = 0,
    TIFF_CMP_LZW        = 1,
    TIFF_CMP_PACKBITS   = 2, // these 3 are for any format, the rest for binary only
    TIFF_CMP_CCITTG3    = 3,
    TIFF_CMP_CCITTG4    = 4,
    TIFF_CMP_RLE        = 5,
    TIFF_CMP_RLEW       = 6,
    TIFF_CMP_THUNDER    = 7
};


/* prototypes */
int setTifftype(int pixeltype,     /* pixel type */
                int imagetype,     /* image type */
                int *pi,            /* photometric interpretation */
                int *sf,            /* sample format */
                int *spp,           /* samples per pixel (this parameter is I/O) */
                int *bps);           /* bits per sample */


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
	      int    compression); /* compression scheme */

#endif // SAVETIFF_H
