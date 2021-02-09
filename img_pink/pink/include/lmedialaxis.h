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
#ifdef __cplusplus
extern "C" {
#endif

/* ==================================== */
// Data structures and functions for the bisector
/* ==================================== */

typedef double * ImageAngle;

struct  Point2D{
    int32_t xCoor, yCoor;
};
struct  Point3D{
    int32_t xCoor, yCoor, zCoor;
};
typedef struct Point2D * ListPoint2D;
typedef struct Point3D * ListPoint3D;

typedef int32_t *tabulateCTg;

typedef int32_t * LookUpTable;
typedef int32_t * Tabuff;
typedef int32_t * TabDTg;

typedef struct  Weighting{
    int32_t x, y, z, RR;
}  Weighting;

typedef Weighting *MaskG;	      	//used to store Mglut

typedef struct Coordinates {
    int32_t x, y, z;
} Coordinates;

typedef struct {		//used to store all possible neighbors of (1/8) Z2 or (1/48) Z3
    Coordinates neig[48];
} Neighbors;

struct  DPoint2D{
    double xCoor, yCoor;
};
struct  DPoint3D{
    double xCoor, yCoor, zCoor;
};
typedef struct DPoint2D * ListDPoint2D;
typedef struct DPoint3D * ListDPoint3D;

/* ==================================== */
// prototypes
/* ==================================== */

extern int32_t lmedialaxis_lmedialaxis(struct xvimage *f, int32_t connex, struct xvimage *medial);
extern int32_t lmedialaxis_lmedialaxisbin(struct xvimage *f, int32_t connex);
extern int32_t lmedialaxis_lbisector(struct xvimage *id, struct xvimage *im, struct xvimage *ia);
extern int32_t lmedialaxis_lbisector_talbot(struct xvimage * image, struct xvimage *angles);
extern int32_t lmedialaxis_lmedax_Remy_Thiel(struct xvimage *ImageDist, struct xvimage *ImageMedial);
extern int32_t llambdamedialaxis(struct xvimage *dist, struct xvimage *lambda);
extern int32_t lmedialaxis_lambdamedialaxis(struct xvimage *image, struct xvimage *lambdaimage);
extern int32_t llambdaprimemedialaxis(struct xvimage *dist, struct xvimage *vor, struct xvimage *lambda);
extern int32_t lmedialaxis_openingfunction(struct xvimage *image, int32_t mode, struct xvimage *result);
extern int32_t lmedialaxis_ExtendedDownstream(int32_t x, int32_t y, uint32_t *image, index_t rs, index_t cs, int32_t *TabIndDec, int32_t nval, Coordinates *ListDecs, ListDPoint2D Aval);
extern int32_t lmedialaxis_ExtendedDownstream3d (int32_t x, int32_t y, int32_t z, uint32_t *image, index_t rs, index_t cs, index_t ds, int32_t *TabIndDec, int32_t nval, Coordinates *ListDecs, ListDPoint3D Aval);
extern int32_t lmedialaxis_scaleaxis(struct xvimage *image, double s, struct xvimage * res);
extern int32_t lmedialaxis_scalefilteredmedialaxis(struct xvimage *image, double s, struct xvimage * res);
#ifdef __cplusplus
}
#endif
