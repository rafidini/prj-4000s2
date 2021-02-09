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
/* \file mcskel2d.c
    \brief algorithmes pour la squelettisation parallèle en 2D - 
           version efficace utilisant mckhalimsky2d
    \author Michel COUPRIE, novembre 2004, d'après Gilles Bertrand
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mckhalimsky2d.h>
#include "mcskel2d.h"

//#define DEBUG
//#define VERBOSE

// table des configurations de faces essentielles pour les singletons de Khalimsky
// générée par GenerateEssTab() dans mcsquel2d.c
unsigned char EssTab[256] = {
  1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
}; // EssTab[]

/* ========================================== */
/*! \fn int K2_CheckFrame(struct xvimage *k)
    \param k : un complexe
    \return : booléen
    \brief teste si le bord est vide (0)
*/
boolean K2_CheckFrame(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_CheckFrame"
  index_t x, y, rs = rowsize(k), cs = colsize(k);
  unsigned char *K = UCHARDATA(k);

  for (x = 0; x < rs; x++)     if (K[x]) return 0;
  for (x = 0; x < rs; x++)     if (K[(cs - 1) * rs + x]) return 0;
  for (y = 1; y < cs - 1; y++) if (K[y * rs]) return 0;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 1]) return 0;
  return 1;
} // K2_CheckFrame()

/* ========================================== */
/*! \fn int K2_CheckComplex(struct xvimage *k)
    \param k : un sous-ensemble de H^2
    \return : booléen
    \brief teste si k est un complexe
*/
boolean K2_CheckComplex(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_CheckComplex"
  index_t x, y, rs = rowsize(k), cs = colsize(k);
  unsigned char *K = UCHARDATA(k);
  index_t tab[8], u;
  int32_t n;
  
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
    if (K[y*rs + x])
    {
      Alphacarre2d(rs, cs, x, y, tab, &n);
      for (u = 0; u < n; u++)
	if (K[tab[u]] == 0) return 0;
    }
#ifdef VERBOSE
  printf("%s: complex OK\n", F_NAME);  
#endif
  return 1;
} // K2_CheckComplex()

/* ========================================== */
/*! \fn void K2_MarkObj(struct xvimage *k)
    \param k : un complexe
    \brief marque FLAG_OBJ toutes les faces du complexe k
*/
void K2_MarkObj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkObj"
  index_t i, N = rowsize(k) * colsize(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i]) K[i] = FLAG_OBJ; else K[i] = 0;
} // K2_MarkObj()

/* ========================================== */
/*! \fn void K2_Binarize(struct xvimage *k)
    \param k : un complexe
    \brief marque NDG_MAX toutes les faces du complexe k
*/
void K2_Binarize(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_Binarize"
  index_t i, N = rowsize(k) * colsize(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i]) K[i] = NDG_MAX; else K[i] = 0;
} // K2_Binarize()

/* ========================================== */
/*! \fn void K2_SelMarked(struct xvimage *k, unsigned char mask)
    \param k : un complexe
    \param mask : une marque (mot 8 bits avec un seul bit à 1)
    \brief met a FLAG_OBJ tous le éléments marqués, à 0 les autres
*/
void K2_SelMarked(struct xvimage *k, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_SelMarked"
  index_t i, N = rowsize(k) * colsize(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i] & mask) K[i] = FLAG_OBJ; else K[i] = 0;
} // K2_SelMarked()

/* ========================================== */
/*! \fn void K2_MarkAlphaCarre(struct xvimage *k, int f, unsigned char mask)
    \param k : un complexe
    \param f : une face
    \param mask : valeur de la marque (mot 8 bits avec un seul bit à 1)
    \brief ajoute la marque mask à tous les éléments de alphacarre(f)
*/
void K2_MarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkAlphaCarre"
  index_t rs = rowsize(k), cs = colsize(k), x = f%rs, y = f/rs;
  index_t tab[8], u;
  int32_t n;
  unsigned char *K = UCHARDATA(k);
  Alphacarre2d(rs, cs, x, y, tab, &n);
  for (u = 0; u < n; u++) K[tab[u]] |= mask;
} // K2_MarkAlphaCarre()

/* ========================================== */
/*! \fn void K2_UnMarkAlphaCarre(struct xvimage *k, int f, unsigned char mask)
    \param k : un complexe
    \param f : une face
    \param mask : valeur de la marque (mot 8 bits avec un seul bit à 1)
    \brief retire la marque mask de tous les éléments de alphacarre(f)
*/
void K2_UnMarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_UnMarkAlphaCarre"
  index_t rs = rowsize(k), cs = colsize(k), x = f%rs, y = f/rs;
  index_t tab[8], u;
  int32_t n;
  unsigned char *K = UCHARDATA(k);
  Alphacarre2d(rs, cs, x, y, tab, &n);
  for (u = 0; u < n; u++) K[tab[u]] &= ~mask;
} // K2_UnMarkAlphaCarre()

/* ========================================== */
/*! \fn void K2_MarkPrinc(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque face principale f de k, rajoute (OR) la marque FLAG_PRINC au label de f
*/
void K2_MarkPrinc(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkPrinc"
  index_t card, i, x, y, rs = rowsize(k), cs = colsize(k);
  unsigned char *K = UCHARDATA(k);
  index_t tab[8], u;
  int32_t n;
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (IS_OBJ(K[i])) 
    {
      Betacarre2d(rs, cs, x, y, tab, &n);
      card = 0;
      for (u = 0; u < n; u++) if (IS_OBJ(K[tab[u]])) card++;
      if (card == 0) K[i] |= FLAG_PRINC;
    }
  }
} // K2_MarkPrinc()

/* ========================================== */
/*! \fn void K2_MarkEss(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque face essentielle f de k, rajoute (OR) la marque FLAG_ESS au label de f
    \warning les faces principales doivent avoir été marquées auparavant
*/
void K2_MarkEss(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkEss"
  index_t x, y, i, rs = rowsize(k), cs = colsize(k);
  unsigned char *K = UCHARDATA(k);
  index_t tab[8], u;
  int32_t n;
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (IS_OBJ(K[i])) 
    {
      if (CARRE(x,y)) 
      { 
	if (IS_PRINC(K[i])) K[i] |= FLAG_ESS; 
      }
      else if (INTER(x,y)) 
      {
	index_t card = 0;	
	Betacarre2d(rs, cs, x, y, tab, &n);
	for (u = 0; u < n; u++) if (IS_PRINC(K[tab[u]])) card++;
	if (IS_PRINC(K[i]) && ((card == 0) || (card == 2))) K[i] |= FLAG_ESS; 
	if (!IS_PRINC(K[i]) && (card == 2)) K[i] |= FLAG_ESS; 
      }
      else // SINGL
      {
	index_t mask = 0, bitmask = 1;
	Betacarre2d(rs, cs, x, y, tab, &n);
	for (u = 0; u < n; u++) 
	{
	  if (IS_PRINC(K[tab[u]])) mask |= bitmask;
	  bitmask = bitmask << 1;
	}    
	if (EssTab[mask]) K[i] |= FLAG_ESS;
      }
    }
  }
} // K2_MarkEss()

/* ========================================== */
/*! \fn int K2_MarkCore(struct xvimage *k, int f)
    \param k : un complexe
    \param f : une face de k
    \return le cardinal (nombre de faces) de la fermeture du noyau
    \brief marque le complexe formé du noyau (core) de la face f pour le complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant
*/
int K2_MarkCore(struct xvimage *k, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkCore"
  index_t rs = rowsize(k), cs = colsize(k), nf = 0;
  unsigned char *K = UCHARDATA(k);
  index_t tab[8], tabi[8], u, v, i, j;
  int32_t n, ni;
  index_t x = f % rs, y = f / rs, xi, yi;

  Alphacarre2d(rs, cs, x, y, tab, &n);
  for (u = 0; u < n; u++) 
  {
    i = tab[u];
    if (IS_ESS(K[i]))
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
#ifdef DEBUG
printf("mark_core : %d,%d\n", i%rs, i/rs);
#endif
      } 
      xi = i % rs, yi = i / rs;
      Alphacarre2d(rs, cs, xi, yi, tabi, &ni);
      for (v = 0; v < ni; v++) 
      {
	j = tabi[v];
#ifdef DEBUG
printf("PRE_mark_core_aux : %d,%d [%x]\n", j%rs, j/rs, K[j]);
#endif
	if (!(IS_CORE(K[j]))) 
	{ 
	  K[j] |= FLAG_CORE; nf++; 
#ifdef DEBUG
printf("mark_core_aux : %d,%d\n", j%rs, j/rs);
#endif
	} 
      }
    }
  }
  return nf;
} // K2_MarkCore()

/* ========================================== */
/*! \fn int K2_MarkCore2(struct xvimage *k, struct xvimage *m, index_t f)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \param f : une face de k
    \return le cardinal (nombre de faces) de la fermeture du noyau de f pour k, conditionné par m
    \brief marque le complexe formé du noyau (core) de la face f pour le complexe k, conditionné par m
    (m est un ensemble de faces qui doivent être préservées)
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
int K2_MarkCore2(struct xvimage *k, struct xvimage *m, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkCore2"
  index_t rs = rowsize(k), cs = colsize(k), nf = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t tab[8], tabi[8], u, v, i, j;
  int32_t n, ni;
  index_t x = f % rs, y = f / rs, xi, yi;

  Alphacarre2d(rs, cs, x, y, tab, &n);
  for (u = 0; u < n; u++) 
  {
    i = tab[u];
    xi = i % rs; yi = i / rs;
    if (M[i])
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
	Alphacarre2d(rs, cs, xi, yi, tabi, &ni);
	for (v = 0; v < ni; v++) 
        {
	  j = tabi[v];
	  if (!(IS_CORE(K[j]))) 
	  { 
	    K[j] |= FLAG_CORE; nf++; 
	  } 
	}

      } 
    }
    else if (IS_ESS(K[i]))
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
      } 
      Alphacarre2d(rs, cs, xi, yi, tabi, &ni);
      for (v = 0; v < ni; v++) 
      {
	j = tabi[v];
	if (!(IS_CORE(K[j]))) 
	{ 
	  K[j] |= FLAG_CORE; nf++; 
	} 
      }
    }
  }
  return nf;
} // K2_MarkCore2()

/* ========================================== */
/*! \fn int K2_CardCore(struct xvimage *k, int f)
    \param k : un complexe
    \param f : une face de k
    \return un entier 
    \brief retourne le cardinal du noyau de f, i.e., le nombre de faces marquées MARK2_ESS du noyau 
    \warning les faces essentielles doivent avoir été marquées auparavant
*/
int K2_CardCore(struct xvimage *k, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_CardCore"
  index_t rs = rowsize(k), cs = colsize(k), nf = 0;
  unsigned char *K = UCHARDATA(k);
  index_t tab[8], u;
  index_t x = f % rs, y = f / rs;
  int32_t n;

  Alphacarre2d(rs, cs, x, y, tab, &n);
  for (u = 0; u < n; u++) if (IS_ESS(K[tab[u]])) nf++; 
  return nf;
} // K2_CardCore()

/* ========================================== */
/*! \fn int K2_CardCore2(struct xvimage *k, struct xvimage *m, index_t f)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \param f : une face de k
    \return un entier 
    \brief retourne le cardinal du noyau de f pour k conditionné par m, i.e.,
    le nombre de faces marquées MARK2_ESS ou appartenant à m dans le noyau 
    (m est un ensemble de faces qui doivent être préservées)
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
int K2_CardCore2(struct xvimage *k, struct xvimage *m, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_CardCore2"
  index_t rs = rowsize(k), cs = colsize(k), nf = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t tab[8], u;
  index_t x = f % rs, y = f / rs;
  int32_t n;
  
  Alphacarre2d(rs, cs, x, y, tab, &n);
  for (u = 0; u < n; u++) if (IS_ESS(K[tab[u]]) || M[tab[u]]) nf++; 
  return nf;
} // K2_CardCore2()

/* ========================================== */
/*! \fn void K2_MarkRegul(struct xvimage *k)
    \param k : un complexe
    \brief marque les faces régulières du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant
    \warning les points de bord ne sont pas testés
*/
void K2_MarkRegul(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkRegul"
  index_t x, y, i, rs = rowsize(k), cs = colsize(k);
  unsigned char *K = UCHARDATA(k);
  index_t n, ncore, ntrans;

  for (y = 1; y < cs-1; y++)
  for (x = 1; x < rs-1; x++)
  {
    if (SINGL(x,y)) n = 0; else if (CARRE(x,y)) n = 8; else n = 2; 
    i = y*rs + x;
    if (IS_ESS(K[i])) 
    {
      ncore = K2_MarkCore(k, i);
#ifdef DEBUG
printf("ess : %d,%d ; n = %d, ncore = %d\n", x, y, n, ncore);
#endif
      if (ncore == 0) goto next;
      if (ncore == n) { K2_UnMarkAlphaCarre(k, i, FLAG_CORE); goto next; }
      //if (SINGL(x,y)) goto next; // inutile car le core d'un singl est forcément vide
      if (INTERH(x,y) || INTERV(x,y))
      {
	if (ncore == 1) K[i] |= FLAG_REGUL;
	K2_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      //if (CARRE(x,y)) // inutile car c'est le seul choix qui reste
      ntrans = 0;
      if ((K[(y)  *rs + x+1] & FLAG_CORE) != (K[(y-1)*rs + x+1] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x+1] & FLAG_CORE) != (K[(y-1)*rs + x  ] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x  ] & FLAG_CORE) != (K[(y-1)*rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x-1] & FLAG_CORE) != (K[(y)  *rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y)  *rs + x-1] & FLAG_CORE) != (K[(y+1)*rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x-1] & FLAG_CORE) != (K[(y+1)*rs + x  ] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x  ] & FLAG_CORE) != (K[(y+1)*rs + x+1] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x+1] & FLAG_CORE) != (K[(y)  *rs + x+1] & FLAG_CORE)) ntrans++;
      if (ntrans == 2) K[i] |= FLAG_REGUL;
      K2_UnMarkAlphaCarre(k, i, FLAG_CORE);
    next:;
    }
  }
} // K2_MarkRegul()

/* ========================================== */
/*! \fn void K2_MarkRegul2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief marque les faces régulières du complexe k conditionné par le complexe m
    (m est un ensemble de faces qui doivent être préservées)
    \warning les faces essentielles doivent avoir été marquées auparavant
    \warning les points de bord ne sont pas testés
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkRegul2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkRegul2"
  index_t x, y, i, rs = rowsize(k), cs = colsize(k);
  unsigned char *K = UCHARDATA(k);
  index_t n, ncore, ntrans;

  for (y = 1; y < cs-1; y++)
  for (x = 1; x < rs-1; x++)
  {
    if (SINGL(x,y)) n = 0; else if (CARRE(x,y)) n = 8; else n = 2; 
    i = y*rs + x;
    if (IS_ESS(K[i])) 
    {
      ncore = K2_MarkCore2(k, m, i);
      if (ncore == 0) goto next;
      if (ncore == n) { K2_UnMarkAlphaCarre(k, i, FLAG_CORE); goto next; }
      //if (SINGL(x,y)) goto next; // inutile car le core d'un singl est forcément vide
      if (INTERH(x,y) || INTERV(x,y))
      {
	if (ncore == 1) K[i] |= FLAG_REGUL;
	K2_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      //if (CARRE(x,y)) // inutile car c'est le seul choix qui reste
      ntrans = 0;
      if ((K[(y)  *rs + x+1] & FLAG_CORE) != (K[(y-1)*rs + x+1] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x+1] & FLAG_CORE) != (K[(y-1)*rs + x  ] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x  ] & FLAG_CORE) != (K[(y-1)*rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x-1] & FLAG_CORE) != (K[(y)  *rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y)  *rs + x-1] & FLAG_CORE) != (K[(y+1)*rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x-1] & FLAG_CORE) != (K[(y+1)*rs + x  ] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x  ] & FLAG_CORE) != (K[(y+1)*rs + x+1] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x+1] & FLAG_CORE) != (K[(y)  *rs + x+1] & FLAG_CORE)) ntrans++;
      if (ntrans == 2) K[i] |= FLAG_REGUL;
      K2_UnMarkAlphaCarre(k, i, FLAG_CORE);
    next:;
    }
  }
} // K2_MarkRegul2()

/* ========================================== */
/*! \fn void K2_MarkCritic(struct xvimage *k)
    \param k : un complexe
    \brief marque FLAG_CRITIC les fermetures des faces critiques du complexe k, 
           et marque FLAG_CRITIC_T les faces critiques du complexe k
    \warning les faces essentielles et les faces régulières doivent avoir été marquées auparavant
*/
void K2_MarkCritic(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkCritic"
  index_t i, x, y, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) 
    if (IS_ESS(K[i]) && !IS_REGUL(K[i]))
    {
      K[i] |= FLAG_CRITIC;
      K[i] |= FLAG_CRITIC_T;
      K2_MarkAlphaCarre(k, i, FLAG_CRITIC);
    }
  for (x = 0; x < rs; x++)     if (K[x]) K[x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[x+rs]) K[x+rs] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 1) * rs + x]) K[(cs - 1) * rs + x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 2) * rs + x]) K[(cs - 2) * rs + x] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs]) K[y * rs] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + 1]) K[y * rs + 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 1]) K[y * rs + rs - 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 2]) K[y * rs + rs - 2] |= FLAG_CRITIC;
} // K2_MarkCritic()

/* ========================================== */
/*! \fn void K2_MarkCritic1(struct xvimage *k)
    \param k : un complexe
    \brief marque FLAG_CRITIC les faces critiques du complexe k
    \warning les faces essentielles et les faces régulières doivent avoir été marquées auparavant
*/
void K2_MarkCritic1(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkCritic1"
  index_t i, x, y, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) 
    if (IS_ESS(K[i]) && !IS_REGUL(K[i])) 
    {
      K[i] |= FLAG_CRITIC;
    }
  for (x = 0; x < rs; x++)     if (K[x]) K[x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[x+rs]) K[x+rs] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 1) * rs + x]) K[(cs - 1) * rs + x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 2) * rs + x]) K[(cs - 2) * rs + x] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs]) K[y * rs] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + 1]) K[y * rs + 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 1]) K[y * rs + rs - 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 2]) K[y * rs + rs - 2] |= FLAG_CRITIC;
} // K2_MarkCritic1()

/* ========================================== */
/*! \fn void K2_MarkCritic2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief marque FLAG_CRITIC les fermetures des faces critiques du complexe k, conditionné par m, et 
           marque FLAG_CRITIC_T les faces critiques du complexe k (cond. m)
    (m est un ensemble de faces qui doivent être préservées)
    \warning les faces essentielles et les faces régulières (conditionnellement à m) 
    doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkCritic2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkCritic2"
  index_t i, x, y, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  for (i = 0; i < N; i++) 
    if (M[i] || (IS_ESS(K[i]) && !IS_REGUL(K[i])))
    {
      K[i] |= FLAG_CRITIC;
      K[i] |= FLAG_CRITIC_T;
      K2_MarkAlphaCarre(k, i, FLAG_CRITIC);
    }
  for (x = 0; x < rs; x++)     if (K[x]) K[x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[x+rs]) K[x+rs] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 1) * rs + x]) K[(cs - 1) * rs + x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 2) * rs + x]) K[(cs - 2) * rs + x] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs]) K[y * rs] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + 1]) K[y * rs + 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 1]) K[y * rs + rs - 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 2]) K[y * rs + rs - 2] |= FLAG_CRITIC;
} // K2_MarkCritic2()

/* ========================================== */
/*! \fn void K2_MarkCritic3(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief marque FLAG_CRITIC les faces critiques du complexe k, conditionné par m
    (m est un ensemble de faces qui doivent être préservées)
    \warning les faces essentielles et les faces régulières (conditionnellement à m) 
    doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkCritic3(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkCritic3"
  index_t i, x, y, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  for (i = 0; i < N; i++) 
    if (M[i] || (IS_ESS(K[i]) && !IS_REGUL(K[i])))
    {
      K[i] |= FLAG_CRITIC;
    }
  for (x = 0; x < rs; x++)     if (K[x]) K[x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[x+rs]) K[x+rs] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 1) * rs + x]) K[(cs - 1) * rs + x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 2) * rs + x]) K[(cs - 2) * rs + x] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs]) K[y * rs] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + 1]) K[y * rs + 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 1]) K[y * rs + rs - 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 2]) K[y * rs + rs - 2] |= FLAG_CRITIC;
} // K2_MarkCritic3()

/* ========================================== */
/*! \fn void K2_MarkMCritic(struct xvimage *k)
    \param k : un complexe 
    \brief marque FLAG_CRITIC les faces m-critiques du complexe k
    \warning les faces essentielles et les faces régulières doivent avoir été marquées auparavant
*/
void K2_MarkMCritic(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkMCritic"
  index_t i, x, y, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  index_t tab[8], u;
  int32_t n;
  for (i = 0; i < N; i++) 
    if (IS_ESS(K[i]) && !IS_REGUL(K[i])) 
    {
      K[i] |= FLAG_CRITIC;
    }
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    Betacarre2d(rs, cs, i%rs, i/rs, tab, &n);
    for (u = 0; u < n; u++) if (IS_CRITIC(K[tab[u]])) break;	
    if (u == n) K[i] |= FLAG_TMP;
  }
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    if (!IS_TMP(K[i])) K[i] &= ~FLAG_CRITIC;
    K[i] &= ~FLAG_TMP;
  }
  for (x = 0; x < rs; x++)     if (K[x]) K[x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[x+rs]) K[x+rs] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 1) * rs + x]) K[(cs - 1) * rs + x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 2) * rs + x]) K[(cs - 2) * rs + x] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs]) K[y * rs] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + 1]) K[y * rs + 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 1]) K[y * rs + rs - 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 2]) K[y * rs + rs - 2] |= FLAG_CRITIC;
} // K2_MarkMCritic()

/* ========================================== */
/*! \fn void K2_MarkMCritic2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe 
    \param m : un complexe "marqueur"
    \brief marque FLAG_CRITIC les faces M-critiques du complexe k, conditionné par m
    (m est un ensemble de faces qui doivent être préservées)
    \warning les faces essentielles et les faces régulières (conditionnellement à m)  
    doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkMCritic2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkMCritic2"
  index_t i, x, y, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t tab[8], u;
  int32_t n;
  for (i = 0; i < N; i++) 
    if (M[i] || (IS_ESS(K[i]) && !IS_REGUL(K[i])))
    {
      K[i] |= FLAG_CRITIC;
    }
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    Betacarre2d(rs, cs, i%rs, i/rs, tab, &n);
    for (u = 0; u < n; u++) if (IS_CRITIC(K[tab[u]])) break;	
    if (u == n) K[i] |= FLAG_TMP;
  }
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    if (!IS_TMP(K[i])) K[i] &= ~FLAG_CRITIC;
    K[i] &= ~FLAG_TMP;
  }
  for (x = 0; x < rs; x++)     if (K[x]) K[x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[x+rs]) K[x+rs] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 1) * rs + x]) K[(cs - 1) * rs + x] |= FLAG_CRITIC;
  for (x = 0; x < rs; x++)     if (K[(cs - 2) * rs + x]) K[(cs - 2) * rs + x] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs]) K[y * rs] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + 1]) K[y * rs + 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 1]) K[y * rs + rs - 1] |= FLAG_CRITIC;
  for (y = 1; y < cs - 1; y++) if (K[y * rs + rs - 2]) K[y * rs + rs - 2] |= FLAG_CRITIC;
} // K2_MarkMCritic2()

/* ========================================== */
/*! \fn int K2_Critic2Obj(struct xvimage *k)
    \param k : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les faces marquées FLAG_CRITIC du complexe k et remet à 0 les autres marques
*/
int K2_Critic2Obj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_Critic2Obj"
  index_t i, N = rowsize(k) * colsize(k), n = 0;
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) 
    if (IS_CRITIC(K[i]))
    {
      K[i] = FLAG_OBJ;
      n++;
    }
    else
      K[i] = 0;
  return n;
} // K2_Critic2Obj()

/* ========================================== */
/*! \fn int K2_MCritic2Obj(struct xvimage *k)
    \param k : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les fermetures des faces non nulles 
    contenant des faces m-critiques (marquées FLAG_CRITIC) 
    du complexe k et remet à 0 les autres marques
*/
int K2_MCritic2Obj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MCritic2Obj"
  index_t i, j, x, y, rs = rowsize(k), cs = colsize(k), N = rs * cs, n = 0;
  unsigned char *K = UCHARDATA(k);
  index_t tab1[8], tab2[8], u, v;
  int32_t n1, n2;
  for (i = 0; i < N; i++) K[i] &= ~FLAG_OBJ;
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (IS_CRITIC(K[i])) 
    {
      K[i] |= FLAG_OBJ;
      Betacarre2d(rs, cs, x, y, tab1, &n1);
      for (u = 0; u < n1; u++) 
      {
	j = tab1[u];
	if (K[j])
	{
	  K[j] |= FLAG_OBJ;
	  Alphacarre2d(rs, cs, j%rs, j/rs, tab2, &n2);
	  for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
	}
      }
      Alphacarre2d(rs, cs, x, y, tab2, &n2);
      for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
    }
  }

  for (i = 0; i < N; i++) 
    if (IS_OBJ(K[i])) { n++; K[i] = FLAG_OBJ; } else K[i] = 0;
  return n;
} // K2_MCritic2Obj()

/* ========================================== */
/*! \fn int K2_MCriticSE2Obj(struct xvimage *k)
    \param k : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les fermetures des faces non nulles 
    contenant des faces m-critiques (marquées FLAG_CRITIC)
    en position S, E pour les 1-faces et SE, SW pour les 0-faces 
    du complexe k et remet à 0 les autres marques
*/
int K2_MCriticSE2Obj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MCriticSE2Obj"
  index_t i, j, x, y, xj, yj, rs = rowsize(k), cs = colsize(k), N = rs * cs, n = 0;
  unsigned char *K = UCHARDATA(k);
  index_t tab1[8], tab2[8], u, v;
  int32_t n1, n2;
  for (i = 0; i < N; i++) K[i] &= ~FLAG_OBJ;
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (IS_CRITIC(K[i])) 
    {
printf("Critic : %d (%d %d)\n", i, x, y);
      K[i] |= FLAG_OBJ;
      Betacarre2d(rs, cs, x, y, tab1, &n1);
      for (u = 0; u < n1; u++) 
      {
	j = tab1[u];
	xj = j % rs;
	yj = j / rs;
	if (CARRE(xj,yj) && K[j])
	{
printf("  j : %d (%d %d) ih:%d iv:%d s:%d\n", j, xj, yj, INTERH(x,y), INTERV(x,y), SINGL(x,y));
	  if ((INTERH(x,y) && (xj == x) && (yj == y-1)) ||    // S
	      (INTERV(x,y) && (xj == x-1) && (yj == y)) ||    // E
	      (SINGL(x,y) && (xj == x-1) && (yj == y-1)) ||   // SE
	      (SINGL(x,y) && (xj == x+1) && (yj == y-1))      // SW
	     )
	  {
	    K[j] |= FLAG_OBJ;
	    Alphacarre2d(rs, cs, j%rs, j/rs, tab2, &n2);
	    for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
	  }
	}
      }
      Alphacarre2d(rs, cs, x, y, tab2, &n2);
      for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
    }
  }

  for (i = 0; i < N; i++) 
    if (IS_OBJ(K[i])) { n++; K[i] = FLAG_OBJ; } else K[i] = 0;
  return n;
} // K2_MCriticSE2Obj()

/* ========================================== */
/*! \fn int K2_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les fermetures des faces 
    contenant des faces m-critiques (marquées FLAG_CRITIC)
    du complexe k ou des faces de m et remet à 0 les autres marques.
*/
int K2_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MCriticOrMarked2Obj"
  index_t i, j, x, y, rs = rowsize(k), cs = colsize(k), N = rs * cs, n = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t tab1[8], tab2[8], u, v;
  int32_t n1, n2;
  for (i = 0; i < N; i++) K[i] &= ~FLAG_OBJ;
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (M[i] || IS_CRITIC(K[i])) 
    {
      K[i] |= FLAG_OBJ;
      Betacarre2d(rs, cs, x, y, tab1, &n1);
      for (u = 0; u < n1; u++) 
      {
	j = tab1[u];
	K[j] |= FLAG_OBJ;
	Alphacarre2d(rs, cs, j%rs, j/rs, tab2, &n2);
	for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
      }
      Alphacarre2d(rs, cs, x, y, tab2, &n2);
      for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
    }
  }

  for (i = 0; i < N; i++) 
    if (IS_OBJ(K[i])) { n++; K[i] = FLAG_OBJ; } else K[i] = 0;
  return n;
} // K2_MCriticOrMarked2Obj()

/* ========================================== */
/*! \fn void K2_MarkThin(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \brief ajoute au complexe m les faces séparantes (Tb > 1) du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkThin(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkThin"
  index_t x, y, i, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t n, ncore, ntrans;

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    if (SINGL(x,y)) n = 0; else if (CARRE(x,y)) n = 8; else n = 2; 
    i = y*rs + x;
    if (IS_ESS(K[i])) 
    {
      if (SINGL(x,y)) goto next;
      if (INTERH(x,y) || INTERV(x,y))
      {
	if (IS_PRINC(K[i]))
	{
	  K[i] |= FLAG_TMP;
	  K2_MarkAlphaCarre(k, i, FLAG_TMP);
	}
	goto next;
      }
      //if (CARRE(x,y)) // inutile car c'est le seul choix qui reste
      ncore = K2_MarkCore(k, i);
#ifdef DEBUG
printf("ess : %d,%d ; n = %d, ncore = %d\n", x, y, n, ncore);
#endif
      if (ncore == 0) goto next;
      if (ncore == n) { K2_UnMarkAlphaCarre(k, i, FLAG_CORE); goto next; }
      ntrans = 0;
      if ((K[(y)  *rs + x+1] & FLAG_CORE) != (K[(y-1)*rs + x+1] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x+1] & FLAG_CORE) != (K[(y-1)*rs + x  ] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x  ] & FLAG_CORE) != (K[(y-1)*rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y-1)*rs + x-1] & FLAG_CORE) != (K[(y)  *rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y)  *rs + x-1] & FLAG_CORE) != (K[(y+1)*rs + x-1] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x-1] & FLAG_CORE) != (K[(y+1)*rs + x  ] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x  ] & FLAG_CORE) != (K[(y+1)*rs + x+1] & FLAG_CORE)) ntrans++;
      if ((K[(y+1)*rs + x+1] & FLAG_CORE) != (K[(y)  *rs + x+1] & FLAG_CORE)) ntrans++;
      if (ntrans != 2) 
      {
	K[i] |= FLAG_TMP;
	K2_MarkAlphaCarre(k, i, FLAG_TMP);
      }
      K2_UnMarkAlphaCarre(k, i, FLAG_CORE);
    next:;
    }
  }
  for (i = 0; i < N; i++) if (IS_TMP(K[i])) M[i] = 1;
} // K2_MarkThin()

/* ========================================== */
/*! \fn int K2_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \return le nombre de faces critiques ou marquées trouvées
    \brief marque FLAG_OBJ les faces critiques du complexe k ou marquées par m et remet à 0 les flags dans k
*/
int K2_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_CriticOrMarked2Obj"
  index_t i, N = rowsize(k) * colsize(k), nobj = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  for (i = 0; i < N; i++) 
    if (M[i] || IS_CRITIC(K[i])) { K[i] = FLAG_OBJ; nobj++; } else K[i] = 0;
  return nobj;
} // K2_CriticOrMarked2Obj()

/* ========================================== */
/*! \fn void K2_HitPrinc(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque face f marquée FLAG_PRINC de k, marque FLAG_OBJ le alpha du beta de f
*/
void K2_HitPrinc(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_HitPrinc"
  index_t i, j, x, y, rs = rowsize(k), cs = colsize(k);
  unsigned char *K = UCHARDATA(k);
  index_t tab1[8], tab2[8], u, v;
  int32_t n1, n2;
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (IS_PRINC(K[i])) 
    {
      K[i] |= FLAG_OBJ;
      Betacarre2d(rs, cs, x, y, tab1, &n1);
      for (u = 0; u < n1; u++) 
      {
	j = tab1[u];
	K[j] |= FLAG_OBJ;
	Alphacarre2d(rs, cs, j%rs, j/rs, tab2, &n2);
	for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
      }
      Alphacarre2d(rs, cs, x, y, tab2, &n2);
      for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
    }
  }
} // K2_HitPrinc()

/* ========================================== */
/*! \fn void K2_MarkEnd(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \brief ajoute au complexe m les faces extrémités du complexe k
    \warning les faces essentielles et régulières doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkEnd(struct xvimage *k, struct xvimage *m)
/* ========================================== */
// une face "CARRE" est extrémité si 
//   i) elle est régulière et 
//   ii) le cardinal de son core est 1 ou 2 ou 3
// une face "INTER" est extrémité si 
//   i) elle est régulière et  
//   ii) elle est sous deux CARRES qui sont dans k et
//   iii) ces carrés sont réguliers et 
//   iv) le cardinal du core de ces deux CARRES est de 3
{
#undef F_NAME
#define F_NAME "K2_MarkEnd"
  index_t x, y, i, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t ncore, tab[8], u, card;
  int32_t n;

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (IS_REGUL(K[i])) 
    {
      if (SINGL(x,y)) goto next;
      if (INTERH(x,y) || INTERV(x,y))
      {
	Betacarre2d(rs, cs, x, y, tab, &n);
	card = 0;
	for (u = 0; u < n; u++) if (IS_OBJ(K[tab[u]])) card++;
	if (card != 2) goto next;
        for (u = 0; u < n; u++)
	{
	  if (!IS_REGUL(K[tab[u]])) goto next;
	  ncore = K2_CardCore(k, tab[u]);
	  if (ncore != 3) goto next;
	}
	K[i] |= FLAG_TMP;	
      }
      //if (CARRE(x,y)) // inutile car c'est le seul choix qui reste
      ncore = K2_CardCore(k, i);
      if (ncore <= 3) 
      {
	K[i] |= FLAG_TMP;
	//	K2_MarkAlphaCarre(k, i, FLAG_TMP);
      }
    next:;
    }
  }
  for (i = 0; i < N; i++) if (IS_TMP(K[i])) { M[i] = 1; K[i] &= ~FLAG_TMP; }
} // K2_MarkEnd()

/* ========================================== */
/*! \fn void K2_MarkEnd2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \brief ajoute au complexe m les faces extrémités du complexe k
    \warning les faces principales, essentielles et régulières 
    doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkEnd2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
// une face principale, régulière f est extrémité si 
// si f contient exactement une face critique
{
#undef F_NAME
#define F_NAME "K2_MarkEnd2"
  index_t x, y, i, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t ncritic, tab[8], u;
  int32_t n;

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (IS_PRINC(K[i]) && IS_REGUL(K[i]))
    {
      if (SINGL(x,y)) goto next;
      if (INTERH(x,y) || INTERV(x,y))
      {
	Alphacarre2d(rs, cs, x, y, tab, &n);
	ncritic = 0;
        for (u = 0; u < n; u++)
	{
	  if (IS_ESS(K[tab[u]]) && !IS_REGUL(K[tab[u]])) ncritic++;
	}
	if (ncritic != 1) goto next;
	K[i] |= FLAG_TMP;	
      }
      //if (CARRE(x,y)) // inutile car c'est le seul choix qui reste
      Alphacarre2d(rs, cs, x, y, tab, &n);
      ncritic = 0;
      for (u = 0; u < n; u++)
      {
	if (IS_ESS(K[tab[u]]) && !IS_REGUL(K[tab[u]])) ncritic++;
      }
      if (ncritic != 1) goto next;
      K[i] |= FLAG_TMP;
    next:;
    }
  }
  for (i = 0; i < N; i++) if (IS_TMP(K[i])) { M[i] = 1; K[i] &= ~FLAG_TMP; }
} // K2_MarkEnd2()

/* ========================================== */
/*! \fn void K2_MarkSE(struct xvimage *k)
    \param k : un complexe
    \brief ajoute au complexe m les faces SE du complexe k.
    Une face f est dite SE si elle principale et régulière et si elle 
    contient une 1-face m-critique S ou E, ou une 0-face m-critique SE ou SW.
    \warning les faces m-critiques doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkSE(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_MarkSE"
  index_t x, y, i, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  unsigned char vE, vSE, vS, vSW;

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (CARRE(x,y) && IS_PRINC(K[i]) && IS_REGUL(K[i]))
    {
      vE  = K[(y)*rs + x+1];
      vSE = K[(y+1)*rs + x+1];
      vS  = K[(y+1)*rs + x];
      vSW = K[(y+1)*rs + x-1];
      if (IS_CRITIC(vE) || IS_CRITIC(vSE) || IS_CRITIC(vS) || IS_CRITIC(vSW)) 
	K[i] |= FLAG_TMP;
    }
  }
  for (i = 0; i < N; i++) if (IS_TMP(K[i])) { M[i] = 1; K[i] &= ~FLAG_TMP; }
} // K2_MarkSE()

/* ========================================== */
/*! \fn void K2_MarkNotNeighInterior(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \brief ajoute au complexe m les faces carrées non voisines (par un côté) 
    d'au moins une face carrée intérieure du complexe k
    \warning les faces essentielles et régulières doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void K2_MarkNotNeighInterior(struct xvimage *k, struct xvimage *m)
/* ========================================== */
// une face "CARRE" est intérieure le cardinal de son core est 8
{
#undef F_NAME
#define F_NAME "K2_MarkNotNeighInterior"
  index_t x, y, xx, yy, HasNeighInt, i, j, rs = rowsize(k), cs = colsize(k), N = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t ncore;

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (CARRE(x,y)) 
    { 
      ncore = K2_CardCore(k, i);
      if (ncore == 8) K[i] |= FLAG_TMP;
    }
  }

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    j = y*rs + x;
    if (CARRE(x,y) && IS_REGUL(K[j])) 
    {
      HasNeighInt = 0;
      xx = x - 2; yy = y; i = yy*rs + xx;
      if ((xx >= 0) && IS_TMP(K[i])) HasNeighInt = 1; 
      xx = x + 2; yy = y; i = yy*rs + xx;
      if ((xx < rs) && IS_TMP(K[i])) HasNeighInt = 1; 
      xx = x; yy = y - 2; i = yy*rs + xx;
      if ((yy >= 0) && IS_TMP(K[i])) HasNeighInt = 1; 
      xx = x; yy = y + 2; i = yy*rs + xx;
      if ((yy < cs) && IS_TMP(K[i])) HasNeighInt = 1; 
      if (!HasNeighInt) M[j] = 1;
    }
  }

  for (i = 0; i < N; i++) K[i] &= ~FLAG_TMP;
} // K2_MarkNotNeighInterior()

/* ========================================== */
/*! \fn void K2_LabelDifference(struct xvimage *k, struct xvimage *m, struct xvimage *l, int lab)
    \param k,m : deux complexes
    \param m : un complexe "labels"
    \brief pour chaque face dans k et pas dans m,
    met le label "lab" dans l'élément correspondant de "l"
*/
void K2_LabelDifference(struct xvimage *k, struct xvimage *m, struct xvimage *l, index_t lab)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K2_LabelDifference"
  index_t i, rs = rowsize(k), cs = colsize(k), N = rs * cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  unsigned char *L = UCHARDATA(l);
  for (i = 0; i < N; i++) if (K[i] && !M[i]) L[i] = lab;
} // K2_LabelDifference()

/* ==================================================================================== */
/* ==================================================================================== */
/* ==================================================================================== */
/* ALGORITHMES D'AMINCISSEMENT */
/* ==================================================================================== */
/* ==================================================================================== */
/* ==================================================================================== */

/* =============================================================== */
void lskel2d4(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
    \brief Noyau homotopique, version "Z^2 directe" (sans reconstruction) - version efficace basée sur Khalimski 2D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel2d4"
#ifdef DEBUG
  struct xvimage * k2;
#endif
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!K2_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!K2_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  K2_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    K2_MarkPrinc(k);
    K2_MarkEss(k);
    K2_MarkRegul(k);
    K2_MarkMCritic(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

    n_new = K2_MCritic2Obj(k);
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  K2_SelMarked(k, FLAG_OBJ);
  K2_Binarize(k);
  
#ifdef DEBUG
    freeimage(k2);
#endif
} // lskel2d4()

/* =============================================================== */
void lskel2d4b(struct xvimage * k, struct xvimage * m, index_t nsteps) 
/* =============================================================== */
/*
    \brief Noyau homotopique avec contrainte, version "Z^2 directe" (sans reconstruction) - version efficace basée sur Khalimski 2D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel2d4b"
#ifdef DEBUG
  struct xvimage * k2;
#endif
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!K2_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!K2_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  K2_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    K2_MarkPrinc(k);
    K2_MarkEss(k);
    K2_MarkRegul2(k, m);
    K2_MarkMCritic2(k, m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

    n_new = K2_MCritic2Obj(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_OBJ);
    writeimage(k2, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  K2_SelMarked(k, FLAG_OBJ);
  K2_Binarize(k);

#ifdef DEBUG
    freeimage(k2);
#endif
} // lskel2d4b()

/* =============================================================== */
void lskel2d5(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
  \brief Squelette curviligne, version "Z^2" avec détection de points extrémités 
         - version efficace basée sur Khalimski 2D
  \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel2d5"
#ifdef DEBUG
  struct xvimage * k2;
#endif
  struct xvimage * m;
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!K2_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!K2_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);
#ifdef DEBUG
  k2 = copyimage(k);
#endif

  K2_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
#ifdef DEBUG
    copy2image(k2, k);
#endif
    n++;
    n_old = n_new;
    K2_MarkPrinc(k);
    K2_MarkEss(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_ESS);
    writeimage(k2, "_k_ess");
#endif

    K2_MarkRegul2(k, m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_REGUL);
    writeimage(k2, "_k_regul");
#endif

    K2_MarkEnd2(k, m);

#ifdef DEBUG
    k2 = copyimage(m);
    K2_Binarize(k2);
    writeimage(k2, "_k_end");
#endif

    K2_MarkMCritic2(k,m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

    n_new = K2_MCriticOrMarked2Obj(k,m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_OBJ);
    writeimage(k2, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  K2_SelMarked(k, FLAG_OBJ);
  K2_Binarize(k);

#ifdef DEBUG
  freeimage(k2);
#endif

  freeimage(m);
} // lskel2d5()

/* =============================================================== */
void lskel2d7(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
    \brief Squelette curviligne, version "Z^2" avec détection de points extrémités 
    et des points non voisins de points intérieurs - version efficace basée sur Khalimski 2D
    \author Michel COUPRIE, novembre 2004, d'après Gilles Bertrand
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel2d7"
#ifdef DEBUG
  struct xvimage * k2;
#endif
  struct xvimage * m;
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!K2_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!K2_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);

  K2_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    K2_MarkPrinc(k);
    K2_MarkEss(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_ESS);
    writeimage(k2, "_k_ess");
#endif

    K2_MarkRegul2(k, m);

#ifdef DEBUG1
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_REGUL);
    writeimage(k2, "_k_regul");
#endif

    K2_MarkEnd2(k, m);

#ifdef DEBUG
    k2 = copyimage(m);
    K2_Binarize(k2);
    writeimage(k2, "_k_end");
#endif

    K2_MarkNotNeighInterior(k, m);

#ifdef DEBUG
    k2 = copyimage(m);
    K2_Binarize(k2);
    writeimage(k2, "_k_end_nni");
#endif

    K2_MarkMCritic2(k, m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

    n_new = K2_MCriticOrMarked2Obj(k, m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_OBJ);
    writeimage(k2, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  K2_SelMarked(k, FLAG_OBJ);
  K2_Binarize(k);

#ifdef DEBUG
  freeimage(k2);
#endif

  freeimage(m);
} // lskel2d7()

/* =============================================================== */
void lskel2d7b(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
    \brief Squelette curviligne, version "Z^2" avec détection
    des points non voisins de points intérieurs - version efficace basée sur Khalimski 2D
    \author Michel COUPRIE, novembre 2004, d'après Gilles Bertrand
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel2d7b"
#ifdef DEBUG
  struct xvimage * k2;
#endif
  struct xvimage * m;
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!K2_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!K2_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);

  K2_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    K2_MarkPrinc(k);
    K2_MarkEss(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_ESS);
    writeimage(k2, "_k_ess");
#endif

    K2_MarkRegul2(k, m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_REGUL);
    writeimage(k2, "_k_regul");
#endif

    K2_MarkNotNeighInterior(k, m);

#ifdef DEBUG
    k2 = copyimage(m);
    K2_Binarize(k2);
    writeimage(k2, "_k_end_nni");
#endif

    K2_MarkMCritic2(k, m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

    n_new = K2_MCriticOrMarked2Obj(k, m);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_OBJ);
    writeimage(k2, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  K2_SelMarked(k, FLAG_OBJ);
  K2_Binarize(k);

#ifdef DEBUG
  freeimage(k2);
#endif

  freeimage(m);
} // lskel2d7b()

/* =============================================================== */
void lskel2d9(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
  \brief Squelette ultime asymétrique, version "Z^2" 
         - version efficace basée sur Khalimski 2D
  \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel2d9"
#ifdef DEBUG
  struct xvimage * k2;
#endif
  struct xvimage * m;
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!K2_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!K2_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);
#ifdef DEBUG
  k2 = copyimage(k);
#endif

  K2_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
#ifdef DEBUG
    copy2image(k2, k);
#endif
    n++;
    n_old = n_new;
    K2_MarkPrinc(k);
    K2_MarkEss(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_ESS);
    writeimage(k2, "_k_ess");
#endif

    K2_MarkRegul(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_REGUL);
    writeimage(k2, "_k_regul");
#endif

    K2_MarkMCritic(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

    n_new = K2_MCriticSE2Obj(k);

#ifdef DEBUG
    k2 = copyimage(k);
    K2_SelMarked(k2, FLAG_OBJ);
    writeimage(k2, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  K2_SelMarked(k, FLAG_OBJ);
  K2_Binarize(k);

#ifdef DEBUG
  freeimage(k2);
#endif

  freeimage(m);
} // lskel2d9()

/* =============================================================== */
int32_t lskel2d1(struct xvimage * k, int32_t nsteps, struct xvimage * inhi) 
/* =============================================================== */
/*
    \brief Squelette curviligne (dans H2) par sélection et mémorisation
	  des isthmes 1D - version basée sur Khalimski 2D
 */
{
#undef F_NAME
#define F_NAME "lskel2d1"
  struct xvimage * m; // pour marquer les isthmes1D
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!K2_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (inhi == NULL) 
  {
    m = copyimage(k);
    razimage(m);
  }
  else
    m = copyimage(inhi);

  K2_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    K2_MarkPrinc(k);
    K2_MarkEss(k);
    K2_MarkRegul(k);
    K2_MarkThin(k, m);
    K2_MarkCritic2(k, m);
    n_new = K2_CriticOrMarked2Obj(k, m); 
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  K2_SelMarked(k, FLAG_OBJ);
  K2_Binarize(k);

  freeimage(m);
  return(1);
} // lskel2d1()

/* =============================================================== */
int32_t lskel2d0(struct xvimage * k, int32_t nsteps, struct xvimage * inhi) 
/* =============================================================== */
/*
    \brief Squelette ultime (dans H2) - version basée sur Khalimski 2D
 */
{
#undef F_NAME
#define F_NAME "lskel2d0"
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!K2_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }
  K2_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    K2_MarkPrinc(k);
    K2_MarkEss(k);
    K2_MarkRegul(k);
    K2_MarkCritic(k);
    if (inhi != NULL)
      n_new = K2_CriticOrMarked2Obj(k, inhi); 
    else
      n_new = K2_Critic2Obj(k); 
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  K2_SelMarked(k, FLAG_OBJ);
  K2_Binarize(k);

  return(1);
} // lskel2d0()

/* =============================================================== */
int32_t l2dskelck(struct xvimage * k, int32_t mode, int32_t nsteps, struct xvimage * inhi) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "l2dskelck"
/*
\brief Homotopic thinning of a 2d Khalimsky image.
The maximum number of steps is given by <B>niter</B>.
If the value given for \b niter equals -1, the thinning is continued
until stability.
The parameter \b mode specifies the algorithm used for thinning.
Possible choices are:
\li 0: ultimate thinning, symmetrical
\li 1: curve thinning, symmetrical

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

\author Michel Couprie
*/
{

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  switch(mode)
  {
  case 0: 
    return lskel2d0(k, nsteps, inhi);
    break;
  case 1: 
    return lskel2d1(k, nsteps, inhi);
    break;
  default: 
    fprintf(stderr, "%s: bad mode %d\n", F_NAME, mode);
    return(0);
  }
  return(1);
} // l2dskelck()
