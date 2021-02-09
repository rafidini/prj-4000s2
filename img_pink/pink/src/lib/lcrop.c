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
/* 
  lcrop
  lcrop3d
  lenframe
  lenframe3d
  lsetframe
  linsert
  lexpandframe
*/

/* 
  Michel Couprie - aout 2000
  update MC avril 2011 - multiband images
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcrop.h>

/* =============================================================== */
struct xvimage * lcrop(struct xvimage *in, int32_t x, int32_t y, int32_t w, int32_t h) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lcrop"
{
  index_t i, j, i0, j0, i1, j1, x0, y0, x1, y1, xx, yy, n;
  index_t rs, cs, N, N1, nb;
  struct xvimage * temp1;

  ONLY_2D(in);
  assert(tsize(in) == 1);

  rs = rowsize(in);
  cs = colsize(in);
  nb = nbands(in);
  N = rs * cs;
  i0 = 0; i1 = w;
  j0 = 0; j1 = h;
  x0 = x; y0 = y;
  x1 = x+w; y1 = y+h;

  if ((x0 >= rs) || (y0 >= cs) || (x1 < 0) || (y1 < 0))
  {
    fprintf(stderr, "%s : out of bounds\n", F_NAME);
    return NULL;
  }

  if (x0 < 0) { i1 += x0; x0 = 0; }
  if (y0 < 0) { j1 += y0; y0 = 0; }
  if (x1 > rs) { i1 -= (x1-rs); x1 = rs; }
  if (y1 > cs) { j1 -= (y1-cs); y1 = cs; }


  temp1 = allocmultimage(NULL, i1, j1, 1, 1, nb, datatype(in));
  if (temp1 == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }
  N1 = i1 * j1;          /* taille bande image finale */

  if (datatype(in) == VFF_TYP_1_BYTE)
  {
    uint8_t *T1 = UCHARDATA(temp1);
    uint8_t *I = UCHARDATA(in);
    for (n = 0; n < nb; n++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
	for (i = i0, xx = x0; i < i1; i++, xx++)
	{
	  T1[(n*N1) + (j*i1) + i] = I[(n*N) + (yy*rs) + xx];
	}
  }
  else if (datatype(in) == VFF_TYP_2_BYTE)
  {
    int16_t *T1L = SSHORTDATA(temp1);
    int16_t *IL = SSHORTDATA(in);
    for (n = 0; n < nb; n++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
	for (i = i0, xx = x0; i < i1; i++, xx++)
	{
	  T1L[(n*N1) + (j*i1) + i] = IL[(n*N) + (yy*rs) + xx];
	}
  }
  else if (datatype(in) == VFF_TYP_4_BYTE)
  {
    int32_t *T1L = SLONGDATA(temp1);
    int32_t *IL = SLONGDATA(in);
    for (n = 0; n < nb; n++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
	for (i = i0, xx = x0; i < i1; i++, xx++)
	{
	  T1L[(n*N1) + (j*i1) + i] = IL[(n*N) + (yy*rs) + xx];
	}
  }
  else if (datatype(in) == VFF_TYP_FLOAT)
  {
    float *T1F = FLOATDATA(temp1);
    float *IF = FLOATDATA(in);
    for (n = 0; n < nb; n++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
	for (i = i0, xx = x0; i < i1; i++, xx++)
	{
	  T1F[(n*N1) + (j*i1) + i] = IF[(n*N) + (yy*rs) + xx];
	}
  }
  else if (datatype(in) == VFF_TYP_DOUBLE)
  {
    double *T1F = DOUBLEDATA(temp1);
    double *IF = DOUBLEDATA(in);
    for (n = 0; n < nb; n++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
	for (i = i0, xx = x0; i < i1; i++, xx++)
	{
	  T1F[(n*N1) + (j*i1) + i] = IF[(n*N) + (yy*rs) + xx];
	}
  }
  else if (datatype(in) == VFF_TYP_COMPLEX)
  {
    fcomplex *T1C = COMPLEXDATA(temp1);
    fcomplex *IC = COMPLEXDATA(in);
    for (n = 0; n < nb; n++)
      for (j = j0, yy = y0; j < j1; j++, yy++)
	for (i = i0, xx = x0; i < i1; i++, xx++)
	{
	  T1C[(n*N1) + (j*i1) + i].re = IC[(n*N) + (yy*rs) + xx].re;
	  T1C[(n*N1) + (j*i1) + i].im = IC[(n*N) + (yy*rs) + xx].im;
	}
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }
  return temp1;
} // lcrop()


/*
 * simple wrapper for Python
 *
 * Hugues Talbot 15/feb/2011
 */

#ifdef NOT_USED_TO_BE_REMOVED
int lpycrop(struct xvimage *in, struct xvimage *out, int32_t x, int32_t y,int32_t w, int32_t h)
{
    out = lcrop(in, x, y, w, h);

    if (out != NULL)
        return 1; /* success */
    else
        return 0;
}
#endif

/* =============================================================== */
struct xvimage * lcrop3d(struct xvimage *in, int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lcrop3d"
{
  index_t i, j, k, i0, j0, k0, i1, j1, k1, x0, y0, z0, x1, y1, z1, xx, yy, zz, n;
  index_t rs, cs, ds, ps, p1, N, N1, nb;
  struct xvimage *temp1;

  ONLY_3D(in);
  assert(tsize(in) == 1);

  rs = rowsize(in);     /* taille rangee image originale */
  cs = colsize(in);     /* taille colonne image originale */
  ds = depth(in);       /* nb. plans image originale */
  ps = rs * cs;         /* taille plan image originale */
  nb = nbands(in);
  N = ps * ds;

  i0 = 0; i1 = w;
  j0 = 0; j1 = h;
  k0 = 0; k1 = d;
  x0 = x; y0 = y; z0 = z;
  x1 = x+w; y1 = y+h; z1 = z+d;

  if ((x0 >= rs) || (y0 >= cs) || (z0 >= ds) || (x1 < 0) || (y1 < 0) || (z1 < 0))
  {
    fprintf(stderr, "%s : out of bounds\n", F_NAME);
    return NULL;
  }

  if (x0 < 0) { i1 += x0; x0 = 0; }
  if (y0 < 0) { j1 += y0; y0 = 0; }
  if (z0 < 0) { k1 += z0; z0 = 0; }
  if (x1 > rs) { i1 -= (x1-rs); x1 = rs; }
  if (y1 > cs) { j1 -= (y1-cs); y1 = cs; }
  if (z1 > ds) { k1 -= (z1-ds); z1 = ds; }

  p1 = i1 * j1;          /* taille plan image finale */
  N1 = p1 * k1;          /* taille bande image finale */

  temp1 = allocmultimage(NULL, i1, j1, k1, 1, nb, datatype(in));
  if (temp1 == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }

  if (datatype(in) == VFF_TYP_1_BYTE)
  {
    uint8_t *T1 = UCHARDATA(temp1);
    uint8_t *I = UCHARDATA(in);
    for (n = 0; n < nb; n++)
    for (k = k0, zz = z0; k < k1; k++, zz++)
    for (j = j0, yy = y0; j < j1; j++, yy++)
    for (i = i0, xx = x0; i < i1; i++, xx++)
    {
      T1[(n*N1) + (k*p1) + (j*i1) + i] = I[(n*N) + (zz*ps) + (yy*rs) + xx];
    }
  }
  else if (datatype(in) == VFF_TYP_4_BYTE)
  {
    int32_t *T1L = SLONGDATA(temp1);
    int32_t *IL = SLONGDATA(in);
    for (n = 0; n < nb; n++)
    for (k = k0, zz = z0; k < k1; k++, zz++)
    for (j = j0, yy = y0; j < j1; j++, yy++)
    for (i = i0, xx = x0; i < i1; i++, xx++)
    {
      T1L[(n*N1) + (k*p1) + (j*i1) + i] = IL[(n*N) + (zz*ps) + (yy*rs) + xx];
    }
  }
  else if (datatype(in) == VFF_TYP_FLOAT)
  {
    float *T1F = FLOATDATA(temp1);
    float *IF = FLOATDATA(in);
    for (n = 0; n < nb; n++)
    for (k = k0, zz = z0; k < k1; k++, zz++)
    for (j = j0, yy = y0; j < j1; j++, yy++)
    for (i = i0, xx = x0; i < i1; i++, xx++)
    {
      T1F[(n*N1) + (k*p1) + (j*i1) + i] = IF[(n*N) + (zz*ps) + (yy*rs) + xx];
    }
  }
  else if (datatype(in) == VFF_TYP_DOUBLE)
  {
    double *T1F = DOUBLEDATA(temp1);
    double *IF = DOUBLEDATA(in);
    for (n = 0; n < nb; n++)
    for (k = k0, zz = z0; k < k1; k++, zz++)
    for (j = j0, yy = y0; j < j1; j++, yy++)
    for (i = i0, xx = x0; i < i1; i++, xx++)
    {
      T1F[(n*N1) + (k*p1) + (j*i1) + i] = IF[(n*N) + (zz*ps) + (yy*rs) + xx];
    }
  }
  else if (datatype(in) == VFF_TYP_COMPLEX)
  {
    fcomplex *T1C = COMPLEXDATA(temp1);
    fcomplex *IC = COMPLEXDATA(in);
    for (n = 0; n < nb; n++)
    for (k = k0, zz = z0; k < k1; k++, zz++)
    for (j = j0, yy = y0; j < j1; j++, yy++)
    for (i = i0, xx = x0; i < i1; i++, xx++)
    {
      T1C[(n*N1)+(k*p1)+(j*i1)+i].re = IC[(n*N)+(zz*ps)+(yy*rs)+xx].re;
      T1C[(n*N1)+(k*p1)+(j*i1)+i].im = IC[(n*N)+(zz*ps)+(yy*rs)+xx].im;
    }
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }
  return temp1;
} // lcrop3d()

/* =============================================================== */
void lsetframe(struct xvimage *image, int32_t grayval) 
/* =============================================================== */
// sets the border of image to value grayval
#undef F_NAME
#define F_NAME "lsetframe"
{
  index_t rs, cs, ds, ps, x, y, z;
  uint8_t * Im;

  assert(datatype(image) == VFF_TYP_1_BYTE);
  assert(nbands(image) == 1);
  assert(tsize(image) == 1);

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  Im = UCHARDATA(image);

  if (ds > 1)
  {
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      Im[0 * ps + y * rs + x] = grayval;          /* plan z = 0 */
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      Im[(ds-1) * ps + y * rs + x] = grayval;     /* plan z = ds-1 */

    for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + 0 * rs + x] = grayval;          /* plan y = 0 */
    for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + (cs-1) * rs + x] = grayval;     /* plan y = cs-1 */

    for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + y * rs + 0] = grayval;          /* plan x = 0 */
    for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      Im[z * ps + y * rs + (rs-1)] = grayval;     /* plan x = rs-1 */
  }
  else
  {
    for (x = 0; x < rs; x++) Im[x] = grayval;
    for (x = 0; x < rs; x++) Im[(cs - 1) * rs + x] = grayval;

    for (y = 1; y < cs - 1; y++) Im[y * rs] = grayval;
    for (y = 1; y < cs - 1; y++) Im[y * rs + rs - 1] = grayval;
  }
} // lsetframe()

/* =============================================================== */
void lsetthickframe(struct xvimage *image, int32_t width, int32_t grayval) 
/* =============================================================== */
// sets the (thick) border of image to value grayval
#undef F_NAME
#define F_NAME "lsetthickframe"
{
  index_t rs, cs, ds, ps, x, y, z, w;
  uint8_t * Im;

  assert(datatype(image) == VFF_TYP_1_BYTE);
  assert(nbands(image) == 1);
  assert(tsize(image) == 1);

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  Im = UCHARDATA(image);
  assert(width < rs); assert(width < cs);

  if (ds > 1)
  {
    assert(width < ds);
    for (w = 0; w < width; w++)
    {
      for (x = 0; x < rs; x++)
	for (y = 0; y < cs; y++) 
	  Im[w * ps + y * rs + x] = grayval;          /* plan z = w */
      for (x = 0; x < rs; x++)
	for (y = 0; y < cs; y++) 
	  Im[(ds-1-w) * ps + y * rs + x] = grayval;     /* plan z = ds-1-w */

      for (x = 0; x < rs; x++)
	for (z = 0; z < ds; z++) 
	  Im[z * ps + w * rs + x] = grayval;          /* plan y = w */
      for (x = 0; x < rs; x++)
	for (z = 0; z < ds; z++) 
	  Im[z * ps + (cs-1-w) * rs + x] = grayval;     /* plan y = cs-1-w */

      for (y = 0; y < cs; y++)
	for (z = 0; z < ds; z++) 
	  Im[z * ps + y * rs + w] = grayval;          /* plan x = w */
      for (y = 0; y < cs; y++)
	for (z = 0; z < ds; z++) 
	  Im[z * ps + y * rs + (rs-1-w)] = grayval;     /* plan x = rs-1-w */
    }
  }
  else
  {
    for (w = 0; w < width; w++)
    {
      for (x = 0; x < rs; x++) Im[w*rs + x] = grayval;
      for (x = 0; x < rs; x++) Im[(cs-1-w) * rs + x] = grayval;

      for (y = 1; y < cs - 1; y++) Im[y * rs + w] = grayval;
      for (y = 1; y < cs - 1; y++) Im[y * rs + rs-1-w] = grayval;
    }
  }
} // lsetthickframe()

//REMOVED:
//struct xvimage * lencadre(struct xvimage *image, int32_t grayval);
//use lenframe instead
/* =============================================================== */
struct xvimage * lenframe(struct xvimage *image, int32_t grayval, int32_t width) 
/* =============================================================== */
// adds a border with a given gray value to an image
#undef F_NAME
#define F_NAME "lenframe"
{
  index_t rs, cs, ds, ps, N, x, y, nb, n;
  index_t rs2, cs2, ds2, ps2, N2;
  struct xvimage * imageout;

  if (depth(image) > 1) return lenframe3d(image, grayval, width);

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  nb = nbands(image);
  ps = rs * cs;
  N = ds * ps;
  rs2 = rs + 2*width;
  cs2 = cs + 2*width;
  ds2 = 1;
  ps2 = rs2 * cs2;
  N2 = ds2 * ps2;

  assert(tsize(image) == 1);
  imageout = allocmultimage(NULL, rs+2*width, cs+2*width, 1, 1, nb, datatype(image));
  if (imageout == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return NULL;
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    uint8_t *Imout = UCHARDATA(imageout);
    uint8_t *Im = UCHARDATA(image);
    uint8_t grayvalue = (uint8_t)grayval;
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++) Imout[n * N2 + y*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++) Imout[n * N2 + (cs2-1-y)*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 1; y < cs2 - 1; y++) Imout[n * N2 + y*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 1; y < cs2 - 1; y++) Imout[n * N2 + y*rs2 + rs2-1-x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = width; x < rs2-width; x++)
	for (y = width; y < cs2-width; y++) 
	  Imout[n * N2 + y * rs2 + x] = Im[n * N + (y-width) * rs + (x-width)];
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    int32_t *Imout = SLONGDATA(imageout);
    int32_t *Im = SLONGDATA(image);
    int32_t grayvalue = (int32_t)grayval;
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++) Imout[n * N2 + y*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++) Imout[n * N2 + (cs2-1-y)*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 1; y < cs2 - 1; y++) Imout[n * N2 + y*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 1; y < cs2 - 1; y++) Imout[n * N2 + y*rs2 + rs2-1-x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = width; x < rs2-width; x++)
	for (y = width; y < cs2-width; y++) 
	  Imout[n * N2 + y * rs2 + x] = Im[n * N + (y-width) * rs + (x-width)];
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float *Imout = FLOATDATA(imageout);
    float *Im = FLOATDATA(image);
    float grayvalue = (float)grayval;
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++) Imout[n * N2 + y*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++) Imout[n * N2 + (cs2-1-y)*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 1; y < cs2 - 1; y++) Imout[n * N2 + y*rs2 + x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 1; y < cs2 - 1; y++) Imout[n * N2 + y*rs2 + rs2-1-x] = grayvalue;
    
    for (n = 0; n < nb; n++)
      for (x = width; x < rs2-width; x++)
	for (y = width; y < cs2-width; y++) 
	  Imout[n * N2 + y * rs2 + x] = Im[n * N + (y-width) * rs + (x-width)];
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }

  return imageout;
} /* lenframe() */

/* =============================================================== */
struct xvimage * lenframe3d(struct xvimage *image, int32_t grayval, int32_t width) 
/* =============================================================== */
// adds a border with a given gray value to an image
// produces a 3d image, even when the depth of input image is 1
#undef F_NAME
#define F_NAME "lenframe3d"
{
  index_t rs, cs, ds, ps, N, x, y, z, nb, n;
  index_t rs2, cs2, ds2, ps2, N2;
  struct xvimage * imageout;

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  nb = nbands(image);
  ps = rs * cs;
  N = ds * ps;
  rs2 = rs + 2*width;
  cs2 = cs + 2*width;
  ds2 = ds + 2*width;
  ps2 = rs2 * cs2;
  N2 = ds2 * ps2;

  assert(tsize(image) == 1);

  imageout = allocmultimage(NULL, rs+2*width, cs+2*width, ds+2*width, 1, nb, datatype(image));
  if (imageout == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return NULL;
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    uint8_t *Imout = UCHARDATA(imageout);
    uint8_t *Im = UCHARDATA(image);
    uint8_t grayvalue = (uint8_t)grayval;

    for (n = 0; n < nb; n++)
      for (z = 0; z < width; z++)
	for (x = 0; x < rs2; x++)
	  for (y = 0; y < cs2; y++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue; /* plans z = 0.. */
    for (n = 0; n < nb; n++)
      for (z = 0; z < width; z++)
	for (x = 0; x < rs2; x++)
	  for (y = 0; y < cs2; y++) 
	    Imout[n * N2 + (ds2-1-z) * ps2 + y * rs2 + x] = grayvalue; /* plans z = ..ds2-1 */
  
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue;         /* plans y = 0.. */
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + (cs2-1-y) * rs2 + x] = grayvalue; /* plans y = ..cs2-1 */
  
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 0; y < cs2; y++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue;         /* plans x = 0.. */
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 0; y < cs2; y++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + (rs2-1-x)] = grayvalue; /* plans x = ..rs2-1 */
  
    for (n = 0; n < nb; n++)
      for (x = width; x < rs2-width; x++)
	for (y = width; y < cs2-width; y++) 
	  for (z = width; z < ds2-width; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = Im[n * N + (z-width) * ps + (y-width) * rs + (x-width)];
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    int32_t *Imout = SLONGDATA(imageout);
    int32_t *Im = SLONGDATA(image);
    int32_t grayvalue = (int32_t)grayval;

    for (n = 0; n < nb; n++)
      for (z = 0; z < width; z++)
	for (x = 0; x < rs2; x++)
	  for (y = 0; y < cs2; y++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue;         /* plans z = 0.. */
    for (n = 0; n < nb; n++)
      for (z = 0; z < width; z++)
	for (x = 0; x < rs2; x++)
	  for (y = 0; y < cs2; y++) 
	    Imout[n * N2 + (ds2-1-z) * ps2 + y * rs2 + x] = grayvalue; /* plans z = ..ds2-1 */
  
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue;         /* plans y = 0.. */
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + (cs2-1-y) * rs2 + x] = grayvalue; /* plans y = ..cs2-1 */
  
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 0; y < cs2; y++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue;         /* plans x = 0.. */
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 0; y < cs2; y++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + (rs2-1-x)] = grayvalue; /* plans x = ..rs2-1 */
  
    for (n = 0; n < nb; n++)
      for (x = width; x < rs2-width; x++)
	for (y = width; y < cs2-width; y++) 
	  for (z = width; z < ds2-width; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = Im[n * N + (z-width) * ps + (y-width) * rs + (x-width)];
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float *Imout = FLOATDATA(imageout);
    float *Im = FLOATDATA(image);
    float grayvalue = (float)grayval;

    for (n = 0; n < nb; n++)
      for (z = 0; z < width; z++)
	for (x = 0; x < rs2; x++)
	  for (y = 0; y < cs2; y++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue;         /* plans z = 0.. */
    for (n = 0; n < nb; n++)
      for (z = 0; z < width; z++)
	for (x = 0; x < rs2; x++)
	  for (y = 0; y < cs2; y++) 
	    Imout[n * N2 + (ds2-1-z) * ps2 + y * rs2 + x] = grayvalue; /* plans z = ..ds2-1 */
  
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue;         /* plans y = 0.. */
    for (n = 0; n < nb; n++)
      for (y = 0; y < width; y++)
	for (x = 0; x < rs2; x++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + (cs2-1-y) * rs2 + x] = grayvalue; /* plans y = ..cs2-1 */
  
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 0; y < cs2; y++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = grayvalue;         /* plans x = 0.. */
    for (n = 0; n < nb; n++)
      for (x = 0; x < width; x++)
	for (y = 0; y < cs2; y++)
	  for (z = 0; z < ds2; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + (rs2-1-x)] = grayvalue; /* plans x = ..rs2-1 */
  
    for (n = 0; n < nb; n++)
      for (x = width; x < rs2-width; x++)
	for (y = width; y < cs2-width; y++) 
	  for (z = width; z < ds2-width; z++) 
	    Imout[n * N2 + z * ps2 + y * rs2 + x] = Im[n * N + (z-width) * ps + (y-width) * rs + (x-width)];
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }

  return imageout;
} /* lenframe3d() */

/* =============================================================== */
int32_t linsert(struct xvimage *a, struct xvimage *b, int32_t x, int32_t y, int32_t z) 
/* =============================================================== */
// inserts a into b at position (x,y,z)
#undef F_NAME
#define F_NAME "linsert"
{
  index_t i, j, k, n;
  index_t rsa, csa, dsa, psa, rsb, csb, dsb, psb, Na, Nb, nb;

  rsa = rowsize(a);
  rsb = rowsize(b);
  csa = colsize(a);
  csb = colsize(b);
  dsa = depth(a);
  dsb = depth(b);
  psa = rsa * csa;
  psb = rsb * csb;
  Na = dsa * psa;
  Nb = dsb * psb;
  nb = nbands(a);

  assert(datatype(a) == datatype(b));
  assert(nbands(a) == nbands(b));
  assert(tsize(a) == tsize(b));
  assert(tsize(a) == 1);

  if (datatype(a) == VFF_TYP_1_BYTE)
  {
    uint8_t *A = UCHARDATA(a);
    uint8_t *B = UCHARDATA(b);

    for (n = 0; n < nb; n++)
    for (k = 0; k < dsa; k++)
    for (j = 0; j < csa; j++)
    for (i = 0; i < rsa; i++)
      if ((z + k < dsb) && (y + j < csb) && (x + i < rsb) && 
          (z + k >= 0) && (y + j >= 0) && (x + i >= 0))
        B[(n*Nb) + ((z + k) * psb) + ((y + j) * rsb) + x + i] = 
          A[(n*Na) + (k * psa) + (j * rsa) + i]; 
  }
  else if (datatype(a) == VFF_TYP_4_BYTE)
  {
    int32_t *A = SLONGDATA(a);
    int32_t *B = SLONGDATA(b);

    for (n = 0; n < nb; n++)
    for (k = 0; k < dsa; k++)
    for (j = 0; j < csa; j++)
    for (i = 0; i < rsa; i++)
      if ((z + k < dsb) && (y + j < csb) && (x + i < rsb) && 
          (z + k >= 0) && (y + j >= 0) && (x + i >= 0)) 
        B[(n*Nb) + ((z + k) * psb) + ((y + j) * rsb) + x + i] = 
          A[(n*Na) + (k * psa) + (j * rsa) + i]; 
  }
  else if (datatype(a) == VFF_TYP_FLOAT)
  {
    float *A = FLOATDATA(a);
    float *B = FLOATDATA(b);

    for (n = 0; n < nb; n++)
    for (k = 0; k < dsa; k++)
    for (j = 0; j < csa; j++)
    for (i = 0; i < rsa; i++)
      if ((z + k < dsb) && (y + j < csb) && (x + i < rsb) && 
          (z + k >= 0) && (y + j >= 0) && (x + i >= 0)) 
        B[(n*Nb) + ((z + k) * psb) + ((y + j) * rsb) + x + i] = 
          A[(n*Na) + (k * psa) + (j * rsa) + i]; 
  }
  else if (datatype(a) == VFF_TYP_DOUBLE)
  {
    double *A = DOUBLEDATA(a);
    double *B = DOUBLEDATA(b);

    for (n = 0; n < nb; n++)
    for (k = 0; k < dsa; k++)
    for (j = 0; j < csa; j++)
    for (i = 0; i < rsa; i++)
      if ((z + k < dsb) && (y + j < csb) && (x + i < rsb) && 
          (z + k >= 0) && (y + j >= 0) && (x + i >= 0)) 
        B[(n*Nb) + ((z + k) * psb) + ((y + j) * rsb) + x + i] = 
          A[(n*Na) + (k * psa) + (j * rsa) + i]; 
  }
  else if (datatype(a) == VFF_TYP_COMPLEX)
  {
    fcomplex *A = COMPLEXDATA(a);
    fcomplex *B = COMPLEXDATA(b);

    for (n = 0; n < nb; n++)
    for (k = 0; k < dsa; k++)
    for (j = 0; j < csa; j++)
    for (i = 0; i < rsa; i++)
    if ((z + k < dsb) && (y + j < csb) && (x + i < rsb) && 
        (z + k >= 0) && (y + j >= 0) && (x + i >= 0)) 
    {
      B[(n*Nb) + ((z + k) * psb) + ((y + j) * rsb) + x + i].re = A[(n*Na) + (k * psa) + (j * rsa) + i].re; 
      B[(n*Nb) + ((z + k) * psb) + ((y + j) * rsb) + x + i].im = A[(n*Na) + (k * psa) + (j * rsa) + i].im; 
    }
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return 0;
  }

  return 1;
} // linsert()

/* =============================================================== */
struct xvimage * lexpandframe(struct xvimage *in, int32_t n) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lexpandframe"
{
  index_t ds1 = depth(in);
  index_t rs1 = rowsize(in);
  index_t cs1 = colsize(in);
  index_t rs2, cs2, x, y;
  struct xvimage *temp1;

  assert(nbands(in) == 1);
  assert(tsize(in) == 1);

  if (ds1 > 1)
  {
    fprintf(stderr, "%s: 3D Not Yet Implemented\n", F_NAME);
    return NULL;
  }
  else
  { 
    if (datatype(in) == VFF_TYP_1_BYTE)
    {
      uint8_t *T1, *I = UCHARDATA(in);
      rs2 = rs1 + n + n;
      cs2 = cs1 + n + n;
      temp1 = allocimage(NULL, rs2, cs2, 1, VFF_TYP_1_BYTE);
      if (temp1 == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", F_NAME);
        return NULL;
      }
      T1 = UCHARDATA(temp1);
  
      for (y = 0; y < n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[0];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[rs1 - 1];
      }
      for (y = n; y < cs1+n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(y-n) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + rs1 - 1];
      }
      for (y = cs1+n; y < cs2; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + rs1 - 1];
      }
    }
    else if (datatype(in) == VFF_TYP_4_BYTE)
    {
      int32_t *T1, *I = SLONGDATA(in);
      rs2 = rs1 + n + n;
      cs2 = cs1 + n + n;
      temp1 = allocimage(NULL, rs2, cs2, 1, VFF_TYP_4_BYTE);
      if (temp1 == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", F_NAME);
        return NULL;
      }
      T1 = SLONGDATA(temp1);
  
      for (y = 0; y < n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[0];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[rs1 - 1];
      }
      for (y = n; y < cs1+n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(y-n) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + rs1 - 1];
      }
      for (y = cs1+n; y < cs2; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + rs1 - 1];
      }
    }
    else if (datatype(in) == VFF_TYP_FLOAT)
    {
      float *T1, *I = FLOATDATA(in);
      rs2 = rs1 + n + n;
      cs2 = cs1 + n + n;
      temp1 = allocimage(NULL, rs2, cs2, 1, VFF_TYP_FLOAT);
      if (temp1 == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", F_NAME);
        return NULL;
      }
      T1 = FLOATDATA(temp1);
  
      for (y = 0; y < n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[0];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[rs1 - 1];
      }
      for (y = n; y < cs1+n; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(y-n) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(y-n) * rs1 + rs1 - 1];
      }
      for (y = cs1+n; y < cs2; y++)
      {
        for (x = 0; x < n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1];
        for (x = n; x < rs1+n; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + x-n];
        for (x = rs1+n; x < rs2; x++) T1[y * rs2 + x] = I[(cs1 - 1) * rs1 + rs1 - 1];
      }
    }
    else
    {
      fprintf(stderr, "%s: bad datatype\n", F_NAME);
      return NULL;
    }

  }

  return temp1;
} // lexpandframe()

/* =============================================================== */
struct xvimage * lautocrop(struct xvimage *in, double seuil)
/* =============================================================== */
#undef F_NAME
#define F_NAME "lautocrop"
{
  index_t x, y, z, w, h, p, n;
  index_t xmin, xmax, ymin, ymax, zmin, zmax;
  index_t rs, cs, ds, ps, N, nb;

  assert(tsize(in) == 1);

  rs = rowsize(in);
  cs = colsize(in);
  ps = rs * cs;
  ds = depth(in);
  N = ds * ps;
  nb = nbands(in);
  xmin = rs; xmax = 0; 
  ymin = cs; ymax = 0; 
  zmin = ds; zmax = 0; 

#ifdef VERBOSE
  printf("Crop: xmin=%d, ymin=%d, zmin=%d, w=%d, h=%d, p=%d\n",
	 xmin, ymin, zmin, w, h, p);
#endif


  if (datatype(in) == VFF_TYP_1_BYTE)
  {
    uint8_t *I = UCHARDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if ((double)(I[n*N + z*ps + y*rs + x]) > seuil)
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else if (datatype(in) == VFF_TYP_4_BYTE)
  {
    int32_t *I = SLONGDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if ((double)(I[n*N + z*ps + y*rs + x]) > seuil)
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else if (datatype(in) == VFF_TYP_FLOAT)
  {
    float *I = FLOATDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if ((double)(I[n*N + z*ps + y*rs + x]) > seuil)
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else if (datatype(in) == VFF_TYP_DOUBLE)
  {
    double *I = DOUBLEDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if (I[n*N + z*ps + y*rs + x] > seuil)
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else if (datatype(in) == VFF_TYP_COMPLEX)
  {
    fcomplex *I = COMPLEXDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if (((double)(I[n*N + z*ps + y*rs + x].re) > seuil) && 
	  ((double)(I[n*N + z*ps + y*rs + x].im) > seuil))
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return NULL;
  }

  w = xmax - xmin + 1;
  h = ymax - ymin + 1;
  p = zmax - zmin + 1;

  if (ds == 1) return lcrop(in, xmin, ymin, w, h);
  else return lcrop3d(in, xmin, ymin, zmin, w, h, p);
} // lautocrop()

/* =============================================================== */
void lautocrop2(struct xvimage *in, double seuil, index_t *Xmin, index_t *Ymin, index_t *Zmin, index_t *w, index_t *h, index_t *p)
/* =============================================================== */
#undef F_NAME
#define F_NAME "lautocrop2"
{
  index_t x, y, z, n;
  index_t xmin, xmax, ymin, ymax, zmin, zmax;
  index_t rs, cs, ds, ps, N, nb;

  assert(tsize(in) == 1);

  rs = rowsize(in);
  cs = colsize(in);
  ps = rs * cs;
  ds = depth(in);
  N = ds * ps;
  nb = nbands(in);
  xmin = rs; xmax = 0; 
  ymin = cs; ymax = 0; 
  zmin = ds; zmax = 0; 

  if (datatype(in) == VFF_TYP_1_BYTE)
  {
    uint8_t *I = UCHARDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if ((double)(I[n*N + z*ps + y*rs + x]) > seuil)
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else if (datatype(in) == VFF_TYP_4_BYTE)
  {
    int32_t *I = SLONGDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if ((double)(I[n*N + z*ps + y*rs + x]) > seuil)
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else if (datatype(in) == VFF_TYP_FLOAT)
  {
    float *I = FLOATDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if ((double)(I[n*N + z*ps + y*rs + x]) > seuil)
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else if (datatype(in) == VFF_TYP_DOUBLE)
  {
    double *I = DOUBLEDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if (I[n*N + z*ps + y*rs + x] > seuil)
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else if (datatype(in) == VFF_TYP_COMPLEX)
  {
    fcomplex *I = COMPLEXDATA(in);
    for (n = 0; n < nb; n++)
    for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if (((double)(I[n*N + z*ps + y*rs + x].re) > seuil) && 
	  ((double)(I[n*N + z*ps + y*rs + x].im) > seuil))
    {
      if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
      if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
      if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
    }
  }
  else
  {
    fprintf(stderr, "%s : bad data type\n", F_NAME);
    return;
  }

  *Xmin = xmin;
  *Ymin = ymin;
  *Zmin = zmin;
  *w = xmax - xmin + 1;
  *h = ymax - ymin + 1;
  *p = zmax - zmin + 1;
} // lautocrop2()
