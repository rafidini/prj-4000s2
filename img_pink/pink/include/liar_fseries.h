/*
 * header for the fseries
 *
 * Hugues Talbot	 3 Jun 2011
 *
 * These functions are the C <-> C++ bridge to access the f-series from the command line
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* these are all pure C functions */

/* 3D square morphological operation*/
int imferode3D_rect( const struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfdilat3D_rect( const struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfopen3D_rect( const struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

int imfclose3D_rect( const struct xvimage *input, int SEnx, int SEny, int SEnz, struct xvimage *output);

/* 3D Line morphological operation*/
int imferode3D_line( const struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);

int imfdilat3D_line( const struct xvimage *input, int SEnx, int SEny, int SEnz, int lenght,struct xvimage *output);

int imfopen3D_line( const struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);

int imfclose3D_line( const struct xvimage *input, int SEnx, int SEny, int SEnz,int lenght, struct xvimage *output);


/* 2D square morphological operation*/
int imferode_rect( const struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

int imfdilat_rect( const struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

int imfopen_rect( const struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

int imfclose_rect( const struct xvimage *input, int SEnx, int SEny, struct xvimage *output);

/* 2D Line morphological operation*/
int imferode_line( const struct xvimage *input, int length,int angle, struct xvimage *output);

int imfdilat_line( const struct xvimage *input, int length,int angle, struct xvimage *output);

int imfopen_line( const struct xvimage *input, int length,int angle, struct xvimage *output);

int imfclose_line( const struct xvimage *input, int length,int angle, struct xvimage *output);

/* 2D polygone morphological operation*/
int imfdilate_poly(const struct xvimage *input, int radius, int type, int sides, struct xvimage *output);
int imferode_poly(const struct xvimage *input, int radius, int type, int sides, struct xvimage *output);
int imfclose_poly(const struct xvimage *input, int radius, int type, int sides, struct xvimage *output);
int imfopen_poly(const struct xvimage *input, int radius, int type, int sides, struct xvimage *output);

#ifdef __cplusplus
}
#endif
