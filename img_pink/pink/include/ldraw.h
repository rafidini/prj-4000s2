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
#ifndef LDRAW__H__
#define LDRAW__H__

#ifdef __cplusplus
extern "C" {
#endif

extern int32_t ldrawline(struct xvimage * image1, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
extern int32_t ldrawfilledtriangle(struct xvimage * image1, int32_t v1x, int32_t v1y, int32_t v2x, int32_t v2y, int32_t v3x, int32_t v3y);
  extern int32_t ldrawfilledquadrangle(struct xvimage * image1, int32_t v1x, int32_t v1y, int32_t v2x, int32_t v2y, int32_t v3x, int32_t v3y, int32_t v4x, int32_t v4y);
extern int32_t ldrawlinelist(int32_t *lx, int32_t *ly, int32_t *n, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
extern int32_t ldrawline3d(struct xvimage * image1, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);
extern int32_t ldrawline3d_val(struct xvimage * image1, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2, uint8_t v);
extern int32_t ldrawline3dlist(int32_t *lx, int32_t *ly, int32_t *lz, int32_t *npoints, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);
extern int32_t ldrawline2(struct xvimage * image1);
extern void ldrawfilledellipse(struct xvimage * image, double R, double S, double T, double U, double V, double Z);
extern void ldrawcubic1(struct xvimage * image1, double *x, double *y, int32_t nseg, double sx, double sy);
extern void ldrawcubic2(struct xvimage * image1, double *x, double *y, int32_t nseg, double tmin, double tmax);
extern void ldrawcubic2list(int32_t *lx, int32_t *ly, int32_t *npoints, double *x, double *y, int32_t nseg, double tmin, double tmax);
extern void ldrawcubic3d(struct xvimage * image1, double *x, double *y, double *z, int32_t nseg, double tmin, double tmax);
extern void ldrawcubic3d_val(struct xvimage * image1, double *x, double *y, double *z, int32_t nseg, double tmin, double tmax, uint8_t v);
extern void ldrawcubic3dlist(int32_t *lx, int32_t *ly, int32_t *lz, int32_t *npoints, double *x, double *y, double *z, int32_t nseg, double tmin, double tmax);
extern int32_t ldrawball(struct xvimage * image1, double r, double xc, double yc, double zc);
extern void ldrawdisc(struct xvimage * image1, double r, double xc, double yc);
extern int32_t ldrawtorus(struct xvimage * image1, double c, double a, double xc, double yc, double zc);
extern void ldrawtangents2d(struct xvimage *field, double *x, double *y, int32_t nseg, double tmin, double tmax);
extern void ldrawtangents3d(struct xvimage *field, double *x, double *y, double *z, int32_t nseg, double tmin, double tmax);
extern void ldrawdirtangents3dlist(double *Vx, double *Vy, double *Vz, double *x, double *y, double *z, int32_t nseg, double tmin, double tmax);
extern void ldrawfield2dlist(int32_t npoints, int32_t *X, int32_t *Y, double *tx, double *ty, struct xvimage *image, double len);
extern void ldrawfield3dlist(int32_t npoints, int32_t *X, int32_t *Y, int32_t *Z, double *tx, double *ty, double *tz, struct xvimage *image, double len);
extern struct xvimage *ldrawfield2d(struct xvimage *field, double len);
extern struct xvimage *ldrawfield3d(struct xvimage *field, double len);
extern int32_t ldrawcylinder (
			      struct xvimage* img, 
			      double xA, double yA, double zA, 
			      double a1, double b1, double c1, 		       
			      double thickness, double radius);

#ifdef __cplusplus
}
#endif

#endif /* LDRAW__H__ */
