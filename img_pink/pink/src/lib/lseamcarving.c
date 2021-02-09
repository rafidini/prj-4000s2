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
  Michel Couprie - july 2012
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseamcarving.h>

#define VERBOSE

/* =============================================================== */
int32_t lseamcarving_old(struct xvimage *in, struct xvimage *en, int32_t w, struct xvimage *out) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lseamcarving"
{
  index_t i, j, rs, cs, N, NO, imin;
  float *D;   // pour l'algo de programmation dynamique (coûts)
  uint8_t *P; // pour l'algo de programmation dynamique (pointeurs)
  uint8_t *E = UCHARDATA(en);
  uint8_t *I = UCHARDATA(in);
  uint8_t *O = UCHARDATA(out);
  int32_t r;
  float vmin;

  ONLY_2D(in);
  ONLY_2D(en);
  ONLY_2D(out);
  COMPARE_SIZE(in, en);
  ACCEPTED_TYPES1(in, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(out, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(en, VFF_TYP_1_BYTE);
  assert(tsize(in) == 1);
  assert(nbands(in) == 3);
  assert(tsize(out) == 1);
  assert(nbands(out) == 3);
  assert(tsize(en) == 1);
  assert(nbands(en) == 1);
  assert(rowsize(out) == w);
  rs = rowsize(in); assert(w <= rs);
  cs = colsize(in); assert(colsize(out) == cs);
  N = rs * cs;
  NO = w * cs;
  D = (float *)malloc(N * sizeof(float)); assert(D != NULL);
  P = (uint8_t *)malloc(N * sizeof(uint8_t)); assert(P != NULL);

  for (r = 0; r < rs-w; r++)
  {
#ifdef VERBOSE
    printf("%s: r = %d\n", F_NAME, r);
#endif
    // remplit les tableaux D et P
    for (i = 0; i < rs-r; i++) D[i] = (float)E[i]; // init 1ere ligne
    for (j = 1; j < cs; j++)
    {
      i = 0; // cas particulier 1er element
      D[j*rs + i] = D[(j-1)*rs + i];
      P[j*rs + i] = 1;
      if (D[(j-1)*rs + i+1] < D[j*rs + i])
      {
	D[j*rs + i] = D[(j-1)*rs + i+1];
	P[j*rs + i] = 2;
      }
      D[j*rs + i] += (float)E[j*rs + i];

      for (i = 1; i < (rs-r)-1; i++) // cas général
      {
	D[j*rs + i] = D[(j-1)*rs + i];
	P[j*rs + i] = 1;
	if (D[(j-1)*rs + i-1] < D[j*rs + i])
	{
	  D[j*rs + i] = D[(j-1)*rs + i-1];
	  P[j*rs + i] = 0;
	}
	if (D[(j-1)*rs + i+1] < D[j*rs + i])
	{
	  D[j*rs + i] = D[(j-1)*rs + i+1];
	  P[j*rs + i] = 2;
	}
	D[j*rs + i] += (float)E[j*rs + i];
      }

      i = (rs-r) - 1; // cas particulier dernier element
      D[j*rs + i] = D[(j-1)*rs + i];
      P[j*rs + i] = 1;
      if (D[(j-1)*rs + i-1] < D[j*rs + i])
      {
	D[j*rs + i] = D[(j-1)*rs + i-1];
	P[j*rs + i] = 0;
      }
      D[j*rs + i] += (float)E[j*rs + i];
    } // for (j = 0; j < cs; j++)
    
    // recherche d'un min dans la dernière ligne
    vmin = D[(cs-1)*rs + 0]; imin = 0;
    for (i = 1; i < rs-r; i++)
      if (D[(cs-1)*rs + i] < vmin) { vmin = D[(cs-1)*rs + i]; imin = i; }

    // remontée d'un chemin min et décalages
    for (j = cs-1; j >= 0; j--)
    {
      // décalages
      for (i = imin; i < (rs-r)-1; i++)
      {
	E[j*rs + i] = E[j*rs + i+1]; 
	I[j*rs + i] = I[j*rs + i+1]; 
	I[N + j*rs + i] = I[N + j*rs + i+1]; 
	I[N+N+ j*rs + i] = I[N+N + j*rs + i+1]; 
      }
      // maj imin
      if (P[j*rs + imin] == 0) imin -= 1;
      else if (P[j*rs + imin] == 2) imin += 1;
    } // for (j = cs-1; j >= 0; j--)
  } // for (r = 0; r < rs-w; r++)

  // recopie des valeurs dans l'image résultat
  for (j = 0; j < cs; j++)
    for (i = 0; i < w; i++)
    {
      O[j*w + i] = I[j*rs + i]; 
      O[NO + j*w + i] = I[N + j*rs + i]; 
      O[NO+NO + j*w + i] = I[N+N + j*rs + i]; 
    }

  free(D);
  free(P);
  return 1;
} // lseamcarving_old()

/* =============================================================== */
int32_t lseamcarving(struct xvimage *in, struct xvimage *en, int32_t w, struct xvimage *out) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "lseamcarving"
{
  index_t i, j, rs, cs, N, NO, imin;
  float *D;   // pour l'algo de programmation dynamique (coûts)
  uint8_t *E = UCHARDATA(en);
  uint8_t *I = UCHARDATA(in);
  uint8_t *O = UCHARDATA(out);
  int32_t r;
  float vmin;

  ONLY_2D(in);
  ONLY_2D(en);
  ONLY_2D(out);
  COMPARE_SIZE(in, en);
  ACCEPTED_TYPES1(in, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(out, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(en, VFF_TYP_1_BYTE);
  assert(tsize(in) == 1);
  assert(nbands(in) == 3);
  assert(tsize(out) == 1);
  assert(nbands(out) == 3);
  assert(tsize(en) == 1);
  assert(nbands(en) == 1);
  assert(rowsize(out) == w);
  rs = rowsize(in); assert(w <= rs);
  cs = colsize(in); assert(colsize(out) == cs);
  N = rs * cs;
  NO = w * cs;
  D = (float *)malloc(N * sizeof(float)); assert(D != NULL);


  for (r = 0; r < rs-w; r++)
  {
#ifdef VERBOSE
    printf("%s: r = %d\n", F_NAME, r);
#endif
    // remplit le tableau D
    for (i = 0; i < rs-r; i++) D[i] = (float)E[i]; // init 1ere ligne
    for (j = 1; j < cs; j++)
    {
      i = 0; // cas particulier 1er element
      D[j*rs + i] = D[(j-1)*rs + i];
      if (D[(j-1)*rs + i+1] < D[j*rs + i])
      {
	D[j*rs + i] = D[(j-1)*rs + i+1];
      }
      D[j*rs + i] += (float)E[j*rs + i];

      for (i = 1; i < (rs-r)-1; i++) // cas général
      {
	D[j*rs + i] = D[(j-1)*rs + i];
	if (D[(j-1)*rs + i-1] < D[j*rs + i])
	{
	  D[j*rs + i] = D[(j-1)*rs + i-1];
	}
	if (D[(j-1)*rs + i+1] < D[j*rs + i])
	{
	  D[j*rs + i] = D[(j-1)*rs + i+1];
	}
	D[j*rs + i] += (float)E[j*rs + i];
      }

      i = (rs-r) - 1; // cas particulier dernier element
      D[j*rs + i] = D[(j-1)*rs + i];
      if (D[(j-1)*rs + i-1] < D[j*rs + i])
      {
	D[j*rs + i] = D[(j-1)*rs + i-1];
      }
      D[j*rs + i] += (float)E[j*rs + i];
    } // for (j = 0; j < cs; j++)
    
    // recherche d'un min dans la dernière ligne
    vmin = D[(cs-1)*rs + 0]; imin = 0;
    for (i = 1; i < rs-r; i++)
      if (D[(cs-1)*rs + i] < vmin) { vmin = D[(cs-1)*rs + i]; imin = i; }

    // remontée d'un chemin min et décalages
    for (j = cs-1; j >= 0; j--)
    {
      // décalages
      for (i = imin; i < (rs-r)-1; i++)
      {
	E[j*rs + i] = E[j*rs + i+1]; 
	I[j*rs + i] = I[j*rs + i+1]; 
	I[N + j*rs + i] = I[N + j*rs + i+1]; 
	I[N+N+ j*rs + i] = I[N+N + j*rs + i+1]; 
      }
      // maj imin
      if (j > 0)
      {
	if ((imin < rs-1) && (D[(j-1)*rs + imin+1] < D[(j-1)*rs + imin])) imin += 1;
	else if ((imin > 0) && (D[(j-1)*rs + imin-1] < D[(j-1)*rs + imin])) imin -= 1;
      }
    } // for (j = cs-1; j >= 0; j--)
  } // for (r = 0; r < rs-w; r++)

  // recopie des valeurs dans l'image résultat
  for (j = 0; j < cs; j++)
    for (i = 0; i < w; i++)
    {
      O[j*w + i] = I[j*rs + i]; 
      O[NO + j*w + i] = I[N + j*rs + i]; 
      O[NO+NO + j*w + i] = I[N+N + j*rs + i]; 
    }

  free(D);
  return 1;
} // lseamcarving()
