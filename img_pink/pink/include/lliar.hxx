#ifndef LFINDLINE_H_INCLUDED
#define LFINDLINE_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#  include "mcimage.h"
#  include "mccodimage.h"

#  ifndef liarmax
#  define liarmax(x,y) (x) >= (y)? (x) : (y)
#  endif

#  define PERIODIC	0
#  define BRESENHAM	1

typedef void (*genffunc_CHAR) (PIX_TYPE *f,PIX_TYPE *g,PIX_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);

typedef INT4_TYPE* (*line_period) (int dx, int dy, int dz, int imwidth, int imheight, int imdepth, int *ol, int *period);


int lfdilate3d_rect(PIX_TYPE *inbuf, PIX_TYPE *outbuf, int ncol, int nrow,
		    int nslice, int dimx, int dimy, int dimz);

int lfdilate3d_line(PIX_TYPE *inbuf, PIX_TYPE *outbuf, int ncol, int nrow,
		   int nslice, int length, int dx, int dy, int dz, int type);

void rect3dminmax_CHAR(PIX_TYPE *in, int nx, int ny, int nz, int w, int b, int d, genffunc_CHAR f );

void genfmax(PIX_TYPE *f,PIX_TYPE *g,PIX_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K)	;

void genfmax_CHAR(PIX_TYPE *f,PIX_TYPE *g,PIX_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);

void genfmax_int4(INT4_TYPE *f,INT4_TYPE *g,INT4_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);

void genfmax_INT4(INT4_TYPE *f,INT4_TYPE *g,INT4_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);

void genfmax_dbl(DBL_TYPE *f,DBL_TYPE *g,DBL_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);

void genfmax_DBL(DBL_TYPE *f,DBL_TYPE *g,DBL_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);

void genfmin_char(PIX_TYPE *f,PIX_TYPE *g, PIX_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);
void genfmin_int4(INT4_TYPE *f,INT4_TYPE *g, INT4_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);
void genfmin_dbl(DBL_TYPE *f,DBL_TYPE *g, DBL_TYPE *h,INT4_TYPE *p, unsigned int nx, unsigned int K);

INT4_TYPE* bresenham3d(int dx, int dy, int dz, int imwidth, int imheight, int imdepth, int *ol, int *period);

INT4_TYPE* periodic3d(int dx, int dy, int dz, int imwidth, int imheight, int imdepth, int *ol, int *period);

int glineminmax3d(PIX_TYPE *f, int nx, int ny, int nz, int k,
		   int dx, int dy, int dz,
		   genffunc_CHAR operation,
		   line_period lineop );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LFINDLINE_H_INCLUDED
