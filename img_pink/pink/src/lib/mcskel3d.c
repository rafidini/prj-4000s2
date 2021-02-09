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
/* \file mcskel3d.c
    \brief algorithmes pour la squelettisation parallèle en 3D - 
           version utilisant mckhalimsky3d
    \author Michel COUPRIE, janvier-septembre 2006, d'après Gilles Bertrand
    update : Michel Couprie, août 2007, version par propagation
    update : Michel Couprie, août 2007, suppression du flag REGUL (complementaire de CRITIC)
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <mctopo3d_table.h>
#include <mckhalimsky3d.h>
#include <mcrbt.h>
#include <mcliste.h>
#include <mcgeo.h>
#include <ldist.h>
#include <mcskel3d.h>

#define SetEss3(x) EssTab3[x/8]|=(1<<(x%8))
#define IsEss3(x)  (EssTab3[x/8]&(1<<(x%8)))

#define ESS3DTABNAME "Ess3DTab.dat"

//#define DEBUG
//#define DEBUG1
//#define DEBUGMARKCRITIC
//#define DEBUGMARKCRITIC2
//#define VERBOSE
#define PARANO

// table des configurations de faces essentielles pour les intervalles de Khalimsky
// tables générées par GenerateEssTab2x() dans mcsquel2d.c
unsigned char EssTab2X[256] = {
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
}; // EssTab2X[]

unsigned char EssTab2Y[256] = {
  1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
}; // EssTab2Y[]

unsigned char EssTab2Z[256] = {
  1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
}; // EssTab2Z[]

/* ==================================== */
uint32_t PrincBetacarre3d(uint8_t *X, index_t rs, index_t cs, index_t ds, int32_t i, int32_t j, int32_t k)
/* ==================================== */
/* 
  retourne la configuration des faces principales de beta-adherence 
  de (i,j) privee de (i,j), dans l'objet X, sous la forme d'un tableau de bits:
  cube : 0 bits
  carre: 2 bits
  inter: 8 bits
  singl: 26 bits
*/
{
  index_t ps = rs * cs;
  uint32_t b = 0; /* accumulateur pour les bits */
  int32_t b1 = 1;         /* un bit a decaler */

  if (i % 2 == 0)
  {
    if ((i>0)      && IS_PRINC(X[k*ps + j*rs + i-1])) b |= b1; b1 = b1 << 1;
    if (((i+1)<rs) && IS_PRINC(X[k*ps + j*rs + i+1])) b |= b1; b1 = b1 << 1;
  }
  if (j % 2 == 0)
  {
    if ((j>0)      && IS_PRINC(X[k*ps + (j-1)*rs + i])) b |= b1; b1 = b1 << 1;
    if (((j+1)<cs) && IS_PRINC(X[k*ps + (j+1)*rs + i])) b |= b1; b1 = b1 << 1;
  }
  if (k % 2 == 0)
  {
    if ((k>0)      && IS_PRINC(X[(k-1)*ps + j*rs + i])) b |= b1; b1 = b1 << 1;
    if (((k+1)<ds) && IS_PRINC(X[(k+1)*ps + j*rs + i])) b |= b1; b1 = b1 << 1;
  }
  if ((i%2) + (j%2) == 0)
  {
    if (((i>0)&&(j>0))           && IS_PRINC(X[k*ps + (j-1)*rs + i-1])) b |= b1; b1 = b1 << 1;
    if (((i>0)&&((j+1)<cs))      && IS_PRINC(X[k*ps + (j+1)*rs + i-1])) b |= b1; b1 = b1 << 1;
    if ((((i+1)<rs)&&(j>0))      && IS_PRINC(X[k*ps + (j-1)*rs + i+1])) b |= b1; b1 = b1 << 1;
    if ((((i+1)<rs)&&((j+1)<cs)) && IS_PRINC(X[k*ps + (j+1)*rs + i+1])) b |= b1; b1 = b1 << 1;
  }
  if ((i%2) + (k%2) == 0)
  {
    if (((i>0)&&(k>0))           && IS_PRINC(X[(k-1)*ps + j*rs + i-1])) b |= b1; b1 = b1 << 1;
    if (((i>0)&&((k+1)<ds))      && IS_PRINC(X[(k+1)*ps + j*rs + i-1])) b |= b1; b1 = b1 << 1;
    if ((((i+1)<rs)&&(k>0))      && IS_PRINC(X[(k-1)*ps + j*rs + i+1])) b |= b1; b1 = b1 << 1;
    if ((((i+1)<rs)&&((k+1)<ds)) && IS_PRINC(X[(k+1)*ps + j*rs + i+1])) b |= b1; b1 = b1 << 1;
  }
  if ((k%2) + (j%2) == 0)
  {
    if (((k>0)&&(j>0))           && IS_PRINC(X[(k-1)*ps + (j-1)*rs + i])) b |= b1; b1 = b1 << 1;
    if (((k>0)&&((j+1)<cs))      && IS_PRINC(X[(k-1)*ps + (j+1)*rs + i])) b |= b1; b1 = b1 << 1;
    if ((((k+1)<ds)&&(j>0))      && IS_PRINC(X[(k+1)*ps + (j-1)*rs + i])) b |= b1; b1 = b1 << 1;
    if ((((k+1)<ds)&&((j+1)<cs)) && IS_PRINC(X[(k+1)*ps + (j+1)*rs + i])) b |= b1; b1 = b1 << 1;
  }
  if ((i%2) + (j%2) + (k%2) == 0)
  {
    if (((i>0)&&(j>0)&&(k>0))                && IS_PRINC(X[(k-1)*ps + (j-1)*rs + i-1])) b |= b1; b1 = b1<<1;
    if (((i>0)&&((j+1)<cs)&&(k>0))           && IS_PRINC(X[(k-1)*ps + (j+1)*rs + i-1])) b |= b1; b1 = b1<<1;
    if ((((i+1)<rs)&&(j>0)&&(k>0))           && IS_PRINC(X[(k-1)*ps + (j-1)*rs + i+1])) b |= b1; b1 = b1<<1;
    if ((((i+1)<rs)&&((j+1)<cs)&&(k>0))      && IS_PRINC(X[(k-1)*ps + (j+1)*rs + i+1])) b |= b1; b1 = b1<<1;
    if (((i>0)&&(j>0)&&((k+1)<ds))           && IS_PRINC(X[(k+1)*ps + (j-1)*rs + i-1])) b |= b1; b1 = b1<<1;
    if (((i>0)&&((j+1)<cs)&&((k+1)<ds))      && IS_PRINC(X[(k+1)*ps + (j+1)*rs + i-1])) b |= b1; b1 = b1<<1;
    if ((((i+1)<rs)&&(j>0)&&((k+1)<ds))      && IS_PRINC(X[(k+1)*ps + (j-1)*rs + i+1])) b |= b1; b1 = b1<<1;
    if ((((i+1)<rs)&&((j+1)<cs)&&((k+1)<ds)) && IS_PRINC(X[(k+1)*ps + (j+1)*rs + i+1])) b |= b1; b1 = b1<<1;
  }
  return b;
} /* PrincBetacarre3d() */

// already implemented in mcskel2d3d.c
/* ========================================== */
/*! \fn int32_t mcskel3d_K3_CheckFrame(struct xvimage *k)
    \param k : un complexe
    \return : booléen
    \brief teste si le bord est vide (0)
*/
int32_t mcskel3d_K3_CheckFrame(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_CheckFrame"
  int32_t x, y, z; 
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
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
} // mcskel3d_K3_CheckFrame()

/* ========================================== */
/*! \fn int32_t mcskel3d_K3_CheckComplex(struct xvimage *k)
    \param k : un sous-ensemble de H^2
    \return : booléen
    \brief teste si k est un complexe
*/
int32_t mcskel3d_K3_CheckComplex(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_CheckComplex"
  int32_t x, y, z;
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  int32_t tab[26], u, n;
  
  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
    if (K[z*ps + y*rs + x])
    {
      Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
      for (u = 0; u < n; u++)
	if (K[tab[u]] == 0) return 0;
    }
  return 1;
} // mcskel3d_K3_CheckComplex()

/* ========================================== */
/*! \fn void mcskel3d_K3_CloseComplex(struct xvimage *k)
    \param k : un sous-ensemble de H^3
    \brief en sortie, k est un complexe qui est la fermeture de l'ensemble de départ
*/
void mcskel3d_K3_CloseComplex(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_CloseComplex"
  int32_t x, y, z; 
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  int32_t tab[26], u, n;
  
  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
    if (K[z*ps + y*rs + x])
    {
      Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
      for (u = 0; u < n; u++) K[tab[u]] = FLAG_OBJ;
    }
} // mcskel3d_K3_CloseComplex()

// already implemented in mcskel2d3d.c
/* ========================================== */
/*! \fn void mcskel3d_K3_MarkObj(struct xvimage *k)
    \param k : un complexe
    \brief marque FLAG_OBJ toutes les faces du complexe k
*/
void mcskel3d_K3_MarkObj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkObj"
  index_t i, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i]) K[i] = FLAG_OBJ; else K[i] = 0;
} // mcskel3d_K3_MarkObj()


/* ========================================== */
index_t mcskel3d_K3_CardObj(struct xvimage *k)
/* ========================================== */
{
  index_t i, N = rowsize(k) * colsize(k) * depth(k), n=0;
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i]) n++;
  return n;
} // mcskel3d_K3_CardObj()

// already implemented in mcskel2d3d.c
/* ========================================== */
/*! \fn void mcskel3d_K3_Binarize(struct xvimage *k)
    \param k : un complexe
    \brief marque NDG_MAX toutes les faces du complexe k
*/
void mcskel3d_K3_Binarize(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_Binarize"
  index_t i, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i]) K[i] = NDG_MAX; else K[i] = 0;
} // mcskel3d_K3_Binarize()

// already implemented in mcskel2d3d.c
/* ========================================== */
/*! \fn void mcskel3d_K3_SelMarked(struct xvimage *k, unsigned char mask)
    \param k : un complexe
    \param mask : une marque (mot 8 bits avec un seul bit à 1)
    \brief met a FLAG_OBJ tous le éléments marqués, à 0 les autres
*/
void mcskel3d_K3_SelMarked(struct xvimage *k, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_SelMarked"
  index_t i, N = rowsize(k) * colsize(k) * depth(k);
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) if (K[i] & mask) K[i] = FLAG_OBJ; else K[i] = 0;
} // mcskel3d_K3_SelMarked()

// already implemented in mcskel2d3d.c
/* ========================================== */
/*! \fn void mcskel3d_K3_MarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
    \param k : un complexe
    \param f : une face
    \param mask : valeur de la marque (mot 8 bits avec un seul bit à 1)
    \brief ajoute la marque mask à tous les éléments de alphacarre(f)
*/
void mcskel3d_K3_MarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkAlphaCarre"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs; 
  int32_t x = f%rs, y = (f%ps)/rs, z = f/ps;
  int32_t tab[26], u, n;
  unsigned char *K = UCHARDATA(k);
  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) K[tab[u]] |= mask;
} // mcskel3d_K3_MarkAlphaCarre()

/* ========================================== */
/*! \fn void mcskel3d_K3_UnMarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
    \param k : un complexe
    \param f : une face
    \param mask : valeur de la marque (mot 8 bits avec un seul bit à 1)
    \brief retire la marque mask de tous les éléments de alphacarre(f)
*/
void mcskel3d_K3_UnMarkAlphaCarre(struct xvimage *k, index_t f, unsigned char mask)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_UnMarkAlphaCarre"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs; 
  int32_t x = f%rs, y = (f%ps)/rs, z = f/ps;
  int32_t tab[26], u, n;
  unsigned char *K = UCHARDATA(k);
  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) K[tab[u]] &= ~mask;
} // mcskel3d_K3_UnMarkAlphaCarre()

// already implemented in mcskel2d3d.c
/* ========================================== */
/*! \fn void mcskel3d_K3_MarkPrinc(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque face principale f de k, rajoute (OR) la marque FLAG_PRINC au label de f
*/
void mcskel3d_K3_MarkPrinc(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkPrinc"
  int32_t card, x, y, z;
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, i; 
  unsigned char *K = UCHARDATA(k);
  int32_t tab[26], u, n;
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
} // mcskel3d_K3_MarkPrinc()

static uint8_t *EssTab3 = NULL;

/* ========================================== */
/*! \fn void mcskel3d_K3_MarkEss(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque face essentielle f de k, rajoute (OR) la marque FLAG_ESS au label de f
    \warning les faces principales doivent avoir été marquées auparavant
*/
void mcskel3d_K3_MarkEss(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkEss"
  int32_t x, y, z; 
  index_t i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *P;
  int32_t ret, tab[26], u, n;
  char tablefilename[256];

  if (EssTab3 == NULL) 
  {
    FILE *fd;
    int32_t tablesize = 1<<23;

    // ON EN PROFITE POUR INITIALISER LA TABLE DES POINTS SIMPLES
    mctopo3d_table_init_topo3d();

    EssTab3 = malloc(tablesize);
    if (EssTab3 == NULL)
    {
      fprintf(stderr, "%s: malloc failed\n", F_NAME);
      exit(1);
    }
//    sprintf(tablefilename, "%s/src/tables/%s", getenv("PINK"), ESS3DTABNAME);
    sprintf(tablefilename, "/tmp/%s", ESS3DTABNAME);
    fd = fopen (tablefilename, "r");
    if (fd == NULL) 
    {   
      char command[256];
      sprintf(command, "cp %s/src/tables/%s %s", getenv("PINK"), ESS3DTABNAME, tablefilename);
      (void)system(command);
      fd = fopen (tablefilename, "r");
      if (fd == NULL) 
      {
	fprintf(stderr, "%s: error while opening table\n", F_NAME);
	exit(1);
      }
    }
    ret = fread(EssTab3, sizeof(char), tablesize, fd);
    if (ret != tablesize)
    {
      fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, tablesize, ret);
      exit(1);
    }
#ifdef DEBUG
  printf("%s: %s loaded\n", F_NAME, ESS3DTABNAME);
#endif
    fclose(fd);
  }

  P = calloc(1, N);
  if (P == NULL)
  {
    fprintf(stderr, "%s: calloc failed\n", F_NAME);
    exit(1);
  }

  for (i = 0; i < N; i++) if (IS_PRINC(K[i])) P[i] = 1;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_OBJ(K[i])) 
    {
      if (CUBE3D(x,y,z)) 
      { 
	if (P[i]) K[i] |= FLAG_ESS; 
      }
      else if (CARRE3D(x,y,z)) 
      {
	int32_t card = 0;
	Betacarre3d(rs, cs, ds, x, y, z, tab, &n);
	for (u = 0; u < n; u++) if (P[tab[u]]) card++;
	if (P[i] && ((card == 0) || (card == 2))) K[i] |= FLAG_ESS; 
	if (!P[i] && (card == 2)) K[i] |= FLAG_ESS; 
      }
      else if (INTER3D(x,y,z)) 
      {
	uint32_t mask = XBetacarre3d(P, rs, cs, ds, x, y, z);
	if ((INTER3DX(x,y,z) && EssTab2X[mask]) || 
	    (INTER3DY(x,y,z) && EssTab2Y[mask]) || 
	    (INTER3DZ(x,y,z) && EssTab2Z[mask]))
	    K[i] |= FLAG_ESS;
      }
      else // SINGL3D
      {
	uint32_t mask = XBetacarre3d(P, rs, cs, ds, x, y, z);
	if (IsEss3(mask)) K[i] |= FLAG_ESS;
      }
    }
  }
  free(P);
} // mcskel3d_K3_MarkEss()

/* ========================================== */
/*! \fn int32_t mcskel3d_K3_MarkCore(struct xvimage *k, index_t f)
    \param k : un complexe
    \param f : une face de k
    \return le cardinal (nombre de faces) de la fermeture du noyau
    \brief marque le complexe formé du noyau (core) de la face f pour le complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant
*/
int32_t mcskel3d_K3_MarkCore(struct xvimage *k, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkCore"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs * cs, nf = 0, i, j;
  unsigned char *K = UCHARDATA(k);
  int32_t tab[26], tabi[26], u, v, n, ni;
  int32_t x = f % rs, y = (f % ps) / rs, z = f / ps, xi, yi, zi;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) 
  {
    i = tab[u];
    if (IS_ESS(K[i]))
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE
printf("mark_core : %d,%d,%d\n", i%rs, (i%ps)/rs, i/ps);
#endif
      } 
      xi = i % rs; yi = (i%ps) / rs; zi = i / ps;
      Alphacarre3d(rs, cs, ds, xi, yi, zi, tabi, &ni);
      for (v = 0; v < ni; v++) 
      {
	j = tabi[v];
#ifdef DEBUGMARKCORE
printf("PRE_mark_core_aux : %d,%d,%d [%x]\n", j%rs, (j%ps)/rs, j/ps, K[j]);
#endif
	if (!(IS_CORE(K[j]))) 
	{ 
	  K[j] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE
printf("mark_core_aux : %d,%d,%d\n", j%rs, (j%ps)/rs, j/ps);
#endif
	} 
      }
    }
  }
  return nf;
} // mcskel3d_K3_MarkCore()

/* ========================================== */
/*! \fn int32_t mcskel3d_K3_MarkCore2(struct xvimage *k, struct xvimage *m, index_t f)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \param f : une face de k
    \return le cardinal (nombre de faces) de la fermeture du noyau de f pour k, conditionné par m
    \brief marque le complexe formé du noyau (core) de la face f pour le complexe k, conditionné par m
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
int32_t mcskel3d_K3_MarkCore2(struct xvimage *k, struct xvimage *m, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkCore2"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, nf = 0, i, j;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  int32_t tab[26], tabi[26], u, v, n, ni;
  int32_t x = f % rs, y = (f % ps) / rs, z = f / ps, xi, yi, zi;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) 
  {
    i = tab[u];
    xi = i % rs; yi = (i%ps) / rs; zi = i / ps;
    if (M[i])
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE2
printf("mark_core2 (marqueur) : %d,%d,%d\n", i%rs, (i%ps)/rs, i/ps);
#endif
	Alphacarre3d(rs, cs, ds, xi, yi, zi, tabi, &ni);
	for (v = 0; v < ni; v++) 
        {
	  j = tabi[v];
	  if (!(IS_CORE(K[j]))) 
	  { 
	    K[j] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE2
printf("mark_core2_aux : %d,%d,%d\n", j%rs, (j%ps)/rs, j/ps);
#endif
	  } 
	}
      } 
    }
    else if (IS_ESS(K[i]))
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE2
printf("mark_core2 (ess) : %d,%d,%d\n", i%rs, (i%ps)/rs, i/ps);
#endif        
	Alphacarre3d(rs, cs, ds, xi, yi, zi, tabi, &ni);
	for (v = 0; v < ni; v++) 
        {
	  j = tabi[v];
	  if (!(IS_CORE(K[j]))) 
	  { 
	    K[j] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE2
printf("mark_core2_aux : %d,%d,%d\n", j%rs, (j%ps)/rs, j/ps);
#endif
	  } 
	}
      }
    }
  }
  return nf;
} // mcskel3d_K3_MarkCore2()

/* ========================================== */
/*! \fn int32_t mcskel3d_K3_CardCore(struct xvimage *k, index_t f)
    \param k : un complexe
    \param f : une face de k
    \return un entier 
    \brief retourne le cardinal du noyau de f, i.e., le nombre de faces marquées MARK3_ESS du noyau 
    \warning les faces essentielles doivent avoir été marquées auparavant
*/
int32_t mcskel3d_K3_CardCore(struct xvimage *k, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_CardCore"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, nf = 0;
  unsigned char *K = UCHARDATA(k);
  int32_t tab[26], u, n;
  int32_t x = f % rs, y = (f % ps) / rs, z = f / ps;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) if (IS_ESS(K[tab[u]])) nf++; 
  return nf;
} // mcskel3d_K3_CardCore()

/* ========================================== */
/*! \fn int32_t mcskel3d_K3_CardCore2(struct xvimage *k, struct xvimage *m, index_t f)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \param f : une face de k
    \return un entier 
    \brief retourne le cardinal du noyau de f pour k conditionné par m, i.e.,
    le nombre de faces marquées MARK3_ESS ou appartenant à m dans le noyau 
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
int32_t mcskel3d_K3_CardCore2(struct xvimage *k, struct xvimage *m, index_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_CardCore2"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, nf = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  int32_t tab[26], u, n;
  int32_t x = f % rs, y = (f % ps) / rs, z = f / ps;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) if (IS_ESS(K[tab[u]]) || M[tab[u]]) nf++; 
  return nf;
} // mcskel3d_K3_CardCore2()

/* ==================================== */
int32_t  get3Dconfiguration(
  uint8_t *img,                   /* pointeur base image */
  int32_t flag,
  index_t p,                       /* index du point */
  index_t rs,                      /* taille rangee */
  index_t ps,                      /* taille plan */
  index_t N)                       /* taille image */
/* ==================================== */
{
  uint32_t mask = 0, v, k;
  for (k = 0; k < 26; k++)
  {    
    v = voisin26(p, k, rs, ps, N);
    if (img[v] & flag) mask = mask | (1 << k);
  }
  return mask;
} /* get3Dconfiguration() */

/* ==================================== */
int32_t  get3Dcount(
  uint8_t *img,                   /* pointeur base image */
  int32_t flag,
  index_t p,                       /* index du point */
  index_t rs,                      /* taille rangee */
  index_t ps,                      /* taille plan */
  index_t N)                       /* taille image */
/* ==================================== */
{
  uint32_t count = 0, v, k;
  for (k = 0; k < 26; k++)
  {    
    v = voisin26(p, k, rs, ps, N);
    if (img[v] & flag) count++;
  }
  return count;
} /* get3Dcount() */

/* ==================================== */
void print3Dconfiguration(int32_t mask)
/* ==================================== */
{
  uint32_t k;
  for (k = 0; k < 3; k++) if (mask & (1 << k)) printf("1 "); else printf("0 ");
  printf("\n");
  for (k = 3; k < 6; k++) if (mask & (1 << k)) printf("1 "); else printf("0 ");
  printf("\n");
  for (k = 6; k < 9; k++) if (mask & (1 << k)) printf("1 "); else printf("0 ");
  printf("\n\n");

  for (k = 9; k < 12; k++) if (mask & (1 << k)) printf("1 "); else printf("0 ");
  printf("\n");
  if (mask & (1 << 12)) printf("1 "); else printf("0 ");
  printf("1 ");
  if (mask & (1 << 13)) printf("1 "); else printf("0 ");
  printf("\n");
  for (k = 14; k < 17; k++) if (mask & (1 << k)) printf("1 "); else printf("0 ");
  printf("\n\n");

  for (k = 17; k < 20; k++) if (mask & (1 << k)) printf("1 "); else printf("0 ");
  printf("\n");
  for (k = 20; k < 23; k++) if (mask & (1 << k)) printf("1 "); else printf("0 ");
  printf("\n");
  for (k = 23; k < 26; k++) if (mask & (1 << k)) printf("1 "); else printf("0 ");
  printf("\n\n");
} /* print3Dconfiguration() */

/* ==================================== */
int32_t get2Dconfiguration(
  uint8_t *img,                   /* pointeur base image */
  int32_t flag,
  int32_t x,
  int32_t y,
  int32_t z,
  index_t rs,
  index_t ps)
/* ==================================== */
{
  uint32_t mask = 0;

  if (z%2==0)
  {
    if (img[z*ps + y    *rs + x+1] & flag) mask  = 1;
    if (img[z*ps + (y-1)*rs + x+1] & flag) mask |= 2;
    if (img[z*ps + (y-1)*rs + x  ] & flag) mask |= 4;
    if (img[z*ps + (y-1)*rs + x-1] & flag) mask |= 8;
    if (img[z*ps + y    *rs + x-1] & flag) mask |= 16;
    if (img[z*ps + (y+1)*rs + x-1] & flag) mask |= 32;
    if (img[z*ps + (y+1)*rs + x  ] & flag) mask |= 64;
    if (img[z*ps + (y+1)*rs + x+1] & flag) mask |= 128;
  }

  if (y%2==0)
  {
    if (img[z    *ps + y*rs + x+1] & flag) mask  = 1;
    if (img[(z-1)*ps + y*rs + x+1] & flag) mask |= 2;
    if (img[(z-1)*ps + y*rs + x  ] & flag) mask |= 4;
    if (img[(z-1)*ps + y*rs + x-1] & flag) mask |= 8;
    if (img[z    *ps + y*rs + x-1] & flag) mask |= 16;
    if (img[(z+1)*ps + y*rs + x-1] & flag) mask |= 32;
    if (img[(z+1)*ps + y*rs + x  ] & flag) mask |= 64;
    if (img[(z+1)*ps + y*rs + x+1] & flag) mask |= 128;
  }

  if (x%2==0)
  {
    if (img[(z+1)*ps + y    *rs + x] & flag) mask  = 1;
    if (img[(z+1)*ps + (y-1)*rs + x] & flag) mask |= 2;
    if (img[z    *ps + (y-1)*rs + x] & flag) mask |= 4;
    if (img[(z-1)*ps + (y-1)*rs + x] & flag) mask |= 8;
    if (img[(z-1)*ps + y    *rs + x] & flag) mask |= 16;
    if (img[(z-1)*ps + (y+1)*rs + x] & flag) mask |= 32;
    if (img[z    *ps + (y+1)*rs + x] & flag) mask |= 64;
    if (img[(z+1)*ps + (y+1)*rs + x] & flag) mask |= 128;
  }

  return mask;
} /* get2Dconfiguration() */

/* ==================================== */
int32_t get2Dcount(
  uint8_t *img,                   /* pointeur base image */
  int32_t flag,
  int32_t x,
  int32_t y,
  int32_t z,
  index_t rs,
  index_t ps)
/* ==================================== */
{
  uint32_t count = 0;

  if (z%2==0)
  {
    if (img[z*ps + y    *rs + x+1] & flag) count++;
    if (img[z*ps + (y-1)*rs + x+1] & flag) count++;
    if (img[z*ps + (y-1)*rs + x  ] & flag) count++;
    if (img[z*ps + (y-1)*rs + x-1] & flag) count++;
    if (img[z*ps + y    *rs + x-1] & flag) count++;
    if (img[z*ps + (y+1)*rs + x-1] & flag) count++;
    if (img[z*ps + (y+1)*rs + x  ] & flag) count++;
    if (img[z*ps + (y+1)*rs + x+1] & flag) count++;
  }

  if (y%2==0)
  {
    if (img[z    *ps + y*rs + x+1] & flag) count++;
    if (img[(z-1)*ps + y*rs + x+1] & flag) count++;
    if (img[(z-1)*ps + y*rs + x  ] & flag) count++;
    if (img[(z-1)*ps + y*rs + x-1] & flag) count++;
    if (img[z    *ps + y*rs + x-1] & flag) count++;
    if (img[(z+1)*ps + y*rs + x-1] & flag) count++;
    if (img[(z+1)*ps + y*rs + x  ] & flag) count++;
    if (img[(z+1)*ps + y*rs + x+1] & flag) count++;
  }

  if (x%2==0)
  {
    if (img[(z+1)*ps + y    *rs + x] & flag) count++;
    if (img[(z+1)*ps + (y-1)*rs + x] & flag) count++;
    if (img[z    *ps + (y-1)*rs + x] & flag) count++;
    if (img[(z-1)*ps + (y-1)*rs + x] & flag) count++;
    if (img[(z-1)*ps + y    *rs + x] & flag) count++;
    if (img[(z-1)*ps + (y+1)*rs + x] & flag) count++;
    if (img[z    *ps + (y+1)*rs + x] & flag) count++;
    if (img[(z+1)*ps + (y+1)*rs + x] & flag) count++;
  }

  return count;
} /* get2Dcount() */

/* ========================================== */
/*! \fn void mcskel3d_K3_MarkCritic(struct xvimage *k)
    \param k : un complexe
    \brief marque FLAG_CRITIC_E les fermetures des faces critiques du complexe k
           et marque FLAG_CRITIC les faces critiques du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant
*/
void mcskel3d_K3_MarkCritic(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkCritic"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds, i;
  unsigned char *K = UCHARDATA(k);
  int32_t x, y, z, n, ncore;
  uint32_t mask;

  for (i = 0; i < N; i++) if (IS_ESS(K[i])) K[i] |= FLAG_CRITIC; 

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    if (CARRE3D(x,y,z)) n = 8; else 
      if (INTER3D(x,y,z)) n = 2; else 
	if (SINGL3D(x,y,z)) n = 0; else n = 26; 
    i = z*ps + y*rs + x;

    if (IS_ESS(K[i])) 
    {
      ncore = mcskel3d_K3_MarkCore(k, i);
#ifdef DEBUGMARKCRITIC
printf("%s ess : %d,%d,%d ; n = %d, ncore = %d\n", F_NAME, x, y, z, n, ncore);
#endif
      if (ncore == 0) goto next;
      if (ncore == n) 
      { 
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE); 
	goto next; 
      }
      // le core d'un singl est forcément vide (cas déjà traité)
      if (INTER3D(x,y,z))
      {
	if (ncore == 1) 
	{
          K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC
printf("  inter, REGUL\n");
#endif
        }
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      if (CARRE3D(x,y,z))
      {
	mask = get2Dconfiguration(K, FLAG_CORE, x, y, z, rs, ps);
	if ((t8(mask) == 1) && (t4b(mask) == 1))
	{
	  K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC
printf("  carre, REGUL\n");
#endif
	}
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      //if (CUBE3D(x,y,z)) // inutile car c'est le seul choix qui reste
      mask = get3Dconfiguration(K, FLAG_CORE, i, rs, ps, N);
      if (mctopo3d_table_simple26mask(mask)) 
      {
	K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC
printf("  cube, REGUL\n");
#endif
      }
      mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);

    next:;
    }
  }

  for (i = 0; i < N; i++) 
    if (IS_CRITIC(K[i])) 
    {
      K[i] |= FLAG_CRITIC_E;
      mcskel3d_K3_MarkAlphaCarre(k, i, FLAG_CRITIC_E);
    }
} // mcskel3d_K3_MarkCritic()

/* ========================================== */
/*! \fn void mcskel3d_K3_MarkCritic2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \brief marque FLAG_CRITIC_E les fermetures des faces critiques du complexe k, conditionné par m, et 
           marque FLAG_CRITIC les faces critiques du complexe k (cond. m)
    (m est un ensemble de faces qui doivent être préservées)
    \warning les faces essentielles (conditionnellement à m) doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel3d_K3_MarkCritic2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkCritic2"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds, i;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  int32_t x, y, z, n, ncore;
  uint32_t mask;

  for (i = 0; i < N; i++) if (IS_ESS(K[i])) K[i] |= FLAG_CRITIC; 

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    if (CARRE3D(x,y,z)) n = 8; else 
      if (INTER3D(x,y,z)) n = 2; else 
	if (SINGL3D(x,y,z)) n = 0; else n = 26; 
    i = z*ps + y*rs + x;
    if (IS_ESS(K[i])) 
    {
      ncore = mcskel3d_K3_MarkCore2(k, m, i);
#ifdef DEBUGMARKCRITIC2
printf("%s ess : %d,%d,%d ; n = %d, ncore = %d\n", F_NAME, x, y, z, n, ncore);
#endif
      if (ncore == 0) goto next;
      if (ncore == n) 
      { 
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE); 
	goto next; 
      }
      // le core d'un singl est forcément vide (cas déjà traité)
      if (INTER3D(x,y,z))
      {
	if (ncore == 1) 
	{  
	  K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC2
printf("  inter, REGUL\n");
#endif
	}
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      if (CARRE3D(x,y,z))
      {
	mask = get2Dconfiguration(K, FLAG_CORE, x, y, z, rs, ps);
	if ((t8(mask) == 1) && (t4b(mask) == 1)) 
	{
	  K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC2
printf("  carre, REGUL\n");
#endif
	}
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      //if (CUBE3D(x,y,z)) // inutile car c'est le seul choix qui reste
      mask = get3Dconfiguration(K, FLAG_CORE, i, rs, ps, N);
      if (mctopo3d_table_simple26mask(mask)) 
      {	
	K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC2
printf("  cube, REGUL\n");
print3Dconfiguration(mask);
#endif
      }
      mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);

    next:;
    }
  }

  for (i = 0; i < N; i++) if (M[i] || IS_CRITIC(K[i]))
  {
    K[i] |= FLAG_CRITIC_E;
    K[i] |= FLAG_CRITIC;
    mcskel3d_K3_MarkAlphaCarre(k, i, FLAG_CRITIC_E);
  }
} // mcskel3d_K3_MarkCritic2()

/* ========================================== */
/*! \fn void mcskel3d_K3_MarkMCritic(struct xvimage *k)
    \param k : un complexe 
    \brief marque FLAG_CRITIC les faces m-critiques du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant
    \warning utilise le flag TMP1
*/
void mcskel3d_K3_MarkMCritic(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkMCritic"
  index_t i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  int32_t tab[26], u, x, y, z, n, ncore;
  uint32_t mask;

  for (i = 0; i < N; i++) if (IS_ESS(K[i])) K[i] |= FLAG_CRITIC; 

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    if (CARRE3D(x,y,z)) n = 8; else 
      if (INTER3D(x,y,z)) n = 2; else 
	if (SINGL3D(x,y,z)) n = 0; else n = 26; 
    i = z*ps + y*rs + x;

    if (IS_ESS(K[i])) 
    {
      ncore = mcskel3d_K3_MarkCore(k, i);
#ifdef DEBUGMARKCRITIC
      printf("%s ess : %d,%d,%d ; n = %d, ncore = %d\n", F_NAME, x, y, z, n, ncore);
#endif
      if (ncore == 0) goto next;
      if (ncore == n) 
      { 
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE); 
	goto next; 
      }
      // le core d'un singl est forcément vide (cas déjà traité)
      if (INTER3D(x,y,z))
      {
	if (ncore == 1) K[i] &= ~FLAG_CRITIC;
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      if (CARRE3D(x,y,z))
      {
	mask = get2Dconfiguration(K, FLAG_CORE, x, y, z, rs, ps);
	if ((t8(mask) == 1) && (t4b(mask) == 1)) K[i] &= ~FLAG_CRITIC;
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
      //if (CUBE3D(x,y,z)) // inutile car c'est le seul choix qui reste
      mask = get3Dconfiguration(K, FLAG_CORE, i, rs, ps, N);
      if (mctopo3d_table_simple26mask(mask)) K[i] &= ~FLAG_CRITIC;
      mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);

    next:;
    }
  }

  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    Betacarre3d(rs, cs, ds, i%rs, (i%ps)/rs, i/ps, tab, &n);
    for (u = 0; u < n; u++) if (IS_CRITIC(K[tab[u]])) break;	
    if (u == n) K[i] |= FLAG_TMP1;
  }
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    if (!IS_TMP1(K[i])) K[i] &= ~FLAG_CRITIC;
    K[i] &= ~FLAG_TMP1;
  }
} // mcskel3d_K3_MarkMCritic()

/* ========================================== */
/*! \fn void mcskel3d_K3_MarkMCritic2(struct xvimage *k, struct xvimage *m)
    \param k : un complexe 
    \param m : un complexe "marqueur"
    \brief marque FLAG_CRITIC les faces M-critiques du complexe k, conditionné par m
    \warning les faces essentielles (conditionnellement à m) doivent avoir été marquées auparavant
    \warning pas de vérification de compatibilité entre k et m
    \warning utilise le flag TMP1
*/
void mcskel3d_K3_MarkMCritic2(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkMCritic2"
  index_t i, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  int32_t tab[26], x, y, z, u, n, ncore;
  uint32_t mask;

  for (i = 0; i < N; i++) if (IS_ESS(K[i])) K[i] |= FLAG_CRITIC; 

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    if (CARRE3D(x,y,z)) n = 8; else 
      if (INTER3D(x,y,z)) n = 2; else 
	if (SINGL3D(x,y,z)) n = 0; else n = 26; 
    i = z*ps + y*rs + x;
    if (IS_ESS(K[i])) 
    {
      ncore = mcskel3d_K3_MarkCore2(k, m, i);
#ifdef DEBUGMARKCRITIC2
printf("%s ess : %d,%d,%d ; n = %d, ncore = %d\n", F_NAME, x, y, z, n, ncore);
#endif
      if (ncore == 0) 
      { 
	K[i] |= FLAG_CRITIC;
	goto next;
      }
      if (ncore == n) 
      { 
	K[i] |= FLAG_CRITIC;
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE); 
	goto next; 
      }
      // le core d'un singl est forcément vide (cas déjà traité)
      if (INTER3D(x,y,z))
      {
	if (ncore == 1) 
	{  
	  K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC2
printf("  inter, REGUL\n");
#endif
	}
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
	goto next;
      }
      if (CARRE3D(x,y,z))
      {
	mask = get2Dconfiguration(K, FLAG_CORE, x, y, z, rs, ps);
	if ((t8(mask) == 1) && (t4b(mask) == 1)) 
	{
	  K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC2
printf("  carre, REGUL\n");
#endif
	}
	mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);
      }
      //if (CUBE3D(x,y,z)) // inutile car c'est le seul choix qui reste
      mask = get3Dconfiguration(K, FLAG_CORE, i, rs, ps, N);
      if (mctopo3d_table_simple26mask(mask)) 
      {	
	K[i] &= ~FLAG_CRITIC;
#ifdef DEBUGMARKCRITIC2
printf("  cube, REGUL\n");
print3Dconfiguration(mask);
#endif
      }
      mcskel3d_K3_UnMarkAlphaCarre(k, i, FLAG_CORE);

    next:;
    }
  }

  for (i = 0; i < N; i++) if (M[i]) K[i] |= FLAG_CRITIC;
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    Betacarre3d(rs, cs, ds, i%rs, (i%ps)/rs, i/ps, tab, &n);
    for (u = 0; u < n; u++) if (IS_CRITIC(K[tab[u]])) break;	
    if (u == n) K[i] |= FLAG_TMP1;
  }
  for (i = 0; i < N; i++) if (IS_CRITIC(K[i])) 
  {
    if (!IS_TMP1(K[i])) K[i] &= ~FLAG_CRITIC;
    K[i] &= ~FLAG_TMP1;
  }
} // mcskel3d_K3_MarkMCritic2()

/* ========================================== */
/*! \fn index_t mcskel3d_K3_Critic2Obj(struct xvimage *k)
    \param k : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les faces marquées FLAG_CRITIC du complexe k et remet à 0 les autres marques
*/
index_t mcskel3d_K3_Critic2Obj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_Critic2Obj"
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
} // mcskel3d_K3_Critic2Obj()

/* ========================================== */
/*! \fn index_t mcskel3d_K3_CriticE2Obj(struct xvimage *k)
    \param k : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les faces marquées FLAG_CRITIC_E du complexe k et remet à 0 les autres marques
*/
index_t mcskel3d_K3_CriticE2Obj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_CriticE2Obj"
  index_t i, N = rowsize(k) * colsize(k) * depth(k), n = 0;
  unsigned char *K = UCHARDATA(k);
  for (i = 0; i < N; i++) 
    if (IS_CRITIC_E(K[i]))
    {
      K[i] = FLAG_OBJ;
      n++;
    }
    else
      K[i] = 0;
  return n;
} // mcskel3d_K3_CriticE2Obj()

/* ========================================== */
/*! \fn index_t mcskel3d_K3_MCritic2Obj(struct xvimage *k)
    \param k : un complexe
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les fermetures des faces non nulles 
    contenant des faces m-critiques (marquées FLAG_CRITIC) 
    du complexe k et remet à 0 les autres marques
*/
index_t mcskel3d_K3_MCritic2Obj(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MCritic2Obj"
  index_t i, j, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds, n = 0;
  unsigned char *K = UCHARDATA(k);
  int32_t tab1[26], tab2[26], u, v, n1, n2, x, y, z;
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
	if (K[j])
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
} // mcskel3d_K3_MCritic2Obj()

/* ========================================== */
/*! \fn index_t mcskel3d_K3_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \return le nombre de faces objet trouvées
    \brief marque FLAG_OBJ les fermetures des faces 
    contenant des faces m-critiques (marquées FLAG_CRITIC)
    du complexe k ou des faces de m et remet à 0 les autres marques.
*/
index_t mcskel3d_K3_MCriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MCriticOrMarked2Obj"
  index_t i, j, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds, n = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  int32_t tab1[26], tab2[26], u, v, n1, n2, x, y, z;
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
	K[j] |= FLAG_OBJ;
	Alphacarre3d(rs, cs, ds, j%rs, (j%ps)/rs, j/ps, tab2, &n2);
	for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
      }
      Alphacarre3d(rs, cs, ds, x, y, z, tab2, &n2);
      for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
    }
  }

  for (i = 0; i < N; i++) 
    if (IS_OBJ(K[i])) { n++; K[i] = FLAG_OBJ; } else K[i] = 0;
  return n;
} // mcskel3d_K3_MCriticOrMarked2Obj()

/* ========================================== */
/*! \fn index_t mcskel3d_K3_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    \return le nombre de faces critiques ou marquées trouvées
    \brief marque FLAG_OBJ les faces CRITIC ou CRITIC_E du complexe k ou marquées par m et remet à 0 les flags dans k
*/
index_t mcskel3d_K3_CriticOrMarked2Obj(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_CriticOrMarked2Obj"
  index_t i, N = rowsize(k) * colsize(k) * depth(k), nobj = 0;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  for (i = 0; i < N; i++) 
    if (M[i] || IS_CRITIC(K[i]) || IS_CRITIC_E(K[i])) { K[i] = FLAG_OBJ; nobj++; } else K[i] = 0;
  return nobj;
} // mcskel3d_K3_CriticOrMarked2Obj()

/* ========================================== */
/*! \fn void mcskel3d_K3_HitPrinc(struct xvimage *k)
    \param k : un complexe
    \brief pour chaque face f marquée FLAG_PRINC de k, marque FLAG_OBJ le alpha du beta de f
*/
void mcskel3d_K3_HitPrinc(struct xvimage *k)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_HitPrinc"
  index_t i, j, rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs;
  unsigned char *K = UCHARDATA(k);
  int32_t tab1[26], tab2[26], u, v, n1, n2, x, y, z;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (IS_PRINC(K[i])) 
    {
      K[i] |= FLAG_OBJ;
      Betacarre3d(rs, cs, ds, x, y, z, tab1, &n1);
      for (u = 0; u < n1; u++) 
      {
	j = tab1[u];
	K[j] |= FLAG_OBJ;
	Alphacarre3d(rs, cs, ds, j%rs, (j%ps)/rs, j/ps, tab2, &n2);
	for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
      }
      Alphacarre3d(rs, cs, ds, x, y, z, tab2, &n2);
      for (v = 0; v < n2; v++) K[tab2[v]] |= FLAG_OBJ;
    }
  }
} // mcskel3d_K3_HitPrinc()

/* ==================================== */
void mcskel3d_K3_CopieComplCore3d(struct xvimage *b, int32_t i, int32_t j, int32_t k, struct xvimage *g)
/* ==================================== */
/* 
  Copie le complementaire du core du point i,j,k de B dans l'image G.
  G doit etre de taille 7x7x7.
*/    
{
  index_t rs = rowsize(b), cs = colsize(b), ds = depth(b), ps = rs*cs;
  unsigned char *B = UCHARDATA(b);
  unsigned char *G = UCHARDATA(g);
  int32_t x = GRS3D+(i%2);
  int32_t y = GCS3D+(j%2);
  int32_t z = GDS3D+(k%2);
  int32_t grs = 2 * GRS3D + 1;
  int32_t gps = grs * (2 * GCS3D + 1);
  int32_t gN = gps * (2 * GDS3D + 1);
  index_t I = k*ps+j*rs+i;

  memset(G, VAL_NULLE, gN); /* init a VAL_NULLE */

  (void)mcskel3d_K3_MarkCore(b, I);
  
  /* copie le Alphacarre */
  if (i % 2)
  {
    if (!(i>0)||!IS_CORE(B[k*ps+j*rs+i-1])) G[z*gps+y*grs+x-1] = VAL_OBJET;
    if (!((i+1)<rs)||!IS_CORE(B[k*ps+j*rs+i+1])) G[z*gps+y*grs+x+1] = VAL_OBJET;
  }
  if (j % 2)
  {
    if (!(j>0)||!IS_CORE(B[k*ps+(j-1)*rs+i])) G[z*gps+(y-1)*grs+x] = VAL_OBJET;
    if (!((j+1)<cs)||!IS_CORE(B[k*ps+(j+1)*rs+i])) G[z*gps+(y+1)*grs+x] = VAL_OBJET;
  }
  if (k % 2)
  {
    if (!(k>0)||!IS_CORE(B[(k-1)*ps+j*rs+i])) G[(z-1)*gps+y*grs+x] = VAL_OBJET;
    if (!((k+1)<ds)||!IS_CORE(B[(k+1)*ps+j*rs+i])) G[(z+1)*gps+y*grs+x] = VAL_OBJET;
  }
  if ((i%2) + (j%2) == 2)
  {
    if (!((i>0)&&(j>0))||!IS_CORE(B[k*ps+(j-1)*rs+i-1])) G[z*gps+(y-1)*grs+x-1] = VAL_OBJET;
    if (!((i>0)&&((j+1)<cs))||!IS_CORE(B[k*ps+(j+1)*rs+i-1])) G[z*gps+(y+1)*grs+x-1] = VAL_OBJET;
    if (!(((i+1)<rs)&&(j>0))||!IS_CORE(B[k*ps+(j-1)*rs+i+1])) G[z*gps+(y-1)*grs+x+1] = VAL_OBJET;
    if (!(((i+1)<rs)&&((j+1)<cs))||!IS_CORE(B[k*ps+(j+1)*rs+i+1])) G[z*gps+(y+1)*grs+x+1] = VAL_OBJET;
  }
  if ((i%2) + (k%2) == 2)
  {
    if (!((i>0)&&(k>0))||!IS_CORE(B[(k-1)*ps+j*rs+i-1])) G[(z-1)*gps+y*grs+x-1] = VAL_OBJET;
    if (!((i>0)&&((k+1)<ds))||!IS_CORE(B[(k+1)*ps+j*rs+i-1])) G[(z+1)*gps+y*grs+x-1] = VAL_OBJET;
    if (!(((i+1)<rs)&&(k>0))||!IS_CORE(B[(k-1)*ps+j*rs+i+1])) G[(z-1)*gps+y*grs+x+1] = VAL_OBJET;
    if (!(((i+1)<rs)&&((k+1)<ds))||!IS_CORE(B[(k+1)*ps+j*rs+i+1])) G[(z+1)*gps+y*grs+x+1] = VAL_OBJET;
  }
  if ((k%2) + (j%2) == 2)
  {
    if (!((k>0)&&(j>0))||!IS_CORE(B[(k-1)*ps+(j-1)*rs+i])) G[(z-1)*gps+(y-1)*grs+x] = VAL_OBJET;
    if (!((k>0)&&((j+1)<cs))||!IS_CORE(B[(k-1)*ps+(j+1)*rs+i])) G[(z-1)*gps+(y+1)*grs+x] = VAL_OBJET;
    if (!(((k+1)<ds)&&(j>0))||!IS_CORE(B[(k+1)*ps+(j-1)*rs+i])) G[(z+1)*gps+(y-1)*grs+x] = VAL_OBJET;
    if (!(((k+1)<ds)&&((j+1)<cs))||!IS_CORE(B[(k+1)*ps+(j+1)*rs+i])) G[(z+1)*gps+(y+1)*grs+x] = VAL_OBJET;
  }
  if ((i%2) + (j%2) + (k%2) == 3)
  {
    if (!((i>0)&&(j>0)&&(k>0))||!IS_CORE(B[(k-1)*ps+(j-1)*rs+i-1])) G[(z-1)*gps+(y-1)*grs+x-1] = VAL_OBJET;
    if (!((i>0)&&((j+1)<cs)&&(k>0))||!IS_CORE(B[(k-1)*ps+(j+1)*rs+i-1])) G[(z-1)*gps+(y+1)*grs+x-1] = VAL_OBJET;
    if (!(((i+1)<rs)&&(j>0)&&(k>0))||!IS_CORE(B[(k-1)*ps+(j-1)*rs+i+1])) G[(z-1)*gps+(y-1)*grs+x+1] = VAL_OBJET;
    if (!(((i+1)<rs)&&((j+1)<cs)&&(k>0))||!IS_CORE(B[(k-1)*ps+(j+1)*rs+i+1])) G[(z-1)*gps+(y+1)*grs+x+1] = VAL_OBJET;
    if (!((i>0)&&(j>0)&&((k+1)<ds))||!IS_CORE(B[(k+1)*ps+(j-1)*rs+i-1])) G[(z+1)*gps+(y-1)*grs+x-1] = VAL_OBJET;
    if (!((i>0)&&((j+1)<cs)&&((k+1)<ds))||!IS_CORE(B[(k+1)*ps+(j+1)*rs+i-1])) G[(z+1)*gps+(y+1)*grs+x-1] = VAL_OBJET;
    if (!(((i+1)<rs)&&(j>0)&&((k+1)<ds))||!IS_CORE(B[(k+1)*ps+(j-1)*rs+i+1])) G[(z+1)*gps+(y-1)*grs+x+1] = VAL_OBJET;
    if (!(((i+1)<rs)&&((j+1)<cs)&&((k+1)<ds))||!IS_CORE(B[(k+1)*ps+(j+1)*rs+i+1])) G[(z+1)*gps+(y+1)*grs+x+1] = VAL_OBJET;
  }

  mcskel3d_K3_UnMarkAlphaCarre(b, I, FLAG_CORE); 

} /* mcskel3d_K3_CopieComplCore3d() */

/* ==================================== */
void mcskel3d_K3_CopieCore3d(struct xvimage *b, int32_t i, int32_t j, int32_t k, struct xvimage *g)
/* ==================================== */
/* 
  Copie le core du point i,j,k de B dans l'image G.
  G doit etre de taille 7x7x7.
*/    
{
  index_t rs = rowsize(b), cs = colsize(b), ds = depth(b), ps = rs*cs;
  unsigned char *B = UCHARDATA(b);
  unsigned char *G = UCHARDATA(g);
  int32_t x = GRS3D+(i%2);
  int32_t y = GCS3D+(j%2);
  int32_t z = GDS3D+(k%2);
  int32_t grs = 2 * GRS3D + 1;
  int32_t gps = grs * (2 * GCS3D + 1);
  int32_t gN = gps * (2 * GDS3D + 1);
  index_t I = k*ps+j*rs+i;

  memset(G, VAL_NULLE, gN); /* init a VAL_NULLE */

  (void)mcskel3d_K3_MarkCore(b, I);
  
  /* copie le Alphacarre */
  if (i % 2)
  {
    if (!(i>0)||IS_CORE(B[k*ps+j*rs+i-1])) G[z*gps+y*grs+x-1] = VAL_OBJET;
    if (!((i+1)<rs)||IS_CORE(B[k*ps+j*rs+i+1])) G[z*gps+y*grs+x+1] = VAL_OBJET;
  }
  if (j % 2)
  {
    if (!(j>0)||IS_CORE(B[k*ps+(j-1)*rs+i])) G[z*gps+(y-1)*grs+x] = VAL_OBJET;
    if (!((j+1)<cs)||IS_CORE(B[k*ps+(j+1)*rs+i])) G[z*gps+(y+1)*grs+x] = VAL_OBJET;
  }
  if (k % 2)
  {
    if (!(k>0)||IS_CORE(B[(k-1)*ps+j*rs+i])) G[(z-1)*gps+y*grs+x] = VAL_OBJET;
    if (!((k+1)<ds)||IS_CORE(B[(k+1)*ps+j*rs+i])) G[(z+1)*gps+y*grs+x] = VAL_OBJET;
  }
  if ((i%2) + (j%2) == 2)
  {
    if (!((i>0)&&(j>0))||IS_CORE(B[k*ps+(j-1)*rs+i-1])) G[z*gps+(y-1)*grs+x-1] = VAL_OBJET;
    if (!((i>0)&&((j+1)<cs))||IS_CORE(B[k*ps+(j+1)*rs+i-1])) G[z*gps+(y+1)*grs+x-1] = VAL_OBJET;
    if (!(((i+1)<rs)&&(j>0))||IS_CORE(B[k*ps+(j-1)*rs+i+1])) G[z*gps+(y-1)*grs+x+1] = VAL_OBJET;
    if (!(((i+1)<rs)&&((j+1)<cs))||IS_CORE(B[k*ps+(j+1)*rs+i+1])) G[z*gps+(y+1)*grs+x+1] = VAL_OBJET;
  }
  if ((i%2) + (k%2) == 2)
  {
    if (!((i>0)&&(k>0))||IS_CORE(B[(k-1)*ps+j*rs+i-1])) G[(z-1)*gps+y*grs+x-1] = VAL_OBJET;
    if (!((i>0)&&((k+1)<ds))||IS_CORE(B[(k+1)*ps+j*rs+i-1])) G[(z+1)*gps+y*grs+x-1] = VAL_OBJET;
    if (!(((i+1)<rs)&&(k>0))||IS_CORE(B[(k-1)*ps+j*rs+i+1])) G[(z-1)*gps+y*grs+x+1] = VAL_OBJET;
    if (!(((i+1)<rs)&&((k+1)<ds))||IS_CORE(B[(k+1)*ps+j*rs+i+1])) G[(z+1)*gps+y*grs+x+1] = VAL_OBJET;
  }
  if ((k%2) + (j%2) == 2)
  {
    if (!((k>0)&&(j>0))||IS_CORE(B[(k-1)*ps+(j-1)*rs+i])) G[(z-1)*gps+(y-1)*grs+x] = VAL_OBJET;
    if (!((k>0)&&((j+1)<cs))||IS_CORE(B[(k-1)*ps+(j+1)*rs+i])) G[(z-1)*gps+(y+1)*grs+x] = VAL_OBJET;
    if (!(((k+1)<ds)&&(j>0))||IS_CORE(B[(k+1)*ps+(j-1)*rs+i])) G[(z+1)*gps+(y-1)*grs+x] = VAL_OBJET;
    if (!(((k+1)<ds)&&((j+1)<cs))||IS_CORE(B[(k+1)*ps+(j+1)*rs+i])) G[(z+1)*gps+(y+1)*grs+x] = VAL_OBJET;
  }
  if ((i%2) + (j%2) + (k%2) == 3)
  {
    if (!((i>0)&&(j>0)&&(k>0))||IS_CORE(B[(k-1)*ps+(j-1)*rs+i-1])) G[(z-1)*gps+(y-1)*grs+x-1] = VAL_OBJET;
    if (!((i>0)&&((j+1)<cs)&&(k>0))||IS_CORE(B[(k-1)*ps+(j+1)*rs+i-1])) G[(z-1)*gps+(y+1)*grs+x-1] = VAL_OBJET;
    if (!(((i+1)<rs)&&(j>0)&&(k>0))||IS_CORE(B[(k-1)*ps+(j-1)*rs+i+1])) G[(z-1)*gps+(y-1)*grs+x+1] = VAL_OBJET;
    if (!(((i+1)<rs)&&((j+1)<cs)&&(k>0))||IS_CORE(B[(k-1)*ps+(j+1)*rs+i+1])) G[(z-1)*gps+(y+1)*grs+x+1] = VAL_OBJET;
    if (!((i>0)&&(j>0)&&((k+1)<ds))||IS_CORE(B[(k+1)*ps+(j-1)*rs+i-1])) G[(z+1)*gps+(y-1)*grs+x-1] = VAL_OBJET;
    if (!((i>0)&&((j+1)<cs)&&((k+1)<ds))||IS_CORE(B[(k+1)*ps+(j+1)*rs+i-1])) G[(z+1)*gps+(y+1)*grs+x-1] = VAL_OBJET;
    if (!(((i+1)<rs)&&(j>0)&&((k+1)<ds))||IS_CORE(B[(k+1)*ps+(j-1)*rs+i+1])) G[(z+1)*gps+(y-1)*grs+x+1] = VAL_OBJET;
    if (!(((i+1)<rs)&&((j+1)<cs)&&((k+1)<ds))||IS_CORE(B[(k+1)*ps+(j+1)*rs+i+1])) G[(z+1)*gps+(y+1)*grs+x+1] = VAL_OBJET;
  }

  mcskel3d_K3_UnMarkAlphaCarre(b, I, FLAG_CORE); 

} /* mcskel3d_K3_CopieCore3d() */

/* ==================================== */
int32_t mcskel3d_K3_Tbar3D(struct xvimage *b, int32_t i, int32_t j, int32_t k, struct xvimage *g)
/* ==================================== */
/* 
  Retourne le nombre de composantes connexes du
    complementaire du core du point i,j,k de b.
  g (image temporaire) doit etre allouee de taille 7x7x7.
*/    
{
  mcskel3d_K3_CopieComplCore3d(b, i, j, k, g);
  return NbCompConnexe3d(g);
} /* mcskel3d_K3_Tbar3D() */

/* ==================================== */
int32_t mcskel3d_K3_T3D(struct xvimage *b, int32_t i, int32_t j, int32_t k, struct xvimage *g)
/* ==================================== */
/* 
  Retourne le nombre de composantes connexes du
    core du point i,j,k de b, privé du point b.
  g (image temporaire) doit etre allouee de taille 7x7x7.
*/    
{
  mcskel3d_K3_CopieCore3d(b, i, j, k, g);
  return NbCompConnexe3d(g);
} /* mcskel3d_K3_T3D() */

/* ========================================== */
/*! \fn void mcskel3d_K3_Mark2DIsthmus(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \brief ajoute au complexe m les faces séparantes (Tb > 1) du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel3d_K3_Mark2DIsthmus(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_Mark2DIsthmus"
  int32_t x, y, z;
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), i;
  index_t ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  struct xvimage *g;

  g = allocimage(NULL, 7, 7, 7, VFF_TYP_1_BYTE);
  if (g == NULL)
  {   
    fprintf(stderr,"%s: malloc failed\n", F_NAME);
    exit(0);
  }

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (!K[i]) goto next;
    if (SINGL3D(x,y,z)) goto next; // ne peut être séparant
    if (INTER3D(x,y,z)) goto next; // ne peut être séparant
    if (IS_ESS(K[i])) 
    {
      if (CARRE3D(x,y,z)) // séparant seulement s'il est principal
      {
	if (IS_PRINC(K[i]))
	{
	  K[i] |= FLAG_TMP1;
	  mcskel3d_K3_MarkAlphaCarre(k, i, FLAG_TMP1);
	}
	goto next;
      }
      else // CUBE3D
      {
	if (mcskel3d_K3_Tbar3D(k, x, y, z, g) > 1)
	{
	  K[i] |= FLAG_TMP1;
	  mcskel3d_K3_MarkAlphaCarre(k, i, FLAG_TMP1);
	}
      }
    }
  next:;
  }
  for (i = 0; i < N; i++) if (IS_TMP1(K[i])) M[i] = 1;
  freeimage(g);
} // mcskel3d_K3_Mark2DIsthmus()

/* ========================================== */
/*! \fn void mcskel3d_K3_Mark1DIsthmus(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \brief ajoute au complexe m les isthmes 1D du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel3d_K3_Mark1DIsthmus(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_Mark1DIsthmus"
  int32_t x, y, z;
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), i;
  index_t ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  struct xvimage *g;
  uint32_t mask;

  g = allocimage(NULL, 7, 7, 7, VFF_TYP_1_BYTE);
  if (g == NULL)
  {   
    fprintf(stderr,"%s: malloc failed\n", F_NAME);
    exit(0);
  }

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (!K[i]) goto next;
    if (SINGL3D(x,y,z)) goto next; // ne peut être un isthme 1D
    if (IS_ESS(K[i])) 
    {
      if (INTER3D(x,y,z)) // isthme 1D seulement s'il est principal
      {
	if (IS_PRINC(K[i]))
	{
	  K[i] |= FLAG_TMP1;
	  mcskel3d_K3_MarkAlphaCarre(k, i, FLAG_TMP1);
	}
	goto next;
      }
      else
      if (CARRE3D(x,y,z)) 
      {
	mask = get2Dconfiguration(K, FLAG_OBJ, x, y, z, rs, ps);
	if (t8(mask) > 1) K[i] |= FLAG_TMP1;
	goto next;
      }
      else // CUBE3D
      {
	if (mcskel3d_K3_T3D(k, x, y, z, g) > 1)
	{
	  K[i] |= FLAG_TMP1;
	  mcskel3d_K3_MarkAlphaCarre(k, i, FLAG_TMP1);
	}
      }
    }
  next:;
  }
  for (i = 0; i < N; i++) if (IS_TMP1(K[i])) M[i] = 1;
  freeimage(g);
} // mcskel3d_K3_Mark1DIsthmus()

/* ========================================== */
/*! \fn void mcskel3d_K3_MarkCurveElts(struct xvimage *k, struct xvimage *m)
    \param k : un complexe
    \param m : un complexe "marqueur"
    (m est un ensemble de faces qui doivent être préservées)
    \brief ajoute au complexe m les éléments curvilignes du complexe k
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
void mcskel3d_K3_MarkCurveElts(struct xvimage *k, struct xvimage *m)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "mcskel3d_K3_MarkCurveElts"
  int32_t x, y, z;
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), i;
  index_t ps = rs*cs, N = ps*ds;
  unsigned char *K = UCHARDATA(k);
  unsigned char *M = UCHARDATA(m);
  uint32_t mask;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = z*ps + y*rs + x;
    if (!K[i]) goto next;
    if (SINGL3D(x,y,z)) goto next; // ne peut être un isthme 1D
    if (IS_ESS(K[i])) 
    {
      if (INTER3D(x,y,z)) // element de courbe seulement s'il est principal
      {
	if (IS_PRINC(K[i]))
	{
	  K[i] |= FLAG_TMP1;
	  mcskel3d_K3_MarkAlphaCarre(k, i, FLAG_TMP1);
	}
	goto next;
      }
      else
      if (CARRE3D(x,y,z)) 
      {
	if (get2Dcount(K, FLAG_PRINC, x, y, z, rs, ps) == 2)
	  K[i] |= FLAG_TMP1;
	goto next;
      }
      else // CUBE3D
      {
	if (get3Dcount(K, FLAG_PRINC, i, rs, ps, N) == 2)
	{
	  K[i] |= FLAG_TMP1;
	  mcskel3d_K3_MarkAlphaCarre(k, i, FLAG_TMP1);
	}
      }
    }
  next:;
  }
  for (i = 0; i < N; i++) if (IS_TMP1(K[i])) M[i] = 1;
} // mcskel3d_K3_MarkCurveElts()

/* =============================================================== */
int32_t lskeleuc3d(struct xvimage * k, struct xvimage * inhi, int32_t nsteps) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lskeleuc3d"
/*
    \brief Noyau homotopique dans H3 avec fonction de priorité (distance euclidienne)
           Version avec correction géométrique 
    \author Michel COUPRIE
    \warning Le complexe doit être fermé, il ne doit pas toucher le bord
 */
{
  struct xvimage * m;
  struct xvimage * m2;
  struct xvimage * dist;
  struct xvimage * prio;
#ifdef DEBUG
  struct xvimage * k2;
#endif
  index_t n_new, i, j, j2, q, rs, cs, ds, ps, n, N;
  int32_t l, x, y, z;
  unsigned char *K, *M, *I = NULL;
  double *D;
  double *P, p, t;
  Rbt * RBT;
  Liste * LIST;

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(inhi, VFF_TYP_1_BYTE);
  COMPARE_SIZE(k, inhi);

  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  ps = rs * cs;
  N = ps * ds;
  K = UCHARDATA(k);

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  I = UCHARDATA(inhi);

  m = copyimage(k); // tous les points sont marqués (protégés) au départ
  if (m == NULL)
  {   
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    exit(1);
  }
  M = UCHARDATA(m);

  m2 = copyimage(k);
  if (m2 == NULL)
  {   
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    exit(1);
  }

  dist = allocimage(NULL, rs, cs, ds, VFF_TYP_DOUBLE);
  prio = allocimage(NULL, rs, cs, ds, VFF_TYP_DOUBLE);
  if ((dist == NULL) || (prio == NULL))
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    exit(1);
  }
  P = DOUBLEDATA(prio);
  D = DOUBLEDATA(dist);

  if (! ldistMeijster(k, dist))
  {
    fprintf(stderr, "%s: ldistMeijster failed\n", F_NAME);
    exit(1);
  }

#ifdef VERBOSE
    printf("distance map computed\n");
#endif

  LIST = CreeListeVide(N);
  if (LIST == NULL)
  {   fprintf(stderr, "%s : CreeListeVide failed\n", F_NAME);
      exit(0);
  }

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) goto fin;

// --------------------------
// Init RBT
// --------------------------
  RBT = mcrbt_CreeRbtVide(N);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : mcrbt_CreeRbtVide failed\n", F_NAME);
    exit(1);
  }
  for (i = 0; i < N; i++)
  {
    P[i] = 1e15;
    if (K[i] && (!I[i]) && mctopo3d_bordext26(K, i, rs, ps, N))
    {
      p = D[i];
      for (l = 0; l < 26; l += 1)
      {
	j = voisin26(i, l, rs, ps, N);
	if ((j != -1) && K[j] && I[j] && (D[i] > D[j]))
	{
	  t = D[j] + (D[i] - D[j]) / 
	    dist3((double)(j%rs), (double)((j % ps) / rs), (double)(j/ps), 
		  (double)(i%rs), (double)((i % ps) / rs), (double)(i/ps));
	  if (t < p) p = t;
	}
      } // for l
      x = i % rs;
      y = (i % ps) / rs;
      z = i / ps;
      if (CUBE3D(x,y,z))
      {
	for (l = 0; l < 26; l += 1)
        {
	  j = voisin26(i, l, rs, ps, N);
	  if (j != -1)
	  {
	    for (q = 0; q < 26; q += 1)
	    {
	      j2 = voisin26(j, q, rs, ps, N);
	      if ((j2 != -1) && K[j2] && I[j2] && (D[i] > D[j2]))
	      {
		t = D[j2] + (D[i] - D[j2]) / 
		  dist3((double)(j2%rs), (double)((j2 % ps) / rs), (double)(j2/ps), 
			(double)(i%rs), (double)((i % ps) / rs), (double)(i/ps));
		if (t < p) p = t;
	      }
	    } // for q
	  } // if (j != -1)
	} // for l
      } // if (CUBE3D(x,y,z))
      if (p < P[i])
      {
	P[i] = p;
	mcrbt_RbtInsert(&RBT, p, i);
      }
    }
  } // for (i = 0; i < N; i++)

#ifdef VERBOSE
    printf("RBT initialized\n");
#endif

// --------------------------
// MAIN LOOP
// --------------------------
  n = 0;
  while (!mcrbt_RbtVide(RBT) && (n < nsteps))
  {
    n++;
    p = RbtMinLevel(RBT); 
#ifdef VERBOSE
    printf("step %d, prio %g ", n, p);
#endif
    mcskel3d_K3_MarkObj(k);
    while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))
    {
      i = RbtPopMin(RBT);
      if (IS_OBJ(K[i]) && M[i]) 
      { // pas déjà retiré (K[i]) et pas encore traité (M[i])
	if (I[i] == 0)
	{  
	  M[i] = 0; // démarque pour traiter
	  ListePush(LIST, i); // mémorise pour la détection des nouveaux
	                     // points du squelette apparus dans l'itération
	}
      }
    }
#ifdef DEBUG
    writeimage(k, "_k");
#endif
    copy2image(m2, m);
    mcskel3d_K3_CloseComplex(m2);
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
#ifdef DEBUG
    k2 = copyimage(k);
    mcskel3d_K3_SelMarked(k2, FLAG_ESS);
    writeimage(k2, "_k_ess");
#endif
    mcskel3d_K3_MarkCritic2(k, m2);
#ifdef DEBUG
    k2 = copyimage(k);
    mcskel3d_K3_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

#ifdef PARANO
    if (!mcskel3d_K3_CheckComplex(k))
    {
      fprintf(stderr, "%s: not a complex\n", F_NAME);
      exit(0);
    }
#endif

    while (!ListeVide(LIST))
    {
      i = ListePop(LIST);
      if (IS_CRITIC(K[i]))
      { // EMPILEMENT AVEC MISE A JOUR DES PRIORITES DES VOISINS
	I[i] = 1;
	for (l = 0; l < 26; l += 1)
	{
	  j = voisin26(i, l, rs, ps, N);
	  if ((j != -1) && IS_OBJ(K[j]) && (!I[j]) && (D[j] > D[i]))
	  {
	    p = D[i] + (D[j] - D[i]) / 
	      dist3((double)(j%rs), (double)((j % ps) / rs), (double)(j/ps), 
		    (double)(i%rs), (double)((i % ps) / rs), (double)(i/ps));
	    if (p < P[j])
	    {
	      mcrbt_RbtInsert(&RBT, p, j);
	      P[j] = p;
	    }	    
	  }
	} // for l
	x = i % rs;
	y = (i % ps) / rs;
	z = i / ps;
	if (CUBE3D(x,y,z))
	{
	  for (l = 0; l < 26; l += 1)
	  {
	    j = voisin26(i, l, rs, ps, N);
	    if (j != -1)
	    {
	      for (q = 0; q < 26; q += 1)
	      {
		j2 = voisin26(j, q, rs, ps, N);
		if ((j2 != -1) && IS_OBJ(K[j2]) && (!I[j2]) && (D[j2] > D[i]))
		{
		  p = D[i] + (D[j2] - D[i]) / 
		    dist3((double)(j2%rs), (double)((j2 % ps) / rs), (double)(j2/ps), 
			  (double)(i%rs), (double)((i % ps) / rs), (double)(i/ps));
		  if (p < P[j])
		  {
		    mcrbt_RbtInsert(&RBT, p, j);
		    P[j] = p;
		  }
		}
	      } // for q
	    } // if (j != -1)
	  } // for l
	} // if (CUBE3D(x,y,z))
      } // if (IS_CRITIC(K[i]))
      else
      { // EMPILEMENT SIMPLE DES VOISINS
	for (l = 0; l < 26; l += 1)
	{
	  j = voisin26(i, l, rs, ps, N);
	  if ((j != -1) && IS_OBJ(K[j]) && (!I[j]) && (D[j] > D[i]))
	  {
	    p = D[j];
	    if (p < P[j])
	    {
	      mcrbt_RbtInsert(&RBT, p, j);
	      P[j] = p;
	    }
	  }
	} // for l
      }
    } // while (!ListeVide(LIST))

    n_new = mcskel3d_K3_CriticE2Obj(k);

#ifdef VERBOSE
    printf("  fin step : nb. points = %d\n", n_new);
#endif
  } // while (!mcrbt_RbtVide(RBT) && (n < nsteps))

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);

 fin:
  ListeTermine(LIST);
  mcrbt_RbtTermine(RBT);
  freeimage(dist);
  freeimage(prio);
  freeimage(m);
  freeimage(m2);
  return(1);
} // lskeleuc3d()

#ifdef EUCLIDIEN

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VERSION PAR PROPAGATION (AVEC LISTE)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// Appliqués à l'image I (inhi) :
// Le flag MARK1 sert à représenter le marqueur initial
// Le flag MARK2 sert à représenter le marqueur courant
// Le flag MARK3 sert à représenter la fermeture du marqueur courant

/* ========================================== */
/*! \fn void K3List_MarkPrinc(struct xvimage *k, Liste *l)
    \param k : un complexe
    \param l : un ensemble de faces
    \brief pour chaque face f de alphacarre(l) inter k, 
           actualise la marque FLAG_PRINC de f et le cas échéant, 
	   mémorise la face f en fin de la liste l.
           Mémorise aussi les faces de l en fin de la liste l.
*/
void K3List_MarkPrinc(struct xvimage *k, Liste *l)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K3List_MarkPrinc"
  int32_t card, x, y, z, xx, yy, zz;
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, i, p, q; 
  unsigned char *K = UCHARDATA(k);
  int32_t tabu[26], tabv[26], u, n, v, m;

  for (i = 0; i < l->Sp; i++)
  {
    p = l->Pts[i];
    ListePush2(l, p);
    x = p % rs; y = (p % ps) / rs; z = p / ps;
    Alphacarre3d(rs, cs, ds, x, y, z, tabu, &n);
    for (u = 0; u < n; u++)
    {
      q = tabu[u];
      if (IS_OBJ(K[q]) && !IS_PRINC(K[q])) 
      {
	xx = q % rs; yy = (q % ps) / rs; zz = q / ps;
	Betacarre3d(rs, cs, ds, xx, yy, zz, tabv, &m);
	card = 0;
	for (v = 0; v < m; v++) if (IS_OBJ(K[tabv[v]])) card++;
	if (card == 0) 
	{ 
	  K[q] |= FLAG_PRINC;
	  ListePush2(l, q);
	}
      }
    } // for (u = 0; u < n; u++)
  } // for (i = 0; i < l->Sp; i++)
} // K3List_MarkPrinc()

/* ========================================== */
/*! \fn void K3List_MarkEss(struct xvimage *k, Liste *l)
    \param k : un complexe
    \param l : un sous-ensemble de k
    \brief pour chaque face essentielle f de l, actualise la marque FLAG_ESS
    \warning les faces principales doivent avoir été marquées auparavant
*/
void K3List_MarkEss(struct xvimage *k, Liste *l)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K3List_MarkEss"
  int32_t x, y, z; 
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds, i, p;
  unsigned char *K = UCHARDATA(k);
  int32_t tab[26], u, n;
  char tablefilename[256];

  if (EssTab3 == NULL) 
  {
    FILE *fd;
    int32_t ret, tablesize = 1<<23;

    // ON EN PROFITE POUR INITIALISER LA TABLE DES POINTS SIMPLES
    mctopo3d_table_init_topo3d();

    EssTab3 = malloc(tablesize);
    if (EssTab3 == NULL)
    {
      fprintf(stderr, "%s: malloc failed\n", F_NAME);
      exit(1);
    }
//    sprintf(tablefilename, "%s/src/tables/%s", getenv("PINK"), ESS3DTABNAME);
    sprintf(tablefilename, "/tmp/%s", ESS3DTABNAME);
    fd = fopen (tablefilename, "r");
    if (fd == NULL) 
    {   
      int32_t ret;
      char command[256];
      sprintf(command, "cp %s/src/tables/%s %s", getenv("PINK"), ESS3DTABNAME, tablefilename);
      ret = system(command);
      fd = fopen (tablefilename, "r");
      if (fd == NULL) 
      {
	fprintf(stderr, "%s: error while opening table\n", F_NAME);
	exit(1);
      }
    }
    ret = fread(EssTab3, sizeof(char), tablesize, fd);
    if (ret != tablesize)
    {
      fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, tablesize, ret);
      exit(1);
    }
#ifdef VERBOSE
  printf("%s: %s loaded\n", F_NAME, ESS3DTABNAME);
#endif
    fclose(fd);
  }

  for (i = 0; i < l->Sp; i++)
  {
    p = l->Pts[i];
    K[p] &= ~FLAG_ESS; 
    if (IS_OBJ(K[p])) 
    {
      x = p % rs; y = (p % ps) / rs; z = p / ps;
      if (CUBE3D(x,y,z)) 
      { 
	if (IS_PRINC(K[p])) K[p] |= FLAG_ESS; 
      }
      else if (CARRE3D(x,y,z)) 
      {
	int32_t card = 0;
	Betacarre3d(rs, cs, ds, x, y, z, tab, &n);
	for (u = 0; u < n; u++) if (IS_PRINC(K[tab[u]])) card++;
	if (IS_PRINC(K[p]) && ((card == 0) || (card == 2))) K[p] |= FLAG_ESS; 
	if (!IS_PRINC(K[p]) && (card == 2)) K[p] |= FLAG_ESS; 
      }
      else if (INTER3D(x,y,z)) 
      {
	uint32_t mask = PrincBetacarre3d(K, rs, cs, ds, x, y, z);
	if ((INTER3DX(x,y,z) && EssTab2X[mask]) || 
	    (INTER3DY(x,y,z) && EssTab2Y[mask]) || 
	    (INTER3DZ(x,y,z) && EssTab2Z[mask]))
	    K[p] |= FLAG_ESS;
      }
      else // SINGL3D
      {
	uint32_t mask = PrincBetacarre3d(K, rs, cs, ds, x, y, z);
	if (IsEss3(mask)) K[p] |= FLAG_ESS;
      }
    }
  }
} // K3List_MarkEss()

/* ========================================== */
/*! \fn int32_t K3List_MarkCore2(struct xvimage *k, unsigned char *I, int32_t f)
    \param k : un complexe
    \param I : un "marqueur" m représenté par le flag MARK3
    \param f : une face de k
    \return le cardinal (nombre de faces) de la fermeture du noyau de f pour k, conditionné par m
    \brief marque le complexe formé du noyau (core) de la face f pour le complexe k, conditionné par m
    \warning les faces essentielles doivent avoir été marquées auparavant,
    pas de vérification de compatibilité entre k et m
*/
int32_t K3List_MarkCore2(struct xvimage *k, unsigned char *I, int32_t f)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K3List_MarkCore2"
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, nf = 0, i, j;
  unsigned char *K = UCHARDATA(k);
  int32_t tab[26], tabi[26], u, v, n, ni;
  int32_t x = f % rs, y = (f % ps) / rs, z = f / ps, xi, yi, zi;

  Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
  for (u = 0; u < n; u++) 
  {
    i = tab[u];
    xi = i % rs; yi = (i%ps) / rs; zi = i / ps;
    if (IS_MARKED3(I[i]))
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE2
printf("mark_core2 (marqueur) : %d,%d,%d\n", i%rs, (i%ps)/rs, i/ps);
#endif
	Alphacarre3d(rs, cs, ds, xi, yi, zi, tabi, &ni);
	for (v = 0; v < ni; v++) 
        {
	  j = tabi[v];
	  if (!(IS_CORE(K[j]))) 
	  { 
	    K[j] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE2
printf("mark_core2_aux : %d,%d,%d\n", j%rs, (j%ps)/rs, j/ps);
#endif
	  } 
	}
      } 
    }
    else if (IS_ESS(K[i]))
    {
      if (!(IS_CORE(K[i]))) 
      { 
	K[i] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE2
printf("mark_core2 (ess) : %d,%d,%d\n", i%rs, (i%ps)/rs, i/ps);
#endif        
	Alphacarre3d(rs, cs, ds, xi, yi, zi, tabi, &ni);
	for (v = 0; v < ni; v++) 
        {
	  j = tabi[v];
	  if (!(IS_CORE(K[j]))) 
	  { 
	    K[j] |= FLAG_CORE; nf++; 
#ifdef DEBUGMARKCORE2
printf("mark_core2_aux : %d,%d,%d\n", j%rs, (j%ps)/rs, j/ps);
#endif
	  } 
	}
      }
    }
  }
  return nf;
} // K3List_MarkCore2()

/* ========================================== */
/*! \fn void K3List_MarkCritic2(struct xvimage *k, unsigned char *I, Liste *l)
    \param k : un complexe
    \param I : un "marqueur" m représenté par le flag MARK3
    \param l : un sous-ensemble de k
    \brief marque FLAG_CRITIC_E les fermetures des faces critiques de l, 
           conditionné par m, et marque FLAG_CRITIC les faces critiques 
	   de l (cond. m)
	   (m est un ensemble de faces qui doivent être préservées)
    \warning les faces essentielles (conditionnellement à m) 
           doivent avoir été marquées auparavant,
    \warning pas de vérification de compatibilité entre k et m
*/
void K3List_MarkCritic2(struct xvimage *k, unsigned char *I, Liste *l)
/* ========================================== */
{
#undef F_NAME
#define F_NAME "K3List_MarkCritic2"
  unsigned char *K = UCHARDATA(k);
  index_t rs = rowsize(k), cs = colsize(k), ds = depth(k), ps = rs*cs, N = ps*ds, i, p, q;
  int32_t x, y, z, n, ncore;
  u_int32_t mask;
  int32_t tab[26], u;
  
  for (i = 0; i < l->Sp; i++) 
  {
    p = l->Pts[i];
    K[p] &= ~FLAG_CRITIC; 
    K[p] &= ~FLAG_CRITIC_E; 
    x = p % rs; y = (p % ps) / rs; z = p / ps;
    Alphacarre3d(rs, cs, ds, x, y, z, tab, &n);
    for (u = 0; u < n; u++) 
    {
      q = tab[u];
      if (!IS_MARKED3(I[q])) K[q] &= ~FLAG_CRITIC_E;
    }
  }

  for (i = 0; i < l->Sp; i++)
  {
    p = l->Pts[i];
    if (IS_ESS(K[p])) 
    {
      x = p % rs; y = (p % ps) / rs; z = p / ps;
      if (CARRE3D(x,y,z)) n = 8; else 
	if (INTER3D(x,y,z)) n = 2; else 
	  if (SINGL3D(x,y,z)) n = 0; else n = 26; 
      ncore = K3List_MarkCore2(k, I, p);
#ifdef DEBUGMARKCRITIC2
printf("%s ess : %d,%d,%d ; n = %d, ncore = %d\n", F_NAME, x, y, z, n, ncore);
#endif
      if (ncore == 0) 
      { 
	K[p] |= FLAG_CRITIC;
	goto next;
      }
      if (ncore == n) 
      { 
	K[p] |= FLAG_CRITIC;
	mcskel3d_K3_UnMarkAlphaCarre(k, p, FLAG_CORE); 
	goto next; 
      }
      // le core d'un singl est forcément vide (cas déjà traité)
      if (INTER3D(x,y,z))
      {
	if (ncore != 1) K[p] |= FLAG_CRITIC;
	mcskel3d_K3_UnMarkAlphaCarre(k, p, FLAG_CORE);
	goto next;
      }
      if (CARRE3D(x,y,z))
      {
	mask = get2Dconfiguration(K, FLAG_CORE, x, y, z, rs, ps);
	if ((t8(mask) != 1) || (t4b(mask) != 1)) 
	  K[p] |= FLAG_CRITIC;
	mcskel3d_K3_UnMarkAlphaCarre(k, p, FLAG_CORE);
	goto next;
      }
      //if (CUBE3D(x,y,z)) // inutile car c'est le seul choix qui reste
      mask = get3Dconfiguration(K, FLAG_CORE, p, rs, ps, N);
      if (!mctopo3d_table_simple26mask(mask)) 
        K[p] |= FLAG_CRITIC;
      mcskel3d_K3_UnMarkAlphaCarre(k, p, FLAG_CORE);

    next:;
    } // if (IS_ESS(K[p])) 
  } // for (i = 0; i < l->Sp; i++)

  for (i = 0; i < l->Sp; i++)
  {
    p = l->Pts[i];
    //    if (IS_ESS(K[p]) && (IS_MARKED3(I[p]) || IS_CRITIC(K[p])))
    if (IS_MARKED3(I[p]) || IS_CRITIC(K[p]))
    {
      K[p] |= FLAG_CRITIC_E;
      K[p] |= FLAG_CRITIC;
      mcskel3d_K3_MarkAlphaCarre(k, p, FLAG_CRITIC_E);
    } // if (IS_MARKED3(I[p]) || (IS_ESS(K[p]) && IS_CRITIC(K[p])))
  } // for (i = 0; i < l->Sp; i++)
} // K3List_MarkCritic2()

/* =============================================================== */
int32_t llistskeleuc3d(struct xvimage * k, struct xvimage * inhi, int32_t nsteps) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "llistskeleuc3d"
/*
    \brief Noyau homotopique dans H3 avec fonction de priorité (distance euclidienne)
           Version avec correction géométrique 
	   Version avec propagation par listes
    \author Michel COUPRIE
    \warning Le complexe doit être fermé, il ne doit pas toucher le bord
 */
{
  struct xvimage * dist;
  struct xvimage * prio;
  index_t i, ii, j, j2, q, rs, cs, ds, ps, n, N;
  int32_t l, x, y, z, xx, yy, zz;
  unsigned char *K, *I = NULL;
  double *D, d;
  double *P, p, t;
  int32_t tabu[26], tabv[26], u, nu, v, nv;
  Rbt * RBT;
  Liste * LIST;
#ifdef VERBOSE
  int32_t nd;
#endif
#ifdef DEBUG
  struct xvimage * k2;
  k2 = copyimage(k);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(inhi, VFF_TYP_1_BYTE);
  COMPARE_SIZE(k, inhi);

  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  ps = rs * cs;
  N = ps * ds;
  K = UCHARDATA(k);

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  I = UCHARDATA(inhi);

  mcskel3d_K3_MarkObj(k);
  for (i = 0; i < N; i++) if (K[i])
  {
    if (I[i]) I[i] = MARK1;
    I[i] |= (MARK2 | MARK3);
  }
  // tous les points sont marqués (protégés) au départ

  dist = allocimage(NULL, rs, cs, ds, VFF_TYP_DOUBLE);
  prio = allocimage(NULL, rs, cs, ds, VFF_TYP_DOUBLE);
  if ((dist == NULL) || (prio == NULL))
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    exit(1);
  }
  P = DOUBLEDATA(prio);
  D = DOUBLEDATA(dist);

  if (! ldistMeijster(k, dist))
  {
    fprintf(stderr, "%s: ldistMeijster failed\n", F_NAME);
    exit(1);
  }

#ifdef VERBOSE
    printf("distance map computed\n");
#endif

  LIST = CreeListeVide(N);
  if (LIST == NULL)
  {   fprintf(stderr, "%s : CreeListeVide failed\n", F_NAME);
      exit(0);
  }

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) goto fin;

// --------------------------
// Init RBT
// --------------------------
  RBT = mcrbt_CreeRbtVide(N);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : mcrbt_CreeRbtVide failed\n", F_NAME);
    exit(1);
  }
  for (i = 0; i < N; i++)
  {
    P[i] = 1e15;
    if (K[i] && (!IS_MARKED1(I[i])) && mctopo3d_bordext26(K, i, rs, ps, N))
    {
      p = D[i];
      for (l = 0; l < 26; l += 1)
      {
	j = voisin26(i, l, rs, ps, N);
	if ((j != -1) && K[j] && IS_MARKED1(I[j]) && (D[i] > D[j]))
	{
	  t = D[j] + (D[i] - D[j]) / 
	    dist3((double)(j%rs), (double)((j % ps) / rs), (double)(j/ps), 
		  (double)(i%rs), (double)((i % ps) / rs), (double)(i/ps));
	  if (t < p) p = t;
	}
      } // for l
      x = i % rs; y = (i % ps) / rs; z = i / ps;
      if (CUBE3D(x,y,z))
      {
	for (l = 0; l < 26; l += 1)
        {
	  j = voisin26(i, l, rs, ps, N);
	  if (j != -1)
	  {
	    for (q = 0; q < 26; q += 1)
	    {
	      j2 = voisin26(j, q, rs, ps, N);
	      if ((j2 != -1) && K[j2] && IS_MARKED1(I[j2]) && (D[i] > D[j2]))
	      {
		t = D[j2] + (D[i] - D[j2]) / 
		  dist3((double)(j2%rs), (double)((j2 % ps) / rs), (double)(j2/ps), 
			(double)(i%rs), (double)((i % ps) / rs), (double)(i/ps));
		if (t < p) p = t;
	      }
	    } // for q
	  } // if (j != -1)
	} // for l
      } // if (CUBE3D(x,y,z))
      if (p < P[i])
      {
	P[i] = p;
	mcrbt_RbtInsert(&RBT, p, i);
#ifdef DEBUG1
	      printf("RbtInsert 0 : prio = %g, pt = %d\n", p, i);
#endif
      }
    }
  } // for (i = 0; i < N; i++)

#ifdef VERBOSE
  printf("RBT initialized\n");
#endif

  mcskel3d_K3_MarkPrinc(k);
  mcskel3d_K3_MarkEss(k);
  for (i = 0; i < N; i++)
    if (IS_OBJ(K[i]))
    {
      K[i] |= FLAG_CRITIC_E;
      if (IS_ESS(K[i]) || IS_MARKED1(I[i])) K[i] |= FLAG_CRITIC;
    }
// --------------------------
// MAIN LOOP
// --------------------------
  n = 0;
  while (!mcrbt_RbtVide(RBT) && (n < nsteps))
  {
    n++;
    p = RbtMinLevel(RBT); 
#ifdef VERBOSE
    printf("step %d, prio %g ", n, p);
#endif

    // CONSTRUIT LA LISTE DES POINTS A TRAITER ET MAJ. MARQUEUR COURANT (MARK2)
    while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))
    {
      i = RbtPopMin(RBT);
      if (IS_OBJ(K[i]) && IS_MARKED2(I[i])) 
      { // pas déjà retiré et pas encore traité
	if (!IS_MARKED1(I[i]))
	{  
	  I[i] &= ~MARK2;     // démarque pour traiter
	  ListePush(LIST, i); // mémorise pour la détection des nouveaux
	                      // points du squelette apparus dans l'itération
	}
      } // if (IS_OBJ(K[i]) && IS_MARKED2(I[i])) 
    } // while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))
#ifdef DEBUG
    writeimage(k, "_k");
#endif
    // MET A JOUR LA FERMETURE DU MARQUEUR COURANT (MARK3)
    for (ii = 0; ii < LIST->Sp; ii++)
    {
      i = LIST->Pts[ii];
      x = i % rs; y = (i % ps) / rs; z = i / ps;
      Betacarre3d(rs, cs, ds, x, y, z, tabv, &nv);
      for (v = 0; v < nv; v++)
	if (IS_MARKED2(I[tabv[v]])) break;
      if (v == nv) // pas de point MARK2 dans le beta
	I[i] &= ~MARK3;
      Alphacarre3d(rs, cs, ds, x, y, z, tabu, &nu);
      for (u = 0; u < nu; u++)
      {
	j = tabu[u];
	if (!IS_MARKED2(I[j]))
	{  
	  xx = j % rs; yy = (j % ps) / rs; zz = j / ps;
	  Betacarre3d(rs, cs, ds, xx, yy, zz, tabv, &nv);
	  for (v = 0; v < nv; v++)
	    if (IS_MARKED2(I[tabv[v]])) break;
	  if (v == nv) // pas de point MARK2 dans le beta
	    I[j] &= ~MARK3;
	}
      }
    } // for (ii = 0; ii < LIST->Sp; ii++)

#ifdef DEBUG
    copy2image(k2, inhi);
    mcskel3d_K3_SelMarked(k2, MARK2);
    writeimage(k2, "_i_mark2");
    copy2image(k2, inhi);
    mcskel3d_K3_SelMarked(k2, MARK3);
    writeimage(k2, "_i_mark3");
#endif

    K3List_MarkCritic2(k, I, LIST);

#ifdef DEBUG
    copy2image(k2, k);
    mcskel3d_K3_SelMarked(k2, FLAG_PRINC);
    writeimage(k2, "_k_princ_1");
    copy2image(k2, k);
    mcskel3d_K3_SelMarked(k2, FLAG_ESS);
    writeimage(k2, "_k_ess_1");
    copy2image(k2, k);
    mcskel3d_K3_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
    copy2image(k2, k);
    mcskel3d_K3_SelMarked(k2, FLAG_CRITIC_E);
    writeimage(k2, "_k_critic_E");
#endif
#ifdef PARANO
    if (!mcskel3d_K3_CheckComplex(k))
    {
      fprintf(stderr, "%s: not a complex\n", F_NAME);
      exit(0);
    }
#endif

    // MISE A JOUR DE LA STRUCTURE DE PRIORITES
    for (ii = 0; ii < LIST->Sp; ii++)
    {
      i = LIST->Pts[ii];
      if (!IS_CRITIC(K[i]) || IS_TMP1(K[i]))
      { // EMPILEMENT SIMPLE DES VOISINS
	for (l = 0; l < 26; l += 1)
	{
	  j = voisin26(i, l, rs, ps, N);
	  if ((j != -1) && IS_OBJ(K[j]) && (!IS_MARKED1(I[j])) && (D[j] > D[i]))
	  {
	    p = D[j];
	    if (p < P[j])
	    {
	      mcrbt_RbtInsert(&RBT, p, j);
#ifdef DEBUG1
	      printf("RbtInsert 3 : prio = %g, pt = %d\n", p, j);
#endif
	      P[j] = p;
	    }
	  }
	} // for l
      } // if (!IS_CRITIC(K[i]))
      else
      { // EMPILEMENT AVEC MISE A JOUR DES PRIORITES DES VOISINS
	I[i] |= MARK1;
	for (l = 0; l < 26; l += 1)
	{
	  j = voisin26(i, l, rs, ps, N);
	  if ((j != -1) && IS_OBJ(K[j]) && (!IS_MARKED1(I[j])) && (D[j] > D[i]))
	  {
	    p = D[i] + (D[j] - D[i]) / 
	      dist3((double)(j%rs), (double)((j % ps) / rs), (double)(j/ps), 
		    (double)(i%rs), (double)((i % ps) / rs), (double)(i/ps));
	    if (p < P[j])
	    {
	      mcrbt_RbtInsert(&RBT, p, j);
#ifdef DEBUG1
	      printf("RbtInsert 1 : prio = %g, pt = %d\n", p, j);
#endif
	      P[j] = p;
	    }	    
	  }
	} // for l
	x = i % rs; y = (i % ps) / rs; z = i / ps;
	if (CUBE3D(x,y,z))
	{
	  for (l = 0; l < 26; l += 1)
	  {
	    j = voisin26(i, l, rs, ps, N);
	    if (j != -1)
	    {
	      for (q = 0; q < 26; q += 1)
	      {
		j2 = voisin26(j, q, rs, ps, N);
		if ((j2 != -1) && IS_OBJ(K[j2]) && (!IS_MARKED1(I[j2])) && (D[j2] > D[i]))
		{
		  p = D[i] + (D[j2] - D[i]) / 
		    dist3((double)(j2%rs), (double)((j2 % ps) / rs), (double)(j2/ps), 
			  (double)(i%rs), (double)((i % ps) / rs), (double)(i/ps));
		  if (p < P[j])
		  {
		    mcrbt_RbtInsert(&RBT, p, j);
#ifdef DEBUG1
	      printf("RbtInsert 2 : prio = %g, pt = %d\n", p, j);
#endif
		    P[j] = p;
		  }
		}
	      } // for q
	    } // if (j != -1)
	  } // for l
	} // if (CUBE3D(x,y,z))
      } // if (IS_CRITIC(K[i]))
    } // for (ii = 0; ii < LIST->Sp; ii++)

    // ENLEVEMENT DES POINTS NE FAISANT PAS PARTIE DE CELLULES CRITIQUES
#ifdef VERBOSE
    nd = 0;
#endif
    for (ii = 0; ii < LIST->Sp; ii++)
    {
      i = LIST->Pts[ii];
      if (!IS_CRITIC(K[i]) && !IS_MARKED3(I[i]))
      { 
	K[i] = 0;
	ListePush2(LIST, i); // mémorise pour la maj des faces principales,...
#ifdef VERBOSE
	nd++;
#endif
	x = i % rs; y = (i % ps) / rs; z = i / ps;
	Alphacarre3d(rs, cs, ds, x, y, z, tabu, &nu);
	for (u = 0; u < nu; u++)
	{
	  j = tabu[u];
	  if (K[j] && !IS_CRITIC_E(K[j]) && !IS_MARKED3(I[j]))
	  {
	    K[j] = 0;
	    ListePush2(LIST, j); // mémorise pour la maj des faces principales,...
#ifdef VERBOSE
	    nd++;
#endif
	  }
	} // for (u = 0; u < nu; u++)
      } // if (!IS_CRITIC(K[i]))
    } // for (ii = 0; ii < LIST->Sp; ii++)
#ifdef VERBOSE
    printf("nb. pts retirés = %d ", nd);	
#endif    
    ListeFlush1(LIST);
    Liste2to1(LIST); // LIST(1) contient les points enlevés

    K3List_MarkPrinc(k, LIST); // LIST(2) contient les points 
    ListeFlush1(LIST);         // dont le statut PRINC a été modifié

    for (ii = LIST->Sp2+1; ii < LIST->Max; ii++) 
    { // transfère dans LIST(1) les points dont le statut ESS 
      // doit être ré-évalué (alphacarré des points de LIST(2))
      i = LIST->Pts[ii];
      x = i % rs; y = (i % ps) / rs; z = i / ps;
      Alphacarre3d(rs, cs, ds, x, y, z, tabu, &nu);
      for (u = 0; u < nu; u++)
      {
	j = tabu[u];
	if (!IS_TMP2(K[j]))
	{
	  K[j] |= FLAG_TMP2;
	  ListePush(LIST, j); 
	}
      }
    } // for (ii = LIST->Sp2+1; ii < LIST->Max; ii++) 
    while (!ListeVide2(LIST)) // vide LIST(2) et remarque TMP2
    {
      i = ListePop2(LIST);
      x = i % rs; y = (i % ps) / rs; z = i / ps;
      Alphacarre3d(rs, cs, ds, x, y, z, tabu, &nu);
      for (u = 0; u < nu; u++) K[tabu[u]] &= ~FLAG_TMP2;      
    } // while (!ListeVide2(LIST))

    K3List_MarkEss(k, LIST);
    ListeFlush(LIST);

#ifdef DEBUG
    copy2image(k2, k);
    mcskel3d_K3_SelMarked(k2, FLAG_PRINC);
    writeimage(k2, "_k_princ_2");
    copy2image(k2, k);
    mcskel3d_K3_SelMarked(k2, FLAG_ESS);
    writeimage(k2, "_k_ess_2");
#endif

#ifdef VERBOSE
    printf("nb. pts = %d\n", mcskel3d_K3_CardObj(k));
#endif
  } // while (!mcrbt_RbtVide(RBT) && (n < nsteps))

  mcskel3d_K3_SelMarked(k, FLAG_CRITIC);
  mcskel3d_K3_CloseComplex(k);
  mcskel3d_K3_Binarize(k);

 fin:
  ListeTermine(LIST);
  mcrbt_RbtTermine(RBT);
  freeimage(dist);
  freeimage(prio);
  return(1);
} // llistskeleuc3d()
#endif


/* =============================================================== */
int32_t lskel3d1(struct xvimage * k, int32_t nsteps) 
/* =============================================================== */
/*
    \brief Noyau homotopique - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d1"
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_MarkCritic(k);
    n_new = mcskel3d_K3_CriticE2Obj(k);
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);
  return(1);
} // lskel3d1()

/* =============================================================== */
int32_t lskel3d1b(struct xvimage * k, struct xvimage * m, int32_t nsteps) 
/* =============================================================== */
/*
    \brief Noyau homotopique avec contrainte - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d1b"
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_MarkCritic2(k, m);
    n_new = mcskel3d_K3_CriticOrMarked2Obj(k, m); 
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);
  return(1);
} // lskel3d1b()

/* =============================================================== */
int32_t lskel3d2(struct xvimage * k, int32_t nsteps) 
/* =============================================================== */
/*
    \brief Squelette surfacique (dans H3) par sélection et mémorisation
	  des isthmes 2D - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d2"
  struct xvimage * m; // pour marquer les éléments "minces"
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_Mark2DIsthmus(k, m);
    mcskel3d_K3_MarkCritic2(k, m);
    n_new = mcskel3d_K3_CriticOrMarked2Obj(k, m); 
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);

  freeimage(m);
  return(1);
} // lskel3d2()

/* =============================================================== */
int32_t lskel3d21(struct xvimage * k, int32_t nsteps) 
/* =============================================================== */
/*
    \brief Squelette surfacique (dans H3) par sélection et mémorisation
	  des isthmes 2D et 1D  - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d21"
  struct xvimage * m; // pour marquer les éléments "minces"
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_Mark2DIsthmus(k, m);
    mcskel3d_K3_Mark1DIsthmus(k, m);
    mcskel3d_K3_MarkCritic2(k, m);
    n_new = mcskel3d_K3_CriticOrMarked2Obj(k, m); 
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);

  freeimage(m);
  return(1);
} // lskel3d21()

/* =============================================================== */
int32_t lskel3d3(struct xvimage * k, int32_t nsteps) 
/* =============================================================== */
/*
    \brief Squelette curviligne (dans H3) par sélection et mémorisation
	  des isthmes 1D - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d3"
  struct xvimage * m; // pour marquer les isthmes1D
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_Mark1DIsthmus(k, m);
    mcskel3d_K3_MarkCritic2(k, m);
    n_new = mcskel3d_K3_CriticOrMarked2Obj(k, m); 
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);

  freeimage(m);
  return(1);
} // lskel3d3()

/* =============================================================== */
int32_t lskel3d4(struct xvimage * k, int32_t nsteps) 
/* =============================================================== */
/*
    \brief Noyau homotopique, version "Z^3 directe" (sans reconstruction) - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d4"
#ifdef DEBUG
  struct xvimage * k2;
#endif
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);
  
  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_MarkCritic(k);

#ifdef DEBUG
    k2 = copyimage(k);
    mcskel3d_K3_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

    n_new = mcskel3d_K3_CriticE2Obj(k);
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);
  
#ifdef DEBUG
    freeimage(k2);
#endif
  return(1);
} // lskel3d4()

/* =============================================================== */
int32_t lskel3d4b(struct xvimage * k, struct xvimage * m, int32_t nsteps) 
/* =============================================================== */
/*
    \brief Noyau homotopique avec contrainte, version "Z^3 directe" (sans reconstruction) - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d4b"
#ifdef DEBUG
  struct xvimage * k2;
#endif
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  COMPARE_SIZE(k, m);
  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(m, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_MarkCritic2(k, m);

#ifdef DEBUG
    k2 = copyimage(k);
    mcskel3d_K3_SelMarked(k2, FLAG_CRITIC);
    writeimage(k2, "_k_critic");
#endif

    n_new = mcskel3d_K3_CriticE2Obj(k);

#ifdef DEBUG
    k2 = copyimage(k);
    mcskel3d_K3_SelMarked(k2, FLAG_OBJ);
    writeimage(k2, "_k_obj");
#endif

#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);

#ifdef DEBUG
    freeimage(k2);
#endif
  return(1);
} // lskel3d4b()

/* =============================================================== */
int32_t lskel3d5(struct xvimage * k, int32_t nsteps) 
/* =============================================================== */
/*
   TODO A FAIRE
    \brief Squelette surfacique (dans H3) par sélection et mémorisation
	  des elements de courbe et surface  - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d5"
  struct xvimage * m; // pour marquer les éléments "minces"
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_Mark2DIsthmus(k, m); // A CHANGER
    mcskel3d_K3_Mark1DIsthmus(k, m); // A CHANGER
    mcskel3d_K3_MarkCritic2(k, m);
    n_new = mcskel3d_K3_CriticOrMarked2Obj(k, m); 
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);

  freeimage(m);
  return(1);
} // lskel3d5()

/* =============================================================== */
int32_t lskel3d6(struct xvimage * k, int32_t nsteps) 
/* =============================================================== */
/*
    \brief Squelette curviligne (dans H3) par sélection et mémorisation
	  des éléments de courbe - version basée sur Khalimski 3D
    \warning L'objet ne doit pas toucher le bord
 */
{
#undef F_NAME
#define F_NAME "lskel3d6"
  struct xvimage * m; // pour marquer les éléments de courbe
  index_t n_old, n_new, n;

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);

  if (nsteps == -1) nsteps = 2000000000;
  if (nsteps == 0) return 1;

  if (!mcskel3d_K3_CheckComplex(k))
  {
    fprintf(stderr, "%s: not a complex\n", F_NAME);
    exit(0);
  }

  if (!mcskel3d_K3_CheckFrame(k))
  {
    fprintf(stderr, "%s: complex must not hit the frame\n", F_NAME);
    exit(0);
  }

  m = copyimage(k);
  razimage(m);

  mcskel3d_K3_MarkObj(k);
  n_new = -1;
  n = 0;
  do
  {
    n++;
    n_old = n_new;
    mcskel3d_K3_MarkPrinc(k);
    mcskel3d_K3_MarkEss(k);
    mcskel3d_K3_MarkCurveElts(k, m);
    mcskel3d_K3_MarkCritic2(k, m);
    n_new = mcskel3d_K3_CriticOrMarked2Obj(k, m); 
#ifdef VERBOSE
    printf("step %d ; new %d ; old %d\n", n, n_new, n_old);
#endif
  } while ((n_new != n_old) && (n < nsteps));

  mcskel3d_K3_SelMarked(k, FLAG_OBJ);
  mcskel3d_K3_Binarize(k);

  freeimage(m);
  return(1);
} // lskel3d6()

/* =============================================================== */
int32_t l3dskelck(struct xvimage * k, int32_t mode, int32_t nsteps, struct xvimage * inhi) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "l3dskelck"
/*
\brief Homotopic thinning of a 3d Khalimsky image.
The maximum number of steps is given by <B>niter</B>.
If the value given for \b niter equals -1, the thinning is continued
until stability.
The parameter \b mode specifies the algorithm used for thinning.
Possible choices are:
\li 0: ultimate thinning without reconstruction, symmetrical
\li 1: ultimate crucial thinning, symmetrical
\li 2: surface thinning, symmetrical
\li 3: curve thinning, symmetrical
\li 4: curve and surface thinning, symmetrical
\li 5: curve and surface thinning (variant), symmetrical
\li 6: curve thinning (variant), symmetrical

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

\warning Input must be a complex with no point on the image border.
\author Michel Couprie
*/
{

#ifdef VERBOSE
  printf("%s: begin\n", F_NAME);
#endif

  switch(mode)
  {
  case 0: 
    if (inhi != NULL) return lskel3d4b(k, inhi, nsteps);
    else return lskel3d4(k, nsteps);
    break;
  case 1: 
    if (inhi != NULL) return lskel3d1b(k, inhi, nsteps);
    else return lskel3d1(k, nsteps);
    break;
  case 2: 
    return lskel3d2(k, nsteps);
    break;
  case 3: 
    return lskel3d3(k, nsteps);
    break;
  case 4: 
    return lskel3d21(k, nsteps);
    break;
  case 5: 
    return lskel3d5(k, nsteps);
    break;
  case 6: 
    return lskel3d6(k, nsteps);
    break;
  default: 
    fprintf(stderr, "%s: bad mode %d\n", F_NAME, mode);
    return(0);
  }
  return(1);
} // l3dskelck()
