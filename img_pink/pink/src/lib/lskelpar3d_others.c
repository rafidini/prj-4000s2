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
   Algorithmes 3D de squelettisation

\li 0: Palagyi-Kuba (curvilinear, 6 directions, 1998)
\li 1: Palagyi (curvilinear, sequential, 2006)
\li 2: Palagyi (surface, parallel directional, 2002)
\li 3: Palagyi (surface, fully parallel, 2008)
\li 4: Raynal  (curvilinear, 6 directions, 2011)
\li 5: Raynal  (surface, directional, 2011)
\li 6: Lohou-Bertrand  (curvilinear, symmetric, 2007)
\li 7: Ma-Wan-Chang (curvilinear, 2 subfields, 2002)
\li 8: Tsao-Fu (curvilinear, 6 directions, 1982)
\li 9: Ma-Sonka (curvilinear, fully parallel, does not preserve topology 1996)
\li 10: Ma-Wan (curvilinear (18/6) 6 directions, CVIU 2000)
\li 11: Lohou-Bertrand (curvilinear 6 directions, DAM 2005)
\li 12: Lohou-Bertrand (curvilinear 12 directions, DAM 2004)
\li 13: ACK3a - see lskelpar3d.c
\li 14: CKSC - see lskeletons.c
\li 15: Ma-Wan-Lee (curvilinear, 4 subfields, 2002)
\li 16: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 1)
\li 17: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 2)
\li 18: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 3)
\li 19: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 1)
\li 20: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 2)
\li 21: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 3)
\li 22: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 1)
\li 23: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 2)
\li 24: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 3)
\li 25: She et al. (curvilinear, symmetric, DICTA 2009)
\li 26: Tsao-Fu (surface, 6 directions, 1981)
\li 27: Tsao-Fu (curvilinear, 6 directions, 1981)
\li 28: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 0)
\li 29: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 0)
\li 30: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 0)
\li 31: Lohou-Bertrand  (surface, symmetric, 2007)
\li 32: Manzanera et al. (surface, symmetric, 1999)
\li 33: Gong-Bertrand (curvilinear, 6 directions, 1990)
\li 34: Palagyi-Kuba (curvilinear, 12 directions, 1999)
\li 35: Bertrand-Aktouf (curvilinear, 8 subfields, 1994)
\li 36: Palagyi-Kuba (curvilinear, hybrid, 1998)
\li 37: Palagyi-Kuba (curvilinear, 8 directions, 1999)
\li 38: Saha et al. (surface, 8 subfields, 1997)
\li 39: Saha et al. (curvilinear, 8 subfields, 1997)
\li 40: Nemeth-Kardos-Palagyi (curvilinear, 6 directions, 2011)
\li 41: Nemeth-Palagyi (curvilinear, 6 directions, isthmus, 2012)
\li 42: ACK3a with ends- see lskel_ACK3a.c
\li 43: Palagyi-Kuba (curvilinear, 12 directions, isthmus, 2013)
\li 44: Nemeth-Kardos-Palagyi (curvilinear, asymmetric, fully parallel, 2012)
\li 45: Nemeth-Kardos-Palagyi (curvilinear, asymmetric, 6 directions, ends, 2012)
\li 46: Nemeth-Kardos-Palagyi (curvilinear, asymmetric, 4 subfields, ends, 2012)
\li 47: Nemeth-Palagyi (curvilinear, asymmetric, fully parallel, isthmus, 2012)
\li 48: Nemeth-Palagyi (curvilinear, asymmetric, 4 subfields, isthmus, 2012)
\li 49: Lohou-Dehos d'apres Ma-Sonka (curvilinear, fully parallel, 2010)
\li 50: Nemeth-Kardos-Palagyi (curvilinear, asymmetric, 8 subfields, isthmus, 2012)
\li 51: Nemeth-Palagyi (curvilinear, asymmetric, 8 subfields, ends, 2012)
\li 52: Palagyi (surface, symmetric, fully parallel, 2015)
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <assert.h>
#include <limits.h>

// #include <unistd.h> in Microsoft Windows it does not exist, but we only need a subset of it
#ifdef UNIXIO
#  include <unistd.h>
#else /* NOT UNIXIO */
#  include <stdlib.h>
#  include <io.h>
#endif /* NOT UNIXIO */

#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcindic.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <lskelpar3d_others.h>

//#define VERBOSE
//#define VERBOSE2
//#define DEBUG
//#define DEBUG_lsahaetal8subfieldscurv1997
//#define DEBUGSAHA
//#define DEBUG_lmasonka1996
//#define DEBUG_lmawanleecurv4subfields2002
//#define DEBUG_lnemethetalcurv2subfields2010
//#define DEBUG_lnemethetalcurv4subfields2010
//#define DEBUG_lpalagyikuba12dir1999
//#define DEBUG_lpalagyi12dir2013

int trace = 0; // DEBUG

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

  le point p ne doit pas être un point de bord de l'image
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
static index_t return_vois(
  index_t p,
  int32_t dir,
  index_t rs,                      /* taille rangee */
  index_t ps)                      /* taille plan */
/* 
  retourne l'index du point voisin q de p dans la direction dir

               12      11      10       
               13       8       9
               14      15      16

		3	2	1
		4      26	0
		5	6	7

               21      20      19
               22      17      18
               23      24      25

  le point p ne doit pas être un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "return_vois"
  switch (dir)
  {
  case 0: return p+1;
  case 1: return p+1-rs;
  case 2: return p-rs;
  case 3: return p-rs-1;
  case 4: return p-1;
  case 5: return p-1+rs;
  case 6: return p+rs;
  case 7: return p+rs+1;

  case 8: return p-ps;
  case 9: return p-ps+1;
  case 10: return p-ps+1-rs;
  case 11: return p-ps-rs;
  case 12: return p-ps-rs-1;
  case 13: return p-ps-1;
  case 14: return p-ps-1+rs;
  case 15: return p-ps+rs;
  case 16: return p-ps+rs+1;

  case 17: return p+ps;
  case 18: return p+ps+1;
  case 19: return p+ps+1-rs;
  case 20: return p+ps-rs;
  case 21: return p+ps-rs-1;
  case 22: return p+ps-1;
  case 23: return p+ps-1+rs;
  case 24: return p+ps+rs;
  case 25: return p+ps+rs+1;
  case 26: return p;
  default: 
    printf("%s: ERREUR: mauvais code dir %d\n", F_NAME, dir);
    exit(0);
  }
} /* return_vois() */

#ifdef DEBUG
/* ==================================== */
static void print_vois(uint8_t *v)
/* ==================================== */
{
  printf("%d %d %d\n", v[12], v[11], v[10]);
  printf("%d %d %d\n", v[13], v[8], v[9]);
  printf("%d %d %d\n\n", v[15], v[15], v[16]);
  printf("%d %d %d\n", v[3], v[2], v[1]);
  printf("%d %d %d\n", v[4], v[26], v[0]);
  printf("%d %d %d\n\n", v[5], v[6], v[7]);
  printf("%d %d %d\n", v[21], v[20], v[19]);
  printf("%d %d %d\n", v[22], v[17], v[18]);
  printf("%d %d %d\n\n", v[23], v[24], v[25]);
} // print_vois()
#endif

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
  le point p ne doit pas être un point de bord de l'image
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
static void isometrieXZ_vois(uint8_t *vois) 
// effectue une isométrie du voisinage "vois" par échange des axes X et Z (+ symétries)
// cette isométrie est de plus une involution
/* ==================================== */
{
  uint8_t v[27];
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
// effectue une isométrie du voisinage "vois" par échange des axes Y et Z (+ symétries)  
// cette isométrie est de plus une involution
/* ==================================== */
{
  uint8_t v[27];
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

static void rotate_90_Z(uint8_t *v)
{
  uint8_t t;
  t = v[ 9]; v[ 9] = v[11]; v[11] = v[13]; v[13] = v[15]; v[15] = t;
  t = v[10]; v[10] = v[12]; v[12] = v[14]; v[14] = v[16]; v[16] = t;
  t = v[ 0]; v[ 0] = v[ 2]; v[ 2] = v[ 4]; v[ 4] = v[ 6]; v[ 6] = t;
  t = v[ 1]; v[ 1] = v[ 3]; v[ 3] = v[ 5]; v[ 5] = v[ 7]; v[ 7] = t;
  t = v[18]; v[18] = v[20]; v[20] = v[22]; v[22] = v[24]; v[24] = t;
  t = v[19]; v[19] = v[21]; v[21] = v[23]; v[23] = v[25]; v[25] = t;
} // rotate_90_Z()

static void swap_U_L(uint8_t *v)
{
  uint8_t t;
  t = v[ 8]; v[ 8] = v[17]; v[17] = t;
  t = v[ 9]; v[ 9] = v[18]; v[18] = t;
  t = v[10]; v[10] = v[19]; v[19] = t;
  t = v[11]; v[11] = v[20]; v[20] = t;
  t = v[12]; v[12] = v[21]; v[21] = t;
  t = v[13]; v[13] = v[22]; v[22] = t;
  t = v[14]; v[14] = v[23]; v[23] = t;
  t = v[15]; v[15] = v[24]; v[24] = t;
  t = v[16]; v[16] = v[25]; v[25] = t;
} // swap_U_L()

/* ==================================== */
static void extract_vois125(
  uint8_t *img,                    /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  uint8_t *vois)
/*
  retourne dans "vois" les valeurs des 125 voisins de p, dans l'ordre suivant:

  0   1   2   3   4
  5   6   7   8   9
 10  11  12  13  14
 15  16  17  18  19
 20  21  22  23  24

 25  26  27  28  29
 30  31  32  33  34
 35  36  37  38  39
 40  41  42  43  44
 45  46  47  48  49

 50  51  52  53  54
 55  56  57  58  59
 60  61  62  63  64
 65  66  67  68  69
 70  71  72  73  74

 75  76  77  78  79
 80  81  82  83  84
 85  86  87  88  89
 90  91  92  93 ...

 le tableau vois doit avoir été alloué
 
 le point central est le 62, ses voisins sont :

  31  32  33
  36  37  38
  41  42  43

  56  57  58
  61  62  63
  66  67  68

  81  82  83
  86  87  88
  91  92  93

*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois125"
  uint32_t xx, yy, zz, i, j, k;
  if ((p%rs>=rs-2) || (p%ps<rs-1) || (p%rs<=1) || (p%ps>=ps-rs-rs) ||
      (p < ps-1) || (p >= N-ps-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }

  zz = p / ps;
  yy = (p % ps) / rs;
  xx = p % rs;
  for (k = 0; k < 5; k++)
    for (j = 0; j < 5; j++)
      for (i = 0; i < 5; i++)
      {
	vois[(k * 25) + (j * 5) + i] =
	  img[((zz-2+k) * ps) + ((yy-2+j) * rs) + xx-2+i];
      }

} /* extract_vois125() */

/* ==================================== */
static void extract_vois27(
  uint8_t *img,                    /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  uint8_t *vois)
/*
  retourne dans "vois" les valeurs des 27 voisins de p, dans l'ordre suivant:

  0   1   2   
  3   4   5
  6   7   8   

  9   10  11
  12  13  14
  15  16  17

  18  19  20
  21  22  23
  24  25  26

  le tableau vois doit avoir été alloué
  le point central est le 13
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois27"
  uint32_t xx, yy, zz, i, j, k;
  if ((p%rs>=rs-1) || (p%ps<rs) || (p%rs<=0) || (p%ps>=ps-rs) ||
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }

  zz = p / ps;
  yy = (p % ps) / rs;
  xx = p % rs;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
      {
	vois[(k * 9) + (j * 3) + i] =
	  img[((zz-1+k) * ps) + ((yy-1+j) * rs) + xx-1+i];
      }

} /* extract_vois27() */

#ifdef COMPILE_NOTUSED
/* ==================================== */
static index_t return_vois27(
  index_t p,
  int32_t dir,
  index_t rs,                      /* taille rangee */
  index_t ps)                      /* taille plan */
/* 
  retourne l'index du point voisin q de p dans la direction dir

  0   1   2   
  3   4   5
  6   7   8   

  9   10  11
  12  13  14
  15  16  17

  18  19  20
  21  22  23
  24  25  26

  le point p ne doit pas être un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "return_vois27"
  switch (dir)
  {
  case  0: return p-ps-rs-1;
  case  1: return p-ps-rs;
  case  2: return p-ps+1-rs;
  case  3: return p-ps-1;
  case  4: return p-ps;
  case  5: return p-ps+1;
  case  6: return p-ps-1+rs;
  case  7: return p-ps+rs;
  case  8: return p-ps+rs+1;

  case  9: return p-rs-1;
  case 10: return p-rs;
  case 11: return p+1-rs;
  case 12: return p-1;
  case 13: return p;
  case 14: return p+1;
  case 15: return p-1+rs;
  case 16: return p+rs;
  case 17: return p+rs+1;

  case 18: return p+ps-rs-1;
  case 19: return p+ps-rs;
  case 20: return p+ps+1-rs;
  case 21: return p+ps-1;
  case 22: return p+ps;
  case 23: return p+ps+1;
  case 24: return p+ps-1+rs;
  case 25: return p+ps+rs;
  case 26: return p+ps+rs+1;
  default: 
    printf("%s: ERREUR: mauvais code dir %d\n", F_NAME, dir);
    exit(0);
  }
} /* return_vois27() */
#endif

#ifdef DEBUG
/* ==================================== */
static void print_vois27(uint8_t *v)
/* ==================================== */
{
  int32_t i, j, k;
  for (k = 0; k < 3; k++)
  {
    for (j = 0; j < 3; j++)
    {
      for (i = 0; i < 3; i++) printf("%d ", v[(k * 9) + (j * 3) + i]);
      printf("\n");
    }
    printf("\n");
  }
  printf("\n");
} // print_vois27()

/* ==================================== */
static int equal_vois27(uint8_t *v, uint8_t *w)
/* ==================================== */
{
  int32_t i;
  for (i = 0; i < 27; i++) if (v[i] != w[i]) return 0;
  return 1;
} // equal_vois27()

/* ==================================== */
static void print_vois125(uint8_t *v)
/* ==================================== */
{
  int32_t i, j, k;
  for (k = 0; k < 5; k++)
  {
    for (j = 0; j < 5; j++)
    {
      for (i = 0; i < 5; i++) printf("%d ", v[(k * 25) + (j * 5) + i]);
      printf("\n");
    }
    printf("\n");
  }
} // print_vois125()

/* ==================================== */
static void printf_vois125(
  uint8_t *img,                    /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */
/* ==================================== */
{
  uint32_t xx, yy, zz, i, j, k;
  if ((p%rs>=rs-2) || (p%ps<rs-1) || (p%rs<=1) || (p%ps>=ps-rs-rs) ||
      (p < ps-1) || (p >= N-ps-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }

  zz = p / ps;
  yy = (p % ps) / rs;
  xx = p % rs;
  for (k = 0; k < 5; k++)
  {
    for (j = 0; j < 5; j++)
    {
      for (i = 0; i < 5; i++)
	printf("%d ", img[((zz-2+k) * ps) + ((yy-2+j) * rs) + xx-2+i]);
      printf("\n");
    }
    printf("\n");
  }
} /* printf_vois125() */
#endif

/* ==================================== */
static void isometrieXZ_vois27(uint8_t *vois) 
// effectue une isométrie du voisinage "vois" par échange des axes X et Z (+ symétries)
// cette isométrie est de plus une involution
// version pour la numérotation de extract_vois27()
/* ==================================== */
{
  uint8_t v[27];
  int32_t i, j, k;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
	v[k*9 + j*3 + i] = vois[i*9 + j*3 + k];
  for (i = 0; i < 27; i++) vois[i] = v[i];
} /* isometrieXZ_vois27() */

/* ==================================== */
static void isometrieYZ_vois27(uint8_t *vois)
// effectue une isométrie du voisinage "vois" par échange des axes Y et Z (+ symétries)  
// cette isométrie est de plus une involution
// version pour la numérotation de extract_vois27()
/* ==================================== */
{
  uint8_t v[27];
  int32_t i, j, k;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
	v[k*9 + j*3 + i] = vois[j*9 + k*3 + i];
  for (i = 0; i < 27; i++) vois[i] = v[i];
} /* isometrieYZ_vois27() */

static void rotate_90_Z_27(uint8_t *vois)
{
// version pour la numérotation de extract_vois27()
  uint8_t v[27];
  int32_t i, j, k;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
	v[k*9 + j*3 + i] = vois[k*9 + (-i+2)*3 + j];
  for (i = 0; i < 27; i++) vois[i] = v[i];
} // rotate_90_Z_27()

static void swap_U_L_27(uint8_t *vois)
{
// version pour la numérotation de extract_vois27()
  uint8_t v[27];
  int32_t i, j, k;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
	v[k*9 + j*3 + i] = vois[(-k+2)*9 + j*3 + i];
  for (i = 0; i < 27; i++) vois[i] = v[i];
} // swap_U_L_27()

static void swap_W_E_27(uint8_t *vois)
{
// version pour la numérotation de extract_vois27()
  uint8_t v[27];
  int32_t i, j, k;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
	v[k*9 + j*3 + i] = vois[k*9 + j*3 + (-i+2)];
  for (i = 0; i < 27; i++) vois[i] = v[i];
} // swap_U_L_27()

static void swap_S_N_27(uint8_t *vois)
{
// version pour la numérotation de extract_vois27()
  uint8_t v[27];
  int32_t i, j, k;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
	v[k*9 + j*3 + i] = vois[k*9 + (-j+2)*3 + i];
  for (i = 0; i < 27; i++) vois[i] = v[i];
} // swap_U_L_27()

#ifdef COMPILE_NOTUSED
/* ==================================== */
static void isometrieXZ_vois125(uint8_t *vois) 
// effectue une isométrie du voisinage "vois" par échange des axes X et Z (+ symétries)
// cette isométrie est de plus une involution
// version pour la numérotation de extract_vois125()
/* ==================================== */
{
  uint8_t v[125];
  int32_t i, j, k;
  for (k = 0; k < 5; k++)
    for (j = 0; j < 5; j++)
      for (i = 0; i < 5; i++)
	v[k*25 + j*5 + i] = vois[i*25 + j*5 + k];
  for (i = 0; i < 125; i++) vois[i] = v[i];
} /* isometrieXZ_vois125() */

/* ==================================== */
static void isometrieYZ_vois125(uint8_t *vois)
// effectue une isométrie du voisinage "vois" par échange des axes Y et Z (+ symétries)  
// cette isométrie est de plus une involution
// version pour la numérotation de extract_vois125()
/* ==================================== */
{
  uint8_t v[125];
  int32_t i, j, k;
  for (k = 0; k < 5; k++)
    for (j = 0; j < 5; j++)
      for (i = 0; i < 5; i++)
	v[k*25 + j*5 + i] = vois[j*25 + k*5 + i];
  for (i = 0; i < 125; i++) vois[i] = v[i];
} /* isometrieYZ_vois125() */

static void rotate_90_Z_125(uint8_t *vois)
{
// version pour la numérotation de extract_vois125()
  uint8_t v[125];
  int32_t i, j, k;
  for (k = 0; k < 5; k++)
    for (j = 0; j < 5; j++)
      for (i = 0; i < 5; i++)
	v[k*25 + j*5 + i] = vois[k*25 + (-i+4)*5 + j];
  for (i = 0; i < 125; i++) vois[i] = v[i];
} // rotate_90_Z_125()

static void swap_U_L_125(uint8_t *vois)
{
// version pour la numérotation de extract_vois125()
  uint8_t v[125];
  int32_t i, j, k;
  for (k = 0; k < 5; k++)
    for (j = 0; j < 5; j++)
      for (i = 0; i < 5; i++)
	v[k*25 + j*5 + i] = vois[(-k+4)*25 + j*5 + i];
  for (i = 0; i < 125; i++) vois[i] = v[i];
} // swap_U_L_125()
#endif

/* ============================================================ */
/* ============================================================ */
// Algo. de Ma & Sonka (curviligne fully parallel, CVIU 1996)
// M. Couprie, jan. 2012
/* ============================================================ */
/* ============================================================ */

#define MS_OBJECT      1
#define MS_DELETABLE   2
#define IS_MS_DELETABLE(f) (f&MS_DELETABLE)
#define SET_MS_DELETABLE(f) (f|=MS_DELETABLE)

#define __W(p) ((p)-1) 
#define __N(p) ((p)-3) 
#define __U(p) ((p)-9) 
#define __E(p) ((p)+1) 
#define __S(p) ((p)+3) 
#define __D(p) ((p)+9) 

#define _W(p) ((p)-1) 
#define _N(p) ((p)-5) 
#define _U(p) ((p)-25) 
#define _E(p) ((p)+1) 
#define _S(p) ((p)+5) 
#define _D(p) ((p)+25) 

#define MS_S  16
#define MS_W  12
#define MS_D  22
#define MS_U   4
#define MS_E  14
#define MS_N  10
#define MS_SU  7
#define MS_SD 25
#define MS_SE 17
#define MS_SW 15
#define MS_WU  3
#define MS_WD 21

#define OBJ 1
#define BGD 2
#define ANY 0

// Pour les masques
static uint8_t A[27], B[27], C[27], D[27];

/* ==================================== */
int32_t ma_sonka_match(uint8_t *v5, uint8_t *m)
/* ==================================== */
{
  int32_t i;
  uint8_t v3[27];
  extract_vois27(v5, 62, 5, 25, 125, v3);
  for (i = 0; i < 27; i++)
  {
    if ((m[i] == OBJ) && !v3[i]) return 0;
    if ((m[i] == BGD) && v3[i]) return 0;
  }
  return 1;
} // ma_sonka_match()

/* ==================================== */
int32_t ma_sonka_dir_ABC(uint8_t *v, uint8_t *m)
/* ==================================== */
{
  int32_t x = 62, xm = 13;
  //if (trace) print_vois125(v);
  //if (trace) print_vois27(m);
  //if (trace) printf("match_dir_ABC 0 - S=%d\n", m[__S(x)]);
  if ((m[__S(xm)]==OBJ) && !v[_S(_S(x))]) return 0;
  //if (trace) printf("match_dir_ABC 1 - W=%d\n", m[__W(x)]);
  if ((m[__W(xm)]==OBJ) && !v[_W(_W(x))]) return 0;
  //if (trace) printf("match_dir_ABC 2 - D=%d\n", m[__D(x)]);
  if ((m[__D(xm)]==OBJ) && !v[_D(_D(x))]) return 0;
  //if (trace) printf("match_ABC\n");
  return 1;
} // ma_sonka_dir_ABC()

/* ==================================== */
int32_t ma_sonka_dir_D(uint8_t *v, uint8_t *m)
/* ==================================== */
{
  int32_t x = 62, xm = 13, y, ym;
  // le test "x simple" a été fait en amont.
  ym = __S(__U(xm)); y = _S(_U(x));
  if ((m[ym]==OBJ) && !v[_S(_S(x))] && !v[_S(y)] && !v[_S(_U(y))] && !v[_U(y)] && !v[_U(_U(x))]) return 0;
  ym = __S(__D(xm)); y = _S(_D(x));
  if ((m[ym]==OBJ) && !v[_S(_S(x))] && !v[_S(y)] && !v[_S(_D(y))] && !v[_D(y)] && !v[_D(_D(x))]) return 0;
  ym = __S(__E(xm)); y = _S(_E(x));
  if ((m[ym]==OBJ) && !v[_S(_S(x))] && !v[_S(y)] && !v[_S(_E(y))] && !v[_E(y)] && !v[_E(_E(x))]) return 0;
  ym = __S(__W(xm)); y = _S(_W(x));
  if ((m[ym]==OBJ) && !v[_S(_S(x))] && !v[_S(y)] && !v[_S(_W(y))] && !v[_W(y)] && !v[_W(_W(x))]) return 0;
  ym = __W(__U(xm)); y = _W(_U(x));
  if ((m[ym]==OBJ) && !v[_W(_W(x))] && !v[_W(y)] && !v[_W(_U(y))] && !v[_U(y)] && !v[_U(_U(x))]) return 0;
  ym = __W(__D(xm)); y = _W(_D(x));
  if ((m[ym]==OBJ) && !v[_W(_W(x))] && !v[_W(y)] && !v[_W(_D(y))] && !v[_D(y)] && !v[_D(_D(x))]) return 0;
  return 1;
} // ma_sonka_dir_D()

#ifdef DEBUG
/* ==================================== */
void test_ma_sonka_deletable()
/* ==================================== */
{

  swap_U_L_27(A);
  swap_U_L_27(A); // retour

  isometrieXZ_vois27(A);
  swap_W_E_27(A);
  swap_W_E_27(A); // retour
  isometrieXZ_vois27(A); // retour

  isometrieYZ_vois27(A);
  swap_S_N_27(A);
  swap_S_N_27(A); // retour
  isometrieYZ_vois27(A); // retour

  rotate_90_Z_27(C);
  rotate_90_Z_27(C);
  rotate_90_Z_27(C);
  rotate_90_Z_27(C);

  swap_U_L_27(C);
  rotate_90_Z_27(C);
  rotate_90_Z_27(C);
  rotate_90_Z_27(C);
  rotate_90_Z_27(C);
  swap_U_L_27(C); // retour

  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);

  swap_U_L_27(B); swap_U_L_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  swap_U_L_27(B); swap_U_L_27(D); // retour

  isometrieYZ_vois27(B); isometrieYZ_vois27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  swap_S_N_27(B); swap_S_N_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  swap_S_N_27(B); swap_S_N_27(D); // retour
  isometrieYZ_vois27(B); isometrieYZ_vois27(D); // retour

} // test_ma_sonka_deletable()
#endif

/* ==================================== */
int32_t ma_sonka_deletable(uint8_t *v)
/* ==================================== */
{
  //if (trace) print_vois125(v);

  //if (trace) printf("deletable A0\n");
  //if (trace) print_vois27(A);
  if (ma_sonka_match(v, A) && ma_sonka_dir_ABC(v, A)) return 1; // U
  swap_U_L_27(A);
  //if (trace) printf("deletable A1\n");
  //if (trace) print_vois27(A);
  if (ma_sonka_match(v, A) && ma_sonka_dir_ABC(v, A)) return 1; // D
  swap_U_L_27(A); // retour

  isometrieXZ_vois27(A);
  //if (trace) printf("deletable A2\n");
  //if (trace) print_vois27(A);
  if (ma_sonka_match(v, A) && ma_sonka_dir_ABC(v, A)) return 1; // E
  swap_W_E_27(A);
  //if (trace) printf("deletable A3\n");
  //if (trace) print_vois27(A);
  if (ma_sonka_match(v, A) && ma_sonka_dir_ABC(v, A)) return 1; // W
  swap_W_E_27(A); // retour
  isometrieXZ_vois27(A); // retour

  isometrieYZ_vois27(A);
  //if (trace) printf("deletable A4\n");
  //if (trace) print_vois27(A);
  if (ma_sonka_match(v, A) && ma_sonka_dir_ABC(v, A)) return 1; // N
  swap_S_N_27(A);
  //if (trace) printf("deletable A5\n");
  //if (trace) print_vois27(A);
  if (ma_sonka_match(v, A) && ma_sonka_dir_ABC(v, A)) return 1; // S
  swap_S_N_27(A); // retour
  isometrieYZ_vois27(A); // retour

  //if (trace) printf("deletable C0\n");
  //if (trace) print_vois27(C);
  if (ma_sonka_match(v, C) && ma_sonka_dir_ABC(v, C)) return 1;
  rotate_90_Z_27(C);
  //if (trace) printf("deletable C1\n");
  //if (trace) print_vois27(C);
  if (ma_sonka_match(v, C) && ma_sonka_dir_ABC(v, C)) return 1;
  rotate_90_Z_27(C);
  //if (trace) printf("deletable C2\n");
  //if (trace) print_vois27(C);
  if (ma_sonka_match(v, C) && ma_sonka_dir_ABC(v, C)) return 1;
  rotate_90_Z_27(C);
  //if (trace) printf("deletable C3\n");
  //if (trace) print_vois27(C);
  if (ma_sonka_match(v, C) && ma_sonka_dir_ABC(v, C)) return 1;
  rotate_90_Z_27(C);

  swap_U_L_27(C);
  //if (trace) printf("deletable C4\n");
  //if (trace) print_vois27(C);
  if (ma_sonka_match(v, C) && ma_sonka_dir_ABC(v, C)) return 1;
  rotate_90_Z_27(C);
  //if (trace) printf("deletable C5\n");
  //if (trace) print_vois27(C);
  if (ma_sonka_match(v, C) && ma_sonka_dir_ABC(v, C)) return 1;
  rotate_90_Z_27(C);
  //if (trace) printf("deletable C6\n");
  //if (trace) print_vois27(C);
  if (ma_sonka_match(v, C) && ma_sonka_dir_ABC(v, C)) return 1;
  rotate_90_Z_27(C);
  //if (trace) printf("deletable C7\n");
  //if (trace) print_vois27(C);
  if (ma_sonka_match(v, C) && ma_sonka_dir_ABC(v, C)) return 1;
  rotate_90_Z_27(C);
  swap_U_L_27(C); // retour

  //if (trace) printf("deletable B-D0\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  //if (trace) printf("deletable B-D1\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  //if (trace) printf("deletable B-D2\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  //if (trace) printf("deletable B-D3\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);

  swap_U_L_27(B); swap_U_L_27(D);
  //if (trace) printf("deletable B-D4\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  //if (trace) printf("deletable B-D5\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  //if (trace) printf("deletable B-D6\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  //if (trace) printf("deletable B-D7\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  swap_U_L_27(B); swap_U_L_27(D); // retour

  isometrieYZ_vois27(B); isometrieYZ_vois27(D);
  //if (trace) printf("deletable B-D8\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  //if (trace) printf("deletable B-D9\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  swap_S_N_27(B); swap_S_N_27(D);
  //if (trace) printf("deletable B-D10\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  rotate_90_Z_27(B); rotate_90_Z_27(D);
  //if (trace) printf("deletable B-D11\n");
  //if (trace) print_vois27(B);
  //if (trace) print_vois27(D);
  if (ma_sonka_match(v, B) && ma_sonka_dir_ABC(v, B)) return 1;
  if (ma_sonka_match(v, D) && ma_sonka_dir_D(v, D)) return 1;
  swap_S_N_27(B); swap_S_N_27(D); // retour
  isometrieYZ_vois27(B); isometrieYZ_vois27(D); // retour

  return 0;
} // ma_sonka_deletable()

/* ==================================== */
int32_t ma_sonka_nontail(uint8_t *v, uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int n = mctopo3d_nbvoiso26(S, p, rs, ps, N);
  if (n == 1) // line_end point = tail
    return 0;
  if (n == 2)
  {
    if (v[MS_S] && v[MS_E]) return 0; // near_line_end point = tail
    if (v[MS_S] && v[MS_U]) return 0;
    if (v[MS_N] && v[MS_W]) return 0;
    if (v[MS_U] && v[MS_W]) return 0;
    if (v[MS_N] && v[MS_D]) return 0;
    if (v[MS_E] && v[MS_D]) return 0;
  }
  return 1;
} // ma_sonka_nontail()

/* ==================================== */
int32_t lmasonka1996(
		     struct xvimage *image,
		     int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lmasonka1996"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab;
  uint8_t v3[27];
  uint8_t v5[125];
  uint8_t AS[27], BS[27], CS[27], DS[27];

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif  
  mctopo3d_init_topo3d();

  // init masques
  memset(A, ANY, 27); memset(B, ANY, 27); memset(C, ANY, 27); memset(D, ANY, 27); 
  A[13] = A[22] = OBJ;
  A[0] = A[1] = A[2] = A[3] = A[4] = A[5] = A[6] = A[7] = A[8] = BGD;
  B[13] = B[22] = B[12] = OBJ;
  B[1] = B[2] = B[4] = B[5] = B[7] = B[8] = B[11] = B[14] = B[17] = BGD;
  C[13] = C[22] = C[12] = C[16] = OBJ;
  C[1] = C[2] = C[4] = C[5] = C[10] = C[11] = C[14] = BGD;
  D[13] = D[21] = OBJ;
  D[12] = D[3] = D[4] = D[5] = D[14] = D[23] = D[22] = BGD;
  memcpy(AS, A, 27); memcpy(BS, B, 27); memcpy(CS, C, 27); memcpy(DS, D, 27); // pour restaurer les masques après chaque appel à ma_sonka_deletable

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    step++;
    nonstab = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = MS_OBJECT;

    for (k = 2; k < ds-2; k++)
    for (j = 2; j < cs-2; j++)
    for (i = 2; i < rs-2; i++)
    {
      x = k*ps + j*rs + i;
      if (S[x] && mctopo3d_simple26(S, x, rs, ps, N))
      {
	extract_vois27(S, x, rs, ps, N, v3);
	extract_vois125(S, x, rs, ps, N, v5);
	if (ma_sonka_nontail(v3,S,x,rs,ps,N) && ma_sonka_deletable(v5))
	{
	  SET_MS_DELETABLE(S[x]);
	}
	memcpy(A, AS, 27); memcpy(B, BS, 27); memcpy(C, CS, 27); memcpy(D, DS, 27);
	// restauration nécessaire car on sort de ma_sonka_deletable sans forcément terminer

      } // if (S[x])
    } // for i, j, k

    for (k = 2; k < ds-2; k++)
    for (j = 2; j < cs-2; j++)
    for (i = 2; i < rs-2; i++)
    {
      x = k*ps + j*rs + i;
      if (IS_MS_DELETABLE(S[x]))
      {
	S[x] = 0;
	nonstab = 1;
      }
    } // for i, j, k
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  mctopo3d_termine_topo3d();
  return(1);
} /* lmasonka1996() */

/* ==================================== */
int32_t lmasonka_psimple2010(
		     struct xvimage *image,
		     int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lmasonka_psimple2010"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  struct xvimage *t;
  uint8_t *T;
  struct xvimage *p;               /* pour les points P-simples */ 
  uint8_t *P;
  int32_t step, nonstab;
  uint8_t v3[27];
  uint8_t v5[125];
  uint8_t AS[27], BS[27], CS[27], DS[27]; // DEBUG

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif  
  mctopo3d_init_topo3d();

  // init masques
  memset(A, ANY, 27); memset(B, ANY, 27); memset(C, ANY, 27); memset(D, ANY, 27); 
  A[13] = A[22] = OBJ;
  A[0] = A[1] = A[2] = A[3] = A[4] = A[5] = A[6] = A[7] = A[8] = BGD;
  B[13] = B[22] = B[12] = OBJ;
  B[1] = B[2] = B[4] = B[5] = B[7] = B[8] = B[11] = B[14] = B[17] = BGD;
  C[13] = C[22] = C[12] = C[16] = OBJ;
  C[1] = C[2] = C[4] = C[5] = C[10] = C[11] = C[14] = BGD;
  D[13] = D[21] = OBJ;
  D[12] = D[3] = D[4] = D[5] = D[14] = D[23] = D[22] = BGD;
  memcpy(AS, A, 27); memcpy(BS, B, 27); memcpy(CS, C, 27); memcpy(DS, D, 27); // pour restaurer les masques après chaque appel à ma_sonka_deletable

  if (nsteps == -1) nsteps = 1000000000;

  p = copyimage(image); assert(p != NULL);
  P = UCHARDATA(p);
  t = copyimage(image); assert(t != NULL);
  T = UCHARDATA(t);

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    step++;
    nonstab = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = MS_OBJECT;
    razimage(p);

    for (k = 2; k < ds-2; k++)
    for (j = 2; j < cs-2; j++)
    for (i = 2; i < rs-2; i++)
    {
      x = k*ps + j*rs + i;
      if (S[x] && mctopo3d_simple26(S, x, rs, ps, N))
      {
	extract_vois27(S, x, rs, ps, N, v3);
	extract_vois125(S, x, rs, ps, N, v5);
	if (ma_sonka_nontail(v3,S,x,rs,ps,N) && ma_sonka_deletable(v5)) P[x] = 1;
	memcpy(A, AS, 27); memcpy(B, BS, 27); memcpy(C, CS, 27); memcpy(D, DS, 27);
	// restauration nécessaire car on sort de ma_sonka_deletable sans forcément terminer
      } // if (S[x])
    } // for i, j, k

    for (k = 2; k < ds-2; k++) // retire en parallèle les points P_simples
    for (j = 2; j < cs-2; j++)
    for (i = 2; i < rs-2; i++)
    {
      x = k*ps + j*rs + i;
      if (P[x] && P_simple26(S, P, x, rs, ps, N))      
      {
	T[x] = 0;
	nonstab = 1;
      }
    } // for i, j, k

    memcpy(S, T, N);

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  mctopo3d_termine_topo3d();
  freeimage(p);
  freeimage(t);
  return(1);
} /* lmasonka_psimple2010() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Ma & Wan (curviligne (18/6) 6 subiterations, CVIU 2000)
// M. Couprie, jan. 2012
/* ============================================================ */
/* ============================================================ */

#define MW_OBJECT      1
#define MW_DELETE      2

#define IS_MW_OBJECT(f)     (f&MW_OBJECT)
#define IS_MW_DELETE(f)     (f&MW_DELETE)

#define SET_MW_OBJECT(f)    (f|=MW_OBJECT)
#define SET_MW_DELETE(f)    (f|=MW_DELETE)

static void mw_copyslice1(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[1] = v[1]; w[4] = v[4]; w[7] = v[7];
  w[10] = v[10]; w[13] = v[13]; w[16] = v[16];
  w[19] = v[19]; w[22] = v[22]; w[25] = v[25];
} // mw_copyslice1()

static void mw_copyslice2(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[3] = v[3]; w[4] = v[4]; w[5] = v[5];
  w[12] = v[12]; w[13] = v[13]; w[14] = v[14];
  w[21] = v[21]; w[22] = v[22]; w[23] = v[23];
} // mw_copyslice2()

static void mw_copyclique1(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[13] = v[13]; w[14] = v[14]; w[16] = v[16]; w[17] = v[17];
  w[22] = v[22]; w[23] = v[23]; w[25] = v[25]; w[26] = v[26];
} // mw_copyclique1()

static void mw_copyclique2(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[13] = v[13]; w[14] = v[14]; w[10] = v[10]; w[11] = v[11];
  w[22] = v[22]; w[23] = v[23]; w[19] = v[19]; w[20] = v[20];
} // mw_copyclique2()

static void mw_copyclique3(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[9] = v[9]; w[10] = v[10]; w[12] = v[12]; w[13] = v[13];
  w[18] = v[18]; w[19] = v[19]; w[21] = v[21]; w[22] = v[22];
} // mw_copyclique3()

static void mw_copyclique4(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[12] = v[12]; w[13] = v[13]; w[15] = v[15]; w[16] = v[16];
  w[21] = v[21]; w[22] = v[22]; w[24] = v[24]; w[25] = v[25];
} // mw_copyclique4()

/* ==================================== */
static int32_t mw_cond1(uint8_t *v)
/* ==================================== */
{
  uint8_t w[27]; 
  int32_t top, topb;

  if (!v[22]) return 0;

  mw_copyslice1(v, w);
  mctopo3d_top18(w, 13, 3, 9, 27, &top, &topb);
  if (top != 1) return 0;
  mw_copyslice2(v, w);
  mctopo3d_top18(w, 13, 3, 9, 27, &top, &topb);
  if (top != 1) return 0;

  mw_copyclique1(v, w);
  mctopo3d_top18(w, 13, 3, 9, 27, &top, &topb);
  if (top != 1) return 0;
  mw_copyclique2(v, w);
  mctopo3d_top18(w, 13, 3, 9, 27, &top, &topb);
  if (top != 1) return 0;
  mw_copyclique3(v, w);
  mctopo3d_top18(w, 13, 3, 9, 27, &top, &topb);
  if (top != 1) return 0;
  mw_copyclique4(v, w);
  mctopo3d_top18(w, 13, 3, 9, 27, &top, &topb);
  if (top != 1) return 0;

  return 1;
} // mw_cond1()

/* ==================================== */
static int32_t mw_cond2_aux(uint8_t *v)
/* ==================================== */
{
  if (!v[25]) return 0;
  if (v[16] || v[7] || v[4] || v[1] || v[10] || v[19] || v[22]) return 0;
  if (v[5] && !v[14]) return 0;
  if (v[3] && !v[12]) return 0;
  if (v[14] && !v[23] && !v[26]) return 0;
  if (v[12] && !v[21] && !v[24]) return 0;
  if (v[11] && !v[14] && !v[23]) return 0;
  if (v[9] && !v[12] && !v[21]) return 0;
  return 1;
} // mw_cond2_aux()

/* ==================================== */
static int32_t mw_cond2(uint8_t *v)
/* ==================================== */
{
  if (mw_cond2_aux(v)) return 1;
  rotate_90_Z_27(v);
  if (mw_cond2_aux(v)) return 1;
  rotate_90_Z_27(v);
  if (mw_cond2_aux(v)) return 1;
  rotate_90_Z_27(v);
  if (mw_cond2_aux(v)) return 1;
  rotate_90_Z_27(v);
  return 0;
} // mw_cond2()

/* ==================================== */
int32_t lmawan2000(struct xvimage *image,
			    int32_t n_steps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lmawan2000"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab;
  uint8_t v[27];

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = MW_OBJECT;

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

    // SUBITERATION FOR UPPER VOXELS
    // PREMIERE PASSE : MARQUE LES POINTS EFFACABLES
    for (i = 0; i < N; i++) 
      if (S[i] && (mctopo3d_nbvoiso18(S, i, rs, ps, N) != 1))
      {
	extract_vois27(S, i, rs, ps, N, v);
	if (!v[4] && (mw_cond1(v) || mw_cond2(v))) SET_MW_DELETE(S[i]);
      }
    // DEUXIEME PASSE : EFFACE LES POINTS MARQUES DELETE
    for (i = 0; i < N; i++) if (IS_MW_DELETE(S[i])) { S[i] = 0; nonstab = 1; }

    // SUBITERATION FOR LOWER VOXELS
    // PREMIERE PASSE : MARQUE LES POINTS EFFACABLES
    for (i = 0; i < N; i++) 
      if (S[i] && (mctopo3d_nbvoiso18(S, i, rs, ps, N) != 1))
      {
	extract_vois27(S, i, rs, ps, N, v);
	swap_U_L_27(v);
	if (!v[4] && (mw_cond1(v) || mw_cond2(v))) SET_MW_DELETE(S[i]);
      }
    // DEUXIEME PASSE : EFFACE LES POINTS MARQUES DELETE
    for (i = 0; i < N; i++) if (IS_MW_DELETE(S[i])) { S[i] = 0; nonstab = 1; }

    // SUBITERATION FOR EAST VOXELS
    // PREMIERE PASSE : MARQUE LES POINTS EFFACABLES
    for (i = 0; i < N; i++) 
      if (S[i] && (mctopo3d_nbvoiso18(S, i, rs, ps, N) != 1))
      {
	extract_vois27(S, i, rs, ps, N, v);
	isometrieXZ_vois27(v);
	if (!v[4] && (mw_cond1(v) || mw_cond2(v))) SET_MW_DELETE(S[i]);
      }
    // DEUXIEME PASSE : EFFACE LES POINTS MARQUES DELETE
    for (i = 0; i < N; i++) if (IS_MW_DELETE(S[i])) { S[i] = 0; nonstab = 1; }

    // SUBITERATION FOR WEST VOXELS
    // PREMIERE PASSE : MARQUE LES POINTS EFFACABLES
    for (i = 0; i < N; i++) 
      if (S[i] && (mctopo3d_nbvoiso18(S, i, rs, ps, N) != 1))
      {
	extract_vois27(S, i, rs, ps, N, v);
	isometrieXZ_vois27(v);
	swap_U_L_27(v);
	if (!v[4] && (mw_cond1(v) || mw_cond2(v))) SET_MW_DELETE(S[i]);
      }
    // DEUXIEME PASSE : EFFACE LES POINTS MARQUES DELETE
    for (i = 0; i < N; i++) if (IS_MW_DELETE(S[i])) { S[i] = 0; nonstab = 1; }

    // SUBITERATION FOR NORTH VOXELS
    // PREMIERE PASSE : MARQUE LES POINTS EFFACABLES
    for (i = 0; i < N; i++) 
      if (S[i] && (mctopo3d_nbvoiso18(S, i, rs, ps, N) != 1))
      {
	extract_vois27(S, i, rs, ps, N, v);
	isometrieYZ_vois27(v);
	if (!v[4] && (mw_cond1(v) || mw_cond2(v))) SET_MW_DELETE(S[i]);
      }
    // DEUXIEME PASSE : EFFACE LES POINTS MARQUES DELETE
    for (i = 0; i < N; i++) if (IS_MW_DELETE(S[i])) { S[i] = 0; nonstab = 1; }
    // SUBITERATION FOR SOUTH VOXELS
    // PREMIERE PASSE : MARQUE LES POINTS EFFACABLES
    for (i = 0; i < N; i++) 
      if (S[i] && (mctopo3d_nbvoiso18(S, i, rs, ps, N) != 1))
      {
	extract_vois27(S, i, rs, ps, N, v);
	isometrieYZ_vois27(v);
	swap_U_L_27(v);
	if (!v[4] && (mw_cond1(v) || mw_cond2(v))) SET_MW_DELETE(S[i]);
      }
    // DEUXIEME PASSE : EFFACE LES POINTS MARQUES DELETE
    for (i = 0; i < N; i++) if (IS_MW_DELETE(S[i])) { S[i] = 0; nonstab = 1; }

  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  return(1);
  
} // lmawan2000()

/* ============================================================ */
/* ============================================================ */
// Algo. de Tsao & Fu (surfacique et curviligne 6 subiterations, CGIP 1981)
// M. Couprie, feb. 2012
/* ============================================================ */
/* ============================================================ */

#define TF_OBJECT      1
#define TF_DELETABLE   2

#define IS_TF_OBJECT(f)     (f&TF_OBJECT)
#define IS_TF_DELETABLE(f)     (f&TF_DELETABLE)

#define SET_TF_OBJECT(f)    (f|=TF_OBJECT)
#define SET_TF_DELETABLE(f)    (f|=TF_DELETABLE)

static int32_t TF_3x3_deletable(uint8_t *v)
{
  int32_t t, tb;
  if (nbvois8(v, 4, 3, 9) < 2) return 0;
  top8(v, 4, 3, 9, &t, &tb);
  if (t > 1) return 0;
  return 1;
} // TF_3x3_deletable()

static int32_t TF_deletable(int32_t d, uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
{
  uint8_t v[9], w[9];
  int32_t x, y, z, i, j, k;
  x = p % rs; y = (p % ps) / rs; z = p / ps;
  switch (d)
  {
#ifndef DIRTOURNE
  case 0: case 1:
    for (k = 0; k < 3; k++) 
      for (i = 0; i < 3; i++) 
	v[k*3 + i] = S[(z-1+k)*ps + y*rs + (x-1+i)];
    for (j = 0; j < 3; j++) 
      for (i = 0; i < 3; i++) 
	w[j*3 + i] = S[z*ps + (y-1+j)*rs + (x-1+i)];
    break;
  case 2: case 3:
    for (k = 0; k < 3; k++) 
      for (j = 0; j < 3; j++) 
	v[k*3 + j] = S[(z-1+k)*ps + (y-1+j)*rs + x];
    for (j = 0; j < 3; j++) 
      for (i = 0; i < 3; i++) 
	w[j*3 + i] = S[z*ps + (y-1+j)*rs + (x-1+i)];
    break;
  case 4: case 5:
    for (k = 0; k < 3; k++) 
      for (j = 0; j < 3; j++) 
	v[k*3 + j] = S[(z-1+k)*ps + (y-1+j)*rs + x];
    for (k = 0; k < 3; k++) 
      for (i = 0; i < 3; i++) 
	w[k*3 + i] = S[(z-1+k)*ps + y*rs + (x-1+i)];
    break;
#else
    assert(1);
#endif
  } // switch (d)
  if (TF_3x3_deletable(v) && TF_3x3_deletable(w)) return 1;
  return 0;
} // TF_deletable()

/* ==================================== */
int32_t ltsaofu6dirsurf1981(struct xvimage *image,
			    int32_t n_steps)
/* ==================================== */
#undef F_NAME
#define F_NAME "ltsaofu6dirsurf1981"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  if (n_steps == -1) n_steps = 1000000000;

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

    for (d = 0; d < 6; d++)
    {
#ifdef VERBOSE
      printf("%s: substep %d\n", F_NAME, d);
#endif

      for (i = 0; i < N; i++) if (S[i]) S[i] = TF_OBJECT;

      // MARQUE LES POINTS DELETABLES DE DIRECTION d
      for (i = 0; i < N; i++) 
	if (S[i] && direction(S, i, d, rs, ps, N) &&
	    (mctopo3d_nbvoiso26(S, i, rs, ps, N) > 2) && 
	    mctopo3d_simple26(S, i, rs, ps, N) && 
	    TF_deletable(d, S, i, rs, ps, N))
	{
	  SET_TF_DELETABLE(S[i]);
	}

      // EFFACE LES POINTS DELETABLE
      for (i = 0; i < N; i++)
	if (IS_TF_DELETABLE(S[i]))
	{
	  S[i] = 0; 
	  nonstab = 1; 
	}

#ifdef DEBUG_ltsaofu6dirsurf1981
      if (d==0) writeimage(image, "_ss0");
      if (d==1) writeimage(image, "_ss1");
      if (d==2) writeimage(image, "_ss2");
      if (d==3) writeimage(image, "_ss3");
      if (d==4) writeimage(image, "_ss4");
      if (d==5) writeimage(image, "_ss5");
#endif
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  return(1);
  
} // ltsaofu6dirsurf1981()

static int32_t TF_3x3_deletablecurv(uint8_t *v)
{
  int32_t t, tb;
  //printf("%d %d %d | %d %d %d | %d %d %d\n", 
  //       v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
  top8(v, 4, 3, 9, &t, &tb);
  if (t > 1) return 0;
  if (nbvois8(v, 4, 3, 9) >= 2) return 1;
  // nbvois8 == 1 : allowed if it is on the opposite direction
  if (!v[5] && !v[3]) return 0;
  return 1;
} // TF_3x3_deletablecurv()

static int32_t TF_deletablecurv(int32_t d, uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
{
  uint8_t v[9], w[9];
  int32_t x, y, z, i, j, k;
  x = p % rs; y = (p % ps) / rs; z = p / ps;
  //printf("dir %d point %d %d %d \n", d, x, y, z);
  switch (d)
  {
#ifndef DIRTOURNE
  case 0: case 1:
    for (k = 0; k < 3; k++) 
      for (i = 0; i < 3; i++) 
	v[k*3 + i] = S[(z-1+k)*ps + y*rs + (x-1+i)];
    for (j = 0; j < 3; j++) 
      for (i = 0; i < 3; i++) 
	w[j*3 + i] = S[z*ps + (y-1+j)*rs + (x-1+i)];
    break;
  case 2: case 3:
    for (k = 0; k < 3; k++) 
      for (j = 0; j < 3; j++) 
	v[k*3 + j] = S[(z-1+k)*ps + (y-1+j)*rs + x];
    for (j = 0; j < 3; j++) 
      for (i = 0; i < 3; i++) 
	w[i*3 + j] = S[z*ps + (y-1+j)*rs + (x-1+i)];
    break;
  case 4: case 5:
    for (k = 0; k < 3; k++) 
      for (j = 0; j < 3; j++) 
	v[j*3 + k] = S[(z-1+k)*ps + (y-1+j)*rs + x];
    for (k = 0; k < 3; k++) 
      for (i = 0; i < 3; i++) 
	w[i*3 + k] = S[(z-1+k)*ps + y*rs + (x-1+i)];
    break;
#else
    assert(1);
#endif
  } // switch (d)
  if (TF_3x3_deletablecurv(v) && TF_3x3_deletablecurv(w)) return 1;
  return 0;
} // TF_deletablecurv()

/* ==================================== */
int32_t ltsaofu6dircurv1981(struct xvimage *image,
			    int32_t n_steps)
/* ==================================== */
#undef F_NAME
#define F_NAME "ltsaofu6dircurv1981"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  if (n_steps == -1) n_steps = 1000000000;

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

    for (d = 0; d < 6; d++)
    {
#ifdef VERBOSE
      printf("%s: substep %d\n", F_NAME, d);
#endif

      for (i = 0; i < N; i++) if (S[i]) S[i] = TF_OBJECT;

      // MARQUE LES POINTS DELETABLES DE DIRECTION d
      for (i = 0; i < N; i++) 
	if (S[i] && direction(S, i, d, rs, ps, N))
	{
	  //printf("examen point %d %d %d\n", i % rs, (i % ps) / rs, i / ps);
	  if ((mctopo3d_nbvoiso26(S, i, rs, ps, N) >= 2) && 
	      mctopo3d_simple26(S, i, rs, ps, N) && 
	      TF_deletablecurv(d, S, i, rs, ps, N))
	  SET_TF_DELETABLE(S[i]);
	}

      // EFFACE LES POINTS DELETABLE
      for (i = 0; i < N; i++)
	if (IS_TF_DELETABLE(S[i]))
	{
	  S[i] = 0; 
	  nonstab = 1; 
	}

#ifdef DEBUG_ltsaofu6dircurv1981
      if (d==0) writeimage(image, "_ss0");
      if (d==1) writeimage(image, "_ss1");
      if (d==2) writeimage(image, "_ss2");
      if (d==3) writeimage(image, "_ss3");
      if (d==4) writeimage(image, "_ss4");
      if (d==5) writeimage(image, "_ss5");
#endif
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  return(1);
  
} // ltsaofu6dircurv1981()

/* ============================================================ */
/* ============================================================ */
// Algo. de Tsao & Fu (curviligne 6 subiterations, IEEE PRIP 1982)
// M. Couprie, jan. 2012
/* ============================================================ */
/* ============================================================ */

#define T_OBJECT      1
#define T_SIMPLE      2
#define T_NONEND      4

#define IS_T_OBJECT(f)     (f&T_OBJECT)
#define IS_T_SIMPLE(f)     (f&T_SIMPLE)
#define IS_T_NONEND(f)     (f&T_NONEND)

#define SET_T_OBJECT(f)    (f|=T_OBJECT)
#define SET_T_SIMPLE(f)    (f|=T_SIMPLE)
#define SET_T_NONEND(f)    (f|=T_NONEND)

#define UNSET_T_OBJECT(f)  (f&=~T_OBJECT)
#define UNSET_T_SIMPLE(f)  (f&=~T_SIMPLE)

/* ==================================== */
int32_t ltsaofu6dircurv1982(struct xvimage *image,
			    struct xvimage *inhibit,
			    int32_t n_steps)
/* ==================================== */
#undef F_NAME
#define F_NAME "ltsaofu6dircurv1982"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL;
  int32_t step, nonstab, d;

  if (n_steps == -1) n_steps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = T_OBJECT;

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
    printf("%s: step %d\n", F_NAME, step);
#endif

    for (d = 0; d < 6; d++)
    {
#ifdef VERBOSE
      printf("%s: substep %d\n", F_NAME, d);
#endif
      // PREMIERE PASSE : MARQUE LES POINTS SIMPLES DE DIRECTION d ET PAS DANS I
      for (i = 0; i < N; i++) 
	if (IS_T_OBJECT(S[i]) && mctopo3d_simple26(S, i, rs, ps, N) && 
	    direction(S, i, d, rs, ps, N) && (!I || !I[i]))
	  {
	    SET_T_SIMPLE(S[i]);
	  }
      // DEUXIEME PASSE : MARQUE LES POINTS SIMPLES NONEND
      for (i = 0; i < N; i++) 
	if (IS_T_SIMPLE(S[i]))
	{ 
	  if (mctopo3d_tsao_fu_nonend(S, i, T_OBJECT|T_SIMPLE, rs, ps, N))
	    SET_T_NONEND(S[i]);
	}

      // TROISIEME PASSE : EFFACE LES POINTS SIMPLES NONEND
      for (i = 0; i < N; i++)
	if (IS_T_NONEND(S[i]))
	{
	  S[i] = 0; 
	  nonstab = 1; 
	}

      for (i = 0; i < N; i++) if (S[i]) S[i] = T_OBJECT;
#ifdef DEBUG_ltsaofu6dircurv1982
      if (d==0) writeimage(image, "_ss0");
      if (d==1) writeimage(image, "_ss1");
      if (d==2) writeimage(image, "_ss2");
      if (d==3) writeimage(image, "_ss3");
      if (d==4) writeimage(image, "_ss4");
      if (d==5) writeimage(image, "_ss5");
#endif
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  return(1);
  
} // ltsaofu6dircurv1982()

/* ============================================================ */
/* ============================================================ */
// Algo. de Ma, Wan & Chang (curviligne 2 subfields, PRL 2002)
// M. Couprie, dec. 2011
/* ============================================================ */
/* ============================================================ */

#define MWC_OBJECT      1
#define MWC_ORTH        2
#define MWC_DIAG        4
#define MWC_DIAGPRES    8
#define MWC_TWIG        16

#define IS_MWC_ORTH(f) (f&MWC_ORTH)
#define SET_MWC_ORTH(f) (f|=MWC_ORTH)
#define IS_MWC_DIAG(f) (f&MWC_DIAG)
#define SET_MWC_DIAG(f) (f|=MWC_DIAG)
//#define UNSET_MWC_DIAG(f) (f&=~MWC_DIAG)
#define IS_MWC_DIAGPRES(f) (f&MWC_DIAGPRES)
#define SET_MWC_DIAGPRES(f) (f|=MWC_DIAGPRES)
#define IS_MWC_TWIG(f) (f&MWC_TWIG)
#define SET_MWC_TWIG(f) (f|=MWC_TWIG)

static int32_t MWC_ORTH_U_deletable(uint8_t *v)
{
  if (v[8]) return 0;
  if (!v[17]) return 0;
  if (v[9] && !v[0]) return 0;
  if (v[16] && !v[0] && !v[6]) return 0;
  if (v[11] && !v[2]) return 0;
  if (v[10] && !v[2] && !v[0]) return 0;
  if (v[13] && !v[4]) return 0;
  if (v[12] && !v[4] && !v[2]) return 0;
  if (v[15] && !v[6]) return 0;
  if (v[14] && !v[6] && !v[4]) return 0;
  return 1;
} // MWC_ORTH_U_deletable()

static int32_t MWC_ORTH_deletable(uint8_t *v)
{
  if (MWC_ORTH_U_deletable(v)) return 1; // U-deletable
  swap_U_L(v);
  if (MWC_ORTH_U_deletable(v)) return 1; // L-deletable

  isometrieXZ_vois(v);
  if (MWC_ORTH_U_deletable(v)) return 1; // E-deletable
  swap_U_L(v);
  if (MWC_ORTH_U_deletable(v)) return 1; // W-deletable
  isometrieXZ_vois(v);

  isometrieYZ_vois(v);
  if (MWC_ORTH_U_deletable(v)) return 1; // N-deletable
  swap_U_L(v);
  if (MWC_ORTH_U_deletable(v)) return 1; // S-deletable
  isometrieYZ_vois(v);

  return 0;
} // MWC_ORTH_deletable()

static int32_t MWC_DIAG_UE_deletable(uint8_t *v)
{
#ifdef DEBUG_lmawanchangcurv2subfields2002
  print_vois(v);
#endif
  if (!v[22]) return 0;
  if (v[4] || v[13] || v[8] || v[9] || v[0] || v[18] || v[17]) return 0;

  if (v[16] && !v[15] && !v[7] && !v[6]) return 0; // a1
  if (v[10] && !v[11] && !v[1] && !v[2]) return 0; // a2

  if ((v[15] || v[14]) && !v[6] && !v[5]) return 0; // b1 or c1
  if ((v[11] || v[12]) && !v[2] && !v[3]) return 0; // b2 or c2

  if ((v[7] || v[25]) && !v[6] && !v[24]) return 0; // d1 or g1
  if ((v[1] || v[19]) && !v[2] && !v[20]) return 0; // d2 or g2

  if (!v[6] && v[5] && v[7] && v[24] && v[15]) return 0; // e1
  if (!v[2] && v[1] && v[3] && v[20] && v[11]) return 0; // e2

  return 1;
} // MWC_DIAG_UE_deletable()

static int32_t MWC_DIAG_deletable(uint8_t *v)
{
  if (MWC_DIAG_UE_deletable(v)) return 1; // UE
  rotate_90_Z(v);
  if (MWC_DIAG_UE_deletable(v)) return 1; // UN
  rotate_90_Z(v);
  if (MWC_DIAG_UE_deletable(v)) return 1; // UW
  rotate_90_Z(v);
  if (MWC_DIAG_UE_deletable(v)) return 1; // US
  rotate_90_Z(v);
  swap_U_L(v);
  if (MWC_DIAG_UE_deletable(v)) return 1; // LE
  rotate_90_Z(v);
  if (MWC_DIAG_UE_deletable(v)) return 1; // LN
  rotate_90_Z(v);
  if (MWC_DIAG_UE_deletable(v)) return 1; // LW
  rotate_90_Z(v);
  if (MWC_DIAG_UE_deletable(v)) return 1; // LS
  rotate_90_Z(v);
  swap_U_L(v);
  isometrieYZ_vois(v);
  if (MWC_DIAG_UE_deletable(v)) return 1;
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (MWC_DIAG_UE_deletable(v)) return 1;
  swap_U_L(v);
  if (MWC_DIAG_UE_deletable(v)) return 1;
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (MWC_DIAG_UE_deletable(v)) return 1;
  
  return 0;
} // MWC_DIAG_deletable()

#ifdef DEBUG_lmawanchangcurv2subfields2002
static void TEST_MWC_DIAG_deletable()
{
  int i; uint8_t v[27];  
  for (i = 0; i < 27; i++) v[i] = 10+i;
  (void)MWC_DIAG_deletable(v);
}
#endif

static void MWC_DIAG_preserve(uint8_t *v, index_t p, uint8_t *S, index_t rs, index_t ps)
{
  index_t q;
  //#define VARIANT_MWC_DIAG_preserve
#ifndef VARIANT_MWC_DIAG_preserve
  if (v[13] && !v[8] && !v[4]) // UE
  { q = return_vois(p, 13, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[11] && !v[8] && !v[2]) // UN
  { q = return_vois(p, 11, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[9] && !v[8] && !v[0])  // UW
  { q = return_vois(p, 9, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[15] && !v[8] && !v[6]) // US
  { q = return_vois(p, 15, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[1] && !v[2] && !v[0])  // NW
  { q = return_vois(p, 1, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[7] && !v[6] && !v[0])  // SW
  { q = return_vois(p, 7, rs, ps); SET_MWC_DIAGPRES(S[q]); }
#else
  if (v[22] && !v[17] && !v[4]) // LE
  { q = return_vois(p, 22, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[20] && !v[17] && !v[2]) // LN
  { q = return_vois(p, 20, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[18] && !v[17] && !v[0]) // LW
  { q = return_vois(p, 18, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[24] && !v[17] && !v[6]) // LS
  { q = return_vois(p, 24, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[5] && !v[4] && !v[6]) // SE
  { q = return_vois(p, 5, rs, ps); SET_MWC_DIAGPRES(S[q]); }
  if (v[3] && !v[4] && !v[2]) // NE
  { q = return_vois(p, 3, rs, ps); SET_MWC_DIAGPRES(S[q]); }
#endif
} // MWC_DIAG_preserve()

static int32_t MWC_TWIG_detect(  
  uint8_t *B,
  index_t x,
  index_t rs,
  index_t ps,
  index_t N)
{
  int32_t n = 0;
  index_t y = -1, z;
  if (((x<N-ps)&&(x%rs!=rs-1)) && B[ps+x+1]) { n++; if (y==-1) y=ps+x+1; else z=ps+x+1; }
  if (((x<N-ps)&&(x%rs!=rs-1)&&(x%ps>=rs)) && B[ps+x+1-rs]) { n++; if (y==-1) y=ps+x+1-rs; else z=ps+x+1-rs; }
  if (((x<N-ps)&&(x%ps>=rs)) && B[ps+x-rs]) { n++; if (y==-1) y=ps+x-rs; else z=ps+x-rs; }
  if (((x<N-ps)&&(x%ps>=rs)&&(x%rs!=0)) && B[ps+x-rs-1]) { n++; if (y==-1) y=ps+x-rs-1; else z=ps+x-rs-1; }
  if (((x<N-ps)&&(x%rs!=0)) && B[ps+x-1]) { n++; if (y==-1) y=ps+x-1; else z=ps+x-1; }
  if (((x<N-ps)&&(x%rs!=0)&&(x%ps<ps-rs)) && B[ps+x-1+rs]) { n++; if (y==-1) y=ps+x-1+rs; else z=ps+x-1+rs; }
  if (((x<N-ps)&&(x%ps<ps-rs)) && B[ps+x+rs]) { n++; if (y==-1) y=ps+x+rs; else z=ps+x+rs; }
  if (((x<N-ps)&&(x%ps<ps-rs)&&(x%rs!=rs-1)) && B[ps+x+rs+1]) { n++; if (y==-1) y=ps+x+rs+1; else z=ps+x+rs+1; }
  if (((x<N-ps)) && B[ps+x]) { n++; if (y==-1) y=ps+x; else z=ps+x; }
  if (((x%rs!=rs-1)) && B[x+1]) { n++; if (y==-1) y=x+1; else z=x+1; }
  if (((x%rs!=rs-1)&&(x%ps>=rs)) && B[x+1-rs]) { n++; if (y==-1) y=x+1-rs; else z=x+1-rs; }
  if (((x%ps>=rs)) && B[x-rs]) { n++; if (y==-1) y=x-rs; else z=x-rs; }
  if (((x%ps>=rs)&&(x%rs!=0)) && B[x-rs-1]) { n++; if (y==-1) y=x-rs-1; else z=x-rs-1; }
  if (((x%rs!=0)) && B[x-1]) { n++; if (y==-1) y=x-1; else z=x-1; }
  if (((x%rs!=0)&&(x%ps<ps-rs)) && B[x-1+rs]) { n++; if (y==-1) y=x-1+rs; else z=x-1+rs; }
  if (((x%ps<ps-rs)) && B[x+rs]) { n++; if (y==-1) y=x+rs; else z=x+rs; }
  if (((x%ps<ps-rs)&&(x%rs!=rs-1)) && B[x+rs+1]) { n++; if (y==-1) y=x+rs+1; else z=x+rs+1; }
  if (((x>=ps)&&(x%rs!=rs-1)) && B[-ps+x+1]) { n++; if (y==-1) y=-ps+x+1; else z=-ps+x+1; }
  if (((x>=ps)&&(x%rs!=rs-1)&&(x%ps>=rs)) && B[-ps+x+1-rs]) { n++; if (y==-1) y=-ps+x+1-rs; else z=-ps+x+1-rs; }
  if (((x>=ps)&&(x%ps>=rs)) && B[-ps+x-rs]) { n++; if (y==-1) y=-ps+x-rs; else z=-ps+x-rs; }
  if (((x>=ps)&&(x%ps>=rs)&&(x%rs!=0)) && B[-ps+x-rs-1]) { n++; if (y==-1) y=-ps+x-rs-1; else z=-ps+x-rs-1; }
  if (((x>=ps)&&(x%rs!=0)) && B[-ps+x-1]) { n++; if (y==-1) y=-ps+x-1; else z=-ps+x-1; }
  if (((x>=ps)&&(x%rs!=0)&&(x%ps<ps-rs)) && B[-ps+x-1+rs]) { n++; if (y==-1) y=-ps+x-1+rs; else z=-ps+x-1+rs; }
  if (((x>=ps)&&(x%ps<ps-rs)) && B[-ps+x+rs]) { n++; if (y==-1) y=-ps+x+rs; else z=-ps+x+rs; }
  if (((x>=ps)&&(x%ps<ps-rs)&&(x%rs!=rs-1)) && B[-ps+x+rs+1]) { n++; if (y==-1) y=-ps+x+rs+1; else z=-ps+x+rs+1; }
  if (((x>=ps)) && B[-ps+x]) { n++; if (y==-1) y=-ps+x; else z=-ps+x; }
  if (n == 1)
  {
    int32_t ny = 0;
    B[x] = 0;
    if (((y<N-ps)&&(y%rs!=rs-1)) && B[ps+y+1]) { ny++; z=ps+y+1; }
    if (((y<N-ps)&&(y%rs!=rs-1)&&(y%ps>=rs)) && B[ps+y+1-rs]) { ny++; z=ps+y+1-rs; }
    if (((y<N-ps)&&(y%ps>=rs)) && B[ps+y-rs]) { ny++; z=ps+y-rs; }
    if (((y<N-ps)&&(y%ps>=rs)&&(y%rs!=0)) && B[ps+y-rs-1]) { ny++; z=ps+y-rs-1; }
    if (((y<N-ps)&&(y%rs!=0)) && B[ps+y-1]) { ny++; z=ps+y-1; }
    if (((y<N-ps)&&(y%rs!=0)&&(y%ps<ps-rs)) && B[ps+y-1+rs]) { ny++; z=ps+y-1+rs; }
    if (((y<N-ps)&&(y%ps<ps-rs)) && B[ps+y+rs]) { ny++; z=ps+y+rs; }
    if (((y<N-ps)&&(y%ps<ps-rs)&&(y%rs!=rs-1)) && B[ps+y+rs+1]) { ny++; z=ps+y+rs+1; }
    if (((y<N-ps)) && B[ps+y]) { ny++; z=ps+y; }
    if (((y%rs!=rs-1)) && B[y+1]) { ny++; z=y+1; }
    if (((y%rs!=rs-1)&&(y%ps>=rs)) && B[y+1-rs]) { ny++; z=y+1-rs; }
    if (((y%ps>=rs)) && B[y-rs]) { ny++; z=y-rs; }
    if (((y%ps>=rs)&&(y%rs!=0)) && B[y-rs-1]) { ny++; z=y-rs-1; }
    if (((y%rs!=0)) && B[y-1]) { ny++; z=y-1; }
    if (((y%rs!=0)&&(y%ps<ps-rs)) && B[y-1+rs]) { ny++; z=y-1+rs; }
    if (((y%ps<ps-rs)) && B[y+rs]) { ny++; z=y+rs; }
    if (((y%ps<ps-rs)&&(y%rs!=rs-1)) && B[y+rs+1]) { ny++; z=y+rs+1; }
    if (((y>=ps)&&(y%rs!=rs-1)) && B[-ps+y+1]) { ny++; z=-ps+y+1; }
    if (((y>=ps)&&(y%rs!=rs-1)&&(y%ps>=rs)) && B[-ps+y+1-rs]) { ny++; z=-ps+y+1-rs; }
    if (((y>=ps)&&(y%ps>=rs)) && B[-ps+y-rs]) { ny++; z=-ps+y-rs; }
    if (((y>=ps)&&(y%ps>=rs)&&(y%rs!=0)) && B[-ps+y-rs-1]) { ny++; z=-ps+y-rs-1; }
    if (((y>=ps)&&(y%rs!=0)) && B[-ps+y-1]) { ny++; z=-ps+y-1; }
    if (((y>=ps)&&(y%rs!=0)&&(y%ps<ps-rs)) && B[-ps+y-1+rs]) { ny++; z=-ps+y-1+rs; }
    if (((y>=ps)&&(y%ps<ps-rs)) && B[-ps+y+rs]) { ny++; z=-ps+y+rs; }
    if (((y>=ps)&&(y%ps<ps-rs)&&(y%rs!=rs-1)) && B[-ps+y+rs+1]) { ny++; z=-ps+y+rs+1; }
    if (((y>=ps)) && B[-ps+y]) { ny++; z=-ps+y; }
    B[x] = MWC_OBJECT;
    if (ny >= 2) return 1;
    if ((ny == 1) && (mctopo3d_nbvoiso26(B, z, rs, ps, N) >= 3)) return 1;
    return 0;
  } // if (n == 1)
  else if ((n == 2) && voisins26(y, z, rs, ps))
  {
    if (mctopo3d_nbvoiso26(B, y, rs, ps, N) >= 3) return 1;
    if (mctopo3d_nbvoiso26(B, z, rs, ps, N) >= 3) return 1;
  }
  return 0;
} // MWC_TWIG_detect()

/* ==================================== */
int32_t lmawanchangcurv2subfields2002(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lmawanchangcurv2subfields2002"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab[2];
  uint8_t v[27];

#ifdef DEBUG_lmawanchangcurv2subfields2002
  TEST_MWC_DIAG_deletable();
#endif

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#define INITSTEP -1
  //#define INITSTEP 0
  step = INITSTEP;
  nonstab[0] = nonstab[1] = 1;
  while ((nonstab[0] || nonstab[1]) && (step < nsteps+INITSTEP))
  {
    step++;
    nonstab[step%2] = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = MWC_OBJECT;

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (S[x] && (mctopo3d_nbvoisc6(S, x, rs, ps, N) >= 1))
	{
#ifdef DEBUG_lmawanchangcurv2subfields2002
printf("point %d %d %d\n", i, j, k);	  
#endif
	  extract_vois(S, x, rs, ps, N, v);
	  if (MWC_ORTH_deletable(v)) SET_MWC_ORTH(S[x]);
	  if (MWC_DIAG_deletable(v)) SET_MWC_DIAG(S[x]);
	  if (MWC_TWIG_detect(S, x, rs, ps, N)) SET_MWC_TWIG(S[x]);
	} // if (S[x])
      } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (S[x] && IS_MWC_DIAG(S[x]))
	{
	  extract_vois(S, x, rs, ps, N, v);
	  MWC_DIAG_preserve(v, x, S, rs, ps);
	} // if (S[x])
      } // for i, j, k

    //if (step%2) writeimage(image, "_S1"); else writeimage(image, "_S0");

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (IS_MWC_ORTH(S[x]) || (IS_MWC_DIAG(S[x]) && !IS_MWC_DIAGPRES(S[x])) || IS_MWC_TWIG(S[x]))
	  {
	    S[x] = 0;
	    nonstab[step%2] = 1;
	  }
      } // for i, j, k

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  return(1);
} /* lmawanchangcurv2subfields2002() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Ma, Wan & Lee (curviligne 4 subfields, PAMI 2002)
// M. Couprie, feb. 2012
/* ============================================================ */
/* ============================================================ */

#define MWL_OBJECT      1
#define MWL_ORTH        2
#define MWL_DIAG        4
#define MWL_WEAKEND     8
#define MWL_UL_END     16

#define IS_MWL_ORTH(f) (f&MWL_ORTH)
#define SET_MWL_ORTH(f) (f|=MWL_ORTH)
#define IS_MWL_DIAG(f) (f&MWL_DIAG)
#define SET_MWL_DIAG(f) (f|=MWL_DIAG)
//#define UNSET_MWL_DIAG(f) (f&=~MWL_DIAG)
#define IS_MWL_WEAKEND(f) (f&MWL_WEAKEND)
#define SET_MWL_WEAKEND(f) (f|=MWL_WEAKEND)
#define IS_MWL_UL_END(f) (f&MWL_UL_END)
#define SET_MWL_UL_END(f) (f|=MWL_UL_END)

static int32_t MWL_ORTH_U_deletable(uint8_t *v)
{
  if (v[8]) return 0;
  if (!v[17]) return 0;
  if (v[9] && !v[0]) return 0;
  if (v[11] && !v[2]) return 0;
  if (v[13] && !v[4]) return 0;
  if (v[15] && !v[6]) return 0;
  if (v[16] && !v[0] && !v[6] && !v[7]) return 0;
  if (v[10] && !v[0] && !v[1] && !v[2]) return 0;
  if (v[12] && !v[2] && !v[3] && !v[4]) return 0;
  if (v[14] && !v[4] && !v[5] && !v[6]) return 0;
  return 1;
} // MWL_ORTH_U_deletable()

static int32_t MWL_ORTH_deletable(uint8_t *v)
{
  if (MWL_ORTH_U_deletable(v)) // U-deletable
  {
     return 1;
  }
  swap_U_L(v);
  if (MWL_ORTH_U_deletable(v)) // L-deletable
  {
     return 1;
  }

  isometrieXZ_vois(v);
  if (MWL_ORTH_U_deletable(v)) // E-deletable
  {
     return 1;
  }
  swap_U_L(v);
  if (MWL_ORTH_U_deletable(v)) // W-deletable
  {
     return 1;
  }
  isometrieXZ_vois(v);

  isometrieYZ_vois(v);
  if (MWL_ORTH_U_deletable(v)) // N-deletable
  {
     return 1;
  }
  swap_U_L(v);
  if (MWL_ORTH_U_deletable(v)) // S-deletable
  {
     return 1;
  }
  isometrieYZ_vois(v);

  return 0;
} // MWL_ORTH_deletable()

static int32_t MWL_DIAG_UE_deletable(uint8_t *v)
{
  if (!v[22]) return 0;
  if (v[4] || v[13] || v[8] || v[9] || v[0] || v[18] || v[17]) return 0;

  if (!v[6] && v[15] && v[7]) return 0; // a1
  if (!v[2] && v[11] && v[1]) return 0; // a2

  return 1;
} // MWL_DIAG_UE_deletable()

static int32_t MWL_DIAG_deletable(uint8_t *v)
{
  if (MWL_DIAG_UE_deletable(v)) // UE
  {
    return 1;
  }
  rotate_90_Z(v);
  if (MWL_DIAG_UE_deletable(v)) // UN
  {
    return 1;
  }
  rotate_90_Z(v);
  if (MWL_DIAG_UE_deletable(v)) // UW
  {
    return 1;
  }
  rotate_90_Z(v);
  if (MWL_DIAG_UE_deletable(v)) // US
  {
    return 1;
  }
  rotate_90_Z(v);
  swap_U_L(v);
  if (MWL_DIAG_UE_deletable(v)) // LE
  {
    return 1;
  }
  rotate_90_Z(v);
  if (MWL_DIAG_UE_deletable(v)) // LN
  {
    return 1;
  }
  rotate_90_Z(v);
  if (MWL_DIAG_UE_deletable(v)) // LW
  {
    return 1;
  }
  rotate_90_Z(v);
  if (MWL_DIAG_UE_deletable(v)) // LS
  {
    return 1;
  }
  rotate_90_Z(v);
  swap_U_L(v);
  isometrieYZ_vois(v);
  if (MWL_DIAG_UE_deletable(v))
  {
    return 1;
  }
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (MWL_DIAG_UE_deletable(v))
  {
    return 1;
  }
  swap_U_L(v);
  if (MWL_DIAG_UE_deletable(v))
  {
    return 1;
  }
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (MWL_DIAG_UE_deletable(v))
  {
    return 1;
  }
  
  return 0;
} // MWL_DIAG_deletable()

static void MWL_copysliceW(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[3] = v[3]; w[4] = v[4]; w[5] = v[5];
  w[12] = v[12]; w[13] = v[13]; w[14] = v[14];
  w[21] = v[21]; w[22] = v[22]; w[23] = v[23];
} // MWL_copysliceW()

static void MWL_copysliceE(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[1] = v[1]; w[0] = v[0]; w[7] = v[7];
  w[10] = v[10]; w[9] = v[9]; w[16] = v[16];
  w[19] = v[19]; w[18] = v[18]; w[25] = v[25];
} // MWL_copysliceE()

static void MWL_copysliceU(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[12] = v[12]; w[11] = v[11]; w[10] = v[10];
  w[13] = v[13]; w[8] = v[8]; w[9] = v[9];
  w[14] = v[14]; w[15] = v[15]; w[16] = v[16];
} // MWL_copysliceU()

static void MWL_copysliceL(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[21] = v[21]; w[20] = v[20]; w[19] = v[19];
  w[22] = v[22]; w[17] = v[17]; w[18] = v[18];
  w[23] = v[23]; w[24] = v[24]; w[25] = v[25];
} // MWL_copysliceL()

static void MWL_copysliceN(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[12] = v[12]; w[11] = v[11]; w[10] = v[10];
  w[3] = v[3]; w[2] = v[2]; w[1] = v[1];
  w[21] = v[21]; w[20] = v[20]; w[19] = v[19];
} // MWL_copysliceN()

static void MWL_copysliceS(uint8_t *v, uint8_t *w)
{
  memset(w, 0, 27);
  w[14] = v[14]; w[15] = v[15]; w[16] = v[16];
  w[5] = v[5]; w[6] = v[6]; w[7] = v[7];
  w[23] = v[23]; w[24] = v[24]; w[25] = v[25];
} // MWL_copysliceS()

static int32_t MWL_NbPtVois(uint8_t *v)
{
  int32_t i, n = 0;
  for (i = 0; i < 26; i++) if (v[i]) n++;
  return n;
}

static int32_t MWL_WeaklyEnd(uint8_t *v)
{
  uint8_t w[27];
  int32_t n = MWL_NbPtVois(v);
  if (n == 2) return 1;
  if (n == 3)
  {
    MWL_copysliceW(v, w); if (MWL_NbPtVois(w) == 3) return 1;
    MWL_copysliceE(v, w); if (MWL_NbPtVois(w) == 3) return 1;
    MWL_copysliceU(v, w); if (MWL_NbPtVois(w) == 3) return 1;
    MWL_copysliceL(v, w); if (MWL_NbPtVois(w) == 3) return 1;
    MWL_copysliceN(v, w); if (MWL_NbPtVois(w) == 3) return 1;
    MWL_copysliceS(v, w); if (MWL_NbPtVois(w) == 3) return 1;
    return 0;
  }
  return 0;
} // MWL_WeaklyEnd()

static int32_t MWL_UpperEnd(uint8_t *v)
{
  int32_t i, n=0, q;
  for (i = 0; i < 26; i++) if (v[i]) { q = i; n++; }
  if (n != 1) return 0;
  if ((q==19) || (q==21) || (q==23) || (q==25)) return 0;
  return 1;
} // MWL_UpperEnd()

static int32_t MWL_LowerEnd(uint8_t *v)
{
  int32_t i, n=0, q;
  for (i = 0; i < 26; i++) if (v[i]) { q = i; n++; }
  if (n != 1) return 0;
  if ((q==10) || (q==12) || (q==14) || (q==16)) return 0;
  return 1;
} // MWL_LowerEnd()

/* ==================================== */
int32_t lmawanleecurv4subfields2002(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lmawanleecurv4subfields2002"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab[4];
  uint8_t v[27];
  int32_t top, topb;

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 100000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab[0] = nonstab[1] = nonstab[2] = nonstab[3] = 1;
  while ((nonstab[0] || nonstab[1] || nonstab[2] || nonstab[3]) && 
	 (step < 4*nsteps))
  {
    nonstab[step%4] = 0;
#ifdef VERBOSE
    printf("step %d, substep %d\n", step/4, step%4);
#endif
#ifdef DEBUG_lmawanleecurv4subfields2002
    char buf[128];
    sprintf(buf,"_begiter%d",step);
    writeimage(image, buf);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = MWL_OBJECT;

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (step%2)) && (((k+j)%2) == ((step%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (S[x] && (mctopo3d_nbvoiso26(S, x, rs, ps, N) > 1))
	{
#ifdef DEBUG_lmawanleecurv4subfields2002
printf("point %d %d %d\n", i, j, k);	  
#endif
	  extract_vois(S, x, rs, ps, N, v);
	  if (MWL_ORTH_deletable(v))
	  { 
	    SET_MWL_ORTH(S[x]);
#ifdef DEBUG_lmawanleecurv4subfields2002
	    printf("set ORTH\n");	  	  
#endif
	  }
	  mctopo3d_top26(S, x, rs, ps, N, &top, &topb);
	  if ((top == 1) && MWL_DIAG_deletable(v))
	  {
	    SET_MWL_DIAG(S[x]);
#ifdef DEBUG_lmawanleecurv4subfields2002
	    printf("set DIAG\n");	  	  
#endif
	  }
	} // if (S[x])
      } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (step%2)) && (((k+j)%2) == ((step%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (S[x] && (IS_MWL_ORTH(S[x]) || IS_MWL_DIAG(S[x])))
	{
#ifdef DEBUG_lmawanleecurv4subfields2002
	  printf("deleting point %d %d %d\n", i, j, k);
	  if (!mctopo3d_simple26(S, x, rs, ps, N))
	  {
	    printf("ERROR: simple point\n");
	    exit(0);
	  }
#endif
	  S[x] = 0;
	  nonstab[step%4] = 1;
	} // if (S[x])
      } // for i, j, k
    step++;
  } // while (nonstab && (step < nsteps))

#define PASS2
#ifdef PASS2
  // SECOND PASS - "CLEANING"
  nsteps = (int32_t)ceilf(((float)step)/2);
  step = 0;
  nonstab[0] = nonstab[1] = nonstab[2] = nonstab[3] = 1;
  while ((nonstab[0] || nonstab[1] || nonstab[2] || nonstab[3]) && 
	 (step < nsteps))
  {
    nonstab[step%4] = 0;
#ifdef VERBOSE
    printf("2nd pass: step %d, substep %d\n", step/4, step%4);
#endif
#ifdef DEBUG_lmawanleecurv4subfields2002
    char buf[128];
    sprintf(buf,"_begp2iter%d",step);
    writeimage(image, buf);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = MWL_OBJECT;

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (step%2)) && (((k+j)%2) == ((step%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (S[x] && (mctopo3d_nbvoiso26(S, x, rs, ps, N) > 1))
	{
#ifdef DEBUG_lmawanleecurv4subfields2002
printf("point %d %d %d\n", i, j, k);	  
#endif
	  extract_vois(S, x, rs, ps, N, v);
	  if (MWL_WeaklyEnd(v) && mctopo3d_simple26(S, x, rs, ps, N)) 
	    SET_MWL_WEAKEND(S[x]);
	  if (((step%2) == 0) && MWL_UpperEnd(v)) SET_MWL_UL_END(S[x]);
	  if (((step%2) == 1) && MWL_LowerEnd(v)) SET_MWL_UL_END(S[x]);
	} // if (S[x])
      } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (step%2)) && (((k+j)%2) == ((step%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (S[x] && (IS_MWL_WEAKEND(S[x]) || IS_MWL_UL_END(S[x])))
	{
	  S[x] = 0;
	  nonstab[step%4] = 1;
	} // if (S[x])
      } // for i, j, k
    step++;
  } // while (nonstab && (step < nsteps))
#endif

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  mctopo3d_termine_topo3d();
  return(1);
} /* lmawanleecurv4subfields2002() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Nemeth, Kardos & Palagyi (curviligne 2 subfields, IASTED 2010)
// M. Couprie, feb. 2012
/* ============================================================ */
/* ============================================================ */

#define NKP_OBJECT      1
#define NKP_SELF        2
#define NKP_SQUARE      4
#define NKP_CUBE        8
#define NKP_END        16

#define IS_NKP_SELF(f) (f&NKP_SELF)
#define SET_NKP_SELF(f) (f|=NKP_SELF)
#define UNSET_NKP_SELF(f)  (f&=~NKP_SELF)
#define IS_NKP_SQUARE(f) (f&NKP_SQUARE)
#define SET_NKP_SQUARE(f) (f|=NKP_SQUARE)
#define IS_NKP_CUBE(f) (f&NKP_CUBE)
#define SET_NKP_CUBE(f) (f|=NKP_CUBE)
#define IS_NKP_END(f) (f&NKP_END)
#define SET_NKP_END(f) (f|=NKP_END)
#define UNSET_NKP_END(f)  (f&=~NKP_END)

static int32_t NKP_end(uint8_t *S, index_t p, index_t rs, index_t ps, index_t N, int32_t mode)
{
  if (mode == ISTHMUS) 
  // cette condition est la condition C1 du papier IASTED
  {
    int32_t top, topbar;
    mctopo3d_top26(S, p, rs, ps, N, &top, &topbar);
    if (top > 1) return 1;
    return 0;
  } // if (mode == 0)
  if (mode == ENDS) 
  // cette condition est la condition C1 du papier ICIAR 2010
  {
    if (mctopo3d_nbvoiso26(S, p, rs, ps, N) == 1) return 1;
    return 0;
  } // if (mode == 1)
  if (mode == 2)
  // cette condition est la condition C2 des papiers ICIAR et IASTED
  {
    int32_t k, n=0;
    index_t y, q;
    for (k = 0; k < 26; k += 1)
    {
      y = voisin26(p, k, rs, ps, N);
      if ((y != -1) && S[y]) { n++; q = y; }
    } // for k
    if (n != 1) return 0;
    if (mctopo3d_nbvoiso26(S, q, rs, ps, N) > 2) return 0;
    return 1;
  } // if (mode == 2)
  if (mode == 3)
  // cette condition est la condition C3 des papiers ICIAR et IASTED
  {
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
  } // if (mode == 3)
  assert(1);
  return 0;
} //NKP_end()

static int32_t NKP_square_deletable(uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
{
  int32_t k;
  index_t q;
  if (!IS_NKP_SELF(S[p])) return 0;
  for (k = 0; k < 12; k += 1)
  {
    q = voisin12(p, k, rs, ps, N);
    if ((q != -1) && S[q]  && IS_NKP_SELF(S[q]))
    {
      if (!mctopo3d_simplepair26(S, p, q, rs, ps, N)) return 0;
    }
  } // for k
  return 1;
} //NKP_square_deletable()

#ifdef NKP_CHECK_CUBES
/* ==================================== */
static int32_t NKP_match0(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[17])) return 0;
  if (!IS_NKP_SQUARE(v[23])) return 0;
  if (IS_NKP_SQUARE(v[14])) return 0;
  if (IS_NKP_SQUARE(v[16])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[25])) return 0;
  if (IS_NKP_SQUARE(v[26])) return 0;
  return 1;
} // NKP_match0()

/* ==================================== */
static int32_t NKP_match1(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[17])) return 0;
  if (!IS_NKP_SQUARE(v[25])) return 0;
  if (IS_NKP_SQUARE(v[14])) return 0;
  if (IS_NKP_SQUARE(v[16])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[23])) return 0;
  if (IS_NKP_SQUARE(v[26])) return 0;
  return 1;
} // NKP_match1()

/* ==================================== */
static int32_t NKP_match2(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[23])) return 0;
  if (!IS_NKP_SQUARE(v[25])) return 0;
  if (IS_NKP_SQUARE(v[14])) return 0;
  if (IS_NKP_SQUARE(v[16])) return 0;
  if (IS_NKP_SQUARE(v[17])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[26])) return 0;
  return 1;
} // NKP_match2()

/* ==================================== */
static int32_t NKP_match3(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[17])) return 0;
  if (!IS_NKP_SQUARE(v[23])) return 0;
  if (!IS_NKP_SQUARE(v[25])) return 0;
  if (IS_NKP_SQUARE(v[14])) return 0;
  if (IS_NKP_SQUARE(v[16])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[26])) return 0;
  return 1;
} // NKP_match3()

/* ==================================== */
static int32_t NKP_match4(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[19])) return 0;
  if (!IS_NKP_SQUARE(v[21])) return 0;
  if (IS_NKP_SQUARE(v[10])) return 0;
  if (IS_NKP_SQUARE(v[9])) return 0;
  if (IS_NKP_SQUARE(v[12])) return 0;
  if (IS_NKP_SQUARE(v[18])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  return 1;
} // NKP_match4()

/* ==================================== */
static int32_t NKP_match5(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[15])) return 0;
  if (!IS_NKP_SQUARE(v[21])) return 0;
  if (IS_NKP_SQUARE(v[12])) return 0;
  if (IS_NKP_SQUARE(v[16])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[24])) return 0;
  if (IS_NKP_SQUARE(v[25])) return 0;
  return 1;
} // NKP_match5()

/* ==================================== */
static int32_t NKP_match6(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[15])) return 0;
  if (!IS_NKP_SQUARE(v[25])) return 0;
  if (IS_NKP_SQUARE(v[12])) return 0;
  if (IS_NKP_SQUARE(v[16])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[24])) return 0;
  if (IS_NKP_SQUARE(v[21])) return 0;
  return 1;
} // NKP_match6()

/* ==================================== */
static int32_t NKP_match7(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[25])) return 0;
  if (!IS_NKP_SQUARE(v[21])) return 0;
  if (IS_NKP_SQUARE(v[12])) return 0;
  if (IS_NKP_SQUARE(v[16])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[24])) return 0;
  if (IS_NKP_SQUARE(v[15])) return 0;
  return 1;
} // NKP_match7()

/* ==================================== */
static int32_t NKP_match8(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[15])) return 0;
  if (!IS_NKP_SQUARE(v[21])) return 0;
  if (!IS_NKP_SQUARE(v[25])) return 0;
  if (IS_NKP_SQUARE(v[12])) return 0;
  if (IS_NKP_SQUARE(v[16])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[24])) return 0;
  return 1;
} // NKP_match8()

/* ==================================== */
static int32_t NKP_match9(uint8_t *v)
/* ==================================== */
{
  if (!IS_NKP_SQUARE(v[13])) return 0;
  if (!IS_NKP_SQUARE(v[19])) return 0;
  if (!IS_NKP_SQUARE(v[23])) return 0;
  if (IS_NKP_SQUARE(v[10])) return 0;
  if (IS_NKP_SQUARE(v[11])) return 0;
  if (IS_NKP_SQUARE(v[14])) return 0;
  if (IS_NKP_SQUARE(v[22])) return 0;
  if (IS_NKP_SQUARE(v[20])) return 0;
  return 1;
} // NKP_match9()

static int32_t NKP_cube_deletable(uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
{
  // CAUTION: incorrect in this version. Must be applied only to small (2x2x2) connected components.

  uint8_t v[27];
  extract_vois27(S, p, rs, ps, N, v);
  if (NKP_match0(v)) return 0;
  if (NKP_match1(v)) return 0;
  if (NKP_match2(v)) return 0;
  if (NKP_match3(v)) return 0;
  if (NKP_match4(v)) return 0;
  if (NKP_match5(v)) return 0;
  if (NKP_match6(v)) return 0;
  if (NKP_match7(v)) return 0;
  if (NKP_match8(v)) return 0;
  if (NKP_match9(v)) return 0;
  return 1;
} //NKP_cube_deletable()
#endif

/* ==================================== */
int32_t lnemethetalcurv2subfields2010(
				  struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode)
/* ==================================== */
#undef F_NAME
#define F_NAME "lnemethetalcurv2subfields2010"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab[2];

  printf("%s: WARNING: cube-deletable test not implemented (necessary only for small (2x2x2) connected components) \n", F_NAME);

#ifdef VERBOSE
  printf("%s: nsteps = %d, mode = %d\n", F_NAME, nsteps, mode);
#endif

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab[0] = nonstab[1] = 1;
  while ((nonstab[0] || nonstab[1]) && (step < nsteps))
  {
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = NKP_OBJECT;

    nonstab[step%2] = 0;

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (S[x] && NKP_end(S, x, rs, ps, N, mode)) SET_NKP_END(S[x]);
	if (S[x] && mctopo3d_simple26(S, x, rs, ps, N) && !IS_NKP_END(S[x]))
	{
#ifdef DEBUG_lnemethetalcurv2subfields2010
printf("mark self point %d %d %d\n", i, j, k);	  
#endif
	  SET_NKP_SELF(S[x]);
	}
      } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (S[x] && NKP_square_deletable(S, x, rs, ps, N))
	{
#ifdef DEBUG_lnemethetalcurv2subfields2010
printf("mark square point %d %d %d\n", i, j, k);	  
#endif
	  SET_NKP_SQUARE(S[x]);
	}
      } // for i, j, k

#ifdef NKP_CHECK_CUBES
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (S[x] && IS_NKP_SQUARE(S[x]) && NKP_cube_deletable(S, x, rs, ps, N))
	{
#ifdef DEBUG_lnemethetalcurv2subfields2010
printf("mark cube point %d %d %d\n", i, j, k);	  
#endif
	  SET_NKP_CUBE(S[x]);
	}
      } // for i, j, k
#endif

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
#ifdef NKP_CHECK_CUBES
	if (IS_NKP_SELF(S[x]) && IS_NKP_SQUARE(S[x]) && IS_NKP_CUBE(S[x]))
#else
	if (IS_NKP_SELF(S[x]) && IS_NKP_SQUARE(S[x]))
#endif
	{
	  S[x] = 0;
	  nonstab[step%2] = 1;
	}
      } // for i, j, k
    step++;
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  mctopo3d_termine_topo3d();
  return(1);
} /* lnemethetalcurv2subfields2010() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Nemeth, Kardos & Palagyi (curviligne 4 and 8 subfields, ICIAR 2010)
// M. Couprie, feb. 2012
/* ============================================================ */
/* ============================================================ */

#define NKP_4DELETABLE 4
#define NKP_8DELETABLE 4
#define NKP_BORDNONEND 8

#define IS_NKP_4DELETABLE(f) (f&NKP_4DELETABLE)
#define SET_NKP_4DELETABLE(f) (f|=NKP_4DELETABLE)
#define UNSET_NKP_4DELETABLE(f)  (f&=~NKP_4DELETABLE)
#define IS_NKP_8DELETABLE(f) (f&NKP_8DELETABLE)
#define SET_NKP_8DELETABLE(f) (f|=NKP_8DELETABLE)
#define UNSET_NKP_8DELETABLE(f)  (f&=~NKP_8DELETABLE)
#define IS_NKP_BORDNONEND(f) (f&NKP_BORDNONEND)
#define SET_NKP_BORDNONEND(f) (f|=NKP_BORDNONEND)
#define UNSET_NKP_BORDNONEND(f)  (f&=~NKP_BORDNONEND)

static int32_t NKP_4_deletable(uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
{
  int32_t k;
  index_t q;
  if (!IS_NKP_SELF(S[p])) return 0;
  if (mctopo3d_nbvoiso26(S, p, rs, ps, N) != 1) return 1;
  for (k = 0; k < 26; k += 1)
  {
    q = voisin26(p, k, rs, ps, N);
    if ((q != -1) && S[q]  && !voisins18(p, q, rs, ps) && 
	(mctopo3d_nbvoiso26(S, q, rs, ps, N) == 1) &&
	IS_NKP_SELF(S[q]))
    {
      if (p < q) return 0;
    }
  } // for k
  return 1;
} //NKP_4_deletable()

/* ==================================== */
int32_t lnemethetalcurv4subfields2010(
				  struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode)
/* ==================================== */
#undef F_NAME
#define F_NAME "lnemethetalcurv4subfields2010"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab[4];

#ifdef VERBOSE
  printf("%s: mode %d\n", F_NAME, mode);
#endif

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = NKP_OBJECT;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab[0] = nonstab[1] = nonstab[2] = nonstab[3] = 1;
  while ((nonstab[0] || nonstab[1] || nonstab[2] || nonstab[3]) && (step < nsteps))
  {
    nonstab[step%4] = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
    {
      x = k*ps + j*rs + i;
      if (S[x])
      {
	UNSET_NKP_SELF(S[x]);
	UNSET_NKP_4DELETABLE(S[x]);
	if ((step%4) == 0)
	{
	  if (mode != 0) UNSET_NKP_END(S[x]);
	  if (NKP_end(S, x, rs, ps, N, mode)) SET_NKP_END(S[x]);
	  UNSET_NKP_BORDNONEND(S[x]);
	  if (S[x] && (mctopo3d_nbvoisc6(S, x, rs, ps, N) > 0) && 
	      !IS_NKP_END(S[x]))
	  {
#ifdef DEBUG_lnemethetalcurv4subfields2010
	    printf("mark bord non end %d %d %d\n", i, j, k);	  
#endif
	    SET_NKP_BORDNONEND(S[x]);
	  }
	} // if ((step%4) == 0)
      } // if (S[x])
    } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (step%2)) && (((k+j)%2) == ((step%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (S[x] && mctopo3d_simple26(S, x, rs, ps, N))
	{
#ifdef DEBUG_lnemethetalcurv4subfields2010
printf("mark self %d %d %d\n", i, j, k);	  
#endif
	  SET_NKP_SELF(S[x]);
	}
      } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (step%2)) && (((k+j)%2) == ((step%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (S[x] && NKP_4_deletable(S, x, rs, ps, N))
	{
#ifdef DEBUG_lnemethetalcurv4subfields2010
printf("mark 4 deletable %d %d %d\n", i, j, k);	  
#endif
	  SET_NKP_4DELETABLE(S[x]);
	}
      } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (step%2)) && (((k+j)%2) == ((step%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (IS_NKP_BORDNONEND(S[x]) && IS_NKP_4DELETABLE(S[x]))
	{
#ifdef DEBUG_lnemethetalcurv4subfields2010
printf("delete %d %d %d\n", i, j, k);	  
#endif
	  S[x] = 0;
	  nonstab[step%4] = 1;
	}
      } // for i, j, k
    step++;
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  mctopo3d_termine_topo3d();
  return(1);
} /* lnemethetalcurv4subfields2010() */

/* ==================================== */
int32_t lnemethetalcurv8subfields2010(
				  struct xvimage *image,
				  int32_t nsteps,
				  int32_t mode)
/* ==================================== */
#undef F_NAME
#define F_NAME "lnemethetalcurv8subfields2010"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab[8];

#ifdef VERBOSE
  printf("%s: mode %d\n", F_NAME, mode);
#endif

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;
  
  for (i = 0; i < N; i++) if (S[i]) S[i] = NKP_OBJECT;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab[0] = nonstab[1] = nonstab[2] = nonstab[3] = 1;
  nonstab[4] = nonstab[5] = nonstab[6] = nonstab[7] = 1;
  while ((nonstab[0] || nonstab[1] || nonstab[2] || nonstab[3] ||
	  nonstab[4] || nonstab[5] || nonstab[6] || nonstab[7]) && (step < nsteps))
  {
    nonstab[step%8] = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
    {
      x = k*ps + j*rs + i;
      if (S[x])
      {
	UNSET_NKP_8DELETABLE(S[x]);
	if ((step%8) == 0)
	{
	  if (mode != 0) UNSET_NKP_END(S[x]);
	  if (NKP_end(S, x, rs, ps, N, mode)) SET_NKP_END(S[x]);
	  UNSET_NKP_BORDNONEND(S[x]);
	  if (S[x] && (mctopo3d_nbvoisc6(S, x, rs, ps, N) > 0) && 
	      !IS_NKP_END(S[x]))
	  {
#ifdef DEBUG_lnemethetalcurv8subfields2010
	    printf("mark bord non end %d %d %d\n", i, j, k);	  
#endif
	    SET_NKP_BORDNONEND(S[x]);
	  }
	} // if ((step%8) == 0)
      } // if (S[x])
    } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((i%2) == (step%2)) && ((j%2) == ((step>>1)%2)) && ((k%2) == ((step>>2)%2)))
      {
	x = k*ps + j*rs + i;
	if (S[x] && mctopo3d_simple26(S, x, rs, ps, N))
	{
#ifdef DEBUG_lnemethetalcurv8subfields2010
printf("mark 8 deletable %d %d %d\n", i, j, k);	  
#endif
	  SET_NKP_8DELETABLE(S[x]);
	}
      } // for i, j, k

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((i%2) == (step%2)) && ((j%2) == ((step>>1)%2)) && ((k%2) == ((step>>2)%2)))
      {
	x = k*ps + j*rs + i;
	if (IS_NKP_BORDNONEND(S[x]) && IS_NKP_8DELETABLE(S[x]))
	{
#ifdef DEBUG_lnemethetalcurv8subfields2010
printf("delete %d %d %d\n", i, j, k);	  
#endif
	  S[x] = 0;
	  nonstab[step%8] = 1;
	}
      } // for i, j, k
    step++;
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  mctopo3d_termine_topo3d();
  return(1);
} /* lnemethetalcurv8subfields2010() */

/* ============================================================ */
/* ============================================================ */
// Algo. de She et al. (curviligne symétrique, DICTA 2009)
// M. Couprie, feb. 2012
/* ============================================================ */
/* ============================================================ */

#define SHE_OBJECT      1
#define SHE_MARK        2
#define SHE_DELETE      4

#define IS_SHE_MARK(f) (f&SHE_MARK)
#define SET_SHE_MARK(f) (f|=SHE_MARK)
#define UNSET_SHE_MARK(f)  (f&=~SHE_MARK)
#define IS_SHE_DELETE(f) (f&SHE_DELETE)
#define SET_SHE_DELETE(f) (f|=SHE_DELETE)

static int32_t SHE_mask_A(uint8_t *v)
{
  if (!v[17]) return 0;
  if (v[8] || v[9] || v[10] || v[11] || v[12] || v[13] || v[14] || v[15] || v[16]) return 0;
  return 1;
} // SHE_mask_A()

static int32_t SHE_deletable_A(uint8_t *v)
{
  if (SHE_mask_A(v)) return 1; // U-deletable
  swap_U_L(v);
  if (SHE_mask_A(v)) return 1; // L-deletable

  isometrieXZ_vois(v);
  if (SHE_mask_A(v)) return 1; // E-deletable
  swap_U_L(v);
  if (SHE_mask_A(v)) return 1; // W-deletable
  isometrieXZ_vois(v);

  isometrieYZ_vois(v);
  if (SHE_mask_A(v)) return 1; // N-deletable
  swap_U_L(v);
  if (SHE_mask_A(v)) return 1; // S-deletable
  isometrieYZ_vois(v);

  return 0;
} // SHE_deletable_A()

static int32_t SHE_mask_B(uint8_t *v)
{
  if (!v[22]) return 0;
  if (v[4] || v[13] || v[8] || v[9] || v[0] || v[18] || v[17]) return 0;
  return 1;
} // SHE_mask_B()

static int32_t SHE_deletable_B(uint8_t *v)
{
  if (SHE_mask_B(v)) return 1; // UE
  rotate_90_Z(v);
  if (SHE_mask_B(v)) return 1; // UN
  rotate_90_Z(v);
  if (SHE_mask_B(v)) return 1; // UW
  rotate_90_Z(v);
  if (SHE_mask_B(v)) return 1; // US
  rotate_90_Z(v);
  swap_U_L(v);
  if (SHE_mask_B(v)) return 1; // LE
  rotate_90_Z(v);
  if (SHE_mask_B(v)) return 1; // LN
  rotate_90_Z(v);
  if (SHE_mask_B(v)) return 1; // LW
  rotate_90_Z(v);
  if (SHE_mask_B(v)) return 1; // LS
  rotate_90_Z(v);
  swap_U_L(v);
  isometrieYZ_vois(v);
  if (SHE_mask_B(v)) return 1;
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (SHE_mask_B(v)) return 1;
  swap_U_L(v);
  if (SHE_mask_B(v)) return 1;
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (SHE_mask_B(v)) return 1;
  
  return 0;
} // SHE_deletable_B()

static int32_t SHE_mask_D(uint8_t *v)
{
  if (!v[4] || !v[17]) return 0;
  if (v[15] || v[11] || v[8] || v[9] || v[10] || v[16] || v[1] || v[0] || v[7]) return 0;
  return 1;
} // SHE_mask_D()

static int32_t SHE_deletable_D(uint8_t *v)
{
  if (SHE_mask_D(v)) return 1; // UE
  rotate_90_Z(v);
  if (SHE_mask_D(v)) return 1; // UN
  rotate_90_Z(v);
  if (SHE_mask_D(v)) return 1; // UW
  rotate_90_Z(v);
  if (SHE_mask_D(v)) return 1; // US
  rotate_90_Z(v);
  swap_U_L(v);
  if (SHE_mask_D(v)) return 1; // LE
  rotate_90_Z(v);
  if (SHE_mask_D(v)) return 1; // LN
  rotate_90_Z(v);
  if (SHE_mask_D(v)) return 1; // LW
  rotate_90_Z(v);
  if (SHE_mask_D(v)) return 1; // LS
  rotate_90_Z(v);
  swap_U_L(v);
  isometrieYZ_vois(v);
  if (SHE_mask_D(v)) return 1;
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (SHE_mask_D(v)) return 1;
  swap_U_L(v);
  if (SHE_mask_D(v)) return 1;
  rotate_90_Z(v);
  rotate_90_Z(v);
  if (SHE_mask_D(v)) return 1;
  
  return 0;
} // SHE_deletable_D()

static int32_t SHE_mask_C(uint8_t *v)
{
  if (!v[4] || !v[17] || !v[6]) return 0;
  if (v[11] || v[8] || v[9] || v[10] || v[1] || v[0] || v[2]) return 0;
  return 1;
} // SHE_mask_C()

static int32_t SHE_deletable_C(uint8_t *v)
{
  if (SHE_mask_C(v)) return 1; // UE
  rotate_90_Z(v);
  if (SHE_mask_C(v)) return 1; // UN
  rotate_90_Z(v);
  if (SHE_mask_C(v)) return 1; // UW
  rotate_90_Z(v);
  if (SHE_mask_C(v)) return 1; // US
  rotate_90_Z(v);
  swap_U_L(v);
  if (SHE_mask_C(v)) return 1; // LE
  rotate_90_Z(v);
  if (SHE_mask_C(v)) return 1; // LN
  rotate_90_Z(v);
  if (SHE_mask_C(v)) return 1; // LW
  rotate_90_Z(v);
  if (SHE_mask_C(v)) return 1; // LS
  rotate_90_Z(v);
  swap_U_L(v);  
  return 0;
} // SHE_deletable_C()

/* ==================================== */
int32_t lsheetalcurvsym2009(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lsheetalcurvsym2009"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, nonstab2;
  uint8_t v[27];

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    step++;
    nonstab = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = SHE_OBJECT;

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == (step%2))
      {
	x = k*ps + j*rs + i;
	if (S[x] && (mctopo3d_nbvoisc26(S, x, rs, ps, N) >= 1))
	{
#ifdef DEBUG_lsheetalcurvsym2009
printf("point %d %d %d\n", i, j, k);	  
#endif
	  SET_SHE_MARK(S[x]);
	} // if (S[x])
      } // for i, j, k

    nonstab2 = 1;
    while (nonstab2)
    {
      nonstab2 = 0;
#ifdef VERBOSE
      printf("step %d - substep\n", step);
#endif

      for (k = 1; k < ds-1; k++)
      for (j = 1; j < cs-1; j++)
      for (i = 1; i < rs-1; i++)
      {
	x = k*ps + j*rs + i;
	if (S[x] && IS_SHE_MARK(S[x]))
	{
	  extract_vois(S, x, rs, ps, N, v);
	  if (SHE_deletable_A(v) || SHE_deletable_B(v) || SHE_deletable_C(v) || SHE_deletable_D(v))
	    SET_SHE_DELETE(S[x]);
	} // if (S[x])
      } // for i, j, k

      for (k = 1; k < ds-1; k++)
      for (j = 1; j < cs-1; j++)
      for (i = 1; i < rs-1; i++)
      {
	x = k*ps + j*rs + i;
	if (S[x] && IS_SHE_DELETE(S[x]) && mctopo3d_simple26(S, x, rs, ps, N))
	{
	  S[x] = 0;
	  nonstab = nonstab2 = 1;
	} // if (S[x])
      } // for i, j, k

    } // while (nonstab2)

    for (x = 0; x < N; x++) UNSET_SHE_MARK(S[x]);

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  mctopo3d_termine_topo3d();
  return(1);
} /* lsheetalcurvsym2009() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Lohou et Bertrand (curviligne symmétrique, Pat. Rec. 2007) 
// M. Couprie, dec. 2011
/* ============================================================ */
/* ============================================================ */

/* ==================================== */
int32_t llohoubertrandsymcurv2007(
				  struct xvimage *image,
				  struct xvimage *inhibit,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "llohoubertrandsymcurv2007"
{
  int32_t i, j, k, x, y, v;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t;
  uint8_t *T;
  struct xvimage *c;
  uint8_t *C;
  uint8_t *I;
  int32_t step, nonstab, top, topbar;

  t = copyimage(image); assert(t != NULL);
  T = UCHARDATA(t);
  c = copyimage(image); assert(c != NULL);
  C = UCHARDATA(c);
  I = NULL;

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;
  if (inhibit != NULL) I = UCHARDATA(inhibit);

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    
    razimage(c);
    for (k = 2; k < ds-2; k++) // prépare l'ensemble C des points candidats
      for (j = 2; j < cs-2; j++)
	for (i = 2; i < rs-2; i++)
	{
	  x = k*ps + j*rs + i;
	  if (T[x] && ((I==NULL) || !I[x]))
	  {
	    mctopo3d_top26(T, x, rs, ps, N, &top, &topbar);
	    if (topbar == 1)
	    {
	      for (v = 0; v < 12; v += 2)
	      {
		y = voisin6(x, v, rs, ps, N);
		if ((y != -1) && (T[y]))
		{
		  mctopo3d_top26(T, y, rs, ps, N, &top, &topbar);
		  if (topbar == 1)
		  {
		    T[y] = 0;
		    mctopo3d_top26(T, x, rs, ps, N, &top, &topbar);
		    T[y] = 1;
		    if (topbar != 1) break;
		  } // if (topbar y == 1)
		} // if ((y != -1) && (T[y]))
	      } // for (v = 0; v <= 10; v += 2)
	      if (v == 12) C[x] = 1;
	    } // if (topbar x == 1)
	  } // if (T[x])
	} // for i, j, k

    //#define DEBUG_llohoubertrandsymcurv2007
#ifdef DEBUG_llohoubertrandsymcurv2007
    writeimage(c, "_c");
#endif

    for (k = 2; k < ds-2; k++) // retire en parallèle les points PC_simples
      for (j = 2; j < cs-2; j++)
	for (i = 2; i < rs-2; i++)
	{
	  x = k*ps + j*rs + i;
	  if (C[x] && ((I==NULL) || !I[x]) && P_simple26(T, C, x, rs, ps, N))
	  {
	    X[x] = 0;
	    nonstab = 1;
	  }
	} // for k,j,i

    memcpy(T, X, N);
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i]) X[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  freeimage(t);
  freeimage(c);
  return(1);
} /* llohoubertrandsymcurv2007() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Lohou et Bertrand (surfacique symmétrique, Pat. Rec. 2007) 
// M. Couprie, sept. 2012
/* ============================================================ */
/* ============================================================ */

/* ==================================== */
int32_t llohoubertrandsymsurf2007(
				  struct xvimage *image,
				  struct xvimage *inhibit,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "llohoubertrandsymsurf2007"
{

  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t;
  uint8_t *T;
  struct xvimage *s;
  uint8_t *S;
  uint8_t *I;
  int32_t step, nonstab, top, topbar;

  t = copyimage(image); assert(t != NULL);
  T = UCHARDATA(t);
  s = copyimage(image); assert(s != NULL);
  S = UCHARDATA(s);
  I = NULL;

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;
  if (inhibit != NULL) I = UCHARDATA(inhibit);

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    
    razimage(s);
    for (k = 2; k < ds-2; k++) // prépare l'ensemble S des points candidats
      for (j = 2; j < cs-2; j++)
	for (i = 2; i < rs-2; i++)
	{
	  x = k*ps + j*rs + i;
	  if (T[x] && ((I==NULL) || !I[x]))
	  {
	    mctopo3d_top26(T, x, rs, ps, N, &top, &topbar);
	    if (topbar == 1) S[x] = 1;
	  } // if (T[x])
	} // for i, j, k

    //#define DEBUG_llohoubertrandsymsurf2007
#ifdef DEBUG_llohoubertrandsymsurf2007
    writeimage(s, "_s");
#endif

    for (k = 2; k < ds-2; k++) // retire en parallèle les points PS_simples
      for (j = 2; j < cs-2; j++)
	for (i = 2; i < rs-2; i++)
	{
	  x = k*ps + j*rs + i;
	  if (S[x] && ((I==NULL) || !I[x]) && P_simple26(T, S, x, rs, ps, N))
	  {
	    X[x] = 0;
	    nonstab = 1;
	  }
	} // for k,j,i

    memcpy(T, X, N);
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i]) X[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  freeimage(t);
  freeimage(s);
  return(1);
} /* llohoubertrandsymsurf2007() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Lohou et Bertrand (curviligne 6 subiterations, DAM 2005) 
// M. Couprie, jan. 2012
/* ============================================================ */
/* ============================================================ */

#define LB6_OBJECT      1
#define LB6_PXSIMPLE        2

#define IS_PXSIMPLE(f) (f&LB6_PXSIMPLE)
#define SET_PXSIMPLE(f) (f|=LB6_PXSIMPLE)

static int32_t lb6_matchmask(uint8_t *s, int32_t i, int32_t j, int32_t k)
// tests if point i,j,k in s matches (restriction of) one of the masks
{
  if (k < 1) return 1;
  if (s[(k-1)*9 + j*3 + i]) return 0;
  if ((j >= 1) && !s[k*9 + (j-1)*3 + i] && s[(k-1)*9 + (j-1)*3 + i]) return 0;
  if ((j <= 1) && !s[k*9 + (j+1)*3 + i] && s[(k-1)*9 + (j+1)*3 + i]) return 0;
  if ((i >= 1) && !s[k*9 + j*3 + i-1] && s[(k-1)*9 + j*3 + i-1]) return 0;
  if ((i <= 1) && !s[k*9 + j*3 + i+1] && s[(k-1)*9 + j*3 + i+1]) return 0;
  return 1;
} // lb6_matchmask()

static void lb6_setp(uint8_t *s, uint8_t *p)
{
  int32_t i, j, k;
  memset(p, 0, 27); 
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
	if (s[k*9 + j*3 + i])
	{
	  if (lb6_matchmask(s, i, j, k)) 
	    p[k*9 + j*3 + i] = 1;
	}
}

static int32_t lb6_match(int32_t d, uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  uint8_t s[27], p[27]; 
//printf("lb12_match: point %d\n", i);//DEBUG
  extract_vois27(S, i, rs, ps, N, s); 
  switch (d)
  {
  case 0: break;
  case 1: swap_U_L_27(s); break;
  case 2: isometrieXZ_vois27(s); break;
  case 3: isometrieXZ_vois27(s); swap_U_L_27(s); break;
  case 4: isometrieYZ_vois27(s); break;
  case 5: isometrieYZ_vois27(s); swap_U_L_27(s); break;
  }
  lb6_setp(s, p); 
//printf("lb6_match: s = \n");//DEBUG
//print_vois27(s);//DEBUG
//printf("lb6_match: p = \n");//DEBUG
//print_vois27(p);//DEBUG
//if (p[13]) printf("lb6_match: point %d, dir %d\n", i, d);//DEBUG
  return p[13] && P_simple26(s, p, 13, 3, 9, 27);
}

/* ==================================== */
int32_t llohoubertrand6dir2005(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "llohoubertrand6dir2005"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (S[i]) S[i] = LB6_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (d = 0; d < 6; d++)
    {
      for (i = 0; i < N; i++) 
	if (S[i] && (mctopo3d_nbvoiso26(S, i, rs, ps, N) > 1) && lb6_match(d, S, i, rs, ps, N)) 
	  SET_PXSIMPLE(S[i]);
      for (i = 0; i < N; i++) 
	if (IS_PXSIMPLE(S[i])) 
	{ 
	  S[i] = 0; 
	  nonstab = 1; 
	}
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  return(1);
} /* llohoubertrand6dir2005() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Lohou et Bertrand (curviligne 12 subiterations, DAM 2004) 
// M. Couprie, jan. 2012
/* ============================================================ */
/* ============================================================ */

#define LB12_OBJECT      1
#define LB12_PXSIMPLE        2

#undef IS_PXSIMPLE
#undef SET_PXSIMPLE
#define IS_PXSIMPLE(f) (f&LB12_PXSIMPLE)
#define SET_PXSIMPLE(f) (f|=LB12_PXSIMPLE)

// 0: don't care; 1: object; 2: background; 3: at least one is object
static uint8_t lb12_mask1[27] = {
  2, 2, 2, 2, 2, 2, 2, 2, 2, 
  0, 0, 0, 0, 1, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0, 0, 0, 0
};

static uint8_t lb12_mask2[27] = {
  0, 0, 0, 0, 1, 0, 2, 2, 2,
  0, 1, 0, 0, 1, 0, 2, 2, 2,
  0, 0, 0, 0, 0, 0, 2, 2, 2
};

static uint8_t lb12_mask3[27] = {
  0, 0, 0, 0, 2, 0, 0, 2, 0,
  0, 3, 0, 0, 1, 0, 0, 2, 0,
  0, 3, 0, 0, 3, 0, 0, 0, 0
};

static uint8_t *lb12_masks_1[12];
static uint8_t *lb12_masks_2[12];
static uint8_t *lb12_masks_3[12];

static int32_t lb12_matchmask(uint8_t *s, int32_t i, int32_t j, int32_t k, uint8_t *m)
// tests if point i,j,k in s matches (restriction of) mask m
{
  int32_t x, y, z;
  int32_t t = 0, n = 0;
  for (z = -1; z <= 1; z++) if (((k+z) >= 0) && ((k+z) < 3))
  {
    for (y = -1; y <= 1; y++) if (((j+y) >= 0) && ((j+y) < 3))
    {
      for (x = -1; x <= 1; x++) if (((i+x) >= 0) && ((i+x) < 3))
      {
	if (!s[(k+z)*9 + (j+y)*3 + i+x] && (m[(1+z)*9 + (1+y)*3 + 1+x] == 1)) 
	{
//printf("lb12_matchmask: fail1 with i,j,k = %d %d %d ; x,y,z = %d %d %d\n", i, j, k, x, y, z); DEBUG
	  return 0;
	}
	if (s[(k+z)*9 + (j+y)*3 + i+x] && (m[(1+z)*9 + (1+y)*3 + 1+x] == 2)) 
	{
//printf("lb12_matchmask: fail2 with i,j,k = %d %d %d ; x,y,z = %d %d %d\n", i, j, k, x, y, z);DEBUG
	  return 0;
	}
	if (m[z*9 + y*3 + x] == 3) 
	{
	  t++; 
	  if (s[(k+z)*9 + (j+y)*3 + i+x]) n++;
	} 
      }
    }
  }
  if ((t == 3) && (n == 0)) 
  {
//printf("lb12_matchmask: fail3\n"); DEBUG
    return 0;
  }
  return 1;
}

static void lb12_setp(int32_t d, uint8_t *s, uint8_t *p)
{
  int32_t i, j, k;
  memset(p, 0, 27); 
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
	if (s[k*9 + j*3 + i])
	{
//printf("lb12_setp: i,j,k = %d,%d,%d, matchmask1 = %d\n", i, j, k, DEBUG
//lb12_matchmask(s, i, j, k, lb12_masks_1[d]));DEBUG
//printf("lb12_setp: i,j,k = %d,%d,%d, matchmask2 = %d\n", i, j, k, DEBUG
//lb12_matchmask(s, i, j, k, lb12_masks_2[d]));DEBUG
//printf("lb12_setp: i,j,k = %d,%d,%d, matchmask3 = %d\n", i, j, k, DEBUG
//lb12_matchmask(s, i, j, k, lb12_masks_3[d]));DEBUG
	  if (lb12_matchmask(s, i, j, k, lb12_masks_1[d]) ||
	      lb12_matchmask(s, i, j, k, lb12_masks_2[d]) ||
	      lb12_matchmask(s, i, j, k, lb12_masks_3[d])) 
	    p[k*9 + j*3 + i] = 1;
	}
}

static int32_t lb12_match(int32_t d, uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  uint8_t s[27], p[27]; 
  //printf("lb12_match: point %d\n", i);DEBUG
  extract_vois27(S, i, rs, ps, N, s); 
  lb12_setp(d, s, p); 
  //printf("lb12_match: s = \n");DEBUG
  //print_vois27(s);DEBUG
  //printf("lb12_match: p = \n");DEBUG
  //print_vois27(p);DEBUG
  //if (p[13]) printf("lb12_match: point %d, dir %d\n", i, d);DEBUG
  return p[13] && P_simple26(s, p, 13, 3, 9, 27);
}

/* ==================================== */
int32_t llohoubertrand12dir2004(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "llohoubertrand12dir2004"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;
  uint8_t v1[27], v2[27], v3[27]; 

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (S[i]) S[i] = LB12_OBJECT; 

  /* ================================================ */
  /* Préparation des masques */
  /* ================================================ */

  for (d = 0; d < 12; d++)
  {
    lb12_masks_1[d] = (uint8_t *)malloc(27 * sizeof(uint8_t)); assert(lb12_masks_1[d]);
    lb12_masks_2[d] = (uint8_t *)malloc(27 * sizeof(uint8_t)); assert(lb12_masks_2[d]);
    lb12_masks_3[d] = (uint8_t *)malloc(27 * sizeof(uint8_t)); assert(lb12_masks_3[d]);
  }

  extract_vois27(lb12_mask1, 13, 3, 9, 27, v1);
  extract_vois27(lb12_mask2, 13, 3, 9, 27, v2);
  extract_vois27(lb12_mask3, 13, 3, 9, 27, v3);
 
  memcpy(lb12_masks_1[0], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[0], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[0], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  memcpy(lb12_masks_1[2], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[2], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[2], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  memcpy(lb12_masks_1[4], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[4], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[4], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  memcpy(lb12_masks_1[6], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[6], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[6], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);

  swap_U_L_27(v1); swap_U_L_27(v2); swap_U_L_27(v3);
  memcpy(lb12_masks_1[5], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[5], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[5], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  memcpy(lb12_masks_1[7], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[7], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[7], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  memcpy(lb12_masks_1[1], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[1], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[1], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  memcpy(lb12_masks_1[3], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[3], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[3], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  swap_U_L_27(v1); swap_U_L_27(v2); swap_U_L_27(v3);

  isometrieYZ_vois27(v1); isometrieYZ_vois27(v2); isometrieYZ_vois27(v3);
  memcpy(lb12_masks_1[8], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[8], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[8], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  memcpy(lb12_masks_1[10], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[10], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[10], (uint8_t *)v3, 27);
  swap_U_L_27(v1); swap_U_L_27(v2); swap_U_L_27(v3);
  memcpy(lb12_masks_1[9], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[9], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[9], (uint8_t *)v3, 27);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  rotate_90_Z_27(v1); rotate_90_Z_27(v2); rotate_90_Z_27(v3);
  memcpy(lb12_masks_1[11], (uint8_t *)v1, 27);
  memcpy(lb12_masks_2[11], (uint8_t *)v2, 27);
  memcpy(lb12_masks_3[11], (uint8_t *)v3, 27);

//printf("mask v1\n");DEBUG
//print_vois27(lb12_masks_1[0]);DEBUG

//printf("mask v2\n");DEBUG
//print_vois27(lb12_masks_2[0]);DEBUG

//printf("mask v3\n");DEBUG
//print_vois27(lb12_masks_3[0]);DEBUG

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (d = 0; d < 12; d++)
    {
      for (i = 0; i < N; i++) 
	if (S[i] && lb12_match(d, S, i, rs, ps, N)) 
	  SET_PXSIMPLE(S[i]);
      for (i = 0; i < N; i++) 
	if (IS_PXSIMPLE(S[i])) 
	{ 
	  S[i] = 0; 
	  nonstab = 1; 
	}
    } // for (d = 0; d < 12; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  return(1);
} /* llohoubertrand12dir2004() */

/* ============================================================ */
/* ============================================================ */


/* global variables */
  long int           long_mask[26];
  unsigned char      char_mask[8];
  unsigned long int  neighbours;
  unsigned long int  neighbours_UD;
  unsigned long int  neighbours_NS;
  unsigned long int  neighbours_EW;
  unsigned long int  GLOBAL_direction;
  unsigned char     *lut_deletable;
  unsigned long int  size_x;
  unsigned long int  size_y;
  unsigned long int  size_z;
  unsigned long int  size_xy;
  unsigned long int  size_xyz;
  unsigned char     *image;
  List               SurfaceVoxels;
  List               BorderVoxels;
  unsigned long int  act_addr;
  struct xvimage *pgmimage;
  Bordercell *borderlist;
  int                DEL6NEI;
  unsigned long int  endflag;
  unsigned char      *lut_simple;
  unsigned long int  z_size_xy, zm_size_xy, zp_size_xy;
  unsigned long int  y_size_x, ym_size_x, yp_size_x;


//#define DEBUG
//#ifdef DEBUG
//int32_t trace = 1;
//#endif
//
//typedef uint32_t mat[3][3];
//typedef uint32_t vec[3];
//
//// matrices pour les 8 symétries de type 1
//// et translations associées
//
//mat R0 = {
//  {  1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0,  1}
//};
//vec T0 = {0,0,0};
//
//mat R1 = {
//  { -1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0,  1}
//};
//vec T1 = {-4,0,0};
//
//mat R2 = {
//  {  1,  0,  0},
//  {  0, -1,  0},
//  {  0,  0,  1}
//};
//vec T2 = {0,-4,0};
//
//mat R3 = {
//  { -1,  0,  0},
//  {  0, -1,  0},
//  {  0,  0,  1}
//};
//vec T3 = {-4,-4,0};
//
//mat R4 = {
//  {  1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0, -1}
//};
//vec T4 = {0,0,-4};
//
//mat R5 = {
//  { -1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0, -1}
//};
//vec T5 = {-4,0,-4};
//
//mat R6 = {
//  {  1,  0,  0},
//  {  0, -1,  0},
//  {  0,  0, -1}
//};
//vec T6 = {0,-4,-4};
//
//mat R7 = {
//  { -1,  0,  0},
//  {  0, -1,  0},
//  {  0,  0, -1}
//};
//vec T7 = {-4,-4,-4};
//
//// matrices pour les 6 symétries de type 2
//
//mat S0 = {
//  {  1,  0,  0},
//  {  0,  1,  0},
//  {  0,  0,  1}
//};
//
//mat S1 = {
//  {  1,  0,  0},
//  {  0,  0,  1},
//  {  0,  1,  0}
//};
//
//mat S2 = {
//  {  0,  1,  0},
//  {  1,  0,  0},
//  {  0,  0,  1}
//};
//
//mat S3 = {
//  {  0,  1,  0},
//  {  0,  0,  1},
//  {  1,  0,  0}
//};
//
//mat S4 = {
//  {  0,  0,  1},
//  {  1,  0,  0},
//  {  0,  1,  0}
//};
//
//mat S5 = {
//  {  0,  0,  1},
//  {  0,  1,  0},
//  {  1,  0,  0}
//};

///* ==================================== */
//static void multmatvec(
//  mat m,
//  vec v,
//  vec sol)
///* ==================================== */
//{
//  int32_t i, j, t;
//  for (i = 0; i < 3; i++)
//  {
//    t = 0;
//    for (j = 0; j < 3; j++)
//      t += m[i][j] * v[j];
//    sol[i] = t;
//  }
//} // multmatvec()
//
///* ==================================== */
//static int32_t isometry_coord(mat s1, vec t1, mat s2, vec x, vec tmp)
///* ==================================== */
//{
//  multmatvec(s1,x,tmp);
//  tmp[0] -= t1[0];
//  tmp[1] -= t1[1];
//  tmp[2] -= t1[2];
//  multmatvec(s2,tmp,x);
//  return x[2]*25 + x[1]*5 + x[0];
//} // isometry_coord()
//
///* ==================================== */
//static void isometry(uint8_t *dest, uint8_t *sour, mat s1, vec t1, mat s2)
///* ==================================== */
//{
//  vec x, tmp = {0,0,0};
//  int32_t i, j, k, p;
//  for (k = 0; k < 5; k++)
//    for (j = 0; j < 5; j++)
//      for (i = 0; i < 5; i++)
//      {
//	x[0] = i; x[1] = j; x[2] = k;
//	p = isometry_coord(s1, t1, s2, x, tmp);
//	dest[p] = sour[k*25 + j*5 + i];
//      }
//} // isometry()
//
///* ==================================== */
//static void extract_vois(
//  uint8_t *img,                    /* pointeur base image */
//  int32_t p,                       /* index du point */
//  int32_t rs,                      /* taille rangee */
//  int32_t ps,                      /* taille plan */
//  int32_t N,                       /* taille image */
//  uint8_t *vois)
///*
//  retourne dans "vois" les valeurs des 125 voisins de p, dans l'ordre suivant:
//
//  0   1   2   3   4
//  5   6   7   8   9
// 10  11  12  13  14
// 15  16  17  18  19
// 20  21  22  23  24
//
// 25  26  27  28  29
// 30  31  32  33  34
// 35  36  37  38  39
// 40  41  42  43  44
// 45  46  47  48  49
//
// 50  51  52  53  54
// 55  56  57  58  59
// 60  61  62  63  64
// 65  66  67  68  69
// 70  71  72  73  74
//
// 75  76  77  78  79
// 80  81  82  83  84
// 85  86  87  88  89
// 90  91  92  93 ...
//
// le tableau vois doit avoir été alloué
//
//*/
///* ==================================== */
//{
//#undef F_NAME
//#define F_NAME "extract_vois"
//  uint32_t xx, yy, zz, i, j, k;
//  if ((p%rs>=rs-2) || (p%ps<rs-1) || (p%rs<=1) || (p%ps>=ps-rs-rs) ||
//      (p < ps-1) || (p >= N-ps-ps)) /* point de bord */
//  {
//    printf("%s: ERREUR: point de bord\n", F_NAME);
//    exit(0);
//  }
//
//  zz = p / ps;
//  yy = (p % ps) / rs;
//  xx = p % rs;
//  for (k = 0; k < 5; k++)
//    for (j = 0; j < 5; j++)
//      for (i = 0; i < 5; i++)
//      {
//	vois[(k * 25) + (j * 5) + i] =
//	  img[((zz-2+k) * ps) + ((yy-2+j) * rs) + xx-2+i];
//      }
//
//} /* extract_vois() */
//
///* ==================================== */
//int32_t pala_match1(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88] &&
//      !v[91] && !v[92] && !v[93])
//    return 1;
//  else
//    return 0;
//} // palagyi_match1()
//
///* ==================================== */
//int32_t pala_match2(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      v[67] && (!v[61] || !v[63]) && !v[86] && !v[87] && !v[88] &&
//      !v[81] && !v[82] && !v[83])
//    return 1;
//  else
//    return 0;
//} // palagyi_match2()
//
///* ==================================== */
//int32_t pala_match3(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      v[67] && v[61] && !v[57] && !v[63] && !v[87] && !v[88] &&
//      !v[82] && !v[83])
//    return 1;
//  else
//    return 0;
//} // palagyi_match3()
//
///* ==================================== */
//int32_t pala_match4(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      v[61] && v[63] && v[67] &&
//      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88] && !v[92])
//    return 1;
//  else
//    return 0;
//} // palagyi_match4()
//
///* ==================================== */
//int32_t pala_match5(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] &&
//      v[61] && v[63] && v[66] && v[67] && v[68] &&
//      !v[81] && !v[82] && !v[83] && !v[86] && !v[87] && !v[88])
//    return 1;
//  else
//    return 0;
//} // palagyi_match5()
//
///* ==================================== */
//int32_t pala_match6(uint8_t *v)
///* ==================================== */
//{
//  if (v[12] && v[37] && v[32] && v[36] && v[38] && v[42] && v[57] && v[67] &&
//      !v[61] && !v[63] && !v[86] && !v[87] && !v[88])
//    return 1;
//  else
//    return 0;
//} // palagyi_match6()
//
///* ==================================== */
//int32_t palagyi_match1(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match1(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match1(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match1(vv)) return 1;
//
//  return 0;
//} // palagyi_match1()
//
///* ==================================== */
//int32_t palagyi_match2(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match2(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match2(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match2(vv)) return 1;
//
//  return 0;
//} // palagyi_match2()
//
///* ==================================== */
//int32_t palagyi_match3(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match3(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match3(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match3(vv)) return 1;
//
//  return 0;
//} // palagyi_match3()
//
///* ==================================== */
//int32_t palagyi_match4(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match4(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match4(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match4(vv)) return 1;
//
//  return 0;
//} // palagyi_match4()
//
///* ==================================== */
//int32_t palagyi_match5(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match5(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match5(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match5(vv)) return 1;
//
//  return 0;
//} // palagyi_match5()
//
///* ==================================== */
//int32_t palagyi_match6(uint8_t *v, uint8_t *vv)
///* ==================================== */
//{
//  isometry(vv, v, R0, T0, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S0); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S0); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S1); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S1); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S2); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S2); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S3); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S3); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S4); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S4); if (pala_match6(vv)) return 1;
//
//  isometry(vv, v, R0, T0, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R1, T1, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R2, T2, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R3, T3, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R4, T4, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R5, T5, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R6, T6, S5); if (pala_match6(vv)) return 1;
//  isometry(vv, v, R7, T7, S5); if (pala_match6(vv)) return 1;
//
//  return 0;
//} // palagyi_match6()
//
///* ==================================== */
//int32_t lpalagyi(struct xvimage *image,
//		 int32_t nsteps)
///* ==================================== */
//#undef F_NAME
//#define F_NAME "lpalagyi"
//{
//  int32_t i, j, k, x;
//  int32_t rs = rowsize(image);     /* taille ligne */
//  int32_t cs = colsize(image);     /* taille colonne */
//  int32_t ds = depth(image);       /* nb plans */
//  int32_t ps = rs * cs;            /* taille plan */
//  int32_t N = ps * ds;             /* taille image */
//  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
//  struct xvimage *t = copyimage(image);
//  uint8_t *T = UCHARDATA(t);
//  int32_t step, nonstab;
//  uint8_t v[125];
//  uint8_t vv[125];
//
//#ifdef VERBOSE
//    printf("Warning: this is a very slow implementation. I might take time...\n");
//#endif
//
//  if (nsteps == -1) nsteps = 1000000000;
//
//  /* ================================================ */
//  /*               DEBUT ALGO                         */
//  /* ================================================ */
//
//  step = 0;
//  nonstab = 1;
//  while (nonstab && (step < nsteps))
//  {
//    nonstab = 0;
//    step++;
//#ifdef VERBOSE
//    printf("step %d\n", step);
//#endif
//    for (k = 2; k < ds-2; k++)
//      for (j = 2; j < cs-2; j++)
//	for (i = 2; i < rs-2; i++)
//	{
//	  x = k*ps + j*rs + i;
//	  if (T[x])
//	  {
//	    extract_vois(T, x, rs, ps, N, v);
//	    if (palagyi_match1(v, vv) || palagyi_match2(v, vv) ||
//		palagyi_match3(v, vv) || palagyi_match4(v, vv) ||
//		palagyi_match5(v, vv) || palagyi_match6(v, vv))
//	    {
//	      S[x] = 0;
//	      nonstab = 1;
//	    }
//	  }
//	} // for k,j,i
//    memcpy(T, S, N);
//  } // while (nonstab && (step < nsteps))
//
//#ifdef VERBOSE1
//    printf("number of steps: %d\n", step);
//#endif
//
//  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
//
//  freeimage(t);
//  return(1);
//} /* lpalagyi() */
//

/*******************************************************************************
*
*       Author:              K. Palagyi (adapted by J. Chaussard for PINK)
*
*******************************************************************************/

/*========= function read_image =========*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      pgmimage
      size_x
      size_y
      size_z
      size_xy
      size_xyz
      image
----------------*/
void read_image()
{
	unsigned long int  i;

	image=(unsigned char*)pgmimage->image_data;
	size_x=rowsize(pgmimage);
	size_y=colsize(pgmimage);
	size_z=depth(pgmimage);
	size_xy=size_x*size_y;
	size_xyz=size_xy*size_z;

	if ( size_z <= 1 )
      	{
        	printf("ERROR: The image data is not a 3D one. Number of slices is %lu \n", size_z);
        	exit(-1);
      	}

	for(i=0; i<size_xyz; i++)
		if(UCHARDATA(pgmimage)[i]!=0) UCHARDATA(pgmimage)[i]=1; //Threshold
}
/*========= end of function read_image =========*/



/*========= function write_image =========*/
/* -------------
  considered global variables:
      pgmimage
      size_xyz
  changed global variables:
      image (data)
----------------*/
void write_image()
{
	unsigned long int  i;

	for(i=0; i<size_xyz; i++)
		if(UCHARDATA(pgmimage)[i]!=0) UCHARDATA(pgmimage)[i]=255; //Threshold
}

/*========= end of function write_image =========*/


/*========= function set_long_mask =========*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      long_mask
----------------*/
void set_long_mask( void )
  {
    long_mask[ 0] = 0x00000001;
    long_mask[ 1] = 0x00000002;
    long_mask[ 2] = 0x00000004;
    long_mask[ 3] = 0x00000008;
    long_mask[ 4] = 0x00000010;
    long_mask[ 5] = 0x00000020;
    long_mask[ 6] = 0x00000040;
    long_mask[ 7] = 0x00000080;
    long_mask[ 8] = 0x00000100;
    long_mask[ 9] = 0x00000200;
    long_mask[10] = 0x00000400;
    long_mask[11] = 0x00000800;
    long_mask[12] = 0x00001000;
    long_mask[13] = 0x00002000;
    long_mask[14] = 0x00004000;
    long_mask[15] = 0x00008000;
    long_mask[16] = 0x00010000;
    long_mask[17] = 0x00020000;
    long_mask[18] = 0x00040000;
    long_mask[19] = 0x00080000;
    long_mask[20] = 0x00100000;
    long_mask[21] = 0x00200000;
    long_mask[22] = 0x00400000;
    long_mask[23] = 0x00800000;
    long_mask[24] = 0x01000000;
    long_mask[25] = 0x02000000;
  }
/*========= end of function set_char_mask =========*/


/*========= function set_char_mask =========*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      char_mask
----------------*/
void set_char_mask( void )
  {
    char_mask[0] = 0x01;
    char_mask[1] = 0x02;
    char_mask[2] = 0x04;
    char_mask[3] = 0x08;
    char_mask[4] = 0x10;
    char_mask[5] = 0x20;
    char_mask[6] = 0x40;
    char_mask[7] = 0x80;
  }
/*========= end of function set_long_mask =========*/


/*============= function init_lut =============*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      lut_deletable
      long_mask
      char_mask
----------------*/
void init_lut( char* mask_name )
{
  char  lutname [100];
  FILE  *lutfile;

  /* alloc lut_deletable */
    lut_deletable = (unsigned char *)malloc(0x00400000);    /* 4 Mbyte */
    if ( lut_deletable == NULL)
      {
        printf("\n Alloc error!!!\n");
        exit(1);
      }  /* end if */

  /* open lutfile */
    strcpy( lutname, mask_name);
    lutfile = fopen( lutname, "rb");
    if ( lutfile == NULL)
      {
        printf("\n\n file %s is not found!!!\n", mask_name);
        exit(1);
      }  /* end if */

  /* reading lutfile */
    if ( fread( lut_deletable, 1, 0x00400000, lutfile) != 0x00400000 )      /* 4 Mbyte */
      {
         printf("ERROR: Couldn't read LUT\n");
         exit(1);
      }

    fclose(lutfile);

  /* setting masks */
    set_long_mask();
    set_char_mask();

}
/*=========== end of function init_lut ===========*/



/*==================================*/
/*========= list functions =========*/
/*==================================*/

void NewSurfaceVoxel( void ) {
ListElement * LE;
	LE=(ListElement *)malloc(sizeof(ListElement));
	(*LE).addr=act_addr;
	(*LE).next=NULL;
	(*LE).prev=SurfaceVoxels.last;
	if (SurfaceVoxels.last!=NULL) (*((ListElement*)(SurfaceVoxels.last))).next=LE;
	SurfaceVoxels.last=LE;
	if (SurfaceVoxels.first==NULL) SurfaceVoxels.first=LE;
}

void RemoveSurfaceVoxel(ListElement * LE) {
ListElement * LE2;
	if (SurfaceVoxels.first==LE) SurfaceVoxels.first=(*LE).next;
	if (SurfaceVoxels.last==LE) SurfaceVoxels.last=(*LE).prev;
	if ((*LE).next!=NULL) {
		LE2=(ListElement*)((*LE).next);
		(*LE2).prev=(*LE).prev;
	}
	if ((*LE).prev!=NULL) {
		LE2=(ListElement*)((*LE).prev);
		(*LE2).next=(*LE).next;
	}
	free(LE);
}

void CreatePointList(PointList *s) {
	s->Head=NULL;
	s->Tail=NULL;
	s->Length=0;
}

void AddToList(PointList *s, ListElement * ptr) {
Cell * newcell;
	newcell=(Cell *)malloc(sizeof(Cell));
	newcell->addr=act_addr;
	newcell->ptr=ptr;
	newcell->next=NULL;
	if (s->Head==NULL) {
		s->Head=newcell;
		s->Tail=newcell;
		s->Length=1;
	}
	else {
		s->Tail->next=newcell;
		s->Tail=newcell;
		s->Length++;
	}
}

unsigned long int GetFromList(PointList *s, ListElement **ptr) {
unsigned long int curr_addr;
Cell *tmp;
	(*ptr)=NULL;
	if(s->Length==0) return 0;
	else {
	        curr_addr = s->Head->addr;
		(*ptr)=s->Head->ptr;
		tmp=(Cell *)s->Head->next;
		free(s->Head);
		s->Head=tmp;
		s->Length--;
		if(s->Length==0) {
			s->Head=NULL;
			s->Tail=NULL;
		}
                return curr_addr;
	}
}

void DestroyPointList(PointList *s) {
ListElement * ptr;
	while(s->Length>0) GetFromList(s, &ptr);
}

void CollectSurfaceVoxels(void) {
unsigned long int x,y,z, num;
unsigned long int z_size_xy, y_size_x;

  SurfaceVoxels.first = NULL;
  SurfaceVoxels.last  = NULL;
  num = 0;

  for( z=1, z_size_xy=size_xy;
       z<size_z-1;
       z++, z_size_xy+=size_xy )
    {
      for( y=1, y_size_x=size_x;
           y<size_y-1;
           y++, y_size_x+=size_x )
        {
          for(x=1; x<size_x-1; x++)
	    {
	      act_addr = x + y_size_x + z_size_xy;
              if ( *(image + act_addr ) )
                {
                  if (  ( *(image + act_addr     - size_x           ) ==0 ) ||
                        ( *(image + act_addr     + size_x           ) ==0 ) ||
                        ( *(image + act_addr              - size_xy ) ==0 ) ||
                        ( *(image + act_addr              + size_xy ) ==0 ) ||
                        ( *(image + act_addr + 1                    ) ==0 ) ||
                        ( *(image + act_addr - 1                    ) ==0 )    )
                     {
                        *(image + act_addr ) = 2;    /* surface point */
                        NewSurfaceVoxel();
		        num ++;
                     } /* endif */
                } /* endif */
            } /* endfor x */
        } /* endfor y */
    } /* endfor z */
#ifdef VERBOSE
  printf("\n Number of surface/border points in the original image: %lu\n", num);
#endif
}

/*===============================================================*/
/*========= functions concerning topological properties =========*/
/*===============================================================*/


/*========= function collect_26_neighbours =========*/
void collect_26_neighbours( void )
  {

    neighbours = 0x00000000;

    if ( GLOBAL_direction == PALAGYI_U )
      {
        /*  U
          indices in "neighbours":
           0  1  2     8  9 10     16  17  18   y-1
           3  .  4    11  . 12     19  20  21   y
           5  6  7    13 14 15     22  23  24   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[15];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[24];
      } /* endif U */
   else
      { /* not-U */
       if ( GLOBAL_direction == PALAGYI_D )
      {
        /*  D
          indices in "neighbours":
           16  17  18     8  9 10     0  1  2   y-1
           19  20  21    11  . 12     3  .  4   y
           22  23  24    13 14 15     5  6  7   y+1
          x-1 x x+1     x-1 x x+1   x-1  x  x+1
              z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[15];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 7];
      } /* endif D */
    else
      { /* not-D */
    if ( GLOBAL_direction == PALAGYI_N )
      {
        /*  N
          indices in "neighbours":
           5  6  7     3  .  4      0   1   2   y-1
          13 14 15    11  . 12      8   9  10   y
          22 23 24    19 20 21     16  17  18   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[21];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[18];
      } /* endif N */
    else
      { /* not-N */
     if ( GLOBAL_direction == PALAGYI_S )
      {
        /*  S
          indices in "neighbours":
          22 23 24    19 20 21     16  17  18   y-1
          13 14 15    11  . 12      8   9  10   y
           5  6  7     3  .  4      0   1   2   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[24];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[ 4];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 2];
      } /* endif S */
    else
      { /* not-S */
     if ( GLOBAL_direction == PALAGYI_E )
      {
        /*  E
          indices in "neighbours":
           2 10 18     1  9 17      0   8  16   y-1
           4 12 21     .  . 20      3  11  19   y
           7 15 24     6 14 23      5  13  22   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[23];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];
      } /* endif E */
    else
      { /* not-E */
     if ( GLOBAL_direction == PALAGYI_W )
      {
        /*  W
          indices in "neighbours":
          18 10  2    17  9  1     16   8  0   y-1
          21 12  4    20  .  .     19  11  3   y
          24 15  7    23 14  6     22  13  5   y+1
          x-1 x x+1   x-1 x x+1    x-1  x x+1
             z-1          z           z+1
        */
        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[ 6];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 5];
      } /* endif W */
      } /* endelse not-E */
      } /* endelse not-S */
      } /* endelse not-N */
      } /* endelse not-D */
      } /* endelse not-U */


  }
/*========= end of function collect_26_neighbours =========*/


/*========= function deletable =========*/
int deletable( void )
{
  collect_26_neighbours();
  return ( ( *(lut_deletable + (neighbours>>3) ) ) & char_mask[neighbours%8]);
}
/*========= end of function deletable =========*/


/*=========== function DetectDeletablePoints ===========*/
void DetectDeletablePoints(PointList *s) {
ListElement * LE3;
unsigned char maybe;

  LE3=(ListElement *)SurfaceVoxels.first;
  while (LE3!=NULL)
    {

      act_addr = (*LE3).addr;

      maybe = 0;

      if ( GLOBAL_direction == PALAGYI_U )
        {
          if ( *(image + act_addr - size_xy ) == 0 )
            maybe = 1;
	} /* endif U */
      else
        { /* not-U */
      if ( GLOBAL_direction == PALAGYI_D )
        {
          if ( *(image + act_addr + size_xy ) == 0 )
            maybe = 1;
	}  /* endif D */
      else
        { /* not-D */
      if ( GLOBAL_direction == PALAGYI_N )
        {
          if ( *(image + act_addr - size_x  ) == 0 )
            maybe = 1;
	} /* endif N */
      else
        { /* not-N */
      if ( GLOBAL_direction == PALAGYI_S )
        {
          if ( *(image + act_addr + size_x  ) == 0 )
            maybe = 1;
	} /* endif S */
      else
        { /* not-S */
      if ( GLOBAL_direction == PALAGYI_E )
        {
          if ( *(image + act_addr - 1       ) == 0 )
            maybe = 1;
	} /* endif E */
      else
        { /* not_E */
      if ( GLOBAL_direction == PALAGYI_W )
        {
          if ( *(image + act_addr + 1       ) == 0 )
            maybe = 1;
	} /* endif W */
	} /* endelse not-E */
	} /* endelse not-S */
	} /* endelse not-N */
	} /* endelse not-D */
	} /* endelse not-U */

      if ( maybe )
        {
          if ( deletable() )
            {
	       AddToList(s,LE3);
            } /* endif */
        } /* endif */

      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

}
/*=========== end of function DetectDeletablePoints ===========*/


/*========= function thinning_iteration_step =========*/
unsigned int thinning_iteration_step(void)
{
  unsigned long int i, changed;
  ListElement * ptr;
  PointList s;
  unsigned long int  curr_addr;

  changed = 0;
  for ( i=0; i<6; i++ )                          /* 6-subiteration alg. */
    {
      GLOBAL_direction = i;
      CreatePointList(&s);
      DetectDeletablePoints(&s);
      while ( s.Length > 0 )
        {
           curr_addr = GetFromList( &s, &ptr );
           *(image + curr_addr ) = 0;            /* deleted */
	  changed ++;
           /* investigating v's 6-neighbours */
	      act_addr = curr_addr - 1;          /* (x-1,y,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + 1;          /* (x+1,y,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr - size_x;     /* (x,y-1,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + size_x;     /* (x,y+1,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr - size_xy;    /* (x,y,z-1) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + size_xy;    /* (x,y,z+1) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
            RemoveSurfaceVoxel(ptr);
        } /* endwhile */
      DestroyPointList(&s);
    } /* endfor */

  return changed;
}
/*========= end of function thinning_iteration_step =========*/

/*========= function thinning =========*/
void thinning(void)
{
  unsigned int iter, changed;

  CollectSurfaceVoxels();

  iter = 0;
  changed = 1;
  while ( changed )
    {
      changed = thinning_iteration_step();
      iter++;
#ifdef VERBOSE
      printf("\n  iteration step: %3d.    (deleted point(s): %6d)",
             iter, changed );
#endif
    }
#ifdef VERBOSE
      printf("\n");
#endif

}
/*========= end of function thinning =========*/


int32_t palagyi_skelpar_curv_98(struct xvimage *input)
{
#undef F_NAME
#define F_NAME "palagyi_skelpar_curv_98"
	char tablefilename[512];
	pgmimage=input;
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image();

  /***************/
  /* READING LUT */
  /***************/
	sprintf(tablefilename, "%s/src/tables/palagyi_pardir_curv_98.dat", getenv("PINK"));
    init_lut(tablefilename);

  /************/
  /* THINNING */
  /************/
#ifdef VERBOSE
    printf("%s: 6-subiteration directional curve-thinning ...\n", F_NAME);
#endif
    thinning();

  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    write_image( );

  /********/
  /* FREE */
  /********/
    free(lut_deletable);


    return(0);
}
/*===========================================================================
    end of function    m a i n
  ===========================================================================*/


/*============= function init_lut_simple =============*/
/* -------------
  considered global variables:
      NONE
  changed global variables:
      lut_simple
      long_mask
      char_mask
----------------*/
void init_lut_simple( char* mask_name )
{
  char  lutname [100];
  FILE  *lutfile;

  /* alloc lut_simple */
    lut_simple = (unsigned char *)malloc(0x00800000);
    if ( lut_simple == NULL)
      {
        printf("\n Alloc error!!!\n");
        exit(1);
      }  /* end if */

  /* open lutfile */
    strcpy( lutname, mask_name);
    lutfile = fopen( lutname, "rb");
    if ( lutfile == NULL)
      {
        printf("\n\n file %s is not found!!!\n", mask_name);
        exit(1);
      }  /* end if */

  /* reading lutfile */
    fread( lut_simple, 1, 0x00800000, lutfile);
    fclose(lutfile);

  /* setting masks */
    set_long_mask();
    set_char_mask();

}
/*=========== end of function init_lut_simple ===========*/


void NewSurfaceVoxel2(unsigned long int x,
                     unsigned long int y,
		     unsigned long int z) {
ListElement * LE;
	LE=(ListElement *)malloc(sizeof(ListElement));
	(*LE).x=x;
	(*LE).y=y;
	(*LE).z=z;
	(*LE).next=NULL;
	(*LE).prev=SurfaceVoxels.last;
	if (SurfaceVoxels.last!=NULL) (*((ListElement*)(SurfaceVoxels.last))).next=LE;
	SurfaceVoxels.last=LE;
	if (SurfaceVoxels.first==NULL) SurfaceVoxels.first=LE;
}


void AddToList2(PointList *s,Voxel e, ListElement * ptr) {
Cell * newcell;
	newcell=(Cell *)malloc(sizeof(Cell));
	newcell->v=e;
	newcell->ptr=ptr;
	newcell->next=NULL;
	if (s->Head==NULL) {
		s->Head=newcell;
		s->Tail=newcell;
		s->Length=1;
	}
	else {
		s->Tail->next=newcell;
		s->Tail=newcell;
		s->Length++;
	}
}

Voxel GetFromList2(PointList *s, ListElement **ptr) {
Voxel R;
Cell *tmp;
        R.i = -1;
        R.j = -1;
        R.k = -1;
        R.oldnei = 0;    /*** +field ***/
	(*ptr)=NULL;
	if(s->Length==0) return R;
	else {
		R=s->Head->v;
		(*ptr)=s->Head->ptr;
		tmp=(Cell *)s->Head->next;
		free(s->Head);
		s->Head=tmp;
		s->Length--;
		if(s->Length==0) {
			s->Head=NULL;
			s->Tail=NULL;
		}
		return R;
	}
}

void CollectSurfaceVoxels2(void) {
unsigned long int x,y,z;

  SurfaceVoxels.first = NULL;
  SurfaceVoxels.last  = NULL;

  for( z=1, z_size_xy=size_xy;
       z<size_z-1;
       z++, z_size_xy+=size_xy )
    {
      zm_size_xy = z_size_xy - size_xy;
      zp_size_xy = z_size_xy + size_xy;
      for( y=1, y_size_x=size_x;
           y<size_y-1;
           y++, y_size_x+=size_x )
        {
          ym_size_x  = y_size_x - size_x;
          yp_size_x  = y_size_x + size_x;
          for(x=1; x<size_x-1; x++)
            if ( *(image + x + y_size_x + z_size_xy ) )
              {
                if (  ( *(image +   x + ym_size_x +  z_size_xy ) ==0 ) ||
                      ( *(image +   x + yp_size_x +  z_size_xy ) ==0 ) ||
                      ( *(image +   x +  y_size_x + zm_size_xy ) ==0 ) ||
                      ( *(image +   x +  y_size_x + zp_size_xy ) ==0 ) ||
                      ( *(image + x+1 +  y_size_x +  z_size_xy ) ==0 ) ||
                      ( *(image + x-1 +  y_size_x +  z_size_xy ) ==0 )    )
                   {
                      *(image + x + y_size_x + z_size_xy ) = 2;
                      NewSurfaceVoxel2(x,y,z);
                   } /* endif */
              } /* endif */
        } /* endfor y */
    } /* endfor z */

}

void DestroyPointList2(PointList *s) {
ListElement * ptr;
	while(s->Length>0) GetFromList2(s, &ptr);
}

/*===============================================================*/
/*========= functions concerning topological properties =========*/
/*===============================================================*/

/*========= function collect_26_neighbours =========*/
void collect_26_neighbours2(unsigned long int x,
                           unsigned long int y,
                           unsigned long int z )
  {
    /*
      indices in "neighbours":
      0  1  2     9 10 11     17 18 19    y-1
      3  4  5    12    13     20 21 22     y
      6  7  8    14 15 16     23 24 25    y+1
     x-1 x x+1   x-1 x x+1    x-1 x x+1
        z-1          z           z+1
    */

    z_size_xy  = z*size_xy;
    zm_size_xy = z_size_xy - size_xy;
    zp_size_xy = z_size_xy + size_xy;
    y_size_x   = y*size_x;
    ym_size_x  = y_size_x  - size_x;
    yp_size_x  = y_size_x  + size_x;

    neighbours = 0x00000000;
    endflag = 0;

    if ( *(image + x-1 + ym_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 0];
        endflag++;
      }
    if ( *(image +   x + ym_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 1];
        endflag++;
      }
    if ( *(image + x+1 + ym_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 2];
        endflag++;
      }
    if ( *(image + x-1 +  y_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 3];
        endflag++;
      }
    if ( *(image +   x +  y_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 4];
        endflag++;
      }
    if ( *(image + x+1 +  y_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 5];
        endflag++;
      }
    if ( *(image + x-1 + yp_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 6];
        endflag++;
      }
    if ( *(image +   x + yp_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 7];
        endflag++;
      }
    if ( *(image + x+1 + yp_size_x + zm_size_xy ) )
      {
        neighbours |= long_mask[ 8];
        endflag++;
      }

    if ( *(image + x-1 + ym_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[ 9];
        endflag++;
      }
    if ( *(image +   x + ym_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[10];
        endflag++;
      }
    if ( *(image + x+1 + ym_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[11];
        endflag++;
      }
    if ( *(image + x-1 +  y_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[12];
        endflag++;
      }
    if ( *(image + x+1 +  y_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[13];
        endflag++;
      }
    if ( *(image + x-1 + yp_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[14];
        endflag++;
      }
    if ( *(image +   x + yp_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[15];
        endflag++;
      }
    if ( *(image + x+1 + yp_size_x +  z_size_xy ) )
      {
        neighbours |= long_mask[16];
        endflag++;
      }

    if ( *(image + x-1 + ym_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[17];
        endflag++;
      }
    if ( *(image +   x + ym_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[18];
        endflag++;
      }
    if ( *(image + x+1 + ym_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[19];
        endflag++;
      }
    if ( *(image + x-1 +  y_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[20];
        endflag++;
      }
    if ( *(image +   x +  y_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[21];
        endflag++;
      }
    if ( *(image + x+1 +  y_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[22];
        endflag++;
      }
    if ( *(image + x-1 + yp_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[23];
        endflag++;
      }
    if ( *(image +   x + yp_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[24];
        endflag++;
      }
    if ( *(image + x+1 + yp_size_x + zp_size_xy ) )
      {
        neighbours |= long_mask[25];
        endflag++;
      }

  }
/*========= end of function collect_26_neighbours =========*/


/*========= function simple_26_6 =========*/
int simple_26_62(unsigned long int x,
                unsigned long int y,
                unsigned long int z )
{
  collect_26_neighbours2(x,y,z);
  return ( ( *(lut_simple + (neighbours>>3) ) ) & char_mask[neighbours%8]);
}
/*========= end of function simple_26_6 =========*/


/*=========== function DetectSimpleBorderPoints ===========*/
void DetectSimpleBorderPoints2(PointList *s) {
unsigned char value;
Voxel v;
ListElement * LE3;
unsigned long int x, y, z;

  LE3=(ListElement *)SurfaceVoxels.first;
  while (LE3!=NULL)
    {
      x = (*LE3).x;
      y = (*LE3).y;
      z = (*LE3).z;
      y_size_x   = y*size_x;
      ym_size_x  = y_size_x - size_x;
      yp_size_x  = y_size_x + size_x;
      z_size_xy  = z*size_xy;
      zm_size_xy = z_size_xy - size_xy;
      zp_size_xy = z_size_xy + size_xy;
      switch( GLOBAL_direction )
        {
          case PALAGYI_U: value = *(image + x   + ym_size_x + z_size_xy  );
                  break;
          case PALAGYI_D: value = *(image + x   + yp_size_x + z_size_xy  );
                  break;
          case PALAGYI_N: value = *(image + x   + y_size_x  + zm_size_xy );
                  break;
          case PALAGYI_S: value = *(image + x   + y_size_x  + zp_size_xy );
                  break;
          case PALAGYI_E: value = *(image + x+1 + y_size_x  + z_size_xy  );
                  break;
          case PALAGYI_W: value = *(image + x-1 + y_size_x  + z_size_xy  );
                  break;
        } /* endswitch */
      if( value == 0 )
        {
          if ( simple_26_62(x, y, z) )
            {
              v.i = x;
              v.j = y;
              v.k = z;
              v.oldnei = neighbours;     /*** storing oldnei ***/
              AddToList2(s,v,LE3);
            } /* endif */
        } /* endif */
      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

}
/*=========== end of function DetectSimpleBorderPoints ===========*/


/*========= function thinning_iteration_step =========*/
unsigned int thinning_iteration_step2(void)
{
  unsigned long int i, changed, deletable, diffnei, del6nei;
  ListElement * ptr;
  PointList s;
  Voxel v;

  changed = 0;
  for ( i=0; i<6; i++ )
    {
      GLOBAL_direction = i;
      CreatePointList(&s);
      DetectSimpleBorderPoints2(&s);
      while ( s.Length > 0 )
        {
           v = GetFromList2( &s, &ptr );
           deletable = 0;
           if ( simple_26_62( v.i, v.j, v.k ) )
             {
               if ( endflag > 1 )
                 {
                   deletable = 1;    /*** simple and not endpoint ***/
                 }
               else                  /*** endpoint ***/
                 {
                   diffnei = v.oldnei ^ neighbours;
                   del6nei = 0;
                   if ( diffnei & long_mask[ 4] )
                     del6nei++;
                   if ( diffnei & long_mask[10] )
                     del6nei++;
                   if ( diffnei & long_mask[12] )
                     del6nei++;
                   if ( diffnei & long_mask[13] )
                     del6nei++;
                   if ( diffnei & long_mask[15] )
                     del6nei++;
                   if ( diffnei & long_mask[21] )
                     del6nei++;
                   if ( del6nei >= DEL6NEI )
                     deletable = 1;          /*** deletable endpoint found ***/
                 }
               if ( deletable )
                 {
                    z_size_xy = (v.k)*size_xy;
                    y_size_x  = (v.j)*size_x;
                    *(image + v.i + y_size_x + z_size_xy ) = 0; /* simple point is deleted */
                    changed ++;
                    /* investigating v's 6-neighbours */
                      if (*(image + v.i-1 + y_size_x + z_size_xy                )==1)
                        {
                          NewSurfaceVoxel2( v.i-1, v.j  , v.k   );
                          *(image + v.i-1 + y_size_x + z_size_xy                ) = 2;
                        }
                      if (*(image + v.i+1 + y_size_x        + z_size_xy         )==1)
                        {
                          NewSurfaceVoxel2( v.i+1, v.j  , v.k   );
                          *(image + v.i+1 + y_size_x        + z_size_xy         ) = 2;
                        }
                      if (*(image + v.i   + y_size_x-size_x + z_size_xy         )==1)
                        {
                          NewSurfaceVoxel2( v.i  , v.j-1, v.k   );
                          *(image + v.i   + y_size_x-size_x + z_size_xy         ) = 2;
                        }
                      if (*(image + v.i   + y_size_x+size_x + z_size_xy         )==1)
                        {
                          NewSurfaceVoxel2( v.i  , v.j+1, v.k   );
                          *(image + v.i   + y_size_x+size_x + z_size_xy         ) = 2;
                        }
                      if (*(image + v.i   + y_size_x        + z_size_xy-size_xy )==1)
                        {
                          NewSurfaceVoxel2( v.i  , v.j  , v.k-1 );
                          *(image + v.i   + y_size_x        + z_size_xy-size_xy ) = 2;
                        }
                      if (*(image + v.i   + y_size_x        + z_size_xy+size_xy )==1)
                        {
                          NewSurfaceVoxel2( v.i  , v.j  , v.k+1 );
                          *(image + v.i   + y_size_x        + z_size_xy+size_xy ) = 2;
                        }
                    RemoveSurfaceVoxel(ptr);
                 } /* endif */
             } /* endif */
        } /* endwhile */
      DestroyPointList2(&s);
    } /* endfor */

  return changed;
}
/*========= end of function thinning_iteration_step =========*/

/*========= function sequential_thinning =========*/
void sequential_thinning2(void)
{
  unsigned int iter, changed;

  CollectSurfaceVoxels2();

  iter = 0;
  changed = 1;
  while ( changed )
    {
      changed = thinning_iteration_step2();
      iter++;
#ifdef VERBOSE
      printf("\n  iteration step: %3d.    (deleted point(s): %6d)",
             iter, changed );
#endif
    }
#ifdef VERBOSE
      printf("\n");
#endif

}
/*========= end of function sequential_thinning =========*/


int32_t palagyi_skelpar_curv_06(struct xvimage *input)
{
#undef F_NAME
#define F_NAME "palagyi_skelpar_curv_06"
	char tablefilename[512];
	pgmimage=input;
	DEL6NEI = 1;
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image();

  /***************/
  /* READING LUT */
  /***************/
	sprintf(tablefilename, "%s/src/tables/palagyi_pardir_curv_06.dat", getenv("PINK"));
    init_lut_simple(tablefilename);

  /************/
  /* THINNING */
  /************/
#ifdef VERBOSE
    printf("%s: Centerline extraction by thinning ...\n", F_NAME);
#endif
    sequential_thinning2();

  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    write_image( );

  /********/
  /* FREE */
  /********/
    free(lut_simple);


    return(0);
}

/*========= function collect_26_neighbours =========*/
void collect_26_neighbours3( void )
  {

    neighbours = 0x00000000;

    if ( GLOBAL_direction == PALAGYI_UD )
      {
        /*  UD
          indices in "neighbours":
           0  1  2     9 10 11     17  18  19   y-1
           3  4  5    12    13     20      21   y
           6  7  8    14 15 16     22  23  24   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1
        */

        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr              - size_xy ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 8];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr     + size_x           ) )
          neighbours |= long_mask[15];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[16];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[24];

      } /* endif UD */

    else
      {

    if ( GLOBAL_direction == PALAGYI_NS )
      {
        /*  NS
          indices in "neighbours":
          6   7   8      3   4  5      0   1   2    y-1
         14  15  16     12     13      9  10  11     y
         22  23  24     20     21     17  18  19    y+1
         x-1  x x+1     x-1 x  x+1     x-1  x x+1
            z-1              z              z+1
        */

        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr              - size_xy ) )
	  neighbours |= long_mask[15];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[22];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[23];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr     - size_x           ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1                    ) )
          neighbours |= long_mask[13];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[21];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[10];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[19];

      } /* endif NS */

    else
      {

    if ( GLOBAL_direction == PALAGYI_EW )
      {
        /*  EW
          indices in "neighbours":
          2  11  19     1  10  18      0   9  17    y-1
          5  13  21     4              3  12  20     y
          8  16  24     7  15  23      6  14  22    y+1
         x-1  x x+1    x-1  x x+1     x-1  x  x+1
            z-1             z              z+1
        */

        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
          neighbours |= long_mask[ 2];
        if ( *(image + act_addr     - size_x - size_xy ) )
          neighbours |= long_mask[11];
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
          neighbours |= long_mask[19];
        if ( *(image + act_addr - 1          - size_xy ) )
          neighbours |= long_mask[ 5];
        if ( *(image + act_addr              - size_xy ) )
	  neighbours |= long_mask[13];
        if ( *(image + act_addr + 1          - size_xy ) )
          neighbours |= long_mask[21];
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
          neighbours |= long_mask[ 8];
        if ( *(image + act_addr     + size_x - size_xy ) )
          neighbours |= long_mask[16];
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
          neighbours |= long_mask[24];
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
          neighbours |= long_mask[ 1];
        if ( *(image + act_addr     - size_x           ) )
	  neighbours |= long_mask[10];
        if ( *(image + act_addr + 1 - size_x           ) )
          neighbours |= long_mask[18];
        if ( *(image + act_addr - 1                    ) )
          neighbours |= long_mask[ 4];
        if ( *(image + act_addr - 1 + size_x           ) )
          neighbours |= long_mask[ 7];
        if ( *(image + act_addr     + size_x           ) )
	  neighbours |= long_mask[15];
        if ( *(image + act_addr + 1 + size_x           ) )
          neighbours |= long_mask[23];
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
          neighbours |= long_mask[ 0];
        if ( *(image + act_addr     - size_x + size_xy ) )
          neighbours |= long_mask[ 9];
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
          neighbours |= long_mask[17];
        if ( *(image + act_addr - 1          + size_xy ) )
          neighbours |= long_mask[ 3];
        if ( *(image + act_addr              + size_xy ) )
          neighbours |= long_mask[12];
        if ( *(image + act_addr + 1          + size_xy ) )
          neighbours |= long_mask[20];
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
          neighbours |= long_mask[ 6];
        if ( *(image + act_addr     + size_x + size_xy ) )
          neighbours |= long_mask[14];
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
          neighbours |= long_mask[22];

      } /* endif EW */
      } /* endelse */
      } /* endelse */

  }
/*========= end of function collect_26_neighbours =========*/


/*========= function deletable =========*/
int deletable3( void )
{
  collect_26_neighbours3();
  return ( ( *(lut_deletable + (neighbours>>3) ) ) & char_mask[neighbours%8]);
}
/*========= end of function deletable =========*/


/*=========== function UpdateSurfaceList ===========*/
void UpdateSurfaceList(PointList *r) {
ListElement *LE3;
ListElement *ptr;
unsigned char surface_end;

  LE3=(ListElement *)SurfaceVoxels.first;
  while (LE3!=NULL)
    {

      act_addr = (*LE3).addr;
      surface_end = 0;

      if (  ( *(image + act_addr - 1 ) == 0 ) &&
            ( *(image + act_addr + 1 ) == 0 )     )
        {
           surface_end = 1;
	}
      else
        {  /* 1 */
          if (  ( *(image + act_addr - size_x ) == 0 ) &&
                ( *(image + act_addr + size_x ) == 0 )     )
            {
               surface_end = 1;
	    }
          else
            {  /* 2*/
              if (  ( *(image + act_addr - size_xy ) == 0 ) &&
                    ( *(image + act_addr + size_xy ) == 0 )     )
                {
                   surface_end = 1;
	        }
	    }  	/* 2 */
	}  /* 1 */

      if ( surface_end  )
        AddToList(r,LE3);

      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

  while ( r->Length > 0 )
    {
       GetFromList( r, &ptr );
       RemoveSurfaceVoxel(ptr);
    } /* endwhile */

}
/*=========== end of function UpdateSurfaceList ===========*/


/*=========== function DetectDeletablePoints ===========*/
void DetectDeletablePoints3(PointList *s) {
ListElement * LE3;
unsigned char maybe;
PointList r;

  /* update suurface list */
    CreatePointList(&r);
    UpdateSurfaceList(&r);
    DestroyPointList(&r);


  LE3=(ListElement *)SurfaceVoxels.first;
  while (LE3!=NULL)
    {

      act_addr = (*LE3).addr;

      maybe = 0;
      if ( GLOBAL_direction == PALAGYI_UD )
        {
           if ( ( ( *(image + act_addr - size_xy           ) == 0 ) &&
	          ( *(image + act_addr + size_xy           )      ) &&
		  ( *(image + act_addr + size_xy + size_xy )      )     )
		                                                                  ||
		( ( *(image + act_addr + size_xy           ) == 0 ) &&
	          ( *(image + act_addr - size_xy           )      )     )    )
          maybe = 1;
	}
      else
        {
      if ( GLOBAL_direction == PALAGYI_NS )
        {
	   if ( ( ( *(image + act_addr - size_x            ) == 0 ) &&
	          ( *(image + act_addr + size_x            )      ) &&
		  ( *(image + act_addr + size_x + size_x   )      )     )
		                                                                  ||
		( ( *(image + act_addr + size_x            ) == 0 ) &&
	          ( *(image + act_addr - size_x            )      )     )    )
          maybe = 1;
	}
      else
        {
      if ( GLOBAL_direction == PALAGYI_EW )
        {
           if ( ( ( *(image + act_addr - 1                 ) == 0 ) &&
	          ( *(image + act_addr + 1                 )      ) &&
		  ( *(image + act_addr + 2                 )      )     )
		                                                                  ||
		( ( *(image + act_addr + 1                 ) == 0 ) &&
	          ( *(image + act_addr - 1                 )      )     )    )
          maybe = 1;
	}
	}  /* endelse */
	}  /* endelse */

      if ( maybe )
        {
          if ( deletable3() )
            {
               AddToList(s,LE3);
            } /* endif */
        } /* endif */

      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

}
/*=========== end of function DetectDeletablePoints ===========*/


/*========= function thinning_iteration_step =========*/
unsigned int thinning_iteration_step3(void)
{
  unsigned long int i, changed;
  ListElement * ptr;
  PointList s;
  unsigned long int  curr_addr;

  changed = 0;
  for ( i=0; i<3; i++ )   /* 3-subiteration alg. */
    {
      GLOBAL_direction = i;
      CreatePointList(&s);
      DetectDeletablePoints3(&s);
      while ( s.Length > 0 )
        {
           curr_addr = GetFromList( &s, &ptr );
           *(image + curr_addr ) = 0;            /* deleted */
           changed ++;
           /* investigating v's 6-neighbours */
	      act_addr = curr_addr - 1;          /* (x-1,y,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + 1;          /* (x+1,y,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr - size_x;     /* (x,y-1,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + size_x;     /* (x,y+1,z) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr - size_xy;    /* (x,y,z-1) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
	      act_addr = curr_addr + size_xy;    /* (x,y,z+1) */
              if (*(image + act_addr )==1)
                {
                  NewSurfaceVoxel();
                  *(image + act_addr ) = 2;
                }
            RemoveSurfaceVoxel(ptr);
        } /* endwhile */
      DestroyPointList(&s);
    } /* endfor */

  return changed;
}
/*========= end of function thinning_iteration_step =========*/

/*========= function thinning =========*/
void thinning3(void)
{
  unsigned int iter, changed;

  CollectSurfaceVoxels();

  iter = 0;
  changed = 1;
  while ( changed )
    {
      changed = thinning_iteration_step3();
      iter++;
#ifdef VERBOSE
      printf("\n  iteration step: %3d.    (deleted point(s): %6d)",
             iter, changed );
#endif
    }
#ifdef VERBOSE
      printf("\n");
#endif

}
/*========= end of function thinning =========*/



int32_t palagyi_skelpar_surf_02(struct xvimage *input)
{
#undef F_NAME
#define F_NAME "palagyi_skelpar_surf_02"
	char tablefilename[512];
	pgmimage=input;
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image();

  /***************/
  /* READING LUT */
  /***************/
	sprintf(tablefilename, "%s/src/tables/palagyi_pardir_surf_02.dat", getenv("PINK"));
    init_lut(tablefilename);

  /************/
  /* THINNING */
  /************/
#ifdef VERBOSE
    printf("%s: 3-subiteration directional surface-thinning ...\n", F_NAME);
#endif
    thinning3();

  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    write_image( );

  /********/
  /* FREE */
  /********/
    free(lut_deletable);


    return(0);
}
/*===========================================================================
    end of function    m a i n
  ===========================================================================*/


void NewBorderVoxel( void ) {
ListElement * LE;
	LE=(ListElement *)malloc(sizeof(ListElement));
	(*LE).addr=act_addr;
	(*LE).next=NULL;
	(*LE).prev=BorderVoxels.last;
	if (BorderVoxels.last!=NULL) (*((ListElement*)(BorderVoxels.last))).next=LE;
	BorderVoxels.last=LE;
	if (BorderVoxels.first==NULL) BorderVoxels.first=LE;
}

void RemoveBorderVoxel(ListElement * LE) {
ListElement * LE2;
	if (BorderVoxels.first==LE) BorderVoxels.first=(*LE).next;
	if (BorderVoxels.last==LE) BorderVoxels.last=(*LE).prev;
	if ((*LE).next!=NULL) {
		LE2=(ListElement*)((*LE).next);
		(*LE2).prev=(*LE).prev;
	}
	if ((*LE).prev!=NULL) {
		LE2=(ListElement*)((*LE).prev);
		(*LE2).next=(*LE).next;
	}
	free(LE);
}

void CollectBorderVoxels(void) {
unsigned long int x,y,z, num;
unsigned long int z_size_xy, y_size_x;

  BorderVoxels.first = NULL;
  BorderVoxels.last  = NULL;
  num = 0;

  for( z=1, z_size_xy=size_xy;
       z<size_z-1;
       z++, z_size_xy+=size_xy )
    {
      for( y=1, y_size_x=size_x;
           y<size_y-1;
           y++, y_size_x+=size_x )
        {
          for(x=1; x<size_x-1; x++)
	    {
	      act_addr = x + y_size_x + z_size_xy;
              if ( *(image + act_addr ) )
                {
                  if (  ( *(image + act_addr     - size_x           ) ==0 ) ||
                        ( *(image + act_addr     + size_x           ) ==0 ) ||
                        ( *(image + act_addr              - size_xy ) ==0 ) ||
                        ( *(image + act_addr              + size_xy ) ==0 ) ||
                        ( *(image + act_addr + 1                    ) ==0 ) ||
                        ( *(image + act_addr - 1                    ) ==0 )    )
                     {
                        *(image + act_addr ) = 2;    /* Border point */
                        NewBorderVoxel();
		        num ++;
                     } /* endif */
                } /* endif */
            } /* endfor x */
        } /* endfor y */
    } /* endfor z */

  printf("\n Number of border points in the original image: %lu\n", num);
}


/*========= function collect_26_neighbours =========*/
void collect_26_neighbours4( void )
  {

    neighbours_UD = 0x00000000;
    neighbours_NS = 0x00000000;
    neighbours_EW = 0x00000000;

        /*

          indices in "neighbours_UD":
           0  1  2     9 10 11     17  18  19   y-1
           3  4  5    12    13     20   *  21   y
           6  7  8    14 15 16     22  23  24   y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1

          indices in "neighbours_NS":
           6  7  8     3  4  5      0   1   2    y-1
          14 15 16    12    13      9  10  11    y
          22 23 24    20  * 21     17  18  19    y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1

          indices in "neighbours_EW":
          17  9  0    18 10  1     19  11   2    y-1
          20 12  3     *     4     21  13   5    y
          22 14  6    23 15  7     24  10   8    y+1
          x-1 x x+1   x-1 x x+1    x-1  x  x+1
             z-1          z            z+1

        */

        /* face z-1 */
        if ( *(image + act_addr - 1 - size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 0];
	     neighbours_NS |= long_mask[ 6];
	     neighbours_EW |= long_mask[17];
	  }
        if ( *(image + act_addr     - size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 1];
	     neighbours_NS |= long_mask[ 7];
	     neighbours_EW |= long_mask[ 9];
	  }
        if ( *(image + act_addr + 1 - size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 2];
	     neighbours_NS |= long_mask[ 8];
	     neighbours_EW |= long_mask[ 0];
	  }
        if ( *(image + act_addr - 1          - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 3];
	     neighbours_NS |= long_mask[14];
	     neighbours_EW |= long_mask[20];
	  }
        if ( *(image + act_addr              - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 4];
	     neighbours_NS |= long_mask[15];
	     neighbours_EW |= long_mask[12];
	  }
        if ( *(image + act_addr + 1          - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 5];
	     neighbours_NS |= long_mask[16];
	     neighbours_EW |= long_mask[ 3];
	  }
        if ( *(image + act_addr - 1 + size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 6];
	     neighbours_NS |= long_mask[22];
	     neighbours_EW |= long_mask[22];
	  }
        if ( *(image + act_addr     + size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 7];
	     neighbours_NS |= long_mask[23];
	     neighbours_EW |= long_mask[14];
	  }
        if ( *(image + act_addr + 1 + size_x - size_xy ) )
	  {
             neighbours_UD |= long_mask[ 8];
	     neighbours_NS |= long_mask[24];
	     neighbours_EW |= long_mask[ 6];
	  }
        /* face z */
        if ( *(image + act_addr - 1 - size_x           ) )
	  {
             neighbours_UD |= long_mask[ 9];
	     neighbours_NS |= long_mask[ 3];
	     neighbours_EW |= long_mask[18];
	  }
        if ( *(image + act_addr     - size_x           ) )
	  {
             neighbours_UD |= long_mask[10];
	     neighbours_NS |= long_mask[ 4];
	     neighbours_EW |= long_mask[10];
	  }
        if ( *(image + act_addr + 1 - size_x           ) )
	  {
             neighbours_UD |= long_mask[11];
	     neighbours_NS |= long_mask[ 5];
	     neighbours_EW |= long_mask[ 1];
	  }
        if ( *(image + act_addr - 1                    ) )
	  {
             neighbours_UD |= long_mask[12];
	     neighbours_NS |= long_mask[12];
	   /*neighbours_EW  not considered*/
	  }
        if ( *(image + act_addr + 1                    ) )
	  {
             neighbours_UD |= long_mask[13];
	     neighbours_NS |= long_mask[13];
	     neighbours_EW |= long_mask[ 4];
	  }
        if ( *(image + act_addr - 1 + size_x           ) )
	  {
             neighbours_UD |= long_mask[14];
	     neighbours_NS |= long_mask[20];
	     neighbours_EW |= long_mask[23];
	  }
        if ( *(image + act_addr     + size_x           ) )
	  {
             neighbours_UD |= long_mask[15];
	   /*neighbours_NS not considered*/
	     neighbours_EW |= long_mask[15];
	  }
        if ( *(image + act_addr + 1 + size_x           ) )
	  {
             neighbours_UD |= long_mask[16];
	     neighbours_NS |= long_mask[21];
	     neighbours_EW |= long_mask[ 7];
	  }
        /* face z+1 */
        if ( *(image + act_addr - 1 - size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[17];
	     neighbours_NS |= long_mask[ 0];
	     neighbours_EW |= long_mask[19];
	  }
        if ( *(image + act_addr     - size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[18];
	     neighbours_NS |= long_mask[ 1];
	     neighbours_EW |= long_mask[11];
	  }
        if ( *(image + act_addr + 1 - size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[19];
	     neighbours_NS |= long_mask[ 2];
	     neighbours_EW |= long_mask[ 2];
	  }
        if ( *(image + act_addr - 1          + size_xy ) )
	  {
             neighbours_UD |= long_mask[20];
	     neighbours_NS |= long_mask[ 9];
	     neighbours_EW |= long_mask[21];
	  }
        if ( *(image + act_addr              + size_xy ) )
	  {
           /*neighbours_UD not considered*/
	     neighbours_NS |= long_mask[10];
	     neighbours_EW |= long_mask[13];
	  }
        if ( *(image + act_addr + 1          + size_xy ) )
	  {
             neighbours_UD |= long_mask[21];
	     neighbours_NS |= long_mask[11];
	     neighbours_EW |= long_mask[ 5];
	  }
        if ( *(image + act_addr - 1 + size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[22];
	     neighbours_NS |= long_mask[17];
	     neighbours_EW |= long_mask[24];
	  }
        if ( *(image + act_addr     + size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[23];
	     neighbours_NS |= long_mask[18];
	     neighbours_EW |= long_mask[16];
	  }
        if ( *(image + act_addr + 1 + size_x + size_xy ) )
	  {
             neighbours_UD |= long_mask[24];
	     neighbours_NS |= long_mask[19];
	     neighbours_EW |= long_mask[ 8];
	  }

  }
/*========= end of function collect_26_neighbours =========*/


/*========= function deletable =========*/
int deletable4( void )
{
  collect_26_neighbours4();


  /* UD test */
    if  (  ( (
               ( *(image + act_addr              - size_xy )==0 )
               &&
               ( *(image + act_addr              + size_xy )==1 )
	     )
	     ||
	     (
	       ( *(image + act_addr              - size_xy )==1 )
	       &&
               ( *(image + act_addr              + size_xy )==0 )
	     )
	   )
	   &&
           (
	     ( *(lut_deletable + (neighbours_UD>>3) ) ) & char_mask[neighbours_UD%8]
	   )
	 )
      return ( 1 );
  /* NS test */
    if  (  ( (
               ( *(image + act_addr     - size_x           )==0 )
               &&
               ( *(image + act_addr     + size_x           )==1 )
	     )
	     ||
	     (
	       ( *(image + act_addr     - size_x           )==1 )
	       &&
               ( *(image + act_addr     + size_x           )==0 )
	     )
	   )
	   &&
           (
	     ( *(lut_deletable + (neighbours_NS>>3) ) ) & char_mask[neighbours_NS%8]
	   )
	 )
      return ( 1 );
  /* EW test */
    if  (  ( (
               ( *(image + act_addr - 1                    )==0 )
               &&
               ( *(image + act_addr + 1                    )==1 )
	     )
	     ||
	     (
	       ( *(image + act_addr - 1                    )==1 )
	       &&
               ( *(image + act_addr + 1                    )==0 )
	     )
	   )
	   &&
           (
	     ( *(lut_deletable + (neighbours_EW>>3) ) ) & char_mask[neighbours_EW%8]
	   )
	 )
      return ( 1 );

   return ( 0 );
}

/*=========== function UpdateSurfaceList ===========*/
void UpdateSurfaceList4(PointList *r) {
ListElement *LE3;
ListElement *ptr;
unsigned char int_nei_exists;

  LE3=(ListElement *)BorderVoxels.first;
  while (LE3!=NULL)
    {

      act_addr = (*LE3).addr;
      int_nei_exists = 0;

      if ( *(image + act_addr - 1 ) == 1 )
        {
           int_nei_exists = 1;
	}
      else
        {  /* 1 */
          if ( *(image + act_addr + 1 ) == 1 )
            {
              int_nei_exists = 1;
	    }
          else
            {  /* 2*/
              if ( *(image + act_addr - size_x ) == 1 )
                {
                  int_nei_exists = 1;
	        }
              else
                {  /* 3 */
                  if ( *(image + act_addr + size_x ) == 1 )
                    {
                      int_nei_exists = 1;
   	            }
                  else
                    {  /* 4 */
                      if ( *(image + act_addr - size_xy ) == 1 )
                        {
                          int_nei_exists = 1;
   	                }
                      else
                        {  /* 5 */
                          if ( *(image + act_addr + size_xy ) == 1 )
                            {
                              int_nei_exists = 1;
   	                    }
	                }  /* 5 */
	            }   /* 4 */
		}   /* 3 */
	    }  	/* 2 */
	}  /* 1 */

      if ( int_nei_exists == 0 )
        AddToList(r,LE3);

      LE3=(ListElement *)(*LE3).next;
    } /* endwhile */

  while ( r->Length > 0 )
    {
       GetFromList( r, &ptr );
       RemoveBorderVoxel(ptr);
    } /* endwhile */

}
/*=========== end of function UpdateSurfaceList ===========*/


/*=========== function DetectDeletablePoints ===========*/
void DetectDeletablePoints4(PointList *s) {
ListElement * LE3;
PointList r;

  /* update suurface list */
    CreatePointList(&r);
    UpdateSurfaceList4(&r);
    DestroyPointList(&r);

  LE3=(ListElement *)BorderVoxels.first;

  while (LE3!=NULL)
    {
      act_addr = (*LE3).addr;

      if ( deletable4() )
        AddToList(s,LE3);

      LE3=(ListElement *)(*LE3).next;

    } /* endwhile */

}
/*=========== end of function DetectDeletablePoints ===========*/


/*========= function thinning_iteration_step =========*/
unsigned int thinning_iteration_step4(void)
{
  unsigned long int changed;
  ListElement * ptr;
  PointList s;
  unsigned long int  curr_addr;

  changed = 0;
  CreatePointList(&s);
  DetectDeletablePoints4(&s);

  while ( s.Length > 0 )
    {
       curr_addr = GetFromList( &s, &ptr );
       *(image + curr_addr ) = 0;            /* deleted */
       changed ++;
       /* investigating v's 6-neighbours */
	  act_addr = curr_addr - 1;          /* (x-1,y,z) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
	  act_addr = curr_addr + 1;          /* (x+1,y,z) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
	  act_addr = curr_addr - size_x;     /* (x,y-1,z) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
          act_addr = curr_addr + size_x;     /* (x,y+1,z) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
          act_addr = curr_addr - size_xy;    /* (x,y,z-1) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
	  act_addr = curr_addr + size_xy;    /* (x,y,z+1) */
          if (*(image + act_addr )==1)
            {
              NewBorderVoxel();
              *(image + act_addr ) = 2;
            }
        RemoveBorderVoxel(ptr);
    } /* endwhile */

  DestroyPointList(&s);

  return changed;
}
/*========= end of function thinning_iteration_step =========*/

/*========= function thinning =========*/
void thinning4(void)
{
  unsigned int iter, changed;

  CollectBorderVoxels();

  iter = 0;
  changed = 1;
  while ( changed )
    {
      changed = thinning_iteration_step4();
      iter++;
#ifdef VERBOSE
      printf("\n  iteration step: %3d.    (deleted point(s): %6d)",
             iter, changed );
#endif
    }
#ifdef VERBOSE
      printf("\n");
#endif

}
/*========= end of function thinning =========*/


int32_t palagyi_skelpar_surf_08(struct xvimage *input)
{
#undef F_NAME
#define F_NAME "palagyi_skelpar_surf_08"
	char tablefilename[512];
	pgmimage=input;
  /********************/
  /* READ INPUT IMAGE */
  /********************/
    read_image();

  /***************/
  /* READING LUT */
  /***************/
	sprintf(tablefilename, "%s/src/tables/palagyi_pardir_surf_08.dat", getenv("PINK"));
    init_lut(tablefilename);

  /************/
  /* THINNING */
  /************/
#ifdef VERBOSE
    printf("%s: fully parallel surface-thinning ...\n", F_NAME);
#endif
    thinning4();

  /********************/
  /* WRITE OUPUT IMAGE */
  /********************/
    write_image( );

  /********/
  /* FREE */
  /********/
    free(lut_deletable);


    return(0);
}


/* ============================================================ */
/* ============================================================ */
// Algo. de Manzanera et al., (surfacique symétrique, DGCI 1999)
// M. Couprie, oct. 2012
/* ============================================================ */
/* ============================================================ */

#define MAN_OBJECT      1
#define MAN_DELETABLE   2

#define IS_MAN_DELETABLE(f) (f&MAN_DELETABLE)
#define SET_MAN_DELETABLE(f) (f|=MAN_DELETABLE)
#define UNSET_MAN_DELETABLE(f) (f&=~MAN_DELETABLE)

// "tas" des vecteurs utiles pour les masques
// alpha2: a' b c d e f g
// alpha1: a' b f h' i j k l m
// alpha0: a' b c h' l n' o p q r
// beta1: b f' i' 
// beta0: b' l s' ou i' l c'
#define NVEC 19
static int32_t MAN_VECT[NVEC][3] = {
  {0, 0, 1}, // a
  {0, 0, -1}, // f
  {0, 1, -1}, // c
  {0, -1, -1}, // e
  {1, 0, -1}, // b
  {-1, 0, -1}, // d
  {0, 0, -2}, // g
  {-1, 0, 0}, // h
  {1, 0, 0}, // i
  {1, 1, -1}, // l
  {1, -1, -1}, // m
  {2, 0, -1}, // j
  {1, 0, -2}, // k
  {0, -1, 0}, // n
  {1, 1, 0}, // o
  {1, 2, -1}, // p
  {1, 1, -2}, // q
  {2, 1, -1}, // r
  {0, 1, 0} // s
};

static const int Ma = 0, Mf = 1, Mc = 2, Me = 3, Mb = 4, Md = 5, Mg = 6, Mh = 7, Mi = 8, Ml = 9, Mm = 10, Mj = 11, Mk = 12, Mn = 13, Mo = 14, Mp = 15, Mq = 16, Mr = 17, Ms = 18;

/* ==================================== */
static void apply_rot(int32_t * M, int32_t * V , int32_t * R)
/* ==================================== */
/*! \fn void apply_rot(in32_t * M, in32_t * V , in32_t * R)
    \param M (entrée) : matrice de rotation
    \param V (entrée) : vecteur
    \param R (sortie) : résultat
    \brief multiplication matrice \b M par vecteur \b V, résultat vecteur \b R
    \warning la mémoire pour stocker le résultat \b R doit avoir été allouée
*/
{
  int32_t i, j, t;
  for (i = 0; i < 3; i++)
  {
    t = 0;
    for (j = 0; j < 3; j++)  
      t += M[i * 3 + j] * V[j];
    R[i] = t;    
  }
} // apply_rot()

static void rotate_vectors_x(int32_t MVEC[NVEC][3])
{ // rotate around x axis
  int32_t i, *V, R[3];
  int32_t M[9] = {1, 0, 0, 0, 0, 1, 0, -1, 0};
  for (i = 0; i < NVEC; i++)
  {
    V = (int32_t *)(&(MVEC[i]));
    apply_rot((int32_t *)M, V, (int32_t *)R);
    MVEC[i][0] = R[0]; MVEC[i][1] = R[1]; MVEC[i][2] = R[2];
  }
} // rotate_vectors_x()

static void rotate_vectors_y(int32_t MVEC[NVEC][3])
{ // rotate around y axis
  int32_t i, *V, R[3];
  int32_t M[9] = {0, 0, 1, 0, 1, 0, -1, 0, 0};
  for (i = 0; i < NVEC; i++)
  {
    V = (int32_t *)(&(MVEC[i]));
    apply_rot((int32_t *)M, V, (int32_t *)R);
    MVEC[i][0] = R[0]; MVEC[i][1] = R[1]; MVEC[i][2] = R[2];
  }
} // rotate_vectors_y()

static void rotate_vectors_z(int32_t MVEC[NVEC][3])
{ // rotate around z axis
  int32_t i, *V, R[3];
  int32_t M[9] = {0, 1, 0, -1, 0, 0, 0, 0, 1};
  for (i = 0; i < NVEC; i++)
  {
    V = (int32_t *)(&(MVEC[i]));
    apply_rot((int32_t *)M, V, (int32_t *)R);
    MVEC[i][0] = R[0]; MVEC[i][1] = R[1]; MVEC[i][2] = R[2];
  }
} // rotate_vectors_z()

static index_t add_vec(int32_t MVEC[NVEC][3], uint32_t i, index_t p, index_t rs, index_t ps)
{
  int32_t x, y, z, *V;
  x = p % rs; y = (p % ps) / rs; z = p / ps;
  V = (int32_t *)(&(MVEC[i]));
  x += V[0]; y += V[1]; z += V[2];
  return z*ps + y*rs + x;
} // add_vec()

static int32_t check_MAN_ALPHA2_aux(int32_t MVEC[NVEC][3], uint8_t *S, index_t p, index_t rs, index_t ps)
{
// alpha2: a' b c d e f g
  index_t q;
  //printf("check_MAN_ALPHA2_aux begin\n");
  q = add_vec(MVEC, Ma, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mb, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mc, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Md, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Me, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mf, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mg, p, rs, ps); if (!S[q]) return 0;
  return 1;
} // check_MAN_ALPHA2_aux()

static int32_t check_MAN_ALPHA1_aux(int32_t MVEC[NVEC][3], uint8_t *S, index_t p, index_t rs, index_t ps)
{
// alpha1: a' b f h' i j k l m
  index_t q;
  q = add_vec(MVEC, Ma, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mh, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mb, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mf, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mi, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mj, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mk, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Ml, p, rs, ps); if (!S[q]) return 0;  
  q = add_vec(MVEC, Mm, p, rs, ps); if (!S[q]) return 0;  
  return 1;
} // check_MAN_ALPHA1_aux()

static int32_t check_MAN_ALPHA0_aux(int32_t MVEC[NVEC][3], uint8_t *S, index_t p, index_t rs, index_t ps)
{
// alpha0: a' b c h' l n' o p q r
  index_t q;
  q = add_vec(MVEC, Ma, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mh, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mn, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mb, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mc, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Ml, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mo, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mp, p, rs, ps); if (!S[q]) return 0;
  q = add_vec(MVEC, Mq, p, rs, ps); if (!S[q]) return 0;  
  q = add_vec(MVEC, Mr, p, rs, ps); if (!S[q]) return 0;    
  return 1;
} // check_MAN_ALPHA0_aux()

static int32_t check_MAN_BETA1_aux(int32_t MVEC[NVEC][3], uint8_t *S, index_t p, index_t rs, index_t ps)
{
// beta1: b f' i' 
  index_t q;
  q = add_vec(MVEC, Mf, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mi, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mb, p, rs, ps); if (!S[q]) return 0;  
  return 1;
} // check_MAN_BETA1_aux()

static int32_t check_MAN_BETA0_aux(int32_t MVEC[NVEC][3], uint8_t *S, index_t p, index_t rs, index_t ps)
{
// beta0: b' l s' ou i' l c'
  index_t q;
  q = add_vec(MVEC, Mb, p, rs, ps); if (S[q]) goto next;
  q = add_vec(MVEC, Ms, p, rs, ps); if (S[q]) goto next;
  q = add_vec(MVEC, Ml, p, rs, ps); if (!S[q]) goto next;    
  return 1;
 next:
  q = add_vec(MVEC, Mi, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Mc, p, rs, ps); if (S[q]) return 0;
  q = add_vec(MVEC, Ml, p, rs, ps); if (!S[q]) return 0;    
  return 1;
} // check_MAN_BETA0_aux()

static int32_t check_MAN_ALPHA2(uint8_t *S, index_t p, index_t rs, index_t ps)
{
  int32_t MVEC[NVEC][3];
  //printf("check_MAN_ALPHA2 begin\n");
  memcpy((void *)MVEC, (void *)MAN_VECT, NVEC*3*sizeof(int32_t));
  if (check_MAN_ALPHA2_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_ALPHA2_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_ALPHA2_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_ALPHA2_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  rotate_vectors_x(MVEC);
  if (check_MAN_ALPHA2_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  rotate_vectors_x(MVEC);
  if (check_MAN_ALPHA2_aux(MVEC, S, p, rs, ps)) return 1;  
  return 0;  
} // check_MAN_ALPHA2()

static int32_t check_MAN_ALPHA1(uint8_t *S, index_t p, index_t rs, index_t ps)
{
  int32_t MVEC[NVEC][3];
  memcpy((void *)MVEC, (void *)MAN_VECT, NVEC*3*sizeof(int32_t));
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  rotate_vectors_y(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA1_aux(MVEC, S, p, rs, ps)) return 1;  
  return 0;  
} // check_MAN_ALPHA1()

static int32_t check_MAN_ALPHA0(uint8_t *S, index_t p, index_t rs, index_t ps)
{
  int32_t MVEC[NVEC][3];
  memcpy((void *)MVEC, (void *)MAN_VECT, NVEC*3*sizeof(int32_t));
  if (check_MAN_ALPHA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  rotate_vectors_x(MVEC);
  rotate_vectors_x(MVEC);
  if (check_MAN_ALPHA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_ALPHA0_aux(MVEC, S, p, rs, ps)) return 1;
  return 0;  
} // check_MAN_ALPHA0()

static int32_t check_MAN_BETA1(uint8_t *S, index_t p, index_t rs, index_t ps)
{
  int32_t MVEC[NVEC][3];
  memcpy((void *)MVEC, (void *)MAN_VECT, NVEC*3*sizeof(int32_t));
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_y(MVEC);
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_x(MVEC);
  rotate_vectors_y(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA1_aux(MVEC, S, p, rs, ps)) return 1;  
  return 0;  
} // check_MAN_BETA1()

static int32_t check_MAN_BETA0(uint8_t *S, index_t p, index_t rs, index_t ps)
{
  int32_t MVEC[NVEC][3];
  memcpy((void *)MVEC, (void *)MAN_VECT, NVEC*3*sizeof(int32_t));
  if (check_MAN_BETA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  rotate_vectors_x(MVEC);
  rotate_vectors_x(MVEC);
  if (check_MAN_BETA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA0_aux(MVEC, S, p, rs, ps)) return 1;
  rotate_vectors_z(MVEC);
  if (check_MAN_BETA0_aux(MVEC, S, p, rs, ps)) return 1;
  return 0;  
} // check_MAN_BETA0()

static void check_MAN_DELETABLE(uint8_t *S, index_t p, index_t rs, index_t ps)
{
  //printf("check_MAN_DELETABLE begin\n");
  if (check_MAN_ALPHA2(S, p, rs, ps)) 
    SET_MAN_DELETABLE(S[p]);
  else 
  { 
    if (check_MAN_ALPHA1(S, p, rs, ps)) 
      SET_MAN_DELETABLE(S[p]);
    else
      if (check_MAN_ALPHA0(S, p, rs, ps)) SET_MAN_DELETABLE(S[p]);
  }
  if (IS_MAN_DELETABLE(S[p]))
  {
    if (check_MAN_BETA1(S, p, rs, ps)) 
      UNSET_MAN_DELETABLE(S[p]);
    else if (check_MAN_BETA0(S, p, rs, ps)) 
      UNSET_MAN_DELETABLE(S[p]);
  }
  //printf("check_MAN_DELETABLE end\n");
} // check_MAN_DELETABLE()

/* ==================================== */
int32_t lmanzanerasurf1999(	  struct xvimage *image,
				  struct xvimage *inhibit,				  
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lmanzanerasurf1999"
{ 
  index_t x;
  int32_t i, j, k;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL;
  int32_t step, nonstab;

  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);
  if (inhibit != NULL) 
  {
    COMPARE_SIZE(image, inhibit);
    ACCEPTED_TYPES1(inhibit, VFF_TYP_1_BYTE);
    I = UCHARDATA(inhibit);
  }
  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    step++;
    nonstab = 0;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) if (S[i]) S[i] = MAN_OBJECT;

    for (k = 2; k < ds-2; k++)
      for (j = 2; j < cs-2; j++)
	for (i = 2; i < rs-2; i++)
	{
	  x = k*ps + j*rs + i;
	  if (S[x] && (mctopo3d_nbvoisc6(S, x, rs, ps, N) > 0)) check_MAN_DELETABLE(S, x, rs, ps);
	  // l'appel à mctopo3d_nbvoisc6 est facultatif, c'est juste pour accélérer...
	} // for i, j, k

    for (k = 2; k < ds-2; k++)
    for (j = 2; j < cs-2; j++)
    for (i = 2; i < rs-2; i++)
    {
      x = k*ps + j*rs + i;
      if (S[x] && IS_MAN_DELETABLE(S[x]) && (!I || !I[x]))
      {
	S[x] = 0;
	nonstab = 1;
      }
    } // for i, j, k
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  return(1);
} /* lmanzanerasurf1999 */

/* ============================================================ */
/* ============================================================ */
// Algo. de Gong et Bertrand (curviligne 6 subiterations, 1990) 
// M. Couprie, sept. 2014
/* ============================================================ */
/* ============================================================ */

#define GB6_OBJECT      1
#define GB6_MATCHED        2

#define IS_MATCHED(f) (f&GB6_MATCHED)
#define SET_MATCHED(f) (f|=GB6_MATCHED)

static int32_t gb6_match(int32_t d, uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  uint8_t s[27]; 
  extract_vois27(S, i, rs, ps, N, s); 
  switch (d)
  {
  case 0: break;
  case 1: swap_U_L_27(s); break;
  case 2: isometrieXZ_vois27(s); break;
  case 3: isometrieXZ_vois27(s); swap_U_L_27(s); break;
  case 4: isometrieYZ_vois27(s); break;
  case 5: isometrieYZ_vois27(s); swap_U_L_27(s); break;
  }
  
  if (!s[22] || s[4]) return 0;

  if (!s[14] && s[5]) return 0;
  if (!s[10] && s[1]) return 0;
  if (!s[12] && s[3]) return 0;
  if (!s[16] && s[7]) return 0;

  if (!s[14] && !s[11] && !s[10] && s[2]) return 0;
  if (!s[14] && !s[17] && !s[16] && s[8]) return 0;
  if (!s[12] && !s[9] && !s[10] && s[0]) return 0;
  if (!s[12] && !s[15] && !s[16] && s[6]) return 0;

  return 1;
}

/* ==================================== */
int32_t lgongbertrand6dir1990(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lgongbertrand6dir1990"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (S[i]) S[i] = GB6_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (d = 0; d < 6; d++)
    {
      for (i = 0; i < N; i++) 
	if (S[i] && (mctopo3d_nbvoiso26(S, i, rs, ps, N) > 1) && gb6_match(d, S, i, rs, ps, N)) 
	  SET_MATCHED(S[i]);
      for (i = 0; i < N; i++) 
	if (IS_MATCHED(S[i])) 
	{ 
	  S[i] = 0; 
	  nonstab = 1; 
	}
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  return(1);
} /* lgongbertrand6dir1990() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Palagyi et Kuba (curviligne 12 subiterations, 1999) 
// M. Couprie, sept. 2014, d'après la thèse de C. Lohou, annexe B
/* ============================================================ */
/* ============================================================ */

#define PK12_OBJECT      1
#define PK12_MATCHED        2

#undef IS_MATCHED
#undef SET_MATCHED
#define IS_MATCHED(f) (f&PK12_MATCHED)
#define SET_MATCHED(f) (f|=PK12_MATCHED)

typedef int32_t mat[3][3];
typedef int32_t vec[3];

mat I01 = {
  {  0,  1,  0},
  { -1,  0,  0},
  {  0,  0, -1}
};

mat I02 = {
  {  0,  0,  1},
  {  1,  0,  0},
  {  0, -1,  0}
};

mat I03 = {
  {  0,  0,  1},
  {  1,  0,  0},
  {  0,  1,  0}
};

mat I04 = {
  {  0,  1,  0},
  { -1,  0,  0},
  {  0,  0,  1}
};

mat I05 = {
  {  1,  0,  0},
  {  0, -1,  0},
  {  0,  0, -1}
};

mat I06 = {
  {  1,  0,  0},
  {  0,  1,  0},
  {  0,  0, -1}
};

mat I07 = {
  {  0,  1,  0},
  {  1,  0,  0},
  {  0,  0,  1}
};

mat I08 = {
  {  0,  0,  1},
  { -1,  0,  0},
  {  0, -1,  0}
};

mat I09 = {
  {  0,  0,  1},
  { -1,  0,  0},
  {  0,  1,  0}
};

mat I10 = {
  {  0,  1,  0},
  {  1,  0,  0},
  {  0,  0, -1}
};

mat I11 = {
  {  1,  0,  0},
  {  0, -1,  0},
  {  0,  0,  1}
};

/* ==================================== */
static void multmatvec(
  mat m,
  vec v,
  vec sol)
/* ==================================== */
{
  int32_t i, j, t;
  for (i = 0; i < 3; i++)
  {
    for (t = 0, j = 0; j < 3; j++) t += m[i][j] * v[j];
    sol[i] = t;
  }
} // multmatvec()

/* ==================================== */
static void isometry(uint8_t *dest, uint8_t *sour, mat iso)
/* ==================================== */
{
  vec v, w;
  int32_t i, j, k;
  for (k = -1; k <=1; k++)
  {
    v[2] = k;
    for (j = -1; j <= 1; j++)
    {
      v[1] = j;
      for (i = -1; i <= 1; i++)
      {
	v[0] = i;
	multmatvec(iso, v, w);
	dest[(w[0]+1) + (w[1]+1)*3 + (w[2]+1)*9] = sour[i+1 + (j+1)*3 + (k+1)*9];
      }
    }
  }
} // isometry()

#ifdef DEBUG_lpalagyikuba12dir1999_b
static void testisometry()
{
  int i, j, k;
  uint8_t s[27], t[27]; 
  for (i = 0; i < 27; i++) s[i] = i; print_vois27(s); printf("\n");
  isometry(t, s, I01); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I02); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I03); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I04); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I05); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I06); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I07); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I08); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I09); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I10); print_vois27(t); printf("\n");
  for (i = 0; i < 27; i++) s[i] = i; isometry(t, s, I11); print_vois27(t); printf("\n");
}
#endif

static int32_t pk12_match(int32_t d, uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 
  switch (d)
  {
  case 0: memcpy(s, t, 27); break;
  case 1: isometry(s, t, I01); break;
  case 2: isometry(s, t, I02); break;
  case 3: isometry(s, t, I03); break;
  case 4: isometry(s, t, I04); break;
  case 5: isometry(s, t, I05); break;
  case 6: isometry(s, t, I06); break;
  case 7: isometry(s, t, I07); break;
  case 8: isometry(s, t, I08); break;
  case 9: isometry(s, t, I09); break;
  case 10: isometry(s, t, I10); break;
  case 11: isometry(s, t, I11); break;
  }

  //print_vois27(s);

// mask1:
  if (!s[22]) goto mask2;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask2;
  if (!s[9] &&  !s[10] &&  !s[11] &&  !s[12] &&  !s[14] &&  !s[15] &&  !s[16] &&  !s[17] &&
      !s[18] &&  !s[19] &&  !s[20] &&  !s[21] &&  !s[23] &&  !s[24] &&  !s[25] &&  !s[26]) goto mask2;
  return 1;

 mask2:
  if (!s[10]) goto mask3;
  if (s[6] || s[7] || s[8] || s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask3;
  if (!s[3] &&  !s[4] &&  !s[5] &&  !s[12] &&  !s[14] &&  !s[21] &&  !s[22] &&  !s[23] &&
      !s[0] &&  !s[1] &&  !s[2] &&  !s[9] &&  !s[11] &&  !s[18] &&  !s[19] &&  !s[20]) goto mask3;
  return 2;

 mask3:
  if (!s[19]) goto mask4;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
       s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask4;
  if (!s[18] &&  !s[21] &&  !s[9] &&  !s[12] &&  !s[14] &&  !s[11] &&  !s[20] &&  !s[23]) goto mask4;
  return 3;

 mask4:
  if (!s[10] || !s[22]) goto mask5;
  if (s[4] ||  s[6] || s[7] || s[8] || s[16]) goto mask5;
  if (s[3] && s[15]) goto mask5;
  if (s[5] && s[17]) goto mask5;
  return 4;

 mask5: // T7
  if (!s[10] || !s[22] || !s[14]) goto mask6;
  if (s[4] || s[6] || s[7] || s[16]) goto mask6;
  if (s[3] && s[15]) goto mask6;
  return 5;

 mask6: // T8
  if (!s[10] || !s[22] || !s[12]) goto mask7;
  if (s[4] || s[7] || s[8] || s[16]) goto mask7;
  if (s[5] && s[17]) goto mask7;
  return 6;

 mask7: // T5
  if (!s[10] || !s[22] || !s[8]) goto mask8;
  if (s[4] ||  s[6] || s[7] || s[16]) goto mask8;
  if (s[3] && s[15]) goto mask8;
  if (s[5] == s[17]) goto mask8;
  return 7;

 mask8: // T6
  if (!s[10] || !s[22] || !s[6]) goto mask9;
  if (s[4] || s[7] || s[8] || s[16]) goto mask9;
  if (s[3] == s[15]) goto mask9;
  if (s[5] && s[17]) goto mask9;
  return 8;

 mask9: // T11
  if (!s[19] || !s[23]) goto mask10;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
       s[15] || s[16] || s[24] || s[25]) goto mask10;
  return 9;

 mask10: // T12
  if (!s[19] || !s[21]) goto mask11;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
       s[17] || s[16] || s[26] || s[25]) goto mask11;
  return 10;

 mask11: // T13
  if (!s[19] || !s[11]) goto mask12;
  if (s[0] || s[1] || s[3] || s[4] || s[6] || s[7] || s[8] ||
       s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask12;
  return 11;

 mask12: // T14
  if (!s[19] || !s[9]) goto mask13;
  if (s[1] || s[2] || s[4] || s[5] || s[6] || s[7] || s[8] ||
       s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask13;
  return 12;

 mask13: // T9
  if (!s[10] || !s[14] || !s[22] || !s[6]) goto mask14;
  if (s[4] || s[7] || s[16]) goto mask14;
  if (s[3] == s[15]) goto mask14;
  return 13;

 mask14: // T10
  if (!s[10] || !s[12] || !s[22] || !s[8]) return 0;
  if (s[4] || s[7] || s[16]) return 0;
  if (s[5] == s[17]) return 0;
  return 14;
}

/* ==================================== */
int32_t lpalagyikuba12dir1999(
			      struct xvimage *image,
			      int32_t nsteps
			     )
/* ==================================== */
#undef F_NAME
#define F_NAME "lpalagyikuba12dir1999"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (S[i]) S[i] = PK12_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (d = 0; d < 12; d++)
    {
#ifdef DEBUG_lpalagyikuba12dir1999
      int code;
      for (i = 0; i < N; i++) 
	if (S[i] && (code = pk12_match(d, S, i, rs, ps, N)))
	{
          printf("matched : dir=%d i=%d code=%d\n", d, i, code);
	  SET_MATCHED(S[i]);
	}
#else
      for (i = 0; i < N; i++) 
	if (S[i] && pk12_match(d, S, i, rs, ps, N)) 
	  SET_MATCHED(S[i]);
#endif
      for (i = 0; i < N; i++) 
	if (IS_MATCHED(S[i])) 
	{ 
	  S[i] = 0; 
	  nonstab = 1; 
	}

#ifdef DEBUG_lpalagyikuba12dir1999_b
      { int n;
	for (n = i = 0; i < N; i++) if (S[i]) n++;
	printf("3. step %d subs %d reste %d\n", step, d, n);
      }
      if (d==0) writeimage(image, "_ss00");
      if (d==1) writeimage(image, "_ss01");
      if (d==2) writeimage(image, "_ss02");
      if (d==3) writeimage(image, "_ss03");
      if (d==4) writeimage(image, "_ss04");
      if (d==5) writeimage(image, "_ss05");
      if (d==6) writeimage(image, "_ss06");
      if (d==7) writeimage(image, "_ss07");
      if (d==8) writeimage(image, "_ss08");
      if (d==9) writeimage(image, "_ss09");
      if (d==10) writeimage(image, "_ss10");
      if (d==11) writeimage(image, "_ss11");
#endif

    } // for (d = 0; d < 12; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

#ifdef DEBUG_lpalagyikuba12dir1999_b
  testisometry();
#endif

  return(1);
} /* lpalagyikuba12dir1999() */


/* ============================================================ */
/* ============================================================ */
// Algo. de Bertrand et Aktouf (curviligne 8 subgrids, 1994) 
// M. Couprie, sept. 2014, d'après la thèse de C. Lohou, annexe B
/* ============================================================ */
/* ============================================================ */

#define BA_OBJECT      1
#define BA_DELETABLE   2
#define BA_TERMINAL    4

#define IS_BA_DELETABLE(f) (f&BA_DELETABLE)
#define SET_BA_DELETABLE(f) (f|=BA_DELETABLE)
#define IS_BA_TERMINAL(f) (f&BA_TERMINAL)
#define SET_BA_TERMINAL(f) (f|=BA_TERMINAL)

/* ==================================== */
int32_t lbertrandaktouf8subfields1994(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lbertrandaktouf8subfields1994"
{ 
  int32_t i, j, k, x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, substep, nonstab[8];
  int32_t top, topb;

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;
  
  for (i = 0; i < N; i++) if (S[i]) S[i] = BA_OBJECT;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  substep = step = 0;
  nonstab[0] = nonstab[1] = nonstab[2] = nonstab[3] = 1;
  nonstab[4] = nonstab[5] = nonstab[6] = nonstab[7] = 1;
  while ((nonstab[0] || nonstab[1] || nonstab[2] || nonstab[3] ||
	  nonstab[4] || nonstab[5] || nonstab[6] || nonstab[7]) && (step < nsteps))
  {
    nonstab[substep%8] = 0;
#ifdef VERBOSE
    printf("substep %d\n", substep);
#endif

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((i%2) == (substep%2)) && ((j%2) == ((substep>>1)%2)) && ((k%2) == ((substep>>2)%2)))
      {
	x = k*ps + j*rs + i;
	if (S[x] == BA_OBJECT)
	{
	  mctopo3d_top26(S, x, rs, ps, N, &top, &topb);
	  if (top >= 2) 
	  { 
#ifdef DEBUG_lbertrandaktouf8subfields1994
printf("mark terminal %d %d %d\n", i, j, k);	  
#endif
            SET_BA_TERMINAL(S[x]);
	  }
	  else if ((top == 1) && (topb == 1))
	  {
#ifdef DEBUG_lbertrandaktouf8subfields1994
printf("mark deletable %d %d %d\n", i, j, k);	  
#endif
            SET_BA_DELETABLE(S[x]);
	  }
	}
      } // for i, j, k... if

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++)
    for (i = 1; i < rs-1; i++)
      if (((i%2) == (substep%2)) && ((j%2) == ((substep>>1)%2)) && ((k%2) == ((substep>>2)%2)))
      {
	x = k*ps + j*rs + i;
	if (!IS_BA_TERMINAL(S[x]) && IS_BA_DELETABLE(S[x]))
	{
#ifdef DEBUG_lbertrandaktouf8subfields1994
printf("delete %d %d %d\n", i, j, k);	  
#endif
	  S[x] = 0;
	  nonstab[substep%8] = 1;
	}
      } // for i, j, k
    substep++;
    step = substep / 8;
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values
  mctopo3d_termine_topo3d();
  return(1);
} /* lbertrandaktouf8subfields1994() */



/* ============================================================ */
/* ============================================================ */
// Algo. de Palagyi et Kuba (curviligne hybride, 1998) 
// M. Couprie, sept. 2014, d'après la thèse de C. Lohou, annexe B
/* ============================================================ */
/* ============================================================ */

#define PKH_OBJECT      1
#define PKH_MATCHED        2

#undef IS_MATCHED
#undef SET_MATCHED
#define IS_MATCHED(a) (a&PKH_MATCHED)
#define SET_MATCHED(a) (a|=PKH_MATCHED)

// 8 dir
mat J01 = {
  { -1,  0,  0},
  {  0, -1,  0},
  {  0,  0, -1}
};

mat J02 = {
  { -1,  0,  0},
  {  0,  1,  0},
  {  0,  0,  1}
};

mat J03 = {
  {  1,  0,  0},
  {  0, -1,  0},
  {  0,  0, -1}
};

mat J04 = {
  { -1,  0,  0},
  {  0, -1,  0},
  {  0,  0,  1}
};

mat J05 = {
  {  1,  0,  0},
  {  0,  1,  0},
  {  0,  0, -1}
};

mat J06 = {
  {  1,  0,  0},
  {  0, -1,  0},
  {  0,  0,  1}
};

mat J07 = {
  { -1,  0,  0},
  {  0,  1,  0},
  {  0,  0, -1}
};

// 6 dir
mat K01 = {
  {  1,  0,  0},
  {  0,  1,  0},
  {  0,  0, -1}
};

mat K02 = {
  {  1,  0,  0},
  {  0,  0,  1},
  {  0,  1,  0}
};

mat K03 = {
  {  1,  0,  0},
  {  0,  0,  1},
  {  0, -1,  0}
};

mat K04 = {
  {  0,  1,  0},
  {  0,  0,  1},
  {  1,  0,  0}
};

mat K05 = {
  {  0,  1,  0},
  {  0,  0,  1},
  { -1,  0,  0}
};

static int32_t pkh_matchdir(int32_t d, uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 
  switch (d)
  {
  case 0: memcpy(s, t, 27); break;
  case 1: isometry(s, t, J01); break;
  case 2: isometry(s, t, J02); break;
  case 3: isometry(s, t, J03); break;
  case 4: isometry(s, t, J04); break;
  case 5: isometry(s, t, J05); break;
  case 6: isometry(s, t, J06); break;
  case 7: isometry(s, t, J07); break;
  }

  //print_vois27(s);

// mask1:
  if (!s[23]) goto mask2;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || 
      s[8] || s[9] || s[12] || s[15] || s[18] || s[21] || s[24]) goto mask2;
  if (!s[10] && !s[11] && !s[14] && !s[16] && !s[17] && !s[19] && 
      !s[20] && !s[22] && !s[25] && !s[26]) goto mask2;
  return 1;

 mask2:
  if (!s[19]) goto mask3;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || 
      s[8] || s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask3;
  if (!s[9] && !s[10] && !s[11] && !s[12] && !s[14] && !s[18] && 
      !s[20] && !s[21] && !s[22] && !s[23]) goto mask3;
  return 1;

 mask3:
  if (!s[11]) goto mask4;
  if (s[0] || s[9] || s[18] || s[3] || s[12] || s[21] || s[6] || s[7] || 
      s[8] || s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask4;
  if (!s[1] && !s[2] && !s[4] && !s[5] && !s[10] && !s[14] && 
      !s[19] && !s[20] && !s[22] && !s[23]) goto mask4;
  return 1;

 mask4:
  if (!s[11] || !s[23]) goto mask5;
  if (s[0] || s[9] || s[18] || s[3] || s[12] || s[21] || s[6] || s[15] || 
      s[24] || s[4] || s[7] || s[16] || s[8]) goto mask5;
  if (s[5] && s[17]) goto mask5;
  return 1;

 mask5:
  if (!s[19] || !s[23]) goto mask6;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || 
      s[8] || s[12] || s[15] || s[16] || s[24]) goto mask6;
  if (s[21] && s[25]) goto mask6;
  return 1;

 mask6:
  if (!s[19] || !s[11]) return 0;
  if (s[6] || s[7] || s[8] || s[15] || s[16] || s[17] || s[24] || s[25] || 
      s[26] || s[3] || s[4] || s[12] || s[0]) return 0;
  if (s[1] && s[9]) return 0;
  return 1;
}

static int32_t pkh_matchsm_1(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 6; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, K01); break;
    case 2: isometry(s, t, K02); break;
    case 3: isometry(s, t, K03); break;
    case 4: isometry(s, t, K04); break;
    case 5: isometry(s, t, K05); break;
    }
    if (!s[22]) goto endfor;
    if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto endfor;
    if (!s[9] && !s[10] && !s[11] && !s[12] && !s[14] && !s[15] && !s[16] && !s[17] &&
	!s[18] && !s[19] && !s[20] && !s[21] && !s[23] && !s[24] && !s[25] && !s[26]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 6; d++)
  return 0;
}

static int32_t pkh_matchsm_2(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 12; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, I01); break;
    case 2: isometry(s, t, I02); break;
    case 3: isometry(s, t, I03); break;
    case 4: isometry(s, t, I04); break;
    case 5: isometry(s, t, I05); break;
    case 6: isometry(s, t, I06); break;
    case 7: isometry(s, t, I07); break;
    case 8: isometry(s, t, I08); break;
    case 9: isometry(s, t, I09); break;
    case 10: isometry(s, t, I10); break;
    case 11: isometry(s, t, I11); break;
    }
    if (!s[10] || !s[22]) goto endfor;
    if (s[4] ||  s[6] || s[7] || s[8] || s[16]) goto endfor;
    if (s[3] && s[15]) goto endfor;
    if (s[5] && s[17]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 12; d++)
  return 0;
}

static int32_t pkh_matchsm_3a(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 12; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, I01); break;
    case 2: isometry(s, t, I02); break;
    case 3: isometry(s, t, I03); break;
    case 4: isometry(s, t, I04); break;
    case 5: isometry(s, t, I05); break;
    case 6: isometry(s, t, I06); break;
    case 7: isometry(s, t, I07); break;
    case 8: isometry(s, t, I08); break;
    case 9: isometry(s, t, I09); break;
    case 10: isometry(s, t, I10); break;
    case 11: isometry(s, t, I11); break;
    }
    if (!s[10] || !s[22] || !s[12]) goto endfor;
    if (s[4] || s[7] || s[8] || s[16]) goto endfor;
    if (s[5] && s[17]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 12; d++)
  return 0;
}

static int32_t pkh_matchsm_3b(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 12; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, I01); break;
    case 2: isometry(s, t, I02); break;
    case 3: isometry(s, t, I03); break;
    case 4: isometry(s, t, I04); break;
    case 5: isometry(s, t, I05); break;
    case 6: isometry(s, t, I06); break;
    case 7: isometry(s, t, I07); break;
    case 8: isometry(s, t, I08); break;
    case 9: isometry(s, t, I09); break;
    case 10: isometry(s, t, I10); break;
    case 11: isometry(s, t, I11); break;
    }
    if (!s[10] || !s[22] || !s[14]) goto endfor;
    if (s[4] ||  s[6] || s[7] || s[16]) goto endfor;
    if (s[3] && s[15]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 12; d++)
  return 0;
}

static int32_t pkh_matchsm_4a(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 6; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, K01); break;
    case 2: isometry(s, t, K02); break;
    case 3: isometry(s, t, K03); break;
    case 4: isometry(s, t, K04); break;
    case 5: isometry(s, t, K05); break;
    }
    if (!s[22] || !s[12] || !s[14]) goto endfor;
    if (s[1] || s[4] || s[7]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 6; d++)
  return 0;
}

static int32_t pkh_matchsm_4b(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 6; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, K01); break;
    case 2: isometry(s, t, K02); break;
    case 3: isometry(s, t, K03); break;
    case 4: isometry(s, t, K04); break;
    case 5: isometry(s, t, K05); break;
    }
    if (!s[22] || !s[10] || !s[16]) goto endfor;
    if (s[3] || s[4] || s[5]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 6; d++)
  return 0;
}

static int32_t pkh_matchsm_5(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 12; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, I01); break;
    case 2: isometry(s, t, I02); break;
    case 3: isometry(s, t, I03); break;
    case 4: isometry(s, t, I04); break;
    case 5: isometry(s, t, I05); break;
    case 6: isometry(s, t, I06); break;
    case 7: isometry(s, t, I07); break;
    case 8: isometry(s, t, I08); break;
    case 9: isometry(s, t, I09); break;
    case 10: isometry(s, t, I10); break;
    case 11: isometry(s, t, I11); break;
    }
    if (!s[10] || !s[22] || !s[12] || !s[14]) goto endfor;
    if (s[4] || s[7] || s[16]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 12; d++)
  return 0;
}

static int32_t pkh_matchsm_6(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 6; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, K01); break;
    case 2: isometry(s, t, K02); break;
    case 3: isometry(s, t, K03); break;
    case 4: isometry(s, t, K04); break;
    case 5: isometry(s, t, K05); break;
    }
    if (!s[22] || !s[10] || !s[16] || !s[12] || !s[14]) goto endfor;
    if (s[4]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 6; d++)
  return 0;
}

static int32_t pkh_matchsm_7(uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  int d;
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 

  for (d = 0; d < 8; d++)
  {
    switch (d)
    {
    case 0: memcpy(s, t, 27); break;
    case 1: isometry(s, t, J01); break;
    case 2: isometry(s, t, J02); break;
    case 3: isometry(s, t, J03); break;
    case 4: isometry(s, t, J04); break;
    case 5: isometry(s, t, J05); break;
    case 6: isometry(s, t, J06); break;
    case 7: isometry(s, t, J07); break;
    }
    if (!s[20]) goto endfor;
    if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
	s[9] || s[10] || s[12] || s[14] || s[15] || s[16] || s[17] ||
	s[18] || s[21] || s[22] || s[24] || s[25] || s[26]) goto endfor;
    if (!s[11] && !s[19] && !s[23]) goto endfor;
    return 1;
  endfor:;
  } // for (d = 0; d < 6; d++)
  return 0;
}

/* ==================================== */
int32_t lpalagyikubahybrid1998(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lpalagyikubahybrid1998"
{
  index_t i, j, k, x;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d, f;

  if (nsteps == -1) nsteps = 1000000000;

  for (x = 0; x < N; x++) if (S[x]) assert(!bord3d(x, rs, ps, N));
  for (x = 0; x < N; x++) if (S[x]) S[x] = PKH_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    //directional
    for (d = 0; d < 8; d++)
    {
#ifdef DEBUG_lpalagyikubahybrid1998
printf("substep %d dir %d\n", step, d);
#endif
      for (x = 0; x < N; x++) 
	if (S[x] && pkh_matchdir(d, S, x, rs, ps, N)) 
	  SET_MATCHED(S[x]);
      for (x = 0; x < N; x++) 
	if (IS_MATCHED(S[x])) 
	{ 
	  S[x] = 0; 
	  nonstab = 1; 
	}
    } // for (d = 0; d < 8; d++)

    // subfields
    for (f = 0; f < 2; f++)  // the 2 subfields
    {
#ifdef DEBUG_lpalagyikubahybrid1998
printf("substep %d field %d\n", step, f);
#endif
      for (k = 1; k < ds-1; k++)
      for (j = 1; j < cs-1; j++)
      for (i = 1; i < rs-1; i++)
      if (((k+j+i)%2) == f)
      {
	x = k*ps + j*rs + i;
	if (S[x])
	{
	  if (pkh_matchsm_1(S, x, rs, ps, N) ||
	      pkh_matchsm_2(S, x, rs, ps, N) ||
	      pkh_matchsm_3a(S, x, rs, ps, N) ||
	      pkh_matchsm_3b(S, x, rs, ps, N) ||
	      pkh_matchsm_4a(S, x, rs, ps, N) ||
	      pkh_matchsm_4b(S, x, rs, ps, N) ||
	      pkh_matchsm_5(S, x, rs, ps, N) ||
	      pkh_matchsm_6(S, x, rs, ps, N) ||
	      pkh_matchsm_7(S, x, rs, ps, N)
	     )
	    SET_MATCHED(S[x]);
	}
      } // for k for j for i if
      for (x = 0; x < N; x++) 
	if (IS_MATCHED(S[x])) 
	{ 
	  S[x] = 0; 
	  nonstab = 1; 
	} // for (x = 0; x < N; x++) 
    } // for (f = 0; f < 1; f++)    
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  return(1);
} /* lpalagyikubahybrid1998() */


/* ============================================================ */
/* ============================================================ */
// Algo. de Palagyi et Kuba (curviligne 8 subiterations, 1999) 
// M. Couprie, oct. 2014
/* ============================================================ */
/* ============================================================ */

#define PK8_OBJECT      1
#define PK8_MATCHED        2

#undef IS_MATCHED
#undef SET_MATCHED
#define IS_MATCHED(a) (a&PK8_MATCHED)
#define SET_MATCHED(a) (a|=PK8_MATCHED)

// masks generated using program $PINK/private/src/com/GenMasksPK8dir.c
static int32_t pk8_matchdir(int32_t d, uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 
  switch (d)
  {
  case 0: memcpy(s, t, 27); break;
  case 1: isometry(s, t, J01); break;
  case 2: isometry(s, t, J02); break;
  case 3: isometry(s, t, J03); break;
  case 4: isometry(s, t, J04); break;
  case 5: isometry(s, t, J05); break;
  case 6: isometry(s, t, J06); break;
  case 7: isometry(s, t, J07); break;
  }

  //mask0:
  if (!s[22]) goto mask1;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask1;
  if (!s[9] && !s[10] && !s[11] && !s[12] && !s[14] && !s[15] && !s[16] && !s[17] && !s[18] && !s[19] && !s[20] && !s[21] && !s[23] && !s[24] && !s[25] && !s[26]) goto mask1;
  return 1;

mask1:
  if (!s[22]) goto mask2;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask2;
  if (!s[9] && !s[10] && !s[11] && !s[12] && !s[14] && !s[15] && !s[16] && !s[17] && !s[18] && !s[19] && !s[20] && !s[21] && !s[23] && !s[24] && !s[25] && !s[26]) goto mask2;
  return 1;

mask2:
  if (!s[10]) goto mask3;
  if (s[6] || s[7] || s[8] || s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask3;
  if (!s[0] && !s[1] && !s[2] && !s[3] && !s[4] && !s[5] && !s[9] && !s[11] && !s[12] && !s[14] && !s[18] && !s[19] && !s[20] && !s[21] && !s[22] && !s[23]) goto mask3;
  return 1;

mask3:
  if (!s[14]) goto mask4;
  if (s[0] || s[3] || s[6] || s[9] || s[12] || s[15] || s[18] || s[21] || s[24]) goto mask4;
  if (!s[1] && !s[2] && !s[4] && !s[5] && !s[7] && !s[8] && !s[10] && !s[11] && !s[16] && !s[17] && !s[19] && !s[20] && !s[22] && !s[23] && !s[25] && !s[26]) goto mask4;
  return 1;

mask4:
  if (!s[20]) goto mask5;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[9] || s[12] || s[15] || s[16] || s[17] || s[18] || s[21] || s[24] || s[25] || s[26]) goto mask5;
  if (!s[10] && !s[11] && !s[14] && !s[19] && !s[22] && !s[23]) goto mask5;
  return 1;

mask5:
  if (!s[20]) goto mask6;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[9] || s[12] || s[15] || s[16] || s[17] || s[18] || s[21] || s[24] || s[25] || s[26]) goto mask6;
  if (!s[10] && !s[11] && !s[14] && !s[19] && !s[22] && !s[23]) goto mask6;
  return 1;

mask6:
  if (!s[20]) goto mask7;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[9] || s[12] || s[15] || s[16] || s[17] || s[18] || s[21] || s[24] || s[25] || s[26]) goto mask7;
  if (!s[10] && !s[11] && !s[14] && !s[19] && !s[22] && !s[23]) goto mask7;
  return 1;

mask7:
  if (!s[20]) goto mask8;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[9] || s[12] || s[15] || s[16] || s[17] || s[18] || s[21] || s[24] || s[25] || s[26]) goto mask8;
  if (!s[10] && !s[11] && !s[14] && !s[19] && !s[22] && !s[23]) goto mask8;
  return 1;

mask8:
  if (!s[23]) goto mask9;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[9] || s[12] || s[15] || s[18] || s[21] || s[24]) goto mask9;
  if (!s[10] && !s[11] && !s[14] && !s[16] && !s[17] && !s[19] && !s[20] && !s[22] && !s[25] && !s[26]) goto mask9;
  return 1;

mask9:
  if (!s[19]) goto mask10;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask10;
  if (!s[9] && !s[10] && !s[11] && !s[12] && !s[14] && !s[18] && !s[20] && !s[21] && !s[22] && !s[23]) goto mask10;
  return 1;

mask10:
  if (!s[11]) goto mask11;
  if (s[0] || s[3] || s[6] || s[7] || s[8] || s[9] || s[12] || s[15] || s[16] || s[17] || s[18] || s[21] || s[24] || s[25] || s[26]) goto mask11;
  if (!s[1] && !s[2] && !s[4] && !s[5] && !s[10] && !s[14] && !s[19] && !s[20] && !s[22] && !s[23]) goto mask11;
  return 1;

mask11:
  if (!s[23]) goto mask12;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[9] || s[12] || s[15] || s[18] || s[21] || s[24]) goto mask12;
  if (!s[10] && !s[11] && !s[14] && !s[16] && !s[17] && !s[19] && !s[20] && !s[22] && !s[25] && !s[26]) goto mask12;
  return 1;

mask12:
  if (!s[19] || !s[23]) goto mask13;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[12] || s[15] || s[16] || s[21] || s[22] || s[24] || s[25]) goto mask13;
  return 1;

mask13:
  if (!s[19] || !s[23]) goto mask14;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[12] || s[15] || s[16] || s[21] || s[22] || s[24] || s[25]) goto mask14;
  return 1;

mask14:
  if (!s[11] || !s[19]) goto mask15;
  if (s[0] || s[1] || s[3] || s[4] || s[6] || s[7] || s[8] || s[9] || s[10] || s[12] || s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask15;
  return 1;

mask15:
  if (!s[11] || !s[23]) goto mask16;
  if (s[0] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[9] || s[12] || s[14] || s[15] || s[16] || s[17] || s[18] || s[21] || s[24]) goto mask16;
  return 1;

mask16:
  if (!s[10] || !s[22]) goto mask17;
  if (s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask17;
  return 1;

mask17:
  if (!s[14] || !s[22]) goto mask18;
  if (s[0] || s[1] || s[3] || s[4] || s[6] || s[7]) goto mask18;
  return 1;

mask18:
  if (!s[10] || !s[22]) goto mask19;
  if (s[6] || s[7] || s[8] || s[15] || s[16] || s[17]) goto mask19;
  return 1;

mask19:
  if (!s[10] || !s[14]) goto mask20;
  if (s[3] || s[6] || s[12] || s[15] || s[21] || s[24]) goto mask20;
  return 1;

mask20:
  if (!s[10] || !s[14] || !s[22]) goto mask21;
  if (s[3] || s[4] || s[6] || s[7]) goto mask21;
  return 1;

mask21:
  if (!s[10] || !s[14] || !s[22]) goto mask22;
  if (s[3] || s[4] || s[6] || s[7]) goto mask22;
  return 1;

mask22:
  if (!s[10] || !s[14] || !s[22]) goto mask23;
  if (s[6] || s[7] || s[15] || s[16]) goto mask23;
  return 1;

mask23:
  if (!s[10] || !s[14] || !s[22]) goto mask24;
  if (s[3] || s[6] || s[12] || s[15]) goto mask24;
  return 1;

mask24:
  if (!s[2] || !s[11] || !s[22]) goto mask25;
  if (s[0] || s[1] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask25;
  return 1;

mask25:
  if (!s[2] || !s[11] || !s[22]) goto mask26;
  if (s[0] || s[1] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask26;
  return 1;

mask26:
  if (!s[10] || !s[23] || !s[26]) goto mask27;
  if (s[6] || s[7] || s[8] || s[15] || s[16] || s[17] || s[24] || s[25]) goto mask27;
  return 1;

mask27:
  if (!s[14] || !s[18] || !s[19]) goto mask28;
  if (s[0] || s[3] || s[6] || s[9] || s[12] || s[15] || s[21] || s[24]) goto mask28;
  return 1;

mask28:
  return 0;
}

/* ==================================== */
int32_t lpalagyikuba8dir1999(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lpalagyikuba8dir1999"
{
  index_t i, x;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  if (nsteps == -1) nsteps = 1000000000;

  for (x = 0; x < N; x++) if (S[x]) assert(!bord3d(x, rs, ps, N));
  for (x = 0; x < N; x++) if (S[x]) S[x] = PK8_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (d = 0; d < 8; d++)
    {
#ifdef DEBUG_lpalagyikuba8dir1999
printf("substep %d dir %d\n", step, d);
#endif
      for (x = 0; x < N; x++) 
	if (S[x] && pk8_matchdir(d, S, x, rs, ps, N)) 
	  SET_MATCHED(S[x]);
      for (x = 0; x < N; x++) 
	if (IS_MATCHED(S[x])) 
	{ 
	  S[x] = 0; 
	  nonstab = 1; 
	}
    } // for (d = 0; d < 8; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  return(1);
} /* lpalagyikuba8dir1999() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Raynal et Couprie (curviligne 6 subiterations, 2010) 
// M. Couprie, sept. 2014
/* ============================================================ */
/* ============================================================ */

#define RC6_OBJECT          1
#define RC6_DELETABLE       2
#define RC6_ISTHMUS         4

#define IS_DELETABLE(f) (f&RC6_DELETABLE)
#define SET_DELETABLE(f) (f|=RC6_DELETABLE)
#define SET_ISTHMUS(f) (f|=RC6_ISTHMUS)

static int32_t rc6_match(int32_t d, uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  uint8_t s[27]; 
  extract_vois27(S, i, rs, ps, N, s); 
  switch (d)
  {
  case 0: break;
  case 1: swap_U_L_27(s); break;
  case 2: isometrieXZ_vois27(s); break;
  case 3: isometrieXZ_vois27(s); swap_U_L_27(s); break;
  case 4: isometrieYZ_vois27(s); break;
  case 5: isometrieYZ_vois27(s); swap_U_L_27(s); break;
  }
  
  if (!s[22]) goto mask2a;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask2a;
  return 1;

 mask2a:
  if (!s[22] || !s[10]) goto mask2b;
  if (s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask2b;
  return 1;

 mask2b:
  if (!s[22] || !s[14]) goto mask2c;
  if (s[0] || s[1] || s[3] || s[4] || s[6] || s[7]) goto mask2c;
  return 1;

 mask2c:
  if (!s[22] || !s[16]) goto mask2d;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5]) goto mask2d;
  return 1;

 mask2d:
  if (!s[22] || !s[12]) goto mask3a;
  if (s[1] || s[2] || s[4] || s[5] || s[7] || s[8]) goto mask3a;
  return 1;

 mask3a:
  if (!s[22] || !s[10] || !s[14]) goto mask3b;
  if (s[3] || s[4] || s[6] || s[7]) goto mask3b;
  return 1;

 mask3b:
  if (!s[22] || !s[14] || !s[16]) goto mask3c;
  if (s[0] || s[1] || s[3] || s[4]) goto mask3c;
  return 1;

 mask3c:
  if (!s[22] || !s[16] || !s[12]) goto mask3d;
  if (s[1] || s[2] || s[4] || s[5]) goto mask3d;
  return 1;

 mask3d:
  if (!s[22] || !s[12] || !s[10]) goto mask4a;
  if (s[4] || s[5] || s[7] || s[8]) goto mask4a;
  return 1;

 mask4a:
  if (!s[22] || !s[11] || !s[2]) goto mask4b;
  if (s[0] || s[1] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask4b;
  return 1;

 mask4b:
  if (!s[22] || !s[17] || !s[8]) goto mask4c;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7]) goto mask4c;
  return 1;

 mask4c:
  if (!s[22] || !s[15] || !s[6]) goto mask4d;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[7] || s[8]) goto mask4d;
  return 1;

 mask4d:
  if (!s[22] || !s[9] || !s[0]) goto mask5a;
  if (s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask5a;
  return 1;

 mask5a:
  if (!s[19]) goto mask5b;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[22]
       || s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask5b;
  return 1;

 mask5b:
  if (!s[23]) goto mask5c;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[22]
       || s[9] || s[12] || s[15] || s[18] || s[21] || s[24]) goto mask5c;
  return 1;

 mask5c:
  if (!s[25]) goto mask5d;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[22]
       || s[9] || s[10] || s[11] || s[18] || s[19] || s[20]) goto mask5d;
  return 1;

 mask5d:
  if (!s[21]) goto mask6a;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[22]
       || s[11] || s[14] || s[17] || s[20] || s[23] || s[26]) goto mask6a;
  return 1;

 mask6a:
  if (!s[19] || !s[23]) goto mask6b;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[22]
       || s[12] || s[15] || s[16] || s[21] || s[24] || s[25]) goto mask6b;
  return 1;

 mask6b:
  if (!s[23] || !s[25]) goto mask6c;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[22]
       || s[12] || s[9] || s[10] || s[21] || s[18] || s[19]) goto mask6c;
  return 1;

 mask6c:
  if (!s[25] || !s[21]) goto mask6d;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[22]
       || s[10] || s[11] || s[14] || s[19] || s[20] || s[23]) goto mask6d;
  return 1;

 mask6d:
  if (!s[21] || !s[19]) goto mask7a;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[22]
       || s[14] || s[17] || s[16] || s[23] || s[26] || s[25]) goto mask7a;
  return 1;

 mask7a:
  if (!s[20]) goto mask7b;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
      s[9] || s[10] || s[11] || s[12] || s[14] || s[15] || s[16] || s[17] ||
      s[18] || s[19] || s[21] || s[22] || s[23] || s[24] || s[25] || s[26]
      ) goto mask7b;
  return 1;

 mask7b:
  if (!s[26]) goto mask7c;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
      s[9] || s[10] || s[11] || s[12] || s[14] || s[15] || s[16] || s[17] ||
      s[18] || s[19] || s[20] || s[21] || s[22] || s[23] || s[24] || s[25]
      ) goto mask7c;
  return 1;

 mask7c:
  if (!s[24]) goto mask7d;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
      s[9] || s[10] || s[11] || s[12] || s[14] || s[15] || s[16] || s[17] ||
      s[18] || s[19] || s[20] || s[21] || s[22] || s[23] || s[25] || s[26]
      ) goto mask7d;
  return 1;

 mask7d:
  if (!s[18]) return 0;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
      s[9] || s[10] || s[11] || s[12] || s[14] || s[15] || s[16] || s[17] ||
      s[19] || s[20] || s[21] || s[22] || s[23] || s[24] || s[25] || s[26]
      ) return 0;
  return 1;
}

/* ==================================== */
int32_t lraynalcouprie6dir2010(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lraynalcouprie6dir2010"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;
  int32_t top, topb;

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (S[i]) S[i] = RC6_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (i = 0; i < N; i++) 
      if (S[i] == RC6_OBJECT)
      {
	mctopo3d_top26(S, i, rs, ps, N, &top, &topb);
	if (top >= 2) 
	  SET_ISTHMUS(S[i]);
      }

    for (d = 0; d < 6; d++)
    {
      for (i = 0; i < N; i++) 
	if ((S[i] == RC6_OBJECT) && rc6_match(d, S, i, rs, ps, N))
	  SET_DELETABLE(S[i]);
      for (i = 0; i < N; i++) 
	if (IS_DELETABLE(S[i])) 
	{ 
	  S[i] = 0; 
	  nonstab = 1; 
	}
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();

  return(1);
} /* lraynalcouprie6dir2010() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Saha et al. (8 subfields, 1997) 
// M. Couprie, oct. 2014
/* ============================================================ */
/* ============================================================ */

#define IndexPoint(v) (v[0]+rs*v[1]+ps*v[2])
#define WhiteBeforeStep(x) ((x)<INT_MIN+i)
#define BlackBeforeStep(x) ((x)>=INT_MIN+i)

static void delete(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  X[IndexPoint(x)] = INT_MIN+i;
}

static void mark(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  X[IndexPoint(x)] = i;
}

static int32_t is_marked(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  if (X[IndexPoint(x)] > 0) return 1;
  return 0;
}

static vec dirs[3] = { {1,0,0},  {0,1,0},  {0,0,1} };

static void setvec(int32_t i, int32_t j, int32_t k, vec r) 
{
  r[0] = i; r[1] = j; r[2] = k;
}

static void addvec(vec v1, vec v2, vec r) 
{
  r[0] = v1[0] + v2[0]; r[1] = v1[1] + v2[1]; r[2] = v1[2] + v2[2];
}

static void scalevec(vec v, int32_t s, vec r) 
{
  r[0] = s * v[0]; r[1] = s * v[1]; r[2] = s * v[2];
}

static void e_(vec a, vec b, vec x, vec r) 
{
  addvec(a, b, r); addvec(r, x, r);
}

static void v_(vec a, vec b, vec c, vec x, vec r) 
{
  addvec(a, b, r); addvec(r, c, r); addvec(r, x, r);
}

static void f1(vec a, vec x, vec r) 
{
  addvec(a, a, r); addvec(r, x, r);
}

static void f2(vec a, vec b, vec x, vec r) 
{
  addvec(a, a, r); addvec(r, b, r); addvec(r, x, r);
}

static void f3(vec a, vec b, vec x, vec r) 
{
  addvec(a, a, r); addvec(r, b, r); addvec(r, b, r); addvec(r, x, r);
}

static void prinvec(vec v) 
{
  printf("%d %d %d\n", v[0], v[1], v[2]);
}

static int32_t s_open(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t a, s, val;
  vec t;
  // X[x] is supposed to be black (not checked)
  for (a = 0; a < 3; a++)
    for (s = -1; s <= 1; s += 2)
    {
      scalevec(dirs[a], s, t);
      addvec(t, x, t);
      val = X[IndexPoint(t)];
      if (WhiteBeforeStep(val)) return 1;
    }
  return 0;
} // s_open()

static int32_t e_open(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t a, sa, b, sb, ve, vfa, vfb;
  vec ta, tb, e, fa, fb;
  // X[x] is supposed to be black (not checked)
  if (s_open(X, x, rs, ps, i)) return 0;
  for (a = 0; a < 3; a++)
    for (sa = -1; sa <= 1; sa += 2)
      for (b = a+1; b < 3; b++)
	for (sb = -1; sb <= 1; sb += 2)
	{
	  scalevec(dirs[a], sa, ta); scalevec(dirs[b], sb, tb);
	  e_(ta, tb, x, e); ve = X[IndexPoint(e)];
	  f1(ta, x, fa); vfa = X[IndexPoint(fa)];
	  f1(tb, x, fb); vfb = X[IndexPoint(fb)];
	  if ((WhiteBeforeStep(ve)) && (BlackBeforeStep(vfa)) && (BlackBeforeStep(vfb)))
	  {
//printf("e_open match point %d %d %d a = %d sa = %d b = %d sb = %d\n", x[0], x[1], x[2], a, sa, b, sb);//DEBUG
	    return 1;
	  }
	}
  return 0;
} // e_open()

static int32_t v_open(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t a, sa, b, sb, c, sc, vv, vfa, vfb, vfc;
  vec ta, tb, tc, v, fa, fb, fc;
  // X[x] is supposed to be black (not checked)
  if (s_open(X, x, rs, ps, i)) return 0;
  if (e_open(X, x, rs, ps, i)) return 0;
  a = 0; b = 1; c = 2;
  for (sa = -1; sa <= 1; sa += 2)
    for (sb = -1; sb <= 1; sb += 2)
      for (sc = -1; sc <= 1; sc += 2)
      {
	scalevec(dirs[a], sa, ta); scalevec(dirs[b], sb, tb); scalevec(dirs[c], sc, tc);
	v_(ta, tb, tc, x, v); vv = X[IndexPoint(v)];
	f1(ta, x, fa); vfa = X[IndexPoint(fa)];
	f1(tb, x, fb); vfb = X[IndexPoint(fb)];
	f1(tc, x, fc); vfc = X[IndexPoint(fc)];
	if ((WhiteBeforeStep(vv)) && (BlackBeforeStep(vfa)) && 
	                (BlackBeforeStep(vfb)) && (BlackBeforeStep(vfc)))
	    return 1;
	}
  return 0;
} // v_open()

static int32_t condition1(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t val, n, g, h;
  vec t;
  // X[x] is supposed to be black (not checked)
  // dir 0 : orthogonal to (-1,0,0) - (1,0,0)
  setvec(0,1,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
  setvec(0,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
  setvec(0,-1,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
  setvec(0,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
  setvec(0,1,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;

  setvec(0,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(0,-2,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
    setvec(0,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
    setvec(0,-2,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;    
  }

  setvec(0,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(0,1,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
    setvec(0,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
    setvec(0,-1,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;    
  }

  setvec(0,-1,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(0,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
    setvec(0,-2,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;
    setvec(0,-2,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;    
    setvec(0,-1,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;    
    setvec(0,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1;    
  }

  n = 0;
  for (g = -1; g <= 1; g++)
    for (h = -1; h <= 1; h++)
    {
      setvec(-1,g,h,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
    }
  if (n == 0) goto dir1;

  n = 0;
  for (g = -1; g <= 1; g++)
    for (h = -1; h <= 1; h++)
    {
      setvec(1,g,h,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
    }
  if (n == 0) goto dir1;
      
  return 1;

 dir1:
  // dir 1 : orthogonal to (0,-1,0) - (0,1,0)
  setvec(1,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
  setvec(0,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
  setvec(-1,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
  setvec(1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
  setvec(1,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;

  setvec(-1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(-2,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
    setvec(-2,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
    setvec(-2,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;    
  }

  setvec(0,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(1,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
    setvec(0,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
    setvec(-1,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;    
  }

  setvec(-1,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(-2,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
    setvec(-2,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
    setvec(-2,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;    
    setvec(-1,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;    
    setvec(0,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;    
  }

  n = 0;
  for (g = -1; g <=1; g++)
    for (h = -1; h <=1; h++)
    {
      setvec(g,-1,h,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
    }
  if (n == 0) goto dir2;

  n = 0;
  for (g = -1; g <=1; g++)
    for (h = -1; h <=1; h++)
    {
      setvec(g,1,h,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
    }
  if (n == 0) goto dir2;
      
  return 1;

 dir2:
  // dir 2 : orthogonal to (0,0,-1) - (0,0,1)
  setvec(1,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
  setvec(0,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
  setvec(-1,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
  setvec(1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
  setvec(1,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;

  setvec(-1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(-2,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
    setvec(-2,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
    setvec(-2,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;    
  }

  setvec(0,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(1,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
    setvec(0,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
    setvec(-1,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;    
  }

  setvec(-1,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  if (BlackBeforeStep(val)) 
  {
    setvec(-2,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
    setvec(-2,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;
    setvec(-2,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;    
    setvec(-1,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;    
    setvec(0,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out;    
  }

  n = 0;
  for (g = -1; g <=1; g++)
    for (h = -1; h <=1; h++)
    {
      setvec(g,h,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
    }
  if (n == 0) goto out;

  n = 0;
  for (g = -1; g <=1; g++)
    for (h = -1; h <=1; h++)
    {
      setvec(g,h,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
    }
  if (n == 0) goto out;
      
  return 1;

 out:
  return 0;

} // condition1()

static int32_t condition2(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t val, val2, n, f, g, h;
  vec t;
  // X[x] is supposed to be black (not checked)

  // dir 0 : (-1,0,0) - (1,0,0)
  setvec(1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir1; // a is white
  setvec(-1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)];   
  setvec(-2,0,0,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((BlackBeforeStep(val)) && (BlackBeforeStep(val2))) goto dir1;
  for (f = -1; f <= 1; f++)
    for (g = -1; g <= 1; g++)
      if ((f != 0) || (g != 0))
      {
	n = 0;
	for (h = -1; h <= 1; h++)
	{
	  setvec(h,f,g,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
	}
	if (n == 0) goto dir1;	
      }
#ifdef DEBUGSAHA
printf("condition2 point %d %d %d match dir 0\n", x[0], x[1], x[2]);
#endif
  return 1;
 dir1:  
  // dir 1 : (0,-1,0) - (0,1,0)
  setvec(0,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto dir2;
  setvec(0,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  setvec(0,-2,0,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((BlackBeforeStep(val)) && (BlackBeforeStep(val2))) goto dir2;
  for (f = -1; f <= 1; f++)
    for (g = -1; g <= 1; g++)
      if ((f != 0) || (g != 0))
      {
	n = 0;
	for (h = -1; h <= 1; h++)
	{
	  setvec(f,h,g,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
	}
	if (n == 0) goto dir2;	
      }
#ifdef DEBUGSAHA
printf("condition2 point %d %d %d match dir 1\n", x[0], x[1], x[2]);
#endif
  return 1;
 dir2:  
  // dir 2 : (0,0,-1) - (0,0,1)
  setvec(0,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) goto out; // a is white
  setvec(0,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; 
  setvec(0,0,-2,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((BlackBeforeStep(val)) && (BlackBeforeStep(val2))) goto out;
  for (f = -1; f <= 1; f++)
    for (g = -1; g <= 1; g++)
      if ((f != 0) || (g != 0))
      {
	n = 0;
	for (h = -1; h <= 1; h++)
	{
	  setvec(f,g,h,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
	}
	if (n == 0) goto out;	
      }
#ifdef DEBUGSAHA
printf("condition2 point %d %d %d match dir 2\n", x[0], x[1], x[2]);
#endif
  return 1;
 out:  

  return 0;
} // condition2()

static void extractplane_0(int32_t *X, vec x, index_t rs, index_t ps, uint8_t *p)
{
  int32_t i, j, val;
  vec t;
  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
    {
      setvec(0,i-1,j-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (val >= 0) p[j*3+i] = 1; else p[j*3+i] = 0;
    }
} // extractplane_0()

static void extractplane_1(int32_t *X, vec x, index_t rs, index_t ps, uint8_t *p)
{
  int32_t i, j, val;
  vec t;
  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
    {
      setvec(i-1,0,j-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (val >= 0) p[j*3+i] = 1; else p[j*3+i] = 0;
    }
} // extractplane_1()

static void extractplane_2(int32_t *X, vec x, index_t rs, index_t ps, uint8_t *p)
{
  int32_t i, j, val;
  vec t;
  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
    {
      setvec(i-1,j-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (val >= 0) p[j*3+i] = 1; else p[j*3+i] = 0;
    }
} // extractplane_2()

static int32_t condition3(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t val, n;
  vec t;
  uint8_t p[9];

  // returns 0 is there exists a direction such that 
  //                    one e-point in the orthogonal plane is white and  
  //                    the 2D configuration is not simple
  // X[x] is supposed to be black (not checked)

  // dir0 : orthogonal to (-1,0,0) - (1,0,0)
  n = 0;
  setvec(0,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(0,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(0,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(0,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
//printf("condition3 dir0 x = %d %d %d n = %d\n", x[0], x[1], x[2], n);//DEBUG
  if (n == 4) goto dir1;
  extractplane_0(X, x, rs, ps, p);
//printf("condition3 dir0 x = %d %d %d simple8 = %d\n", x[0], x[1], x[2], simple8(p,4,3,9));//DEBUG
  if (!simple8(p,4,3,9)) return 0;
  
 dir1:
  // dir1 : orthogonal to (0-1,0) - (0,1,0)
  n = 0;
  setvec(0,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(-1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(0,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
//printf("condition3 dir1 x = %d %d %d n = %d\n", x[0], x[1], x[2], n);//DEBUG
  if (n == 4) goto dir2;
  extractplane_1(X, x, rs, ps, p);
//printf("condition3 dir1 x = %d %d %d simple8 = %d\n", x[0], x[1], x[2], simple8(p,4,3,9));//DEBUG
  if (!simple8(p,4,3,9)) return 0;
  
 dir2:
  // dir2 : orthogonal to (0,0,-1) - (0,0,1)
  n = 0;
  setvec(0,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(-1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
  setvec(0,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) n++;
//printf("condition3 dir2 x = %d %d %d n = %d\n", x[0], x[1], x[2], n);//DEBUG
  if (n == 4) goto out;
  extractplane_2(X, x, rs, ps, p);
//printf("condition3 dir2 x = %d %d %d simple8 = %d\n", x[0], x[1], x[2], simple8(p,4,3,9));//DEBUG
  if (!simple8(p,4,3,9)) return 0;
  
 out:
  return 1;
} // condition3()

static int32_t thick_0(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t val, val2;
  vec t;
  setvec(1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) return 0;
  setvec(-2,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) return 0;
  setvec(-1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (WhiteBeforeStep(val)) return 0;
  setvec(0,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)];
  setvec(0,1,0,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((WhiteBeforeStep(val)) && WhiteBeforeStep(val2)) return 0;
  setvec(0,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)];
  setvec(0,0,1,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((WhiteBeforeStep(val)) && WhiteBeforeStep(val2)) return 0;
  return 1;
} // thick_0()

static int32_t thick_1(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t val, val2;
  vec t;
  setvec(0,1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) return 0;
  setvec(0,-2,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) return 0;
  setvec(0,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (WhiteBeforeStep(val)) return 0;
  setvec(0,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)];
  setvec(0,0,1,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((WhiteBeforeStep(val)) && WhiteBeforeStep(val2)) return 0;
  setvec(-1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)];
  setvec(1,0,0,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((WhiteBeforeStep(val)) && WhiteBeforeStep(val2)) return 0;
  return 1;
} // thick_1()

static int32_t thick_2(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t val, val2;
  vec t;
  setvec(0,0,1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) return 0;
  setvec(0,0,-2,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (BlackBeforeStep(val)) return 0;
  setvec(0,0,-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (WhiteBeforeStep(val)) return 0;
  setvec(-1,0,0,t); addvec(t, x, t); val = X[IndexPoint(t)];
  setvec(1,0,0,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((WhiteBeforeStep(val)) && WhiteBeforeStep(val2)) return 0;
  setvec(0,-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)];
  setvec(0,1,0,t); addvec(t, x, t); val2 = X[IndexPoint(t)]; 
  if ((WhiteBeforeStep(val)) && WhiteBeforeStep(val2)) return 0;
  return 1;
} // thick_2()

static int32_t condition4(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  uint8_t p1[9], p2[9];
  // X[x] is supposed to be black (not checked)
  if (thick_0(X,x,rs,ps,i))
  {
    extractplane_1(X, x, rs, ps, p1);
    extractplane_2(X, x, rs, ps, p2);
    if (simple8(p1,4,3,9) && simple8(p2,4,3,9)) return 1;
  }
  if (thick_1(X,x,rs,ps,i))
  {
    extractplane_0(X, x, rs, ps, p1);
    extractplane_2(X, x, rs, ps, p2);
    if (simple8(p1,4,3,9) && simple8(p2,4,3,9)) return 1;
  }
  if (thick_2(X,x,rs,ps,i))
  {
    extractplane_0(X, x, rs, ps, p1);
    extractplane_1(X, x, rs, ps, p2);
    if (simple8(p1,4,3,9) && simple8(p2,4,3,9)) return 1;
  }
  return 0;
} // condition4()

static int32_t condition5(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  uint8_t p[9];
  // X[x] is supposed to be black (not checked)
  if (thick_0(X,x,rs,ps,i) && thick_1(X,x,rs,ps,i))
  {
    extractplane_2(X, x, rs, ps, p);
    if (simple8(p,4,3,9)) return 1;
  }
  if (thick_0(X,x,rs,ps,i) && thick_2(X,x,rs,ps,i))
  {
    extractplane_1(X, x, rs, ps, p);
    if (simple8(p,4,3,9)) return 1;
  }
  if (thick_1(X,x,rs,ps,i) && thick_2(X,x,rs,ps,i))
  {
    extractplane_0(X, x, rs, ps, p);
    if (simple8(p,4,3,9)) return 1;
  }
  return 0;
} // condition5()

static int32_t condition6(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  // X[x] is supposed to be black (not checked)
  if (thick_0(X,x,rs,ps,i) && thick_1(X,x,rs,ps,i) && thick_2(X,x,rs,ps,i))  return 1;
  return 0;
} // condition6()

static void extractvois(
  int32_t *img,                    /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  uint8_t *vois)
{
#undef F_NAME
#define F_NAME "extractvois"
  uint32_t xx, yy, zz, i, j, k;
  if ((p%rs>=rs-1) || (p%ps<rs) || (p%rs<=0) || (p%ps>=ps-rs) ||
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }
  memset(vois,0,27);
  zz = p / ps;
  yy = (p % ps) / rs;
  xx = p % rs;
  for (k = 0; k < 3; k++)
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++)
      {
	if (img[((zz-1+k) * ps) + ((yy-1+j) * rs) + xx-1+i] >= 0) // black point
	  vois[(k * 9) + (j * 3) + i] = 255;
      }
} /* extractvois() */

#ifdef DEBUGSAHA
static void princurimage(int32_t *X, index_t rs, index_t cs, index_t ds, int32_t i)
{ 
  index_t ix, jx, kx, ps = rs*cs;

  for (kx = 0; kx < ds; kx++)
  {
    for (jx = 0; jx < cs; jx++)
    {
      for (ix = 0; ix < rs; ix++)
      {
	printf("%11d ", X[ix + jx*rs + kx*ps]);
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("\n");
}
#endif

static index_t sahascansurf(int32_t *X, index_t rs, index_t cs, index_t ds, int32_t i)
{ // returns the number of deleted points during the scan
  index_t ix, jx, kx, xx, ps = rs*cs, N = ps*ds, ndeleted = 0;
  vec x;
  uint8_t vois[27];
  int32_t subfield;

 // first scan: s_open points
 for (subfield = 0; subfield < 8; subfield++)
 {
  for (kx = 0; kx < ds; kx++)
  for (jx = 0; jx < cs; jx++)
  for (ix = 0; ix < rs; ix++)
  if (((ix%2) == (subfield%2)) && ((jx%2) == ((subfield>>1)%2)) && ((kx%2) == ((subfield>>2)%2)))
  { // in the subfield 
    xx = ix + jx*rs + kx*ps;
    if (X[xx] >= 0) // black point
    {
      setvec(ix, jx, kx, x);
      if (s_open(X, x, rs, ps, i) && !is_marked(X, x, rs, ps, i))
      {
	if (condition1(X, x, rs, ps, i) || condition2(X, x, rs, ps, i)) // shape point
	  mark(X, x, rs, ps, i);
	else
	{
	  extractvois(X, xx, rs, ps, N, vois);
	  if (mctopo3d_simple26(vois, 13, 3, 9, 27))
	  { 
//printf("sahascansurf scan 1 x = %d %d %d deleted\n", x[0], x[1], x[2]);//DEBUG
	    delete(X, x, rs, ps, i); 
	    ndeleted++; 
	  }
	}
      }
    }
  } // for for for if
 } // for (subfield = 0; subfield < 8; subfield++)

 // second scan: e_open points
 for (subfield = 0; subfield < 8; subfield++)
 {
  for (kx = 0; kx < ds; kx++)
  for (jx = 0; jx < cs; jx++)
  for (ix = 0; ix < rs; ix++)
  if (((ix%2) == (subfield%2)) && ((jx%2) == ((subfield>>1)%2)) && ((kx%2) == ((subfield>>2)%2)))
  { // in the subfield 
    xx = ix + jx*rs + kx*ps;
    if (X[xx] >= 0) // black point
    {
      setvec(ix, jx, kx, x);
      if (e_open(X, x, rs, ps, i) && !is_marked(X, x, rs, ps, i))
      {
	extractvois(X, xx, rs, ps, N, vois);
//print_vois27(vois);
//printf("e_open check point %d %d %d simple = %d condition3 = %d\n", x[0], x[1], x[2], mctopo3d_simple26(vois, 13, 3, 9, 27), condition3(X, x, rs, ps, i));//DEBUG
	if (mctopo3d_simple26(vois, 13, 3, 9, 27) && condition3(X, x, rs, ps, i))
	{ 
	  delete(X, x, rs, ps, i); 
	  ndeleted++; 
	}
	//	else
	//	  mark(X, x, rs, ps, i);
      }
    }
  } // for for for if
 } // for (subfield = 0; subfield < 8; subfield++)

 // third scan: v_open points
 for (subfield = 0; subfield < 8; subfield++)
 {
  for (kx = 0; kx < ds; kx++)
  for (jx = 0; jx < cs; jx++)
  for (ix = 0; ix < rs; ix++)
  if (((ix%2) == (subfield%2)) && ((jx%2) == ((subfield>>1)%2)) && ((kx%2) == ((subfield>>2)%2)))
  { // in the subfield 
    xx = ix + jx*rs + kx*ps;
    if (X[xx] >= 0) // black point
    {
      setvec(ix, jx, kx, x);
      if (v_open(X, x, rs, ps, i) && !is_marked(X, x, rs, ps, i))
      {
	extractvois(X, xx, rs, ps, N, vois);
	if (mctopo3d_simple26(vois, 13, 3, 9, 27))
	{ delete(X, x, rs, ps, i); ndeleted++; }
	//	else
	//	  mark(X, x, rs, ps, i);
      }
    }
  } // for for for if
 } // for (subfield = 0; subfield < 8; subfield++)
 return ndeleted;
} // sahascansurf()

static void sahafinalscansurf(int32_t *X, index_t rs, index_t cs, index_t ds, int32_t i)
{
  index_t ix, jx, kx, xx, ps = rs*cs, N = ps*ds;
  vec x;
  uint8_t vois[27];
  int32_t subfield;

 for (subfield = 0; subfield < 8; subfield++)
 {
  for (kx = 0; kx < ds; kx++)
  for (jx = 0; jx < cs; jx++)
  for (ix = 0; ix < rs; ix++)
  if (((ix%2) == (subfield%2)) && ((jx%2) == ((subfield>>1)%2)) && ((kx%2) == ((subfield>>2)%2)))
  { // in the subfield 
    xx = ix + jx*rs + kx*ps;
    if (X[xx] >= 0) // black point
    {
      extractvois(X, xx, rs, ps, N, vois);
      if (mctopo3d_simple26(vois, 13, 3, 9, 27))
      {
	setvec(ix, jx, kx, x);
	if (condition4(X, x, rs, ps, i) || condition5(X, x, rs, ps, i) || condition6(X, x, rs, ps, i))
	  delete(X, x, rs, ps, i);
      }
    }
  } // for for for if
 } // for (subfield = 0; subfield < 8; subfield++)
} // sahafinalscansurf()

/* ==================================== */
int32_t lsahaetal8subfieldssurf1997(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lsahaetal8subfieldssurf1997"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step;
  index_t n;
  int32_t *X;

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;
  
  X = (int32_t *)malloc(N * sizeof(int32_t));
  assert(X != NULL);

  for (i = 0; i < N; i++) if (S[i]) X[i] = 0; else X[i] = INT_MIN;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  n = 1; step = 0;
  while ((n > 0) && (step < nsteps))
  {
    n = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    n = sahascansurf(X, rs, cs, ds, step);
  } // while ((n > 0) && (step < nsteps))

#ifdef DEBUGSAHA
princurimage(X, rs, cs, ds, step);
#endif

#define FINALSCANSURF
#ifdef FINALSCANSURF
  sahafinalscansurf(X, rs, cs, ds, step);
#endif
  
#ifdef VERBOSE1
  printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i] >= 0) S[i] = 255; else S[i] = 0; // normalize values
  mctopo3d_termine_topo3d();
  free(X);
  return(1);
} /* lsahaetal8subfieldssurf1997() */

static int32_t s_open_surf(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t a, b, s, val;
  vec ta, tb, td, txa, txb, txd;
  // X[x] is supposed to be black (not checked)
  for (a = 0; a < 3; a++) 
  {
    scalevec(dirs[a], 1, ta);
    scalevec(dirs[a], -1, td);
    addvec(ta, x, txa);
    val = X[IndexPoint(txa)];
    if (BlackBeforeStep(val)) goto nextad;
    addvec(td, x, txd);
    val = X[IndexPoint(txd)];
    if (BlackBeforeStep(val)) goto nextad;
    for (b = 0; b < 3; b++)
      if (b != a) 
	for (s = -1; s <= 1; s += 2)
	{
	  scalevec(dirs[b], s, tb);
	  addvec(tb, x, txb);
	  val = X[IndexPoint(txb)];
	  if (BlackBeforeStep(val)) goto nextb;
	  addvec(ta, tb, txa);
	  addvec(txa, x, txa);
	  val = X[IndexPoint(txa)];
	  if (BlackBeforeStep(val)) goto nextb;
	  addvec(tb, td, txb);
	  addvec(txb, x, txb);
	  val = X[IndexPoint(txb)];
	  if (BlackBeforeStep(val)) goto nextb;
	  return 1;
	nextb:;
	}
  nextad:;
  }
  return 0;
} // s_open_surf()

static int32_t e_open_surf(int32_t *X, vec x, index_t rs, index_t ps, int32_t i)
{
  int32_t ia, ib, sb, ic, sc, v1, v2, n;
  vec a, b, c, d, e, f, r;
  // X[x] is supposed to be black (not checked)
#ifdef DEBUG_lsahaetal8subfieldscurv1997
   printf("e_open_surf %d %d %d\n", x[0], x[1], x[2]);
#endif

  if (s_open_surf(X, x, rs, ps, i)) return 0;

  for (ia = 0; ia < 3; ia++)
  {
    scalevec(dirs[ia], 1, a);
    scalevec(dirs[ia], -1, d);
    for (ib = 0; ib < 3; ib++)
    if (ib != ia) 
    {
      for (sb = -1; sb <= 1; sb += 2)
      {
	scalevec(dirs[ib], sb, b);
	scalevec(b, -1, e);
	for (ic = 0; ic < 3; ic++)
	if ((ic != ia) && (ic != ib))
	{
	  for (sc = -1; sc <= 1; sc += 2)
	  {
	    scalevec(dirs[ic], sc, c);
	    scalevec(c, -1, f);
	    addvec(a, x, r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) goto nextdir; 
	    e_(a,b,x,r); v1 = X[IndexPoint(r)]; e_(a,c,x,r); v2 = X[IndexPoint(r)];
	    if (BlackBeforeStep(v1) && BlackBeforeStep(v2)) goto nextdir;
	    v_(a,b,c,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) goto nextdir;
	    e_(b,c,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) goto nextdir;
	    v_(b,c,d,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) goto nextdir;
	    e_(b,d,x,r); v1 = X[IndexPoint(r)]; e_(c,d,x,r); v2 = X[IndexPoint(r)];
	    if (BlackBeforeStep(v1) && BlackBeforeStep(v2)) goto nextdir;
	    addvec(d, x, r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) goto nextdir;

	    n = 0;
	    f2(a,b,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f3(a,b,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f2(b,a,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f1(b,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f2(b,d,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f3(b,d,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f2(d,b,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    if (n == 0) goto nextdir;

	    n = 0;
	    f2(a,c,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f3(a,c,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f2(c,a,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f1(c,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f2(c,d,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f3(c,d,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    f2(d,c,x,r); v1 = X[IndexPoint(r)]; if (BlackBeforeStep(v1)) n++;
	    if (n == 0) goto nextdir;

	    return 1;
	  nextdir:;
	  } // for sc
	} // for c
      } // for sb
    } // for b
  } // for a
  return 0;
} // e_open_surf()

static index_t sahascancurv(int32_t *X, index_t rs, index_t cs, index_t ds, int32_t i)
{ // returns the number of deleted points during the scan
  index_t ix, jx, kx, xx, ps = rs*cs, N = ps*ds, ndeleted = 0;
  vec x;
  uint8_t vois[27];
  int32_t subfield;

 // first scan: s_open_surf points
 for (subfield = 0; subfield < 8; subfield++)
 {
#ifdef DEBUG_lsahaetal8subfieldscurv1997
   printf("sahascancurv step %d scan 1 subfield %d\n", i, subfield);
#endif
  for (kx = 0; kx < ds; kx++)
  for (jx = 0; jx < cs; jx++)
  for (ix = 0; ix < rs; ix++)
  if (((ix%2) == (subfield%2)) && ((jx%2) == ((subfield>>1)%2)) && ((kx%2) == ((subfield>>2)%2)))
  { // in the subfield 
    xx = ix + jx*rs + kx*ps;
    if (X[xx] >= 0) // black point
    {
      setvec(ix, jx, kx, x);
      if (s_open_surf(X, x, rs, ps, i) && !is_marked(X, x, rs, ps, i))
      {
	if (condition1(X, x, rs, ps, i)) // shape point
	  mark(X, x, rs, ps, i);
	else
	{
	  extractvois(X, xx, rs, ps, N, vois);
	  if (mctopo3d_simple26(vois, 13, 3, 9, 27))
	  { 
//printf("sahascancurv scan 1 x = %d %d %d deleted\n", x[0], x[1], x[2]);//DEBUG
	    delete(X, x, rs, ps, i); 
	    ndeleted++; 
	  }
	}
      }
    }
  } // for for for if
 } // for (subfield = 0; subfield < 8; subfield++)

 // second scan: e_open_surf points
 for (subfield = 0; subfield < 8; subfield++)
 {
#ifdef DEBUG_lsahaetal8subfieldscurv1997
   printf("sahascancurv step %d scan 2 subfield %d\n", i, subfield);
#endif
  for (kx = 0; kx < ds; kx++)
  for (jx = 0; jx < cs; jx++)
  for (ix = 0; ix < rs; ix++)
  if (((ix%2) == (subfield%2)) && ((jx%2) == ((subfield>>1)%2)) && ((kx%2) == ((subfield>>2)%2)))
  { // in the subfield 
    xx = ix + jx*rs + kx*ps;
    if (X[xx] >= 0) // black point
    {
      setvec(ix, jx, kx, x);
      if (e_open_surf(X, x, rs, ps, i) && !is_marked(X, x, rs, ps, i))
      {
	extractvois(X, xx, rs, ps, N, vois);
	if (mctopo3d_simple26(vois, 13, 3, 9, 27))
	{ 
	  delete(X, x, rs, ps, i); 
	  ndeleted++; 
	}
	//	else
	//	  mark(X, x, rs, ps, i);
      }
    }
  } // for for for if
 } // for (subfield = 0; subfield < 8; subfield++)

 return ndeleted;
} // sahascancurv()

static int32_t arc_erodable(int32_t *X, vec x, index_t rs, index_t ps)
{
  int32_t i, j, val, n;
  vec t;
  for (n = j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
    {
      setvec(0,i-1,j-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (val >= 0) n++;
    }
  if (n == 0) return 0;

  for (n = j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
    {
      setvec(i-1,0,j-1,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (val >= 0) n++;
    }
  if (n == 0) return 0;

  for (n = j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
    {
      setvec(i-1,j-1,0,t); addvec(t, x, t); val = X[IndexPoint(t)]; if (val >= 0) n++;
    }
  if (n == 0) return 0;
  return 1;
} // arc_erodable()

static void sahafinalscancurv(int32_t *X, index_t rs, index_t cs, index_t ds, int32_t i)
{
  index_t ix, jx, kx, xx, ps = rs*cs, N = ps*ds;
  vec x;
  uint8_t vois[27];
  int32_t subfield;

 for (subfield = 0; subfield < 8; subfield++)
 {
  for (kx = 0; kx < ds; kx++)
  for (jx = 0; jx < cs; jx++)
  for (ix = 0; ix < rs; ix++)
  if (((ix%2) == (subfield%2)) && ((jx%2) == ((subfield>>1)%2)) && ((kx%2) == ((subfield>>2)%2)))
  { // in the subfield 
    xx = ix + jx*rs + kx*ps;
    if (X[xx] >= 0) // black point
    {
      extractvois(X, xx, rs, ps, N, vois);
      if (mctopo3d_simple26(vois, 13, 3, 9, 27))
      {
	setvec(ix, jx, kx, x);
	if (arc_erodable(X, x, rs, ps))
	  delete(X, x, rs, ps, i);
      }
    }
  } // for for for if
 } // for (subfield = 0; subfield < 8; subfield++)
} // sahafinalscancurv()

/* ==================================== */
int32_t lsahaetal8subfieldscurv1997(
				  struct xvimage *image,
				  int32_t nsteps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lsahaetal8subfieldscurv1997"
{ 
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step;
  index_t n;
  int32_t *X;

  if (!lsahaetal8subfieldssurf1997(image, -1))
  {
    fprintf(stderr, "%s: lsahaetal8subfieldssurf1997 failed\n", F_NAME);
    return 0;
  } 

  mctopo3d_init_topo3d();

  if (nsteps == -1) nsteps = 1000000000;
  
  X = (int32_t *)malloc(N * sizeof(int32_t));
  assert(X != NULL);

  for (i = 0; i < N; i++) if (S[i]) X[i] = 0; else X[i] = INT_MIN;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  n = 1; step = 0;
  while ((n > 0) && (step < nsteps))
  {
    n = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    n = sahascancurv(X, rs, cs, ds, step);
  } // while ((n > 0) && (step < nsteps))

#ifdef DEBUGSAHA
princurimage(X, rs, cs, ds, step);
#endif

#define FINALSCANCURV
#ifdef FINALSCANCURV
  sahafinalscancurv(X, rs, cs, ds, step);
#endif
  
#ifdef VERBOSE1
  printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i] >= 0) S[i] = 255; else S[i] = 0; // normalize values
  mctopo3d_termine_topo3d();
  free(X);
  return(1);
} /* lsahaetal8subfieldscurv1997() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Németh, Kardos et Palagyi (6 directions, 2011) 
// M. Couprie, oct. 2014
/* ============================================================ */
/* ============================================================ */

#define NKP11_OBJECT          1
#define NKP11_DELETABLE       2
#define NKP11_MARK            4
#define NKP11_BSNE            8
// BSNE  : Border, Simple, Non-End

#undef IS_DELETABLE
#undef SET_DELETABLE
#define IS_DELETABLE(f) (f&NKP11_DELETABLE)
#define SET_DELETABLE(f) (f|=NKP11_DELETABLE)
#define IS_MARKED(f) (f&NKP11_MARK)
#define SET_MARK(f) (f|=NKP11_MARK)
#define UNSET_MARK(f) (f&=~NKP11_MARK)
#define IS_BSNE(f) (f&NKP11_BSNE)
#define SET_BSNE(f) (f|=NKP11_BSNE)
#define UNSET_BSNE(f) (f&=~NKP11_BSNE)

static int32_t nkp11_square_considerable(uint8_t *X, index_t dir, index_t x, index_t v1, index_t v2, index_t v3)
{
  int32_t n = 0;
  // tests if the set S(x) = {v1, v2, v3} inter X is square_considerable
  // X[x] is supposed to be black (not checked)
  
  SET_MARK(X[x]); if (X[v1]) SET_MARK(X[v1]); if (X[v2]) SET_MARK(X[v2]); if (X[v3]) SET_MARK(X[v3]);
  if (IS_MARKED(X[x + dir])) n++;
  if (IS_MARKED(X[v1]) && IS_MARKED(X[v1 + dir])) n++;
  if (IS_MARKED(X[v2]) && IS_MARKED(X[v2 + dir])) n++;
  if (IS_MARKED(X[v3]) && IS_MARKED(X[v3 + dir])) n++;
  UNSET_MARK(X[x]); UNSET_MARK(X[v1]); UNSET_MARK(X[v2]); UNSET_MARK(X[v3]);
  if (n == 0) return 1;
  return 0;
} // nkp11_square_considerable()

static int32_t nkp11_test_square(uint8_t *X, index_t dir, index_t i, index_t v1, index_t v2, index_t v3, index_t rs, index_t ps, index_t N)
{
  uint8_t sv1, sv2, sv3;
  int32_t is_simple;
  if (nkp11_square_considerable(X, dir, i, v1, v2, v3))
  {
    if (X[v1] && !IS_BSNE(X[v1])) return 1;
    if (X[v2] && !IS_BSNE(X[v2])) return 1;
    if (X[v3] && !IS_BSNE(X[v3])) return 1;
    sv1 = X[v1]; sv2 = X[v2]; sv3 = X[v3]; X[v1] = X[v2] = X[v3] = 0;
    is_simple = mctopo3d_simple26(X, i, rs, ps, N);
    X[v1] = sv1; X[v2] = sv2; X[v3] = sv3;
    if (!is_simple) return 0;
  }
  return 1;
} // nkp11_test_square()

static int32_t nkp11_match(index_t dir, uint8_t *X, index_t i, index_t rs, index_t ps, index_t N)
{
  index_t v1, v2, v3;

  // point i has been tested BSNE

  // for all 12 squares around point i
  // x constant
  v1 = i + rs; v2 = i + rs + ps; v3 = i + ps; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - rs; v2 = i - rs + ps; v3 = i + ps; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - rs; v2 = i - rs - ps; v3 = i - ps; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i + rs; v2 = i + rs - ps; v3 = i - ps; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 

  // y constant
  v1 = i + 1; v2 = i + 1 + ps; v3 = i + ps; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - 1; v2 = i - 1 + ps; v3 = i + ps; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - 1; v2 = i - 1 - ps; v3 = i - ps; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i + 1; v2 = i + 1 - ps; v3 = i - ps; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 

  // z constant
  v1 = i + rs; v2 = i + rs + 1; v3 = i + 1; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - rs; v2 = i - rs + 1; v3 = i + 1; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - rs; v2 = i - rs - 1; v3 = i - 1; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i + rs; v2 = i + rs - 1; v3 = i - 1; if (!nkp11_test_square(X, dir, i, v1, v2, v3, rs, ps, N)) return 0; 

  return 1;
} // nkp11_match()

static void match_2_2_2_obj(uint8_t *X, index_t i, index_t j, index_t k, index_t rs, index_t ps)
{
  index_t pi, pj, pk, n;
  // plane pk = 0
  for (pj = 0; pj < 4; pj++)
    for (pi = 0; pi < 4; pi++)
      if (X[(k+0)*ps + (j+pj)*rs + (i+pi)]) return;
  // planes pk = 1,2
  n = 0;
  for (pk = 1; pk <= 2; pk++)
    for (pj = 0; pj < 4; pj++)
      for (pi = 0; pi < 4; pi++)
      {
	if ((pj == 0) || (pj == 3) || (pi == 0) || (pi == 3))
	{
	  if (X[(k+pk)*ps + (j+pj)*rs + (i+pi)]) return;
	}
	else
	{
	  if (X[(k+pk)*ps + (j+pj)*rs + (i+pi)]) n++;
	}
      }
  if (n == 0) return;
  // plane pk = 3
  for (pj = 0; pj < 4; pj++)
    for (pi = 0; pi < 4; pi++)
      if (X[(k+3)*ps + (j+pj)*rs + (i+pi)]) return;

  if (n > 1)
  {
    n = 0;
    for (pk = 1; pk <= 2; pk++)
      for (pj = 0; pj < 4; pj++)
	for (pi = 0; pi < 4; pi++)
	  if ((pj != 0) && (pj != 3) && (pi != 0) && (pi != 3))
	  {
	    if (X[(k+pk)*ps + (j+pj)*rs + (i+pi)]) 
	    {
	      if (n > 0) X[(k+pk)*ps + (j+pj)*rs + (i+pi)] = 0;
	      n++;
	    }
	  }
  }
} // match_2_2_2_obj()

/* ==================================== */
int32_t lnemethetal6dir2011(
			    struct xvimage *image,
			    int32_t nsteps,
			    int32_t mode
			   )
/* ==================================== */
#undef F_NAME
#define F_NAME "lnemethetal6dir2011"
{
  index_t i, j, k, dir;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  assert(rs >= 4); assert(cs >= 4); assert(ds >= 4);
  mctopo3d_init_topo3d();
  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (X[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (X[i]) X[i] = NKP11_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (k = 0; k < ds-4; k++) 
      for (j = 0; j < cs-4; j++) 
	for (i = 0; i < rs-4; i++) 
	  match_2_2_2_obj(X, i, j, k, rs, ps);

    for (d = 0; d < 6; d++)
    {
#ifdef VERBOSE2
    printf("dir %d\n", d);
#endif

      switch (d)
      {
      case 0: dir = - 1; break;
      case 1: dir =   1; break;
      case 2: dir = -rs; break;
      case 3: dir =  rs; break;
      case 4: dir = -ps; break;
      case 5: dir =  ps; break;
      }

      for (i = 0; i < N; i++) 
	if (X[i])
	{
	  if (NKP_end(X, i, rs, ps, N, mode)) SET_NKP_END(X[i]);
	  if (!X[i + dir] && (!IS_NKP_END(X[i])) && mctopo3d_simple26(X, i, rs, ps, N))
	    SET_BSNE(X[i]);
	}
      for (i = 0; i < N; i++) 
	if (IS_BSNE(X[i]) && nkp11_match(dir, X, i, rs, ps, N))
	  SET_DELETABLE(X[i]);

      for (i = 0; i < N; i++) 
	if (IS_DELETABLE(X[i])) 
	{ 
	  X[i] = 0; 
	  nonstab = 1; 
	}

      for (i = 0; i < N; i++) UNSET_BSNE(X[i]);
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i]) X[i] = 255; // normalize values

  mctopo3d_termine_topo3d();

  return(1);
} /* lnemethetal6dir2011() */

static int32_t pala12_match(int32_t d, uint8_t *S, index_t i, index_t rs, index_t ps, index_t N)
{
  uint8_t s[27], t[27]; 
  extract_vois27(S, i, rs, ps, N, t); 
  switch (d)
  {
  case 0: memcpy(s, t, 27); break;
  case 1: isometry(s, t, I01); break;
  case 2: isometry(s, t, I02); break;
  case 3: isometry(s, t, I03); break;
  case 4: isometry(s, t, I04); break;
  case 5: isometry(s, t, I05); break;
  case 6: isometry(s, t, I06); break;
  case 7: isometry(s, t, I07); break;
  case 8: isometry(s, t, I08); break;
  case 9: isometry(s, t, I09); break;
  case 10: isometry(s, t, I10); break;
  case 11: isometry(s, t, I11); break;
  }

  //print_vois27(s);

// mask1: I1
  if (!s[22]) goto mask2; // noir
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8]) goto mask2; // blanc
  return 1;

 mask2: // I2
  if (!s[10]) goto mask3;
  if (s[6] || s[7] || s[8] || s[9] || s[11] || s[12] || s[14] || 
      s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask3;
  return 2;
  /*
 mask3:
  if (!s[19]) goto mask4;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
       s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask4;
  if (!s[18] &&  !s[21] &&  !s[9] &&  !s[12] &&  !s[14] &&  !s[11] &&  !s[20] &&  !s[23]) goto mask4;
  return 3;
  */
 mask3: // I3
  if (!s[19]) goto mask4;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || s[9] || s[11] || s[12] || s[14] ||
       s[15] || s[16] || s[17] || s[18] || s[20] || s[21] || s[23] || s[24] || s[25] || s[26]) goto mask4;
  return 3;

 mask4: // C4
  if (!s[10] || !s[22]) goto mask5; // noir
  if (s[4] ||  s[6] || s[7] || s[8] || s[16]) goto mask5;
  if (s[3] && s[15]) goto mask5;
  if (s[5] && s[17]) goto mask5;
  return 4;

 mask5: // C7
  if (!s[10] || !s[22] || !s[14]) goto mask6;
  if (s[4] || s[6] || s[7] || s[16]) goto mask6;
  if (s[3] && s[15]) goto mask6;
  return 5;

 mask6: // C8
  if (!s[10] || !s[22] || !s[12]) goto mask7;
  if (s[4] || s[7] || s[8] || s[16]) goto mask7;
  if (s[5] && s[17]) goto mask7;
  return 6;

 mask7: // C5
  if (!s[10] || !s[22] || !s[8]) goto mask8;
  if (s[4] ||  s[6] || s[7] || s[16]) goto mask8;
  if (s[3] && s[15]) goto mask8;
  if (s[5] == s[17]) goto mask8;
  return 7;

 mask8: // C6
  if (!s[10] || !s[22] || !s[6]) goto mask9;
  if (s[4] || s[7] || s[8] || s[16]) goto mask9;
  if (s[3] == s[15]) goto mask9;
  if (s[5] && s[17]) goto mask9;
  return 8;

 mask9: // C11
  if (!s[19] || !s[23]) goto mask10;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
       s[15] || s[16] || s[24] || s[25]) goto mask10;
  return 9;

 mask10: // C12
  if (!s[19] || !s[21]) goto mask11;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
       s[17] || s[16] || s[26] || s[25]) goto mask11;
  return 10;

 mask11: // C13
  if (!s[19] || !s[11]) goto mask12;
  if (s[0] || s[1] || s[3] || s[4] || s[6] || s[7] || s[8] ||
       s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask12;
  return 11;

 mask12: // C14
  if (!s[19] || !s[9]) goto mask13;
  if (s[1] || s[2] || s[4] || s[5] || s[6] || s[7] || s[8] ||
       s[15] || s[16] || s[17] || s[24] || s[25] || s[26]) goto mask13;
  return 12;

 mask13: // C9
  if (!s[10] || !s[14] || !s[22] || !s[6]) goto mask14;
  if (s[4] || s[7] || s[16]) goto mask14;
  if (s[3] == s[15]) goto mask14;
  return 13;

 mask14: // C10
  if (!s[10] || !s[12] || !s[22] || !s[8]) goto mask15;
  if (s[4] || s[7] || s[16]) goto mask15;
  if (s[5] == s[17]) goto mask15;
  return 14;

 mask15: // I15
  if (!s[18]) goto mask16;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] || 
      s[11] || s[14] || s[15] || s[16] || s[17] || s[20] || s[23] || s[24] || s[25] || s[26]) goto mask16;
  return 15;

 mask16: // I16
  if (!s[20]) return 0;
  if (s[0] || s[1] || s[2] || s[3] || s[4] || s[5] || s[6] || s[7] || s[8] ||
      s[9] || s[12] || s[15] || s[16] || s[17] || s[18] || s[21] || s[24] || s[25] || s[26]) return 0;
  return 16;
}

/* ==================================== */
int32_t lpalagyi12dir2013( struct xvimage *image,
			   int32_t nsteps
			 )
/* ==================================== */
#undef F_NAME
#define F_NAME "lpalagyi12dir2013"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  if (nsteps == -1) nsteps = 1000000000;

  mctopo3d_init_topo3d();

  for (i = 0; i < N; i++) if (S[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (S[i]) S[i] = PK12_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (d = 0; d < 12; d++)
    {
#ifdef DEBUG_lpalagyikuba12dir1999
      int code;
      for (i = 0; i < N; i++) 
	if (S[i] && NKP_end(S, i, rs, ps, N, ISTHMUS)) SET_NKP_END(S[i]);
      for (i = 0; i < N; i++) 
	if (S[i] && !IS_NKP_END(S[i]) && (code = pala12_match(d, S, i, rs, ps, N)))
	{
          printf("matched : dir=%d i=%d code=%d\n", d, i, code);
	  SET_DELETABLE(S[i]);
	}
#else
      for (i = 0; i < N; i++) 
	if (S[i] && NKP_end(S, i, rs, ps, N, ISTHMUS)) SET_NKP_END(S[i]);
      for (i = 0; i < N; i++) 
	if (S[i] && !IS_NKP_END(S[i]) && pala12_match(d, S, i, rs, ps, N)) 
	  SET_DELETABLE(S[i]);
#endif
      for (i = 0; i < N; i++) 
	if (IS_DELETABLE(S[i])) 
	{ 
	  S[i] = 0; 
	  nonstab = 1; 
	}

#ifdef DEBUG_lpalagyi12dir2013_b
      { int n;
	for (n = i = 0; i < N; i++) if (S[i]) n++;
	printf("3. step %d subs %d reste %d\n", step, d, n);
      }
      if (d==0) writeimage(image, "_ss00");
      if (d==1) writeimage(image, "_ss01");
      if (d==2) writeimage(image, "_ss02");
      if (d==3) writeimage(image, "_ss03");
      if (d==4) writeimage(image, "_ss04");
      if (d==5) writeimage(image, "_ss05");
      if (d==6) writeimage(image, "_ss06");
      if (d==7) writeimage(image, "_ss07");
      if (d==8) writeimage(image, "_ss08");
      if (d==9) writeimage(image, "_ss09");
      if (d==10) writeimage(image, "_ss10");
      if (d==11) writeimage(image, "_ss11");
#endif

    } // for (d = 0; d < 12; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();

  return(1);
} /* lpalagyi12dir2013() */

/* ============================================================ */
/* ============================================================ */
// Algo. de Németh, Kardos et Palagyi (6 directions, 2012) 
// M. Couprie, fev. 2015
/* ============================================================ */
/* ============================================================ */

#define NKP12_OBJECT          1
#define NKP12_DELETABLE       2
#define NKP12_END             4
#define NKP12_SIMPLE          8

#undef IS_DELETABLE
#undef SET_DELETABLE
#define IS_DELETABLE(f) (f&NKP12_DELETABLE)
#define SET_DELETABLE(f) (f|=NKP12_DELETABLE)
#define IS_END(f) (f&NKP12_END)
#define SET_END(f) (f|=NKP12_END)
#define UNSET_END(f) (f&=~NKP12_END)
#define IS_SIMPLE(f) (f&NKP12_SIMPLE)
#define SET_SIMPLE(f) (f|=NKP12_SIMPLE)
#define UNSET_SIMPLE(f) (f&=~NKP12_SIMPLE)

static int32_t nkp12_test_square4(uint8_t *X, index_t i, index_t v1, index_t v2, index_t v3, index_t rs, index_t ps, index_t N)
{
  uint8_t sv1, sv2, sv3;
  int32_t is_simple;
  if ((i > v1) || (i > v2) || (i > v3)) return 1;
  sv1 = X[v1]; sv2 = X[v2]; sv3 = X[v3]; X[v1] = X[v2] = X[v3] = 0;
  is_simple = mctopo3d_simple26(X, i, rs, ps, N);
  X[v1] = sv1; X[v2] = sv2; X[v3] = sv3;
  if (!is_simple) return 0;
  return 1;
} // nkp12_test_square4()

static int32_t nkp12_test_square3(uint8_t *X, index_t i, index_t v1, index_t v2, index_t rs, index_t ps, index_t N)
{
  uint8_t sv1, sv2;
  int32_t is_simple;
  if ((i > v1) || (i > v2)) return 1;
  sv1 = X[v1]; sv2 = X[v2]; X[v1] = X[v2] = 0;
  is_simple = mctopo3d_simple26(X, i, rs, ps, N);
  X[v1] = sv1; X[v2] = sv2;
  if (!is_simple) return 0;
  return 1;
} // nkp12_test_square3()

static int32_t nkp12_test_square2(uint8_t *X, index_t i, index_t v1, index_t rs, index_t ps, index_t N)
{
  uint8_t sv1;
  int32_t is_simple;
  if ((i > v1)) return 1;
  sv1 = X[v1]; X[v1] = 0;
  is_simple = mctopo3d_simple26(X, i, rs, ps, N);
  X[v1] = sv1;
  if (!is_simple) return 0;
  return 1;
} // nkp12_test_square2()

static int32_t nkp12_test_square(uint8_t *X, index_t i, index_t v1, index_t v2, index_t v3, index_t rs, index_t ps, index_t N)
{
  index_t T[3];
  int8_t c = 0;
  if (X[v1] && IS_SIMPLE(X[v1])) T[c++] = v1;
  if (X[v2] && IS_SIMPLE(X[v2])) T[c++] = v2;
  if (X[v3] && IS_SIMPLE(X[v3])) T[c++] = v3;
  switch(c)
  {
  case 0: return 1;
  case 1: 
    if (!nkp12_test_square2(X, i, T[0], rs, ps, N)) return 0; 
    return 1;
  case 2: 
    if (!nkp12_test_square3(X, i, T[0], T[1], rs, ps, N)) return 0; 
    if (!nkp12_test_square2(X, i, T[0], rs, ps, N)) return 0; 
    if (!nkp12_test_square2(X, i, T[1], rs, ps, N)) return 0; 
    return 1;
  case 3: 
    if (!nkp12_test_square4(X, i, T[0], T[1], T[2], rs, ps, N)) return 0; 
    if (!nkp12_test_square3(X, i, T[0], T[1], rs, ps, N)) return 0; 
    if (!nkp12_test_square3(X, i, T[0], T[2], rs, ps, N)) return 0; 
    if (!nkp12_test_square3(X, i, T[1], T[2], rs, ps, N)) return 0; 
    if (!nkp12_test_square2(X, i, T[0], rs, ps, N)) return 0; 
    if (!nkp12_test_square2(X, i, T[1], rs, ps, N)) return 0; 
    if (!nkp12_test_square2(X, i, T[2], rs, ps, N)) return 0; 
    return 1;
  }
  assert(0);
} // nkp12_test_square()

// tests if voxel i is deletable
static int32_t nkp12_match(uint8_t *X, index_t i, index_t rs, index_t ps, index_t N)
{
  index_t v1, v2, v3;

  // point i has been tested SIMPLE

  // for all 12 squares around point i
  // x constant
  v1 = i + rs; v2 = i + rs + ps; v3 = i + ps; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - rs; v2 = i - rs + ps; v3 = i + ps; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - rs; v2 = i - rs - ps; v3 = i - ps; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i + rs; v2 = i + rs - ps; v3 = i - ps; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 

  // y constant
  v1 = i + 1; v2 = i + 1 + ps; v3 = i + ps; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - 1; v2 = i - 1 + ps; v3 = i + ps; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - 1; v2 = i - 1 - ps; v3 = i - ps; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i + 1; v2 = i + 1 - ps; v3 = i - ps; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 

  // z constant
  v1 = i + rs; v2 = i + rs + 1; v3 = i + 1; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - rs; v2 = i - rs + 1; v3 = i + 1; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i - rs; v2 = i - rs - 1; v3 = i - 1; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 
  v1 = i + rs; v2 = i + rs - 1; v3 = i - 1; if (!nkp12_test_square(X, i, v1, v2, v3, rs, ps, N)) return 0; 

  return 1;
} // nkp12_match()

/* ==================================== */
int32_t lnemethetal6dir2012(
			    struct xvimage *image,
			    int32_t nsteps,
			    int32_t mode
			   )
/* ==================================== */
#undef F_NAME
#define F_NAME "lnemethetal6dir2012"
{
  index_t i, j, k, dir;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, d;

  assert(rs >= 4); assert(cs >= 4); assert(ds >= 4);
  mctopo3d_init_topo3d();
  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (X[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (X[i]) X[i] = NKP12_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (k = 0; k < ds-4; k++) 
      for (j = 0; j < cs-4; j++) 
	for (i = 0; i < rs-4; i++) 
	  match_2_2_2_obj(X, i, j, k, rs, ps);

    for (d = 0; d < 6; d++)
    {
#ifdef VERBOSE2
    printf("dir %d\n", d);
#endif

      switch (d)
      {
      case 0: dir = -ps; break;
      case 1: dir =  ps; break;
      case 2: dir = -rs; break;
      case 3: dir =   1; break;
      case 4: dir =  rs; break;
      case 5: dir =  -1; break;
      }

      for (i = 0; i < N; i++) 
	if (X[i])
	{
	  if (NKP_end(X, i, rs, ps, N, mode)) SET_END(X[i]);
	  if (mctopo3d_simple26(X, i, rs, ps, N)) SET_SIMPLE(X[i]);
	}
      for (i = 0; i < N; i++) 
	if (IS_SIMPLE(X[i]) && (!IS_END(X[i])) && (!X[i+dir]) && nkp12_match(X, i, rs, ps, N))
	  SET_DELETABLE(X[i]);

      for (i = 0; i < N; i++) 
	if (IS_DELETABLE(X[i])) 
	{ 
	  X[i] = 0; 
	  nonstab = 1; 
	}

      for (i = 0; i < N; i++) UNSET_SIMPLE(X[i]);
    } // for (d = 0; d < 6; d++)
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i]) X[i] = 255; // normalize values

  mctopo3d_termine_topo3d();

  return(1);
} /* lnemethetal6dir2012() */

/* ==================================== */
int32_t lnemethetalfullyparallel2012(
			    struct xvimage *image,
			    int32_t nsteps,
			    int32_t mode
			   )
/* ==================================== */
#undef F_NAME
#define F_NAME "lnemethetalfullyparallel2012"
{
  index_t i, j, k;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *X = UCHARDATA(image);   /* l'image de depart */
  int32_t step, nonstab;

  assert(rs >= 4); assert(cs >= 4); assert(ds >= 4);
  mctopo3d_init_topo3d();
  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (X[i]) assert(!bord3d(i, rs, ps, N));
  for (i = 0; i < N; i++) if (X[i]) X[i] = NKP12_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (k = 0; k < ds-4; k++) 
      for (j = 0; j < cs-4; j++) 
	for (i = 0; i < rs-4; i++) 
	  match_2_2_2_obj(X, i, j, k, rs, ps);

    for (i = 0; i < N; i++) 
      if (X[i])
      {
	if (NKP_end(X, i, rs, ps, N, mode)) SET_END(X[i]);
	if (mctopo3d_simple26(X, i, rs, ps, N)) SET_SIMPLE(X[i]);
      }
    for (i = 0; i < N; i++) 
      if (IS_SIMPLE(X[i]) && (!IS_END(X[i])) && nkp12_match(X, i, rs, ps, N))
	SET_DELETABLE(X[i]);
    
    for (i = 0; i < N; i++) 
      if (IS_DELETABLE(X[i])) 
      { 
	X[i] = 0; 
	nonstab = 1; 
      }
    
    for (i = 0; i < N; i++) UNSET_SIMPLE(X[i]);
  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i]) X[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  
  return(1);
} /* lnemethetalfullyparallel2012() */

/* ==================================== */
int32_t lnemethetal4subfields2012(
			    struct xvimage *image,
			    int32_t nsteps,
			    int32_t mode
			   )
/* ==================================== */
#undef F_NAME
#define F_NAME "lnemethetal4subfields2012"
{
  index_t i, j, k, x;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, s;

  assert(rs >= 4); assert(cs >= 4); assert(ds >= 4);
  mctopo3d_init_topo3d();
  if (nsteps == -1) nsteps = 1000000000;

  for (x = 0; x < N; x++) if (X[x]) assert(!bord3d(x, rs, ps, N));
  for (x = 0; x < N; x++) if (X[x]) X[x] = NKP12_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps)) // macro-step
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (k = 0; k < ds-4; k++) 
      for (j = 0; j < cs-4; j++) 
	for (i = 0; i < rs-4; i++) 
	  match_2_2_2_obj(X, i, j, k, rs, ps);

    for (x = 0; x < N; x++) // endpoint detection: once per macro-step
      if (X[x])
      { 
	if (mode != 0) UNSET_END(X[x]);
	if (NKP_end(X, x, rs, ps, N, mode)) SET_END(X[x]);
      }

    for (s = 0; s < 4; s++) // the 4 substeps
    {
#ifdef VERBOSE2
    printf("substep %d\n", s);
#endif

      for (k = 1; k < ds-1; k++)  // simple point detection
      for (j = 1; j < cs-1; j++)
      for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (s%2)) && (((k+j)%2) == ((s%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (X[x])
	{
	  if (mctopo3d_simple26(X, x, rs, ps, N)) SET_SIMPLE(X[x]);
	}
      }

      for (k = 1; k < ds-1; k++)
      for (j = 1; j < cs-1; j++)
      for (i = 1; i < rs-1; i++)
      if ((((j+i)%2) == (s%2)) && (((k+j)%2) == ((s%4)/2)))
      {
	x = k*ps + j*rs + i;
	if (X[x] && IS_SIMPLE(X[x]) && (!IS_END(X[x])) && nkp12_match(X, x, rs, ps, N))
	  SET_DELETABLE(X[x]);
      }

      for (x = 0; x < N; x++) 
	if (IS_DELETABLE(X[x])) 
	{ 
	  X[x] = 0; 
	  nonstab = 1; 
	}
      for (x = 0; x < N; x++) UNSET_SIMPLE(X[x]);

    } // for (s = 0; s < 4; s++)

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (x = 0; x < N; x++) if (X[x]) X[x] = 255; // normalize values

  mctopo3d_termine_topo3d();

  return(1);
} /* lnemethetal4subfields2012() */

/* ==================================== */
int32_t lnemethetal8subfields2012(
			    struct xvimage *image,
			    int32_t nsteps,
			    int32_t mode
			   )
/* ==================================== */
#undef F_NAME
#define F_NAME "lnemethetal8subfields2012"
{
  index_t i, j, k, x;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab, s;

  assert(rs >= 4); assert(cs >= 4); assert(ds >= 4);
  mctopo3d_init_topo3d();
  if (nsteps == -1) nsteps = 1000000000;

  for (x = 0; x < N; x++) if (X[x]) assert(!bord3d(x, rs, ps, N));
  for (x = 0; x < N; x++) if (X[x]) X[x] = NKP12_OBJECT; 

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab && (step < nsteps)) // macro-step
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    for (k = 0; k < ds-4; k++) 
      for (j = 0; j < cs-4; j++) 
	for (i = 0; i < rs-4; i++) 
	  match_2_2_2_obj(X, i, j, k, rs, ps);

    for (x = 0; x < N; x++) // endpoint detection: once per macro-step
      if (X[x])
      { 
	if (mode != 0) UNSET_END(X[x]);
	if (NKP_end(X, x, rs, ps, N, mode)) SET_END(X[x]);
      }

    for (s = 0; s < 8; s++) // the 8 substeps
    {
#ifdef VERBOSE2
    printf("substep %d\n", s);
#endif

      for (k = 1; k < ds-1; k++)  // simple point detection
      for (j = 1; j < cs-1; j++)
      for (i = 1; i < rs-1; i++)
      if (((i%2) == (s%2)) && ((j%2) == ((s>>1)%2)) && ((k%2) == ((s>>2)%2)))
      {
	x = k*ps + j*rs + i;
	if (X[x])
	{
	  if (mctopo3d_simple26(X, x, rs, ps, N)) SET_SIMPLE(X[x]);
	}
      }

      for (k = 1; k < ds-1; k++)
      for (j = 1; j < cs-1; j++)
      for (i = 1; i < rs-1; i++)
      if (((i%2) == (s%2)) && ((j%2) == ((s>>1)%2)) && ((k%2) == ((s>>2)%2)))
      {
	x = k*ps + j*rs + i;
	if (X[x] && IS_SIMPLE(X[x]) && (!IS_END(X[x])) && nkp12_match(X, x, rs, ps, N))
	  SET_DELETABLE(X[x]);
      }

      for (x = 0; x < N; x++) 
	if (IS_DELETABLE(X[x])) 
	{ 
	  X[x] = 0; 
	  nonstab = 1; 
	}
      for (x = 0; x < N; x++) UNSET_SIMPLE(X[x]);

    } // for (s = 0; s < 8; s++)

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (x = 0; x < N; x++) if (X[x]) X[x] = 255; // normalize values

  mctopo3d_termine_topo3d();

  return(1);
} /* lnemethetal8subfields2012() */


#define PAL15_OBJECT      1
#define PAL15_DELETE      2
#define PAL15_CONSTR      4

#define IS_PAL15_OBJECT(f)     (f&PAL15_OBJECT)
#define IS_PAL15_DELETE(f)     (f&PAL15_DELETE)
#define IS_PAL15_CONSTR(f)     (f&PAL15_CONSTR)

#define SET_PAL15_OBJECT(f)    (f|=PAL15_OBJECT)
#define SET_PAL15_DELETE(f)    (f|=PAL15_DELETE)
#define SET_PAL15_CONSTR(f)    (f|=PAL15_CONSTR)

/* ==================================== */
static int32_t pal15_cond(uint8_t *v)
/* ==================================== */
{
  if (v[4]) return 0; // b
  if (!IS_PAL15_CONSTR(v[22])) return 0; // c
  if ((!v[16] && v[7]) || (!v[14] && v[5]) || (!v[10] && v[1]) || (!v[12] && v[3])) return 0; // e
  if ((!v[16] && !v[15] && !v[12] && v[6]) || (!v[12] && !v[9] && !v[10] && v[0]) || (!v[10] && !v[11] && !v[14] && v[2]) || (!v[14] && !v[17] && !v[16] && v[8])) return 0; // f
  if (IS_PAL15_CONSTR(v[1]) || IS_PAL15_CONSTR(v[3]) || IS_PAL15_CONSTR(v[5]) || IS_PAL15_CONSTR(v[7])) return 0; // g
  if (IS_PAL15_CONSTR(v[9]) || IS_PAL15_CONSTR(v[11]) || IS_PAL15_CONSTR(v[15]) || IS_PAL15_CONSTR(v[17])) return 0; // h
  if ((IS_PAL15_CONSTR(v[0]) && (!(v[10] && v[12]))) || (IS_PAL15_CONSTR(v[2]) && (!(v[10] && v[14]))) || (IS_PAL15_CONSTR(v[6]) && (!(v[16] && v[12]))) || (IS_PAL15_CONSTR(v[8]) && (!(v[14] && v[16])))) return 0; // i
  return 1;
} // pal15_cond()

/* ==================================== */
int32_t lpalagyi2015(struct xvimage *image,
		     int32_t n_steps)
/* ==================================== */
#undef F_NAME
#define F_NAME "lpalagyi2015"
{
  index_t i;
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab;
  uint8_t v[27];

  if (n_steps == -1) n_steps = 1000000000;

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

    for (i = 0; i < N; i++) if (S[i]) S[i] = PAL15_OBJECT;

    // PREMIERE PASSE : MARQUE LES POINTS INTERIEURS (CONSTR)
    for (i = 0; i < N; i++) 
      if (S[i] && (mctopo3d_nbvoiso6(S, i, rs, ps, N) == 6))
	SET_PAL15_CONSTR(S[i]);

    // DEUXIEME PASSE : MARQUE LES POINTS EFFACABLES
    for (i = 0; i < N; i++) 
      if (S[i] && mctopo3d_simple26(S, i, rs, ps, N))
      {
	// UPPER VOXELS
	extract_vois27(S, i, rs, ps, N, v);
	if (pal15_cond(v)) SET_PAL15_DELETE(S[i]);
	// LOWER VOXELS
	extract_vois27(S, i, rs, ps, N, v);
	swap_U_L_27(v);
	if (pal15_cond(v)) SET_PAL15_DELETE(S[i]);
	// EAST VOXELS
	extract_vois27(S, i, rs, ps, N, v);
	isometrieXZ_vois27(v);
	if (pal15_cond(v)) SET_PAL15_DELETE(S[i]);
	// WEST VOXELS
	extract_vois27(S, i, rs, ps, N, v);
	isometrieXZ_vois27(v);
	swap_U_L_27(v);
	if (pal15_cond(v)) SET_PAL15_DELETE(S[i]);
	// NORTH VOXELS
	extract_vois27(S, i, rs, ps, N, v);
	isometrieYZ_vois27(v);
	if (pal15_cond(v)) SET_PAL15_DELETE(S[i]);
	// SOUTH VOXELS
	extract_vois27(S, i, rs, ps, N, v);
	isometrieYZ_vois27(v);
	swap_U_L_27(v);
	if (pal15_cond(v)) SET_PAL15_DELETE(S[i]);
      }

    // TROISIEME PASSE : EFFACE LES POINTS EFFACABLES
    for (i = 0; i < N; i++) if (IS_PAL15_DELETE(S[i])) { S[i] = 0; nonstab = 1; }

  } // while (nonstab && (step < n_steps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  mctopo3d_termine_topo3d();
  return(1);
  
} // lpalagyi2015()
