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
   Algorithmes 3D parall�les de squelettisation
   
   Michel Couprie

\li 0: ultimate, symmetric, without constraint (MK3a)
\li 1: curvilinear, symmetric, based on 1D isthmus (CK3a)
\li 2: medial axis preservation (AK3) - parameter inhibit represents the minimal radius of medial axis balls which are considered
\li 3: ultimate, symmetric (MK3) - if nsteps = -2, returns the topological distance
\li 4: curvilinear, symmetric, based on ends (EK3)
\li 5: curvilinear, symmetric, based on ends, with end reconstruction (CK3b)
\li 6: topological axis (not homotopic)
\li 7: curvilinear, symmetric, based on residual points and 2D isthmus (CK3)
\li 8: ultimate, asymmetric (AMK3)
\li 9: curvilinear, asymmetric, based on thin 1D isthmus (ACK3a)
\li 10: curvilinear, asymmetric, based on 3D and 2D residuals (ACK3)
\li 11: surface, symmetric, based on residual points (RK3)
\li 12: surface, symmetric, based on 2D isthmuses (SK3)
\li 13: ultimate, directional, (DK3)
\li 14: surface, directional, based on residual points (DRK3)
\li 15: surface, directional, based on 2D isthmuses (DSK3)
\li 16: curvilinear, asymmetric, based on thin 1D isthmus with persistence (ACK3p)
\li 17: surface, asymmetric, based on thin 2D isthmus with persistence (ASK3p)
\li 18: curvilinear, symmetric, based on 1D isthmus with persistence (CK3p)
\li 19: surface, symmetric, based on 2D isthmus with persistence (SK3p)
\li 20: surface and curvilinear, symmetric, based on 1D and 2D isthmus with persistence (SCK3p)
\li 21: surface, symmetric, based on residual points (RK3), variant (uses 26-connectivity to define residual points)
\li 22: surface and curvilinear, asymmetric, based on 1D and 2D isthmus with persistence (ASCK3p)
\li 23: curvilinear, asymmetric, based on ends (AEK3)
\li 24: surface, symmetric, based on 2D and 1D isthmuses (SCK3)
\li 25: surface, directional, based on 2D and 1D isthmuses (DSCK3)

   Update MC 19/12/2011 : introduction des cliques D-cruciales
   Update MC 03/08/2012 : fix bug asym_match_vois0
   Update MC 02/11/2012 : squelettes sym�triques avec persistence
   Update MC 27/01/2014 : squelettes asym�triques avec persistence
   Update MC 24/06/2014 : fix bug match_vois0
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <mcutil.h>
#include <mcindic.h>
#include <mcrlifo.h>
#include <lskelpar3d.h>

#define PERS_INIT_VAL 0

#define I_INHIBIT     1
#define I_EARLYCURVE  2

#define IS_INHIBIT(f) (f&I_INHIBIT)
#define SET_INHIBIT(f) (f|=I_INHIBIT)
#define IS_EARLYCURVE(f) (f&I_EARLYCURVE)
#define SET_EARLYCURVE(f) (f|=I_EARLYCURVE)

#define NDG_EARLY   127

#define S_OBJECT      1
#define S_SIMPLE      2
#define S_DCRUCIAL    4
#define S_CURVE      32
#define S_SURF       64
#define S_SELECTED  128

#define IS_OBJECT(f)     (f&S_OBJECT)
#define IS_SIMPLE(f)     (f&S_SIMPLE)
#define IS_DCRUCIAL(f)   (f&S_DCRUCIAL)
#define IS_CURVE(f)      (f&S_CURVE)
#define IS_SURF(f)       (f&S_SURF)
#define IS_SELECTED(f)   (f&S_SELECTED)

#define SET_OBJECT(f)     (f|=S_OBJECT)
#define SET_SIMPLE(f)     (f|=S_SIMPLE)
#define SET_DCRUCIAL(f)   (f|=S_DCRUCIAL)
#define SET_CURVE(f)      (f|=S_CURVE)
#define SET_SURF(f)       (f|=S_SURF)
#define SET_SELECTED(f)   (f|=S_SELECTED)

#define UNSET_CURVE(f)      (f&=~S_CURVE)
#define UNSET_OBJECT(f)     (f&=~S_OBJECT)
#define UNSET_SIMPLE(f)     (f&=~S_SIMPLE)
#define UNSET_DCRUCIAL(f)   (f&=~S_DCRUCIAL)
#define UNSET_SELECTED(f)   (f&=~S_SELECTED)

#define MAXFLOAT	3.40282347e+38F

//#define VERBOSE
//#define DEBUG_SKEL_CK3P
//#define DEBUG_SKEL_MK3A
//#define DEBUG_SKEL_CK3A
//#define DEBUG
#ifdef DEBUG
int32_t trace = 1;
#endif

#define NEW_ISTHMUS
#define RESIDUEL6
#define DIRTOURNE
//#define USE_NKP_END
#define USE_END
#define ANCIENNE_VERSION_SEQ

#define EN_LIFO       0

/* ==================================== */
static void extract_vois(
  uint8_t *img,          /* pointeur base image */
  index_t p,                       /* index du point */
  index_t rs,                      /* taille rangee */
  index_t ps,                      /* taille plan */
  index_t N,                       /* taille image */
  uint8_t *vois)    
/* 
  retourne dans "vois" les valeurs des 27 voisins de p, dans l'ordre suivant: 

               12      11      10       
               13       8       9
               14      15      16

		3	2	1
		4      26	0
		5	6	7

               21      20      19
               22      17      18
               23      24      25

  le point p ne doit pas �tre un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p%ps<rs) || (p%rs==0) || (p%ps>=ps-rs) || 
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }
  vois[ 0] = *(ptr+1);
  vois[ 1] = *(ptr+1-rs);
  vois[ 2] = *(ptr-rs);
  vois[ 3] = *(ptr-rs-1);
  vois[ 4] = *(ptr-1);
  vois[ 5] = *(ptr-1+rs);
  vois[ 6] = *(ptr+rs);
  vois[ 7] = *(ptr+rs+1);

  vois[ 8] = *(ptr-ps);
  vois[ 9] = *(ptr-ps+1);
  vois[10] = *(ptr-ps+1-rs);
  vois[11] = *(ptr-ps-rs);
  vois[12] = *(ptr-ps-rs-1);
  vois[13] = *(ptr-ps-1);
  vois[14] = *(ptr-ps-1+rs);
  vois[15] = *(ptr-ps+rs);
  vois[16] = *(ptr-ps+rs+1);

  vois[17] = *(ptr+ps);
  vois[18] = *(ptr+ps+1);
  vois[19] = *(ptr+ps+1-rs);
  vois[20] = *(ptr+ps-rs);
  vois[21] = *(ptr+ps-rs-1);
  vois[22] = *(ptr+ps-1);
  vois[23] = *(ptr+ps-1+rs);
  vois[24] = *(ptr+ps+rs);
  vois[25] = *(ptr+ps+rs+1);

  vois[26] = *(ptr);
} /* extract_vois() */

/* ==================================== */
static void insert_vois(
  uint8_t *vois,			
  uint8_t *img,          /* pointeur base image */
  index_t p,                       /* index du point */
  index_t rs,                      /* taille rangee */
  index_t ps,                      /* taille plan */
  index_t N)                       /* taille image */    
/* 
  recopie vois dans le voisinage de p
  le point p ne doit pas �tre un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "insert_vois"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p%ps<rs) || (p%rs==0) || (p%ps>=ps-rs) || 
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }
  *(ptr+1) = vois[ 0];
  *(ptr+1-rs) = vois[ 1];
  *(ptr-rs) = vois[ 2];
  *(ptr-rs-1) = vois[ 3];
  *(ptr-1) = vois[ 4];
  *(ptr-1+rs) = vois[ 5];
  *(ptr+rs) = vois[ 6];
  *(ptr+rs+1) = vois[ 7];

  *(ptr-ps) = vois[ 8];
  *(ptr-ps+1) = vois[ 9];
  *(ptr-ps+1-rs) = vois[10];
  *(ptr-ps-rs) = vois[11];
  *(ptr-ps-rs-1) = vois[12];
  *(ptr-ps-1) = vois[13];
  *(ptr-ps-1+rs) = vois[14];
  *(ptr-ps+rs) = vois[15];
  *(ptr-ps+rs+1) = vois[16];

  *(ptr+ps) = vois[17];
  *(ptr+ps+1) = vois[18];
  *(ptr+ps+1-rs) = vois[19];
  *(ptr+ps-rs) = vois[20];
  *(ptr+ps-rs-1) = vois[21];
  *(ptr+ps-1) = vois[22];
  *(ptr+ps-1+rs) = vois[23];
  *(ptr+ps+rs) = vois[24];
  *(ptr+ps+rs+1) = vois[25];

  *(ptr) = vois[26];
} /* insert_vois() */

#ifdef DEBUG
/* ==================================== */
static void print_vois(uint8_t *vois)    
/* 
   affiche vois (debug)
*/
/* ==================================== */
{
  printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n", 
	 vois[12],vois[11],vois[10],vois[3],vois[2],vois[1],vois[21],vois[20],vois[19]);
  printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n", 
	 vois[13],vois[8],vois[9],vois[4],vois[26],vois[0],vois[22],vois[17],vois[18]);
  printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n\n", 
	 vois[14],vois[15],vois[16],vois[5],vois[6],vois[7],vois[23],vois[24],vois[25]);
} /* print_vois() */
#endif

/* ==================================== */
static void isometrieXZ_vois(uint8_t *vois) 
// effectue une isom�trie du voisinage "vois" par �change des axes X et Z (+ sym�tries)
// cette isom�trie est de plus une involution
/* ==================================== */
{
  uint8_t v[26];
  int32_t i;
  v[ 0] = vois[17];  v[ 1] = vois[20];  v[ 2] = vois[ 2];  v[ 3] = vois[11];
  v[ 4] = vois[ 8];  v[ 5] = vois[15];  v[ 6] = vois[ 6];  v[ 7] = vois[24];
  v[ 8] = vois[ 4];  v[ 9] = vois[22];  v[10] = vois[21];  v[11] = vois[ 3];
  v[12] = vois[12];  v[13] = vois[13];  v[14] = vois[14];  v[15] = vois[ 5];
  v[16] = vois[23];  v[17] = vois[ 0];  v[18] = vois[18];  v[19] = vois[19];
  v[20] = vois[ 1];  v[21] = vois[10];  v[22] = vois[ 9];  v[23] = vois[16];
  v[24] = vois[ 7];  v[25] = vois[25];
  for (i = 0; i < 26; i++) vois[i] = v[i];
} /* isometrieXZ_vois() */

/* ==================================== */
static void isometrieYZ_vois(uint8_t *vois)
// effectue une isom�trie du voisinage "vois" par �change des axes Y et Z (+ sym�tries)  
// cette isom�trie est de plus une involution
/* ==================================== */
{
  uint8_t v[26];
  int32_t i;
  v[ 0] = vois[ 0];  v[ 1] = vois[18];  v[ 2] = vois[17];  v[ 3] = vois[22];
  v[ 4] = vois[ 4];  v[ 5] = vois[13];  v[ 6] = vois[ 8];  v[ 7] = vois[ 9];
  v[ 8] = vois[ 6];  v[ 9] = vois[ 7];  v[10] = vois[25];  v[11] = vois[24];
  v[12] = vois[23];  v[13] = vois[ 5];  v[14] = vois[14];  v[15] = vois[15];
  v[16] = vois[16];  v[17] = vois[ 2];  v[18] = vois[ 1];  v[19] = vois[19];
  v[20] = vois[20];  v[21] = vois[21];  v[22] = vois[ 3];  v[23] = vois[12];
  v[24] = vois[11];  v[25] = vois[10];
  for (i = 0; i < 26; i++) vois[i] = v[i];
} /* isometrieYZ_vois() */

/* ==================================== */
static int32_t match_end(uint8_t *v)
/* ==================================== */
/*
               12      11      10
               13       8       9
               14      15      16

		3	2	1
		4      26	0
		5	6	7

               21      20      19
               22      17      18
               23      24      25

Teste si au moins un des points 12, 11, 13, 8, 3, 2, 4 est objet et tous les autres fond
(aussi avec les isom�tries). 
*/
{
#ifdef DEBUG
  if (trace)
  {  
    printf("match_end\n");
    print_vois(v);
  }
#endif

  if ((v[19] || v[20] || v[18] || v[17] || v[2] || v[1] || v[0]) &&
      !v[3 ] && !v[4 ] && !v[21] && !v[22] && 
      !v[25] && !v[24] && !v[7 ] && !v[6 ] && !v[23] && !v[5] && 
      !v[12] && !v[11] && !v[10] && !v[13] && 
      !v[8 ] && !v[9 ] && !v[14] && !v[15] && !v[16]) return 1;

  if ((v[21] || v[20] || v[22] || v[17] || v[2] || v[3] || v[4]) &&
      !v[1 ] && !v[0 ] && !v[19] && !v[18] && 
      !v[23] && !v[24] && !v[5 ] && !v[6 ] && !v[25] && !v[7] && 
      !v[12] && !v[11] && !v[10] && !v[13] && 
      !v[8 ] && !v[9 ] && !v[14] && !v[15] && !v[16]) return 1;

  if ((v[17] || v[22] || v[24] || v[23] || v[4] || v[5] || v[6]) &&
      !v[2 ] && !v[3 ] && !v[20] && !v[21] && 
      !v[18] && !v[25] && !v[0 ] && !v[7 ] && !v[19] && !v[1] && 
      !v[12] && !v[11] && !v[10] && !v[13] && 
      !v[8 ] && !v[9 ] && !v[14] && !v[15] && !v[16]) return 1;

  if ((v[17] || v[18] || v[24] || v[25] || v[0] || v[7] || v[6]) &&
      !v[2 ] && !v[1 ] && !v[20] && !v[19] && 
      !v[22] && !v[23] && !v[4 ] && !v[5 ] && !v[21] && !v[3] && 
      !v[12] && !v[11] && !v[10] && !v[13] && 
      !v[8 ] && !v[9 ] && !v[14] && !v[15] && !v[16]) return 1;

  if ((v[9 ] || v[8] || v[16] || v[15] || v[0] || v[7] || v[6]) &&
      !v[10] && !v[11] && !v[1 ] && !v[2 ] && 
      !v[13] && !v[14] && !v[4 ] && !v[5 ] && !v[12] && !v[3] && 
      !v[21] && !v[20] && !v[19] && !v[22] && 
      !v[17] && !v[18] && !v[23] && !v[24] && !v[25]) return 1;

  if ((v[13] || v[8] || v[14] || v[15] || v[4] || v[5] || v[6]) &&
      !v[12] && !v[11] && !v[3 ] && !v[2 ] && 
      !v[9 ] && !v[16] && !v[0 ] && !v[7 ] && !v[10] && !v[1] && 
      !v[21] && !v[20] && !v[19] && !v[22] && 
      !v[17] && !v[18] && !v[23] && !v[24] && !v[25]) return 1;

  if ((v[11] || v[10] || v[8] || v[9] || v[2] || v[1] || v[0]) &&
      !v[12] && !v[13] && !v[3 ] && !v[4 ] && 
      !v[15] && !v[16] && !v[6 ] && !v[7 ] && !v[14] && !v[5] && 
      !v[21] && !v[20] && !v[19] && !v[22] && 
      !v[17] && !v[18] && !v[23] && !v[24] && !v[25]) return 1;

  if ((v[12] || v[11] || v[13] || v[8] || v[3 ] || v[2 ] || v[4 ]) &&
      !v[10] && !v[9 ] && !v[1 ] && !v[0 ] && 
      !v[14] && !v[15] && !v[5 ] && !v[6 ] && !v[16] && !v[7] && 
      !v[21] && !v[20] && !v[19] && !v[22] && 
      !v[17] && !v[18] && !v[23] && !v[24] && !v[25]) return 1;

  return 0;
} // match_end()

/* ==================================== */
static int32_t match_vois2(uint8_t *v)
/* ==================================== */
/*
               12      11      10       
               13       8       9
               14      15      16

		3	2	1			
		4      26	0
		5	6	7
Teste si les conditions suivantes sont r�unies:
1: v[8] et v[26] doivent �tre dans l'objet et simples
2: for i = 0 to 7 do w[i] = v[i] || v[i+9] ; w[0...7] doit �tre non 2D-simple
Si le test r�ussit, les points 8, 26 sont marqu�s DCRUCIAL
*/
{
  uint8_t t;
#ifdef DEBUG
  if (trace)
  {  
    printf("match_vois2\n");
    print_vois(v);
  }
#endif
  if (!IS_SIMPLE(v[8]) || !IS_SIMPLE(v[26])) return 0;
  if (v[0] || v[9]) t = 1; else t = 0;
  if (v[1] || v[10]) t |= 2;
  if (v[2] || v[11]) t |= 4;
  if (v[3] || v[12]) t |= 8;
  if (v[4] || v[13]) t |= 16;
  if (v[5] || v[14]) t |= 32;
  if (v[6] || v[15]) t |= 64;
  if (v[7] || v[16]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
  SET_DCRUCIAL(v[8]);
  SET_DCRUCIAL(v[26]);
#ifdef DEBUG
  if (trace)
    printf("match !\n");
#endif
  return 1;
} // match_vois2()

/* ==================================== */
static int32_t match_vois2s(uint8_t *v)
/* ==================================== */
/*
               12      11      10       
               13       8       9
               14      15      16

		3	2	1			
		4      26	0
		5	6	7
Pour les conditions de courbe et de surface.
Teste si les deux conditions suivantes sont r�unies:
1: v[8] et v[26] doivent �tre simples
2: for i = 0 to 7 do w[i] = v[i] || v[i+9] ; w[0...7] doit �tre non 2D-simple
Si le test r�ussit, alors les points 8, 26 sont marqu�s DCRUCIAL, de plus:
  Si t4b(w[0...7]) == 0 alors les points 8, 26 sont marqu�s SURF
  Sinon, si t8(w[0...7]) > 1 alors les points 8, 26 sont marqu�s CURVE
*/
{
  uint8_t t;
#ifdef DEBUG
  if (trace)
  {  
    printf("match_vois2s\n");
    print_vois(v);
  }
#endif
  if (!IS_SIMPLE(v[8]) || !IS_SIMPLE(v[26])) return 0;
  if (v[0] || v[9]) t = 1; else t = 0;
  if (v[1] || v[10]) t |= 2;
  if (v[2] || v[11]) t |= 4;
  if (v[3] || v[12]) t |= 8;
  if (v[4] || v[13]) t |= 16;
  if (v[5] || v[14]) t |= 32;
  if (v[6] || v[15]) t |= 64;
  if (v[7] || v[16]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
  SET_DCRUCIAL(v[8]);
  SET_DCRUCIAL(v[26]);
  if (t4b(t) == 0) { SET_SURF(v[8]); SET_SURF(v[26]); }
#ifdef NEW_ISTHMUS
  else if (t8(t) == 2) { SET_CURVE(v[8]); SET_CURVE(v[26]); }
#else
  else if (t8(t) > 1) { SET_CURVE(v[8]); SET_CURVE(v[26]); }
#endif
#ifdef DEBUG
  if (trace)
    printf("match !\n");
#endif
  return 1;
} // match_vois2s()

/* ==================================== */
static int32_t match_vois1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisations possibles :
// 12 11   3  2   21 20 
// 13  8   4 26   22 17
// et :
// 11 10    2 1   20 19
//  8  9   26 0   17 18
//
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s DCRUCIAL
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, les points Pi non nuls sont marques DCRUCIAL
{
  int32_t ret = 0;
#ifdef DEBUG
  if (trace)
  {  
    printf("match_vois1\n");
    print_vois(v);
  }
#endif
  if (!((v[2] && v[4]) || (v[3] && v[26]))) goto next1;
  if ((IS_OBJECT(v[2])  && (!IS_SIMPLE(v[2])  || IS_DCRUCIAL(v[2]))) ||
      (IS_OBJECT(v[3])  && (!IS_SIMPLE(v[3])  || IS_DCRUCIAL(v[3]))) ||
      (IS_OBJECT(v[4])  && (!IS_SIMPLE(v[4])  || IS_DCRUCIAL(v[4]))) ||
      (IS_OBJECT(v[26]) && (!IS_SIMPLE(v[26]) || IS_DCRUCIAL(v[26])))) goto next1;
  if ((v[12] || v[11] || v[13] || v[8] || v[21] || v[20] || v[22] || v[17]) &&
      ((!v[12] && !v[11] && !v[13] && !v[8]) || 
       (!v[21] && !v[20] && !v[22] && !v[17]))) goto next1;
  if (v[2])  SET_DCRUCIAL(v[2]);
  if (v[3])  SET_DCRUCIAL(v[3]);
  if (v[4])  SET_DCRUCIAL(v[4]);
  if (v[26]) SET_DCRUCIAL(v[26]);
  ret = 1;
 next1:
  if (!((v[2] && v[0]) || (v[1] && v[26]))) goto next2;
  if ((IS_OBJECT(v[2])  && (!IS_SIMPLE(v[2])  || IS_DCRUCIAL(v[2]))) ||
      (IS_OBJECT(v[1])  && (!IS_SIMPLE(v[1])  || IS_DCRUCIAL(v[1]))) ||
      (IS_OBJECT(v[0])  && (!IS_SIMPLE(v[0])  || IS_DCRUCIAL(v[0]))) ||
      (IS_OBJECT(v[26]) && (!IS_SIMPLE(v[26]) || IS_DCRUCIAL(v[26])))) goto next2;
  if ((v[10] || v[11] || v[9] || v[8] || v[19] || v[20] || v[18] || v[17]) &&
      ((!v[10] && !v[11] && !v[9] && !v[8]) || 
       (!v[19] && !v[20] && !v[18] && !v[17]))) goto next2;
  if (v[2])  SET_DCRUCIAL(v[2]);
  if (v[1])  SET_DCRUCIAL(v[1]);
  if (v[0])  SET_DCRUCIAL(v[0]);
  if (v[26]) SET_DCRUCIAL(v[26]);
  ret = 1;
 next2:
#ifdef DEBUG
  if (trace && ret)
    printf("match !\n");
#endif
  return ret;
} // match_vois1()

/* ==================================== */
static int32_t match_vois1s(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisations possibles :
// 12 11   3  2   21 20 
// 13  8   4 26   22 17
// et :
// 11 10    2 1   20 19
//  8  9   26 0   17 18
//
// Pour la condition de courbe. 
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non DCRUCIAL
// 3: au moins un A non nul et au moins un B non nul
// Si le test r�ussit, les points Pi non nuls sont marques CURVE
{
  int32_t ret = 0;
#ifdef DEBUG
  if (trace)
  {  
    printf("match_vois1\n");
    print_vois(v);
  }
#endif
  if (!((v[2] && v[4]) || (v[3] && v[26]))) goto next1;
  if ((IS_OBJECT(v[2])  && (!IS_SIMPLE(v[2]) || IS_DCRUCIAL(v[2]))) ||
      (IS_OBJECT(v[3])  && (!IS_SIMPLE(v[3]) || IS_DCRUCIAL(v[3]))) ||
      (IS_OBJECT(v[4])  && (!IS_SIMPLE(v[4]) || IS_DCRUCIAL(v[4]))) ||
      (IS_OBJECT(v[26]) && (!IS_SIMPLE(v[26]) || IS_DCRUCIAL(v[26])))) goto next1;
  if ((!v[12] && !v[11] && !v[13] && !v[8]) || 
      (!v[21] && !v[20] && !v[22] && !v[17])) goto next1;
  if (v[2])  SET_CURVE(v[2]);
  if (v[3])  SET_CURVE(v[3]);
  if (v[4])  SET_CURVE(v[4]);
  if (v[26]) SET_CURVE(v[26]);
  ret = 1;
 next1:
  if (!((v[2] && v[0]) || (v[1] && v[26]))) goto next2;
  if ((IS_OBJECT(v[2])  && (!IS_SIMPLE(v[2]) || IS_DCRUCIAL(v[2]))) ||
      (IS_OBJECT(v[1])  && (!IS_SIMPLE(v[1]) || IS_DCRUCIAL(v[1]))) ||
      (IS_OBJECT(v[0])  && (!IS_SIMPLE(v[0]) || IS_DCRUCIAL(v[0]))) ||
      (IS_OBJECT(v[26]) && (!IS_SIMPLE(v[26]) || IS_DCRUCIAL(v[26])))) goto next2;
  if ((!v[10] && !v[11] && !v[9] && !v[8]) || 
      (!v[19] && !v[20] && !v[18] && !v[17])) goto next2;
  if (v[2])  SET_CURVE(v[2]);
  if (v[1])  SET_CURVE(v[1]);
  if (v[0])  SET_CURVE(v[0]);
  if (v[26]) SET_CURVE(v[26]);
  ret = 1;
 next2:
#ifdef DEBUG
  if (trace)
    printf("match !\n");
#endif
  return ret;
} // match_vois1s()

/* ==================================== */
static int32_t match_vois0(uint8_t *v)
/* ==================================== */
/*
               12      11
               13       8

		3	2
		4      26

Teste si les conditions suivantes sont r�unies:
1: au moins un des ensembles {12,26}, {11,4}, {13,2}, {8,3} est inclus dans l'objet, et
2: les points non nuls sont tous simples, et non marqu�s DCRUCIAL
Si le test r�ussit, les points non nuls sont marqu�s DCRUCIAL
*/
{
#ifdef DEBUG
  if (trace)
  {  
    printf("match_vois0\n");
    print_vois(v);
  }
#endif
  if (!v[26]) return 0;
  if (!IS_SIMPLE(v[26]) || IS_DCRUCIAL(v[26])) return 0;
  if (!(v[12] || v[10] || v[14] || v[21])) return 0;
  if (v[12])
  { /*         12      11
               13       8

		3	2
		4      26 */
     if (!IS_SIMPLE(v[12]) || IS_DCRUCIAL(v[12])) return 0;
     if (v[11] && (!IS_SIMPLE(v[11]) || IS_DCRUCIAL(v[11]))) return 0;
     if (v[13] && (!IS_SIMPLE(v[13]) || IS_DCRUCIAL(v[13]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_DCRUCIAL(v[ 8]))) return 0;
     if (v[ 3] && (!IS_SIMPLE(v[ 3]) || IS_DCRUCIAL(v[ 3]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_DCRUCIAL(v[ 2]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_DCRUCIAL(v[ 4]))) return 0;
     if (v[12]) SET_DCRUCIAL(v[12]);
     if (v[11]) SET_DCRUCIAL(v[11]);
     if (v[ 4]) SET_DCRUCIAL(v[ 4]);
     if (v[13]) SET_DCRUCIAL(v[13]);
     if (v[ 2]) SET_DCRUCIAL(v[ 2]);
     if (v[ 8]) SET_DCRUCIAL(v[ 8]);
     if (v[ 3]) SET_DCRUCIAL(v[ 3]);
  }
  if (v[10])
  { /*
               11      10
               8       9

		2	1
		26	0 */
     if (!IS_SIMPLE(v[10]) || IS_DCRUCIAL(v[10])) return 0;
     if (v[11] && (!IS_SIMPLE(v[11]) || IS_DCRUCIAL(v[11]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_DCRUCIAL(v[ 8]))) return 0;
     if (v[ 9] && (!IS_SIMPLE(v[ 9]) || IS_DCRUCIAL(v[ 9]))) return 0;
     if (v[ 1] && (!IS_SIMPLE(v[ 1]) || IS_DCRUCIAL(v[ 1]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_DCRUCIAL(v[ 2]))) return 0;
     if (v[ 0] && (!IS_SIMPLE(v[ 0]) || IS_DCRUCIAL(v[ 0]))) return 0;
     if (v[10]) SET_DCRUCIAL(v[10]);
     if (v[11]) SET_DCRUCIAL(v[11]);
     if (v[ 8]) SET_DCRUCIAL(v[ 8]);
     if (v[ 9]) SET_DCRUCIAL(v[ 9]);
     if (v[ 1]) SET_DCRUCIAL(v[ 1]);
     if (v[ 2]) SET_DCRUCIAL(v[ 2]);
     if (v[ 0]) SET_DCRUCIAL(v[ 0]);
  }
  if (v[14])
  { /*         13       8
               14      15

		4      26
		5	6 */
     if (!IS_SIMPLE(v[14]) || IS_DCRUCIAL(v[14])) return 0;
     if (v[13] && (!IS_SIMPLE(v[13]) || IS_DCRUCIAL(v[13]))) return 0;
     if (v[15] && (!IS_SIMPLE(v[15]) || IS_DCRUCIAL(v[15]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_DCRUCIAL(v[ 8]))) return 0;
     if (v[ 6] && (!IS_SIMPLE(v[ 6]) || IS_DCRUCIAL(v[ 6]))) return 0;
     if (v[ 5] && (!IS_SIMPLE(v[ 5]) || IS_DCRUCIAL(v[ 5]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_DCRUCIAL(v[ 4]))) return 0;
     if (v[14]) SET_DCRUCIAL(v[14]);
     if (v[13]) SET_DCRUCIAL(v[13]);
     if (v[15]) SET_DCRUCIAL(v[15]);
     if (v[ 8]) SET_DCRUCIAL(v[ 8]);
     if (v[ 6]) SET_DCRUCIAL(v[ 6]);
     if (v[ 5]) SET_DCRUCIAL(v[ 5]);
     if (v[ 4]) SET_DCRUCIAL(v[ 4]);
  }
  if (v[21])
  {  /*		3	2
		4      26

               21      20
               22      17 */
     if (!IS_SIMPLE(v[21]) || IS_DCRUCIAL(v[21])) return 0;
     if (v[17] && (!IS_SIMPLE(v[17]) || IS_DCRUCIAL(v[17]))) return 0;
     if (v[20] && (!IS_SIMPLE(v[20]) || IS_DCRUCIAL(v[20]))) return 0;
     if (v[22] && (!IS_SIMPLE(v[22]) || IS_DCRUCIAL(v[22]))) return 0;
     if (v[ 3] && (!IS_SIMPLE(v[ 3]) || IS_DCRUCIAL(v[ 3]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_DCRUCIAL(v[ 2]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_DCRUCIAL(v[ 4]))) return 0;
     if (v[21]) SET_DCRUCIAL(v[21]);
     if (v[17]) SET_DCRUCIAL(v[17]);
     if (v[20]) SET_DCRUCIAL(v[20]);
     if (v[22]) SET_DCRUCIAL(v[22]);
     if (v[ 3]) SET_DCRUCIAL(v[ 3]);
     if (v[ 2]) SET_DCRUCIAL(v[ 2]);
     if (v[ 4]) SET_DCRUCIAL(v[ 4]);
  }
  SET_DCRUCIAL(v[26]);

#ifdef DEBUG
  if (trace)
    printf("match !\n");
#endif
  return 1;
} // match_vois0()

/* ==================================== */
static int32_t match2(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (match_vois2(v)) ret = 1;
  isometrieXZ_vois(v);
  if (match_vois2(v)) ret = 1;
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  if (match_vois2(v)) ret = 1;
  isometrieYZ_vois(v); // n�cessaire � cause du insert_vois qui suit
  return ret;
} /* match2() */

/* ==================================== */
static int32_t match2s(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (match_vois2s(v)) ret = 1;
  isometrieXZ_vois(v);
  if (match_vois2s(v)) ret = 1;
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  if (match_vois2s(v)) ret = 1;
  isometrieYZ_vois(v); // n�cessaire � cause du insert_vois qui suit
  return ret;
} /* match2s() */

/* ==================================== */
static int32_t match1(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (match_vois1(v)) ret = 1;
  isometrieXZ_vois(v);
  if (match_vois1(v)) ret = 1;
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  if (match_vois1(v)) ret = 1;
  isometrieYZ_vois(v); // n�cessaire � cause du insert_vois qui suit
  return ret;
} /* match1() */

/* ==================================== */
static int32_t match1s(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (match_vois1s(v)) ret = 1;
  isometrieXZ_vois(v);
  if (match_vois1s(v)) ret = 1;
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  if (match_vois1s(v)) ret = 1;
  isometrieYZ_vois(v); // n�cessaire � cause du insert_vois qui suit
  return ret;
} /* match1s() */

/* ==================================== */
static int32_t match0(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (match_vois0(v)) ret = 1;
  return ret;
} /* match0() */

/* ==================================== */
int32_t lskelMK3a(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique ultime
Algo MK3a donn�es: S
R�p�ter jusqu'� stabilit�
  P := voxels simples pour S
  R := voxels de P � pr�server (match2, match1, match0)
  T :=  [S  \  P]  \cup  R
  S := T \cup [S \ (T \oplus \Gamma_26*)]

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelMK3a"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  struct xvimage *r = copyimage(image); 
  uint8_t *R = UCHARDATA(r);
  int32_t step, nonstab;
  uint8_t v[27];

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image not implemented\n", F_NAME);
    return 0;
  }

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("%s: step %d\n", F_NAME, step);
#endif

    // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
#ifdef DEBUG_SKEL_MK3A
writeimage(image,"_S");
#endif

    // DEUXIEME SOUS-ITERATION : MARQUE LES CLIQUES CRUCIALES CORRESPONDANT AUX 2-FACES
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	{
#ifdef DEBUG_SKEL_MK3A
printf("match 2-clique\n");
#endif	  
	  insert_vois(v, S, i, rs, ps, N);
	}
      }

    // TROISIEME SOUS-ITERATION : MARQUE LES CLIQUES CRUCIALES CORRESPONDANT AUX 1-FACES
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	{
#ifdef DEBUG_SKEL_MK3A
printf("match 1-clique\n");
#endif	  
	  insert_vois(v, S, i, rs, ps, N);
	}
      }

    // QUATRIEME SOUS-ITERATION : MARQUE LES CLIQUES CRUCIALES CORRESPONDANT AUX 0-FACES
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	{
#ifdef DEBUG_SKEL_MK3A
printf("match 0-clique\n");
#endif	  
	  insert_vois(v, S, i, rs, ps, N);
	}
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG_SKEL_MK3A
writeimage(t,"_T");
#endif

    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (mctopo3d_nbvoiso26(T, i, rs, ps, N) >= 1) R[i] = 1; // calcule R = Dilat(T)
    for (i = 0; i < N; i++)
      if (T[i] || (S[i] && !R[i])) T[i] = 1; else T[i] = 0; // T := T \cup [S \ R]

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  freeimage(r);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelMK3a() */

/* ==================================== */
int32_t lskelEK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique curviligne bas� sur les extr�mit�s
Algo EK3 donn�es: S
R�p�ter jusqu'� stabilit�
  E := points extr�mit� de S
  P := voxels simples pour S et pas dans E
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelEK3"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // AJOUTE LES EXTREMITES DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match_end(v)) I[i] = 1;
      }

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelEK3() */

/* ==================================== */
int32_t lskelCK3a(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique curviligne
Algo CK3a donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de courbe de S
  I := I \cup C
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelCK3a"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("%s: step %d\n", F_NAME, step);
#endif

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // TROISIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (1)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE COURBE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
#ifdef NEW_ISTHMUS
	if ((top == 2) && (topb == 1)) SET_CURVE(S[i]);
#else
	if (top > 1) SET_CURVE(S[i]);
#endif
      }
    }
    // DEMARQUE PTS DE COURBE ET LES MEMORISE DANS I
    for (i = 0; i < N; i++)
    { 
      UNSET_DCRUCIAL(S[i]);
      if (IS_CURVE(S[i])) { UNSET_SIMPLE(S[i]); I[i] = 1; }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	{
#ifdef DEBUG_SKEL_CK3A
printf("match 0-clique\n");
#endif	  
	  insert_vois(v, S, i, rs, ps, N);
	}
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelCK3a() */

/* ==================================== */
int32_t lskelCK3b(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique curviligne
Variante avec reconstruction des points extr�mit�s
Algo CK3b donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de courbe de S
  E := points extr�mit�s de S
  C := C union [E inter gamma(C)] 
  I := I \cup C
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelCK3b"
{ 
  index_t i, j, k;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  struct xvimage *e = copyimage(image); 
  uint8_t *E = UCHARDATA(e);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // AJOUTE LES EXTREMITES DANS E
    memset(E, 0, N);
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match_end(v)) E[i] = 1;
      }
    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // TROISIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (1)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE COURBE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
#ifdef NEW_ISTHMUS
	if ((top == 2) && (topb == 1)) SET_CURVE(S[i]);
#else
	if (top > 1) SET_CURVE(S[i]);
#endif
      }
    }

    // DEMARQUE PTS DE COURBE ET LES MEMORISE DANS I
    // AJOUTE AUX POINTS DE COURBE LEURS VOISINS QUI SONT DANS E
    for (i = 0; i < N; i++)
    { 
      UNSET_DCRUCIAL(S[i]);
      if (IS_CURVE(S[i])) 
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {
          j = voisin26(i, k, rs, ps, N);
          if ((j != -1) && E[j])
	  {
	    UNSET_SIMPLE(S[j]); 
	    I[j] = 1; 
	  }
	}
	UNSET_SIMPLE(S[i]); 
	I[i] = 1; 
      }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  freeimage(e);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelCK3b() */

/* ==================================== */
int32_t lskelCK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique curviligne, variante
Les points "candidats" � devenir des points de courbes sont les
points qui ne sont pas voisins d'un point isthme 2D ni d'un point interieur
Algo CK3 donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de courbe de S
  P := voxels simples pour S et pas dans C
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelCK3"
{ 
  index_t i, j, k;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // MARQUE LES POINTS DE SURFACE (2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE SURFACE (3) ET LES POINTS INTERIEURS
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (topb > 1) SET_SURF(S[i]);
	if (topb == 0) SET_SELECTED(S[i]);
      }
    }

    // DEMARQUE PTS ET REND "NON-SIMPLES" LES CANDIDATS
    for (i = 0; i < N; i++)
    { 
      UNSET_DCRUCIAL(S[i]);
      if (IS_OBJECT(S[i])) 
      {
#ifdef RESIDUEL6
        for (k = 0; k < 12; k += 2)        /* parcourt les voisins en 6-connexite */
        {
          j = voisin6(i, k, rs, ps, N);
          if ((j != -1) && IS_SELECTED(S[j])) break;
	}
	if (k == 12) // le voxel est r�siduel
#else
	for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {
	  j = voisin26(i, k, rs, ps, N);
          if ((j != -1) && IS_SELECTED(S[j])) break;
	}
	if (k == 26) // le voxel est r�siduel
#endif
	{
	  for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
          {
	    j = voisin26(i, k, rs, ps, N);
	    if ((j != -1) && IS_SURF(S[j]))break;
	  }
	  if (k == 26) UNSET_SIMPLE(S[i]);
	}
      }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelCK3() */

/* ==================================== */
int32_t lskelAK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit,
	     int32_t filter)
/* ==================================== */
/*
Amincissement sym�trique avec inclusion de l'axe m�dian

A REVOIR : 
- pb des "�chancrures"
- si on filtre l'axe m�dian alors l'homotopie n'est plus garantie ?
(cf. prop. sur les 0-cliques cruciales)

Algo AK3 donn�es: S
K := \emptyset ; T := S
R�p�ter jusqu'� stabilit�
  E := T \ominus \Gamma_6
  D := T \ [E \oplus \Gamma_6]
  T := E
  K := K \cup D
  P := voxels de S \ K simples pour S
  R := voxels de P qui s'apparient avec le masque C (bertrand_match3)
  S := [S  \  P]  \cup  R

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelAK3"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  struct xvimage *r = copyimage(image); 
  struct xvimage *e = copyimage(image); 
  uint8_t *E = UCHARDATA(e);
  struct xvimage *d = copyimage(image); 
  uint8_t *D = UCHARDATA(d);
  struct xvimage *k = copyimage(image); 
  uint8_t *K = UCHARDATA(k);
  int32_t step, nonstab;
  uint8_t v[27];

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image not implemented\n", F_NAME);
    return 0;
  }

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = 1; // normalize values

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  // K := \emptyset ; T := S
  memset(K, 0, N);
  memcpy(T, S, N);
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    //  E := T \ominus \Gamma_6 
    memset(E, 0, N);
    for (i = 0; i < N; i++) 
      if (T[i] && (mctopo3d_nbvoiso6(T, i, rs, ps, N) == 6)) E[i] = 1;

    //  D := E \oplus \Gamma_6
    memset(D, 0, N);
    for (i = 0; i < N; i++)
      if (E[i] || (mctopo3d_nbvoiso6(E, i, rs, ps, N) >= 1)) D[i] = 1;

    //  D := T \ D
    for (i = 0; i < N; i++)
      if (T[i] && !D[i]) D[i] = 1; else D[i] = 0;

    //  T := E
    memcpy(T, E, N);

    //  K := K \cup D
    for (i = 0; i < N; i++)
      if (D[i]) K[i] = 1;

    // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES qui ne sont pas dans K
    if (step > filter)
    {
      for (i = 0; i < N; i++) 
	if (S[i] && !K[i] && mctopo3d_simple26(S, i, rs, ps, N))
	  SET_SIMPLE(S[i]);
    }
    else
    {
      for (i = 0; i < N; i++) 
	if (S[i] && mctopo3d_simple26(S, i, rs, ps, N))
	  SET_SIMPLE(S[i]);
    }

    // DEUXIEME SOUS-ITERATION : MARQUE LES CLIQUES CRUCIALES CORRESPONDANT AUX 2-FACES
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // TROISIEME SOUS-ITERATION : MARQUE LES CLIQUES CRUCIALES CORRESPONDANT AUX 1-FACES
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  //if (match1s(v))  // VARIANTE POUR EVITER LES "ECHANCRURES" (� voir)
	  insert_vois(v, S, i, rs, ps, N);
      }

    // D := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
    memset(D, 0, N);
    for (i = 0; i < N; i++) 
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	D[i] = 1;

    for (i = 0; i < N; i++) // pour  tester la stabilit�
      if (S[i] && !D[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  freeimage(r);
  freeimage(e);
  freeimage(d);
  freeimage(k);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelAK3() */

/* ==================================== */
int32_t lskelMK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique ultime avec ensemble de contrainte
Version r�vis�e d'apr�s le papier IWCIA 2006
Algo MK3 donn�es: S, I
R�p�ter jusqu'� stabilit�
  P := voxels simples pour S et non dans I
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelMK3"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I = NULL;
  int32_t step, nonstab;
  uint8_t v[27];

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  if (inhibit != NULL) I = UCHARDATA(inhibit);

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("MK3 step %d\n", step);
#endif

    // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES ET PAS DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N) && (!I || !I[i]))
	SET_SIMPLE(S[i]);
    // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // TROISIEME SOUS-ITERATION : MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // QUATRIEME SOUS-ITERATION : MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelMK3() */

/* ==================================== */
int32_t ldisttopo3(struct xvimage *image, 
		   struct xvimage *inhibit,
		   struct xvimage *res)
/* ==================================== */
/*
Idem squelette sym�trique ultime (algo MK3).
Marque les points (dans res) par le nombre d'it�rations n�cessaires � leur enl�vement.
Les points non enlev�s sont marqu�s MARK_INFTY.
*/
#undef F_NAME
#define F_NAME "ldisttopo3"
//#define MARK_INFTY 2000000000
#define MARK_INFTY 255
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint32_t *O = ULONGDATA(res); 
  int32_t step, nonstab;
  uint8_t v[27];
  
  if ((rowsize(res) != rs) || (colsize(res) != cs) || (depth(res) != ds))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }
  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: image type must be uint8_t\n", F_NAME);
    return(0);
  }
  if (datatype(res) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "%s: result type must be uint32_t\n", F_NAME);
    return(0);
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  for (i = 0; i < N; i++) if (S[i]) O[i] = MARK_INFTY; else O[i] = 0;

  if (inhibit != NULL) 
  {
    fprintf(stderr, "%s: inhibit not yet implemented\n", F_NAME);
    return(0);
  }


  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab)
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("disttopo step %d\n", step);
#endif

    // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
#ifdef DEBUG
writeimage(image,"_S");
#endif
    // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // TROISIEME SOUS-ITERATION : MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // QUATRIEME SOUS-ITERATION : MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	O[i] = step;
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* ldisttopo3() */

/* ==================================== */
int32_t ldistaxetopo3(struct xvimage *image, 
		      struct xvimage *inhibit,
		      struct xvimage *res)
/* ==================================== */
/*
Idem squelette sym�trique ultime (algo MK3).
Marque les points (dans res) par le nombre d'it�rations n�cessaires � leur enl�vement.
Les points non enlev�s sont marqu�s MARK_INFTY.
Retourne dans image l'axe topologique.
*/
#undef F_NAME
#define F_NAME "ldistaxetopo3"
#undef MARK_INFTY
#define MARK_INFTY 2000000000
//#define MARK_INFTY 255
{ 
  index_t i, j, k;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  struct xvimage *r = copyimage(image); 
  uint8_t *R = UCHARDATA(r);
  uint32_t *O = ULONGDATA(res); 
  int32_t step, nonstab, minvois;
  uint8_t v[27];
  
  if ((rowsize(res) != rs) || (colsize(res) != cs) || (depth(res) != ds))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }
  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: image type must be uint8_t\n", F_NAME);
    return(0);
  }
  if (datatype(res) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "%s: result type must be uint32_t\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image not yet implemented\n", F_NAME);
    return 0;
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  for (i = 0; i < N; i++) if (S[i]) O[i] = MARK_INFTY; else O[i] = 0;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab)
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // DEUXIEME SOUS-ITERATION : MARQUE LES CLIQUES CRUCIALES CORRESPONDANT AUX 2-FACES
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // TROISIEME SOUS-ITERATION : MARQUE LES CLIQUES CRUCIALES CORRESPONDANT AUX 1-FACES
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (mctopo3d_nbvoiso26(T, i, rs, ps, N) >= 1) R[i] = 1; // calcule R = Dilat(T)
    for (i = 0; i < N; i++)
      if (T[i] || (S[i] && !R[i])) T[i] = 1; else T[i] = 0; // T := T \cup [S \ R]

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
	O[i] = step;
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  } // while (nonstab)

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  // CALCUL AXE TOPOLOGIQUE
  memset(S, 0, N);
  for (i = 0; i < N; i++) if (O[i] > 0)
  {
    if (O[i] == MARK_INFTY)
      S[i] = 255;
    else
    {
      minvois = MARK_INFTY;
#define VOISIN26
#ifdef VOISIN6
      for (k = 0; k <= 10; k += 2)        /* parcourt les voisins en 6-connexite */
      {
	j = voisin6(i, k, rs, ps, N);
	if ((j != -1) && (O[j] > 0) && (O[j] < minvois)) minvois = O[j];
      }
#endif
#ifdef VOISIN26
      for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
      {
	j = voisin26(i, k, rs, ps, N);
	if ((j != -1) && (O[j] > 0) && (O[j] < minvois)) minvois = O[j];
      }
#endif
      if ((O[i] - minvois) > 1)  S[i] = 255;
    }
  }

  freeimage(t);
  freeimage(r);
  mctopo3d_termine_topo3d();
  return(1);
} /* ldistaxetopo3() */

#ifdef TEST
int32_t main()
{
  uint8_t V[27] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26
  };

  print_vois(V);
  isometrieYZ_vois(V);
  print_vois(V);
  isometrieYZ_vois(V);
  print_vois(V);
  isometrieXZ_vois(V);
  print_vois(V);
  isometrieXZ_vois(V);
  print_vois(V);
}
#endif


// ===================================================================
// ===================================================================
// VERSIONS ASYMETRIQUES
// ===================================================================
// ===================================================================

/* ==================================== */
int32_t seq_asym_match_vois2(uint8_t *v)
/* ==================================== */
/*
               12      11      10       
               13       8       9
               14      15      16

		3	2	1			
		4      26	0
		5	6	7
Teste si les conditions suivantes sont r�unies:
1: v[8] et v[26] doivent �tre dans l'objet et simples et non s�lectionn�s
2: for i = 0 to 7 do w[i] = v[i] || v[i+9] ; w[0...7] doit �tre non 2D-simple
Si le test r�ussit, le point 8 est marqu� SELECTED
*/
{
  uint8_t t;
  if (!IS_SIMPLE(v[8]) || !IS_SIMPLE(v[26]) || IS_SELECTED(v[8]) || IS_SELECTED(v[26])) return 0;
  if (v[0] || v[9]) t = 1; else t = 0;
  if (v[1] || v[10]) t |= 2;
  if (v[2] || v[11]) t |= 4;
  if (v[3] || v[12]) t |= 8;
  if (v[4] || v[13]) t |= 16;
  if (v[5] || v[14]) t |= 32;
  if (v[6] || v[15]) t |= 64;
  if (v[7] || v[16]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
  SET_SELECTED(v[8]);
  return 1;
} // seq_asym_match_vois2()

/* ==================================== */
int32_t asym_match_vois2(uint8_t *v)
/* ==================================== */
/*
               12      11      10       
               13       8       9
               14      15      16

		3	2	1			
		4      26	0
		5	6	7
Teste si les conditions suivantes sont r�unies:
1: v[8] et v[26] doivent �tre dans l'objet et simples
2: for i = 0 to 7 do w[i] = v[i] || v[i+9] ; w[0...7] doit �tre non 2D-simple
Si le test r�ussit, le point 8 est marqu� SELECTED
*/
{
  uint8_t t;
  if (!IS_SIMPLE(v[8]) || !IS_SIMPLE(v[26])) return 0;
  if (v[0] || v[9]) t = 1; else t = 0;
  if (v[1] || v[10]) t |= 2;
  if (v[2] || v[11]) t |= 4;
  if (v[3] || v[12]) t |= 8;
  if (v[4] || v[13]) t |= 16;
  if (v[5] || v[14]) t |= 32;
  if (v[6] || v[15]) t |= 64;
  if (v[7] || v[16]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
  SET_SELECTED(v[8]);
  return 1;
} // asym_match_vois2()

/* ==================================== */
int32_t seq_asym_match_vois1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisations possibles :
// 12 11   3  2   21 20 
// 13  8   4 26   22 17
// et :
// 11 10    2 1   20 19
//  8  9   26 0   17 18
//
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s SELECTED
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, un des points Pi non nuls est marqu� SELECTED
{
  int32_t ret = 0;
  if (!((v[2] && v[4]) || (v[3] && v[26]))) goto next1;
  if ((IS_OBJECT(v[2])  && (!IS_SIMPLE(v[2])  || IS_SELECTED(v[2]))) ||
      (IS_OBJECT(v[3])  && (!IS_SIMPLE(v[3])  || IS_SELECTED(v[3]))) ||
      (IS_OBJECT(v[4])  && (!IS_SIMPLE(v[4])  || IS_SELECTED(v[4]))) ||
      (IS_OBJECT(v[26]) && (!IS_SIMPLE(v[26]) || IS_SELECTED(v[26])))) goto next1;
  if ((v[12] || v[11] || v[13] || v[8] || v[21] || v[20] || v[22] || v[17]) &&
      ((!v[12] && !v[11] && !v[13] && !v[8]) || 
       (!v[21] && !v[20] && !v[22] && !v[17]))) goto next1;
  if (v[2])  SET_SELECTED(v[2]);
  else if (v[3])  SET_SELECTED(v[3]);
  else if (v[4])  SET_SELECTED(v[4]);
  else if (v[26]) SET_SELECTED(v[26]);
  ret = 1;
 next1:
  if (!((v[2] && v[0]) || (v[1] && v[26]))) goto next2;
  if ((IS_OBJECT(v[2])  && (!IS_SIMPLE(v[2])  || IS_SELECTED(v[2]))) ||
      (IS_OBJECT(v[1])  && (!IS_SIMPLE(v[1])  || IS_SELECTED(v[1]))) ||
      (IS_OBJECT(v[0])  && (!IS_SIMPLE(v[0])  || IS_SELECTED(v[0]))) ||
      (IS_OBJECT(v[26]) && (!IS_SIMPLE(v[26]) || IS_SELECTED(v[26])))) goto next2;
  if ((v[10] || v[11] || v[9] || v[8] || v[19] || v[20] || v[18] || v[17]) &&
      ((!v[10] && !v[11] && !v[9] && !v[8]) || 
       (!v[19] && !v[20] && !v[18] && !v[17]))) goto next2;
  if (v[2])  SET_SELECTED(v[2]);
  else if (v[1])  SET_SELECTED(v[1]);
  else if (v[0])  SET_SELECTED(v[0]);
  else if (v[26]) SET_SELECTED(v[26]);
  ret = 1;
 next2:
  return ret;
} // seq_asym_match_vois1()

/* ==================================== */
int32_t asym_match_vois1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisations possibles :
// 12 11   3  2   21 20 
// 13  8   4 26   22 17
// et :
// 11 10    2 1   20 19
//  8  9   26 0   17 18
//
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s DCRUCIAL
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, un des points Pi non nuls est marqu� SELECTED
{
  int32_t ret = 0;
  if (!((v[2] && v[4]) || (v[3] && v[26]))) goto next1;
  if ((IS_OBJECT(v[2])  && (!IS_SIMPLE(v[2])  || IS_DCRUCIAL(v[2]))) ||
      (IS_OBJECT(v[3])  && (!IS_SIMPLE(v[3])  || IS_DCRUCIAL(v[3]))) ||
      (IS_OBJECT(v[4])  && (!IS_SIMPLE(v[4])  || IS_DCRUCIAL(v[4]))) ||
      (IS_OBJECT(v[26]) && (!IS_SIMPLE(v[26]) || IS_DCRUCIAL(v[26])))) goto next1;
  if ((v[12] || v[11] || v[13] || v[8] || v[21] || v[20] || v[22] || v[17]) &&
      ((!v[12] && !v[11] && !v[13] && !v[8]) || 
       (!v[21] && !v[20] && !v[22] && !v[17]))) goto next1;
  if (v[2])  SET_SELECTED(v[2]);
  else if (v[3])  SET_SELECTED(v[3]);
  else if (v[4])  SET_SELECTED(v[4]);
  else if (v[26]) SET_SELECTED(v[26]);
  ret = 1;
 next1:
  if (!((v[2] && v[0]) || (v[1] && v[26]))) goto next2;
  if ((IS_OBJECT(v[2])  && (!IS_SIMPLE(v[2])  || IS_DCRUCIAL(v[2]))) ||
      (IS_OBJECT(v[1])  && (!IS_SIMPLE(v[1])  || IS_DCRUCIAL(v[1]))) ||
      (IS_OBJECT(v[0])  && (!IS_SIMPLE(v[0])  || IS_DCRUCIAL(v[0]))) ||
      (IS_OBJECT(v[26]) && (!IS_SIMPLE(v[26]) || IS_DCRUCIAL(v[26])))) goto next2;
  if ((v[10] || v[11] || v[9] || v[8] || v[19] || v[20] || v[18] || v[17]) &&
      ((!v[10] && !v[11] && !v[9] && !v[8]) || 
       (!v[19] && !v[20] && !v[18] && !v[17]))) goto next2;
  if (v[2])  SET_SELECTED(v[2]);
  else if (v[1])  SET_SELECTED(v[1]);
  else if (v[0])  SET_SELECTED(v[0]);
  else if (v[26]) SET_SELECTED(v[26]);
  ret = 1;
 next2:
  return ret;
} // asym_match_vois1()

/* ==================================== */
int32_t seq_asym_match_vois0(uint8_t *v)
/* ==================================== */
/*
               12      11      10
               13       8       9
               14      15      16

		3	2	1
		4      26	0
		5	6	7

               21      20      19
               22      17      18
               23      24      25

Teste si les conditions suivantes sont r�unies:
1: au moins un des ensembles {26,12}, {26,10}, {26,14}, {26,21} est inclus dans l'objet, et
2: les points non nuls du cube 2x2x2 contenant cet ensemble sont tous simples, 
   non marqu�s SELECTED
Si le test r�ussit, le point 26 est marqu� SELECTED
*/
{
  if (!v[26]) return 0;
  if (!IS_SIMPLE(v[26]) || IS_SELECTED(v[26])) return 0;
  if (!(v[12] || v[10] || v[14] || v[21])) return 0;
  if (v[12])
  { /*         12      11
               13       8

		3	2
		4      26 */
     if (!IS_SIMPLE(v[12]) || IS_SELECTED(v[12])) return 0;
     if (v[11] && (!IS_SIMPLE(v[11]) || IS_SELECTED(v[11]))) return 0;
     if (v[13] && (!IS_SIMPLE(v[13]) || IS_SELECTED(v[13]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_SELECTED(v[ 8]))) return 0;
     if (v[ 3] && (!IS_SIMPLE(v[ 3]) || IS_SELECTED(v[ 3]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_SELECTED(v[ 2]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_SELECTED(v[ 4]))) return 0;
  }
  if (v[10])
  { /*
               11      10
               8       9

		2	1
		26	0 */
     if (!IS_SIMPLE(v[10]) || IS_SELECTED(v[10])) return 0;
     if (v[11] && (!IS_SIMPLE(v[11]) || IS_SELECTED(v[11]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_SELECTED(v[ 8]))) return 0;
     if (v[ 9] && (!IS_SIMPLE(v[ 9]) || IS_SELECTED(v[ 9]))) return 0;
     if (v[ 1] && (!IS_SIMPLE(v[ 1]) || IS_SELECTED(v[ 1]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_SELECTED(v[ 2]))) return 0;
     if (v[ 0] && (!IS_SIMPLE(v[ 0]) || IS_SELECTED(v[ 0]))) return 0;
  }
  if (v[14])
  { /*         13       8
               14      15

		4      26
		5	6 */
     if (!IS_SIMPLE(v[14]) || IS_SELECTED(v[14])) return 0;
     if (v[13] && (!IS_SIMPLE(v[13]) || IS_SELECTED(v[13]))) return 0;
     if (v[15] && (!IS_SIMPLE(v[15]) || IS_SELECTED(v[15]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_SELECTED(v[ 8]))) return 0;
     if (v[ 6] && (!IS_SIMPLE(v[ 6]) || IS_SELECTED(v[ 6]))) return 0;
     if (v[ 5] && (!IS_SIMPLE(v[ 5]) || IS_SELECTED(v[ 5]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_SELECTED(v[ 4]))) return 0;
  }
  if (v[21])
  {  /*		3	2
		4      26

               21      20
               22      17 */
     if (!IS_SIMPLE(v[21]) || IS_SELECTED(v[21])) return 0;
     if (v[17] && (!IS_SIMPLE(v[17]) || IS_SELECTED(v[17]))) return 0;
     if (v[20] && (!IS_SIMPLE(v[20]) || IS_SELECTED(v[20]))) return 0;
     if (v[22] && (!IS_SIMPLE(v[22]) || IS_SELECTED(v[22]))) return 0;
     if (v[ 3] && (!IS_SIMPLE(v[ 3]) || IS_SELECTED(v[ 3]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_SELECTED(v[ 2]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_SELECTED(v[ 4]))) return 0;
  }
  SET_SELECTED(v[26]);
  return 1;
} // seq_asym_match_vois0()

/* ==================================== */
int32_t asym_match_vois0(uint8_t *v)
/* ==================================== */
/*
               12      11      10
               13       8       9
               14      15      16

		3	2	1
		4      26	0
		5	6	7

               21      20      19
               22      17      18
               23      24      25

Teste si les conditions suivantes sont r�unies:
1: au moins un des ensembles {26,12}, {26,10}, {26,14}, {26,21} est inclus dans l'objet, et
2: les points non nuls du cube 2x2x2 contenant cet ensemble sont tous simples, 
   non marqu�s DCRUCIAL
Si le test r�ussit, le point 26 est marqu� SELECTED
*/
{
  if (!v[26]) return 0;
  if (!IS_SIMPLE(v[26]) || IS_DCRUCIAL(v[26])) return 0;
  if (!(v[12] || v[10] || v[14] || v[21])) return 0;
  if (v[12])
  { /*         12      11
               13       8

		3	2
		4      26 */
     if (!IS_SIMPLE(v[12]) || IS_DCRUCIAL(v[12])) return 0;
     if (v[11] && (!IS_SIMPLE(v[11]) || IS_DCRUCIAL(v[11]))) return 0;
     if (v[13] && (!IS_SIMPLE(v[13]) || IS_DCRUCIAL(v[13]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_DCRUCIAL(v[ 8]))) return 0;
     if (v[ 3] && (!IS_SIMPLE(v[ 3]) || IS_DCRUCIAL(v[ 3]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_DCRUCIAL(v[ 2]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_DCRUCIAL(v[ 4]))) return 0;
  }
  if (v[10])
  { /*
               11      10
               8       9

		2	1
		26	0 */
     if (!IS_SIMPLE(v[10]) || IS_DCRUCIAL(v[10])) return 0;
     if (v[11] && (!IS_SIMPLE(v[11]) || IS_DCRUCIAL(v[11]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_DCRUCIAL(v[ 8]))) return 0;
     if (v[ 9] && (!IS_SIMPLE(v[ 9]) || IS_DCRUCIAL(v[ 9]))) return 0;
     if (v[ 1] && (!IS_SIMPLE(v[ 1]) || IS_DCRUCIAL(v[ 1]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_DCRUCIAL(v[ 2]))) return 0;
     if (v[ 0] && (!IS_SIMPLE(v[ 0]) || IS_DCRUCIAL(v[ 0]))) return 0;
  }
  if (v[14])
  { /*         13       8
               14      15

		4      26
		5	6 */
     if (!IS_SIMPLE(v[14]) || IS_DCRUCIAL(v[14])) return 0;
     if (v[13] && (!IS_SIMPLE(v[13]) || IS_DCRUCIAL(v[13]))) return 0;
     if (v[15] && (!IS_SIMPLE(v[15]) || IS_DCRUCIAL(v[15]))) return 0;
     if (v[ 8] && (!IS_SIMPLE(v[ 8]) || IS_DCRUCIAL(v[ 8]))) return 0;
     if (v[ 6] && (!IS_SIMPLE(v[ 6]) || IS_DCRUCIAL(v[ 6]))) return 0;
     if (v[ 5] && (!IS_SIMPLE(v[ 5]) || IS_DCRUCIAL(v[ 5]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_DCRUCIAL(v[ 4]))) return 0;
  }
  if (v[21])
  {  /*		3	2
		4      26

               21      20
               22      17 */
     if (!IS_SIMPLE(v[21]) || IS_DCRUCIAL(v[21])) return 0;
     if (v[17] && (!IS_SIMPLE(v[17]) || IS_DCRUCIAL(v[17]))) return 0;
     if (v[20] && (!IS_SIMPLE(v[20]) || IS_DCRUCIAL(v[20]))) return 0;
     if (v[22] && (!IS_SIMPLE(v[22]) || IS_DCRUCIAL(v[22]))) return 0;
     if (v[ 3] && (!IS_SIMPLE(v[ 3]) || IS_DCRUCIAL(v[ 3]))) return 0;
     if (v[ 2] && (!IS_SIMPLE(v[ 2]) || IS_DCRUCIAL(v[ 2]))) return 0;
     if (v[ 4] && (!IS_SIMPLE(v[ 4]) || IS_DCRUCIAL(v[ 4]))) return 0;
  }
  SET_SELECTED(v[26]);
  return 1;
} // asym_match_vois0()

/* ==================================== */
int32_t seq_asym_match2(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (seq_asym_match_vois2(v)) ret = 1;
  isometrieXZ_vois(v);
  if (seq_asym_match_vois2(v)) ret = 1;
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  if (seq_asym_match_vois2(v)) ret = 1;
  isometrieYZ_vois(v);
  return ret;
} /* seq_asym_match2() */

/* ==================================== */
int32_t seq_asym_match1(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (seq_asym_match_vois1(v)) ret = 1;
  isometrieXZ_vois(v);
  if (seq_asym_match_vois1(v)) ret = 1;
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  if (seq_asym_match_vois1(v)) ret = 1;
  isometrieYZ_vois(v);
  return ret;
} /* seq_asym_match1() */

/* ==================================== */
int32_t seq_asym_match0(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (seq_asym_match_vois0(v)) ret = 1;
  return ret;
} /* seq_asym_match0() */

/* ==================================== */
int32_t asym_match2(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (asym_match_vois2(v)) ret = 1;
  isometrieXZ_vois(v);
  if (asym_match_vois2(v)) ret = 1;
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  if (asym_match_vois2(v)) ret = 1;
  isometrieYZ_vois(v);
  return ret;
} /* asym_match2() */

/* ==================================== */
int32_t asym_match1(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (asym_match_vois1(v)) ret = 1;
  isometrieXZ_vois(v);
  if (asym_match_vois1(v)) ret = 1;
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  if (asym_match_vois1(v)) ret = 1;
  isometrieYZ_vois(v);
  return ret;
} /* asym_match1() */

/* ==================================== */
int32_t asym_match0(uint8_t *v)
/* ==================================== */
{
  int32_t ret = 0;
  if (asym_match_vois0(v)) ret = 1;
  return ret;
} /* asym_match0() */

/* ==================================== */
int32_t lskelAMK3(struct xvimage *image, 
		   int32_t n_steps,
		   struct xvimage *inhibit)
/* ==================================== */
/*
Squelette asym�trique ultime avec ensemble de contrainte
Algo AMK3c donn�es: S
R�p�ter jusqu'� stabilit�
  P := voxels simples pour S et non dans I
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelAMK3"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I = NULL;
  int32_t step, nonstab;
  uint8_t v[27];

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  if (inhibit != NULL) I = UCHARDATA(inhibit);

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("AMK3b step %d\n", step);
#endif

    // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES ET PAS DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N) && (!I || !I[i]))
	SET_SIMPLE(S[i]);

    // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // TROISIEME SOUS-ITERATION : MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // QUATRIEME SOUS-ITERATION : MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelAMK3() */

static int32_t is_end(uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
{
  int32_t k, n;
  index_t y;
  for (n = k = 0; k < 26; k += 1)
  {
    y = voisin26(p, k, rs, ps, N);
    if ((y != -1) && S[y]) n++;
  } // for k
  if (n == 1) return 1;
  return 0;
} //is_end()

/* ==================================== */
int32_t lskelAEK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette asym�trique curviligne bas� sur les points extr�mit�s
Algo AEK3 donn�es: S
R�p�ter jusqu'� stabilit�
  C := points extr�mit�s de S
  I := I \cup C
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelAEK3"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: n_steps = %d\n", F_NAME, n_steps);
#endif

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS EXTREMITES
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]))
	if (is_end(S, i, rs, ps, N)) 
	  SET_INHIBIT(I[i]);
    }

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !IS_INHIBIT(I[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelAEK3() */

#ifdef USE_NKP_END
static int32_t NKP_end(uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
{ // Condition C3 des papiers ICIAR et IASTED de Nemeth, Kalman, Palagyi
  int32_t k, n;
  index_t y, q, r;
  for (n = k = 0; k < 26; k += 1)
  {
    y = voisin26(p, k, rs, ps, N);
    if ((y != -1) && S[y]) { n++; q = y; }
  } // for k
  if (n != 1) return 0;
  for (n = k = 0; k < 26; k += 1)
  {
    y = voisin26(q, k, rs, ps, N);
    if ((y != -1) && S[y] && (y != p)) { n++; r = y; }
  } // for k
  if (n == 0) return 1;
  if ((n == 1) &&
      mctopo3d_nbvoiso26(S, r, rs, ps, N) <= 2) return 1;
  return 0;
} //NKP_end()
#endif

#ifdef ANCIENNE_VERSION_SEQ
/* ==================================== */
int32_t lskelACK3a_old(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t n_earlysteps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette asym�trique curviligne
Algo ACK3a donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de courbe de S
  I := I \cup C
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Les points de courbe d�tect�s dans les n_earlysteps premi�res �tapes
sont marqu�s dans l'image de sortie par une valeur 127 (au lieu de 255)

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelACK3a_old"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab, allocinhib = 0;
  int32_t top, topb;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: n_steps = %d ; n_earlysteps = %d\n", F_NAME, n_steps, n_earlysteps);
#endif

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
    allocinhib = 1;
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS DE COURBE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]))
      {    
//#ifdef USE_NKP_END
//	if (NKP_end(S, i, rs, ps, N)) SET_CURVE(S[i]);
#ifdef USE_END
        if (mctopo3d_nbvoiso26(S, i, rs, ps, N) == 1)  SET_CURVE(S[i]);
#else
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (top > 1) 
	{ 
	  SET_CURVE(S[i]);
	  if (step <= n_earlysteps) SET_EARLYCURVE(I[i]);
	}
#endif
      }
    }

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !IS_INHIBIT(I[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // DEMARQUE PTS DE COURBE ET LES MEMORISE DANS I
    for (i = 0; i < N; i++)
    { 
      if (IS_CURVE(S[i])) { UNSET_SIMPLE(S[i]); SET_INHIBIT(I[i]); }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (seq_asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (seq_asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (seq_asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_SELECTED(S[i]))
	T[i] = 1;

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) 
    if (S[i]) 
    {
      if (IS_EARLYCURVE(I[i]))
	S[i] = NDG_EARLY;
      else
	S[i] = NDG_MAX;
    }
  for (i = 0; i < N; i++) if (I[i]) I[i] = NDG_MAX;

  if (allocinhib) freeimage(inhibit); 
  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelACK3a_old() */
#else
/* ==================================== */
int32_t lskelACK3a_old(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t n_earlysteps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette asym�trique curviligne
Algo ACK3a donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de courbe de S
  I := I \cup C
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Les points de courbe d�tect�s dans les n_earlysteps premi�res �tapes
sont marqu�s dans l'image de sortie par une valeur 127 (au lieu de 255)

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelACK3a_old"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab, allocinhib = 0;
  int32_t top, topb;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: n_steps = %d ; n_earlysteps = %d\n", F_NAME, n_steps, n_earlysteps);
#endif

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
    allocinhib = 1;
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS DE COURBE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]))
      {    
//#ifdef USE_NKP_END
//	if (NKP_end(S, i, rs, ps, N)) SET_CURVE(S[i]);
#ifdef USE_END
        if (mctopo3d_nbvoiso26(S, i, rs, ps, N) == 1)  SET_CURVE(S[i]);
#else
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (top > 1) 
	{ 
	  SET_CURVE(S[i]);
	  if (step <= n_earlysteps) SET_EARLYCURVE(I[i]);
	}
#endif
      }
    }

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !IS_INHIBIT(I[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // DEMARQUE PTS DE COURBE ET LES MEMORISE DANS I
    for (i = 0; i < N; i++)
    { 
      if (IS_CURVE(S[i])) { UNSET_SIMPLE(S[i]); SET_INHIBIT(I[i]); }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) 
    if (S[i]) 
    {
      if (IS_EARLYCURVE(I[i]))
	S[i] = NDG_EARLY;
      else
	S[i] = NDG_MAX;
    }
  for (i = 0; i < N; i++) if (I[i]) I[i] = NDG_MAX;

  if (allocinhib) freeimage(inhibit); 
  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelACK3a_old() */
#endif

/* ==================================== */
int32_t lskelACK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette asym�trique curviligne, variante
Les points "candidats" � devenir des points de courbes sont les
points qui ne sont pas voisins d'un point isthme 2D ni d'un point interieur
Algo ACK3 donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de courbe de S
  I := I \cup C
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelACK3"
{ 
  index_t i, j, k;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // MARQUE LES POINTS DE SURFACE (2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE SURFACE (3) ET LES POINTS INTERIEURS
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (topb > 1) SET_SURF(S[i]);
	if (topb == 0) SET_SELECTED(S[i]);
      }
    }

    // DEMARQUE PTS ET REND "NON-SIMPLES" LES CANDIDATS
    for (i = 0; i < N; i++)
    { 
      UNSET_DCRUCIAL(S[i]);
      if (IS_OBJECT(S[i])) 
      {
	for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {
	  j = voisin26(i, k, rs, ps, N);
          if ((j != -1) && IS_SELECTED(S[j])) break;
	}
	if (k == 26) // le voxel est r�siduel
	{
	  for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
          {
	    j = voisin26(i, k, rs, ps, N);
	    if ((j != -1) && IS_SURF(S[j]))break;
	  }
	  if (k == 26) UNSET_SIMPLE(S[i]);
	}
      }
    }

    for (i = 0; i < N; i++)  UNSET_SELECTED(S[i]);

    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelACK3() */

// ===================================================================
// ===================================================================
// SQUELETTES SURFACIQUES
// ===================================================================
// ===================================================================

/* ==================================== */
int32_t lskelRK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique surfacique (par pr�servation des points r�siduels)
Les points r�siduels sont les points qui ne sont pas voisins d'un point interieur
Algo RK3 donn�es: S
R�p�ter jusqu'� stabilit�
  C := points r�siduels de S
  P := voxels simples pour S et pas dans C
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelRK3"
{ 
  index_t i, j, k;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MARQUE LES POINTS INTERIEURS
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (topb == 0) SET_SELECTED(S[i]);
      }
    }

    // DEMARQUE PTS ET REND "NON-SIMPLES" LES POINTS RESIDUELS
    for (i = 0; i < N; i++)
    { 
      if (IS_OBJECT(S[i])) 
      {
        for (k = 0; k < 12; k += 2)        /* parcourt les voisins en 6-connexite */
        {
          j = voisin6(i, k, rs, ps, N);
          if ((j != -1) && IS_SELECTED(S[j])) break;
	}
	if (k == 12) // le voxel est r�siduel
	  UNSET_SIMPLE(S[i]);
      }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelRK3() */

/* ==================================== */
int32_t lskelRK3_26(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique surfacique (par pr�servation des points r�siduels)
Les points r�siduels sont les points qui ne sont pas voisins d'un point interieur
Algo RK3 donn�es: S
R�p�ter jusqu'� stabilit�
  C := points r�siduels de S
  P := voxels simples pour S et pas dans C
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelRK3_26"
{ 
  index_t i, j, k;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MARQUE LES POINTS INTERIEURS
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (topb == 0) SET_SELECTED(S[i]);
      }
    }

    // DEMARQUE PTS ET REND "NON-SIMPLES" LES POINTS RESIDUELS
    for (i = 0; i < N; i++)
    { 
      if (IS_OBJECT(S[i])) 
      {
	for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {
	  j = voisin26(i, k, rs, ps, N);
          if ((j != -1) && IS_SELECTED(S[j])) break;
	}
	if (k == 26) // le voxel est r�siduel
	  UNSET_SIMPLE(S[i]);
      }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelRK3_26() */

/* ==================================== */
int32_t lskelSK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique surfacique bas� sur les isthmes 2D
Les points "candidats" � devenir des points de surface sont les isthmes 2D
Algo SK3 donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de surface de S
  P := voxels simples pour S et pas dans C
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelSK3"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // MARQUE LES POINTS DE SURFACE (2)
    for (i = 0; i < N; i++)
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE SURFACE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
#ifdef NEW_ISTHMUS
	if ((topb == 2) && (top == 1)) SET_SURF(S[i]);
#else
	if (topb > 1) SET_SURF(S[i]);
#endif
      }
    }

    // DEMARQUE PTS, STOCKE ET REND "NON-SIMPLES" LES POINTS D'ANCRAGE
    for (i = 0; i < N; i++)
    { 
      UNSET_DCRUCIAL(S[i]);
      if (IS_SURF(S[i])) I[i] = 1; 
      if (I[i]) UNSET_SIMPLE(S[i]);
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelSK3() */

/* ==================================== */
int32_t lskelSCK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique surfacique bas� sur les isthmes 2D et 1D
Les points "candidats" � devenir des points de surface sont les isthmes 2D et 1D
Algo SCK3 donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de surface ou de courbe de S
  P := voxels simples pour S et pas dans C
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelSCK3"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // MARQUE LES POINTS DE SURFACE (2)
    for (i = 0; i < N; i++)
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE COURBE (1)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE COURBE ET DE SURFACE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
#ifdef NEW_ISTHMUS
	if ((top == 2) && (topb == 1)) SET_CURVE(S[i]);
	if ((topb == 2) && (top == 1)) SET_SURF(S[i]);
#else
	if (top > 1) SET_CURVE(S[i]);
	if (topb > 1) SET_SURF(S[i]);
#endif
      }
    }

    // DEMARQUE PTS, STOCKE ET REND "NON-SIMPLES" LES POINTS D'ANCRAGE
    for (i = 0; i < N; i++)
    { 
      UNSET_DCRUCIAL(S[i]);
      if (IS_SURF(S[i])) I[i] = 1; 
      if (IS_CURVE(S[i])) I[i] = 1; 
      if (I[i]) UNSET_SIMPLE(S[i]);
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelSCK3() */

/* ==================================== */
int32_t lskelSK3a(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique surfacique et curviligne bas� sur les isthmes 2D et 1D
Algo SK3a donn�es: S
R�p�ter jusqu'� stabilit�
  C := points de surface ou de courbe de S
  P := voxels simples pour S et pas dans C
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelSK3a"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !I[i] && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // MARQUE LES POINTS DE COURBE (1)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE COURBE OU SURFACE (2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE COURBE OU SURFACE(3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
#ifdef NEW_ISTHMUS
	if ((top == 2) && (topb == 1)) SET_CURVE(S[i]);
	if ((topb == 2) && (top == 1)) SET_SURF(S[i]);
#else
	if (top > 1) SET_CURVE(S[i]);
	if (topb > 1) SET_SURF(S[i]);
#endif
      }
    }

    // DEMARQUE PTS, STOCKE ET REND "NON-SIMPLES" LES POINTS D'ANCRAGE
    for (i = 0; i < N; i++)
    { 
      UNSET_DCRUCIAL(S[i]);
      if ((IS_SURF(S[i]))||(IS_CURVE(S[i]))) I[i] = 1; 
      if (I[i]) UNSET_SIMPLE(S[i]);
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    memset(T, 0, N);
    for (i = 0; i < N; i++) // T := [S \ P] \cup M, o� M repr�sente les pts marqu�s
      if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	T[i] = 1;
#ifdef DEBUG
writeimage(t,"_T");
#endif

    for (i = 0; i < N; i++)
      if (S[i] && !T[i]) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelSK3a() */

// ===================================================================
// ===================================================================
// SQUELETTES DIRECTIONNELS (6 sous-it�rations)
// ===================================================================
// ===================================================================


/* ==================================== */
static int32_t direction(
  uint8_t *img,          /* pointeur base image */
  index_t p,             /* index du point */
  int32_t dir,           /* indice direction */
  index_t rs,            /* taille rangee */
  index_t ps,            /* taille plan */
  index_t N              /* taille image */
)    
/* 
  retourne 1 si p a un voisin nul dans la direction dir, 0 sinon :

#ifdef DIRTOURNE
                .       .       .       
                .       2       .       
                .       .       .       

		.	1	.			
		0       x	3
                .       4       .       

                .       .       .       
                .       5       .       
                .       .       .       
#else
                .       .       .       
                .       4       .       
                .       .       .       

		.	2	.			
		0       x	1
                .       3       .       

                .       .       .       
                .       5       .       
                .       .       .       
#endif
  le point p ne doit pas �tre un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "direction"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p%ps<rs) || (p%rs==0) || (p%ps>=ps-rs) || 
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }

  switch (dir)
  {
#ifdef DIRTOURNE
  case 0: if (*(ptr-1)) return 0; else return 1;
  case 1: if (*(ptr-rs)) return 0; else return 1;
  case 2: if (*(ptr-ps)) return 0; else return 1;

  case 3: if (*(ptr+1)) return 0; else return 1;
  case 4: if (*(ptr+rs)) return 0; else return 1;
  case 5: if (*(ptr+ps)) return 0; else return 1;
#else
  case 0: if (*(ptr-1)) return 0; else return 1;
  case 1: if (*(ptr+1)) return 0; else return 1;

  case 2: if (*(ptr-rs)) return 0; else return 1;
  case 3: if (*(ptr+rs)) return 0; else return 1;

  case 4: if (*(ptr-ps)) return 0; else return 1;
  case 5: if (*(ptr+ps)) return 0; else return 1;
#endif
  default:
    printf("%s: ERREUR: bad dir = %d\n", F_NAME, dir);
    exit(0);
  } // switch (dir)
} /* direction() */

/* ==================================== */
int32_t lskelDK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette directionnel ultime avec ensemble de contrainte
Algo DK3 donn�es: S, I
R�p�ter jusqu'� stabilit�
  Pour Dir dans {0..5}
    P := voxels simples pour S, de direction Dir et non dans I
    C2 := voxels 2-D-cruciaux (match2)
    C1 := voxels 1-D-cruciaux (match1)
    C0 := voxels 0-D-cruciaux (match0)
    P := P  \  [C2 \cup C1 \cup C0]
    S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelDK3"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I = NULL;
  int32_t step, nonstab, d;
  uint8_t v[27];

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  if (inhibit != NULL) I = UCHARDATA(inhibit);

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("DK3 step %d\n", step);
#endif

    for (d = 0; d < 6; d++)
    {

      // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES DE DIRECTION d ET PAS DANS I
      for (i = 0; i < N; i++) 
	if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N) && 
	    direction(S, i, d, rs, ps, N) && (!I || !I[i]))
	  SET_SIMPLE(S[i]);

      // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS 2-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match2(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      // TROISIEME SOUS-ITERATION : MARQUE LES POINTS 1-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match1(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      // QUATRIEME SOUS-ITERATION : MARQUE LES POINTS 0-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match0(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      memset(T, 0, N);
      for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
	if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	  T[i] = 1;

      for (i = 0; i < N; i++)
	if (S[i] && !T[i]) 
	{
	  S[i] = 0; 
	  nonstab = 1; 
	}
      for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelDK3() */


/* ==================================== */
int32_t lskelDRK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette directionnel ultime avec ensemble de contrainte
Algo DRK3 donn�es: S, I
R�p�ter jusqu'� stabilit�
  Pour Dir dans {0..5}
    R := points r�siduels de S ; I := I \cup R
    P := voxels simples pour S, de direction Dir et non dans I
    C2 := voxels 2-D-cruciaux (match2)
    C1 := voxels 1-D-cruciaux (match1)
    C0 := voxels 0-D-cruciaux (match0)
    P := P  \  [C2 \cup C1 \cup C0]
    S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelDRK3"
{ 
  int32_t i, d, j, k;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I = NULL;
  int32_t step, nonstab;
  uint8_t v[27];
  int32_t top, topb;

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  if (inhibit != NULL) I = UCHARDATA(inhibit);

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("DRK3 step %d\n", step);
#endif

    for (d = 0; d < 6; d++)
    {

      // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES DE DIRECTION d ET PAS DANS I
      for (i = 0; i < N; i++) 
	if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N) && 
	    direction(S, i, d, rs, ps, N) && (!I || !I[i]))
	  SET_SIMPLE(S[i]);

      // MARQUE LES POINTS INTERIEURS
      for (i = 0; i < N; i++)
      {
	if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
	{    
	  mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	  if (topb == 0) SET_SELECTED(S[i]);
	}
      }

      // DEMARQUE PTS ET REND "NON-SIMPLES" LES POINTS RESIDUELS
      for (i = 0; i < N; i++)
      { 
	if (IS_OBJECT(S[i])) 
	{
#ifdef RESIDUEL6
	  for (k = 0; k < 12; k += 2)        /* parcourt les voisins en 6-connexite */
	  {
	    j = voisin6(i, k, rs, ps, N);
	    if ((j != -1) && IS_SELECTED(S[j])) break;
	  }
	  if (k == 12) // le voxel est r�siduel
	    UNSET_SIMPLE(S[i]);
#else
	  for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
          {
	    j = voisin26(i, k, rs, ps, N);
	    if ((j != -1) && IS_SELECTED(S[j])) break;
	  }
	  if (k == 26) // le voxel est r�siduel
	    UNSET_SIMPLE(S[i]);
#endif
	}
      }

      // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS 2-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match2(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      // TROISIEME SOUS-ITERATION : MARQUE LES POINTS 1-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match1(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      // QUATRIEME SOUS-ITERATION : MARQUE LES POINTS 0-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match0(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      memset(T, 0, N);
      for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
	if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	  T[i] = 1;

      for (i = 0; i < N; i++)
	if (S[i] && !T[i]) 
	{
	  S[i] = 0; 
	  nonstab = 1; 
	}
      for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelDRK3() */


/* ==================================== */
int32_t lskelDSK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette directionnel surfacique bas� sur les isthmes 2D
Algo DSK3 donn�es: S, I
R�p�ter jusqu'� stabilit�
  Pour Dir dans {0..5}
    C := points de surface de S ; I := I \cup C
    P := voxels simples pour S, de direction Dir et non dans I
    C2 := voxels 2-D-cruciaux (match2)
    C1 := voxels 1-D-cruciaux (match1)
    C0 := voxels 0-D-cruciaux (match0)
    P := P  \  [C2 \cup C1 \cup C0]
    S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelDSK3"
{ 
  int32_t i, d;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I = NULL;
  int32_t step, nonstab;
  uint8_t v[27];
  int32_t top, topb;

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("DSK3 step %d\n", step);
#endif

    for (d = 0; d < 6; d++)
    {

      // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES DE DIRECTION d ET PAS DANS I
      for (i = 0; i < N; i++) 
	if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N) && 
	    direction(S, i, d, rs, ps, N) && (!I || !I[i]))
	  SET_SIMPLE(S[i]);

      // MARQUE LES ISTHMES 2D
      for (i = 0; i < N; i++)
      {
	if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
	{    
	  mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	  if (topb > 1) { I[i] = 1; UNSET_SIMPLE(S[i]); }
	}
      }

      // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS 2-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match2(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      // TROISIEME SOUS-ITERATION : MARQUE LES POINTS 1-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match1(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      // QUATRIEME SOUS-ITERATION : MARQUE LES POINTS 0-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match0(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      memset(T, 0, N);
      for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
	if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	  T[i] = 1;

      for (i = 0; i < N; i++)
	if (S[i] && !T[i]) 
	{
	  S[i] = 0; 
	  nonstab = 1; 
	}
      for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelDSK3() */

/* ==================================== */
int32_t lskelDSCK3(struct xvimage *image, 
	     int32_t n_steps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette directionnel surfacique bas� sur les isthmes 2D et 1D
Algo DSCK3 donn�es: S, I
R�p�ter jusqu'� stabilit�
  Pour Dir dans {0..5}
    C := points de surface ou de courbe de S ; I := I \cup C
    P := voxels simples pour S, de direction Dir et non dans I
    C2 := voxels 2-D-cruciaux (match2)
    C1 := voxels 1-D-cruciaux (match1)
    C0 := voxels 0-D-cruciaux (match0)
    P := P  \  [C2 \cup C1 \cup C0]
    S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelDSCK3"
{ 
  int32_t i, d;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image); 
  uint8_t *T = UCHARDATA(t);
  uint8_t *I = NULL;
  int32_t step, nonstab;
  uint8_t v[27];
  int32_t top, topb;

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
  }
  I = UCHARDATA(inhibit);

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("DSCK3 step %d\n", step);
#endif

    for (d = 0; d < 6; d++)
    {

      // PREMIERE SOUS-ITERATION : MARQUE LES POINTS SIMPLES DE DIRECTION d ET PAS DANS I
      for (i = 0; i < N; i++) 
	if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N) && 
	    direction(S, i, d, rs, ps, N) && (!I || !I[i]))
	  SET_SIMPLE(S[i]);

      // MARQUE LES ISTHMES 2D et 1D
      for (i = 0; i < N; i++)
      {
	if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
	{    
	  mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	  if ((topb > 1) || (top > 1)) { I[i] = 1; UNSET_SIMPLE(S[i]); }
	}
      }

      // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS 2-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match2(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      // TROISIEME SOUS-ITERATION : MARQUE LES POINTS 1-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match1(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      // QUATRIEME SOUS-ITERATION : MARQUE LES POINTS 0-D-CRUCIAUX
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(S[i]))
	{ 
	  extract_vois(S, i, rs, ps, N, v);
	  if (match0(v))
	    insert_vois(v, S, i, rs, ps, N);
	}

      memset(T, 0, N);
      for (i = 0; i < N; i++) // T := [S \ P] \cup  R, o� R repr�sente les pts marqu�s
	if ((S[i] && !IS_SIMPLE(S[i])) || IS_DCRUCIAL(S[i]))
	  T[i] = 1;

      for (i = 0; i < N; i++)
	if (S[i] && !T[i]) 
	{
	  S[i] = 0; 
	  nonstab = 1; 
	}
      for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(t);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelDSCK3() */

// =======================================================================
// =======================================================================
//
// Functions for detecting curves and surfaces in a symmetrical skeleton
//
// =======================================================================
// =======================================================================

/* ==================================== */
int32_t lskel1Disthmuspoints(struct xvimage *image)
/* ==================================== */
/*
  Detects 1D isthmuses in image
*/
#undef F_NAME
#define F_NAME "lskel1Disthmuspoints"
{ 
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  int32_t top, topb;
  uint8_t v[27];

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  mctopo3d_init_topo3d();

  // MARQUE LES POINTS SIMPLES
  for (i = 0; i < N; i++) 
    if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
  // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (2)
  for (i = 0; i < N; i++) 
    if (IS_SIMPLE(S[i]))
    { 
      extract_vois(S, i, rs, ps, N, v);
      if (match2s(v))
	insert_vois(v, S, i, rs, ps, N);
    }
  // TROISIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (1)
  for (i = 0; i < N; i++) 
    if (IS_SIMPLE(S[i]))
    { 
      extract_vois(S, i, rs, ps, N, v);
      if (match1s(v))
	insert_vois(v, S, i, rs, ps, N);
    }
  // MARQUE LES POINTS DE COURBE (3)
  for (i = 0; i < N; i++)
  {
    if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
    {    
      mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
      if (top > 1) SET_CURVE(S[i]);
    }
  }
  // RETOURNE PTS DE COURBE DANS S
  for (i = 0; i < N; i++)
    if (IS_CURVE(S[i])) S[i] = NDG_MAX; else S[i] = 0;

  mctopo3d_termine_topo3d();
  return(1);
} /* lskel1Disthmuspoints() */

// =======================================================================
// =======================================================================
//
// Algorithms with persistence
//
// =======================================================================
// =======================================================================

/* ==================================== */
int32_t lskelACK3p(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t isthmus_persistence,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette asym�trique curviligne
Algo ACK3p donn�es: S (image), I (inhibit), n (n_steps), p (isthmus_persistence)
Pour tout x de S faire T[x] := PERS_INIT_VAL
Pour i := 0; i < n; i++
  C := points de courbe de S
  Pour tout x de C tq T[x] == PERS_INIT_VAL faire T[x] := i
  I := I \cup {x | T[x] > PERS_INIT_VAL et (i - T[x]) >= p}
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelACK3p"
{ 
  index_t i; // index de pixel
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  int16_t *T;
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: n_steps = %d ; isthmus_persistence = %d\n", F_NAME, n_steps, isthmus_persistence);
#endif

  assert(n_steps <= INT16_MAX);
  if (n_steps == -1) n_steps = INT16_MAX;
  if (isthmus_persistence == -1) isthmus_persistence = INT16_MAX;

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  T = (int16_t *)malloc(N * sizeof(int16_t)); assert(T != NULL);
  for (i = 0; i < N; i++) T[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS DE COURBE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if ((top > 1) && (T[i] == PERS_INIT_VAL))
	  T[i] = (int16_t)step;
      }
    }

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !IS_INHIBIT(I[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // MEMORISE DANS I LES ISTHMES PERSISTANTS
    for (i = 0; i < N; i++)
    { 
      if ((T[i] > PERS_INIT_VAL) && ((step - T[i]) >= isthmus_persistence)) 
      { 
	UNSET_SIMPLE(S[i]); 
	SET_INHIBIT(I[i]); 
      }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  } // while (nonstab && (step < n_steps))

  for (i = 0; i < N; i++) if (S[i]) S[i] = NDG_MAX;

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  free(T);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelACK3p() */

/* ==================================== */
int32_t lskelACK3c(
		   struct xvimage *image, 
		   struct xvimage *persistence)
/* ==================================== */
/*
Squelette asym�trique curviligne - fonction persistance
Algo ACK3c donn�es: S (image) r�sultat: P (persistance)
Pour tout x de S faire P[x] := PERS_INIT_VAL
i := 0
R�p�ter jusqu'� stabilit�
  i := i + 1
  C := points de courbe de S
  Pour tout x de C tq P[x] == PERS_INIT_VAL faire P[x] := i // date de naissance
  D := voxels simples pour S
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  D := D  \  [C2 \cup C1 \cup C0]
  Pour tout x de D tq P[x] != PERS_INIT_VAL faire P[x] := i - P[x] // date de mort - date de naissance
  S := S \ D
Pour tout x de S faire P[x] := INFINITY

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelACK3c"
{ 
  index_t i; // index de pixel
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  float *P = FLOATDATA(persistence);   /* r�sultat */
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  COMPARE_SIZE(image, persistence);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(persistence, VFF_TYP_FLOAT);

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  for (i = 0; i < N; i++) P[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab)
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // ENREGISTRE LA DATE DE NAISSANCE DES POINTS DE COURBE
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if ((top > 1) && (P[i] == PERS_INIT_VAL))
	  P[i] = (float)step;
      }
    }

    // MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
	if (P[i] != PERS_INIT_VAL) P[i] = (float)step - P[i];
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  } // while (nonstab)

  for (i = 0; i < N; i++) if (S[i]) { S[i] = NDG_MAX; P[i] = MAXFLOAT; }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  mctopo3d_termine_topo3d();
  return(1);
} /* lskelACK3c() */

/* ==================================== */
int32_t lskelASK3p(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t isthmus_persistence,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette asym�trique surfacique
Algo ASK3a donn�es: S (image), I (inhibit), n (n_steps), p (isthmus_persistence)
Pour tout x de S faire T[x] := PERS_INIT_VAL
Pour i := 0; i < n; i++
  C := points de surface de S
  Pour tout x de C tq T[x] == PERS_INIT_VAL faire T[x] := i
  I := I \cup {x | T[x] > PERS_INIT_VAL et (i - T[x]) >= p}
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelASK3p"
{ 
  index_t i; // index de pixel
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  int16_t *T;
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: n_steps = %d ; isthmus_persistence = %d\n", F_NAME, n_steps, isthmus_persistence);
#endif

  assert(n_steps <= INT16_MAX);
  if (n_steps == -1) n_steps = INT16_MAX;
  if (isthmus_persistence == -1) isthmus_persistence = INT16_MAX;

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  T = (int16_t *)malloc(N * sizeof(int16_t)); assert(T != NULL);
  for (i = 0; i < N; i++) T[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS DE SURFACE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if ((topb > 1) && (T[i] == PERS_INIT_VAL))
	  T[i] = (int16_t)step;
      }
    }

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !IS_INHIBIT(I[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MEMORISE DANS I LES ISTHMES PERSISTANTS
    for (i = 0; i < N; i++)
    { 
      if ((T[i] > PERS_INIT_VAL) && ((step - T[i]) >= isthmus_persistence)) 
      { 
	UNSET_SIMPLE(S[i]); 
	SET_INHIBIT(I[i]); 
      }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  } // while (nonstab && (step < n_steps))

  for (i = 0; i < N; i++) if (S[i]) S[i] = NDG_MAX;

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  free(T);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelASK3p() */

/* ==================================== */
int32_t lskelCK3p(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t isthmus_persistence,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique curviligne
Algo CK3p donn�es: S (image), I (inhibit), n (n_steps), p (isthmus_persistence)
Pour tout x de S faire T[x] := PERS_INIT_VAL
i := 0 
Repeat
  i := i + 1
  C := points de courbe de S
  Pour tout x de C tq T[x] == PERS_INIT_VAL faire T[x] := i
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  Z := {x non simples ou cruciaux | T[x] > PERS_INIT_VAL et (i + 1 - T[x]) >= p}
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P
  I := I \cup Z 
Until stability or i = n

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelCK3p"
{ 
  index_t i; // index de pixel
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  int16_t *T;
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];
#ifdef DEBUG_SKEL_CK3P
  char buf[128];
#endif

#ifdef VERBOSE
  printf("%s: n_steps = %d ; isthmus_persistence = %d\n", F_NAME, n_steps, isthmus_persistence);
#endif

  assert(n_steps <= INT16_MAX);
  if (n_steps == -1) n_steps = INT16_MAX;
  if (isthmus_persistence == -1) isthmus_persistence = INT16_MAX;

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  T = (int16_t *)malloc(N * sizeof(int16_t)); assert(T != NULL);
  for (i = 0; i < N; i++) T[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // TROISIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (1)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // MARQUE LES POINTS DE COURBE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (top > 1) SET_CURVE(S[i]);
      }
      if (IS_CURVE(S[i]) && (T[i] == PERS_INIT_VAL)) 
      { 
	T[i] = (int16_t)step;
#ifdef DEBUG_SKEL_CK3P
	printf("point %d (%d %d %d) naissance step %d\n", i, i % rs, (i % ps) / rs,  i / ps, step);
#endif	
      }
    }

    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // MEMORISE DANS I LES ISTHMES PERSISTANTS
    for (i = 0; i < N; i++)
    { 
      if (IS_OBJECT(S[i]) && (!IS_SIMPLE(S[i]) || IS_DCRUCIAL(S[i])) &&
	  (T[i] > PERS_INIT_VAL) && ((step+1 - T[i]) >= isthmus_persistence)) 
      {
	SET_INHIBIT(I[i]); 
#ifdef DEBUG_SKEL_CK3P
	printf("point %d (%d %d %d) ajout � K\n", i, i % rs, (i % ps) / rs,  i / ps);
#endif	
      }
      if (IS_INHIBIT(I[i])) UNSET_SIMPLE(S[i]);
    }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
#ifdef DEBUG_SKEL_CK3P
sprintf(buf, "_S%d", step);
writeimage(image, buf);
#endif
  } // while (nonstab && (step < n_steps))

  for (i = 0; i < N; i++) if (S[i]) S[i] = NDG_MAX;

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  free(T);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelCK3p() */

/* ==================================== */
int32_t lskelSK3p(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t isthmus_persistence,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique surfacique
Algo SK3p donn�es: S (image), I (inhibit), n (n_steps), p (isthmus_persistence)
Pour tout x de S faire T[x] := PERS_INIT_VAL
Pour i := 0; i < n; i++
  C := points de surface de S
  Pour tout x de C tq T[x] == PERS_INIT_VAL faire T[x] := i
  I := I \cup {x | T[x] > PERS_INIT_VAL et (i - T[x]) >= p}
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelSK3p"
{  
  index_t i; // index de pixel
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  int16_t *T;
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: n_steps = %d ; isthmus_persistence = %d\n", F_NAME, n_steps, isthmus_persistence);
#endif

  assert(n_steps <= INT16_MAX);
  if (n_steps == -1) n_steps = INT16_MAX;
  if (isthmus_persistence == -1) isthmus_persistence = INT16_MAX;

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  T = (int16_t *)malloc(N * sizeof(int16_t)); assert(T != NULL);
  for (i = 0; i < N; i++) T[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MARQUE LES POINTS DE SURFACE (2)
    for (i = 0; i < N; i++)
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE SURFACE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (topb > 1) SET_SURF(S[i]);
      }
      if (IS_SURF(S[i]) && (T[i] == PERS_INIT_VAL)) T[i] = (int16_t)step;
    }

    // MEMORISE DANS I LES ISTHMES PERSISTANTS
    for (i = 0; i < N; i++)
    { 
      if ((T[i] > PERS_INIT_VAL) && ((step + 1 - T[i]) >= isthmus_persistence)) 
	SET_INHIBIT(I[i]); 
      if (IS_INHIBIT(I[i])) UNSET_SIMPLE(S[i]);
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  } // while (nonstab && (step < n_steps))

  for (i = 0; i < N; i++) if (S[i]) S[i] = NDG_MAX;

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  free(T);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelSK3p() */

/* ==================================== */
int32_t lskelSCK3p(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t isthmus_persistence,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette sym�trique surfacique-curviligne
Algo SCK3p donn�es: S (image), I (inhibit), n (n_steps), p (isthmus_persistence)
Pour tout x de S faire T[x] := PERS_INIT_VAL
Pour i := 0; i < n; i++
  C := points de surface ou de courbe de S
  Pour tout x de C tq T[x] == PERS_INIT_VAL faire T[x] := i
  I := I \cup {x | T[x] > PERS_INIT_VAL et (i - T[x]) >= p}
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelSCK3p"
{ 
  index_t i; // index de pixel
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  int16_t *T;
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: n_steps = %d ; isthmus_persistence = %d\n", F_NAME, n_steps, isthmus_persistence);
#endif

  assert(n_steps <= INT16_MAX);
  if (n_steps == -1) n_steps = INT16_MAX;
  if (isthmus_persistence == -1) isthmus_persistence = INT16_MAX;

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  T = (int16_t *)malloc(N * sizeof(int16_t)); assert(T != NULL);
  for (i = 0; i < N; i++) T[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MARQUE LES POINTS DE COURBE OU DE SURFACE(2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // MARQUE LES POINTS DE COURBE (1)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // MARQUE LES POINTS DE COURBE OU DE SURFACE(3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (top > 1) SET_CURVE(S[i]);
	if (topb > 1) SET_SURF(S[i]);
      }
      if ((IS_CURVE(S[i]) || IS_SURF(S[i])) && (T[i] == PERS_INIT_VAL)) T[i] = (int16_t)step;
    }

    // MEMORISE DANS I LES ISTHMES PERSISTANTS
    for (i = 0; i < N; i++)
    { 
      if ((T[i] > PERS_INIT_VAL) && ((step - T[i]) >= isthmus_persistence)) 
	SET_INHIBIT(I[i]); 
      if (IS_INHIBIT(I[i])) UNSET_SIMPLE(S[i]);
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  } // while (nonstab && (step < n_steps))

  for (i = 0; i < N; i++) if (S[i]) S[i] = NDG_MAX;

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  free(T);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelSCK3p() */

/* ==================================== */
int32_t lskelASCK3p(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t isthmus_persistence,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette asym�trique surfacique-curviligne
Algo ASCK3p donn�es: S (image), I (inhibit), n (n_steps), p (isthmus_persistence)
Pour tout x de S faire T[x] := PERS_INIT_VAL
Pour i := 0; i < n; i++
  C := points de surface ou de courbe de S
  Pour tout x de C tq T[x] == PERS_INIT_VAL faire T[x] := i
  I := I \cup {x | T[x] > PERS_INIT_VAL et (i - T[x]) >= p}
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (asym_match2)
  C1 := voxels 1-D-cruciaux (asym_match1)
  C0 := voxels 0-D-cruciaux (asym_match0)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelASCK3p"
{ 
  index_t i; // index de pixel
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  int16_t *T;
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

#ifdef VERBOSE
  printf("%s: n_steps = %d ; isthmus_persistence = %d\n", F_NAME, n_steps, isthmus_persistence);
#endif

  assert(n_steps <= INT16_MAX);
  if (n_steps == -1) n_steps = INT16_MAX;
  if (isthmus_persistence == -1) isthmus_persistence = INT16_MAX;

  if (inhibit == NULL) 
  {
    inhibit = copyimage(image); 
    razimage(inhibit);
    I = UCHARDATA(inhibit);
  }
  else
  {
    I = UCHARDATA(inhibit);
    for (i = 0; i < N; i++) if (I[i]) I[i] = I_INHIBIT;
  }

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  T = (int16_t *)malloc(N * sizeof(int16_t)); assert(T != NULL);
  for (i = 0; i < N; i++) T[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < n_steps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS DE COURBE OU DE SURFACE(3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (top > 1) SET_CURVE(S[i]);
	if (topb > 1) SET_SURF(S[i]);
      }
      if ((IS_CURVE(S[i]) || IS_SURF(S[i])) && (T[i] == PERS_INIT_VAL)) T[i] = (int16_t)step;
    }

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && !IS_INHIBIT(I[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MEMORISE DANS I LES ISTHMES PERSISTANTS
    for (i = 0; i < N; i++)
    { 
      if ((T[i] > PERS_INIT_VAL) && ((step - T[i]) >= isthmus_persistence)) 
      { 
	UNSET_SIMPLE(S[i]); 
	SET_INHIBIT(I[i]); 
      }
    }
    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (asym_match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    for (i = 1; i < N; i++) if (IS_SELECTED(S[i])) { UNSET_SELECTED(S[i]); SET_DCRUCIAL(S[i]); }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  } // while (nonstab && (step < n_steps))

  for (i = 0; i < N; i++) if (S[i]) S[i] = NDG_MAX;

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  free(T);
  mctopo3d_termine_topo3d();
  return(1);
} /* lskelASCK3p() */

/* ==================================== */
int32_t lskelCK3_pers(struct xvimage *image, 
		       struct xvimage *persistence)
/* ==================================== */
/*
Squelette sym�trique curviligne - fonction persistance
Algo CK3_pers donn�es: S (image) r�sultat: P (persistance)

Pour tout x de S faire P[x] := PERS_INIT_VAL
i := 0
R�p�ter jusqu'� stabilit�
  i := i + 1
  C := points de courbe de S
  Pour tout x de C tq P[x] == PERS_INIT_VAL faire P[x] := i // date de naissance
  D := voxels simples pour S
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  D := D  \  [C2 \cup C1 \cup C0]
  Pour tout x de D tq P[x] != PERS_INIT_VAL faire P[x] := i - P[x] // date de mort - date de naissance
  S := S \ D
Pour tout x de S faire P[x] := INFINITY

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelCK3_pers"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  float *P = FLOATDATA(persistence);   /* r�sultat */
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  COMPARE_SIZE(image, persistence);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(persistence, VFF_TYP_FLOAT);

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  for (i = 0; i < N; i++) P[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab)
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // TROISIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (1)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // MARQUE LES POINTS DE COURBE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (top > 1) SET_CURVE(S[i]);
      }
    }

    // ENREGISTRE LA DATE DE NAISSANCE DES POINTS DE COURBE
    for (i = 0; i < N; i++)
    {
      if ((P[i] == PERS_INIT_VAL) && IS_CURVE(S[i])) P[i] = (float)step;
    }

    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
	if (P[i] != PERS_INIT_VAL) P[i] = (float)step - P[i];
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) { S[i] = NDG_MAX; P[i] = MAXFLOAT; }

  mctopo3d_termine_topo3d();
  return(1);
} /* lskelCK3_pers() */

/* ==================================== */
int32_t lskelSK3_pers(struct xvimage *image, 
		      struct xvimage *persistence)
/* ==================================== */
/*
Squelette sym�trique surfacique - fonction persistance
Algo SK3_pers donn�es: S (image) r�sultat: P (persistance)

Pour tout x de S faire P[x] := PERS_INIT_VAL
i := 0
R�p�ter jusqu'� stabilit�
  i := i + 1
  C := points de surface de S
  Pour tout x de C tq P[x] == PERS_INIT_VAL faire P[x] := i // date de naissance
  D := voxels simples pour S
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  D := D  \  [C2 \cup C1 \cup C0]
  Pour tout x de D tq P[x] != PERS_INIT_VAL faire P[x] := i - P[x] // date de mort - date de naissance
  S := S \ D
Pour tout x de S faire P[x] := INFINITY

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelSK3_pers"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  float *P = FLOATDATA(persistence);   /* r�sultat */
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  COMPARE_SIZE(image, persistence);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(persistence, VFF_TYP_FLOAT);

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  for (i = 0; i < N; i++) P[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab)
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // MARQUE LES POINTS DE SURFACE (2)
    for (i = 0; i < N; i++)
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS DE SURFACE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (topb > 1) SET_SURF(S[i]);
      }
    }

    // ENREGISTRE LA DATE DE NAISSANCE DES POINTS DE SURFACE
    for (i = 0; i < N; i++)
    {
      if ((P[i] == PERS_INIT_VAL) && IS_SURF(S[i])) P[i] = (float)step;
    }

    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
	if (P[i] != PERS_INIT_VAL) P[i] = (float)step - P[i];
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) { S[i] = NDG_MAX; P[i] = MAXFLOAT; }

  mctopo3d_termine_topo3d();
  return(1);
} /* lskelSK3_pers() */

/* ==================================== */
int32_t lskelSCK3_pers(struct xvimage *image, 
		       struct xvimage *persistence)
/* ==================================== */
/*
Squelette sym�trique curviligne et surfacique - fonction persistance
Algo SCK3_pers donn�es: S (image) r�sultat: P (persistance)

Pour tout x de S faire P[x] := PERS_INIT_VAL
i := 0
R�p�ter jusqu'� stabilit�
  i := i + 1
  C := points de courbe ou de surface de S
  Pour tout x de C tq P[x] == PERS_INIT_VAL faire P[x] := i // date de naissance
  D := voxels simples pour S
  C2 := voxels 2-D-cruciaux (match2)
  C1 := voxels 1-D-cruciaux (match1)
  C0 := voxels 0-D-cruciaux (match0)
  D := D  \  [C2 \cup C1 \cup C0]
  Pour tout x de D tq P[x] != PERS_INIT_VAL faire P[x] := i - P[x] // date de mort - date de naissance
  S := S \ D
Pour tout x de S faire P[x] := INFINITY

Attention : l'objet ne doit pas toucher le bord de l'image

*/
#undef F_NAME
#define F_NAME "lskelSCK3_pers"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  float *P = FLOATDATA(persistence);   /* r�sultat */
  int32_t step, nonstab;
  int32_t top, topb;
  uint8_t v[27];

  COMPARE_SIZE(image, persistence);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(persistence, VFF_TYP_FLOAT);

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  for (i = 0; i < N; i++) P[i] = PERS_INIT_VAL;

  mctopo3d_init_topo3d();

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab)
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // MARQUE LES POINTS SIMPLES
    for (i = 0; i < N; i++) 
      if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
	SET_SIMPLE(S[i]);

    // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE ET DE SURFACE (2)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // TROISIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (1)
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1s(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    // MARQUE LES POINTS DE COURBE ET DE SURFACE (3)
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
      {    
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (top > 1) SET_CURVE(S[i]);
	if (topb > 1) SET_SURF(S[i]);
      }
    }

    // ENREGISTRE LA DATE DE NAISSANCE DES POINTS DE COURBE OU DE SURFACE
    for (i = 0; i < N; i++)
    {
      if ((P[i] == PERS_INIT_VAL) && (IS_CURVE(S[i]) || IS_SURF(S[i]))) 
	P[i] = (float)step;
    }

    // MARQUE LES POINTS 2-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match2(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 1-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match1(v))
	  insert_vois(v, S, i, rs, ps, N);
      }
    // MARQUE LES POINTS 0-D-CRUCIAUX
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(S[i]))
      { 
	extract_vois(S, i, rs, ps, N, v);
	if (match0(v))
	  insert_vois(v, S, i, rs, ps, N);
      }

    for (i = 0; i < N; i++)
      if (S[i] && IS_SIMPLE(S[i]) && !IS_DCRUCIAL(S[i])) 
      {
	S[i] = 0; 
	nonstab = 1; 
	if (P[i] != PERS_INIT_VAL) P[i] = (float)step - P[i];
      }
    for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) { S[i] = NDG_MAX; P[i] = MAXFLOAT; }

  mctopo3d_termine_topo3d();
  return(1);
} /* lskelSCK3_pers() */


/* ==================================== */
int32_t lptthickisthmus(struct xvimage *image)
/* ==================================== */
/*
Detecte les isthmes "�pais"
*/
#undef F_NAME
#define F_NAME "lptthickisthmus"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t top, topb;
  uint8_t v[27];

  mctopo3d_init_topo3d();

  for (i = 0; i < N; i++) if (S[i]) S[i] = S_OBJECT;

  // MARQUE LES POINTS SIMPLES
  for (i = 0; i < N; i++) 
    if (IS_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N))
      SET_SIMPLE(S[i]);

  // DEUXIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (2)
  for (i = 0; i < N; i++) 
    if (IS_SIMPLE(S[i]))
    { 
      extract_vois(S, i, rs, ps, N, v);
      if (match2s(v))
	insert_vois(v, S, i, rs, ps, N);
    }

  // TROISIEME SOUS-ITERATION : MARQUE LES POINTS DE COURBE (1)
  for (i = 0; i < N; i++) 
    if (IS_SIMPLE(S[i]))
    { 
      extract_vois(S, i, rs, ps, N, v);
      if (match1s(v))
	insert_vois(v, S, i, rs, ps, N);
    }

  // MARQUE LES POINTS DE COURBE (3)
  for (i = 0; i < N; i++)
  {
    if (IS_OBJECT(S[i]) && !IS_SIMPLE(S[i]))
    {    
      mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
#ifdef NEW_ISTHMUS
      if ((top == 2) && (topb == 1)) SET_CURVE(S[i]);
#else
      if (top > 1) SET_CURVE(S[i]);
#endif
    }
  }

  // TRANSFERE PTS DE COURBE DANS S
  for (i = 0; i < N; i++)
    if (IS_CURVE(S[i])) S[i] = NDG_MAX; else S[i] = 0; 

  mctopo3d_termine_topo3d();
  return(1);
} /* lptthickisthmus() */
