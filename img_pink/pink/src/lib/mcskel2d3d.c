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
/* \file mcskel2d3d.c
    \brief algorithmes pour la squelettisation parallèle d'objets 2D en 3D - 
           version efficace utilisant mckhalimsky3d
    \author Michel COUPRIE, décembre 2004, d'après Gilles Bertrand
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mckhalimsky3d.h>
#include "mcskel2d3d.h"

#define DEBUG
#define VERBOSE

#define IS_PRINC(f)  (f&FLAG_PRINC)
#define IS_ESS(f)    (f&FLAG_ESS)
#define IS_CORE(f)   (f&FLAG_CORE)
#define IS_REGUL(f)  (f&FLAG_REGUL)
#define IS_CRITIC(f) (f&FLAG_CRITIC)
#define IS_TMP(f)    (f&FLAG_TMP)
#define IS_INTERIOR(f) (f&FLAG_INTERIOR)
#define IS_OBJ(f)    (f&FLAG_OBJ)

/* ==================================== */
int32_t NotIn(
  index_t e,
  index_t *list,                   
  index_t n)                       
/* ==================================== */
{
/* renvoie 1 si e n'est pas dans list, 0 sinon */
/* e : l'element a rechercher */
/* list : la liste (tableau d'entiers) */
/* n : le nombre d'elements dans la liste */
  while (n > 0)
    if (list[--n] == e) return 0;
  return 1;
} /* NotIn() */

/* ==================================== */
void GammaS3d(index_t rs, index_t cs, index_t ds, index_t x, index_t y, index_t z, index_t *tab, index_t *n)
/* ==================================== */
/* 
  (x,y,z) doit être un carre
  retourne dans tab l'ensemble des carrés fortement adjacents à (x,y,z) 
  (i.e., par un côté)
  *n est le cardinal de cet ensemble
  tab doit etre alloue a l'avance
*/
{
#undef F_NAME
#define F_NAME "GammaS3d"
  index_t ps = rs * cs;
  index_t p1, x1, y1, z1, p2, x2, y2, z2, u1, u2, N;
  index_t t1[26], t2[26];
  int32_t n1, n2;

  if (!CARRE3D(x,y,z))
  {
    fprintf(stderr, "%s : fatal error : bad usage\n", F_NAME);
    exit(0);
  }
  N = 0;

  Alphacarre3d(rs, cs, ds, x, y, z, t1, &n1);
  for (u1 = 0; u1 < n1; u1++) 
  {
    p1 = t1[u1];
    x1 = p1%rs, y1 = (p1%ps)/rs, z1 = p1/ps;
    if (INTER3D(x1,y1,z1))
    {
      Betacarre3d(rs, cs, ds, x1, y1, z1, t2, &n2);
      for (u2 = 0; u2 < n2; u2++) 
      {
	p2 = t2[u2];
	x2 = p2%rs, y2 = (p2%ps)/rs, z2 = p2/ps;
	if (CARRE3D(x2,y2,z2) && NotIn(p2, tab, N))
	{
	  tab[N] = p2;
	  N++;
	}
      }
    }  
  }

  *n = N;
} /* GammaS3d() */

/* ==================================== */
void mcskel2d3d_K3_AlphaBeta3d(index_t rs, index_t cs, index_t ds, index_t i, index_t j, index_t k, index_t x, index_t y, index_t z, index_t *tab, index_t *n)
/* ==================================== */
/* 
  (i,j,k) doit être un singleton et (x,y,z) doit être un carre
  retourne dans tab l'intersection de betacarre(i,j,k) et de alphacarre(x,y,z)
  *n est le cardinal de cet ensemble
  tab doit etre alloue a l'avance
*/
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_AlphaBeta3d"
  index_t ps = rs * cs;
  *n = 0;
  if (!SINGL3D(i,j,k) || !CARRE3D(x,y,z))
  {
    fprintf(stderr, "%s : fatal error : bad usage\n", F_NAME);
    exit(0);
  }
  if (CARRE3DXY(x,y,z))
  {
    tab[(*n)++] = k*ps + j*rs + x;      
    tab[(*n)++] = k*ps + y*rs + i;      
  }    
  else if (CARRE3DYZ(x,y,z))
  {
    tab[(*n)++] = k*ps + y*rs + i;      
    tab[(*n)++] = z*ps + j*rs + i;      
  }    
  else // if (CARRE3DXZ(x,y,z))
  {
    tab[(*n)++] = k*ps + j*rs + x;      
    tab[(*n)++] = z*ps + j*rs + i;      
  }    
} /* mcskel2d3d_K3_AlphaBeta3d() */

/* ==================================== */
void mcskel2d3d_K3_Closure(struct xvimage *k)
/* ==================================== */
/*
  Cette fonction donne le flag FLAG_OBJ aux elements de la fermeture de k
*/
{
  index_t rs = rowsize(k);
  index_t cs = colsize(k);
  index_t ps = rs * cs;
  index_t ds = depth(k);
  unsigned char *K = UCHARDATA(k);
  index_t x, y, z, u;
  int32_t n;
  index_t tab[26];

  for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if (K[z*ps + y*rs + x])
        {
	  K[z*ps + y*rs + x] |= FLAG_OBJ;
          Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
          for (u = 0; u < n; u++) K[tab[u]] |= FLAG_OBJ;
        }
} /* mcskel2d3d_K3_Closure() */

/* ==================================== */
void mcskel2d3d_K3_Squares(struct xvimage *k)
/* ==================================== */
/*
  Cette fonction donne le flag FLAG_OBJ aux elements carres de k (0 aux autres) 
*/
{
  index_t rs = rowsize(k);
  index_t cs = colsize(k);
  index_t ps = rs * cs;
  index_t ds = depth(k);
  unsigned char *K = UCHARDATA(k);
  index_t x, y, z;

  for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if (K[z*ps + y*rs + x] && CARRE3D(x,y,z))
	  K[z*ps + y*rs + x] = FLAG_OBJ;
	else
	  K[z*ps + y*rs + x] = 0;
} /* mcskel2d3d_K3_Squares() */

/* ==================================== */
int32_t mcskel2d3d_K3_IsEmpty(struct xvimage *k)
/* ==================================== */
/*
  Retourne 1 si le complexe k est vide, 0 sinon
*/
{
  index_t N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  index_t i;

  for (i = 0; i < N; i++) if (K[i]) return 0;
  return 1;
} /* mcskel2d3d_K3_IsEmpty() */

/* ==================================== */
int32_t mcskel2d3d_K3_Card(struct xvimage *k)
/* ==================================== */
/*
  Retourne le nombre de faces du complexe k
*/
{
  index_t N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  index_t i, n;
  n = 0;
  for (i = 0; i < N; i++) if (K[i]) n++;
  return n;
} /* mcskel2d3d_K3_Card() */

/* ==================================== */
int32_t mcskel2d3d_K3_IsBorder(struct xvimage *k, index_t f)
/* ==================================== */
/*
  k doit être un complexe (non testé).
  Cette fonction retourne 1 si f est border pour k, 0 sinon.
  On dit que f est border pour k s'il existe une face g dans alphacarre(f) inter k telle que 
  f soit la seule face de k qui contienne g.
*/
{
  index_t rs = rowsize(k);
  index_t cs = colsize(k);
  index_t ps = rs * cs;
  index_t ds = depth(k);
  unsigned char *K = UCHARDATA(k);
  index_t card, x, y, z, g, x1, y1, z1, u1, u2;
  int32_t n1, n2;
  index_t tab1[26], tab2[26];

  if (!K[f]) return 0;
  x = f%rs, y = (f%ps)/rs, z = f/ps;
  Alphacarre3d(rs, cs, ds, x, y, z, tab1, &n1);
  for (u1 = 0; u1 < n1; u1++) 
  {
    g = tab1[u1];
    x1 = g%rs, y1 = (g%ps)/rs, z1 = g/ps;
    Betacarre3d(rs, cs, ds, x1, y1, z1, tab2, &n2);
    card = 0;
    for (u2 = 0; u2 < n2; u2++) 
      if (K[tab2[u2]]) card++;  
    if (card == 1) return 1;
  }
  return 0;
} /* mcskel2d3d_K3_IsBorder() */

/* ========================================== */
/*! \fn boolean mcskel2d3d_K3_Check2D(struct xvimage *k)
    \param k : un complexe
    \return : booléen
    \brief teste si le complexe ne contient pas d'elements 3D (cubes)
*/
boolean mcskel2d3d_K3_Check2D(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_Check2D"
  index_t i, x, y, z, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
	i = z*ps + y*rs + x;
	if (IS_OBJ(K[i]) && CUBE3D(x,y,z)) return 0;
      }
  return 1;
} // mcskel2d3d_K3_Check2D()

/* ========================================== */
/*! \fn boolean mcskel2d3d_K3_Extract2Dfaces(struct xvimage *k)
    \param k : un complexe
    \return : booléen
    \brief extrait les faces 2d
*/
void mcskel2d3d_K3_Extract2Dfaces(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_Extract2Dfaces"
  index_t i, x, y, z, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
	i = z*ps + y*rs + x;
	if (!CARRE3D(x,y,z)) K[i] = 0;
      }
} // mcskel2d3d_K3_Check2D()

/* ========================================== */
/*! \fn boolean mcskel2d3d_K3_CheckFrame(struct xvimage *k)
    \param k : un complexe
    \return : booléen
    \brief teste si le bord est vide (0)
*/
boolean mcskel2d3d_K3_CheckFrame(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_CheckFrame"
  index_t x, y, z, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs * cs;
  unsigned char *K = UCHARDATA(k);

  for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      if (K[0 * ps + y * rs + x]) return 0;          /* plan z = 0 */
  for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++) 
      if (K[(ds-1) * ps + y * rs + x]) return 0;     /* plan z = ds-1 */

  for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      if (K[z * ps + 0 * rs + x]) return 0;          /* plan y = 0 */
  for (x = 0; x < rs; x++)
    for (z = 0; z < ds; z++) 
      if (K[z * ps + (cs-1) * rs + x]) return 0;     /* plan y = cs-1 */

  for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      if (K[z * ps + y * rs + 0]) return 0;          /* plan x = 0 */
  for (y = 0; y < cs; y++)
    for (z = 0; z < ds; z++) 
      if (K[z * ps + y * rs + (rs-1)]) return 0;     /* plan x = rs-1 */
  return 1;
} // mcskel2d3d_K3_CheckFrame()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkObj(struct xvimage *k)
    \param k : un complexe
    \brief marque FLAG_OBJ toutes les faces du complexe k
*/
void mcskel2d3d_K3_MarkObj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkObj"
  index_t i, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i]) K[i] = FLAG_OBJ; else K[i] = 0;
} // mcskel2d3d_K3_MarkObj()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_Binarize(struct xvimage *k)
    \param k : un complexe
    \brief marque NDG_MAX toutes les faces du complexe k
*/
void mcskel2d3d_K3_Binarize(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_Binarize"
  index_t i, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i]) K[i] = NDG_MAX; else K[i] = 0;
} // mcskel2d3d_K3_Binarize()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_SelMarked(struct xvimage *k, unsigned char mask)
    \param k : un complexe
    \param mask : une marque (mot 8 bits avec un seul bit à 1)
    \brief met a FLAG_OBJ tous le éléments marqués, à 0 les autres
*/
void mcskel2d3d_K3_SelMarked(struct xvimage *k, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_SelMarked"
  index_t i, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i] & mask) K[i] = FLAG_OBJ; else K[i] = 0;
} // mcskel2d3d_K3_SelMarked()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
    \param k : un complexe
    \param f : une face
    \param mask : valeur de la marque (mot 8 bits avec un seul bit à 1)
    \brief ajoute la marque mask à tous les éléments de alphacarre(f)
*/
void mcskel2d3d_K3_MarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkAlphaCarre"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, 
      x = f%rs, y = (f%ps)/rs, z = f/ps;
  index_t tab[26], u;
  int32_t n;
  unsigned char *K = UCHARDATA(k);
  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) K[tab[u]] |= mask;
} // mcskel2d3d_K3_MarkAlphaCarre()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_UnMarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
    \param k : un complexe
    \param f : une face
    \param mask : valeur de la marque (mot 8 bits avec un seul bit à 1)
    \brief retire la marque mask de tous les éléments de alphacarre(f)
*/
void mcskel2d3d_K3_UnMarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_UnMarkAlphaCarre"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, x = f%rs, y = (f%ps)/rs, z = f/ps;
  index_t tab[26], u;
  int32_t n;
  unsigned char *K = UCHARDATA(k);
  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) K[tab[u]] &= ~mask;
} // mcskel2d3d_K3_UnMarkAlphaCarre()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkPrinc(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque face principale f de k, rajoute (OR) la marque FLAG_PRINC au label de f
*/
void mcskel2d3d_K3_MarkPrinc(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkPrinc"
  index_t card, i, x, y, z, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  index_t tab[26], u;
  int32_t n;
  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_OBJ(K[i])) 
    {
      Betacarre3d(rs, cs, ds, x, y, z, tab, &n);
      card = 0;
      for (u = 0; u < n; u++) if (IS_OBJ(K[tab[u]])) card++;
      if (card == 0) K[i] |= FLAG_PRINC;
    }
  }
} // mcskel2d3d_K3_MarkPrinc()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkInterior(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque 2-face intérieure (i.e. non border) f de k, rajoute (OR) la marque FLAG_INTERIOR au label de f
*/
void mcskel2d3d_K3_MarkInterior(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkInterior"
  index_t f, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  for (f = 0; f < N; f++)
  {
    if (IS_OBJ(K[f]) && !mcskel2d3d_K3_IsBorder(k, f)) 
      K[f] |= FLAG_INTERIOR;
  }
} // mcskel2d3d_K3_MarkInterior()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkNotNeigh(struct xvimage *k, struct xvimage *t, struct xvimage *m)
    \param k : un complexe
    \param t : un complexe 
    \param m : un complexe "marqueur"
    \brief ajoute au complexe m les faces carrées de k non voisines (par un côté) 
    d'au moins une face carrée du complexe t
    \warning pas de vérification de compatibilité entre k, t et m
*/
void mcskel2d3d_K3_MarkNotNeigh(struct xvimage *k, struct xvimage *t, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkNotNeigh"
  index_t card, i, x, y, z, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  unsigned char *T = UCHARDATA(t);
  unsigned char *M = UCHARDATA(m);
  index_t tab[26], u;
  index_t n;
  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_OBJ(K[i]) && CARRE3D(x,y,z)) 
    {
      GammaS3d(rs, cs, ds, x, y, z, tab, &n);
      card = 0;
      for (u = 0; u < n; u++) if (IS_OBJ(T[tab[u]])) card++;
      if (card == 0) M[i] |= FLAG_OBJ;
    }
  }
} // mcskel2d3d_K3_MarkNotNeigh()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkEss(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque face essentielle f de k, rajoute (OR) la marque FLAG_ESS au label de f
    \warning les faces principales doivent avoir été marquées auparavant
*/
void mcskel2d3d_K3_MarkEss(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkEss"
  index_t x, y, z, i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  index_t tab[26], u, xu, yu, zu;
  index_t tab2[26], v, n2;
  int32_t n;
#ifdef DEBUG1
  index_t debug;
#endif
  for (z = 0; z < ds; z++) for (y = 0; y < cs; y++) for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_OBJ(K[i]) && CARRE3D(x,y,z)) K[i] |= FLAG_ESS; 
  }

  for (z = 0; z < ds; z++) for (y = 0; y < cs; y++) for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_OBJ(K[i]) && INTER3D(x,y,z))
    {
      if (IS_PRINC(K[i])) 
	K[i] |= FLAG_ESS; 
      else
      {
	index_t card = 0;
	Betacarre3d(rs, cs, ds, x, y, z, tab, &n);
	for (u = 0; u < n; u++) if (IS_PRINC(K[tab[u]])) card++;
	if (card >= 2) K[i] |= FLAG_ESS; 
      }
    }
  }

  for (z = 0; z < ds; z++) for (y = 0; y < cs; y++) for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_OBJ(K[i]) && SINGL3D(x,y,z)) 
    {
      if (IS_PRINC(K[i])) 
	K[i] |= FLAG_ESS; 
      else
      {
	index_t card = 0;
	Betacarre3d(rs, cs, ds, x, y, z, tab, &n);
	for (u = 0; u < n; u++)
	  if (IS_OBJ(K[tab[u]]))
	  { 
	    xu = tab[u] % rs; yu = (tab[u] % ps) / rs; zu = tab[u] / ps;
	    if (INTER3D(xu,yu,zu))
	    {
	      if (IS_ESS(K[tab[u]])) 
	      {
		card++;
	      }
	    }
	    else if (CARRE3D(xu,yu,zu))
	    {
	      mcskel2d3d_K3_AlphaBeta3d(rs, cs, ds, x, y, z, xu, yu, zu, tab2, &n2);
	      for (v = 0; v < n2; v++) if IS_ESS(K[tab2[v]]) break;
	      if (v == n2) 
	      {
		card++;
	      }
	    }
	  }
	if (card >= 2) K[i] |= FLAG_ESS; 
      }
    }
  }
} // mcskel2d3d_K3_MarkEss()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkCore(struct xvimage *k, index_t f)
    \param k : un complexe
    \param f : une face de k
    \return le cardinal (nombre de faces) de la fermeture du noyau
    \brief marque le complexe formé du noyau (core) de la face f pour le complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant
*/
int32_t mcskel2d3d_K3_MarkCore(struct xvimage *k, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkCore"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, nf = 0;
  unsigned char *K = UCHARDATA(k);
  index_t tab[26], tabi[26], u, v, i, j;
  int32_t n, ni;
  index_t x = f%rs, y = (f%ps)/rs, z = f/ps, xi, yi, zi;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) 
  {
    i = tab[u];
    if (IS_ESS(K[i]))
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
#ifdef DEBUG
printf("mark_core : %d,%d,%d\n", i%rs, (i%ps)/rs, i/ps);
#endif
      } 
      xi = i%rs, yi = (i%ps)/rs, zi = i/ps;
      Alphacarre3d(rs, cs, ds, xi, yi, zi, tabi, &ni);
      for (v = 0; v < ni; v++) 
      {
	j = tabi[v];
#ifdef DEBUG
printf("PRE_mark_core_aux : %d,%d,%d [%x]\n", j%rs, (j%ps)/rs, j/ps, K[j]);
#endif
	if (!(IS_CORE(K[j]))) 
	{ 
	  K[j] |= FLAG_CORE; nf++; 
#ifdef DEBUG
printf("mark_core_aux : %d,%d,%d\n", j%rs, (j%ps)/rs, j/ps);
#endif
	} 
      }
    }
  }
  return nf;
} // mcskel2d3d_K3_MarkCore()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkCore2(struct xvimage *k, index_t f)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \param f : une face de k
    \return le cardinal (nombre de faces) de la fermeture du noyau de f pour k, conditionné par m
    \brief marque le complexe formé du noyau (core) de la face f pour le complexe k, conditionné par m
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
int32_t mcskel2d3d_K3_MarkCore2(struct xvimage *k, struct xvimage *m, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkCore2"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, nf = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t tab[26], tabi[26], u, v, i, j;
  int32_t n, ni;
  index_t x = f%rs, y = (f%ps)/rs, z = f/ps, xi, yi, zi;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) 
  {
    i = tab[u];
    if (M[i])
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
      } 
    }
    else if (IS_ESS(K[i]))
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
      } 
      xi = i%rs, yi = (i%ps)/rs, zi = i/ps;
      Alphacarre3d(rs, cs, ds, xi, yi, zi, tabi, &ni);
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
} // mcskel2d3d_K3_MarkCore2()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_CardCore(struct xvimage *k, index_t f)
    \param k : un complexe
    \param f : une face de k
    \return un entier 
    \brief retourne le cardinal du noyau de f, i.e., le nombre de faces essentielles du noyau 
    \warning les faces essentielles doivent avoir été marquées auparavant
*/
int32_t mcskel2d3d_K3_CardCore(struct xvimage *k, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_CardCore"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, nf = 0;
  unsigned char *K = UCHARDATA(k);
  index_t tab[26], u;
  int32_t n;
  index_t x = f%rs, y = (f%ps)/rs, z = f/ps;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) if (IS_ESS(K[tab[u]])) nf++; 
  return nf;
} // mcskel2d3d_K3_CardCore()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_CardCore2(struct xvimage *k, struct xvimage *m, index_t f)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \param f : une face de k
    \return un entier 
    \brief retourne le cardinal du noyau de f pour k conditionné par m, i.e.,
    le nombre de faces marquées FLAG_ESS ou appartenant à m dans le noyau 
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
int32_t mcskel2d3d_K3_CardCore2(struct xvimage *k, struct xvimage *m, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_CardCore2"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, nf = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t tab[26], u;
  int32_t n;
  index_t x = f%rs, y = (f%ps)/rs, z = f/ps;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) if (IS_ESS(K[tab[u]]) || M[tab[u]]) nf++; 
  return nf;
} // mcskel2d3d_K3_CardCore2()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkRegul(struct xvimage *k)
    \param k : un complexe
    \brief marque les faces régulières du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant
    \warning l'objet ne doit pas toucher le bord (pas de vérification)
*/
void mcskel2d3d_K3_MarkRegul(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkRegul"
  index_t x, y, z, i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  index_t n, ncore, ntrans;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    if (SINGL3D(x,y,z)) n = 0; else if (CARRE3D(x,y,z)) n = 8; else n = 2; 
    i = z*ps + y*rs + x;
    if (IS_ESS(K[i])) 
    {
      ncore = mcskel2d3d_K3_MarkCore(k, i);
#ifdef DEBUG_REGUL
printf("ess : %d,%d,%d ; n = %d, ncore = %d\n", x, y, z, n, ncore);
#endif
      if (ncore == 0) goto next;
      if (ncore == n) { mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE); goto next; }
      //if (SINGL(x,y,z)) goto next; // inutile car le core d'un singl est forcément vide
      if (INTER3D(x,y,z))
      {
	if (ncore == 1) K[i] |= FLAG_REGUL;
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      if (CARRE3DXY(x,y,z))
      {
	ntrans = 0;
	if ((K[z*ps + (y)  *rs + x+1] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x+1] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x  ] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x-1] & FLAG_CORE) != (K[z*ps + (y)  *rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y)  *rs + x-1] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x-1] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x  ] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x+1] & FLAG_CORE) != (K[z*ps + (y)  *rs + x+1] & FLAG_CORE)) ntrans++;
	if (ntrans == 2) K[i] |= FLAG_REGUL;
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
      else if (CARRE3DYZ(x,y,z))
      {
	ntrans = 0;
	if ((K[(z)  *ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y)  *rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y)  *rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z)  *ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z)  *ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y)  *rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y)  *rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z)  *ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if (ntrans == 2) K[i] |= FLAG_REGUL;
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
      else if (CARRE3DXZ(x,y,z))
      {
	ntrans = 0;
	if ((K[(z)  *ps + y*rs + x+1] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x+1] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x  ] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x-1] & FLAG_CORE) != (K[(z)  *ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z)  *ps + y*rs + x-1] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x-1] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x  ] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x+1] & FLAG_CORE) != (K[(z)  *ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if (ntrans == 2) K[i] |= FLAG_REGUL;
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
    next:;
    }
  }
} // mcskel2d3d_K3_MarkRegul()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkRegul2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief marque les faces régulières du complexe k conditionné par le complexe m
    \warning les faces essentielles doivent avoir été marquées auparavant
    \warning l'objet ne doit pas toucher le bord (pas de vérification),
    pas de vérification de compatibilité entre k et m
*/
void mcskel2d3d_K3_MarkRegul2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkRegul2"
  index_t x, y, z, i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  index_t n, ncore, ntrans;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    if (SINGL3D(x,y,z)) n = 0; else if (CARRE3D(x,y,z)) n = 8; else n = 2; 
    i = z*ps + y*rs + x;
    if (IS_ESS(K[i])) 
    {
      ncore = mcskel2d3d_K3_MarkCore2(k, m, i);
#ifdef DEBUG_REGUL
printf("ess : %d,%d,%d ; n = %d, ncore = %d\n", x, y, z, n, ncore);
#endif
      if (ncore == 0) goto next;
      if (ncore == n) { mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE); goto next; }
      //if (SINGL(x,y,z)) goto next; // inutile car le core d'un singl est forcément vide
      if (INTER3D(x,y,z))
      {
	if (ncore == 1) K[i] |= FLAG_REGUL;
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      if (CARRE3DXY(x,y,z))
      {
	ntrans = 0;
	if ((K[z*ps + (y)  *rs + x+1] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x+1] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x  ] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x-1] & FLAG_CORE) != (K[z*ps + (y)  *rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y)  *rs + x-1] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x-1] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x  ] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x+1] & FLAG_CORE) != (K[z*ps + (y)  *rs + x+1] & FLAG_CORE)) ntrans++;
	if (ntrans == 2) K[i] |= FLAG_REGUL;
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
      else if (CARRE3DYZ(x,y,z))
      {
	ntrans = 0;
	if ((K[(z)  *ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y)  *rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y)  *rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z)  *ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z)  *ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y)  *rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y)  *rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z)  *ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if (ntrans == 2) K[i] |= FLAG_REGUL;
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
      else if (CARRE3DXZ(x,y,z))
      {
	ntrans = 0;
	if ((K[(z)  *ps + y*rs + x+1] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x+1] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x  ] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x-1] & FLAG_CORE) != (K[(z)  *ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z)  *ps + y*rs + x-1] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x-1] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x  ] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x+1] & FLAG_CORE) != (K[(z)  *ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if (ntrans == 2) K[i] |= FLAG_REGUL;
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
    next:;
    }
  }
} // mcskel2d3d_K3_MarkRegul2()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkCritic(struct xvimage *k)
    \param k : un complexe
    \brief marque FLAG_CRITIC les fermetures des faces critiques du complexe k
    \warning les faces essentielles et les faces régulières doivent avoir été marquées auparavant
*/
void mcskel2d3d_K3_MarkCritic(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkCritic"
  index_t i, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (IS_ESS(K[i]) && !IS_REGUL(K[i])) 
  {
    K[i] |= FLAG_CRITIC;
    mcskel2d3d_K3_MarkAlphaCarre(k, i, FLAG_CRITIC);
  }
} // mcskel2d3d_K3_MarkCritic()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkCritic2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief marque FLAG_CRITIC les fermetures des faces critiques du complexe k, conditionné par m
    \warning les faces essentielles et les faces régulières doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel2d3d_K3_MarkCritic2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkCritic2"
  index_t i, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  for (i = 0; i < N; i++) if (M[i] || (IS_ESS(K[i]) && !IS_REGUL(K[i])))
  {
    K[i] |= FLAG_CRITIC;
    mcskel2d3d_K3_MarkAlphaCarre(k, i, FLAG_CRITIC);
  }
} // mcskel2d3d_K3_MarkCritic2()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkMCritic(struct xvimage *k)
    \param k : un complexe 
    \brief marque FLAG_CRITIC les faces m-critiques du complexe k
    \warning les faces essentielles et les faces régulières doivent avoir été marquées auparavant
*/
void mcskel2d3d_K3_MarkMCritic(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkMCritic"
  index_t x, y, z, i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  index_t tab[26], u;
  int32_t n;
  for (i = 0; i < N; i++) if (IS_ESS(K[i]) && !IS_REGUL(K[i])) 
  {
    K[i] |= FLAG_CRITIC;
  }
  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_CRITIC(K[i])) 
    {
      Betacarre3d(rs, cs, ds, x, y, z, tab, &n);
      for (u = 0; u < n; u++) if (IS_CRITIC(K[tab[u]])) break;	
      if (u == n) K[i] |= FLAG_TMP;
    }
  }
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    if (!IS_TMP(K[i])) K[i] &= ~FLAG_CRITIC;
    K[i] &= ~FLAG_TMP;
  }
} // mcskel2d3d_K3_MarkMCritic()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkMCritic2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe 
    \param m : un complexe "marqueur"
    \brief marque FLAG_CRITIC les faces M-critiques du complexe k, conditionné par m
    \warning les faces essentielles et les faces régulières (conditionnellement à m)  
    doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel2d3d_K3_MarkMCritic2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkMCritic2"
  index_t x, y, z, i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t tab[26], u;
  int32_t n;

  for (i = 0; i < N; i++) if (M[i] || (IS_ESS(K[i]) && !IS_REGUL(K[i])))
  {
    K[i] |= FLAG_CRITIC;
  }
  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_CRITIC(K[i])) 
    {
      Betacarre3d(rs, cs, ds, x, y, z, tab, &n);
      for (u = 0; u < n; u++) if (IS_CRITIC(K[tab[u]])) break;	
      if (u == n) K[i] |= FLAG_TMP;
    }
  }
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    if (!IS_TMP(K[i])) K[i] &= ~FLAG_CRITIC;
    K[i] &= ~FLAG_TMP;
  }
} // mcskel2d3d_K3_MarkMCritic2()

/* ========================================== */
/*! \fn int32_t mcskel2d3d_K3_Critic2Obj(struct xvimage *k)
    \param k : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les faces marquées FLAG_CRITIC du complexe k et remet à 0 les autres marques
*/
int32_t mcskel2d3d_K3_Critic2Obj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_Critic2Obj"
  index_t i, N = rowsize(k) * colsize(k) * depth(k), n = 0;
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
} // mcskel2d3d_K3_Critic2Obj()

/* ========================================== */
/*! \fn int32_t mcskel2d3d_K3_Interior2Obj(struct xvimage *k)
    \param k : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les faces marquées FLAG_INTERIOR du complexe k et remet à 0 les autres marques
*/
int32_t mcskel2d3d_K3_Interior2Obj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_Interior2Obj"
  index_t i, N = rowsize(k) * colsize(k) * depth(k), n = 0;
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) 
    if (IS_INTERIOR(K[i]))
    {
      K[i] = FLAG_OBJ;
      n++;
    }
    else
      K[i] = 0;
  return n;
} // mcskel2d3d_K3_Interior2Obj()

/* ========================================== */
/*! \fn int32_t mcskel2d3d_K3_MCritic2Obj(struct xvimage *k, struct xvimage *o)
    \param k : un complexe
    \param o : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les fermetures des faces,
    présentes dans le complexe o,
    contenant des faces m-critiques (marquées FLAG_CRITIC)
    du complexe k et remet à 0 les autres marques.
*/
int32_t mcskel2d3d_K3_MCritic2Obj(struct xvimage *k, struct xvimage *o)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MCritic2Obj"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  index_t x, y, z, i, j, n = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *O = UCHARDATA(o);
  index_t tab1[26], tab2[26], u, v;
  int32_t n1, n2;
  for (i = 0; i < N; i++) K[i] &= ~FLAG_OBJ;
  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_CRITIC(K[i])) 
    {
      K[i] |= FLAG_OBJ;
      Betacarre3d(rs, cs, ds, x, y, z, tab1, &n1);
      for (u = 0; u < n1; u++) 
      {
	j = tab1[u];
	if (O[j])
	{
	  K[j] |= FLAG_OBJ;
	  Alphacarre3d(rs, cs, ds, j%rs, (j%ps)/rs, j/ps, tab2, &n2);
	  for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
	}
      }
      Alphacarre3d(rs, cs, ds, x, y, z, tab2, &n2);
      for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
    }
  }

  for (i = 0; i < N; i++) 
    if (IS_OBJ(K[i])) { n++; K[i] = FLAG_OBJ; } else K[i] = 0;
  return n;
} // mcskel2d3d_K3_MCritic2Obj()

/* ========================================== */
/*! \fn int32_t mcskel2d3d_K3_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m, struct xvimage *o)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \param o : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les fermetures des faces,
    présentes dans le complexe o,
    contenant des faces m-critiques (marquées FLAG_CRITIC)
    du complexe k ou des faces de m et remet à 0 les autres marques.
*/
int32_t mcskel2d3d_K3_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m, struct xvimage *o)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MCriticOrMarked2Obj"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  index_t x, y, z, i, j, n = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  unsigned char *O = UCHARDATA(o);
  index_t tab1[26], tab2[26], u, v;
  int32_t n1, n2;
  for (i = 0; i < N; i++) K[i] &= ~FLAG_OBJ;
  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (M[i] || IS_CRITIC(K[i])) 
    {
      K[i] |= FLAG_OBJ;
      Betacarre3d(rs, cs, ds, x, y, z, tab1, &n1);
      for (u = 0; u < n1; u++) 
      {
	j = tab1[u];
	if (O[j])
	{
	  K[j] |= FLAG_OBJ;
	  Alphacarre3d(rs, cs, ds, j%rs, (j%ps)/rs, j/ps, tab2, &n2);
	  for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
	}
      }
      Alphacarre3d(rs, cs, ds, x, y, z, tab2, &n2);
      for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
    }
  }

  for (i = 0; i < N; i++) 
    if (IS_OBJ(K[i])) { n++; K[i] = FLAG_OBJ; } else K[i] = 0;
  return n;
} // mcskel2d3d_K3_MCriticOrMarked2Obj()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkThin(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief ajoute au complexe m les faces séparantes (Tb > 1) du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel2d3d_K3_MarkThin(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkThin"
  index_t i, j, x, y, z, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps * ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t n, ncore, ntrans;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    if (SINGL3D(x,y,z)) n = 0; else if (CARRE3D(x,y,z)) n = 8; else n = 2; 
    i = z*ps + y*rs + x;
    if (IS_ESS(K[i])) 
    {
      if (SINGL3D(x,y,z)) goto next;
      if (INTER3D(x,y,z))
      {
	if (IS_PRINC(K[i]))
	{
	  K[i] |= FLAG_TMP;
	  mcskel2d3d_K3_MarkAlphaCarre(k, i, FLAG_TMP);
	}
	goto next;
      }
      if (CARRE3D(x,y,z))
      {
	ncore = mcskel2d3d_K3_MarkCore(k, i);
#ifdef DEBUG
printf("ess : %d,%d ; n = %d, ncore = %d\n", x, y, n, ncore);
#endif
        if (ncore == 0) goto next;
	if (ncore == n) { mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE); goto next; }
      }
      if (CARRE3DXY(x,y,z))
      {
	ntrans = 0;
	if ((K[z*ps + (y)  *rs + x+1] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x+1] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x  ] & FLAG_CORE) != (K[z*ps + (y-1)*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y-1)*rs + x-1] & FLAG_CORE) != (K[z*ps + (y)  *rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y)  *rs + x-1] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x-1] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x  ] & FLAG_CORE) != (K[z*ps + (y+1)*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[z*ps + (y+1)*rs + x+1] & FLAG_CORE) != (K[z*ps + (y)  *rs + x+1] & FLAG_CORE)) ntrans++;
	if (ntrans != 2) { K[i] |= FLAG_TMP; mcskel2d3d_K3_MarkAlphaCarre(k, i, FLAG_TMP); }
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
      else if (CARRE3DYZ(x,y,z))
      {
	ntrans = 0;
	if ((K[(z)  *ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y)  *rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y)  *rs + x] & FLAG_CORE) != (K[(z-1)*ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z)  *ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z)  *ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y-1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y-1)*rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y)  *rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y)  *rs + x] & FLAG_CORE) != (K[(z+1)*ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + (y+1)*rs + x] & FLAG_CORE) != (K[(z)  *ps + (y+1)*rs + x] & FLAG_CORE)) ntrans++;
	if (ntrans != 2) { K[i] |= FLAG_TMP; mcskel2d3d_K3_MarkAlphaCarre(k, i, FLAG_TMP); }
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
      else if (CARRE3DXZ(x,y,z))
      {
	ntrans = 0;
	if ((K[(z)  *ps + y*rs + x+1] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x+1] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x  ] & FLAG_CORE) != (K[(z-1)*ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z-1)*ps + y*rs + x-1] & FLAG_CORE) != (K[(z)  *ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z)  *ps + y*rs + x-1] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x-1] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x-1] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x  ] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x  ] & FLAG_CORE) != (K[(z+1)*ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if ((K[(z+1)*ps + y*rs + x+1] & FLAG_CORE) != (K[(z)  *ps + y*rs + x+1] & FLAG_CORE)) ntrans++;
	if (ntrans != 2) { K[i] |= FLAG_TMP; mcskel2d3d_K3_MarkAlphaCarre(k, i, FLAG_TMP); }
	mcskel2d3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
    next:;
    }
  }
  for (i = 0; i < N; i++) if (IS_TMP(K[i])) M[i] = 1;
} // mcskel2d3d_K3_MarkThin()

/* ========================================== */
/*! \fn int32_t mcskel2d3d_K3_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \return le nombre de faces critiques ou marquées trouvées
    \brief marque FLAG_OBJ les faces critiques du complexe k ou marquées par m et remet à 0 les flags dans k
*/
int32_t mcskel2d3d_K3_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_CriticOrMarked2Obj"
  index_t i, N = rowsize(k) * colsize(k) * depth(k), nobj = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  for (i = 0; i < N; i++) 
    if (M[i] || IS_CRITIC(K[i])) { K[i] = FLAG_OBJ; nobj++; } else K[i] = 0;
  return nobj;
} // mcskel2d3d_K3_CriticOrMarked2Obj()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkEnd(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief ajoute au complexe m les faces extrémités du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel2d3d_K3_MarkEnd(struct xvimage *k, struct xvimage *m)
/* ========================================== */
// une face "CARRE" est extrémité si 
//   i) elle est régulière et 
//   ii) le cardinal de son core <= 3
// une face "INTER" est extrémité si 
//   i) elle est régulière et  
//   ii) elle est principale
{

#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkEnd"
  index_t x, y, z, i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t ncore, card, tab[26], u, n;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_REGUL(K[i])) 
    {
      if (SINGL3D(x,y,z)) goto next;
      if (INTER3D(x,y,z) && IS_PRINC(K[i]))
      {
	K[i] |= FLAG_TMP;
	mcskel2d3d_K3_MarkAlphaCarre(k, i, FLAG_TMP);
	goto next;
      }
      if (CARRE3D(x,y,z))
      {
	ncore = mcskel2d3d_K3_CardCore(k, i);
	if (ncore <= 3) 
        {
	  K[i] |= FLAG_TMP;
	  mcskel2d3d_K3_MarkAlphaCarre(k, i, FLAG_TMP);
	}
      }
    next:;
    }
  }
  for (i = 0; i < N; i++) if (IS_TMP(K[i])) M[i] = 1;
} // mcskel2d3d_K3_MarkEnd()

/* ========================================== */
/*! \fn void mcskel2d3d_K3_MarkEnd2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief ajoute au complexe m les faces extrémités du complexe k
    \warning les faces essentielles et régulières doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel2d3d_K3_MarkEnd2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
// une face régulière f est extrémité si 
// f est maximale et si f contient exactement une face critique
{
#undef F_NAME
#define F_NAME "mcskel2d3d_K3_MarkEnd2"
  index_t x, y, z, i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  index_t ncritic, tab[26], u;
  int32_t n;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_REGUL(K[i])) 
    {
      if (SINGL3D(x,y,z)) goto next;
      if (INTER3D(x,y,z))
      {
	Betacarre3d(rs, cs, ds, x, y, z, tab, &n);
	if (n != 0) goto next;
	Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
	ncritic = 0;
        for (u = 0; u < n; u++)
	{
	  if (IS_ESS(K[tab[u]]) && !IS_REGUL(K[tab[u]])) ncritic++;
	}
	if (ncritic != 1) goto next;
	K[i] |= FLAG_TMP;	
      }
      //if (CARRE3D(x,y,z)) // inutile car c'est le seul choix qui reste
      Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
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
} // mcskel2d3d_K3_MarkEnd2()

/* =============================================================== */
void mcskel2d3d_lskel3d4(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
  \brief Squelette ultime pour des 2-complexes principaux, 
  version "Z^3" 
         - version efficace basée sur Khalimski 3D
  \author Michel COUPRIE, juillet 2005, d'après Gilles Bertrand
  \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d4"
#ifdef DEBUG
  struct xvimage * k3;
#endif
  struct xvimage * m;
  struct xvimage * o;
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!mcskel2d3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  if (!mcskel2d3d_K3_Check2D(k))
  {
    fprintf(stderr, "%s: complex must be 2D\n", F_NAME);
    exit(0);
  }

  mcskel2d3d_K3_Extract2Dfaces(k);
  mcskel2d3d_K3_Closure(k);

  m = copyimage(k);
  o = copyimage(k);
  razimage(m);
#ifdef DEBUG
  k3 = copyimage(k);
#endif

  mcskel2d3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
#ifdef DEBUG
    copy2image(k3, k);
#endif
    copy2image(o, k);
    n++;
    n_old = n_new;
    mcskel2d3d_K3_MarkPrinc(k);
    mcskel2d3d_K3_MarkEss(k);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_ESS);
    writeimage(k3, "_k_ess");
#endif

    mcskel2d3d_K3_MarkRegul2(k, m);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_REGUL);
    writeimage(k3, "_k_regul");
#endif

    mcskel2d3d_K3_MarkMCritic2(k,m);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_CRITIC);
    writeimage(k3, "_k_critic");
#endif

    n_new = mcskel2d3d_K3_MCritic2Obj(k, o);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_OBJ);
    writeimage(k3, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel2d3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel2d3d_K3_Binarize(k);

 fin:  

#ifdef DEBUG
  freeimage(k3);
#endif

  freeimage(m);
} // lskel3d4()

/* =============================================================== */
void lskel3d5(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
  \brief Squelette curviligne pour des 2-complexes principaux, 
  version "Z^3" avec détection de points extrémités 
         - version efficace basée sur Khalimski 3D
  \author Michel COUPRIE, juillet 2005, d'après Gilles Bertrand
  \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d5"
#ifdef DEBUG
  struct xvimage * k3;
#endif
  struct xvimage * m;
  struct xvimage * o;
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!mcskel2d3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  if (!mcskel2d3d_K3_Check2D(k))
  {
    fprintf(stderr, "%s: complex must be 2D\n", F_NAME);
    exit(0);
  }

  mcskel2d3d_K3_Extract2Dfaces(k);
  mcskel2d3d_K3_Closure(k);

  m = copyimage(k);
  o = copyimage(k);
  razimage(m);
#ifdef DEBUG
  k3 = copyimage(k);
#endif

  mcskel2d3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
#ifdef DEBUG
    copy2image(k3, k);
#endif
    copy2image(o, k);
    n++;
    n_old = n_new;
    mcskel2d3d_K3_MarkPrinc(k);
    mcskel2d3d_K3_MarkEss(k);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_ESS);
    writeimage(k3, "_k_ess");
#endif

    mcskel2d3d_K3_MarkRegul2(k, m);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_REGUL);
    writeimage(k3, "_k_regul");
#endif

    mcskel2d3d_K3_MarkEnd2(k, m);

#ifdef DEBUG
    k3 = copyimage(m);
    mcskel2d3d_K3_Binarize(k3);
    writeimage(k3, "_k_end");
#endif

    mcskel2d3d_K3_MarkMCritic2(k,m);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_CRITIC);
    writeimage(k3, "_k_critic");
#endif

    n_new = mcskel2d3d_K3_MCriticOrMarked2Obj(k, m, o);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_OBJ);
    writeimage(k3, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel2d3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel2d3d_K3_Binarize(k);

 fin:  

#ifdef DEBUG
  freeimage(k3);
#endif

  freeimage(m);
} // lskel3d5()

/* =============================================================== */
void lskel3d6(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
  \brief squelette curviligne contenant l'axe topologique
  \author Michel COUPRIE, août 2005, d'après Gilles Bertrand
  \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d6"
#ifdef DEBUG
  struct xvimage * k3;
#endif
  struct xvimage * o;
  struct xvimage * t;
  struct xvimage * m;
  unsigned char *T;
  unsigned char *M;
  index_t n_old, n_new, n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!mcskel2d3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  if (!mcskel2d3d_K3_Check2D(k))
  {
    fprintf(stderr, "%s: complex must be 2D\n", F_NAME);
    exit(0);
  }

  mcskel2d3d_K3_Extract2Dfaces(k);
  mcskel2d3d_K3_Closure(k);
  mcskel2d3d_K3_MarkObj(k);

  t = copyimage(k);
  m = copyimage(k);
  o = copyimage(k);
  razimage(m);
#ifdef DEBUG
  k3 = copyimage(k);
#endif

  n_new = -1;
  n = 0;
  do
  {
#ifdef DEBUG
    copy2image(k3, k);
#endif
    copy2image(o, k);
    n++;
    n_old = n_new;

    mcskel2d3d_K3_MarkInterior(t);
    mcskel2d3d_K3_Interior2Obj(t);

#ifdef DEBUG
    k3 = copyimage(t);
    mcskel2d3d_K3_Binarize(k3);
    writeimage(k3, "_k_int");
#endif

    mcskel2d3d_K3_MarkPrinc(k);
    mcskel2d3d_K3_MarkEss(k);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_ESS);
    writeimage(k3, "_k_ess");
#endif

    mcskel2d3d_K3_MarkNotNeigh(k, t, m);

#ifdef DEBUG
    k3 = copyimage(m);
    mcskel2d3d_K3_SelMarked(k3, FLAG_OBJ);
    writeimage(k3, "_k_notneigh");
#endif

    mcskel2d3d_K3_MarkRegul2(k, m);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_REGUL);
    writeimage(k3, "_k_regul");
#endif

    mcskel2d3d_K3_MarkMCritic2(k, m);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_CRITIC);
    writeimage(k3, "_k_critic");
#endif

    n_new = mcskel2d3d_K3_MCriticOrMarked2Obj(k, m, o);

#ifdef DEBUG
    k3 = copyimage(k);
    mcskel2d3d_K3_SelMarked(k3, FLAG_OBJ);
    writeimage(k3, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel2d3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel2d3d_K3_Binarize(k);

 fin:  

#ifdef DEBUG
  freeimage(k3);
#endif

  freeimage(m);
} // lskel3d6()

/* =============================================================== */
void lskel3d7(struct xvimage * k, index_t nsteps) 
/* =============================================================== */
/*
  \brief axe topologique
  \author Michel COUPRIE, août 2005, d'après Gilles Bertrand
  \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d7"
#ifdef DEBUG
  struct xvimage * k3;
#endif
  struct xvimage * t;
  struct xvimage * m;
  index_t n;

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return;

  if (!mcskel2d3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  if (!mcskel2d3d_K3_Check2D(k))
  {
    fprintf(stderr, "%s: complex must be 2D\n", F_NAME);
    exit(0);
  }

  mcskel2d3d_K3_Extract2Dfaces(k);
  mcskel2d3d_K3_Closure(k);
  mcskel2d3d_K3_MarkObj(k);

  t = copyimage(k);
  m = copyimage(k);
  razimage(m);
#ifdef DEBUG
  k3 = copyimage(k);
#endif

  n = 0;
  do
  {
    n++;

    mcskel2d3d_K3_MarkInterior(t);
    mcskel2d3d_K3_Interior2Obj(t);
    mcskel2d3d_K3_Squares(t);
    mcskel2d3d_K3_Closure(t);

#ifdef DEBUG
    k3 = copyimage(t);
    mcskel2d3d_K3_Binarize(k3);
    writeimage(k3, "_k_int");
#endif

    mcskel2d3d_K3_MarkNotNeigh(k, t, m);

#ifdef DEBUG
    k3 = copyimage(m);
    mcskel2d3d_K3_SelMarked(k3, FLAG_OBJ);
    writeimage(k3, "_k_notneigh");
#endif

    copy2image(k, t);
    mcskel2d3d_K3_MarkObj(k);

#ifdef DEBUG
    //    k3 = copyimage(k);
    //    mcskel2d3d_K3_SelMarked(k3, FLAG_OBJ);
    writeimage(k, "_k");
#endif

#ifdef VERBOSE
    printf("step %d, remaining faces: %d\n", n, mcskel2d3d_K3_Card(k));
#endif

  } while ((!mcskel2d3d_K3_IsEmpty(k)) && (n < nsteps));

  copy2image(k, m);
  mcskel2d3d_K3_Closure(k);
  mcskel2d3d_K3_Binarize(k);

 fin:  

#ifdef DEBUG
  freeimage(k3);
#endif

  freeimage(m);
} // lskel3d7()
