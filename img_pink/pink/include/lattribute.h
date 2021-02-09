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
#define LABMAX   0
#define LABMIN   1
#define LABPLATEAU   2

#define AREA  0
#define PERIM 1
#define CIRC  2
#define TROUS 3
#define EXCEN 4
#define ORIEN 5
#define HDIAM 6
#define VDIAM 7
#define PDIAM 8
#define DIAM  9
  /* NOT USED YET
#define LINEA 10
#define PLANA 11
  */
/* ============== */
/* prototype for lattribute.c */
/* ============== */

extern int32_t lattribute(
        struct xvimage *img, /* image de depart */
        int32_t connex, 
        int32_t minimum,
        int32_t attrib, 
        int32_t seuil,  
        struct xvimage *lab  /* resultat: image d'attributs */
);

extern int32_t lattribute2d(
        struct xvimage *img, /* image de depart */
        int32_t connex,      /* 4, 8  */
        int32_t minimum,     /* booleen */
        int32_t attrib,      /* 0: surface, 1: perimetre, 2: circularite ... */
        int32_t seuil,       /* en dessous de seuil, l'attribut est mis a 0 */
        struct xvimage *lab  /* resultat: image d'attributs */
);

extern int32_t lattribute3d(
        struct xvimage *img, /* image de depart */
        int32_t connex,          /* 6, 18, 26  */
        int32_t minimum,         /* booleen */
        int32_t attrib,          /* 0: surface, 1: perimetre, 2: circularite ... */
        int32_t seuil,           /* en dessous de seuil, l'attribut est mis a 0 */
        struct xvimage *lab      /* resultat: image d'attributs */
);
extern int32_t lplanarity(
        struct xvimage *img,     /* image de depart */
        int32_t connex,          /* 6, 18, 26  */
        struct xvimage *res      /* resultat: image d'attributs */
);
#ifdef __cplusplus
}
#endif
