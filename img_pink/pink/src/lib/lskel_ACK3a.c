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
/* Michel Couprie -  octobre 2014 */

//#define VERBOSE
//#define DEBUG
// définir seulement un parmi les suivants :
//#define lskelCKSC3_DIR3
//#define lskelCKSC3_DIR2
//#define lskelCKSC3_DIR
#define lskelCKSC3_DIRNO

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mcindic.h>
#include <mcrbt.h>
#include <mctopo3d.h>
#include <mckhalimsky3d.h>
#include <mcfifo.h>
#include <mcliste.h>
#include <mcrlifo.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <pinktypes.h>
#include <ldist.h>
#include <lskelpar3d_others.h>
#include <lskel_ACK3a.h>

#define S_OBJECT       1
#define S_SIMPLE       2
#define S_PSEL         8
#define S_SELECTED    64
#define S_CONSTRAINT 128

#define IS_OBJECT(f)     (f&S_OBJECT)
#define IS_SIMPLE(f)     (f&S_SIMPLE)
#define IS_PSEL(f)       (f&S_PSEL)
#define IS_SELECTED(f)   (f&S_SELECTED)
#define IS_CONSTRAINT(f) (f&S_CONSTRAINT)

#define SET_OBJECT(f)     (f|=S_OBJECT)
#define SET_SIMPLE(f)     (f|=S_SIMPLE)
#define SET_PSEL(f)       (f|=S_PSEL)
#define SET_SELECTED(f)   (f|=S_SELECTED)
#define SET_CONSTRAINT(f) (f|=S_CONSTRAINT)

#define UNSET_SIMPLE(f)   (f&=~S_SIMPLE)
#define UNSET_SELECTED(f) (f&=~S_SELECTED)
#define UNSET_PSEL(f)     (f&=~S_PSEL)
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

/* ==================================== */
static void insert_vois27(
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
#define F_NAME "insert_vois27"
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
	img[((zz-1+k) * ps) + ((yy-1+j) * rs) + xx-1+i] = vois[(k * 9) + (j * 3) + i];
} /* insert_vois27() */

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

static int DBG=0;
#endif

// FONCTIONS match_vois* : marque les points des cliques critiques

/* ==================================== */
static int32_t match_vois2_z(uint8_t *v)
/* ==================================== */
/*

  0   1   2   
  3   4   5
  6   7   8   

  9   10  11
  12  13  14
  15  16  17

Teste si les conditions suivantes sont réunies:
1: v[ 4] et v[13] doivent être marqués SIMPLE et pas CONSTRAINT ni PSEL
2: la projection 2D doit être non 2D-simple
Si le test réussit, le point 4 est marqué SELECTED
*/
{
  uint8_t t;
  if ((!IS_SIMPLE(v[ 4])) || (!IS_SIMPLE(v[13]))) return 0;
  if ((IS_CONSTRAINT(v[ 4])) || (IS_CONSTRAINT(v[13]))) return 0;
  if ((IS_PSEL(v[ 4])) || (IS_PSEL(v[13]))) return 0;
  if (v[ 5] || v[14]) t = 1; else t = 0;
  if (v[ 2] || v[11]) t |= 2;
  if (v[ 1] || v[10]) t |= 4;
  if (v[ 0] || v[ 9]) t |= 8;
  if (v[ 3] || v[12]) t |= 16;
  if (v[ 6] || v[15]) t |= 32;
  if (v[ 7] || v[16]) t |= 64;
  if (v[ 8] || v[17]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
  SET_SELECTED(v[4]);
  //printf("match 2a\n");
  return 1;
} // match_vois2_z()


/* ==================================== */
static int32_t match_vois2_y(uint8_t *v)
/* ==================================== */
/*
                0       1       2       
                3       4       5 

                9      10      11 
               12      13      14 

               18      19      20 
               21      22      23 
Teste si les conditions suivantes sont réunies:
1: v[10] et v[13] doivent être marqués SIMPLE et pas CONSTRAINT ni PSEL
2: la projection 2D doit être non 2D-simple
Si le test réussit, le point 10 est marqué SELECTED
*/
{
  uint8_t t;
  if ((!IS_SIMPLE(v[10])) || (!IS_SIMPLE(v[13]))) return 0;
  if ((IS_CONSTRAINT(v[10])) || (IS_CONSTRAINT(v[13]))) return 0;
  if ((IS_PSEL(v[10])) || (IS_PSEL(v[13]))) return 0;
  if (v[14] || v[11]) t = 1; else t = 0;
  if (v[ 5] || v[ 2]) t |= 2;
  if (v[ 4] || v[ 1]) t |= 4;
  if (v[ 3] || v[ 0]) t |= 8;
  if (v[12] || v[ 9]) t |= 16;
  if (v[21] || v[18]) t |= 32;
  if (v[22] || v[19]) t |= 64;
  if (v[23] || v[20]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
  SET_SELECTED(v[10]);
  //printf("match 2b\n");
  return 1;
} // match_vois2_y()

/* ==================================== */
static int32_t match_vois2_x(uint8_t *v)
/* ==================================== */
/*
                0       1 
                3       4 
                6       7 

                9      10 
               12      13 
               15      16 

               18      19 
               21      22 
               24      25 
         
Teste si les conditions suivantes sont réunies:
1: v[12] et v[13] doivent être simples et pas CONSTRAINT ni PSEL
2: la projection 2D doit être non 2D-simple
Si le test réussit, le point 12 est marqué SELECTED
*/
{
  uint8_t t;
  if ((!IS_SIMPLE(v[12])) || (!IS_SIMPLE(v[13]))) return 0;
  if ((IS_CONSTRAINT(v[12])) || (IS_CONSTRAINT(v[13]))) return 0;
  if ((IS_PSEL(v[12])) || (IS_PSEL(v[13]))) return 0;
  if (v[ 9] || v[10]) t = 1; else t = 0;
  if (v[ 0] || v[ 1]) t |= 2;
  if (v[ 3] || v[ 4]) t |= 4;
  if (v[ 6] || v[ 7]) t |= 8;
  if (v[15] || v[16]) t |= 16;
  if (v[24] || v[25]) t |= 32;
  if (v[21] || v[22]) t |= 64;
  if (v[18] || v[19]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
  SET_SELECTED(v[12]);
  //printf("match 2c\n");
  return 1;
} // match_vois2_x()

/* ==================================== */
static int32_t match_vois1_z_1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//  0   1 
//  3   4 
//
//  9  10 
// 12  13 
//
// 18  19 
// 21  22 

// Teste si les trois conditions suivantes sont réunies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent être simples et non marqués PSEL ou CONSTRAINT
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test réussit, le premier point Pi non nul est marqué SELECTED
{
  if (!((v[10] && v[12]) || (v[ 9] && v[13]))) return 0;
  if ((v[10]  && ((!IS_SIMPLE(v[10])) || IS_PSEL(v[10]) || IS_CONSTRAINT(v[10]))) ||
      (v[ 9]  && ((!IS_SIMPLE(v[ 9])) || IS_PSEL(v[ 9]) || IS_CONSTRAINT(v[ 9]))) ||
      (v[12]  && ((!IS_SIMPLE(v[12])) || IS_PSEL(v[12]) || IS_CONSTRAINT(v[12]))) ||
      (v[13]  && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]) || IS_CONSTRAINT(v[13])))) return 0;
  if ((v[ 0] || v[ 1] || v[ 3] || v[ 4] || v[18] || v[19] || v[21] || v[22]) &&
      ((!v[ 0] && !v[ 1] && !v[ 3] && !v[ 4]) || 
       (!v[18] && !v[19] && !v[21] && !v[22]))) return 0;
  if (v[ 9]) { SET_SELECTED(v[ 9]); return 1; }
  if (v[10]) { SET_SELECTED(v[10]); return 1; }
  if (v[12]) { SET_SELECTED(v[12]); return 1; }
  if (v[13]) { SET_SELECTED(v[13]); return 1; }
  assert(0); return 0;
} // match_vois1_z_1()

/* ==================================== */
static int32_t match_vois1_z_2(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//  1   2 
//  4   5 
//
// 10  11 
// 13  14 
//
// 19  20 
// 22  23 

// Teste si les trois conditions suivantes sont réunies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent être simples et non marqués PSEL ou CONSTRAINT
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test réussit, le premier point Pi non nul est marqué SELECTED
{
  if (!((v[10] && v[14]) || (v[11] && v[13]))) return 0;
  if ((v[10] && ((!IS_SIMPLE(v[10])) || IS_PSEL(v[10]) || IS_CONSTRAINT(v[10]))) ||
      (v[11] && ((!IS_SIMPLE(v[11])) || IS_PSEL(v[11]) || IS_CONSTRAINT(v[11]))) ||
      (v[14] && ((!IS_SIMPLE(v[14])) || IS_PSEL(v[14]) || IS_CONSTRAINT(v[14]))) ||
      (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]) || IS_CONSTRAINT(v[13])))) return 0;
  if ((v[ 2] || v[ 1] || v[ 5] || v[ 4] || v[20] || v[19] || v[23] || v[22]) &&
      ((!v[ 2] && !v[ 1] && !v[ 5] && !v[ 4]) || 
       (!v[20] && !v[19] && !v[23] && !v[22]))) return 0;
  if (v[10]) { SET_SELECTED(v[10]); return 1; }
  if (v[11]) { SET_SELECTED(v[11]); return 1; }
  if (v[13]) { SET_SELECTED(v[13]); return 1; }
  if (v[14]) { SET_SELECTED(v[14]); return 1; }
  assert(0); return 0;
} // match_vois1_z_2()

/* ==================================== */
static int32_t match_vois1_x_1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//   3   4   5 
//   6   7   8 
//
//  12  13  14       
//  15  16  17 
//
// Teste si les trois conditions suivantes sont réunies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent être simples et non marqués PSEL ou CONSTRAINT
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test réussit, le premier point Pi non nul est marqué SELECTED
{
  if (!((v[16] && v[ 4]) || (v[ 7] && v[13]))) return 0;
  if ((v[16] && ((!IS_SIMPLE(v[16])) || IS_PSEL(v[16]) || IS_CONSTRAINT(v[16]))) ||
      (v[ 4] && ((!IS_SIMPLE(v[ 4])) || IS_PSEL(v[ 4]) || IS_CONSTRAINT(v[ 4]))) ||
      (v[ 7] && ((!IS_SIMPLE(v[ 7])) || IS_PSEL(v[ 7]) || IS_CONSTRAINT(v[ 7]))) ||
      (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]) || IS_CONSTRAINT(v[13])))) return 0;
  if ((v[ 6] || v[ 3] || v[15] || v[12] || v[17] || v[14] || v[ 5] || v[ 8]) &&
      ((!v[ 6] && !v[ 3] && !v[15] && !v[12]) || 
       (!v[17] && !v[14] && !v[ 5] && !v[ 8]))) return 0;
  if (v[ 4]) { SET_SELECTED(v[ 4]); return 1; }
  if (v[ 7]) { SET_SELECTED(v[ 7]); return 1; }
  if (v[13]) { SET_SELECTED(v[13]); return 1; }
  if (v[16]) { SET_SELECTED(v[16]); return 1; }
  assert(0); return 0;
} // match_vois1_x_1()

/* ==================================== */
static int32_t match_vois1_x_2(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//   0   1   2 
//   3   4   5 
//
//   9  10  11 
//  12  13  14 
//
// Teste si les trois conditions suivantes sont réunies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent être simples et non marqués PSEL ou CONSTRAINT
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test réussit, le premier point Pi non nul est marqué SELECTED
{
  if (!((v[10] && v[ 4]) || (v[ 1] && v[13]))) return 0;
  if ((v[10] && ((!IS_SIMPLE(v[10])) || IS_PSEL(v[10]) || IS_CONSTRAINT(v[10]))) ||
      (v[ 4] && ((!IS_SIMPLE(v[ 4])) || IS_PSEL(v[ 4]) || IS_CONSTRAINT(v[ 4]))) ||
      (v[ 1] && ((!IS_SIMPLE(v[ 1])) || IS_PSEL(v[ 1]) || IS_CONSTRAINT(v[ 1]))) ||
      (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]) || IS_CONSTRAINT(v[13])))) return 0;
  if ((v[ 0] || v[ 3] || v[ 9] || v[12] || v[11] || v[14] || v[ 5] || v[ 2]) &&
      ((!v[ 0] && !v[ 3] && !v[ 9] && !v[12]) || 
       (!v[11] && !v[14] && !v[ 5] && !v[ 2]))) return 0;
  if (v[ 1]) { SET_SELECTED(v[ 1]); return 1; }
  if (v[ 4]) { SET_SELECTED(v[ 4]); return 1; }
  if (v[10]) { SET_SELECTED(v[10]); return 1; }
  if (v[13]) { SET_SELECTED(v[13]); return 1; }
  assert(0); return 0;
} // match_vois1_x_2()

/* ==================================== */
static int32_t match_vois1_y_1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//   1   2 
//   4   5 
//   7   8 
//
//  10  11 
//  13  14 
//  16  17 
// Teste si les trois conditions suivantes sont réunies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent être simples et non marqués PSEL ou CONSTRAINT
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test réussit, le premier point Pi non nul est marqué SELECTED
{
  if (!((v[ 4] && v[14]) || (v[ 5] && v[13]))) return 0;
  if ((v[ 4] && ((!IS_SIMPLE(v[ 4])) || IS_PSEL(v[ 4]) || IS_CONSTRAINT(v[ 4]))) ||
      (v[ 5] && ((!IS_SIMPLE(v[ 5])) || IS_PSEL(v[ 5]) || IS_CONSTRAINT(v[ 5]))) ||
      (v[14] && ((!IS_SIMPLE(v[14])) || IS_PSEL(v[14]) || IS_CONSTRAINT(v[14]))) ||
      (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]) || IS_CONSTRAINT(v[13])))) return 0;
  if ((v[ 1] || v[ 2] || v[10] || v[11] || v[ 8] || v[ 7] || v[17] || v[16]) &&
      ((!v[ 1] && !v[ 2] && !v[10] && !v[11]) || 
       (!v[ 8] && !v[ 7] && !v[17] && !v[16]))) return 0;
  if (v[ 4]) { SET_SELECTED(v[ 4]); return 1; }
  if (v[ 5]) { SET_SELECTED(v[ 5]); return 1; }
  if (v[13]) { SET_SELECTED(v[13]); return 1; }
  if (v[14]) { SET_SELECTED(v[14]); return 1; }
  assert(0); return 0;
} // match_vois1_y_1()

/* ==================================== */
static int32_t match_vois1_y_2(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//   0   1 
//   3   4 
//   6   7 
//
//   9  10 
//  12  13 
//  15  16 
// Teste si les trois conditions suivantes sont réunies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent être simples et non marqués PSEL ou CONSTRAINT
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test réussit, le premier point Pi non nul est marqué SELECTED
{
  if (!((v[ 4] && v[12]) || (v[ 3] && v[13]))) return 0;
  if ((v[ 4] && ((!IS_SIMPLE(v[ 4])) || IS_PSEL(v[ 4]) || IS_CONSTRAINT(v[ 4]))) ||
      (v[ 3] && ((!IS_SIMPLE(v[ 3])) || IS_PSEL(v[ 3]) || IS_CONSTRAINT(v[ 3]))) ||
      (v[12] && ((!IS_SIMPLE(v[12])) || IS_PSEL(v[12]) || IS_CONSTRAINT(v[12]))) ||
      (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]) || IS_CONSTRAINT(v[13])))) return 0;
  if ((v[ 0] || v[ 1] || v[ 9] || v[10] || v[ 6] || v[ 7] || v[15] || v[16]) &&
      ((!v[ 0] && !v[ 1] && !v[ 9] && !v[10]) || 
       (!v[ 6] && !v[ 7] && !v[15] && !v[16]))) return 0;
  if (v[ 3]) { SET_SELECTED(v[ 3]); return 1; }
  if (v[ 4]) { SET_SELECTED(v[ 4]); return 1; }
  if (v[12]) { SET_SELECTED(v[12]); return 1; }
  if (v[13]) { SET_SELECTED(v[13]); return 1; }
  assert(0); return 0;
} // match_vois1_y_2()

/* ==================================== */
static int32_t match_vois0_1(uint8_t *v)
/* ==================================== */
/*
   0   1 
   3   4 

   9  10 
  12  13 

Teste si les conditions suivantes sont réunies:
1: au moins un des ensembles { 0,13}, { 1,4}, { 3,2}, {8,3} est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqués PSEL
Si le test réussit, le premier point Pi non nul est marqué SELECTED
*/
{
  //printf("match 01:\n");
  if (!((v[ 0]&&v[13]) || (v[ 1]&&v[12]) || (v[ 3]&&v[10]) || (v[ 4]&&v[ 9]) )) return 0;
  if (v[ 0] && ((!IS_SIMPLE(v[ 0])) || IS_PSEL(v[ 0]))) return 0;
  if (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]))) return 0;
  if (v[ 1] && ((!IS_SIMPLE(v[ 1])) || IS_PSEL(v[ 1]))) return 0;
  if (v[12] && ((!IS_SIMPLE(v[12])) || IS_PSEL(v[12]))) return 0;
  if (v[ 3] && ((!IS_SIMPLE(v[ 3])) || IS_PSEL(v[ 3]))) return 0;
  if (v[10] && ((!IS_SIMPLE(v[10])) || IS_PSEL(v[10]))) return 0;
  if (v[ 4] && ((!IS_SIMPLE(v[ 4])) || IS_PSEL(v[ 4]))) return 0;
  if (v[ 9] && ((!IS_SIMPLE(v[ 9])) || IS_PSEL(v[ 9]))) return 0;
  if (v[ 0])  { SET_SELECTED(v[ 0]); return 1; }
  if (v[ 1])  { SET_SELECTED(v[ 1]); return 1; }
  if (v[ 3])  { SET_SELECTED(v[ 3]); return 1; }
  if (v[ 4])  { SET_SELECTED(v[ 4]); return 1; }
  if (v[ 9])  { SET_SELECTED(v[ 9]); return 1; }
  if (v[10])  { SET_SELECTED(v[10]); return 1; }
  if (v[12])  { SET_SELECTED(v[12]); return 1; }
  if (v[13])  { SET_SELECTED(v[13]); return 1; }
  assert(0); return 0;
} // match_vois0_1()

/* ==================================== */
static int32_t match_vois0_2(uint8_t *v)
/* ==================================== */
/*
   1   2 
   4   5 

  10  11 
  13  14 

Teste si les conditions suivantes sont réunies:
1: au moins un des ensembles ... est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqués 2M_CRUCIAL et non marqués 1M_CRUCIAL
Si le test réussit, le premier point Pi non nul est marqué SELECTED
*/
{
  //printf("match 02:\n");
  if (!((v[ 2]&&v[13]) || (v[ 1]&&v[14]) || (v[ 5]&&v[10]) || (v[ 4]&&v[11]) )) return 0;
  if (v[ 2] && ((!IS_SIMPLE(v[ 2])) || IS_PSEL(v[ 2]))) return 0;
  if (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]))) return 0;
  if (v[ 1] && ((!IS_SIMPLE(v[ 1])) || IS_PSEL(v[ 1]))) return 0;
  if (v[11] && ((!IS_SIMPLE(v[11])) || IS_PSEL(v[11]))) return 0;
  if (v[ 5] && ((!IS_SIMPLE(v[ 5])) || IS_PSEL(v[ 5]))) return 0;
  if (v[10] && ((!IS_SIMPLE(v[10])) || IS_PSEL(v[10]))) return 0;
  if (v[ 4] && ((!IS_SIMPLE(v[ 4])) || IS_PSEL(v[ 4]))) return 0;
  if (v[14] && ((!IS_SIMPLE(v[14])) || IS_PSEL(v[14]))) return 0;
  if (v[ 1])  { SET_SELECTED(v[ 1]); return 1; }
  if (v[ 2])  { SET_SELECTED(v[ 2]); return 1; }
  if (v[ 4])  { SET_SELECTED(v[ 4]); return 1; }
  if (v[ 5])  { SET_SELECTED(v[ 5]); return 1; }
  if (v[10])  { SET_SELECTED(v[10]); return 1; }
  if (v[11])  { SET_SELECTED(v[11]); return 1; }
  if (v[13])  { SET_SELECTED(v[13]); return 1; }
  if (v[14])  { SET_SELECTED(v[14]); return 1; }
  assert(0); return 0;
} // match_vois0_2()


/* ==================================== */
static int32_t match_vois0_3(uint8_t *v)
/* ==================================== */
/*
   4   5 
   7   8 

  13  14 
  16  17 

Teste si les conditions suivantes sont réunies:
1: au moins un des ensembles ... est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqués 2M_CRUCIAL et non marqués 1M_CRUCIAL
Si le test réussit, le premier point Pi non nul est marqué SELECTED
*/
{
  //printf("match 03:\n");
  if (!((v[ 8]&&v[13]) || (v[ 7]&&v[14]) || (v[16]&&v[ 5]) || (v[ 4]&&v[17]) )) return 0;
  if (v[ 8] && ((!IS_SIMPLE(v[ 8])) || IS_PSEL(v[ 8]))) return 0;
  if (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]))) return 0;
  if (v[ 7] && ((!IS_SIMPLE(v[ 7])) || IS_PSEL(v[ 7]))) return 0;
  if (v[14] && ((!IS_SIMPLE(v[14])) || IS_PSEL(v[14]))) return 0;
  if (v[16] && ((!IS_SIMPLE(v[16])) || IS_PSEL(v[16]))) return 0;
  if (v[ 5] && ((!IS_SIMPLE(v[ 5])) || IS_PSEL(v[ 5]))) return 0;
  if (v[ 4] && ((!IS_SIMPLE(v[ 4])) || IS_PSEL(v[ 4]))) return 0;
  if (v[17] && ((!IS_SIMPLE(v[17])) || IS_PSEL(v[17]))) return 0;
  if (v[ 4])  { SET_SELECTED(v[ 4]); return 1; }
  if (v[ 5])  { SET_SELECTED(v[ 5]); return 1; }
  if (v[ 7])  { SET_SELECTED(v[ 7]); return 1; }
  if (v[ 8])  { SET_SELECTED(v[ 8]); return 1; }
  if (v[13])  { SET_SELECTED(v[13]); return 1; }
  if (v[14])  { SET_SELECTED(v[14]); return 1; }
  if (v[16])  { SET_SELECTED(v[16]); return 1; }
  if (v[17])  { SET_SELECTED(v[17]); return 1; }
  assert(0); return 0;
} // match_vois0_3()

/* ==================================== */
static int32_t match_vois0_4(uint8_t *v)
/* ==================================== */
/*
   3   4 
   6   7 

  12  13 
  15  16 

Teste si les conditions suivantes sont réunies:
1: au moins un des ensembles ... est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqués 2M_CRUCIAL et non marqués 1M_CRUCIAL
Si le test réussit, le premier point Pi non nul est marqué SELECTED
*/
{
  //printf("match 04:\n");
  if (!((v[ 6]&&v[13]) || (v[ 7]&&v[12]) || (v[ 3]&&v[16]) || (v[ 4]&&v[15]) )) return 0;
  if (v[ 6] && ((!IS_SIMPLE(v[ 6])) || IS_PSEL(v[ 6]))) return 0;
  if (v[13] && ((!IS_SIMPLE(v[13])) || IS_PSEL(v[13]))) return 0;
  if (v[ 7] && ((!IS_SIMPLE(v[ 7])) || IS_PSEL(v[ 7]))) return 0;
  if (v[12] && ((!IS_SIMPLE(v[12])) || IS_PSEL(v[12]))) return 0;
  if (v[ 3] && ((!IS_SIMPLE(v[ 3])) || IS_PSEL(v[ 3]))) return 0;
  if (v[16] && ((!IS_SIMPLE(v[16])) || IS_PSEL(v[16]))) return 0;
  if (v[ 4] && ((!IS_SIMPLE(v[ 4])) || IS_PSEL(v[ 4]))) return 0;
  if (v[15] && ((!IS_SIMPLE(v[15])) || IS_PSEL(v[15]))) return 0;
  if (v[ 3])  { SET_SELECTED(v[ 3]); return 1; }
  if (v[ 4])  { SET_SELECTED(v[ 4]); return 1; }
  if (v[ 6])  { SET_SELECTED(v[ 6]); return 1; }
  if (v[ 7])  { SET_SELECTED(v[ 7]); return 1; }
  if (v[12])  { SET_SELECTED(v[12]); return 1; }
  if (v[13])  { SET_SELECTED(v[13]); return 1; }
  if (v[15])  { SET_SELECTED(v[15]); return 1; }
  if (v[16])  { SET_SELECTED(v[16]); return 1; }
  assert(0); return 0;
} // match_vois0_4()

#ifdef lskelCKSC3_DIR3
static void lskelCKSC3_process_cliques(struct xvimage *image)
{
  uint8_t *F = UCHARDATA(image);
  index_t rs = rowsize(image);
  index_t cs = colsize(image);
  index_t ds = depth(image);
  index_t ps = rs * cs;
  index_t N = ps * ds;
  int32_t i, j, k, m1, m2, m3, m4, m5, m6;
  index_t x;
  uint8_t v[27];

    // DIM = 2
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m1 = match_vois2_z(v);
	if (m1) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m2 = match_vois2_y(v); 
	if (m2) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m3 = match_vois2_x(v); 
	if (m3) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    // DIM = 1
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m1 = match_vois1_z_1(v); 
	if (m1) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m3 = match_vois1_y_1(v);
	if (m3) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m5 = match_vois1_x_1(v);
	if (m5) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m2 = match_vois1_z_2(v);
	if (m2) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m4 = match_vois1_y_2(v);
	if (m4) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m6 = match_vois1_x_2(v);
	if (m6) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    // DIM = 0
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m1 = match_vois0_1(v);
	m2 = match_vois0_2(v);
	m3 = match_vois0_3(v);
	m4 = match_vois0_4(v);
	if (m1 || m2 || m3 || m4) insert_vois27(F, x, rs, ps, N, v);
      }
    }
} // lskelCKSC3_process_cliques
#endif

#ifdef lskelCKSC3_DIR2
static void lskelCKSC3_process_cliques(struct xvimage *image)
{
  uint8_t *F = UCHARDATA(image);
  index_t rs = rowsize(image);
  index_t cs = colsize(image);
  index_t ds = depth(image);
  index_t ps = rs * cs;
  index_t N = ps * ds;
  int32_t i, j, k, m1, m2, m3, m4, m5, m6;
  index_t x;
  uint8_t v[27];

    // DIM = 2
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m1 = match_vois2_z(v);
	if (m1) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m2 = match_vois2_y(v); 
	if (m2) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m3 = match_vois2_x(v); 
	if (m3) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    // DIM = 1
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m1 = match_vois1_z_1(v); 
	if (m1) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m2 = match_vois1_z_2(v);
	if (m2) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m3 = match_vois1_y_1(v);
	if (m3) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m4 = match_vois1_y_2(v);
	if (m4) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m5 = match_vois1_x_1(v);
	if (m5) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m6 = match_vois1_x_2(v);
	if (m6) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    // DIM = 0
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m1 = match_vois0_1(v);
	m2 = match_vois0_2(v);
	m3 = match_vois0_3(v);
	m4 = match_vois0_4(v);
	if (m1 || m2 || m3 || m4) insert_vois27(F, x, rs, ps, N, v);
      }
    }
} // lskelCKSC3_process_cliques
#endif

#ifdef lskelCKSC3_DIR
static void lskelCKSC3_process_cliques(struct xvimage *image)
{
  uint8_t *F = UCHARDATA(image);
  index_t rs = rowsize(image);
  index_t cs = colsize(image);
  index_t ds = depth(image);
  index_t ps = rs * cs;
  index_t N = ps * ds;
  int32_t i, j, k, m1, m2, m3, m4, m5, m6;
  index_t x;
  uint8_t v[27];

    // DIM = 2
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m1 = match_vois2_z(v);
	if (m1) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m2 = match_vois2_y(v); 
	if (m2) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m3 = match_vois2_x(v); 
	if (m3) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    // DIM = 1
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m1 = match_vois1_z_1(v); 
	m2 = match_vois1_z_2(v);
	if (m1 || m2) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m3 = match_vois1_y_1(v);
	m4 = match_vois1_y_2(v);
	if (m3 || m4) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m5 = match_vois1_x_1(v);
	m6 = match_vois1_x_2(v);
	if (m5 || m6) insert_vois27(F, x, rs, ps, N, v);
      }
    }
    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    // DIM = 0
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m1 = match_vois0_1(v);
	m2 = match_vois0_2(v);
	m3 = match_vois0_3(v);
	m4 = match_vois0_4(v);
	if (m1 || m2 || m3 || m4) insert_vois27(F, x, rs, ps, N, v);
      }
    }
} // lskelCKSC3_process_cliques
#endif

#ifdef lskelCKSC3_DIRNO
static void lskelCKSC3_process_cliques(struct xvimage *image)
{
  uint8_t *F = UCHARDATA(image);
  index_t rs = rowsize(image);
  index_t cs = colsize(image);
  index_t ds = depth(image);
  index_t ps = rs * cs;
  index_t N = ps * ds;
  int32_t i, j, k, m1, m2, m3, m4, m5, m6;
  index_t x;
  uint8_t v[27];

    // DIM = 2
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v); 
	m1 = match_vois2_z(v);
	m2 = match_vois2_y(v); 
	m3 = match_vois2_x(v); 
	if (m1 || m2 || m3) insert_vois27(F, x, rs, ps, N, v);
      }
    }

    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    // DIM = 1
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m1 = match_vois1_z_1(v); 
	m2 = match_vois1_z_2(v);
	m3 = match_vois1_x_1(v);
	m4 = match_vois1_x_2(v);
	m5 = match_vois1_y_1(v);
	m6 = match_vois1_y_2(v);
	if (m1 || m2 || m3 || m4 || m5 || m6) insert_vois27(F, x, rs, ps, N, v);
      }
    }

    for (i = 1; i < N; i++) if (IS_SELECTED(F[i])) { UNSET_SELECTED(F[i]); SET_PSEL(F[i]); }

    // DIM = 0
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois27(F, x, rs, ps, N, v);
	m1 = match_vois0_1(v);
	m2 = match_vois0_2(v);
	m3 = match_vois0_3(v);
	m4 = match_vois0_4(v);
	if (m1 || m2 || m3 || m4) insert_vois27(F, x, rs, ps, N, v);
      }
    }
} // lskelCKSC3_process_cliques
#endif

/* ==================================== */
int32_t lskelACK3a(struct xvimage *image, 
		   int32_t nsteps,
		   struct xvimage *inhibit,
		   int32_t mode)
/* ==================================== */
/*
Squelette asymétrique curviligne
Algo ACK3a données: S
Répéter jusqu'à stabilité
  C := points de courbe de S
  I := I \cup C
  P := voxels simples pour S et pas dans I
  C2 := voxels 2-D-cruciaux (1 par clique critique)
  C1 := voxels 1-D-cruciaux (1 par clique critique)
  C0 := voxels 0-D-cruciaux (1 par clique critique)
  P := P  \  [C2 \cup C1 \cup C0]
  S := S \ P

Attention : l'objet ne doit pas toucher le bord de l'image
*/
#undef F_NAME
#define F_NAME "lskelACK3a"
{ 
  int32_t i, j, k, t, tb, stab, nbiter;
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(image);       /* nb plans */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* objet */

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  
  assert((mode == ISTHMUS) || (mode == ENDS));

  if (inhibit != NULL)
  {
    uint8_t *I;
    COMPARE_SIZE(image, inhibit);
    ACCEPTED_TYPES1(inhibit, VFF_TYP_1_BYTE);
    I = UCHARDATA(inhibit);
    for (x = 0; x < N; x++) if (I[x]) SET_CONSTRAINT(F[x]);
  }

  mctopo3d_init_topo3d();  

  if (nsteps == -1) nsteps = 1000000000;

  for (k = 1; k < ds-1; k++) 
  for (j = 1; j < cs-1; j++) 
  for (i = 1; i < rs-1; i++) 
  {
    x = k*ps + j*rs + i;
    if (F[x] && ((k<2)||(j<2)||(i<2)||(k>ds-3)||(j>cs-3)||(i>rs-3))) 
      printf("%s: WARNING - points on extended border may not be treated\n", F_NAME);
  }

  for (x = 0; x < N; x++) if (F[x]) F[x] = S_OBJECT;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  nbiter = 0;
  stab = 0;
  while (!stab && (nbiter < nsteps))
  {
    nbiter++;
#ifdef VERBOSE
    printf("%s: nbiter %d\n", F_NAME, nbiter);
#endif
    stab = 1;

    // détection des isthmes 1D et des points simples
    for (x = 0; x < N; x++) 
      if (F[x])
      { 
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (mode != ISTHMUS)
	{
	  if (mctopo3d_nbvoiso26(F, x, rs, ps, N) == 1)  SET_CONSTRAINT(F[x]);
	}
	else
	{
	  if (t > 1) SET_CONSTRAINT(F[x]);
	}
	if ((t == 1) && (tb == 1)) SET_SIMPLE(F[x]);
      }

    lskelCKSC3_process_cliques(image);

    for (x = 0; x < N; x++)
      if (IS_SIMPLE(F[x]))
      {
	if (F[x] == (S_OBJECT|S_SIMPLE)) // pas marqué
	{
	  F[x] = 0;
	  stab = 0;
	}
	else
	{
	  UNSET_SIMPLE(F[x]);
	  UNSET_PSEL(F[x]);
	  UNSET_SELECTED(F[x]);
	}
      }

  } // while (!stab && (nbiter < nsteps))

  for (x = 0; x < N; x++) if (F[x]) F[x] = NDG_MAX;

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  mctopo3d_termine_topo3d();
  return(1);
} /* lskelACK3a() */

