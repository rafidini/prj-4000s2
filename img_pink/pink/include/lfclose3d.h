#ifndef LFCLOSE3D_H_INCLUDED
#define LFCLOSE3D_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

int lfclose3d_rect(PIX_TYPE *inbuf, PIX_TYPE *outbuf, int ncol, int nrow,
		    int nslice, int dimx, int dimy, int dimz);

int lfclose3d_line(PIX_TYPE *inbuf, PIX_TYPE *outbuf,
		   int ncol, int nrow, int nslice, int length,
		   int dx, int dy, int dz, int type);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LFCLOSE3D_H_INCLUDED
