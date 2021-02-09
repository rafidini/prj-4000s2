/*
 * $Id: readpnm.cxx,v 4.16 2008/10/27 15:34:12 hut66au Exp $
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
 * Reads the pbm, pgm and ppm formats
 * This will be particularly useful for reading just about anything
 * thanks to the ppm library (netpbm or whaterver...)
 *
 * Based on similar readers in Z-IMAGE
 *
 * Hugues Talbot	28 Jul 1998
 *
 * Many changes 2005-2008 to try and accomodate Michel Couprie's extensions
 * for PINK.
 *
 * edited by UjoImro,   25 Feb 2009
 *
 *-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <cstring>

//#include <pink.h>
#include "readpnm.h"
#include "ujimage.hpp"
//#include "imview.hxx"
//#include "imSystem.hxx"
//#include "../imageIO.hxx"
//#include "readpnm.hxx"

// Stuff that are used from the commented includes, but I didn't want to include them just for them

// image types and pixel types
typedef enum {
  IM_INVALID = 0,
  IM_BINARY=10,
  IM_CHAR=31,    IM_INT1 = 31, IM_UINT1 = 30,
  IM_SHORT = 35, IM_INT2 = 35, IM_UINT2 = 37,
  IM_INT = 40,   IM_INT4 = 40, IM_UINT4 = 42,
  IM_INT8 = 43,  IM_UINT8 = 44, 
  IM_FLOAT=45,   IM_DOUBLE=50
} pixtype;


/*-- Multi-component image types --*/
typedef enum { IM_ERROR=0, IM_UNSPEC=1, IM_SINGLE=2, IM_SPECTRUM=3, IM_RGB=4, IM_HLS=5 } imgtype;




//can't hurt that much, can it?
#define HAVE_PINK_PGM_EXTENSIONS


static int load_pbm_ascii(FILE *fp,
			  int   start[3],
			  int   end[3],
			  pixtype *pixtype,
			  imgtype *imgtype,
			  void **inbuffp);

static int load_pbm_raw(FILE *fp,
			int   start[3],
			int   end[3],
			pixtype *pixtype,
			imgtype *imgtype,
			void **inbuffp);

static int load_pgm_ascii(FILE *fp,
			  int   start[3],
			  int   end[3],
			  pixtype *pixtype,
			  imgtype *imgtype,
			  void **inbuffp);

static int load_pgm_raw(FILE *fp,
			int   start[3],
			int   end[3],
			pixtype *pixtype,
			imgtype *imgtype,
			void **inbuffp);

static int load_ppm_ascii(FILE *fp,
                          int   start[3],
                          int   end[3],
                          pixtype *pixtype,
                          imgtype *imgtype,
                          void **inbuffp);


static int load_ppm_raw(FILE *fp,
			int   start[3],
			int   end[3],
			pixtype *pixtype,
			imgtype *imgtype,
			void **inbuffp);
            
static int load_MC_long_ppm_raw(FILE *fp, 
				int start[3], 
				int end[3], 
				pixtype *thepixeltype, 
				imgtype *theimgtype,
                                void **inbuffp);

static int load_MC_float_ppm_raw(FILE *fp, 
				 int start[3], 
				 int end[3], 
				 pixtype *thepixeltype, 
				 imgtype *theimgtype,
				 void **inbuffp);

static int getint(FILE *fp, toberead optional=IV_MANDATORY);

static bool garbage;
static long numgot;

pink::float_image readPNMImage(const std::string & filename)
{
  void      *p=0;
  int       start[3], end[3], nbsamples = 0;
  pixtype   thepixtype;
  imgtype   theimgtype;
  FILE *fp;
  char  magic_num[3];
  int   retval = 0;
  
  if ((fp = fopen(filename.c_str(),"rb")) == NULL){
    std::cout << "problems with reading file '" << filename << "'" << std::endl;
    pink_error("couldn't open the file");
  };

  /* in case nothing is read */
  start[0] = start[1] = start[2] = 0;
  end[0] = end[1] = end[2] = 0;
  thepixtype = IM_UINT1;
  theimgtype = IM_SINGLE;

  fscanf (fp,"%2s",magic_num);

  switch(magic_num[1]) {
  case '1':
    retval = load_pbm_ascii(fp, start, end, &thepixtype, &theimgtype, &p);
    nbsamples = 1;
    break;
  case '2':
    retval = load_pgm_ascii(fp, start, end, &thepixtype, &theimgtype, &p);
    nbsamples = 1;
    break;
  case '3':
    retval = load_ppm_ascii(fp, start, end, &thepixtype, &theimgtype, &p);
    nbsamples = 3;
    break;
  case '4':
    retval = load_pbm_raw(fp, start, end, &thepixtype, &theimgtype, &p);
    nbsamples = 1;
    break;
  case '5':
    retval = load_pgm_raw(fp, start, end, &thepixtype, &theimgtype, &p);
    nbsamples = 1;
    break;
  case '6':
    retval = load_ppm_raw(fp, start, end, &thepixtype, &theimgtype, &p);
    nbsamples = 3;
    break;
  
#ifdef HAVE_PINK_PGM_EXTENSIONS
  case '7':
    pink_error("readPNMImage: obsolete PINK PGM format\n");
    break;
        
  case '8':
    std::cout << "Michel Couprie's raw long PGM\n";
    retval = load_MC_long_ppm_raw(fp, start, end, &thepixtype, &theimgtype, &p);
    nbsamples = 1;
    break;       // raw long 2D-3D
        
  case '9':
    // raw float 2D-3D
    std::cout << "Michel Couprie's raw float format PGM\n";
    retval = load_MC_float_ppm_raw(fp, start, end, &thepixtype, &theimgtype, &p);
    nbsamples = 1;
    break; // raw float format 2D-3D
        
  case 'A':
    // ascii float 2D-3D
    pink_error("PINK ascii float PGM extension not implemented yet, nag the developer\n");
    break;
        
  case 'B':
    // ascii long 2D-3D
    pink_error("PINK ascii long PGM extension not implemented yet, nag the developer\n");
    break;  
        
        
#endif

  default:
    pink_error("Unsupported PNM format\n");
    retval = 100; // unsupported format
    break;
  }

//     if (retval == 0) {
// 		if (garbage)
// 		    error("readPNMImage: There was some garbage in this picture\n");
// 		IOBlackBox->setCurrBuffp((void **)p);
// 		IOBlackBox->setCurrImgWidth(end[0]-start[0]+1);
// 		IOBlackBox->setCurrImgHeight(end[1]-start[1]+1);
// 		IOBlackBox->setCurrImgThickness(end[2]-start[2]+1);
// 		IOBlackBox->setXOffset(start[0]);
// 		IOBlackBox->setYOffset(start[1]);
// 		IOBlackBox->setZOffset(start[2]);
// 		IOBlackBox->setCurrImgType((imgtype)theimgtype);
// 		IOBlackBox->setCurrPixType((pixtype)thepixtype);
// 		IOBlackBox->setCurrImgNbComps(1); // only a single image in a PNM file
// 		IOBlackBox->setCurrImgNbSamples(nbsamples);
// 		IOBlackBox->setCurrImgColourMap(0);
// 		IOBlackBox->setImgDesc("PNM");
//     }

  if (( theimgtype != IM_SPECTRUM ) && (theimgtype != IM_SINGLE)) {
    std::cout << "theimgtype =" << theimgtype << std::endl;
    pink_error("color images are out of question.");    
  };

  pink::types::vint dim;

  if ((end[2]-start[2]+1)==1) { // the image is 2D
    dim.resize(2,-1);
    FOR(w,2) dim[w] = end[w]-start[w]+1;
  } else {
    dim.resize(3,-1);
    FOR(w,3) dim[w] = end[w]-start[w]+1;
  };  

  pink::float_image presult(dim);

  if ((end[2]-start[2]+1)==1) { // the image is 2D
    pink::types::vint curr(2,-1);
    int n = dim.prod();
    FOR(q, n) {
      curr[0]=q % dim[0] ;
      curr[1]=q / dim[0];
      switch (thepixtype){
      case (IM_UINT1):
	presult[curr]=double(((unsigned char*)((void**)p)[0])[q]);
	break;
      case (IM_SHORT):
	presult[curr]=double(((unsigned short*)((void**)p)[0])[q]);
	break;
      case (IM_INT4):
	presult[curr]=double(((unsigned int*)((void**)p)[0])[q]);
	break;
      case (IM_FLOAT):
	presult[curr]=double(((float*)((void**)p)[0])[q]);
	break;
      default:
	std::cout << "image type= "<< thepixtype << std::endl;
	pink_error("3D, This imagetype is not implemented (only tested on char and short, int4 and float, exiting...)");
	break;
      };
    };
  } else { // the image is 3D
    pink::types::vint curr(3,-1);
    index_t n = dim.prod();
    FOR(q, n){
      curr[0]=(q % ( dim[1] * dim[0] )) % dim[0];
      curr[1]=(q % ( dim[1] * dim[0] )) / dim[0];
      curr[2]= q / ( dim[1] * dim[0] );

      switch (thepixtype){
      case (IM_UINT1):
	presult[curr]=float(((unsigned char*)((void**)p)[0])[q]);
	break;
      case (IM_SHORT):
	presult[curr]=float(((unsigned short*)((void**)p)[0])[q]);
	break;
      case (IM_INT4):
	presult[curr]=float(((unsigned int*)((void**)p)[0])[q]);
	break;
      case (IM_FLOAT):
	presult[curr]=double(((float*)((void**)p)[0])[q]);
	break;
      default:
	std::cout << "image type= "<< thepixtype << std::endl;
	pink_error("3D, This imagetype is not implemented (only tested on char and short, int4 and float, exiting...)");
	break;
      };
      
    };
  };
  
  fclose(fp);
  
  return presult;
}

//
// read the next integer and returns true if it can.
// if it can't it returns false
// if optional is set to 1 or true, then if we encounter a carriage 
// return or EOF before, then the integer is not read
// the function then returns 0.
// if optional is set to '2', then a '\n' or '\r' or both are explicitly read
static int getint(FILE *fp, toberead optional)
{
  int c, i, firstchar;

  /* note:  if it sees a '#' character, all characters from there to end of
     line are appended to the comment std::string */

  /* skip forward to start of next number */
  c = getc(fp);
  while (1) {
    /* eat comments */
    if (c=='#') {   /* if we're at a comment, read to end of line */
      char cmt[256], *sp;

      // put the commment in the comment std::string. Probably useless
      sp = cmt;  firstchar = 1;
      while (1) {
	c=getc(fp);
	if (firstchar && c == ' ') firstchar = 0;  /* lop off 1 sp after # */
	else {
	  if (c == '\n' || c == EOF) 
	    break;
	  if ((sp-cmt)<250) *sp++ = c;
	}
      }
      *sp++ = '\n';
      *sp   = '\0';
    }

    if (c==EOF) return 0;
      
    if (c>='0' && c<='9') break;   /* we've found what we were looking for */

    if ((optional==IV_OPTIONAL) &&
	((c == '\n') || (c == '\r'))) {
      std::cout << "getint: carriage return read instead of optional char\n";
      return 0; /* we found an end-of-line instead of a number but that's OK, we didn't really want it :-) */
    }
      
    /* see if we are getting garbage (non-whitespace) */
    if (c!=' ' && c!='\t' && c!='\r' && c!='\n' && c!=',') {
      garbage = true;
    }
    c = getc(fp);
  }
  /* we're at the start of a number, continue until we hit a non-number */
  i = 0;
  while (1) {
    i = (i*10) + (c - '0');
    c = getc(fp);
    if (c==EOF) return i;
    if (c<'0' || c>'9') {
      // this is necessary for the "optional read" logic
      // specs say last number (bit depth) is followed by a
      // single whitespace, which must get eaten
      if (optional != IV_FINAL)
	ungetc(c, fp); // if we don't unget, the \n or \r following a number will get eaten
      break;
    }
  }

  numgot++;
  return i;
}

int pnmnbsubfiles(const char *)
{
  return 1; // particularly simple
}

static int load_pgm_ascii(FILE *fp,
			  int   start[3],
			  int   end[3],
			  pixtype *pixtype,
			  imgtype *imgtype,
			  void **inbuffp)
{
  int             retval = 0;
  void          **buffp;
  int             ncol, nrow, nslice, maxgrey, k, j, i;
  unsigned char  *cp;
  unsigned short *sp;
    
  garbage = false;
  numgot = 0;

  ncol = getint(fp);
  nrow = getint(fp);
  nslice = getint(fp, IV_OPTIONAL); // maybe read a 3rd dimension
  nslice = (nslice == 0) ? 1:nslice;
              
  maxgrey = getint(fp, IV_FINAL);

  std::cout << "load_pgm_ascii: ( << " << ncol << "x" << nrow << "x" << nslice << ")," << maxgrey << "grey-levels\n";

  if (maxgrey <= 1)
    *pixtype = IM_BINARY;
  else if (maxgrey <= 255)
    *pixtype = IM_UINT1;
  else
    *pixtype = IM_UINT2;

  *imgtype = IM_SPECTRUM; // in all cases for a first approximation

  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  end[0] = ncol-1;
  end[1] = nrow-1;
  end[2] = nslice-1; // maybe 3D data in there.

  buffp = (void **)malloc(sizeof(void *));
    
  if ((*pixtype == IM_BINARY) || (*pixtype == IM_UINT1)) {
    // get all the data
    buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
    cp = (unsigned char *)(buffp[0]);
    for (k = 0 ; k < nslice ; ++k) {
      for (j = 0 ; j < nrow ; j++) {
	for (i = 0 ; i < ncol ; i++) {
	  *cp++ = (unsigned char)getint(fp);
	}
      }
    }
  } else { // short
    buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned short));
    sp = (unsigned short *)(buffp[0]);
    for (k = 0 ; k < nslice ; ++k) {
      for (j = 0 ; j < nrow ; j++) {
	for (i = 0 ; i < ncol ; i++) {
	  *sp++ = (unsigned short)getint(fp);
	}
      }
    }
  }

  *inbuffp = buffp;
    
  return retval;
}

//
// Read short values found in RAW pgm output.
//
// Specs http://netpbm.sourceforge.net/doc/pgm.html
// says MSB is first (like sparcs, motorola 68k)
// *unlike* PCs : shorts are read little-endian fashion

static int getshort(FILE *fp)
{
  int c1, c2;

  c1 = getc(fp);
  if (c1 == EOF)
    return 0;
  c2 = getc(fp);
  if (c2 == EOF)
    return 0;

  numgot++;

  return (c1 << 8) | c2;
}


// this is not currently used, dead code
#if 0 

// reads any signed value of given length, returned as long.
// 64-bit maxi
static long getanysigned(FILE *fp, unsigned int length)
{
  int ci[8];
  unsigned int i;
  long retval; 
    
  assert((length <= sizeof(long)) && (length <= 8));
    
  memset(ci, 0, 8);
  for (i = 0 ; i < length ; ++i) {
    ci[i] = getc(fp);
    if (ci[i] == EOF)
      return 0;
    numgot++;   
  }
    
  retval = 0;
  for (i = 0 ; i < length ; ++i) {
    retval |= ci[i] << (8*i);   
  }
    
  return retval;
}
#endif // 0 - dead code

static int load_pgm_raw(FILE *fp,
			int   start[3],
			int   end[3],
			pixtype *pixtype,
			imgtype *imgtype,
			void **inbuffp)
{
  int            retval = 0;
  void         **buffp;
  int            ncol, nrow, nslice, maxgrey, k, j, i;
  unsigned char *cp;
  unsigned short *sp;
    
  garbage = false;
  numgot = 0;

  ncol = getint(fp);
  nrow = getint(fp);
  nslice = getint(fp, IV_OPTIONAL); // optionnally read a 3rd dimension
  nslice = (nslice == 0) ? 1:nslice; // at least one slice
    
  maxgrey = getint(fp, IV_FINAL);

  std::cout << "load_pgm_raw: (" << ncol << "x" << nrow << "x" << nslice << "), " << maxgrey << " grey-levels\n";

  if (maxgrey <= 1)
    *pixtype = IM_BINARY;
  else if (maxgrey <= 255)
    *pixtype = IM_UINT1;
  else *pixtype = IM_UINT2; // unsigned short int is the larger type

  *imgtype = IM_SPECTRUM; // in all cases for a first approximation

  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  end[0] = ncol-1;
  end[1] = nrow-1;
  end[2] = nslice-1; // maybe 3D data in there.

  buffp = (void **)malloc(sizeof(void *));
    
  if ((*pixtype == IM_BINARY) || (*pixtype == IM_UINT1)) {
    // get all the data
    buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
    cp = (unsigned char *)(buffp[0]);
    numgot = fread(cp, (size_t) 1, (size_t) nrow*ncol*nslice, fp);
  } else { // unsigned short
    buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned short));
    sp = (unsigned short *)(buffp[0]);
    for (k = 0 ; k < nslice ; ++k) {
      for (j = 0 ; j < nrow ; j++) {
	for (i = 0 ; i < ncol ; i++) {
	  *sp++ = getshort(fp);
	}
      }
    }
  }

  *inbuffp = buffp;
    
  return retval;
}


static int getbit(FILE *fp)
{
  int c;

  /* skip forward to start of next number */
  c = getc(fp);
  while (1) {
    /* eat comments */
    if (c=='#') {   /* if we're at a comment, read to end of line */
      char cmt[256], *sp;
      sp = cmt;
      while (1) {
        c=getc(fp);
        if (c == '\n' || c == EOF) break;

        if ((sp-cmt)<250) *sp++ = c;
      }
      *sp++ = '\n';
      *sp = '\0';

    }
    if (c==EOF) return 0;
    if (c=='0' || c=='1') break;   /* we've found what we were looking for */

    /* see if we are getting garbage (non-whitespace) */
    if (c!=' ' && c!='\t' && c!='\r' && c!='\n' && c!=',') garbage=true;

    c = getc(fp);
  }
  numgot++;
  return(c-'0');
}



static int load_pbm_ascii(FILE *fp,
			  int   start[3],
			  int   end[3],
			  pixtype *pixtype,
			  imgtype *imgtype,
			  void **inbuffp)
{
  int            retval = 0;
  void         **buffp;
  int            ncol, nrow, nslice, j, i;
  unsigned char *cp;
    
  garbage = false;
  numgot = 0;

  ncol = getint(fp);
  nrow = getint(fp);
  nslice=getint(fp,IV_OPTIONAL); 

  nslice = (nslice==0) ? 1:nslice; // at least one slice
    
  *pixtype = IM_BINARY;
  *imgtype = IM_SPECTRUM; // in all cases for a first approximation

  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  end[0] = ncol-1;
  end[1] = nrow-1;
  end[2] = nslice-1; // no 3D data in there.

  buffp = (void **)malloc(sizeof(void *));
    
  // get all the data
  buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
  cp = (unsigned char *)(buffp[0]);
  for (j = 0 ; j < nrow ; j++) {
    for (i = 0 ; i < ncol ; i++) {
      *cp++ = (unsigned char)getbit(fp);
    }
  }

  *inbuffp = buffp;
    
  return retval;
}

static int load_pbm_raw(FILE *fp,
			int   start[3],
			int   end[3],
			pixtype *pixtype,
			imgtype *imgtype,
			void **inbuffp)
{
  int            retval = 0;
  void         **buffp;
  int            ncol, nrow, nslice, l, k, j, i;

  unsigned char *cp;
    
  garbage = false;
  numgot = 0;

  ncol = getint(fp);
  nrow = getint(fp);
  nslice = getint(fp, IV_OPTIONAL); // get a third dimension if it is present in the file
  if (nslice == 0) // we didn't get a third number, but we do have one slice
    nslice = 1;	

  *pixtype = IM_BINARY;
  *imgtype = IM_SPECTRUM; // in all cases for a first approximation

  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  end[0] = ncol-1;
  end[1] = nrow-1;
  end[2] = nslice-1; // maybe 3D data in there.

  buffp = (void **)malloc(sizeof(void *));
    
  // get all the data
  buffp[0] = malloc(ncol * nrow * nslice * sizeof(unsigned char));
  cp = (unsigned char *)(buffp[0]);

  for (l = 0 , k = 0 ; l < nslice ; ++l) {
    for (j = 0 ; j < nrow ; j++) {
      for (i = 0 ; i < ncol ; i++) {
	if (i%8 == 0) {
	  // read one char, ignore errors
	  k = getc(fp);
	  if (k == EOF) garbage = true;
	}
	// read the leftmost bit
	*cp++ = (unsigned char)(k&0x80 ? 1: 0);
	k <<= 1; // double k
      }
    }
  }
 

  *inbuffp = buffp;
    
  return retval;
}


static int load_ppm_ascii(FILE *fp,
			  int   start[3],
			  int   end[3],
			  pixtype *pixtype,
			  imgtype *imgtype,
			  void **inbuffp)
{
  int            retval = 0;
  void         **buffp;
  int            ncol, nrow, nslice, maxgrey, k, j, i;
  unsigned char *cpR, *cpG, *cpB;
  unsigned short *spR, *spG, *spB;
    
  garbage = false;
  numgot = 0;

  ncol = getint(fp);
  nrow = getint(fp);
  nslice = getint(fp, IV_OPTIONAL); // read optional 3rd dimension
  maxgrey = getint(fp, IV_FINAL);
   
  nslice = (nslice == 0) ? 1:nslice; // we do have one slice
    
  if (maxgrey <= 1)
    *pixtype = IM_BINARY;
  else if (maxgrey <= 255)
    *pixtype = IM_UINT1;
  else *pixtype = IM_UINT2; // unsigned short int is the larger type

  *imgtype = IM_RGB; // in all cases for a first approximation

  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  end[0] = ncol-1;
  end[1] = nrow-1;
  end[2] = nslice-1; // no 3D data in there.

  buffp = (void **)malloc(3*sizeof(void *));
    
  if ((*pixtype == IM_BINARY) || (*pixtype == IM_UINT1)) {
    // get all the data
    buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
    buffp[1] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
    buffp[2] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
    cpR = (unsigned char *)(buffp[0]);
    cpG = (unsigned char *)(buffp[1]);
    cpB = (unsigned char *)(buffp[2]);
    for (k = 0 ; k < nslice ; ++k) {
      for (j = 0 ; j < nrow ; j++) {
	for (i = 0 ; i < ncol ; i++) {
	  *cpR++ = (unsigned char)getint(fp);
	  *cpG++ = (unsigned char)getint(fp);
	  *cpB++ = (unsigned char)getint(fp);
	}
      }
    }
  } else { // short
    buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned short));
    buffp[1] = malloc(ncol*nrow*nslice*sizeof(unsigned short));
    buffp[2] = malloc(ncol*nrow*nslice*sizeof(unsigned short));
    spR = (unsigned short *)(buffp[0]);
    spG = (unsigned short *)(buffp[1]);
    spB = (unsigned short *)(buffp[2]);
    for (k = 0 ; k < nslice ; ++k) {
      for (j = 0 ; j < nrow ; j++) {
	for (i = 0 ; i < ncol ; i++) {
	  *spR++ = (unsigned short)getint(fp);
	  *spG++ = (unsigned short)getint(fp);
	  *spB++ = (unsigned short)getint(fp);
	}
      }
    }
  }

  *inbuffp = buffp;
    
  return retval;
}

/* this is fairly iffy */
/* Here are the assumptions :
 * 1- File contains either 2 or 3 dimensions, then a carriage return, then
 *    the maxval, all in ASCII, then the data in binary form.
 * 2- The data is little-endian (x86 ordering)
 * 3- longs are in fact signed 32 bits integers here.
 */
static int load_MC_long_ppm_raw(FILE *fp,
				int   start[3],
				int   end[3],
				pixtype *pixtype,
				imgtype *imgtype,
				void **inbuffp)
{
  int                 retval = 0, numread = 0;
  void              **buffp = NULL;
  int                 ncol, nrow, nslice, maxgrey;
    
  garbage = false;
  numgot = 0;
  ncol = getint(fp);
  nrow = getint(fp);
  // read the next int unless we find a '\n' first
  nslice = getint(fp, IV_OPTIONAL);
    
  if (nslice == 0)
    nslice = 1; // we have at least one slice

  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  end[0] = ncol-1;
  end[1] = nrow-1;
  end[2] = nslice-1; // maybe some 3D data in there.

  maxgrey = getint(fp, IV_FINAL);
  *pixtype = IM_INT4;    
  *imgtype = IM_SINGLE;
    
  // one buffer
  buffp = (void **)malloc(sizeof(void*));
  buffp[0] = (void*) malloc(ncol*nrow*nslice*sizeof(int32_t));
  numread = fread(buffp[0], sizeof(int32_t), ncol*nrow*nslice, fp);
  if (numread < ncol*nrow*nslice) {
    garbage = true;
    retval = -1;   
  } else {
    numgot += numread;   
  }
    
    
  *inbuffp = buffp;
    
  return retval; 
}

/* Here are the assumptions :
 * 1- File contains either 2 or 3 dimensions, then a carriage return, then
 *    the maxval, all in ASCII, then the data in binary form.
 * 2- The data is little-endian (x86 ordering)
 * 3- longs are in fact signed 32 bits integers here.
 */
static int load_MC_float_ppm_raw(FILE *fp,
                                 int   start[3],
                                 int   end[3],
                                 pixtype *pixtype,
                                 imgtype *imgtype,
                                 void **inbuffp)
{
  int                 retval = 0, numread = 0;
  void              **buffp = NULL;
  int                 ncol, nrow, nslice, maxgrey;
    
  garbage = false;
  numgot = 0;
  ncol = getint(fp);
  nrow = getint(fp);
  // read the next int unless we find a '\n' first
  nslice = getint(fp, IV_OPTIONAL);
    
  if (nslice == 0)
    nslice = 1; // we have at least one slice

  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  end[0] = ncol-1;
  end[1] = nrow-1;
  end[2] = nslice-1; // maybe some 3D data in there.

  maxgrey = getint(fp, IV_FINAL); // should be a zero

  std::cout << "Got a maxgrey value of " << maxgrey << ", expected zero\n";
  *pixtype = IM_FLOAT;    
  *imgtype = IM_SINGLE;
    
  // one buffer
  buffp = (void **)malloc(sizeof(void*));
  buffp[0] = (void*) malloc(ncol*nrow*nslice*sizeof(float));
  numread = fread(buffp[0], sizeof(float), ncol*nrow*nslice, fp);
  if (numread < ncol*nrow*nslice) {
    std::cout << "fread insufficient : got " << numread << " pixels, expected " << ncol*nrow*nslice << std::endl;
    garbage = true;
    retval = -1;   
  } else {
    numgot += numread;   
  }
    
    
  *inbuffp = buffp;
    
  return retval; 
}

static int load_ppm_raw(FILE *fp,
			int   start[3],
			int   end[3],
			pixtype *pixtype,
			imgtype *imgtype,
			void **inbuffp)
{
  int            retval = 0;
  void         **buffp;
  int            ncol, nrow, nslice, maxgrey, k, j, i;
  unsigned char *cpR, *cpG, *cpB;
  unsigned short *spR, *spG, *spB;
    
  garbage = false;
  numgot = 0;

  ncol = getint(fp);
  nrow = getint(fp);
  nslice = getint(fp, IV_OPTIONAL); // optionnally read a 3rd dimension.
  if (nslice == 0)
    nslice = 1; // we do have one slice
  maxgrey = getint(fp, IV_FINAL);

  std::cout << "load_ppm_raw: ( << " << ncol << "x" << nrow << "x" << nslice << ")," << maxgrey << "grey-levels\n";

  if (maxgrey <= 1)
    *pixtype = IM_BINARY;
  else if (maxgrey <= 255)
    *pixtype = IM_UINT1;
  else *pixtype = IM_UINT2; // short int is the larger type

  *imgtype = IM_RGB; // in all cases for a first approximation

  start[0] = 0;
  start[1] = 0;
  start[2] = 0;
  end[0] = ncol-1;
  end[1] = nrow-1;
  end[2] = nslice-1; // maybe 3D data in there.

  buffp = (void **)malloc(3*sizeof(void *));
    
  if ((*pixtype == IM_BINARY) || (*pixtype == IM_UINT1)) {
    // get all the data
    // get all the data
    buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
    buffp[1] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
    buffp[2] = malloc(ncol*nrow*nslice*sizeof(unsigned char));
    cpR = (unsigned char *)(buffp[0]);
    cpG = (unsigned char *)(buffp[1]);
    cpB = (unsigned char *)(buffp[2]);
    for (k = 0 ; k < nslice ; ++k) {
      for (j = 0 ; j < nrow ; j++) {
	for (i = 0 ; i < ncol ; i++) {
	  *cpR++ = getc(fp);
	  *cpG++ = getc(fp);
	  *cpB++ = getc(fp);
	}
      }
    }
  } else { // short
    buffp[0] = malloc(ncol*nrow*nslice*sizeof(unsigned short));
    buffp[1] = malloc(ncol*nrow*nslice*sizeof(unsigned short));
    buffp[2] = malloc(ncol*nrow*nslice*sizeof(unsigned short));
    spR = (unsigned short *)(buffp[0]);
    spG = (unsigned short *)(buffp[1]);
    spB = (unsigned short *)(buffp[2]);
    for (k = 0 ; k < nslice ; ++k) {
      for (j = 0 ; j < nrow ; j++) {
	for (i = 0 ; i < ncol ; i++) {
	  *spR++ = getshort(fp);
	  *spG++ = getshort(fp);
	  *spB++ = getshort(fp);
	}
      }
    }
  }

  *inbuffp = buffp;
    
  return retval;
}
