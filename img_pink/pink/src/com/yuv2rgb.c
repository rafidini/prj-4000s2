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
/* \file yuv2rgb.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* yuv2rgb.c - construct 1 ppm from 3 raw files:
** - basename.Y : The Luminance chunk at the size of the Image
** - basename.U : The Chrominance chunk U at 1/4
** - basename.V : The Chrominance chunk V at 1/4
** The subsampled U and V values are made by arithmetic mean.
**
** If ccir601 is defined, the produced YUV triples have been scaled again
** to fit into the smaller range of values for this standard.
**
** Michel Couprie  -  October 1998
**
** Based on yuvsplittoppm.c
** by Marcel Wijkstra <wijkstra@fwi.uva.nl>
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose and without fee is hereby granted, provided
** that the above copyright notice appear in all copies and that both that
** copyright notice and this permission notice appear in supporting
** documentation.  This software is provided "as is" without express or
** implied warranty.
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

/* x must be signed for the following to work correctly */
#define limit(x) (((x>0xffffff)?0xff0000:((x<=0xffff)?0:x&0xff0000))>>16)

typedef uint32_t pixel;
#define PGM_MAXMAXVAL 255
#define CCIR601 1

int main(int argc, char **argv)
{
	FILE *vf, *uf, *yf;
	int32_t rows, cols, row;
	register int32_t col;
	char *usage=(char *)"basename width height [-split]";
	int32_t u, v, y0, y1, y2, y3, u0, u1, u2, u3, v0, v1, v2, v3;
	uint8_t *y1buf, *y2buf, *ubuf, *vbuf;
	char ufname[256], vfname[256], yfname[256];
/* Whether to create YUV in JFIF(JPEG) or CCIR.601(MPEG) scale */
	int32_t	ccir601 = CCIR601;
        struct xvimage *imr, *img, *imb;
        uint8_t *pr, *pg, *pb;
        int32_t split = 0;

	if ((argc>5) || (argc<4)) { printf("usage: %s %s\n", argv[0], usage); exit(1); }

        if (argc==5)
	{
          if (strcmp(argv[4],"-split") == 0)
            split = 1;
	  else
          { printf("usage: %s %s\n", argv[0], usage); exit(1); }
	}

	strcpy(ufname,argv[1]);
	strcpy(vfname,argv[1]);
	strcpy(yfname,argv[1]);
 
	strcat(ufname,".U");
	strcat(vfname,".V");
	strcat(yfname,".Y");

	uf = fopen(ufname,"rb");
	vf = fopen(vfname,"rb");
	yf = fopen(yfname,"rb");

	if(!(uf && vf && yf)) {
 	 perror("error opening input files");
 	 exit(1);
	}

        cols = atoi(argv[2]);
        rows = atoi(argv[3]);
        if (cols <= 0 || rows <= 0)
        { printf("usage: %s %s\n", argv[0], usage); exit(1); }

        imr = allocimage(NULL, cols, rows, 1, VFF_TYP_1_BYTE);
        img = allocimage(NULL, cols, rows, 1, VFF_TYP_1_BYTE);
        imb = allocimage(NULL, cols, rows, 1, VFF_TYP_1_BYTE);
        pr = UCHARDATA(imr);
        pg = UCHARDATA(img);
        pb = UCHARDATA(imb);

        if(cols & 1) fprintf(stderr,
                             "%s: Warning: odd columns count, exceed ignored\n",
                             argv[0]);
        if(rows & 1) fprintf(stderr,
                             "%s: Warning: odd rows count, exceed ignored\n",
                             argv[0]);

	y1buf = (uint8_t *)calloc(1,cols);
	y2buf = (uint8_t *)calloc(1,cols);
	ubuf = (uint8_t *)calloc(1,cols);
        vbuf = (uint8_t *)calloc(1,cols);

	for (row = 0; row < (rows & ~1); row += 2) {
		uint8_t *y1ptr,*y2ptr,*uptr,*vptr;

		fread(y1buf, (cols & ~1), 1, yf);
		fread(y2buf, (cols & ~1), 1, yf);
		fread(ubuf, cols/2, 1, uf);
		fread(vbuf, cols/2, 1, vf);

		y1ptr = y1buf; y2ptr = y2buf; vptr = vbuf; uptr = ubuf;

		for (col = 0 ; col < (cols & ~1); col += 2) {
			int32_t r0,g0,b0,r1,g1,b1,r2,g2,b2,r3,g3,b3;

			y0 = (int32_t) *y1ptr++;
			y1 = (int32_t) *y1ptr++;
			y2 = (int32_t) *y2ptr++;
			y3 = (int32_t) *y2ptr++;

			u = (int32_t) ((*uptr++) - 128);
			v = (int32_t) ((*vptr++) - 128);

			if (ccir601) {
				y0 = ((y0-16)*255)/219;
				y1 = ((y1-16)*255)/219;
				y2 = ((y2-16)*255)/219;
				y3 = ((y3-16)*255)/219;

				u  = (u*255)/224 ;
				v  = (v*255)/224 ;
			}
			/* mean the chroma for subsampling */

			u0=u1=u2=u3=u;
			v0=v1=v2=v3=v;


/* The inverse of the JFIF RGB to YUV Matrix for $00010000 = 1.0

[Y]   [65496        0   91880][R]
[U] = [65533   -22580  -46799[G]
[V]   [65537   116128      -8][B]

*/

			r0 = 65536 * y0               + 91880 * v0;
			g0 = 65536 * y0 -  22580 * u0 - 46799 * v0;
                        b0 = 65536 * y0 + 116128 * u0             ;

			r1 = 65536 * y1               + 91880 * v1;
			g1 = 65536 * y1 -  22580 * u1 - 46799 * v1;
                        b1 = 65536 * y1 + 116128 * u1             ;

			r2 = 65536 * y2               + 91880 * v2;
			g2 = 65536 * y2 -  22580 * u2 - 46799 * v2;
                        b2 = 65536 * y2 + 116128 * u2             ;

			r3 = 65536 * y3               + 91880 * v3;
			g3 = 65536 * y3 -  22580 * u3 - 46799 * v3;
                        b3 = 65536 * y3 + 116128 * u3             ;

			r0 = limit(r0);
			r1 = limit(r1);
			r2 = limit(r2);
			r3 = limit(r3);
			g0 = limit(g0);
			g1 = limit(g1);
			g2 = limit(g2);
			g3 = limit(g3);
			b0 = limit(b0);
			b1 = limit(b1);
			b2 = limit(b2);
			b3 = limit(b3);

                        pr[row * cols + col] = (uint8_t)r0;
                        pr[row * cols + col + 1] = (uint8_t)r1;
                        pr[(row + 1) * cols + col] = (uint8_t)r2;
                        pr[(row + 1) * cols + col + 1] = (uint8_t)r3;
                        pg[row * cols + col] = (uint8_t)g0;
                        pg[row * cols + col + 1] = (uint8_t)g1;
                        pg[(row + 1) * cols + col] = (uint8_t)g2;
                        pg[(row + 1) * cols + col + 1] = (uint8_t)g3;
                        pb[row * cols + col] = (uint8_t)b0;
                        pb[row * cols + col + 1] = (uint8_t)b1;
                        pb[(row + 1) * cols + col] = (uint8_t)b2;
                        pb[(row + 1) * cols + col + 1] = (uint8_t)b3;
		}
	}

        if (split)
	{
	  strcpy(ufname,argv[1]);
	  strcpy(vfname,argv[1]);
	  strcpy(yfname,argv[1]);
	  strcat(ufname,".R");
	  strcat(vfname,".G");
	  strcat(yfname,".B");
          writeimage(imr, ufname);
          writeimage(img, vfname);
          writeimage(imb, yfname);
	}
        else
	{
	  strcpy(ufname,argv[1]);
	  strcat(ufname,".ppm");
          writergbimage(imr, img, imb, ufname);
	}
        freeimage(imr);
        freeimage(img);
        freeimage(imb);
        fclose(yf);
	fclose(uf);
	fclose(vf);
	exit(1);
  return 0;
}
