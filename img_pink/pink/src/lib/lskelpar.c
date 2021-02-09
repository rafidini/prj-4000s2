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
   Algorithmes 2D "fully parallel" de squelettisation :

\li 0: Pavlidis 1981
\li 1: Eckhardt and Maderlechner 1993
\li 2: Couprie and Bertrand, ultimate (MK2)
\li 3: Couprie and Bertrand, curvilinear
\li 4: Couprie and Bertrand, curvilinear with reconstruction property (AK2)
\li 5: Rutovitz 1966 [DOES NOT PRESERVE TOPOLOGY]
\li 6: Zhang and Wang 1988 [DOES NOT PRESERVE TOPOLOGY]
\li 7: Han, La and Rhee
\li 8: Guo and Hall 1992 (1)
\li 9: Guo and Hall 1992 (2)
\li 10: Guo and Hall 1992 (3)
\li 11: Chin and Wan and Stover and Iverson 1987
\li 12: Jang and Chin 1992 [DOES NOT PRESERVE TOPOLOGY]
\li 13: Jang and Chin (correction mc)
\li 14: MNS preservation
\li 15: Jang and Chin 1993 (reconstruction)
\li 16: Choy, Choy and Siu 1995 [DOES NOT PRESERVE TOPOLOGY]
\li 17: Bernard and Manzanera 1999
\li 18: Holt et al. 1987
\li 19: Hall 1989
\li 20: Wu and Tsai 1992 [DOES NOT PRESERVE TOPOLOGY]
\li 21: Manzanera and Bernard (variant by GB)
\li 22: Couprie and Bertrand, ultimate asymmetrical with medial axis (NK2)
\li 23: Bertrand curvilinear with reconstruction, asymmetrical
\li 24: Bertrand curvilinear with reconstruction, symmetrical
\li 25: Rosenfeld directionnal (NSEW order)
\li 26: Nemeth et Palagyi, 2009 (1)
\li 27: Nemeth et Palagyi, 2009 (2)
\li 28: Nemeth et Palagyi, 2009 (3)
\li 29: Couprie and Bertrand, ultimate, asymmetrical (NK2b)
\li 30: Couprie and Bertrand, ultimate, symmetrical (MK2b)
\li 31: Couprie and Bertrand, curvilinear, symmetrical, based on 1D isthmuses (CK2)
\li 32: Rosenfeld directionnal (variant: WESN order)
\li 33: Rosenfeld directionnal (variant: ENWS order)
\li 34: Couprie and Bertrand, asymmetrical with persistence (NK2p)
\li 35: Zhang and Suen directional, 1984 [DOES NOT PRESERVE TOPOLOGY]
\li 36: Kwon, Gi, Kang directional, 2001

   Couprie ultime:
     input/output F
     repeat until stability
       for all x do if simple8(x) then F[x] = 2
       for all x do if (m1(x) v m2(x) v m3(x) v m4(x) v m5(x) v m6(x)) then T[x] = 1
       for all x do
         if (F[x]=2) then
           if not T[x] then F[x] = 0 else F[x] = 1

     m1 : match   A 2 B   avec origine = (1,0) et [au moins un des A et au moins un des B non nuls]
                  A 2 B   ou [A et B tous nuls] (+ rotation 90)

     m2 : match   x 0 0 x   avec origine = (1,2) et x = don't care
                  0 2 2 0   (+ rotations 90, 180, 270)
		  0 2 2 0
		  x 0 0 x

     m3: match   x 0 0 x   avec origine = (1,2) et x = don't care
                 0 2 2 x   (+ rotations 90, 180, 270)
	         0 2 0 x
		 x x x x

     m4: match   x x x x   avec origine = (1,2) et x = don't care
                 x 0 2 0   (+ rotations 90, 180, 270)
		 x 2 2 0
		 x 0 0 x

     m5: match   x x x x   avec origine = (1,2) et x = don't care
                 0 2 0 x   (+ rotations 90, 180, 270)
		 0 2 2 x
		 x 0 0 x

     m6 : match   2 0      avec origine = (1,1)
                  0 2      (+ rotations 90, 180, 270)

   Couprie curviligne:
     input/output F
     repeat until stability
       for all x do if simple8(x) and not end(x) then F[x] = 2
       for all x do if (m1(x) v m2(x) v m3(x) v m4(x) v m5(x) v m6(x)) then T[x] = 1
       for all x do
         if (F[x]=2) then
           if not T[x] then F[x] = 0 else F[x] = 1

     end : match   0 0 x   ou    0 x 1    avec origine = (1,1) et x = don't care
                   0 1 1         0 1 x
                   0 0 x         0 0 0

     VARIANTE:

     end2: match   0 0 p   ou    0 x 1    avec origine = (1,1) et x = don't care
                   0 1 1         0 1 x    et not (p = 1 and q = 1)
                   0 0 q         0 0 0

     end2: match   0 0 0   ou    0 x 1   ou   0 1 x   avec origine = (1,1) et x = don't care
                   0 1 1         0 1 x        0 1 1
                   0 0 0         0 0 0        0 0 0

   Couprie curviligne avec reconstruction:
     input/output X
     Y = emptyset
     repeat until stability
       I = Interior(X \ Y)
       D = Dilat(I) Inter [X \ Y]
       E = Y Union [X \ D]
       for all x do if simple8(x) and x not in E then X[x] = 2
       for all x do if (m1(x) v m2(x) v m3(x) v m4(x) v m5(x) v m6(x)) then T[x] = 1
       for all x do
         if (X[x]=2) then
           if not T[x] then X[x] = 0 else X[x] = 1
       Y = X \ I

   MNS_preservation:
     input/output F
     repeat until stability
       for all x do if simple8(x) then F[x] = 2
       for all x do if (m1(x) v m2(x) v m3(x) v m4(x) v m5(x) v m6(x)) then T[x] = 1
       for all x do
         if (F[x]=2) then
           if not T[x] then F[x] = 0 else F[x] = 1

     m1 : match   A 2 B   avec origine = (1,0) et [au moins un des A et au moins un des B non nuls]
                  A 2 B   ou [A et B tous nuls] (+ rotation 90)

		  (preuve équivalence par programme : Squel2D/ronse.c)

     m2 : match   0 0 0 0   avec origine = (1,2) et x = don't care
                  0 2 2 0   (+ rotations 90, 180, 270)
		  0 2 2 0
		  0 0 0 0

     m3 : match   0 0 0 0   avec origine = (1,2) et x = don't care
                  0 2 2 0   (+ rotations 90, 180, 270)
		  0 2 0 0
		  0 0 0 x

     m4 : match   x 0 0 0   avec origine = (1,2) et x = don't care
                  0 0 2 0   (+ rotations 90, 180, 270)
		  0 2 2 0
		  0 0 0 0

     m5 : match   0 0 0 x   avec origine = (1,2) et x = don't care
                  0 2 0 0   (+ rotations 90, 180, 270)
		  0 2 2 0
		  0 0 0 0

		  (preuve suffisant papier de Ronse)

     m6 : match   x 0 0 0   avec origine = (1,2) et x = don't care
                  0 0 2 0   (+ rotations 90, 180, 270)
                  0 2 0 0
		  0 0 0 x

		  (preuve suffisant par programme : Squel2D/ronse.c)

   Michel Couprie - juillet 2001
   Benjamin Raynal 2010 - algos Nemeth & Palagyi
   Nivando Bezerra 2011 - lhthinpar* : skel en niveaux de gris
   Michel Couprie 2011 - lskelACK2b : Squelette asymétrique curviligne
             #ifdef NOT_FINISHED
   Michel Couprie 2012 - lskelCK2 : Squelette symétrique curviligne basé sur les isthmes 1D	     
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <pinktypes.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mcrbt.h>
#include <mcutil.h>
#include <lskelpar.h>

//#define DEBUG_BERTRAND
//#define DEBUG_PAV
//#define DEBUG_MC
//#define VERBOSE
//#define VERBOSE1
//#define ETUDE
//#define ANIMATE

#define PERS_INIT_VAL 0
#define PERS_INFTY_VAL FLOAT_MAX

#define I_INHIBIT     1

#define S_OBJECT      1
#define S_SIMPLE      2
#define S_CRUCIAL     4
#define S_CURVE       8

#define IS_OBJECT(f)     (f&S_OBJECT)
#define IS_SIMPLE(f)     (f&S_SIMPLE)
#define IS_CRUCIAL(f)    (f&S_CRUCIAL)
#define IS_CURVE(f)      (f&S_CURVE)

#define SET_OBJECT(f)     (f|=S_OBJECT)
#define SET_SIMPLE(f)     (f|=S_SIMPLE)
#define SET_CRUCIAL(f)    (f|=S_CRUCIAL)
#define SET_CURVE(f)      (f|=S_CURVE)

#define UNSET_OBJECT(f)     (f&=~S_OBJECT)
#define UNSET_SIMPLE(f)     (f&=~S_SIMPLE)
#define UNSET_CRUCIAL(f)    (f&=~S_CRUCIAL)
#define UNSET_CURVE(f)      (f&=~S_CURVE)

static int32_t jangrec_match23(uint8_t *F, int32_t x, int32_t rs, int32_t N);
static int32_t jang_match27b(uint8_t *F, int32_t x, int32_t rs, int32_t N);
static int32_t jang_match28b(uint8_t *F, int32_t x, int32_t rs, int32_t N);


/* ==================================== */
static void extract_vois(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t N,                       /* taille image */
  uint8_t *vois)
/*
  retourne dans "vois" les valeurs des 8 voisins de p, dans l'ordre suivant:

		3	2	1
		4	p	0
		5	6	7
  le point p ne doit pas être un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p<rs) || (p%rs==0) || (p>=N-rs)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }
  vois[0] = *(ptr+1);
  vois[1] = *(ptr+1-rs);
  vois[2] = *(ptr-rs);
  vois[3] = *(ptr-rs-1);
  vois[4] = *(ptr-1);
  vois[5] = *(ptr-1+rs);
  vois[6] = *(ptr+rs);
  vois[7] = *(ptr+rs+1);
} /* extract_vois() */

#ifdef DEBUG
/* ==================================== */
static void print_vois(uint8_t *vois)
/*
   affiche vois (debug)
*/
/* ==================================== */
{
  printf("vois = %d %d %d %d %d %d %d %d\n",
	 vois[0], vois[1], vois[2], vois[3], vois[4], vois[5], vois[6], vois[7]);
} /* print_vois() */
#endif

#ifdef NOT_USED
/* ==================================== */
static void rotate45_vois(uint8_t *vois)
/*
   effectue une rotation du voisinage "vois" de 45 degres dans le sens
   trigonométrique
*/
/* ==================================== */
{
  uint8_t tmp = vois[0];
  vois[0] = vois[7]; vois[7] = vois[6]; vois[6] = vois[5]; vois[5] = vois[4];
  vois[4] = vois[3]; vois[3] = vois[2]; vois[2] = vois[1]; vois[1] = tmp;
} /* rotate45_vois() */
#endif

/* ==================================== */
static void rotate90_vois(uint8_t *vois)
/*
   effectue une rotation du voisinage "vois" de 90 degres dans le sens
   trigonométrique
*/
/* ==================================== */
{
  uint8_t tmp;
  tmp = vois[0]; vois[0] = vois[6]; vois[6] = vois[4]; vois[4] = vois[2]; vois[2] = tmp;
  tmp = vois[7]; vois[7] = vois[5]; vois[5] = vois[3]; vois[3] = vois[1]; vois[1] = tmp;
} /* rotate90_vois() */

/* ==================================== */
static int32_t extract_vois2(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t N,                       /* taille image */
  uint8_t *vois)
/*
  retourne dans "vois" les valeurs des 16 voisins d'ordre 2 de p, dans l'ordre suivant:

		6   5   4   3   2
		7   X   X   X   1
		8   X   p   X   0
		9   X   X   X   15
		10  11  12  13  14
  le point p doit être à une distance minimale de 2 du bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois2"
  register uint8_t * ptr = img+p;
  register int32_t rs2 = rs + rs;
  if ((p%rs>=rs-2) || (p<rs2) || (p%rs<2) || (p>=N-rs2)) /* point de bord */
  {
#ifdef PARANO
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
#else
    return 0;
#endif
  }
  vois[0] = *(ptr+2);
  vois[1] = *(ptr+2-rs);
  vois[2] = *(ptr+2-rs2);
  vois[3] = *(ptr+1-rs2);
  vois[4] = *(ptr-rs2);
  vois[5] = *(ptr-1-rs2);
  vois[6] = *(ptr-2-rs2);
  vois[7] = *(ptr-2-rs);
  vois[8] = *(ptr-2);
  vois[9] = *(ptr-2+rs);
  vois[10] = *(ptr-2+rs2);
  vois[11] = *(ptr-1+rs2);
  vois[12] = *(ptr+rs2);
  vois[13] = *(ptr+1+rs2);
  vois[14] = *(ptr+2+rs2);
  vois[15] = *(ptr+2+rs);
  return 1;
} /* extract_vois2() */

/* ==================================== */
static void rotate90_vois2(uint8_t *vois)
/*
   effectue une rotation du voisinage d'ordre 2 "vois" de 90 degres dans le sens
   trigonométrique
*/
/* ==================================== */
{
  uint8_t tmp;
  tmp = vois[0]; vois[0] = vois[12]; vois[12] = vois[8]; vois[8] = vois[4]; vois[4] = tmp;
  tmp = vois[1]; vois[1] = vois[13]; vois[13] = vois[9]; vois[9] = vois[5]; vois[5] = tmp;
  tmp = vois[2]; vois[2] = vois[14]; vois[14] = vois[10]; vois[10] = vois[6]; vois[6] = tmp;
  tmp = vois[3]; vois[3] = vois[15]; vois[15] = vois[11]; vois[11] = vois[7]; vois[7] = tmp;
} /* rotate90_vois2() */

/* ==================================== */
static int32_t pav_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  A A A   avec origine = (1,1) et au moins un des A et au moins un des B non nuls
  0 P 0
  B B B
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[0] != 0) goto fail;
    if ((v[1] == 0) && (v[2] == 0) && (v[3] == 0)) goto fail;
    if (v[4] != 0) goto fail;
    if ((v[5] == 0) && (v[6] == 0) && (v[7] == 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* pav_match1() */

/* ==================================== */
static int32_t pav_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  D D D     avec origine = (1,1) et valeurs des pixels D quelconques
  D P 0
  D 0 2
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[0] != 0) goto fail;
    //    if ((v[1] == 0) && (v[2] == 0) && (v[3] == 0) && (v[4] == 0) && (v[5] == 0)) goto fail;
    if (v[6] != 0) goto fail;
    if (v[7] != 2) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* pav_match2() */

/* ==================================== */
static int32_t pav_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
// condition for "tentatively multiple" points
/*
  A A C   avec origine = (1,1) et au moins un des A et au moins un des B
  0 2 2   et au moins un des C non nuls - de plus si les deux C sont non
  B B C   nuls, alors les pixels A et B peuvent être quelconques
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if ((v[0] != 1) && (v[1] != 1) && (v[2] != 1) && (v[3] != 1) &&
      (v[4] != 1) && (v[5] != 1) && (v[6] != 1) && (v[7] != 1)) return 1;
  for (i = 0; i < 4; i++)
  {
    if (v[0] != 2) goto fail;
    if (v[4] != 0) goto fail;
    if ((v[1] == 0) && (v[7] == 0)) goto fail;
    if ((v[1] != 0) && (v[7] != 0)) return 1;
    if ((v[2] == 0) && (v[3] == 0)) goto fail;
    if ((v[5] == 0) && (v[6] == 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* pav_match3() */

#ifdef NOT_USED
/* ==================================== */
static int32_t pav_match3b(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
// condition for "tentatively multiple" points
/*
  A A C   avec origine = (1,1) et au moins un des A OU au moins un des B OU au moins un des C non nuls
  0 2 2
  B B C
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if ((v[0] != 1) && (v[1] != 1) && (v[2] != 1) && (v[3] != 1) &&
      (v[4] != 1) && (v[5] != 1) && (v[6] != 1) && (v[7] != 1)) return 1;
  for (i = 0; i < 4; i++)
  {
    if (v[0] != 2) goto fail;
    if (v[4] != 0) goto fail;
    if ((v[1] != 0) || (v[7] != 0)) return 1;
    if ((v[2] != 0) || (v[3] != 0)) return 1;
    if ((v[5] != 0) || (v[6] != 0)) return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* pav_match3b() */
#endif

/* ==================================== */
static int32_t pav_match4(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
// condition for "corner" points
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[0] != 2) goto fail;
    if ((v[1] != 0) || (v[2] != 0) || (v[3] != 0) ||
	(v[4] != 0) || (v[5] != 0)) goto fail;
    if (v[6] != 2) goto fail;
    if (v[7] == 0) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* pav_match4() */

/* ==================================== */
static int32_t pav_multiple(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t m1 = pav_match1(F, x, rs, N);
  int32_t m2 = pav_match2(F, x, rs, N);
  int32_t m3 = pav_match3(F, x, rs, N);
  int32_t m4 = pav_match4(F, x, rs, N);
#ifdef DEBUG_PAV
  if (m1 || m2 || m3 || m4)
    printf("point %d,%d : m1 = %d; m2 = %d; m3 = %d; m4 = %d\n",
	   x % rs, x / rs, m1, m2, m3, m4);
#endif
  return m1 || m2 || m3 || m4;
} /* pav_multiple() */

/* ==================================== */
int32_t lskelpavlidis(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
// T. Pavlidis : "A flexible parallel thinning algorithm"
// see also T. Pavlidis : "An asynchronous thinning algorithm - CGIP 1982"
#undef F_NAME
#define F_NAME "lskelpavlidis"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  while (step < nsteps)
  {
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    memset(T, 0, N);
    for (i = 0; i < N; i++) if (F[i] && bordext4(F, i, rs, N)) F[i] = 2;
    for (i = 0; i < N; i++) if ((F[i] == 2) && pav_multiple(F, i, rs, N)) T[i] = 1;
#ifdef DEBUG_PAV
    printf("\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", F[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif
    for (i = 0; i < N; i++) if ((F[i] == 2) && !T[i]) break;
    if (i == N) break;
    for (i = 0; i < N; i++) if ((F[i] == 1) || T[i]) F[i] = 1; else F[i] = 0;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelpavlidis() */

/* ==================================== */
int32_t lskelpavlidis1(struct xvimage *image,
		  int32_t pixel)
/* ==================================== */
// T. Pavlidis : "A flexible parallel thinning algorithm"
// see also T. Pavlidis : "An asynchronous thinning algorithm - CGIP 1982"
// Une étape - un seul point testé
#undef F_NAME
#define F_NAME "lskelpavlidis1"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  memset(T, 0, N);
  for (i = 0; i < N; i++) if (F[i] && bordext4(F, i, rs, N)) F[i] = 2;
  for (i = 0; i < N; i++) if ((F[i] == 2) && pav_multiple(F, i, rs, N)) T[i] = 1;
  if ((F[pixel] == 2) && !T[pixel]) F[pixel] = 0;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelpavlidis1() */

/* ==================================== */
static int32_t eck_perfect(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i = x % rs;
  int32_t j = x / rs;
  if ((F[j*rs + i-1] == 2) && (F[j*rs + i+1] == 0)) return 1;
  if ((F[j*rs + i+1] == 2) && (F[j*rs + i-1] == 0)) return 1;
  if ((F[(j-1)*rs + i] == 2) && (F[(j+1)*rs + i] == 0)) return 1;
  if ((F[(j+1)*rs + i] == 2) && (F[(j-1)*rs + i] == 0)) return 1;
  return 0;
} /* eck_perfect() */

/* ==================================== */
int32_t lskeleckhardt(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
// Eckhardt and Maderlechner : "Invariant thinning"
#undef F_NAME
#define F_NAME "lskeleckhardt"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++) if (F[i] && nbvois4(F, i, rs, N) == 4) F[i] = 2;
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && eck_perfect(F, i, rs, N))
      {
	nonstab = 1;
	T[i] = 1;
      }
#ifdef DEBUG_ECK
    printf("\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", F[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif
    for (i = 0; i < N; i++)
      if (T[i] == 1) F[i] = 0;
      else if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskeleckhardt() */

/* ==================================== */
static int32_t crossing_nb(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
// retourne le nombre de transitions 1 -> 0 dans la liste ordonnée et cyclique des voisins
{
  int32_t i, n = 0;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 8; i++) if (v[i] && !v[(i+1)%8]) n++;
  return n;
} /* crossing_nb() */

/* ==================================== */
static int32_t rutovitz_match(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "rutovitz_match"
  int32_t n;
  uint8_t v[8];
  if (!F[x]) return 0;
  n = nbvois8(F, x, rs, N);
  //  if ((n < 2) || (n > 6)) return 0; // cond. donnée par Zhang et Wang
  if (n < 2) return 0;
  if (crossing_nb(F, x, rs, N) != 1) return 0;
  extract_vois(F, x, rs, N, v);
  if ((x < rs) || (x%rs == rs-1))
  {
    fprintf(stderr, "%s: object must not hit the frame\n", F_NAME);
    exit(0);
  }
  n = crossing_nb(F, x-rs, rs, N);
  if ((n == 1) && v[2] && v[0] && v[4]) return 0;
  n = crossing_nb(F, x+1, rs, N);
  if ((n == 1) && v[2] && v[0] && v[6]) return 0;
  return 1;
} /* rutovitz_match() */

/* ==================================== */
int32_t lskelrutovitz(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
// described in Zhang & Wang : "A modified parallel thinning algorithm"
#undef F_NAME
#define F_NAME "lskelrutovitz"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (rutovitz_match(F, i, rs, N))
	//if (rutovitz_match(F, i, rs, N) && !jangrec_match23(F, i, rs, N)) // correction mc
      {
	nonstab = 1;
	T[i] = 1;
      }
    for (i = 0; i < N; i++)
      if (T[i] == 1) F[i] = 0;
      else if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelrutovitz() */

/* ==================================== */
static int32_t zhangsuen_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "zhangsuen_match1"
  int32_t n;
  uint8_t v[8];
  if (!F[x]) return 0;
  n = nbvois8(F, x, rs, N);
  if ((n < 2) || (n > 6)) return 0;
  if (crossing_nb(F, x, rs, N) != 1) return 0;
  extract_vois(F, x, rs, N, v);
  if (v[2] && v[0] && v[6]) return 0;
  if (v[6] && v[0] && v[4]) return 0;
  return 1;
} /* zhangsuen_match1() */

/* ==================================== */
static int32_t zhangsuen_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "zhangsuen_match2"
  int32_t n;
  uint8_t v[8];
  if (!F[x]) return 0;
  n = nbvois8(F, x, rs, N);
  if ((n < 2) || (n > 6)) return 0;
  if (crossing_nb(F, x, rs, N) != 1) return 0;
  extract_vois(F, x, rs, N, v);
  if (v[2] && v[4] && v[6]) return 0;
  if (v[2] && v[0] && v[4]) return 0;
  return 1;
} /* zhangsuen_match2() */

/* ==================================== */
int32_t lskelzhangsuen(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
// described in Zhang & Suen : "A fast parallel algorithm for thinning digital patterns"
#undef F_NAME
#define F_NAME "lskelzhangsuen"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    if ((step%2) == 0)
    {
      for (i = 0; i < N; i++)
	if (zhangsuen_match1(F, i, rs, N))
	{
	  nonstab = 1;
	  T[i] = 1;
	}
    }
    else
    {
      for (i = 0; i < N; i++)
	if (zhangsuen_match2(F, i, rs, N))
	{
	  nonstab = 1;
	  T[i] = 1;
	}
    }
    for (i = 0; i < N; i++)
      if (T[i] == 1) F[i] = 0;
      else if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelzhangsuen() */

/* ==================================== */
static int32_t KwonGiKang_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "KwonGiKang_match1"
  int32_t n;
  uint8_t v[8];
  if (!F[x]) return 0;
  n = nbvois8(F, x, rs, N);
  if ((n < 2) || (n > 6)) return 0;
  if (crossing_nb(F, x, rs, N) != 1) return 0;
  extract_vois(F, x, rs, N, v);
  if (v[2] && v[0] && v[6]) return 0;
  if (v[6] && v[0] && v[4]) return 0;
  return 1;
} /* KwonGiKang_match1() */

/* ==================================== */
static int32_t KwonGiKang_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "KwonGiKang_match2"
  int32_t n;
  uint8_t v[8];
  if (!F[x]) return 0;
  n = nbvois8(F, x, rs, N);
  if ((n < 3) || (n > 6)) return 0;
  if (crossing_nb(F, x, rs, N) != 1) return 0;
  extract_vois(F, x, rs, N, v);
  if (v[2] && v[4] && v[6]) return 0;
  if (v[2] && v[0] && v[4]) return 0;
  return 1;
} /* KwonGiKang_match2() */

/* ==================================== */
static int32_t KwonGiKang_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "KwonGiKang_match3"
  uint8_t v[8];
  if (!F[x]) return 0;
  extract_vois(F, x, rs, N, v);
  if (!v[3] || !v[4] || !v[6] || v[1]) return 0;
  if (!v[1] || !v[0] || !v[6] || v[3]) return 0;
  if (!v[7] || !v[4] || !v[6] || v[1]) return 0;
  if (!v[5] || !v[0] || !v[6] || v[3]) return 0;
  return 1;
} /* KwonGiKang_match3() */

/* ==================================== */
int32_t lskelKwonGiKang(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
// described in Zhang & Suen : "A fast parallel algorithm for thinning digital patterns"
#undef F_NAME
#define F_NAME "lskelKwonGiKang"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;

  // PASS 1
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    memset(T, 0, N);
    if ((step%2) == 0)
    {
      for (i = 0; i < N; i++)
	if (KwonGiKang_match1(F, i, rs, N))
	{
	  nonstab = 1;
	  T[i] = 1;
	}
    }
    else
    {
      for (i = 0; i < N; i++)
	if (KwonGiKang_match2(F, i, rs, N))
	{
	  nonstab = 1;
	  T[i] = 1;
	}
    }
    for (i = 0; i < N; i++)
      if (T[i] == 1) F[i] = 0;
      else if (F[i]) F[i] = 1;
  } // while

#ifdef VERBOSE1
    printf("PASS1 ended, number of steps: %d\n", step);
#endif

  // PASS 2
  nonstab = 1;
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (KwonGiKang_match3(F, i, rs, N))
      {
	nonstab = 1;
	T[i] = 1;
      }
    for (i = 0; i < N; i++)
      if (T[i] == 1) F[i] = 0;
      else if (F[i]) F[i] = 1;
  } // while

#ifdef VERBOSE1
    printf("PASS2 ended, number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelKwonGiKang() */

/* ==================================== */
static int32_t zhangwang_match(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "zhangwang_match"
  int32_t n;
  uint8_t v[8];
  if (!F[x]) return 0;
  n = nbvois8(F, x, rs, N);
  if ((n < 2) || (n > 6)) return 0;
  if (crossing_nb(F, x, rs, N) != 1) return 0;
  extract_vois(F, x, rs, N, v);
  if ((x < rs+rs) || (x%rs >= rs-2))
  {
    fprintf(stderr, "%s: object must not hit the 2 pixel-thick frame\n", F_NAME);
    exit(0);
  }
  if ((F[x-(rs+rs)] == 0) && v[2] && v[0] && v[4]) return 0;
  if ((F[x+2]       == 0) && v[2] && v[0] && v[6]) return 0;
  return 1;
} /* zhangwang_match() */

/* ==================================== */
int32_t lskelzhangwang(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
// described in Zhang & Wang : "A modified parallel thinning algorithm 1988"
#undef F_NAME
#define F_NAME "lskelzhangwang"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (zhangwang_match(F, i, rs, N))
	//if (zhangwang_match(F, i, rs, N) && !jangrec_match23(F, i, rs, N)) // correction mc
      {
	nonstab = 1;
	T[i] = 1;
      }
    for (i = 0; i < N; i++)
      if (T[i] == 1) F[i] = 0;
      else if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelzhangwang() */

/* ==================================== */
static int32_t hanlarhee_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);

  // at least one neighbor must be greater than or equal to 3
  for (i = 0; i < 8; i++) if (v[i] >= 3) break;
  if (i == 8) return 0;

  for (i = 0; i < 4; i++)
  {
    if ((v[0])&&(v[1])&&(!v[2])&&(!v[3])&&(!v[4])&&(!v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  for (i = 0; i < 4; i++)
  {
    if ((!v[0])&&(v[1])&&(v[2])&&(!v[3])&&(!v[4])&&(!v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  for (i = 0; i < 4; i++)
  {
    if ((v[0])&&(!v[1])&&(v[2])&&(!v[3])&&(!v[4])&&(!v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  return 0;
} /* hanlarhee_match2() */

/* ==================================== */
static int32_t hanlarhee_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);

  // at least one neighbor must be greater than or equal to 7
  for (i = 0; i < 8; i++) if (v[i] >= 7) break;
  if (i == 8) return 0;

  for (i = 0; i < 4; i++)
  {
    if ((v[0])&&(v[1])&&(v[2])&&(!v[3])&&(!v[4])&&(!v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  for (i = 0; i < 4; i++)
  {
    if ((!v[0])&&(v[1])&&(v[2])&&(v[3])&&(!v[4])&&(!v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  if ((v[0])&&(!v[1])&&(!v[2])&&(!v[3])&&(!v[4])&&(v[5])&&(v[6])&&(!v[7])) return 1;
  if ((!v[0])&&(!v[1])&&(!v[2])&&(v[3])&&(v[4])&&(!v[5])&&(v[6])&&(!v[7])) return 1;
  if ((v[0])&&(v[1])&&(!v[2])&&(!v[3])&&(!v[4])&&(!v[5])&&(v[6])&&(!v[7])) return 1;
  if ((!v[0])&&(!v[1])&&(!v[2])&&(!v[3])&&(v[4])&&(!v[5])&&(v[6])&&(v[7])) return 1;

  return 0;
} /* hanlarhee_match3() */

/* ==================================== */
static int32_t hanlarhee_match4(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);

  for (i = 0; i < 4; i++)
  {
    if ((v[0])&&(v[1])&&(v[2])&&(v[3])&&(!v[4])&&(!v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  for (i = 0; i < 4; i++)
  {
    if ((!v[0])&&(v[1])&&(v[2])&&(v[3])&&(v[4])&&(!v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  if ((v[0])&&(v[1])&&(!v[2])&&(!v[3])&&(!v[4])&&(v[5])&&(v[6])&&(!v[7])) return 1;
  if ((!v[0])&&(!v[1])&&(!v[2])&&(v[3])&&(v[4])&&(!v[5])&&(v[6])&&(v[7])) return 1;

  return 0;
} /* hanlarhee_match4() */

/* ==================================== */
static int32_t hanlarhee_match5(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);

  // at least one neighbor must be equal to 8
  for (i = 0; i < 8; i++) if (v[i] == 8) break;
  if (i == 8) return 0;

  for (i = 0; i < 4; i++)
  {
    if ((v[0])&&(v[1])&&(v[2])&&(v[3])&&(v[4])&&(!v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  for (i = 0; i < 4; i++)
  {
    if ((!v[0])&&(v[1])&&(v[2])&&(v[3])&&(v[4])&&(v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  return 0;
} /* hanlarhee_match5() */

/* ==================================== */
static int32_t hanlarhee_match6(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);

  // at least one neighbor must be equal to 8
  for (i = 0; i < 8; i++) if (v[i] == 8) break;
  if (i == 8) return 0;

  for (i = 0; i < 4; i++)
  {
    if ((v[0])&&(v[1])&&(v[2])&&(v[3])&&(v[4])&&(v[5])&&(!v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  for (i = 0; i < 4; i++)
  {
    if ((!v[0])&&(v[1])&&(v[2])&&(v[3])&&(v[4])&&(v[5])&&(v[6])&&(!v[7])) return 1;
    rotate90_vois(v);
  }

  return 0;
} /* hanlarhee_match6() */

/* ==================================== */
static int32_t hanlarhee_match7(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i, n;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);

  // at least two neighbors must be equal to 8
  n = 0;
  for (i = 0; i < 8; i++) if (v[i] == 8) n++;
  if (n < 2) return 0;

  for (i = 0; i < 4; i++)
  {
    if ((!v[0])&&(v[1])&&(v[2])&&(v[3])&&(v[4])&&(v[5])&&(v[6])&&(v[7])) return 1;
    rotate90_vois(v);
  }

  return 0;
} /* hanlarhee_match7() */

/* ==================================== */
static int32_t hanlarhee_match(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  switch (F[x])
  {
  case 0: return 0;
  case 1:
    extract_vois(F, x, rs, N, v);
    for (i = 0; i < 8; i++)
      if (v[i] >= 3) return 1;
    return 0;
  case 2: return hanlarhee_match2(F, x, rs, N);
  case 3: return hanlarhee_match3(F, x, rs, N);
  case 4: return hanlarhee_match4(F, x, rs, N);
  case 5: return hanlarhee_match5(F, x, rs, N);
  case 6: return hanlarhee_match6(F, x, rs, N);
  case 7: return hanlarhee_match7(F, x, rs, N);
  case 8: return 0;
  }
  assert(1); exit(1);
} /* hanlarhee_match() */

/* ==================================== */
int32_t lskelhanlarhee(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
// described in Han, La & Rhee : "An efficient fully parallel algorithm"
#undef F_NAME
#define F_NAME "lskelhanlarhee"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  struct xvimage *nbn = copyimage(image);
  uint8_t *B = UCHARDATA(nbn);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++) if (F[i]) B[i] = nbvois8(F, i, rs, N);
    for (i = 0; i < N; i++)
      if (F[i] && hanlarhee_match(B, i, rs, N))
      {
	nonstab = 1;
	T[i] = 1;
      }
    for (i = 0; i < N; i++)
      if (T[i] == 1) F[i] = 0;
      else if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  freeimage(nbn);
  return(1);
} /* lskelhanlarhee() */

#undef F_NAME
#define F_NAME "lskelguohall"

/* ==================================== */
static int32_t guohall_L(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  int32_t q1, q2;
  if ((x+rs+rs >= N) || (x%rs < 2))
  {
    fprintf(stderr, "%s: object must not hit the 2 pixel-thick frame\n", F_NAME);
    exit(0);
  }
  q1 = F[x-2]; q2 = F[x+rs+rs];
  extract_vois(F, x, rs, N, v);
  return (((!v[2] && v[6] && !q2) &&
	   (v[1] || v[0] || v[7]) &&
	   (v[3] || v[4] || v[5])) ||
	  (!v[0] && v[4] && !q1));
} /* guohall_L() */

/* ==================================== */
static int32_t guohall_d1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  int32_t q2;
  if ((x+rs+rs >= N) || (x%rs < 2))
  {
    fprintf(stderr, "%s: object must not hit the 2 pixel-thick frame\n", F_NAME);
    exit(0);
  }
  q2 = F[x+rs+rs];
  extract_vois(F, x, rs, N, v);
  return (!v[2] && v[4] && v[5] && v[6] && v[7] && v[0] && !q2);
} /* guohall_d1() */

/* ==================================== */
static int32_t guohall_d2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  int32_t q1;
  if ((x+rs+rs >= N) || (x%rs < 2))
  {
    fprintf(stderr, "%s: object must not hit the 2 pixel-thick frame\n", F_NAME);
    exit(0);
  }
  q1 = F[x-2];
  extract_vois(F, x, rs, N, v);
  return (!v[0] && v[2] && v[3] && v[4] && v[5] && v[6] && !q1);
} /* guohall_d2() */

/* ==================================== */
static int32_t guohall_d3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  int32_t q1, q2;
  if ((x+rs+rs >= N) || (x%rs < 2))
  {
    fprintf(stderr, "%s: object must not hit the 2 pixel-thick frame\n", F_NAME);
    exit(0);
  }
  q1 = F[x-2]; q2 = F[x+rs+rs];
  extract_vois(F, x, rs, N, v);
  return (!v[0] && !v[1] && !v[2] && !v[3] && v[4] && v[5] && v[6] && !v[7] && !q1 && !q2);
} /* guohall_d3() */

/* ==================================== */
static int32_t guohall_b1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return (!v[0] && !v[1] && !v[2] && v[4] && !v[5] && v[6]);
} /* guohall_b1() */

/* ==================================== */
static int32_t guohall_b2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return (!v[2] && !v[3] && !v[4] && v[6] && !v[7] && v[0]);
} /* guohall_b2() */

/* ==================================== */
int32_t lskelguohall(struct xvimage *image,
		 int32_t nsteps,
		 struct xvimage *inhibit,
		 int32_t variante)
/* ==================================== */
// described in Guo & Hall : "Fast fully parallel thinning algorithms"
#undef F_NAME
#define F_NAME "lskelguohall"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);

    switch (variante)
    {
    case 1:
      for (i = 0; i < N; i++)
	if (F[i] && bordext4(F, i, rs, N) && (nbvois8(F, i, rs, N) > 2)
	    && (crossing_nb(F, i, rs, N) == 1) && !guohall_L(F, i, rs, N))
	{
	  nonstab = 1;
	  T[i] = 1;
	}
      for (i = 0; i < N; i++)
	if (T[i] == 1) F[i] = 0;
	else if (F[i]) F[i] = 1;
      break;
    case 2:
      for (i = 0; i < N; i++)
	if (F[i] && bordext4(F, i, rs, N) && (nbvois8(F, i, rs, N) > 2)
	    && (crossing_nb(F, i, rs, N) == 1) && !guohall_d1(F, i, rs, N)
	    && !guohall_d2(F, i, rs, N) && !guohall_d3(F, i, rs, N))
	{
	  nonstab = 1;
	  T[i] = 1;
	}
      for (i = 0; i < N; i++)
	if (T[i] == 1) F[i] = 0;
	else if (F[i]) F[i] = 1;
      break;
    case 3:
      for (i = 0; i < N; i++)
	if (F[i] &&
	    ((bordext4(F, i, rs, N) && (nbvois8(F, i, rs, N) > 2)
	     && (crossing_nb(F, i, rs, N) == 1) && !guohall_d1(F, i, rs, N)
	     && !guohall_d2(F, i, rs, N) && !guohall_d3(F, i, rs, N)) ||
	     guohall_b1(F, i, rs, N) || guohall_b2(F, i, rs, N)))
	{
	  nonstab = 1;
	  T[i] = 1;
	}
      for (i = 0; i < N; i++)
	if (T[i] == 1) F[i] = 0;
	else if (F[i]) F[i] = 1;
      break;
    default:
      fprintf(stderr, "%s: variant not implemented\n", F_NAME);
      return 0;
    } // switch (variante)

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelguohall() */

/* ==================================== */
static int32_t chinwan_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0
  1 P 1
  X 1 X
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || !v[4] || !v[6]) goto fail;
    if (v[1] || v[2] || v[3]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* chinwan_match1() */

/* ==================================== */
static int32_t chinwan_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  X 0 0
  1 P 0
  X 1 X
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[4] || !v[6]) goto fail;
    if (v[0] || v[1] || v[2]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* chinwan_match2() */

/* ==================================== */
static int32_t chinwan_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  int32_t q1, q2;
  if ((x+rs+rs >= N) || (x%rs >= rs - 2))
  {
    fprintf(stderr, "%s: object must not hit the 2 pixel-thick frame\n", F_NAME);
    exit(0);
  }
  q1 = F[x+2]; q2 = F[x+rs+rs];
  extract_vois(F, x, rs, N, v);
  return ((v[0] && !v[4] && !q1) || (!v[2] && v[6] && !q2));
} /* chinwan_match3() */

/* ==================================== */
int32_t lskelchinwan(struct xvimage *image,
		 int32_t nsteps,
		 struct xvimage *inhibit)
/* ==================================== */
// described in Chin, Wan, Stover & Iverson : "A one-pass thinning algorithms and its parallel implementation"
#undef F_NAME
#define F_NAME "lskelchinwan"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);

    for (i = 0; i < N; i++)
      if (F[i] && (chinwan_match1(F, i, rs, N) || chinwan_match2(F, i, rs, N)) &&
	  !chinwan_match3(F, i, rs, N))
      {
	nonstab = 1;
	T[i] = 1;
      }
    for (i = 0; i < N; i++)
      if (T[i] == 1) F[i] = 0;
      else if (F[i]) F[i] = 1;

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelchinwan() */

/* ==================================== */
static int32_t jang_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 x   avec origine = (1,1) et x = don't care
  0 1 1
  x 1 x
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[2] || v[3] || v[4] || !v[6]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* jang_match1() */

/* ==================================== */
static int32_t jang_match5(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 0 x   avec origine = (1,1) et x = don't care
  1 1 1
  1 1 1
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[2] || !v[4] || !v[5] || !v[6] || !v[7]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* jang_match5() */

/* ==================================== */
static int32_t jang_match9(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0   avec origine = (1,1) et (p ou q)
  0 1 0
  p 1 q
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[5] && !v[7]) goto fail;
    if (v[0] || v[1] || v[2] || v[3] || v[4] || !v[6]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* jang_match9() */

/* ==================================== */
static int32_t jang_match13(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if (v[0] || v[1] || v[2] || v[3] || v[4] || v[5] || v[6] || !v[7]) goto fail;
    if (!v2[15] || !v2[14] || !v2[13]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* jang_match13() */

/* ==================================== */
static int32_t jang_match17(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || !v[1] || !v[2] || v[3] || !v[4] || !v[5] || !v[6] || !v[7]) goto fail;
    if (!v2[0] || !v2[1] || !v2[3] || !v2[4] || !v2[8] || !v2[9] ||
	!v2[11] || !v2[12] || !v2[13] || !v2[14] || !v2[15]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* jang_match17() */

/* ==================================== */
static int32_t jang_match21(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[0] || !v[1] || !v[2] || v[3] || !v[4] || v[6] || v[7]) return 0;
  if (v2[4] || v2[5]) return 0;
  return 1;
} /* jang_match21() */

/* ==================================== */
static int32_t jang_match22(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[0] || v[1] || v[2] || !v[4] || v[5] || !v[6] || !v[7]) return 0;
  if (v2[11] || v2[12]) return 0;
  return 1;
} /* jang_match22() */

/* ==================================== */
static int32_t jang_match23(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[0] || v[1] || v[2] || !v[3] || !v[4] || v[5] || !v[6]) return 0;
  if (v2[9] || v2[8]) return 0;
  return 1;
} /* jang_match23() */

/* ==================================== */
static int32_t jang_match24(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[0] || !v[2] || v[3] || !v[4] || !v[5] || v[6] || v[7]) return 0;
  if (v2[7] || v2[8]) return 0;
  return 1;
} /* jang_match24() */

/* ==================================== */
static int32_t jang_match25(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (!v[0] || v[2] || !v[4] || !v[5] || !v[6] || !v[7]) return 0;
  if (v2[12]) return 0;
  return 1;
} /* jang_match25() */

/* ==================================== */
static int32_t jang_match26(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (!v[0] || !v[1] || !v[2] || v[4] || !v[6] || !v[7]) return 0;
  if (v2[0]) return 0;
  return 1;
} /* jang_match26() */

/* ==================================== */
static int32_t jang_match27(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[1] || v[2] || v[3] || v[4] || v[5] || !v[6]) goto fail;
    if (v2[0] || v2[1] || v2[11] || v2[12] || v2[13] || v2[14] || v2[15]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* jang_match27() */

/* ==================================== */
static int32_t jang_match27b(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[1] || v[2] || v[3] || v[4] || v[5] || !v[6] || v[7]) goto fail;
    if (v2[0] || v2[1] || v2[11] || v2[12] || v2[13] || v2[15]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* jang_match27b() */

/* ==================================== */
static int32_t jang_match28b(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[1] || v[2] || v[3] || v[4] || v[5] || !v[6] || !v[7]) goto fail;
    if (v2[0] || v2[1] || v2[11] || v2[12] || v2[13] || v2[14] || v2[15]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* jang_match28b() */

/* ==================================== */
int32_t lskeljang(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit)
/* ==================================== */
// described in Jang and Chin : "One-pass parallel thinning: analysis, properties and quantitative evaluation" 1992
#undef F_NAME
#define F_NAME "lskeljang"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (F[i])
      {
	m1 = jang_match1(F, i, rs, N);
	m2 = jang_match5(F, i, rs, N);
	m3 = jang_match9(F, i, rs, N);
	m4 = jang_match13(F, i, rs, N);
	m5 = jang_match17(F, i, rs, N);
	m6 = jang_match21(F, i, rs, N);
	m7 = jang_match22(F, i, rs, N);
	m8 = jang_match23(F, i, rs, N);
	m9 = jang_match24(F, i, rs, N);
	m10 = jang_match25(F, i, rs, N);
	m11 = jang_match26(F, i, rs, N);
	m12 = jang_match27(F, i, rs, N);
	if ((m1 || m2 || m3 || m4 || m5) && !m6 && !m7 && !m8 && !m9 && !m10 && !m11 && !m12)
	{
//#define DEBUG_JANG
#ifdef DEBUG_JANG
	  printf("i = %d,%d : %d %d %d    %d %d %d    %d %d %d    %d %d %d\n",
		  i%rs, i/rs, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12);
#endif
	  T[i] = 1; // delete point

	}
      }

    for (i = 0; i < N; i++)
      if (T[i]) { F[i] = 0; nonstab = 1; }
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskeljang() */

/* ==================================== */
int32_t lskeljangcor(struct xvimage *image,
		 int32_t nsteps,
		 struct xvimage *inhibit)
/* ==================================== */
// described in Jang and Chin : "One-pass parallel thinning: analysis, properties and quantitative evaluation"
// correction by Michel Couprie
#undef F_NAME
#define F_NAME "lskeljang"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (F[i])
      {
	m1 = jang_match1(F, i, rs, N);
	m2 = jang_match5(F, i, rs, N);
	m3 = jang_match9(F, i, rs, N);
	m4 = jang_match13(F, i, rs, N);
	m5 = jang_match17(F, i, rs, N);
	m6 = jang_match21(F, i, rs, N);
	m7 = jang_match22(F, i, rs, N);
	m8 = jang_match23(F, i, rs, N);
	m9 = jang_match24(F, i, rs, N);
	m10 = jang_match25(F, i, rs, N);
	m11 = jang_match26(F, i, rs, N);
	m12 = jang_match27b(F, i, rs, N);
	m13 = jang_match28b(F, i, rs, N);
	if ((m1 || m2 || m3 || m4 || m5) && !m6 && !m7 && !m8 && !m9 && !m10 && !m11 && !m12 && !m13)
	{
#ifdef DEBUG_JANG
	  printf("i = %d,%d : %d %d %d    %d %d %d    %d %d %d    %d %d %d %d\n",
		  i%rs, i/rs, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13);
#endif
	  T[i] = 1; // delete point

	}
      }

    for (i = 0; i < N; i++)
      if (T[i]) { F[i] = 0; nonstab = 1; }
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskeljangcor() */

/* ==================================== */
int32_t mns_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[0] != 2) goto fail;
    if ((v[1] == 0) && (v[2] == 0) && (v[6] == 0) && (v[7] == 0)) return 1;
    if ((v[1] == 0) && (v[2] == 0)) goto fail;
    if ((v[6] == 0) && (v[7] == 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* mns_match1() */

/* ==================================== */
int32_t mns_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if ((v[0] != 2) || (v[1] != 2) || (v[2] != 2)) goto fail;
    if ((v2[0] != 0) || (v2[1] != 0) || (v2[2] != 0) ||
	(v2[3] != 0) || (v2[4] != 0) || (v2[5] != 0) || (v2[15] != 0) ||
	(v[3] != 0) || (v[4] != 0) || (v[5] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mns_match2() */

/* ==================================== */
int32_t mns_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if ((v[0] != 0) || (v[1] != 2) || (v[2] != 2)) goto fail;
    if ((v2[0] != 0) || (v2[1] != 0) || (v2[2] != 0) ||
	(v2[3] != 0) || (v2[4] != 0) || (v2[5] != 0) ||
	(v[3] != 0) || (v[4] != 0) || (v[5] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mns_match3() */

/* ==================================== */
int32_t mns_match4(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if ((v[0] != 2) || (v[1] != 2) || (v[2] != 0)) goto fail;
    if ((v2[0] != 0) || (v2[1] != 0) || (v2[2] != 0) ||
	(v2[3] != 0) || (v2[4] != 0) || (v2[15] != 0) ||
	(v[3] != 0) || (v[4] != 0) || (v[5] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mns_match4() */

/* ==================================== */
int32_t mns_match5(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if ((v[0] != 2) || (v[1] != 0) || (v[2] != 2)) goto fail;
    if ((v2[0] != 0) || (v2[1] != 0) || (v2[3] != 0) ||
	(v2[4] != 0) || (v2[5] != 0) || (v2[15] != 0) ||
	(v[3] != 0) || (v[4] != 0) || (v[5] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mns_match5() */

/* ==================================== */
int32_t mns_match6(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if ((v[0] != 0) || (v[1] != 2) || (v[2] != 0)) goto fail;
    if ((v2[0] != 0) || (v2[1] != 0) || (v2[2] != 0) || (v2[3] != 0) || (v2[4] != 0) ||
	(v[3] != 0) || (v[4] != 0) || (v[5] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mns_match6() */

/* ==================================== */
int32_t lskelmns(struct xvimage *image,
	     int32_t nsteps,
	     struct xvimage *inhibit)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelmns"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N))
	F[i] = 2;
    for (i = 0; i < N; i++)
      if (F[i] == 2)
      {
	m1 = mns_match1(F, i, rs, N);
	m2 = mns_match2(F, i, rs, N);
	m3 = mns_match3(F, i, rs, N);
	m4 = mns_match4(F, i, rs, N);
	m5 = mns_match5(F, i, rs, N);
	m6 = mns_match6(F, i, rs, N);
	if (m1 || m2 || m3 || m4 || m5 || m6)
	{
#ifdef DEBUG_MNS
	  printf("point %d,%d : m1 = %d; m2 = %d; m3 = %d; m4 = %d; m5 = %d; m6 = %d\n",
		 i % rs, i / rs, m1, m2, m3, m4, m5, m6);
#endif
	  T[i] = 1; // preserve point
	}
      }

#ifdef DEBUG_MNS
    printf("\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", F[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    for (i = 0; i < N; i++)
      if ((F[i] == 2) && !T[i]) { F[i] = 0; nonstab = 1; }
    for (i = 0; i < N; i++) if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelmns() */

/* ==================================== */
static int32_t jangrec_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 q   avec origine = (1,1) et (p ou q)
  0 1 1
  p 1 0
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[1] && !v[5]) goto fail;
    if (!v[0] || v[2] || v[3] || v[4] || !v[6]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* jangrec_match1() */

/* ==================================== */
static int32_t jangrec_match5(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 x   avec origine = (1,1) et x = don't care
  0 1 1
  x 1 1
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[2] || v[3] || v[4] || !v[6] || !v[7]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* jangrec_match5() */

/* ==================================== */
static int32_t jangrec_match9(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 0 x   avec origine = (1,1) et x = don't care
  1 1 1
  1 1 1
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[2] || !v[4] || !v[5] || !v[6] || !v[7]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* jangrec_match9() */

/* ==================================== */
static int32_t jangrec_match13(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0   avec origine = (1,1)
  0 1 0
  1 1 1
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[0] || v[1] || v[2] || v[3] || v[4] || !v[5] || !v[6] || !v[7]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* jangrec_match13() */

/* ==================================== */
static int32_t jangrec_match17(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (!v[0] || v[2] || !v[4] || !v[5] || !v[6] || !v[7]) return 0;
  if (v2[12]) return 0;
  return 1;
} /* jangrec_match17() */

/* ==================================== */
static int32_t jangrec_match18(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (!v[0] || !v[1] || !v[2] || v[4] || !v[6] || !v[7]) return 0;
  if (v2[0]) return 0;
  return 1;
} /* jangrec_match18() */

/* ==================================== */
static int32_t jangrec_match19(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[0] || !v[1] || !v[2] || v[3] || !v[4] || v[6] || v[7]) return 0;
  if (v2[4] || v2[5]) return 0;
  return 1;
} /* jangrec_match19() */

/* ==================================== */
static int32_t jangrec_match20(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[0] || v[1] || v[2] || !v[4] || v[5] || !v[6] || !v[7]) return 0;
  if (v2[11] || v2[12]) return 0;
  return 1;
} /* jangrec_match20() */

/* ==================================== */
static int32_t jangrec_match21(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[0] || v[1] || v[2] || !v[3] || !v[4] || v[5] || !v[6]) return 0;
  if (v2[9] || v2[8]) return 0;
  return 1;
} /* jangrec_match21() */

/* ==================================== */
static int32_t jangrec_match22(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[0] || !v[2] || v[3] || !v[4] || !v[5] || v[6] || v[7]) return 0;
  if (v2[7] || v2[8]) return 0;
  return 1;
} /* jangrec_match22() */

/* ==================================== */
static int32_t jangrec_match23(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
   0 0 0 0   avec origine = (1,1)
   0 1 1 0
   0 1 1 0
   0 0 0 0
*/
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (!v[0] || v[1] || v[2] || v[3] || v[4] || v[5] || !v[6] || !v[7]) return 0;
  if (v2[0] || v2[1] || v2[11] || v2[12] || v2[13] || v2[14] || v2[15]) return 0;
  return 1;
} /* jangrec_match23() */

/* ==================================== */
int32_t lskeljangrec(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit)
/* ==================================== */
// described in Jang and Chin : "Reconstructable  parallel thinning"
#undef F_NAME
#define F_NAME "lskeljangrec"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I;            /* l'image d'inhibition (axe médian) */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11;

  if (inhibit == NULL)
  {
    fprintf(stderr, "%s: inhibit image (medial axis) must be present\n", F_NAME);
    return 0;
  }
  I = UCHARDATA(inhibit);

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (F[i])
      {
	m1 = jangrec_match1(F, i, rs, N);
	m2 = jangrec_match5(F, i, rs, N);
	m3 = jangrec_match9(F, i, rs, N);
	m4 = jangrec_match13(F, i, rs, N);
	m5 = jangrec_match17(F, i, rs, N);
	m6 = jangrec_match18(F, i, rs, N);
	m7 = jangrec_match19(F, i, rs, N);
	m8 = jangrec_match20(F, i, rs, N);
	m9 = jangrec_match21(F, i, rs, N);
	m10 = jangrec_match22(F, i, rs, N);
	m11 = jangrec_match23(F, i, rs, N);
	if ((m1 || m2 || m3 || m4) && !m5 && !m6 && !m7 && !m8 && !m9 && !m10 && !m11)
	{
//#define DEBUG_JANG
#ifdef DEBUG_JANG
	  printf("i = %d,%d : %d %d %d    %d %d %d    %d %d %d    %d %d\n",
		  i%rs, i/rs, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11);
#endif
	  T[i] = 1; // delete point

	}
      }

    for (i = 0; i < N; i++)
      if (T[i] && !I[i]) { F[i] = 0; nonstab = 1; }
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskeljangrec() */

/* ==================================== */
static int32_t choy_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 x   avec origine = (1,1) et x = don't care
  0 1 1
  x 1 x
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[2] || v[3] || v[4] || !v[6]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* choy_match1() */

/* ==================================== */
static int32_t choy_match5(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 0 x   avec origine = (1,1) et x = don't care
  1 1 1
  1 1 1
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[2] || !v[4] || !v[5] || !v[6] || !v[7]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* choy_match5() */

/* ==================================== */
static int32_t choy_match9(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0   avec origine = (1,1) et (p ou q)
  0 1 0
  p 1 q
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[5] && !v[7]) goto fail;
    if (v[0] || v[1] || v[2] || v[3] || v[4] || !v[6]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* choy_match9() */

/* ==================================== */
static int32_t choy_matchb1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 0 x
  1 1 1
  1 1 1
    0
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return v[0] && !v[2] && v[4] && v[5] && v[6] && v[7] && !v2[12];
} /* choy_matchb1() */

/* ==================================== */
static int32_t choy_matchb2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 0 0
  1 1 1
  0 1 1
    0
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return v[0] && !v[1] && !v[2] && v[4] && !v[5] && v[6] && v[7] && !v2[12];
} /* choy_matchb2() */

/* ==================================== */
static int32_t choy_matchb3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 x
  0 1 1
  1 1 0
    0
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return v[0] && !v[2] && !v[3] && !v[4] && v[5] && v[6] && !v[7] && !v2[12];
} /* choy_matchb3() */

/* ==================================== */
static int32_t choy_matchb7(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 1 1
  0 1 1 0
  x 1 1
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return v[0] && v[1] && v[2] && !v[4] && v[6] && v[7] && !v2[0];
} /* choy_matchb7() */

/* ==================================== */
static int32_t choy_matchb8(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 1
  0 1 1 0
  x 1 0
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return v[0] && v[1] && !v[2] && !v[3] && !v[4] && v[6] && !v[7] && !v2[0];
} /* choy_matchb8() */

/* ==================================== */
static int32_t choy_matchb11(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
    1 0 0
  0 1 1 0
    0 1 x
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return !v[0] && !v[1] && !v[2] && v[3] && v[4] && !v[5] && v[6] && !v2[8];
} /* choy_match11() */

/* ==================================== */
static int32_t choy_matchb21(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0
  0 1 1 0
  0 1 1
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return v[0] && !v[1] && !v[2] && !v[3] && !v[4] && !v[5] && v[6] && v[7] && !v2[0];
} /* choy_match21() */

/* ==================================== */
static int32_t choy_matchb22(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0
  0 1 1
  0 1 1
    0
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return v[0] && !v[1] && !v[2] && !v[3] && !v[4] && !v[5] && v[6] && v[7] && !v2[12];
} /* choy_match22() */

/* ==================================== */
static int32_t choy_matchb23(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0
  1 1 0
  1 1 0
    0
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return !v[0] && !v[1] && !v[2] && !v[3] && v[4] && v[5] && v[6] && !v[7] && !v2[12];
} /* choy_match23() */

/* ==================================== */
static int32_t choy_matchb24(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
    0 0 0
  0 1 1 0
    1 1 0
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  return !v[0] && !v[1] && !v[2] && !v[3] && v[4] && v[5] && v[6] && !v[7] && !v2[8];
} /* choy_match24() */

/* ==================================== */
int32_t lskelchoy(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit)
/* ==================================== */
// described in Choy and Choy and Sui : ""
#undef F_NAME
#define F_NAME "lskelchoy"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (F[i])
      {
	m1 = choy_match1(F, i, rs, N);
	m2 = choy_match5(F, i, rs, N);
	m3 = choy_match9(F, i, rs, N);
	m4 = choy_matchb1(F, i, rs, N);
	m5 = choy_matchb2(F, i, rs, N);
	m6 = choy_matchb3(F, i, rs, N);
	m7 = choy_matchb7(F, i, rs, N);
	m8 = choy_matchb8(F, i, rs, N);
	m9 = choy_matchb11(F, i, rs, N);
	m10 = choy_matchb21(F, i, rs, N);
	m11 = choy_matchb22(F, i, rs, N);
	m12 = choy_matchb23(F, i, rs, N);
	m13 = choy_matchb24(F, i, rs, N);
	if ((m1 || m2 || m3) && !m4 && !m5 && !m6 && !m7 && !m8 && !m9 && !m10 && !m11 && !m12 && !m13)
	  //if ((m1 || m2 || m3) && !m4 && !m5 && !m6 && !m7 && !m8 && !m9 && !m10 && !m11 && !m12 && !m13 &&
	  //!jang_match27b(F, i, rs, N) && !jang_match27b(F, i, rs, N)) // correction mc
	{
//#define DEBUG_CHOY
#ifdef DEBUG_CHOY
	  printf("i = %d,%d : %d %d %d    %d %d %d    %d %d %d    %d %d %d    %d\n",
		  i%rs, i/rs, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13);
#endif
	  T[i] = 1; // delete point

	}
      }

    for (i = 0; i < N; i++)
      if (T[i]) { F[i] = 0; nonstab = 1; }
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelchoy() */

/* ==================================== */
int32_t mb_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x x 1 x
  0 P 1 1
  x x 1 x
*/
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || !v[1] || !v[7] || v[4]) goto fail;
    if (!v2[0]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mb_match1() */

/* ==================================== */
int32_t mb_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 x x
  0 P 1 x
  x 1 1 1
  x x 1 x
*/
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if (!v[0] || v[2] || v[3] || v[4] || !v[6] || !v[7]) goto fail;
    if (!v2[13] || !v2[15]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mb_match2() */

/* ==================================== */
int32_t mb_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x x x
  x P 0
  x 0 1
*/
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!v[0] && !v[6] && v[7])  return 1;
    rotate90_vois(v);
  }
  return 0;
} /* mb_match3() */

/* ==================================== */
int32_t lskelmanz(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit,
	      int32_t variante)
/* ==================================== */
// described in Manzanera and Bernard : "Improved low complexity fully parallel thinning algorithm"
// variante =
//  1 : algo original
//  2 : variante GB (seulement masques 1 et 3)
#undef F_NAME
#define F_NAME "lskelmanz"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if ((variante < 1) || (variante > 2))
  {
    fprintf(stderr, "%s: variante: must be 1 or 2\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (F[i])
      {
	m1 = mb_match1(F, i, rs, N);
	if (variante == 1)
	  m2 = mb_match2(F, i, rs, N);
	else
	  m2 = 0;
	m3 = mb_match3(F, i, rs, N);
	if ((m1 || m2) && !m3)
	{
//#define DEBUG_MANZ
#ifdef DEBUG_MANZ
	  printf("i = %d,%d : %d %d %d\n",
		  i%rs, i/rs, m1, m2, m3);
#endif
	  T[i] = 1; // delete point

	}
      }

    for (i = 0; i < N; i++)
      if (T[i]) { F[i] = 0; nonstab = 1; }
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelmanz() */

/* ==================================== */
static int32_t hall_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return ((v[0] == 2) && v[2] && v[6]);
} /* hall_match1() */

/* ==================================== */
static int32_t hall_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return ((v[6] == 2) && v[0] && v[4]);
} /* hall_match2() */

/* ==================================== */
static int32_t hall_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return ((v[6] == 2) && (v[7] == 2) && (v[0] == 2));
} /* hall_match3() */

/* ==================================== */
int32_t lskelhall(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit,
	      int32_t variante)
/* ==================================== */
// described in Hall : "Fast parallel thinning algorithms: parallel speed and connectivity preservation"
// variante 1 : Holt et al.
// variante 2 : variante Hall pour "mieux" préserver les diagonales (?)
#undef F_NAME
#define F_NAME "lskelhall"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

#ifdef VERBOSE
  printf("nstep=%d, variante=%d\n", nsteps, variante);
#endif

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);

    switch (variante)
    {
    case 1:
      for (i = 0; i < N; i++)
	if (F[i] && (nbvois8(F, i, rs, N) > 1)  && (nbvois8(F, i, rs, N) < 7)
	    && (crossing_nb(F, i, rs, N) == 1))
	  F[i] = 2;
      for (i = 0; i < N; i++)
	if ((F[i] == 2) && !hall_match1(F, i, rs, N) && !hall_match2(F, i, rs, N) && !hall_match3(F, i, rs, N))
	{
	  nonstab = 1;
	  T[i] = 1;
	}
      for (i = 0; i < N; i++)
	if (T[i] == 1) F[i] = 0;
	else if (F[i]) F[i] = 1;
      break;
    case 2:
      for (i = 0; i < N; i++)
	if (F[i] && (nbvois8(F, i, rs, N) > 2)  && (nbvois8(F, i, rs, N) < 7)
	    && (crossing_nb(F, i, rs, N) == 1))
	  F[i] = 2;
      for (i = 0; i < N; i++)
	if ((F[i] == 2) && !hall_match1(F, i, rs, N) && !hall_match2(F, i, rs, N) && !hall_match3(F, i, rs, N))
	{
	  nonstab = 1;
	  T[i] = 1;
	}

//#define DEBUG_HALL2
#ifdef DEBUG_HALL2
    printf("\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", F[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

      for (i = 0; i < N; i++)
	if (T[i] == 1) F[i] = 0;
	else if (F[i]) F[i] = 1;
      break;
    default:
      fprintf(stderr, "%s: variant not implemented\n", F_NAME);
      return 0;
    } // switch (variante)

  } // while (nonstab && (step < nsteps))

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelhall() */

/* ==================================== */
static int32_t wutsai_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  1 1 p   avec origine = (1,1) et (!p ou !q)
  1 1 0
  1 1 q
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (v[1] && v[7]) return 0;
  if (v[0] || !v[2] || !v[3] || !v[4] || !v[5] || !v[6]) return 0;
  return 1;
} /* wutsai_match1() */

/* ==================================== */
static int32_t wutsai_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  1 1 1   avec origine = (1,1) et (!p ou !q)
  1 1 1
  p 0 q
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (v[5] && v[7]) return 0;
  if (v[6] || !v[0] || !v[1] || !v[2] || !v[3] || !v[4]) return 0;
  return 1;
} /* wutsai_match2() */

/* ==================================== */
static int32_t wutsai_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  p 1 1     avec origine = (1,1) et (!p ou !q)
  0 1 1 1
  q 1 1
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[3] && v[5]) return 0;
  if (v[4] || !v[0] || !v[1] || !v[2] || !v[6] || !v[7]) return 0;
  if (!v2[0]) return 0;
  return 1;
} /* wutsai_match3() */

/* ==================================== */
static int32_t wutsai_match4(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  p 0 q     avec origine = (1,1) et (!p ou !q)
  1 1 1
  1 1 1
    1
 */
{
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  if (v[1] && v[3]) return 0;
  if (v[2] || !v[0] || !v[4] || !v[5] || !v[6] || !v[7]) return 0;
  if (!v2[12]) return 0;
  return 1;
} /* wutsai_match4() */

/* ==================================== */
static int32_t wutsai_match5(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 0 0   avec origine = (1,1) et x = don't care
  1 1 0
  x 1 x
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return (!v[0] && !v[1] && !v[2] && v[4] && v[6]);
} /* wutsai_match5() */

/* ==================================== */
static int32_t wutsai_match6(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 1 1   avec origine = (1,1) et x = don't care
  0 1 1
  0 0 x
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return (v[0] && v[1] && v[2] && !v[4] && !v[5] && !v[6]);
} /* wutsai_match6() */

/* ==================================== */
static int32_t wutsai_match7(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 1 0   avec origine = (1,1)
  0 1 1
  0 0 0
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return (v[0] && !v[1] && v[2] && !v[3] && !v[4] && !v[5] && !v[6] && !v[7]);
} /* wutsai_match7() */

/* ==================================== */
static int32_t wutsai_match8(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  x 1 x   avec origine = (1,1) et x = don't care
  1 1 0
  x 0 0
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return (!v[0] && v[2] && v[4] && !v[6] && !v[7]);
} /* wutsai_match8() */

/* ==================================== */
static int32_t wutsai_match9(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 x   avec origine = (1,1) et x = don't care
  0 1 1
  x 1 1
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return (v[0] && !v[2] && !v[3] && !v[4] && v[6] && v[7]);
} /* wutsai_match9() */

/* ==================================== */
static int32_t wutsai_match10(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0   avec origine = (1,1)
  0 1 1
  0 1 0
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  return (v[0] && !v[1] && !v[2] && !v[3] && !v[4] && !v[5] && v[6] && !v[7]);
} /* wutsai_match10() */

/* ==================================== */
static int32_t wutsai_match11(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  0 0 0   avec origine = (1,1)
  0 1 0
  1 1 1
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[0] || v[1] || v[2] || v[3] || v[4] || !v[5] || !v[6] || !v[7]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* wutsai_match11() */

/* ==================================== */
int32_t lskelwutsai(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit)
/* ==================================== */
// described in Wutsai and Wutsai and Sui : "A new one-pass parallel thinning algorithm for binary images"
#undef F_NAME
#define F_NAME "lskelwutsai"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (F[i])
      {
	m1 = wutsai_match1(F, i, rs, N);
	m2 = wutsai_match2(F, i, rs, N);
	m3 = wutsai_match3(F, i, rs, N);
	m4 = wutsai_match4(F, i, rs, N);
	m5 = wutsai_match5(F, i, rs, N);
	m6 = wutsai_match6(F, i, rs, N);
	m7 = wutsai_match7(F, i, rs, N);
	m8 = wutsai_match8(F, i, rs, N);
	m9 = wutsai_match9(F, i, rs, N);
	m10 = wutsai_match10(F, i, rs, N);
	m11 = wutsai_match11(F, i, rs, N);
	if (m1 || m2 || m3 || m4 || m5 || m6 || m7 || m8 || m9 || m10 || m11)
	  //if ((m1 || m2 || m3 || m4 || m5 || m6 || m7 || m8 || m9 || m10 || m11) && !jangrec_match23(F, i, rs, N)) // cor. MC
	{
	  //#define DEBUG_WUTSAI
#ifdef DEBUG_WUTSAI
	  printf("i = %d,%d : %d %d %d    %d %d %d    %d %d %d    %d %d\n",
		  i%rs, i/rs, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11);
#endif
	  T[i] = 1; // delete point
	}
      }

    for (i = 0; i < N; i++)
      if (T[i]) { F[i] = 0; nonstab = 1; }
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelwutsai() */

/* ==================================== */
static int32_t mc_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
//      m1 : match   A 2 B   avec origine = (1,0) et [au moins un des A et au moins un des B non nuls]
//                   A 2 B   ou [A et B tous nuls] (+ rotation 90)
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!IS_SIMPLE(v[0])) goto fail;
    if ((v[1] == 0) && (v[2] == 0) && (v[6] == 0) && (v[7] == 0)) return 1;
    if ((v[1] == 0) && (v[2] == 0)) goto fail;
    if ((v[6] == 0) && (v[7] == 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* mc_match1() */

/* ==================================== */
static int32_t mc_match1b(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
//     m1b : match   0 2 0   avec origine = (1,0) (+ rotation 90)
//                   0 2 0
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!IS_SIMPLE(v[0])) goto fail;
    if ((v[1] == 0) && (v[2] == 0) && (v[6] == 0) && (v[7] == 0)) return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* mc_match1b() */

/* ==================================== */
static int32_t mc_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if ((!IS_SIMPLE(v[0])) || (!IS_SIMPLE(v[1])) || (!IS_SIMPLE(v[2]))) goto fail;
    if ((v2[0] != 0) || (v2[1] != 0) || (v2[3] != 0) || (v2[4] != 0) ||
	(v[3] != 0) || (v[4] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mc_match2() */

/* ==================================== */
static int32_t mc_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if ((v[0] != 0) || (!IS_SIMPLE(v[1])) || (!IS_SIMPLE(v[2]))) goto fail;
    if ((v2[3] != 0) || (v2[4] != 0) ||
	(v[3] != 0) || (v[4] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mc_match3() */

/* ==================================== */
static int32_t mc_match4(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8], v2[16];
  extract_vois(F, x, rs, N, v);
  if (!extract_vois2(F, x, rs, N, v2)) return 0;
  for (i = 0; i < 4; i++)
  {
    if ((!IS_SIMPLE(v[0])) || (!IS_SIMPLE(v[1])) || (v[2] != 0)) goto fail;
    if ((v2[0] != 0) || (v2[1] != 0) ||
	(v[6] != 0) || (v[7] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
    rotate90_vois2(v2);
  }
  return 0;
} /* mc_match4() */

/* ==================================== */
static int32_t mc_match5(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if ((!IS_SIMPLE(v[0])) || (v[1] != 0) || (!IS_SIMPLE(v[2]))) goto fail;
    if ((v[3] != 0) || (v[4] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* mc_match5() */

/* ==================================== */
static int32_t mc_match6(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if ((!IS_SIMPLE(v[3])) || (v[2] != 0) || (v[4] != 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* mc_match6() */

#ifdef NOT_USED
/* ==================================== */
static int32_t mc_end(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 8; i++)
  {
    if (v[0] == 0) goto fail;
    if ((v[2] != 0) || (v[3] != 0) || (v[4] != 0) || (v[5] != 0) || (v[6] != 0)) goto fail;
    return 1;
  fail:
    rotate45_vois(v);
  }
  return 0;
} /* mc_end() */
#endif

/* ==================================== */
static int32_t mc_maskend1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[0] == 0) goto fail;
    if ((v[1] != 0) || (v[2] != 0) || (v[3] != 0) || (v[4] != 0) || (v[5] != 0) ||
	(v[6] != 0) || (v[7] != 0)) goto fail;
//printf("mc_maskend1 : %d %d end\n", x % rs, x / rs);
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* mc_maskend1() */

/* ==================================== */
static int32_t mc_maskend2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[1] == 0) goto fail;
    if ((v[3] != 0) || (v[4] != 0) || (v[5] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
//printf("mc_maskend2 : i = %d ; %d %d end\n", i, x % rs, x / rs);
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* mc_maskend2() */

/* ==================================== */
static int32_t mc_maskend3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if ((v[0] == 0) || (v[2] == 0)) goto fail;
    if ((v[3] != 0) || (v[4] != 0) || (v[5] != 0) || (v[6] != 0) || (v[7] != 0)) goto fail;
//printf("mc_maskend3 : %d %d end\n", x % rs, x / rs);
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* mc_maskend3() */

/* ==================================== */
static int32_t mc_end2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  return mc_maskend1(F, x, rs, N) || mc_maskend2(F, x, rs, N) || mc_maskend3(F, x, rs, N);
} /* mc_end2() */

/* ==================================== */
int32_t lskelmcultime(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelmcultime"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N))
	F[i] = 2;
    for (i = 0; i < N; i++)
      if (F[i] == 2)
      {
	m1 = mc_match1(F, i, rs, N);
	m2 = mc_match2(F, i, rs, N);
	m3 = mc_match3(F, i, rs, N);
	m4 = mc_match4(F, i, rs, N);
	m5 = mc_match5(F, i, rs, N);
	m6 = mc_match6(F, i, rs, N);
	if (m1 || m2 || m3 || m4 || m5 || m6)
	{
#ifdef DEBUG_MC
	  printf("point %d,%d : m1 = %d; m2 = %d; m3 = %d; m4 = %d; m5 = %d; m6 = %d\n",
		 i % rs, i / rs, m1, m2, m3, m4, m5, m6);
#endif
	  T[i] = 1; // preserve point
	}
      }

#ifdef DEBUG_MC
    printf("\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", F[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    for (i = 0; i < N; i++)
      if ((F[i] == 2) && !T[i]) { F[i] = 0; nonstab = 1; }
    for (i = 0; i < N; i++) if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelmcultime() */

/* ==================================== */
int32_t lskelmccurv(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelmccurv"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N)  && !mc_end2(F, i, rs, N))
	F[i] = 2;
    for (i = 0; i < N; i++)
      if (F[i] == 2)
      {
	m1 = mc_match1(F, i, rs, N);
	m2 = mc_match2(F, i, rs, N);
	m3 = mc_match3(F, i, rs, N);
	m4 = mc_match4(F, i, rs, N);
	m5 = mc_match5(F, i, rs, N);
	m6 = mc_match6(F, i, rs, N);
	if (m1 || m2 || m3 || m4 || m5 || m6)
	{
#ifdef DEBUG_MC1
	  printf("point %d,%d : m1 = %d; m2 = %d; m3 = %d; m4 = %d; m5 = %d; m6 = %d\n",
		 i % rs, i / rs, m1, m2, m3, m4, m5, m6);
#endif
	  T[i] = 1; // preserve point
	}
      }

#ifdef DEBUG_MC
    printf("\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", F[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    for (i = 0; i < N; i++)
      if ((F[i] == 2) && !T[i]) { F[i] = 0; nonstab = 1; }
    for (i = 0; i < N; i++) if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelmccurv() */

/* ==================================== */
int32_t lskelmccurvrec(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelmccurvrec"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  uint8_t *M = (uint8_t *)malloc(N);
  uint8_t *E = (uint8_t *)malloc(N);
  uint8_t *R = (uint8_t *)malloc(N);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (X[i]) X[i] = 1; // normalize values

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  memset(M, 0, N);
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    memset(E, 0, N);
    for (i = 0; i < N; i++)
      if (T[i] && (nbvois4(T, i, rs, N) == 4)) E[i] = 1; // calcule E = eros (T)
    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (nbvois4(E, i, rs, N) >= 1) R[i] = 1;           // calcule D = Dilat(E)
    for (i = 0; i < N; i++)
      if (T[i] && !R[i]) R[i] = 1; else R[i] = 0;        // calcule D = T \ Dilat(E)
    for (i = 0; i < N; i++) T[i] = E[i];                 // T = E
    for (i = 0; i < N; i++) if (R[i]) M[i] = 1;          // M  = M union D

//#define DEBUG_MC

#ifdef DEBUG_MC
    printf("D, T\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", R[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    for (i = 0; i < N; i++)
      if ((X[i] == 1) && simple8(X, i, rs, N) && !M[i])
	X[i] = 2;

#ifdef DEBUG_MC
    printf("M, X\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", M[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", X[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (X[i] == 2)
      {
	m1 = mc_match1(X, i, rs, N);
	m2 = mc_match2(X, i, rs, N);
	m3 = mc_match3(X, i, rs, N);
	m4 = mc_match4(X, i, rs, N);
	m5 = mc_match5(X, i, rs, N);
	m6 = mc_match6(X, i, rs, N);
	if (m1 || m2 || m3 || m4 || m5 || m6)
	{
#ifdef DEBUG_MC
	  printf("point %d,%d : m1 = %d; m2 = %d; m3 = %d; m4 = %d; m5 = %d\n",
		 i % rs, i / rs, m1, m2, m3, m4, m5);
#endif
	  R[i] = 1; // preserve point
	}
      }

#ifdef DEBUG_MC
    printf("X, R\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", X[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", R[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    for (i = 0; i < N; i++)
      if ((X[i] == 2) && !R[i] && !M[i]) { X[i] = 0; nonstab = 1; }
    for (i = 0; i < N; i++) if (X[i]) X[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i]) X[i] = 255; // normalize values

  freeimage(tmp);
  free(M);
  free(E);
  free(R);
  return(1);
} /* lskelmccurvrec() */

/* ==================================== */
int32_t lskelmccurvrecold(struct xvimage *image,
		  int32_t nsteps,
		  struct xvimage *inhibit)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelmccurvrec"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  uint8_t *Y = (uint8_t *)malloc(N);
  uint8_t *I = (uint8_t *)malloc(N);
  uint8_t *D = (uint8_t *)malloc(N);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (X[i]) X[i] = 1; // normalize values

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  memset(Y, 0, N);
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif
    memset(T, 0, N);
    memset(I, 0, N);
    memset(D, 0, N);
    for (i = 0; i < N; i++) if (X[i] && !Y[i]) T[i] = 1; // calcule T = X \ Y
    for (i = 0; i < N; i++)
      if (T[i] && (nbvois4(T, i, rs, N) == 4))
	I[i] = 1; // calcule I = interior (X \ Y)
    for (i = 0; i < N; i++)
//      if (T[i] && (nbvois4(I, i, rs, N) >= 1)) // test T
      if (nbvois4(I, i, rs, N) >= 1)
	D[i] = 1; // calcule D = Dilat(I) Inter [X \ Y]

//#define DEBUG_MC

#ifdef DEBUG_MC
    printf("I, D\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", I[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", D[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (Y[i] || (X[i] && !D[i]))
	T[i] = 1; // calcule E = Y Union [X \ D]
    for (i = 0; i < N; i++)
      if ((X[i] == 1) && simple8(X, i, rs, N) && !T[i])
	X[i] = 2;

#ifdef DEBUG_MC
    printf("E, X\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", X[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if (X[i] == 2)
      {
	m1 = mc_match1(X, i, rs, N);
	m2 = mc_match2(X, i, rs, N);
	m3 = mc_match3(X, i, rs, N);
	m4 = mc_match4(X, i, rs, N);
	m5 = mc_match5(X, i, rs, N);
	m6 = mc_match6(X, i, rs, N);
	if (m1 || m2 || m3 || m4 || m5 || m6)
	{
#ifdef DEBUG_MC
	  printf("point %d,%d : m1 = %d; m2 = %d; m3 = %d; m4 = %d; m5 = %d\n",
		 i % rs, i / rs, m1, m2, m3, m4, m5);
#endif
	  T[i] = 1; // preserve point
	}
      }

#ifdef DEBUG_MC
    printf("X, T\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", X[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    for (i = 0; i < N; i++)
      if ((X[i] == 2) && !T[i]) { X[i] = 0; nonstab = 1; }
    for (i = 0; i < N; i++) if (X[i]) X[i] = 1;
    for (i = 0; i < N; i++) if (X[i] && !I[i]) Y[i] = 1;

#ifdef DEBUG_MC
    printf("X, Y\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", X[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", Y[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i]) X[i] = 255; // normalize values

  freeimage(tmp);
  free(Y);
  free(I);
  free(D);
  return(1);
} /* lskelmccurvrecold() */

/* ==================================== */
int32_t bertrand_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
A     A
2*    2
B     B
*/
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (!IS_SIMPLE(v[0])) return 0;
  if ((v[1] == 0) && (v[2] == 0)) return 0;
  if ((v[6] == 0) && (v[7] == 0)) return 0;
  return 1;
} /* bertrand_match1() */

/* ==================================== */
int32_t bertrand_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
A   2   B
A   2*  B
*/
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (!IS_SIMPLE(v[2])) return 0;
  if ((v[0] == 0) && (v[1] == 0)) return 0;
  if ((v[3] == 0) && (v[4] == 0)) return 0;
  return 1;
} /* bertrand_match2() */

#ifdef NOT_FINISHED
/* ==================================== */
int32_t cruc_asym_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
A     A
2*    2
B     B
*/
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (!IS_SIMPLE(v[0])) return 0;
  if ((v[1] == 0) && (v[2] == 0)) return 0;
  if ((v[6] == 0) && (v[7] == 0)) return 0;
  return 1;
} /* bertrand_match1() */
#endif

/* ==================================== */
int32_t lskelNK2(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit)
/* ==================================== */
// algo asymetrique - ultime - minimal - avec contrainte axe median
// ATTENTION : les résultats sont incorrects si la contrainte n'est pas l'AM
#undef F_NAME
#define F_NAME "lskelNK2"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* l'image de depart */
  uint8_t *I = NULL;               /* l'image d'inhibition */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2;
#ifdef ETUDE
  struct xvimage *tmp2 = copyimage(image);
  uint8_t *T2 = UCHARDATA(tmp2);
  memset(T2, 0, N);
#endif
#ifdef ANIMATE
  int32_t nimage = 1;
  char imname[128];
#endif

  if (inhibit == NULL) 
  {
    fprintf(stderr, "%s: inhibit image (medial axis) must be given\n", F_NAME);
    return 0;
  }

  I = UCHARDATA(inhibit);

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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
    memset(T, 0, N);

    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && !I[i])
	F[i] = 2;
    for (i = 0; i < N; i++)
      if (F[i] == 2)
      {
	m1 = bertrand_match1(F, i, rs, N);
	m2 = bertrand_match2(F, i, rs, N);
	if (m1 || m2)
	{
#ifdef DEBUG_BERTRAND
	  printf("point %d,%d : m1 = %d; m2 = %d\n",
		 i % rs, i / rs, m1, m2);
#endif
	  T[i] = 1; // preserve point
	}
      }

#ifdef DEBUG_BERTRAND
    { int j;
    printf("\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", F[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
    }
#endif

    for (i = 0; i < N; i++)
      if ((F[i] == 2) && !T[i])
      {
	F[i] = 0;
	nonstab = 1;
#ifdef ETUDE
        T2[i] = step;
#endif
      }
    for (i = 0; i < N; i++) if (F[i]) F[i] = 1;

#ifdef ANIMATE
  sprintf(imname, "anim%03d.pgm", nimage); nimage++;
  writeimage(image, imname);
#endif

  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

#ifdef ETUDE
  writeimage(tmp2,"_etude");
#endif

  freeimage(tmp);
  return(1);
} /* lskelNK2() */

/* ==================================== */
int32_t lskelNK2b(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit)
/* ==================================== */
// algo asymetrique - ultime - minimal - avec contrainte quelconque
#undef F_NAME
#define F_NAME "lskelNK2b"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* l'image de depart */
  uint8_t *I = NULL;               /* l'image d'inhibition */
  int32_t step, nonstab;
  int32_t m1, m2, mc1, mc2, mc3, mc4, mc5, mc6;

  if (inhibit != NULL) I = UCHARDATA(inhibit);
  if (nsteps == -1) nsteps = 1000000000;
  for (i = 0; i < N; i++) if (F[i]) F[i] = S_OBJECT; // normalize values

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
      if ((F[i] == 1) && simple8(F, i, rs, N) && ((I == NULL) || !I[i]))
	SET_SIMPLE(F[i]);
    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]))
      {
	m1 = bertrand_match1(F, i, rs, N);
	m2 = bertrand_match2(F, i, rs, N);
	mc1 = mc_match1b(F, i, rs, N);
	mc2 = mc_match2(F, i, rs, N);
	mc3 = mc_match3(F, i, rs, N);
	mc4 = mc_match4(F, i, rs, N);
	mc5 = mc_match5(F, i, rs, N);
	mc6 = mc_match6(F, i, rs, N);
	if (m1 || m2)
	{
	  SET_CRUCIAL(F[i]); // preserve point
	}
	if (mc1 || mc2 || mc3 || mc4 || mc5 || mc6)
	{
	  SET_CRUCIAL(F[i]);
	  UNSET_SIMPLE(F[i]); // preserve point and unmark as simple
	}
      }

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]) && !IS_CRUCIAL(F[i]))
      {
	F[i] = 0;
	nonstab = 1;
      }
    for (i = 0; i < N; i++) if (F[i]) F[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  return(1);
} /* lskelNK2b() */

/* ==================================== */
int32_t lskelNK2p(struct xvimage *image,
		  int32_t nsteps,
		  double isthmus_persistence,
		  struct xvimage *inhibit)
/* ==================================== */
// algo asymetrique - avec persistence
#undef F_NAME
#define F_NAME "lskelNK2p"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* l'image de depart */
  uint8_t *I = NULL;               /* l'image d'inhibition */
  int16_t *T;
  int32_t step, nonstab;
  int32_t m1, m2, mc1, mc2, mc3, mc4, mc5, mc6;
  int32_t top, topb;

#ifdef VERBOSE
  printf("%s: nsteps = %d ; isthmus_persistence = %g\n", F_NAME, nsteps, isthmus_persistence);
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
  assert(nsteps <= INT16_MAX);
  if (nsteps == -1) nsteps = INT16_MAX;
  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values
  T = (int16_t *)malloc(N * sizeof(int16_t)); assert(T != NULL);
  for (i = 0; i < N; i++) T[i] = PERS_INIT_VAL;

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

    // DETECTE ISTHMES ET INIT PERS
    for (i = 0; i < N; i++)
    {
      if (IS_OBJECT(F[i]))
      {    
	top8(F, i, rs, N, &top, &topb);
	if ((top > 1) && (T[i] == PERS_INIT_VAL))
	  T[i] = (int16_t)step;
      }
    }

    // MARQUE LES POINTS SIMPLES NON DANS I
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && !I[i])
	SET_SIMPLE(F[i]);

    // MEMORISE DANS I LES ISTHMES PERSISTANTS
    for (i = 0; i < N; i++)
    { 
      if ((T[i] != PERS_INIT_VAL) && ((double)(step - T[i]) >= isthmus_persistence)) 
      { 
	UNSET_SIMPLE(F[i]); 
	I[i] = I_INHIBIT; 
      }
    }

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]))
      {
	m1 = bertrand_match1(F, i, rs, N);
	m2 = bertrand_match2(F, i, rs, N);
	mc1 = mc_match1b(F, i, rs, N);
	mc2 = mc_match2(F, i, rs, N);
	mc3 = mc_match3(F, i, rs, N);
	mc4 = mc_match4(F, i, rs, N);
	mc5 = mc_match5(F, i, rs, N);
	mc6 = mc_match6(F, i, rs, N);
	if (m1 || m2)
	{
	  SET_CRUCIAL(F[i]); // preserve point
	}
	if (mc1 || mc2 || mc3 || mc4 || mc5 || mc6)
	{
	  SET_CRUCIAL(F[i]);
	  UNSET_SIMPLE(F[i]); // preserve point and unmark as simple
	}
      }

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]) && !IS_CRUCIAL(F[i]))
      {
	F[i] = 0;
	nonstab = 1;
      }
    for (i = 0; i < N; i++) if (F[i]) F[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  return(1);
} /* lskelNK2p() */

/* ==================================== */
int32_t lskelNK2_pers(struct xvimage *image,
		      struct xvimage *persistence)
/* ==================================== */
// calcule la fonction persistence de la méthode NK2
// VERSION SANS CONTROLE TOPOLOGIQUE
#undef F_NAME
#define F_NAME "lskelNK2_pers"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  struct xvimage *inhibit;
  uint8_t *I = NULL;               /* l'image d'inhibition */
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, mc1, mc2, mc3, mc4, mc5, mc6;
  float *P = FLOATDATA(persistence); /* résultat */

  inhibit = copyimage(image); 
  razimage(inhibit);
  I = UCHARDATA(inhibit);

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values
  for (i = 0; i < N; i++) P[i] = PERS_INIT_VAL;

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
    memset(T, 0, N);


    for (i = 0; i < N; i++)
      if (F[i] && simple8(F, i, rs, N))
	SET_SIMPLE(F[i]);

    for (i = 0; i < N; i++) // detecte les isthmes
      if (F[i] && (t8(mask(F, i, rs, N)) > 1))
      {
	  I[i] = I_INHIBIT;
      }

    // ENREGISTRE LA DATE DE NAISSANCE DES ISTHMES
    for (i = 0; i < N; i++)
    {
      if ((P[i] == PERS_INIT_VAL) && (I[i] == I_INHIBIT)) P[i] = (float)step;
    }

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]))
      {
	m1 = bertrand_match1(F, i, rs, N);
	m2 = bertrand_match2(F, i, rs, N);
	mc1 = mc_match1b(F, i, rs, N);
	mc2 = mc_match2(F, i, rs, N);
	mc3 = mc_match3(F, i, rs, N);
	mc4 = mc_match4(F, i, rs, N);
	mc5 = mc_match5(F, i, rs, N);
	mc6 = mc_match6(F, i, rs, N);
	if (m1 || m2)
	{
	  SET_CRUCIAL(F[i]); // preserve point
	}
	if (mc1 || mc2 || mc3 || mc4 || mc5 || mc6)
	{
	  SET_CRUCIAL(F[i]);
	  UNSET_SIMPLE(F[i]); // preserve point and unmark as simple
	}
      }

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]) && !IS_CRUCIAL(F[i]))
      { 
	F[i] = 0; 
	nonstab = 1; 
	if (P[i] != PERS_INIT_VAL) P[i] = (float)step - P[i]; else P[i] = 0;
      }
    for (i = 0; i < N; i++) if (F[i]) F[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) 
    if (F[i]) 
    {
      F[i] = NDG_MAX; // normalize values
      P[i] = PERS_INFTY_VAL;
    }

  freeimage(tmp);
  return(1);
} /* lskelNK2_pers() */

/* ==================================== */
int32_t bertrand_match3(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
// A 2 B   avec origine = (1,0) et au moins un des A et au moins un des B non nuls
// A 2 B   (+ rotation 90)
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!IS_SIMPLE(v[0])) goto fail;
    if ((v[1] == 0) && (v[2] == 0)) goto fail;
    if ((v[6] == 0) && (v[7] == 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* bertrand_match3() */

/* ==================================== */
int32_t bertrand_match3b(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
// A 2 B   avec origine = (1,0) et (au moins un des A et au moins un des B non nuls ou tous les A,B nuls)
// A 2 B   (+ rotation 90)
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (!IS_SIMPLE(v[0])) goto fail;
    if ((v[1] == 0) && (v[2] == 0) && (v[6] == 0) && (v[7] == 0)) return 1;
    if ((v[1] == 0) && (v[2] == 0)) goto fail;
    if ((v[6] == 0) && (v[7] == 0)) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* bertrand_match3b() */

/* ==================================== */
int32_t bertrand_match4(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
// 2 A   
// B C   (+ rotation 90)
// avec origine = (0,0) et 
//      C = 2 ou A = B = 2 et
//      pour tout x dans {A,B,C} si x > 0 alors x = 2
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if ((!IS_SIMPLE(v[7])) && ((!IS_SIMPLE(v[0])) || (!IS_SIMPLE(v[6])))) goto fail;
    if ((v[0] > 0) && (!IS_SIMPLE(v[0]))) goto fail;
    if ((v[6] > 0) && (!IS_SIMPLE(v[6]))) goto fail;
    if ((v[7] > 0) && (!IS_SIMPLE(v[7]))) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* bertrand_match4() */

/* ==================================== */
int32_t lskelbertrand_sym(struct xvimage *image,
	      int32_t nsteps,
	      struct xvimage *inhibit)
/* ==================================== */
/*
Algo CPSR1 données: S
T := S
Répéter jusqu'à stabilité
     T := T \ominus \Gamma_4
     P := pixels simples pour S et 4-voisins d'un point de T
     R := pixels de P qui s'apparient avec C2
     S :=  [S  \  P]  \cup  R
*/
#undef F_NAME
#define F_NAME "lskelbertrand_sym"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image);
  uint8_t *T = UCHARDATA(t);
  struct xvimage *r = copyimage(image);
  uint8_t *R = UCHARDATA(r);
  int32_t step, nonstab;
  int32_t m1;
#ifdef ETUDE
  struct xvimage *tmp2 = copyimage(image);
  uint8_t *T2 = UCHARDATA(tmp2);
  memset(T2, 0, N);
#endif

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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

    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (T[i] && (nbvois4(T, i, rs, N) == 4)) R[i] = 1; // calcule R = eros (T)
    for (i = 0; i < N; i++) T[i] = R[i];                 // T = eros (T)
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N)  && (nbvois4(T, i, rs, N) > 0))
	F[i] = 2; //  pixels simples pour F et 4-voisins d'un point de T
    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (F[i] == 2)
      {
	m1 = bertrand_match3(F, i, rs, N);
	if (m1)
	{
	  R[i] = 1; // preserve point
	}
      }

    for (i = 0; i < N; i++)
      if ((F[i] == 2) && !R[i])
      {
	F[i] = 0;
	nonstab = 1;
#ifdef ETUDE
        T2[i] = step;
#endif
      }
    for (i = 0; i < N; i++) if (F[i]) F[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

#ifdef ETUDE
  writeimage(tmp2,"_etude");
#endif

  freeimage(t);
  freeimage(r);
  return(1);
} /* lskelbertrand_sym() */

/* ==================================== */
int32_t lskelbertrand_asym_s(struct xvimage *image,
			int32_t nsteps,
			struct xvimage *inhibit)
/* ==================================== */
// described in [G. Bertrand - personnal communication - 26/10/2005]
// variante algo NK^2 (comme BK^2)
#undef F_NAME
#define F_NAME "lskelbertrand_asym_s"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *X = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  uint8_t *M = (uint8_t *)malloc(N);
  uint8_t *E = (uint8_t *)malloc(N);
  uint8_t *R = (uint8_t *)malloc(N);
  int32_t step, nonstab;
  int32_t m1, m2;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (X[i]) X[i] = 1; // normalize values

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

    memset(E, 0, N);   // T = eros (T)
    for (i = 0; i < N; i++)
      if (T[i] && (nbvois4(T, i, rs, N) == 4)) E[i] = 1;
    memcpy(T, E, N);

    memset(M, 0, N);
    for (i = 0; i < N; i++)
      if (nbvois4(T, i, rs, N) >= 1) M[i] = 1;           // calcule M = Dilat(T)
    for (i = 0; i < N; i++)
      if ((X[i] == 1) && simple8(X, i, rs, N) && M[i])
	X[i] = 2;

    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (X[i] == 2)
      {
	m1 = bertrand_match1(X, i, rs, N);
	m2 = bertrand_match2(X, i, rs, N);
	if (m1 || m2)
	{
	  R[i] = 1; // preserve point
	}
      }

    for (i = 0; i < N; i++)
      if ((X[i] == 2) && !R[i]) { X[i] = 0; nonstab = 1; }
    for (i = 0; i < N; i++) if (X[i]) X[i] = 1;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (X[i]) X[i] = 255; // normalize values

  freeimage(tmp);
  free(M);
  free(E);
  free(R);
  return(1);
} /* lskelbertrand_asym_s() */

/* ==================================== */
int32_t lskelMK2(struct xvimage *image,
	     int32_t nsteps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Algo MK2 données: S, I
Répéter jusqu'à stabilité
  P := pixels simples pour S
  R := pixels de P qui s'apparient avec le masque C (bertrand_match3)
  T :=  [S  \  P] \cup  R \cup  I
  S := T \cup [S \ (T \oplus \Gamma_8*)]
*/
#undef F_NAME
#define F_NAME "lskelMK2"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  uint8_t *I;                      /* l'image d'inhibition */
  struct xvimage *t = copyimage(image);
  uint8_t *T = UCHARDATA(t);
  struct xvimage *r = copyimage(image);
  uint8_t *R = UCHARDATA(r);
  int32_t step, nonstab;
  int32_t m1;

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs))
    {
      fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
      return 0;
    }
    if (datatype(inhibit) != VFF_TYP_1_BYTE)
    {
      fprintf(stderr, "%s: incompatible image types\n", F_NAME);
      return 0;
    }
    I = UCHARDATA(inhibit);
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = 1; // normalize values

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
      if ((S[i] == 1) && simple8(S, i, rs, N))
	S[i] = 2; //  pixels simples pour S
    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (S[i] == 2)
      {
	m1 = bertrand_match3(S, i, rs, N);
	if (m1) R[i] = 1; // preserve point
      }
    memset(T, 0, N);
    if (inhibit != NULL)
    {
      for (i = 0; i < N; i++) // T := [S \ P] \cup  R \cup I
	if ((S[i] == 1) || R[i] || I[i])
	  T[i] = 1;
    }
    else
    {
      for (i = 0; i < N; i++) // T := [S \ P] \cup  R
	if ((S[i] == 1) || R[i])
	  T[i] = 1;
    }
    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (nbvois8(T, i, rs, N) >= 1) R[i] = 1; // calcule R = Dilat(T)
    for (i = 0; i < N; i++)
      if (T[i] || (S[i] && !R[i])) T[i] = 1; else T[i] = 0; // T := T \cup [S \ R]

    for (i = 0; i < N; i++)
      if (S[i] && !T[i])
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
  return(1);
} /* lskelMK2() */

/* ==================================== */
int32_t lskelMK2b(struct xvimage *image,
	     int32_t nsteps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Algo MK2b données: S, I
Répéter jusqu'à stabilité
  P := pixels simples pour S et pas dans I
  R := pixels de P qui vérifient bertrand_match3b
  P := P \ R
  R := pixels de P qui vérifient bertrand_match4  
  S = S \ [P \ R]
(l'ensemble P est représenté par la valeur 2 dans l'image S)
*/
#undef F_NAME
#define F_NAME "lskelMK2b"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  uint8_t *I;                      /* l'image d'inhibition */
  struct xvimage *r = copyimage(image);
  uint8_t *R = UCHARDATA(r);
  int32_t step, nonstab;
  int32_t m1;

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs))
    {
      fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
      return 0;
    }
    if (datatype(inhibit) != VFF_TYP_1_BYTE)
    {
      fprintf(stderr, "%s: incompatible image types\n", F_NAME);
      return 0;
    }
    I = UCHARDATA(inhibit);
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = 1; // normalize values

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
      if ((S[i] == 1) && simple8(S, i, rs, N) && 
	  ((inhibit == NULL) || (I[i] == 0)))
	S[i] = 2; //  pixels simples pour S et pas de contrainte

    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (S[i] == 2)
      {
	m1 = bertrand_match3b(S, i, rs, N);
	if (m1) R[i] = 1; // preserve point
      }
    for (i = 0; i < N; i++) if (R[i]) S[i] = 1;


    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (S[i] == 2)
      {
	m1 = bertrand_match4(S, i, rs, N);
	if (m1) R[i] = 1; // preserve point
      }

    for (i = 0; i < N; i++) 
      if ((S[i] == 2) && (R[i] == 0))
      {
	S[i] = 0;
	nonstab = 1;
      }
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (S[i]) S[i] = 255; // normalize values

  freeimage(r);
  return(1);
} /* lskelMK2b() */

/* ==================================== */
int32_t lskelAK2(struct xvimage *image,
	     int32_t nsteps,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Amincissement symétrique avec inclusion de l'axe médian
Algo AK2 données: S
K := \emptyset ; T := S
Répéter jusqu'à stabilité
  E := T \ominus \Gamma_4
  D := T \ [E \oplus \Gamma_4]
  T := E
  K := K \cup D
  P := pixels de S \ K simples pour S
  R := pixels de P qui s'apparient avec le masque C (bertrand_match3)
  S := [S  \  P]  \cup  R
*/
#undef F_NAME
#define F_NAME "lskelAK2"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *S = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *t = copyimage(image);
  uint8_t *T = UCHARDATA(t);
  struct xvimage *r = copyimage(image);
  uint8_t *R = UCHARDATA(r);
  struct xvimage *e = copyimage(image);
  uint8_t *E = UCHARDATA(e);
  struct xvimage *d = copyimage(image);
  uint8_t *D = UCHARDATA(d);
  struct xvimage *k = copyimage(image);
  uint8_t *K = UCHARDATA(k);

  int32_t step, nonstab;
  int32_t m1;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (S[i]) S[i] = 1; // normalize values

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  // K := \emptyset ; T := S
  memset(K, 0, N);
  memcpy(T, S, N);
  while (nonstab && (step < nsteps))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    //  E := T \ominus \Gamma_4
    memset(E, 0, N);
    for (i = 0; i < N; i++)
      if (T[i] && (nbvois4(T, i, rs, N) == 4)) E[i] = 1;

    //  D := E \oplus \Gamma_4
    memset(D, 0, N);
    for (i = 0; i < N; i++)
      if (E[i] || nbvois4(E, i, rs, N) >= 1) D[i] = 1;

    //  D := T \ D
    for (i = 0; i < N; i++)
      if (T[i] && !D[i]) D[i] = 1; else D[i] = 0;

    //  T := E
    memcpy(T, E, N);

    //  K := K \cup D
    for (i = 0; i < N; i++)
      if (D[i]) K[i] = 1;

    //  P := pixels de S \ K simples pour S (P est représenté par les points à 2 dans S)
    for (i = 0; i < N; i++)
      if (S[i] && !K[i] && simple8(S, i, rs, N))
	S[i] = 2;

    //  R := pixels de P qui s'apparient avec le masque C (bertrand_match3)
    memset(R, 0, N);
    for (i = 0; i < N; i++)
      if (S[i] == 2)
      {
	m1 = bertrand_match3(S, i, rs, N);
	if (m1) R[i] = 1; // preserve point
      }

    //  D := [S  \  P]  \cup  R
    for (i = 0; i < N; i++)
      if ((S[i] == 1) || R[i])
	D[i] = 1;

    for (i = 0; i < N; i++) // pour  tester la stabilité
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
  return(1);
} /* lskelAK2() */

/* ==================================== */
static int32_t ros_match1(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  D D D     avec origine = (1,1) et valeurs des pixels D quelconques
  D P 0
  D 0 1
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[0] != 0) goto fail;
    if (v[6] != 0) goto fail;
    if (!v[7]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* ros_match1() */

/* ==================================== */
static int32_t ros_match2(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  D 0 0     avec origine = (1,1)
  D P 1
  D 0 0
 */
{
  int32_t i;
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  for (i = 0; i < 4; i++)
  {
    if (v[1] != 0) goto fail;
    if (v[2] != 0) goto fail;
    if (v[6] != 0) goto fail;
    if (v[7] != 0) goto fail;
    if (!v[0]) goto fail;
    return 1;
  fail:
    rotate90_vois(v);
  }
  return 0;
} /* ros_match2() */

/* ==================================== */
static int32_t ros_north(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  D 0 D     avec origine = (1,1) et valeurs des pixels D quelconques
  D P D
  D D D
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (v[2] != 0) return 0;
  return 1;

} /* ros_north() */

/* ==================================== */
static int32_t ros_south(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  D D D     avec origine = (1,1) et valeurs des pixels D quelconques
  D P D
  D 0 D
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (v[6] != 0) return 0;
  return 1;

} /* ros_south() */

/* ==================================== */
static int32_t ros_east(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  D D D     avec origine = (1,1) et valeurs des pixels D quelconques
  D P 0
  D D D
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (v[0] != 0) return 0;
  return 1;

} /* ros_east() */

/* ==================================== */
static int32_t ros_west(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  D D D     avec origine = (1,1) et valeurs des pixels D quelconques
  0 P D
  D D D
 */
{
  uint8_t v[8];
  extract_vois(F, x, rs, N, v);
  if (v[4] != 0) return 0;
  return 1;

} /* ros_west() */

/* ==================================== */
int32_t lskelrosenfeld(struct xvimage *image,
		       int32_t nsteps,
		       struct xvimage *inhibit)
/* ==================================== */
// Rosenfeld : algo directionnel (ordre NSEW)
#undef F_NAME
#define F_NAME "lskelrosenfeld"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_north(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_south(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_east(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_west(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelrosenfeld() */

/* ==================================== */
int32_t lskelrosenfeld_var1(struct xvimage *image,
		       int32_t nsteps,
		       struct xvimage *inhibit)
/* ==================================== */
// Rosenfeld : algo directionnel (variante : ordre WESN)
#undef F_NAME
#define F_NAME "lskelrosenfeld_var1"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_west(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_east(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_south(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_north(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelrosenfeld_var1() */

/* ==================================== */
int32_t lskelrosenfeld_var2(struct xvimage *image,
		       int32_t nsteps,
		       struct xvimage *inhibit)
/* ==================================== */
// Rosenfeld : algo directionnel (variante : ordre ENWS)
#undef F_NAME
#define F_NAME "lskelrosenfeld_var2"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = 1; // normalize values

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

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_east(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_north(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_west(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 1) && simple8(F, i, rs, N) && ros_south(F, i, rs, N) && !ros_match1(F, i, rs, N) && !ros_match2(F, i, rs, N))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;

  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = 255; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelrosenfeld_var2() */


//Detects endpoints of type 1, 2, 3 as defined in "Parallel connectivity preserving thinning algorithms", by Hall
int32_t is_hall_2dendpoint(struct xvimage* img, uint32_t x, uint32_t rs, uint32_t N, uint32_t type)
{
	uint32_t v, i, v1, v2;

	if(type <= 0 || type > 3)
	{
		fprintf(stderr, "hall_2dendpoint(): given type is not valid");
		return(0);
	}

	if(UCHARDATA(img)[x]==0) return 0;

	v = nbvois8((uint8_t*)(img->image_data), x, rs, N);

	if(v == 1)
	{
		return(1);
	}

	else if(v == 2)
	{
		if(type==1)
			return 0;

		else
		{
			for(i=0; i<8; i++)
			{
				v1 = voisin(x, i, rs, N);
				if((v1!=-1) && (UCHARDATA(img)[v1]!=0))
				{
					break;
				}
			}

			for(i=0; i<8; i++)
			{
				v2 = voisin(x, 7-i, rs, N);
				if((v2!=-1) && (UCHARDATA(img)[v2]!=0))
				{
					break;
				}
			}

			if(v1==v2) fprintf(stderr, "Bouh\n");

			if(type==2)
			{
				if(voisins8(v1, v2, rs))
					return 1;
				else
					return 0;
			}

			else
			{
				if(voisins4(v1, v2, rs))
					return 1;
				else
					return 0;
			}
		}
	}

	return 0;
}



//Detects i-self-deletable points as defined in "Parallel thinning algorithms based on Ronse's sufficient conditions for topology preservation", by Nemeth and Palagyi
int32_t is_self_deletable(struct xvimage *img, uint32_t x, uint32_t rs, uint32_t N, uint32_t type)
{
	if(type <= 0 || type > 3)
	{
		fprintf(stderr, "is_self_deletable(): given type is not valid");
		return(0);
	}

	if(UCHARDATA(img)[x]==0) return 0;

	return (simple8((uint8_t*)(img->image_data), x, rs, N) && (!is_hall_2dendpoint(img, x, rs, N, type)));
}


//Detects i-double-deletable points as defined in "Parallel thinning algorithms based on Ronse's sufficient conditions for topology preservation", by Nemeth and Palagyi
int32_t is_double_deletable(struct xvimage *img, uint32_t x, uint32_t rs, uint32_t N, uint32_t type)
{
	uint32_t i, v, ok1, ok2;

	if(type <= 0 || type > 3)
	{
		fprintf(stderr, "is_double_deletable(): given type is not valid");
		return(0);
	}

	if(UCHARDATA(img)[x]==0) return 0;

	if(is_self_deletable(img, x, rs, N, type))
	{
		for(i=0; i<8; i=i+2)
		{
			v = voisin(x, i, rs, N);
			if(v!=-1)
			{
				if((UCHARDATA(img)[v] != 0) && is_self_deletable(img, v, rs, N, type))
				{
					UCHARDATA(img)[v]=0;
					ok1=simple8((uint8_t*)(img->image_data), x, rs, N);
					UCHARDATA(img)[v]=255;

					if(!ok1)
					{
						UCHARDATA(img)[x]=0;
						ok2=simple8((uint8_t*)(img->image_data), v, rs, N);
						UCHARDATA(img)[x]=255;

						if(!ok2) return 0;
					}
				}
			}
		}
		return 1;
	}

	return 0;
}



//Detects if point x is surrounded by a ring of black points of radius 2 (necessary for i-square deletable points)
// 0 0 0 0
// 0 x * 0
// 0 * * 0
// 0 0 0 0
int32_t is_surrounded_by_radius2_ring(struct xvimage *img, uint32_t x, uint32_t rs, uint32_t N)
{
	if(nonbord(x, rs, N) && nonbord(x+rs, rs, N) && nonbord(x+1, rs, N) && nonbord(x+rs+1, rs, N))
	{
		if( (UCHARDATA(img)[x-1-rs] == 0) &&
			(UCHARDATA(img)[x-rs] == 0) &&
			(UCHARDATA(img)[x+1-rs] == 0) &&
			(UCHARDATA(img)[x+2-rs] == 0) &&
			(UCHARDATA(img)[x-1] == 0) &&
			(UCHARDATA(img)[x+2] == 0) &&
			(UCHARDATA(img)[x-1+rs] == 0) &&
			(UCHARDATA(img)[x+2+rs] == 0) &&
			(UCHARDATA(img)[x-1+2*rs] == 0) &&
			(UCHARDATA(img)[x+2*rs] == 0) &&
			(UCHARDATA(img)[x+1+2*rs] == 0) &&
			(UCHARDATA(img)[x+2+2*rs] == 0) )
		{
			return 1;
		}
	}

	return (0);
}

//Detects i-square-deletable points as defined in "Parallel thinning algorithms based on Ronse's sufficient conditions for topology preservation", by Nemeth and Palagyi
int32_t is_square_deletable(struct xvimage *img, uint32_t x, uint32_t rs, uint32_t N, uint32_t type)
{
	if(type <= 0 || type > 3)
	{
		fprintf(stderr, "is_square_deletable(): given type is not valid");
		return(0);
	}

	if(UCHARDATA(img)[x]==0) return 0;

	if(nonbord(x, rs, N))
	{
		if( is_surrounded_by_radius2_ring(img, x, rs, N) )
		{
			//Test configuration e
			if( (UCHARDATA(img)[x+1]==255) && (UCHARDATA(img)[x+rs+1]==255) && (UCHARDATA(img)[x+rs]==255) )
			{
				return 0;
			}

			//Stop here if type is 2 or 3
			if(type!=1)
			{
				return 1;
			}

			//Test configuration a
			if( (UCHARDATA(img)[x+1]==0) && (UCHARDATA(img)[x+rs+1]==255) && (UCHARDATA(img)[x+rs]==255) )
			{
				return 0;
			}

			//Test configuration b
			if( (UCHARDATA(img)[x+1]==255) && (UCHARDATA(img)[x+rs+1]==255) && (UCHARDATA(img)[x+rs]==0) )
			{
				return 0;
			}

			//Test configuration c
			if( (UCHARDATA(img)[x+1]==255) && (UCHARDATA(img)[x+rs+1]==0) && (UCHARDATA(img)[x+rs]==255) )
			{
				return 0;
			}
		}

		if(type==1)
		{
			//test configuration d
			if( is_surrounded_by_radius2_ring(img, x-1, rs, N) &&
				(UCHARDATA(img)[x-1]==0) &&
				(UCHARDATA(img)[x+rs]==255) &&
				(UCHARDATA(img)[x-1+rs]==255) )
			{
				return 0;
			}
		}

		return 1;
	}

	return 0;
}

/* ==================================== */
int32_t lskelnemethpalagyi(struct xvimage *image,
	     int32_t nsteps,
	     struct xvimage *inhibit,
	     int32_t type)
/* ==================================== */
/*
Voir "Parallel thinning algorithms based on Ronse's sufficient conditions for topology preservation", by Nemeth and Palagyi
*/
#undef F_NAME
#define F_NAME "lskelnemethpalagyi"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;

  if (inhibit != NULL)
  {
    fprintf(stderr, "%s: inhibit image: not implemented\n", F_NAME);
    return 0;
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  step = 0;
  nonstab = 1;
  while (nonstab &&  ((nsteps==-1) || (step < nsteps)))
  {
    nonstab = 0;
    step++;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    memset(T, 0, N);
    for (i = 0; i < N; i++)
      if ((F[i] == 255) && (is_double_deletable(image, i, rs, N, type)==1) && (is_square_deletable(image, i, rs, N, type)==1))
      { nonstab = 1; T[i] = 1; }
    for (i = 0; i < N; i++) if (T[i] == 1) F[i] = 0;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif


  freeimage(tmp);
  return(1);
} /* lskelnemethpalagyi() */



// ====================================================
// ====================================================
// squelettes en niveaux de gris - Nivando Bezerra
// ====================================================
// ====================================================

#define  E(_p,_rs) (_p+1)      
#define NE(_p,_rs) (_p+1-_rs)   
#define  S(_p,_rs) (_p+_rs)     
#define SW(_p,_rs) (_p+_rs-1)   
#define  W(_p,_rs) (_p-1)      
#define NW(_p,_rs) (_p-1-_rs)   
#define  N(_p,_rs) (_p-_rs)     
#define SE(_p,_rs) (_p+_rs+1)   
       	              
#define NN(_p,_rs) (N(_p,_rs) - _rs)  
#define SS(_p,_rs) (S(_p,_rs) + _rs)   
#define WW(_p,_rs) (W(_p,_rs) - 1)    
#define EE(_p,_rs) (E(_p,_rs) + 1)    
       	              
#define NEE(_p,_rs) (EE(_p,_rs) - _rs) 
#define NWW(_p,_rs) (WW(_p,_rs) - _rs) 
#define SEE(_p,_rs) (EE(_p,_rs) + _rs) 
#define SWW(_p,_rs) (WW(_p,_rs) + _rs) 
#define NNE(_p,_rs) (NN(_p,_rs) + 1)  
#define NNW(_p,_rs) (NN(_p,_rs) - 1)  
#define SSE(_p,_rs) (SS(_p,_rs) + 1)  
#define SSW(_p,_rs) (SS(_p,_rs) - 1)

#define _nnw55         1
#define _nn55 	       2
#define _nne55	       3
#define _nww55	       5
#define _nw55 	       6
#define _n55  	       7
#define _ne55 	       8
#define _nee55	       9
#define	_ww55 	      10
#define _w55  	      11
#define _center55     12
#define _e55  	      13
#define _ee55 	      14
#define	_sww55	      15
#define _sw55 	      16
#define _s55  	      17
#define _se55	      18
#define _see55	      19
#define _ssw55	      21
#define _ss55 	      22
#define _sse55        23

#define IN_SET 1
#define NB_SIMPLE 2
#define NON_DESTRUCTIBLE 0
#define DESTRUCTIBLE 1
#define DESTRUCTIBLE_PAR 2
#define CRUCIAL 3
#define CRUCIAL_C 9
#define CRUCIAL_C1 5
#define TRUE 1
#define FALSE 0

#define DEBUG

/* C1 */
/* |---+---| */
/* | 0 | P | */
/* |---+---| */
/* | P | 0 | */
/* |---+---| */

/* C2 */
/* |---+---| */
/* | 0 | 0 | */
/* |---+---| */
/* | P | P | */
/* |---+---| */
/* | 0 | 0 | */
/* |---+---| */

/* C3 */
/* |---+---+---| */
/* |   | 0 | 0 | */
/* |---+---+---| */
/* | 0 | P | P | */
/* |---+---+---| */
/* | 0 | P | 0 | */
/* |---+---+---| */

/* C4 */
/* |---+---+---+---| */
/* |   | 0 | 0 |   | */
/* |---+---+---+---| */
/* | 0 | P | P | 0 | */
/* |---+---+---+---| */
/* | 0 | P | P | 0 | */
/* |---+---+---+---| */
/* |   | 0 | 0 |   | */
/* |---+---+---+---| */

/*
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
*/

// D: set of destructible points
// verify p and ne match in mask C1
#define MATCH_C1(D, p, ne, n, e) (D[p] && D[ne] && (!D[n]) && (!D[e]))

// D: set of destructible points
// verify p and e match in mask C2
#define MATCH_C2(D, p, n, e, s, ne, se) (D[p] && D[e] && (!D[s]) && (!D[n]) && (!D[ne]) && (!D[se]))

// D: set of destructible points
// verify p  match in mask C3
#define MATCH_C3(D, p, n, e, s, w, ne, se, sw) \
  (D[p] && D[e] && D[s] && !D[n] && !D[ne] && !D[se] && !D[sw] && !D[w])

// D: set of destructible points
// verify p  match in mask C4
#define MATCH_C4(D, p, n, e, s, w, ne, se, sw, ee, see, sse, ss) \
  (D[p] && D[e] && D[s] && D[se] && !D[n] && !D[ne] && !D[ee] && !D[see] && !D[sse] && !D[ss] && !D[sw] && !D[w])

#define MATCH_C(D, p, n, e, s, ne, se) (D[p] && D[e] && (D[n]||D[ne]) && (D[s] || D[se]))
#define MATCH_C(D, p, n, e, s, ne, se) (D[p] && D[e] && (D[n]||D[ne]) && (D[s] || D[se]))

void getNeighborhood(uint8_t *F, uint32_t rs, uint32_t N, uint32_t p, uint8_t k, uint8_t * neighborhood);
void printNeighborhood(uint8_t *neighborhood);
void match_C1(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);
void match_C2(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);
void match_C3(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);
void match_C4(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);
void match_C(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);

void match_C1_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);
void match_C2_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);
void match_C3_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);
void match_C4_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);
void match_C_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N);

/* |--------+--------+-------+--------+--------| */
/* |        | 1  nnw | 2  nn | 3  nne |        | */
/* |--------+--------+-------+--------+--------| */
/* | 5  nww | 6   nw | 7   n | 8   ne | 9  nee | */
/* |--------+--------+-------+--------+--------| */
/* | 10  ww | 11   w | 12  P | 13   e | 14  ee | */
/* |--------+--------+-------+--------+--------| */
/* | 15 sww | 16  sw | 17  s | 18  se | 19 see | */
/* |--------+--------+-------+--------+--------| */
/* |        | 21 sww | 22 ss | 23 sse |        | */
/* |--------+--------+-------+--------+--------| */

void getNeighborhood(uint8_t *F, uint32_t rs, uint32_t N, uint32_t p, uint8_t k, uint8_t * neighborhood)
/* neighborhood is a 25 (5x5) elements array */
{
  neighborhood[ 1] = F[NNW(p,rs)]>=k ? IN_SET : 0;  //nnw
  neighborhood[ 2] = F[NN(p,rs) ]>=k ? IN_SET : 0;  //nn 
  neighborhood[ 3] = F[NNE(p,rs)]>=k ? IN_SET : 0;  //nne
  neighborhood[ 5] = F[NWW(p,rs)]>=k ? IN_SET : 0;  //nww
  neighborhood[ 6] = F[NW(p,rs) ]>=k ? IN_SET : 0;  //nw 
  neighborhood[ 7] = F[N(p,rs)  ]>=k ? IN_SET : 0;  //n  
  neighborhood[ 8] = F[NE(p,rs) ]>=k ? IN_SET : 0;  //ne 
  neighborhood[ 9] = F[NEE(p,rs)]>=k ? IN_SET : 0;  //nee
  neighborhood[10] = F[WW(p,rs) ]>=k ? IN_SET : 0;  //ww 
  neighborhood[11] = F[W(p,rs)  ]>=k ? IN_SET : 0;  //w  
  neighborhood[12] = F[p]        >=k ? IN_SET : 0;  //center
  neighborhood[13] = F[E(p,rs)  ]>=k ? IN_SET : 0;  //e  
  neighborhood[14] = F[EE(p,rs) ]>=k ? IN_SET : 0;  //ee 
  neighborhood[15] = F[SWW(p,rs)]>=k ? IN_SET : 0;  //sww
  neighborhood[16] = F[SW(p,rs) ]>=k ? IN_SET : 0;  //sw 
  neighborhood[17] = F[S(p,rs)  ]>=k ? IN_SET : 0;  //s  
  neighborhood[18] = F[SE(p,rs) ]>=k ? IN_SET : 0;  //se 
  neighborhood[19] = F[SEE(p,rs)]>=k ? IN_SET : 0;  //see
  neighborhood[21] = F[SSW(p,rs)]>=k ? IN_SET : 0;  //ssw
  neighborhood[22] = F[SS(p,rs) ]>=k ? IN_SET : 0;  //ss 
  neighborhood[23] = F[SSE(p,rs)]>=k ? IN_SET : 0;  //sse

  neighborhood[0] = neighborhood[4] = neighborhood[20] = neighborhood[24] = 0; // not used
}


void printNeighborhood(uint8_t *neighborhood)
{
  int l, c; 
  int32_t p = 0;
  for (l = 0; l<5; l++)
    {
      for (c = 0; c<5; c++)
      {
	printf("%d ", neighborhood[p]);
	p++;
      }
      printf("\n");
    }
}

int32_t lhthinpar(struct xvimage *image, int32_t nsteps)
{
#undef F_NAME
#define F_NAME "lhthinpar"
  int32_t p;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab;
  // Windows(tm) the declaration must go on the beginning of the block
  struct xvimage *destructible;
  uint8_t *D;
  struct xvimage *alpha;
  uint8_t *A;
  struct xvimage *tempimage;
  uint8_t *G;

  destructible = copyimage(image);
  if (destructible == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return 0;
  }
  D = UCHARDATA(destructible);       

  alpha = copyimage(image);
  if (alpha == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return 0;
  }
  A = UCHARDATA(alpha);       
  
  tempimage = copyimage(image);
  if (tempimage == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return 0;
  }
  G = UCHARDATA(tempimage);       

  if (nsteps == -1) nsteps = 1000000000;
  
  step = 0;
  
  do {
    nonstab = 0;

    // compute D and A
     for (p=0; p<N; p++) {
      A[p] =  alpha8m(F, p, rs, N);
      if ( pdestr4(F, p, rs, N)) 
	D[p] = DESTRUCTIBLE;
      else 
	D[p] = NON_DESTRUCTIBLE;
    }

#ifdef DEBUG
     writeimage(destructible, "DESTR.pgm");
     writeimage(alpha, "ALPHA.pgm");
#endif


     // find crucial points
     match_C(F, D, A, rs, N);
     match_C1(F, D, A, rs, N);
     match_C2(F, D, A, rs, N);
     match_C3(F, D, A, rs, N);
     match_C4(F, D, A, rs, N);
    
#ifdef DEBUG
     writeimage(destructible, "CRUCIAL.pgm");
#endif

    for (p=0; p<N; p++)
      if (D[p]==DESTRUCTIBLE) {
	G[p] = A[p];
	nonstab = 1;
      }
      else
	G[p] = F[p];
    
    step++;
    memcpy(F, G, N);
     
  } while (nonstab && step<nsteps);

  return 1;
} 


int32_t lhthinpar_asymmetric(struct xvimage *image, int32_t nsteps)
{
#undef F_NAME
#define F_NAME "lhthinpar_asymmetric"
  int32_t p;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t step, nonstab;
  struct xvimage *destructible;
  uint8_t *D;
  struct xvimage *alpha;
  uint8_t *A;
  struct xvimage *tempimage;
  uint8_t *G;

  destructible = copyimage(image);
  if (destructible == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return 0;
  }
  D = UCHARDATA(destructible);       

  alpha = copyimage(image);
  if (alpha == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return 0;
  }
  A = UCHARDATA(alpha);       

  tempimage = copyimage(image);
  if (tempimage == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return 0;
  }
  G = UCHARDATA(tempimage);       

  if (nsteps == -1) nsteps = 1000000000;

  step = 0;
  
  do {
    nonstab = 0;

    // compute D and A
     for (p=0; p<N; p++) {
      A[p] =  alpha8m(F, p, rs, N);
      if ( pdestr4(F, p, rs, N)) 
	D[p] = DESTRUCTIBLE;
      else 
	D[p] = NON_DESTRUCTIBLE;
    }

#ifdef DEBUG
     writeimage(destructible, "DESTR.pgm");
     writeimage(alpha, "ALPHA.pgm");
#endif


     // find crucial points
     match_C_asymmetric(F, D, A, rs, N);
     match_C1_asymmetric(F, D, A, rs, N);
     match_C2_asymmetric(F, D, A, rs, N);
     match_C3_asymmetric(F, D, A, rs, N);
     match_C4_asymmetric(F, D, A, rs, N);
    
#ifdef DEBUG
     writeimage(destructible, "CRUCIAL.pgm");
#endif

    for (p=0; p<N; p++)
      if (D[p]==DESTRUCTIBLE) {
	G[p] = A[p];
	nonstab = 1;
      }
      else
	G[p] = F[p];
    
    step++;
    memcpy(F, G, N);
     
  } while (nonstab && step<nsteps);

  return 1;
} 

void match_C1(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);	  

      // MASK C1
      if ( D[NE(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C1(Fk, _center55, _ne55, _n55, _e55) &&
	   A[NE(p,rs)]<k ) {
	D[p] = D[NE(p,rs)] = CRUCIAL;
      }
      if ( D[SE(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C1(Fk, _center55, _se55, _s55, _e55) &&
	   A[SE(p,rs)]<k ) {
	D[p] = D[SE(p,rs)] = CRUCIAL;         
      }
      if ( D[NW(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C1(Fk, _center55, _nw55, _n55, _w55) &&
	   A[NW(p,rs)]<k ) { 
	D[p] = D[NW(p,rs)] = CRUCIAL; 
      }
      if ( D[SW(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C1(Fk, _center55, _sw55, _s55, _w55) && 
	   A[SW(p,rs)]<k ) { 
	D[p] = D[SW(p,rs)] = CRUCIAL; 
      }
    }
  }
}


void match_C1_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);	  

      // MASK C1
      if ( D[NE(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C1(Fk, _center55, _ne55, _n55, _e55) &&
	   A[NE(p,rs)]<k ) {
	D[p] = CRUCIAL;
      }
      if ( D[SE(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C1(Fk, _center55, _se55, _s55, _e55) &&
	   A[SE(p,rs)]<k ) {
	D[p] = CRUCIAL;         
      }
      if ( D[NW(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C1(Fk, _center55, _nw55, _n55, _w55) &&
	   A[NW(p,rs)]<k ) { 
	D[p] = CRUCIAL; 
      }
      if ( D[SW(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C1(Fk, _center55, _sw55, _s55, _w55) && 
	   A[SW(p,rs)]<k ) { 
	D[p] = CRUCIAL; 
      }
    }
  }
}

void match_C2(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);	  

      if ( D[E(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C2(Fk, _center55, _n55, _e55, _s55, _ne55, _se55) &&
	   A[E(p,rs)]<k) {
	D[p] = D[E(p,rs)] = CRUCIAL;
      }
      if ( D[S(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C2(Fk, _center55, _e55, _s55, _w55, _se55, _sw55) &&
	   A[S(p,rs)]<k ) {
	D[p] = D[S(p,rs)] = CRUCIAL;
      }
      if ( D[W(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C2(Fk, _center55, _s55, _w55, _n55, _sw55, _nw55) &&
	   A[W(p,rs)]<k) {
	D[p] = D[W(p,rs)] = CRUCIAL;
      }
      if ( D[N(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C2(Fk, _center55, _w55, _n55, _e55, _nw55, _ne55) &&
	   A[N(p,rs)]<k ) {
	D[p] = D[N(p,rs)] = CRUCIAL;
      }
    }
  }
}


void match_C2_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);	  

      if ( D[E(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C2(Fk, _center55, _n55, _e55, _s55, _ne55, _se55) &&
	   A[E(p,rs)]<k) {
	D[p] = CRUCIAL;
      }
      if ( D[S(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C2(Fk, _center55, _e55, _s55, _w55, _se55, _sw55) &&
	   A[S(p,rs)]<k ) {
	D[p] = CRUCIAL;
      }
      if ( D[W(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C2(Fk, _center55, _s55, _w55, _n55, _sw55, _nw55) &&
	   A[W(p,rs)]<k) {
	D[p] = CRUCIAL;
      }
      if ( D[N(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C2(Fk, _center55, _w55, _n55, _e55, _nw55, _ne55) &&
	   A[N(p,rs)]<k ) {
	D[p] = CRUCIAL;
      }
    }
  }
}

void match_C3(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);	  

      if ( D[E(p,rs)]==DESTRUCTIBLE && D[S(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C3(Fk, _center55, _n55, _e55, _s55, _w55, _ne55, _se55, _sw55) &&
	   A[E(p,rs)]<k && A[S(p,rs)]<k) {
	D[p] = D[E(p,rs)] = D[S(p,rs)] = CRUCIAL;
      }
      if ( D[S(p,rs)]==DESTRUCTIBLE && D[W(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C3(Fk, _center55, _e55, _s55, _w55, _n55, _se55, _sw55, _nw55) &&
	   A[S(p,rs)]<k && A[W(p,rs)]<k ) {
	D[p] = D[S(p,rs)] = D[W(p,rs)] = CRUCIAL;
      }
      if ( D[W(p,rs)]==DESTRUCTIBLE && D[N(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C3(Fk, _center55, _s55, _w55, _n55, _e55, _sw55, _nw55, _ne55) &&
	   A[W(p,rs)]<k && A[N(p,rs)]<k ) {
	D[p] = D[W(p,rs)] = D[N(p,rs)] = CRUCIAL;
      }
      if ( D[N(p,rs)]==DESTRUCTIBLE && D[E(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C3(Fk, _center55, _w55, _n55, _e55, _s55, _nw55, _ne55, _se55) &&
	  A[N(p,rs)]<k && A[E(p,rs)]<k) {
	D[p] = D[N(p,rs)] = D[E(p,rs)] = CRUCIAL;
      }
    }
  }
}


void match_C3_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);	  

      if ( D[E(p,rs)]==DESTRUCTIBLE && D[S(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C3(Fk, _center55, _n55, _e55, _s55, _w55, _ne55, _se55, _sw55) &&
	   A[E(p,rs)]<k && A[S(p,rs)]<k) {
	D[p] = CRUCIAL;
      }
      if ( D[S(p,rs)]==DESTRUCTIBLE && D[W(p,rs)]==DESTRUCTIBLE && 
	   MATCH_C3(Fk, _center55, _e55, _s55, _w55, _n55, _se55, _sw55, _nw55) &&
	   A[S(p,rs)]<k && A[W(p,rs)]<k ) {
	D[p] = CRUCIAL;
      }
      if ( D[W(p,rs)]==DESTRUCTIBLE && D[N(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C3(Fk, _center55, _s55, _w55, _n55, _e55, _sw55, _nw55, _ne55) &&
	   A[W(p,rs)]<k && A[N(p,rs)]<k ) {
	D[p] = CRUCIAL;
      }
      if ( D[N(p,rs)]==DESTRUCTIBLE && D[E(p,rs)]==DESTRUCTIBLE &&
	   MATCH_C3(Fk, _center55, _w55, _n55, _e55, _s55, _nw55, _ne55, _se55) &&
	  A[N(p,rs)]<k && A[E(p,rs)]<k) {
	D[p] = CRUCIAL;
      }
    }
  }
}

void match_C4(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]==DESTRUCTIBLE) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);	  
      
      if (D[E(p,rs)]==DESTRUCTIBLE && D[S(p,rs)]==DESTRUCTIBLE && D[SE(p,rs)]==DESTRUCTIBLE &&
	  MATCH_C4(Fk, _center55, _n55, _e55, _s55, _w55, _ne55, _se55, _sw55, _ee55, _see55, _sse55, _ss55) &&
	  A[E(p,rs)]<k && A[S(p,rs)]<k && A[SE(p,rs)]<k) {
	D[p] = D[E(p,rs)] = D[S(p,rs)] = D[SE(p,rs)] = CRUCIAL;
      }
      
      if (D[S(p,rs)]==DESTRUCTIBLE && D[W(p,rs)]==DESTRUCTIBLE && D[SW(p,rs)]==DESTRUCTIBLE &&
	  MATCH_C4(Fk, _center55, _e55, _s55, _w55, _n55, _se55, _sw55, _nw55, _ss55, _ssw55, _sww55, _ww55) &&
	  A[S(p,rs)]<k && A[W(p,rs)]<k && A[SW(p,rs)]<k ) {
	D[p] = D[S(p,rs)] = D[W(p,rs)] = D[SW(p,rs)] = CRUCIAL;
      }
      
      if (D[W(p,rs)]==DESTRUCTIBLE && D[N(p,rs)]==DESTRUCTIBLE && D[NW(p,rs)]==DESTRUCTIBLE && 
	  MATCH_C4(Fk, _center55, _s55, _w55, _n55, _e55, _sw55, _nw55, _ne55, _ww55, _nww55, _nnw55, _nn55) &&
	  A[W(p,rs)]<k && A[N(p,rs)]<k && A[NW(p,rs)]<k ) {
	D[p] = D[W(p,rs)] = D[N(p,rs)] = D[NW(p,rs)] = CRUCIAL;
      }
      
      if (D[N(p,rs)]==DESTRUCTIBLE && D[E(p,rs)]==DESTRUCTIBLE && D[NE(p,rs)]==DESTRUCTIBLE &&
	  MATCH_C4(Fk, _center55, _w55, _n55, _e55, _s55, _nw55, _ne55, _se55, _nn55, _nne55, _nee55, _ee55) &&
	  A[N(p,rs)]<k && A[E(p,rs)]<k && A[NE(p,rs)]<k ) {
	D[p] = D[N(p,rs)] = D[E(p,rs)] = D[NE(p,rs)] = CRUCIAL;
      }
    }
  }
}

void match_C4_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]==DESTRUCTIBLE) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);	  
      
      if (D[E(p,rs)]==DESTRUCTIBLE && D[S(p,rs)]==DESTRUCTIBLE && D[SE(p,rs)]==DESTRUCTIBLE &&
	  MATCH_C4(Fk, _center55, _n55, _e55, _s55, _w55, _ne55, _se55, _sw55, _ee55, _see55, _sse55, _ss55) &&
	  A[E(p,rs)]<k && A[S(p,rs)]<k && A[SE(p,rs)]<k) {
	D[p] = CRUCIAL;
      }
      
      if (D[S(p,rs)]==DESTRUCTIBLE && D[W(p,rs)]==DESTRUCTIBLE && D[SW(p,rs)]==DESTRUCTIBLE &&
	  MATCH_C4(Fk, _center55, _e55, _s55, _w55, _n55, _se55, _sw55, _nw55, _ss55, _ssw55, _sww55, _ww55) &&
	  A[S(p,rs)]<k && A[W(p,rs)]<k && A[SW(p,rs)]<k ) {
	D[p] = CRUCIAL;
      }
      
      if (D[W(p,rs)]==DESTRUCTIBLE && D[N(p,rs)]==DESTRUCTIBLE && D[NW(p,rs)]==DESTRUCTIBLE && 
	  MATCH_C4(Fk, _center55, _s55, _w55, _n55, _e55, _sw55, _nw55, _ne55, _ww55, _nww55, _nnw55, _nn55) &&
	  A[W(p,rs)]<k && A[N(p,rs)]<k && A[NW(p,rs)]<k ) {
	D[p] = CRUCIAL;
      }
      
      if (D[N(p,rs)]==DESTRUCTIBLE && D[E(p,rs)]==DESTRUCTIBLE && D[NE(p,rs)]==DESTRUCTIBLE &&
	  MATCH_C4(Fk, _center55, _w55, _n55, _e55, _s55, _nw55, _ne55, _se55, _nn55, _nne55, _nee55, _ee55) &&
	  A[N(p,rs)]<k && A[E(p,rs)]<k && A[NE(p,rs)]<k ) {
	D[p] = CRUCIAL;
      }
    }
  }
}

void match_C(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]==DESTRUCTIBLE) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);
      
      if (D[E(p,rs)]==DESTRUCTIBLE && MATCH_C(Fk, _center55, _n55, _e55, _s55, _ne55, _se55) && 
	  A[E(p,rs)]<k ) {
	D[p] = D[E(p,rs)] = CRUCIAL_C;
      }
                
      if ( D[W(p,rs)]==DESTRUCTIBLE && MATCH_C(Fk, _center55, _s55, _w55, _n55, _sw55, _nw55) &&
	   A[W(p,rs)]<k ) {
	D[p] = D[W(p,rs)] = CRUCIAL_C;
      }

      if ( D[S(p,rs)]==DESTRUCTIBLE && MATCH_C(Fk, _center55, _e55, _s55, _w55, _se55, _sw55) &&
	   A[S(p,rs)]<k  ) {
	D[p] = D[S(p,rs)] = CRUCIAL_C;
      }
      
      if ( D[N(p,rs)]==DESTRUCTIBLE && MATCH_C(Fk, _center55, _w55, _n55, _e55, _nw55, _ne55) &&  
	   A[N(p,rs)]<k ) {
	D[p] = D[N(p,rs)] = CRUCIAL_C;
      }    
    }    
  }
}


void match_C_asymmetric(uint8_t* F, uint8_t* D, uint8_t* A, uint32_t rs, uint32_t N) {
  uint32_t p;
  uint8_t k;
  uint8_t Fk[25];

  for (p=0; p<N; p++) {     
    if (D[p]==DESTRUCTIBLE) {
      
      k = F[p];
      getNeighborhood(F, rs, N, p, k, Fk);
      
      if (D[E(p,rs)]==DESTRUCTIBLE && MATCH_C(Fk, _center55, _n55, _e55, _s55, _ne55, _se55) && 
	  A[E(p,rs)]<k ) {
	D[p] = CRUCIAL_C;
      }
      
      if ( D[W(p,rs)]==DESTRUCTIBLE && MATCH_C(Fk, _center55, _s55, _w55, _n55, _sw55, _nw55) &&
	   A[W(p,rs)]<k ) {
	D[p] = CRUCIAL_C;
      }

      if ( D[S(p,rs)]==DESTRUCTIBLE && MATCH_C(Fk, _center55, _e55, _s55, _w55, _se55, _sw55) &&
	   A[S(p,rs)]<k  ) {
	D[p] = CRUCIAL_C;
      }
      
      if (D[N(p,rs)]==DESTRUCTIBLE &&  MATCH_C(Fk, _center55, _w55, _n55, _e55, _nw55, _ne55) &&  
	  A[N(p,rs)]<k ) {
	D[p] = CRUCIAL_C;
      }    
    }    
  }
}

/* ==================================== */
int32_t lskelCK2(struct xvimage *image,
		 int32_t nsteps,
		 struct xvimage *inhibit)
/* ==================================== */
/*
Squelette symétrique curviligne basé sur les isthmes 1D
Algo CK2 données: S (image), I (inhibit), n (n_steps)
Pour i := 0; i < n; i++
  C := points de courbe (isthmes) de S
  I := I \cup C
  P := voxels simples pour S et pas dans I
  C2 := voxels cruciaux (matchXXX)
  P := P \ C2
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelCK2"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  uint8_t *I;
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6;

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

  if (nsteps == -1) nsteps = 1000000000;

  for (i = 0; i < N; i++) if (F[i]) F[i] = S_OBJECT; // normalize values

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
    memset(T, 0, N);

    for (i = 0; i < N; i++)
      if (F[i] && simple8(F, i, rs, N))
	SET_SIMPLE(F[i]);

    for (i = 0; i < N; i++)
      if (F[i])
      {
	if (IS_SIMPLE(F[i]) && mc_match1(F, i, rs, N))
	  I[i] = I_INHIBIT;
	else if (t8(mask(F, i, rs, N)) > 1)
	  I[i] = I_INHIBIT;
      }

    for (i = 0; i < N; i++)
      if (I[i] == I_INHIBIT)
	UNSET_SIMPLE(F[i]);

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]))
      {
	m1 = mc_match1(F, i, rs, N);
	m2 = mc_match2(F, i, rs, N);
	m3 = mc_match3(F, i, rs, N);
	m4 = mc_match4(F, i, rs, N);
	m5 = mc_match5(F, i, rs, N);
	m6 = mc_match6(F, i, rs, N);
	if (m1 || m2 || m3 || m4 || m5 || m6)
	{
#ifdef DEBUG_MC
	  printf("point %d,%d : m1 = %d; m2 = %d; m3 = %d; m4 = %d; m5 = %d; m6 = %d\n",
		 i % rs, i / rs, m1, m2, m3, m4, m5, m6);
#endif
	  T[i] = 1; // preserve point
	}
      }

#ifdef DEBUG_MC
    printf("\n");
    for (j = 0; j < cs; j++)
    {
      for (i = 0; i < rs; i++) printf("%d ", F[j*rs + i]);
      printf("      ");
      for (i = 0; i < rs; i++) printf("%d ", T[j*rs + i]);
      printf("\n");
    }
    printf("\n");
#endif

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]) && !T[i]) { F[i] = 0; nonstab = 1; }
    for (i = 0; i < N; i++) if (F[i]) F[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) if (F[i]) F[i] = NDG_MAX; // normalize values

  freeimage(tmp);
  return(1);
} /* lskelCK2() */

/* ==================================== */
int32_t lskelCK2_pers(struct xvimage *image, 
			  struct xvimage *persistence)
/* ==================================== */
// calcule la fonction persistence de la méthode CK2
// VERSION SANS CONTROLE TOPOLOGIQUE
#undef F_NAME
#define F_NAME "lskelCK2_pers"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* l'image de depart */
  struct xvimage *tmp = copyimage(image);
  struct xvimage *inhibit;
  uint8_t *I;
  uint8_t *T = UCHARDATA(tmp);
  int32_t step, nonstab;
  int32_t m1, m2, m3, m4, m5, m6;
  float *P = FLOATDATA(persistence); /* résultat */

  inhibit = copyimage(image); 
  razimage(inhibit);
  I = UCHARDATA(inhibit);

  for (i = 0; i < N; i++) if (F[i]) F[i] = S_OBJECT; // normalize values
  for (i = 0; i < N; i++) P[i] = PERS_INIT_VAL;

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
    memset(T, 0, N);

    for (i = 0; i < N; i++)
      if (F[i] && simple8(F, i, rs, N))
	SET_SIMPLE(F[i]);

    for (i = 0; i < N; i++) // detecte les isthmes
      if (F[i])
      {
	if (IS_SIMPLE(F[i]) && mc_match1(F, i, rs, N))
	  I[i] = I_INHIBIT;
	else if (t8(mask(F, i, rs, N)) > 1)
	  I[i] = I_INHIBIT;
      }

    // ENREGISTRE LA DATE DE NAISSANCE DES ISTHMES
    for (i = 0; i < N; i++)
    {
      if ((P[i] == PERS_INIT_VAL) && (I[i] == I_INHIBIT)) P[i] = (float)step;
    }

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]))
      {
	m1 = mc_match1(F, i, rs, N);
	m2 = mc_match2(F, i, rs, N);
	m3 = mc_match3(F, i, rs, N);
	m4 = mc_match4(F, i, rs, N);
	m5 = mc_match5(F, i, rs, N);
	m6 = mc_match6(F, i, rs, N);
	if (m1 || m2 || m3 || m4 || m5 || m6)
	{
	  T[i] = 1; // preserve point
	}
      }

    for (i = 0; i < N; i++)
      if (IS_SIMPLE(F[i]) && !T[i]) 
      { 
	F[i] = 0; 
	nonstab = 1; 
	if (P[i] != PERS_INIT_VAL) P[i] = (float)step - P[i]; else P[i] = 0;
      }
    for (i = 0; i < N; i++) if (F[i]) F[i] = S_OBJECT;
  }

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  for (i = 0; i < N; i++) 
    if (F[i]) 
    {
      F[i] = NDG_MAX; // normalize values
      P[i] = PERS_INFTY_VAL;
    }

  freeimage(tmp);
  return(1);
} /* lskelCK2_pers() */

/* ==================================== */
static int32_t clique0_match(uint8_t *F, uint8_t *Y, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  3 2 1
  4 . 0
  5 6 7

A B   origine C (forcément dans F et pas dans Y)
C D   au moins AD ou BC dans F
      tous les pts dans F sont hors de Y

 */
{
  int32_t i;
  uint8_t f[8];
  uint8_t y[8];
  extract_vois(F, x, rs, N, f);
  extract_vois(Y, x, rs, N, y);
  for (i = 0; i < 4; i++)
  {
    if (f[0] && y[0]) goto fail;
    if (f[1] && y[1]) goto fail;
    if (f[2] && y[2]) goto fail;
    if (!f[1] && (!f[0] || !f[2])) goto fail;
    return 1;
  fail:
    rotate90_vois(f);
    rotate90_vois(y);
  }
  return 0;
} /* clique0_match() */

/* ==================================== */
static int32_t clique1_match(uint8_t *F, uint8_t *Y, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
/*
  3 2 1
  4 . 0
  5 6 7

A A   avec x : dans F mais pas dans Y
x x   origine : premier x (dans F mais pas dans Y, déjà testé)
B B   (+ rotation 90)

  1-critique : au moins un des A et au moins un des B non nuls
  critique mais pas 1-critique : A et B tous nuls

  Si 1-critique (isthme) retourne 2
  Sinon si critique retourne 1
  Sinon retourne 0
 */
{
  int32_t i;
  uint8_t f[8];
  uint8_t y[8];
  extract_vois(F, x, rs, N, f);
  extract_vois(Y, x, rs, N, y);
  for (i = 0; i < 4; i++)
  {
    if (!f[0] || y[0]) goto fail;
    if ((f[1] == 0) && (f[2] == 0) && (f[6] == 0) && (f[7] == 0)) return 1;
    if ((f[1] == 0) && (f[2] == 0)) goto fail;
    if ((f[6] == 0) && (f[7] == 0)) goto fail;
    return 2;
  fail:
    rotate90_vois(f);
    rotate90_vois(y);
  }
  return 0;
} /* clique1_match() */

/* ==================================== */
static void D_Crucial_Isthmes(uint8_t *F, uint8_t *K, int32_t rs, int32_t N, uint8_t *Y, uint8_t *Z, uint8_t *A, uint8_t *B)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "D_Crucial_Isthmes"
  int32_t i, r;
  memcpy(Y, K, N);
  memset(Z, 0, N);
  memset(A, 0, N);
  memset(B, 0, N);
  // dim = 2
  for (i = 0; i < N; i++)
    if (F[i] && (K[i] || !simple8(F, i, rs, N)))
    {
#ifdef DEBUG_D_Crucial_Isthmes
printf("2-crucial %d %d\n", i % rs, i / rs);
#endif
      A[i] = 1;
      if (!K[i] && t8(mask(F, i, rs, N)) > 1)
      {
#ifdef DEBUG_D_Crucial_Isthmes
printf("isthme mince %d %d\n", i % rs, i / rs);
#endif
        B[i] = 1;
      }
    }
  for (i = 0; i < N; i++) { if (A[i]) Y[i] = 1; if (B[i]) Z[i] = 1; }
    
  // dim = 1
  for (i = 0; i < N; i++)
    if (F[i] && !Y[i])
    {
      r = clique1_match(F, Y, i, rs, N);
      if (r > 0)
      {	
#ifdef DEBUG_D_Crucial_Isthmes
printf("1-crucial %d %d\n", i % rs, i / rs);
#endif
	A[i] = 1;
	if (r > 1) 
	{
#ifdef DEBUG_D_Crucial_Isthmes
printf("isthme epais %d %d\n", i % rs, i / rs);
#endif
	  B[i] = 1;
	}
      }
    }
  for (i = 0; i < N; i++) { if (A[i]) Y[i] = 1; if (B[i]) Z[i] = 1; }

  // dim = 0
  for (i = 0; i < N; i++)
    if (F[i] && !Y[i] && clique0_match(F, Y, i, rs, N))
    {
#ifdef DEBUG_D_Crucial_Isthmes
printf("0-crucial %d %d\n", i % rs, i / rs);
#endif
      Y[i] = 1;
    }
  for (i = 0; i < N; i++) { if (A[i]) Y[i] = 1; if (B[i]) Z[i] = 1; }
} // D_Crucial_Isthmes()

/* ==================================== */
int32_t lskelCK2_pers_topo(struct xvimage *image, 
		      struct xvimage *persistence)
/* ==================================== */
  /* ATTENTION : PAS TESTE */
  /* ATTENTION : PAS TESTE */
  /* ATTENTION : PAS TESTE */
  /* ATTENTION : PAS TESTE */
  /* ATTENTION : PAS TESTE */
/* Hiérarchie de squelettes basées sur la persistence des isthmes 1D - version symétrique
 
   // ETAPE 1 : calcul des persistences et du squelette à persistence 0
   step = 0;
   foreach x in X do P(x) = 0;
   repeat
     step++;
     Y = D_crucial(X,0); Z = Isthmes(X,0,1); 
     foreach x in Z such that P(x) = 0 do P(x) = step;
     foreach x in X \ Y such that P(x) > 0 do P(x) = step - P(x);
     X = Y
   until stability
   foreach x in X do P(x) = +infty
   X = {x such that P(x) > 0}

   // ETAPE 2 : contrôle topologique sur toutes les coupes de la fonction P
   foreach x in X do InsertQueue(Q,x,P(x));
   repeat
     lambda = MinValQueue(Q);
     T = PopMinSetQueue(Q,lambda);
     Y = D_crucial(X,X \ T)
     foreach x in T inter Y do // non-deletable candidates
       psi = +infty
       foreach y in Gamma*(x) such that InQueue(Q,y) do
         psi = min(psi,P(y));
       P(x) = psi
       InsertQueue(Q,x,psi);
     X = Y
   until lambda = +infty

 */

#undef F_NAME
#define F_NAME "lskelCK2_pers_topo"
{
  int32_t x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *X = UCHARDATA(image);   /* l'image de depart */
  struct xvimage *Yimage = copyimage(image);
  struct xvimage *Zimage = copyimage(image);
  struct xvimage *Kimage = copyimage(image);
  struct xvimage *Aimage = copyimage(image);
  struct xvimage *Bimage = copyimage(image);
  uint8_t *A = UCHARDATA(Aimage);
  uint8_t *B = UCHARDATA(Bimage);
  uint8_t *Y = UCHARDATA(Yimage);
  uint8_t *Z = UCHARDATA(Zimage);
  uint8_t *K = UCHARDATA(Kimage);
  int32_t step, ndel;
  float *P = FLOATDATA(persistence); /* résultat */
  Rbt * Q;
  index_t taillemaxrbt;

  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  Q = mcrbt_CreeRbtVide(taillemaxrbt);
  if (Q == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  for (x = 0; x < N; x++) if (X[x]) X[x] = S_OBJECT; // normalize values

  //   step = 0; K = empty; 
  step = 0;
  razimage(Kimage);
  // foreach x in X do P(x) = 0;
  for (x = 0; x < N; x++) P[x] = PERS_INIT_VAL;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  do
  {
    ndel = 0;
    step++;
//if (step > 10) goto stop;
#ifdef VERBOSE
    printf("step %d\n", step);
#endif

    // Y = D_crucial(X,0); Z = Isthmes(X,0,1);
    D_Crucial_Isthmes(X, K, rs, N, Y, Z, A, B);

    // foreach x in Z such that P(x) = 0 do P(x) = step;
    for (x = 0; x < N; x++)
      if (Z[x] && (P[x] == PERS_INIT_VAL)) 
      {
	P[x] = (float)step;
      }

    // foreach x in X \ Y such that P(x) > 0 do
    for (x = 0; x < N; x++)
      if (X[x] && !Y[x] && (P[x] != PERS_INIT_VAL)) 
      { 
	// P(x) = step - P(x);
	P[x] = (float)step - P[x];
      }

    // X = Y
    for (x = 0; x < N; x++) 
      if (X[x])
      { 
	if (Y[x])
	  X[x] = S_OBJECT;
	else
	{
	  X[x] = 0;
	  ndel++; 
	}
      }

#ifdef VERBOSE
    printf("ndel = %d\n", ndel);
#endif

  } while (ndel > 0);

stop:;

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

    for (x = 0; x < N; x++) if (X[x]) P[x] = PERS_INFTY_VAL;

    for (x = 0; x < N; x++) if (P[x] > 0) X[x] = S_OBJECT;

  for (x = 0; x < N; x++) 
    if (X[x]) 
    {
      X[x] = NDG_MAX; // normalize values
    }




  mcrbt_RbtTermine(Q);
  freeimage(Aimage);
  freeimage(Bimage);
  freeimage(Yimage);
  freeimage(Zimage);
  freeimage(Kimage);
  return(1);
} /* lskelCK2_pers_topo() */

#ifdef COMPILE_lskelCK2p

PAS FINI !!!


/* ==================================== */
int32_t lskelCK2p(struct xvimage *image, 
	     int32_t n_steps,
	     int32_t isthmus_persistence,
	     struct xvimage *inhibit)
/* ==================================== */
/*
Squelette symétrique curviligne
Algo CK2p données: S (image), I (inhibit), n (n_steps), p (isthmus_persistence)
Pour tout x de S faire T[x] := PERS_INIT_VAL
i := 0 
Repeat
  i := i + 1
  C := points de courbe de S
  Pour tout x de C tq T[x] == PERS_INIT_VAL faire T[x] := i
  P := pixels simples pour S et pas dans I
  C1 := pixels 1-D-cruciaux (match1)
  C0 := pixels 0-D-cruciaux (match0)
  Z := {x non simples ou cruciaux | T[x] > PERS_INIT_VAL et (i + 1 - T[x]) >= p}
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P
  I := I \cup Z 
Until stability or i = n

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelCK2p"
{ 
  index_t i; // index de pixel
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t N = rs * cs;            /* taille plan */
  uint8_t *S = UCHARDATA(image);   /* l'image de depart */
  int16_t *T;
  uint8_t *I;
  int32_t step, nonstab;
  int32_t top, topb;

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
	printf("point %d (%d %d %d) ajout à K\n", i, i % rs, (i % ps) / rs,  i / ps);
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
  } // while (nonstab && (step < n_steps))

  for (i = 0; i < N; i++) if (S[i]) S[i] = NDG_MAX;

#ifdef VERBOSE1
    printf("number of steps: %d\n", step);
#endif

  free(T);
  return(1);
} /* lskelCK2p() */

#endif
