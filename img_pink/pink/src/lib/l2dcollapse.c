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
   MC avril 2007
   l2dcollapse: collapse guidÅÈ et contraint (OBSOLETE)

   MC juin 2009
   l2dpardircollapse: collapse parallÅËle par sous-ÅÈtapes directionnelles
   l2dpardircollapse_l: collapse guidÅÈ et contraint - prioritÅÈ ULONG
   l2dpardircollapse_f: collapse guidÅÈ et contraint - prioritÅÈ FLOAT
   l2dtopoflow_l: topological flow - prioritÅÈ ULONG
   l2dtopoflow_f: topological flow - prioritÅÈ FLOAT

   MC avril 2011
   vÅÈrification empirique d'une propriÅÈtÅÈ du flow graph G :
   on considÅËre le sous-graphe H de G dont les sommets 
   correspondent aux facettes (2-faces) et aux 1 faces participant 
   Å‡ un 2-collapse ie. ayant un successeur facette
   soit x un tel sommet
   soit y l'unique sommet source de H tel que x est un descendant de y  
   alors x est dans la region de Voronoi de y
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcrlifo.h>
#include <mcrbt.h>
#include <mcindic.h>
#include <mcutil.h>
#include <mcgraphe.h>
#include <mckhalimsky2d.h>
#include <mcgeo.h>
#include <mcdrawps.h>
#include <ldist.h>
#include <lmedialaxis.h>
#include <l2dkhalimsky.h>
#include <l2dcollapse.h>

#define EN_RBT        0
#define EN_RLIFO       1
#define BORDER        2

//#define FOREST

//#define VERBOSE

//#define TRACECOL
//#define TRACECONSGRAPH
//#define DESSINECOLSEQ

/* =============================================================== */
int32_t l2dcollapse(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit)
/* =============================================================== */
/* 
  collapse sÅÈquentiel, guidÅÈ et contraint
  OBSOLETE - utiliser l2dpardircollapse_l
*/
#undef F_NAME
#define F_NAME "l2dcollapse"
{
  int32_t u, v, n, x, y, xv, yv;
  index_t i, rs, cs, N;
  uint8_t * K;
  int32_t * P;
  uint8_t * I = NULL;
  Rbt * RBT;
  index_t taillemaxrbt;
  index_t tab[GRS2D*GCS2D];

  rs = rowsize(k);
  cs = colsize(k);
  N = rs * cs;
  K = UCHARDATA(k);

  IndicsInit(N);

  if (prio == NULL)
  {
    fprintf(stderr, "%s : prio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(prio) != rs) || (colsize(prio) != cs) || (depth(prio) != 1))
  {
    fprintf(stderr, "%s : bad size for prio\n", F_NAME);
    return(0);
  }
  if (datatype(prio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(prio); 
  else 
  {
    fprintf(stderr, "%s : datatype(prio) must be int32_t\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s : bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit); 
    else 
    {
      fprintf(stderr, "%s : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
  }

  taillemaxrbt = 2 * (rs + cs);
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  fprintf(stderr, "%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    i = y*rs + x;
    if (K[i] && ((I == NULL) || (!I[i])) && FaceLibre2d(k, x, y))
    {
      mcrbt_RbtInsert(&RBT, P[i], i);
      Set(i, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    i = RbtPopMin(RBT);
    UnSet(i, EN_RBT);
    x = i % rs; y = i / rs;
    u = Collapse2d(k, x, y);
    if (u != -1)
    {
      x = u % rs; y = u / rs;
      Alphacarre2d(rs, cs, x, y, tab, &n);
      for (u = 0; u < n; u += 1)
      {
        v = tab[u];
	xv = v % rs; yv = v / rs;
	if (K[v] && !IsSet(v, EN_RBT) && ((I == NULL) || (!I[v])) && FaceLibre2d(k, xv, yv))
	{
	  mcrbt_RbtInsert(&RBT, P[v], v);
	  Set(v, EN_RBT);
	}
      }
    }
  } /* while (!mcrbt_RbtVide(RBT)) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return 1;

} /* l2dcollapse() */

/* =============================================================== */
int32_t l2dpardircollapse_l(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, int32_t priomax)
/* =============================================================== */
/* 
  collapse parallÅËle directionnel
  fonction de prioritÅÈ en entiers longs
  les ÅÈlÅÈments Å‡ prÅÈserver sont ceux de l'image "inhibit" ou, si celle-ci est "NULL", ceux supÅÈrieurs Å‡ "priomax" 
*/
#undef F_NAME
#define F_NAME "l2dpardircollapse_l"
{
  int32_t g, f, u, n, xf, yf, xg, yg;
  index_t i, rs, cs, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  int32_t * P;
  uint8_t * I = NULL;
  Rbt * RBT;
  Rlifo * RLIFO;
  Rlifo * RLIFOb;
  index_t taillemaxrbt;
  index_t tab[GRS2D*GCS2D];
  TypRbtKey p, pp;

  rs = rowsize(k);
  cs = colsize(k);
  N = rs * cs;
  K = UCHARDATA(k);

  IndicsInit(N);

  if (prio == NULL)
  {
    fprintf(stderr, "%s : prio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(prio) != rs) || (colsize(prio) != cs) || (depth(prio) != 1))
  {
    fprintf(stderr, "%s : bad size for prio\n", F_NAME);
    return(0);
  }
  if (datatype(prio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(prio); 
  else 
  {
    fprintf(stderr, "%s : datatype(prio) must be long\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s : bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit); 
    else 
    {
      fprintf(stderr, "%s : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
  }

  taillemaxrbt = (rs * cs)/8;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  RLIFO = CreeRlifoVide(taillemaxrbt);
  RLIFOb = CreeRlifoVide(taillemaxrbt);
  if ((RLIFO == NULL) || (RLIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  printf("%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = yg*rs + xg;
    if (K[g])
    {
      f = PaireLibre2d(k, xg, yg);
      if ((f != -1) && 
	  (((I != NULL) && (!I[g] && !I[f])) || 
	   ((I == NULL) && (P[g] < priomax) && (P[f] < priomax)) ) )
      {
	pp = (TypRbtKey)(mcmax(P[g],P[f]));
	mcrbt_RbtInsert(&RBT, pp, g);
	Set(g, EN_RBT);
      }
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    // construit la liste de toutes les paires libres ayant la prioritÅÈ courante
    p = RbtMinLevel(RBT); 
    while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))
    {
      g = RbtPopMin(RBT);
      UnSet(g, EN_RBT);
      xg = g % rs; yg = g / rs;
      f = PaireLibre2d(k, xg, yg);
      if (f != -1)
      {
	RlifoPush(&RLIFO, f);
	RlifoPush(&RLIFO, g);
	Set(g, EN_RLIFO);
      }
    } // while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))

    for (dir = 0; dir <= 1; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 2; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < RLIFO->Sp; i += 2) // Scan the free faces list
	  {
	    f = RLIFO->Pts[i];
	    g = RLIFO->Pts[i+1];
	    xf = f % rs; yf = f / rs;
	    if (K[f] && K[g] &&
		(((I != NULL) && (!I[g] && !I[f])) || 
		 ((I == NULL) && (P[g] < priomax) && (P[f] < priomax)) ) )
	    {
	      xg = g % rs; yg = g / rs;
	      if (xf - xg) { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else         { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      if ((DIM2D(xf,yf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE; // COLLAPSE
#ifdef TRACECOL
		printf("Collapse : %d,%d -> %d,%d\n", xg, yg, xf, yf);
#endif
		Alphacarre2d(rs, cs, xf, yf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  g = tab[u];
		  xg = g % rs; yg = g / rs;
		  if (K[g] && 
		      (((I != NULL) && (!I[g])) || ((I == NULL) && (P[g] < priomax))) )
		  {
		    f = PaireLibre2d(k, xg, yg);
		    if ((f != -1) &&
			(((I != NULL) && (!I[f])) || ((I == NULL) && (P[f] < priomax))) )
		    {
		      if ((P[g] <= p) && (P[f] <= p) && !IsSet(g, EN_RLIFO))
		      { // PrÅÈparation sous-ÅÈtapes suivantes
			RlifoPush(&RLIFOb, f);
			RlifoPush(&RLIFOb, g);
		      }
		      if (!IsSet(g, EN_RBT))
		      { // PrÅÈparation ÅÈtape suivante
			pp = (TypRbtKey)(mcmax(P[g],P[f]));
			mcrbt_RbtInsert(&RBT, pp, g);
			Set(g, EN_RBT);
		      }
		    } // if (f != -1)
		  } // if K[g] && ...
		} // for u
	      } // if ((DIM2D(xf,yf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < RLIFO->Sp; i += 2)
	  while (!RlifoVide(RLIFOb))
	  { 
	    g = RlifoPop(RLIFOb);
	    f = RlifoPop(RLIFOb);
	    RlifoPush(&RLIFO, f); 
	    RlifoPush(&RLIFO, g); 
	  }
      } // for (dim = 2; dim >= 1; dim--)
    } // for for

    RlifoFlush(RLIFO);
  } /* while (!mcrbt_RbtVide(RBT)) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  RlifoTermine(RLIFOb);
  return 1;

} /* l2dpardircollapse_l() */

/* =============================================================== */
int32_t l2dpardircollapse_f(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax)
/* =============================================================== */
/* 
  collapse parallÅËle directionnel
  fonction de prioritÅÈ en flottants
  les ÅÈlÅÈments Å‡ prÅÈserver sont ceux de l'image "inhibit" ou, si celle-ci est "NULL", ceux supÅÈrieurs Å‡ "priomax" 
*/
#undef F_NAME
#define F_NAME "l2dpardircollapse_f"
{
  int32_t g, f, u, n, xf, yf, xg, yg;
  index_t i, rs, cs, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  float * P;
  uint8_t * I = NULL;
  Rbt * RBT;
  Rlifo * RLIFO;
  Rlifo * RLIFOb;
  index_t taillemaxrbt;
  index_t tab[GRS2D*GCS2D];
  TypRbtKey p, pp;

  rs = rowsize(k);
  cs = colsize(k);
  N = rs * cs;
  K = UCHARDATA(k);

  IndicsInit(N);

  if (prio == NULL)
  {
    fprintf(stderr, "%s : prio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(prio) != rs) || (colsize(prio) != cs) || (depth(prio) != 1))
  {
    fprintf(stderr, "%s : bad size for prio\n", F_NAME);
    return(0);
  }
  if (datatype(prio) == VFF_TYP_FLOAT) 
    P = FLOATDATA(prio); 
  else 
  {
    fprintf(stderr, "%s : datatype(prio) must be float\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s : bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit); 
    else 
    {
      fprintf(stderr, "%s : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
  }

  taillemaxrbt = (rs * cs)/8;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  RLIFO = CreeRlifoVide(taillemaxrbt);
  RLIFOb = CreeRlifoVide(taillemaxrbt);
  if ((RLIFO == NULL) || (RLIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  printf("%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = yg*rs + xg;
    if (K[g])
    {
      f = PaireLibre2d(k, xg, yg);
      if ((f != -1) && 
	  (((I != NULL) && (!I[g] && !I[f])) || 
	   ((I == NULL) && (P[g] < priomax) && (P[f] < priomax)) ) )
      {
	pp = (TypRbtKey)(mcmax(P[g],P[f]));
	mcrbt_RbtInsert(&RBT, pp, g);
	Set(g, EN_RBT);
      }
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    // construit la liste de toutes les paires libres ayant la prioritÅÈ courante
    p = RbtMinLevel(RBT); 
    while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))
    {
      g = RbtPopMin(RBT);
      UnSet(g, EN_RBT);
      xg = g % rs; yg = g / rs;
      f = PaireLibre2d(k, xg, yg);
      if (f != -1)
      {
	RlifoPush(&RLIFO, f);
	RlifoPush(&RLIFO, g);
	Set(g, EN_RLIFO);
      }
    } // while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))

    for (dir = 0; dir <= 1; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 2; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < RLIFO->Sp; i += 2) // Scan the free faces list
	  {
	    f = RLIFO->Pts[i];
	    g = RLIFO->Pts[i+1];
	    xf = f % rs; yf = f / rs;
	    if (K[f] && K[g] &&
		(((I != NULL) && (!I[g] && !I[f])) || 
		 ((I == NULL) && (P[g] < priomax) && (P[f] < priomax)) ) )
	    {
	      xg = g % rs; yg = g / rs;
	      if (xf - xg) { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else         { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      if ((DIM2D(xf,yf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE; // COLLAPSE
#ifdef TRACECOL
		printf("Collapse : %d,%d -> %d,%d\n", xg, yg, xf, yf);
#endif
		Alphacarre2d(rs, cs, xf, yf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  g = tab[u];
		  xg = g % rs; yg = g / rs;
		  if (K[g] && 
		      (((I != NULL) && (!I[g])) || ((I == NULL) && (P[g] < priomax))) )
		  {
		    f = PaireLibre2d(k, xg, yg);
		    if ((f != -1) &&
			(((I != NULL) && (!I[f])) || ((I == NULL) && (P[f] < priomax))) )
		    {
		      if ((P[g] <= p) && (P[f] <= p) && !IsSet(g, EN_RLIFO))
		      { // PrÅÈparation sous-ÅÈtapes suivantes
			RlifoPush(&RLIFOb, f);
			RlifoPush(&RLIFOb, g);
		      }
		      if (!IsSet(g, EN_RBT))
		      { // PrÅÈparation ÅÈtape suivante
			pp = (TypRbtKey)(mcmax(P[g],P[f]));
			mcrbt_RbtInsert(&RBT, pp, g);
			Set(g, EN_RBT);
		      }
		    } // if (f != -1)
		  } // if K[g] && ...
		} // for u
	      } // if ((DIM2D(xf,yf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < RLIFO->Sp; i += 2)
	  while (!RlifoVide(RLIFOb))
	  { 
	    g = RlifoPop(RLIFOb);
	    f = RlifoPop(RLIFOb);
	    RlifoPush(&RLIFO, f); 
	    RlifoPush(&RLIFO, g); 
	  }
      } // for (dim = 2; dim >= 1; dim--)
    } // for for

    RlifoFlush(RLIFO);
  } /* while (!mcrbt_RbtVide(RBT)) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  RlifoTermine(RLIFOb);
  return 1;

} /* l2dpardircollapse_f() */

/* =============================================================== */
int32_t l2dpardircollapse_short(struct xvimage * k, int32_t nsteps)
/* =============================================================== */
{
#undef F_NAME
#define F_NAME "l2dpardircollapse_short"

  int result;
  result = l2dpardircollapse(k, nsteps, NULL);
  return result;
} /* l2dpardircollapse_short */

/* =============================================================== */
int32_t l2dpardircollapse(struct xvimage * k, int32_t nsteps, struct xvimage * inhibit)
/* =============================================================== */
/* 
  collapse parallÅËle directionnel
  sans fonction de prioritÅÈ
*/
#undef F_NAME
#define F_NAME "l2dpardircollapse"
{
  int32_t g, f, u, n, xf, yf, xg, yg;
  index_t i, rs, cs, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  uint8_t * I = NULL;
  Rlifo * RLIFO;
  Rlifo * RLIFOb;
  Rlifo * RLIFOt;
  index_t taillemax;
  index_t tab[GRS2D*GCS2D];

  rs = rowsize(k);
  cs = colsize(k);
  N = rs * cs;
  K = UCHARDATA(k);

  IndicsInit(N);

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s : bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit); 
    else 
    {
      fprintf(stderr, "%s : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
  }

  taillemax = (rs * cs)/4;
  RLIFO = CreeRlifoVide(taillemax);
  RLIFOb = CreeRlifoVide(taillemax);
  if ((RLIFO == NULL) || (RLIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  if (nsteps == -1) nsteps = 1000000000;

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  fprintf(stderr, "%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /* INITIALISATION DE LA RLIFO ET DE LA "BORDER" */
  /* ========================================================= */

  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = yg*rs + xg;
    if (K[g] && ((I == NULL) || (!I[g])))
    {
      f = PaireLibre2d(k, xg, yg);
      if (f != -1) 
      { 
	RlifoPush(&RLIFO, f); 
	RlifoPush(&RLIFO, g);
	Set(g, EN_RLIFO);
	xf = f % rs; yf = f / rs;
	Alphacarre2d(rs, cs, xf, yf, tab, &n);
	for (u = 0; u < n; u += 1)
	{
	  g = tab[u];
	  Set(g, BORDER);
	} // for u
      }
    }
  }

  /* ================================================ */
  /*              DEBUT BOUCLE PRINCIPALE             */
  /* ================================================ */

  while (!RlifoVide(RLIFO) && nsteps > 0)
  {
    nsteps --;

    for (dir = 0; dir <= 1; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 2; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < RLIFO->Sp; i += 2) // Scan the free faces list
	  {
	    f = RLIFO->Pts[i];
	    g = RLIFO->Pts[i+1];
	    xf = f % rs; yf = f / rs;
	    if (K[f] && K[g] && ((I == NULL) || (!I[g] && !I[f])))
	    {
	      xg = g % rs; yg = g / rs;
	      if (xf - xg) { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else         { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      if ((DIM2D(xf,yf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE; // COLLAPSE
		// PrÅÈparation sous-ÅÈtapes suivantes
		Alphacarre2d(rs, cs, xf, yf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  g = tab[u];
		  xg = g % rs; yg = g / rs;
		  if (K[g] && ((I == NULL) || (!I[g])))
		  {
		    f = PaireLibre2d(k, xg, yg);
		    if ((f != -1) && IsSet(f, BORDER) && !IsSet(g, EN_RLIFO))
		    { 
		      RlifoPush(&RLIFOb, f); 
		      RlifoPush(&RLIFOb, g);
		    }
		  }
		} // for u
	      } // if ((DIM2D(xf,yf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < RLIFO->Sp; i += 2)
	  while (!RlifoVide(RLIFOb))
	  { 
	    g = RlifoPop(RLIFOb);
	    f = RlifoPop(RLIFOb);
	    RlifoPush(&RLIFO, f); 
	    RlifoPush(&RLIFO, g); 
	  }
      } // for (dim = 2; dim >= 1; dim--)
    } // for for

    // PREPARATION ETAPE SUIVANTE
    for (i = 0; i < RLIFO->Sp; i++) UnSet(RLIFO->Pts[i], EN_RLIFO);
    for (i = 0; i < RLIFO->Sp; i += 2)
    {
      f = RLIFO->Pts[i];
      UnSet(f, BORDER);
      xf = f % rs; yf = f / rs;
      Alphacarre2d(rs, cs, xf, yf, tab, &n);
      for (u = 0; u < n; u += 1)
      {
	g = tab[u];
	UnSet(g, BORDER);
	if (K[g] && !IsSet(g, EN_RLIFO) && ((I == NULL) || (!I[g])))
	{
	  xg = g % rs; yg = g / rs;
	  f = PaireLibre2d(k, xg, yg);
	  if (f != -1) 
	  { 
	    RlifoPush(&RLIFOb, f); 
	    RlifoPush(&RLIFOb, g);
	    Set(g, EN_RLIFO);
	  }
	}
      } // for u
    }

    for (i = 0; i < RLIFOb->Sp; i += 2)
    {
      f = RLIFOb->Pts[i];
      xf = f % rs; yf = f / rs;
      Alphacarre2d(rs, cs, xf, yf, tab, &n);
      for (u = 0; u < n; u += 1)
      {
	g = tab[u];
	Set(g, BORDER);
      } // for u
    }

    RlifoFlush(RLIFO);

    RLIFOt = RLIFOb;
    RLIFOb = RLIFO;
    RLIFO = RLIFOt;

  } // while (!RlifoVide(RLIFO) && nsteps > 0)

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RlifoTermine(RLIFO);
  RlifoTermine(RLIFOb);
  return 1;

} /* l2dpardircollapse() */

// 888888888888888888888888888888888888888888888888888888888888888888
// 888888888888888888888888888888888888888888888888888888888888888888
//                        TOPOLOGICAL FLOW
// 888888888888888888888888888888888888888888888888888888888888888888
// 888888888888888888888888888888888888888888888888888888888888888888
 
/* =============================================================== */
graphe * l2dtopoflow_l(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, int32_t priomax)
/* =============================================================== */
/* 
  construction du flot topologique associÅÈ Å‡ un collapse parallÅËle directionnel
  (voir l2dpardircollapse_f)
  fonction de prioritÅÈ en entiers longs
  le rÅÈsultat est un graphe dont les sommets (faces du complexe k) 
  sont valuÅÈs par :
    TF_NOT_IN_I: la face n'appartient pas au complexe initial (avant collapse)
    TF_HEAD: la face est une tete de paire libre (face libre)
    TF_TAIL: la face est une queue de paire libre
    TF_PERMANENT: la face n'appartient pas au complexe final (aprÅËs collapse)
*/
#undef F_NAME
#define F_NAME "l2dtopoflow_l"
{
  int32_t g, gg, f, ff, u, n, xf, yf, xg, yg;
  index_t i, rs, cs, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  int32_t * PRIO;
  uint8_t * I = NULL;
  Rbt * RBT;
  Rlifo * RLIFO;
  Rlifo * RLIFOb;
  index_t taillemaxrbt;
  index_t tab[GRS2D*GCS2D];
  TypRbtKey p, pp;
  graphe * flow = NULL;
  index_t narcs, ncoll = 0;

  rs = rowsize(k);
  cs = colsize(k);
  N = rs * cs;
  K = UCHARDATA(k);

  IndicsInit(N);

  if (prio == NULL)
  {
    fprintf(stderr, "%s : prio is needed\n", F_NAME);
    return(NULL);
  }

  if ((rowsize(prio) != rs) || (colsize(prio) != cs) || (depth(prio) != 1))
  {
    fprintf(stderr, "%s : bad size for prio\n", F_NAME);
    return(NULL);
  }
  if (datatype(prio) == VFF_TYP_4_BYTE) 
    PRIO = SLONGDATA(prio); 
  else 
  {
    fprintf(stderr, "%s : datatype(prio) must be long int\n", F_NAME);
    return(NULL);
  }

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s : bad size for inhibit\n", F_NAME);
      return(NULL);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit); 
    else 
    {
      fprintf(stderr, "%s : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(NULL);
    }
  }

  taillemaxrbt = N/8;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : mcrbt_CreeRbtVide failed\n", F_NAME);
    return(NULL);
  }

  RLIFO = CreeRlifoVide(taillemaxrbt);
  RLIFOb = CreeRlifoVide(taillemaxrbt);
  if ((RLIFO == NULL) || (RLIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(NULL);
  }

  /* ================================================ */
  /*               INITIALISATION GRAPHE              */
  /* ================================================ */

  narcs = 0; // evalue la taille max du graphe
  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    if (K[yg*rs + xg]) 
    { 
      if (CARRE(xg,yg)) narcs += 8;
      else if (INTER(xg,yg)) narcs += 2;
    }
  }
  flow = InitGraphe(N, narcs); // toutes les faces (dans K ou non) sont des sommets
  if (flow == NULL)
  {
    fprintf(stderr, "%s : InitGraphe failed\n", F_NAME);
    return(NULL);
  }
  for (yg = 0; yg < cs; yg++) // coordonnÅÈes des sommets
  for (xg = 0; xg < rs; xg++)
  {
    g = yg*rs + xg;
    flow->x[g] = xg; // coordonnÅÈes des sommets
    flow->y[g] = yg;
    if (!K[g]) flow->v_sommets[g] = TF_NOT_IN_I;
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  fprintf(stderr, "%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = yg*rs + xg;
    if (K[g])
    {
      f = PaireLibre2d(k, xg, yg);
      if ((f != -1) && 
	  (((I != NULL) && (!I[g] && !I[f])) || 
	   ((I == NULL) && (PRIO[g] < priomax) && (PRIO[f] < priomax)) ) )
      {
	pp = (TypRbtKey)(mcmax(PRIO[g],PRIO[f]));
	mcrbt_RbtInsert(&RBT, pp, g);
	Set(g, EN_RBT);
      }
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    // construit la liste de toutes les paires libres ayant la prioritÅÈ courante
    p = RbtMinLevel(RBT); 
    while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))
    {
      g = RbtPopMin(RBT);
      UnSet(g, EN_RBT);
      xg = g % rs; yg = g / rs;
      f = PaireLibre2d(k, xg, yg);
      if (f != -1)
      {
	RlifoPush(&RLIFO, f);
	RlifoPush(&RLIFO, g);
	Set(g, EN_RLIFO);
      }
    } // while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))

    for (dir = 0; dir <= 1; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 2; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < RLIFO->Sp; i += 2) // Scan the free faces list
	  {
	    f = RLIFO->Pts[i];
	    g = RLIFO->Pts[i+1];
	    xf = f % rs; yf = f / rs;
	    if (K[f] && K[g] && 
		(((I != NULL) && (!I[g] && !I[f])) || 
		 ((I == NULL) && (PRIO[g] < priomax) && (PRIO[f] < priomax)) ) )
	    {
	      xg = g % rs; yg = g / rs;
	      if (xf - xg) { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else         { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      if ((DIM2D(xf,yf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE; // COLLAPSE
		ncoll ++;
#ifdef TRACECOL
		printf("Collapse : %d,%d -> %d,%d\n", xg, yg, xf, yf);
#endif
		flow->v_sommets[g] = TF_HEAD;
#ifdef DESSINECOLSEQ
		flow->v_sommets[f] = -ncoll;//pour dessiner une col. seq.
#else
      		flow->v_sommets[f] = TF_TAIL;
#endif
		AjouteArcValue(flow, g, f, (TYP_VARC)1);
#ifdef TRACECONSGRAPH
		printf("Ajoute arc : %d [%d,%d] -> %d [%d,%d] (%g)\n", g, xg, yg, f, xf, yf, (TYP_VARC)1);
#endif
		Alphacarre2d(rs, cs, xf, yf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  gg = tab[u];
		  xg = gg % rs; yg = gg / rs;
		  if (K[gg]) 
		  {
#ifdef TRACECONSGRAPH
		    printf("Ajoute arc : %d [%d,%d] -> %d [%d,%d] (%g)\n", f, xf, yf, gg, xg, yg, (TYP_VARC)1);
#endif
		    AjouteArcValue(flow, f, gg, (TYP_VARC)1);
		  }
		  if (K[gg] && 
		      (((I != NULL) && (!I[gg])) || ((I == NULL) && (PRIO[gg] < priomax))) )
		  {
		    ff = PaireLibre2d(k, xg, yg);
		    if ((ff != -1) &&
			(((I != NULL) && (!I[ff])) || ((I == NULL) && (PRIO[ff] < priomax))) )
		    {
		      if ((PRIO[gg] <= p) && (PRIO[ff] <= p) && !IsSet(gg, EN_RLIFO))
		      { // PrÅÈparation sous-ÅÈtapes suivantes
			RlifoPush(&RLIFOb, ff);
			RlifoPush(&RLIFOb, gg);
		      }
		      if (!IsSet(gg, EN_RBT))
		      { // PrÅÈparation ÅÈtape suivante
			pp = (TypRbtKey)(mcmax(PRIO[gg],PRIO[ff]));
			mcrbt_RbtInsert(&RBT, pp, gg);
			Set(gg, EN_RBT);
		      }
		    } // if (ff != -1)
		  } // if K[gg] && ...
		} // for u
	      } // if ((DIM2D(xf,yf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < RLIFO->Sp; i += 2)
	  while (!RlifoVide(RLIFOb))
	  { 
	    g = RlifoPop(RLIFOb);
	    f = RlifoPop(RLIFOb);
	    RlifoPush(&RLIFO, f); 
	    RlifoPush(&RLIFO, g); 
	  }
      } // for (dim = 2; dim >= 1; dim--)
    } // for for

    RlifoFlush(RLIFO);

  } /* while (!mcrbt_RbtVide(RBT)) */

  for (g = 0; g < N; g++)
    if (K[g]) flow->v_sommets[g] = TF_PERMANENT;

#ifdef VERBOSE
  fprintf(stderr, "%s: Fin traitement\n", F_NAME);
#endif

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  RlifoTermine(RLIFOb);
  return(flow);

} /* l2dtopoflow_l() */
 
/* =============================================================== */
graphe * l2dtopoflow_f(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax)
/* =============================================================== */
/* 
  construction du flot topologique associÅÈ Å‡ un collapse parallÅËle directionnel
  (voir l2dpardircollapse_f)
  fonction de prioritÅÈ en flottants
  le rÅÈsultat est un graphe dont les sommets (faces du complexe k) 
  sont valuÅÈs par :
    TF_NOT_IN_I: la face n'appartient pas au complexe initial (avant collapse)
    TF_HEAD: la face est une tete de paire libre (face libre)
    TF_TAIL: la face est une queue de paire libre
    TF_PERMANENT: la face n'appartient pas au complexe final (aprÅËs collapse)
*/
#undef F_NAME
#define F_NAME "l2dtopoflow_f"
{
  int32_t g, gg, f, ff, u, n, xf, yf, xg, yg;
  index_t i, rs, cs, N;
  int32_t dim, ori, dir, direc, orien;
  uint8_t * K;
  float * P;
  uint8_t * I = NULL;
  Rbt * RBT;
  Rlifo * RLIFO;
  Rlifo * RLIFOb;
  index_t taillemaxrbt;
  index_t tab[GRS2D*GCS2D];
  TypRbtKey p, pp;
  graphe * flow = NULL;
  index_t narcs, ncoll = 0;

  rs = rowsize(k);
  cs = colsize(k);
  N = rs * cs;
  K = UCHARDATA(k);

  IndicsInit(N);

  if (prio == NULL)
  {
    fprintf(stderr, "%s : prio is needed\n", F_NAME);
    return(NULL);
  }

  if ((rowsize(prio) != rs) || (colsize(prio) != cs) || (depth(prio) != 1))
  {
    fprintf(stderr, "%s : bad size for prio\n", F_NAME);
    return(NULL);
  }
  if (datatype(prio) == VFF_TYP_FLOAT) 
    P = FLOATDATA(prio); 
  else 
  {
    fprintf(stderr, "%s : datatype(prio) must be float\n", F_NAME);
    return(NULL);
  }

  if (inhibit != NULL)
  {
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s : bad size for inhibit\n", F_NAME);
      return(NULL);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit); 
    else 
    {
      fprintf(stderr, "%s : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(NULL);
    }
  }

  taillemaxrbt = N/8;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s : mcrbt_CreeRbtVide failed\n", F_NAME);
    return(NULL);
  }

  RLIFO = CreeRlifoVide(taillemaxrbt);
  RLIFOb = CreeRlifoVide(taillemaxrbt);
  if ((RLIFO == NULL) || (RLIFOb == NULL))
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(NULL);
  }

  /* ================================================ */
  /*               INITIALISATION GRAPHE              */
  /* ================================================ */

  narcs = 0; // evalue la taille max du graphe
  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    if (K[yg*rs + xg]) 
    { 
      if (CARRE(xg,yg)) narcs += 8;
      else if (INTER(xg,yg)) narcs += 2;
    }
  }
  flow = InitGraphe(N, narcs); // toutes les faces (dans K ou non) sont des sommets
  if (flow == NULL)
  {
    fprintf(stderr, "%s : InitGraphe failed\n", F_NAME);
    return(NULL);
  }
  for (yg = 0; yg < cs; yg++) // coordonnÅÈes des sommets
  for (xg = 0; xg < rs; xg++)
  {
    g = yg*rs + xg;
    flow->x[g] = xg; // coordonnÅÈes des sommets
    flow->y[g] = yg;
    if (!K[g]) flow->v_sommets[g] = TF_NOT_IN_I;
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

#ifdef VERBOSE
  fprintf(stderr, "%s: Debut traitement\n", F_NAME);
#endif

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (yg = 0; yg < cs; yg++)
  for (xg = 0; xg < rs; xg++)
  {
    g = yg*rs + xg;
    if (K[g])
    {
      f = PaireLibre2d(k, xg, yg);
      if ((f != -1) && 
	  (((I != NULL) && (!I[g] && !I[f])) || 
	   ((I == NULL) && (P[g] < priomax) && (P[f] < priomax)) ) )
      {
	pp = (TypRbtKey)(mcmax(P[g],P[f]));
	mcrbt_RbtInsert(&RBT, pp, g);
	Set(g, EN_RBT);
      }
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    // construit la liste de toutes les paires libres ayant la prioritÅÈ courante
    p = RbtMinLevel(RBT); 
    while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))
    {
      g = RbtPopMin(RBT);
      UnSet(g, EN_RBT);
      xg = g % rs; yg = g / rs;
      f = PaireLibre2d(k, xg, yg);
      if (f != -1)
      {
	RlifoPush(&RLIFO, f);
	RlifoPush(&RLIFO, g);
	Set(g, EN_RLIFO);
      }
    } // while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == p))

    for (dir = 0; dir <= 1; dir++) // For all face directions
      for (ori = 0; ori <= 1; ori++) // For both orientations
      {
	for (dim = 2; dim >= 1; dim--) // For dimensions in decreasing order
	{
	  for (i = 0; i < RLIFO->Sp; i += 2) // Scan the free faces list
	  {
	    f = RLIFO->Pts[i];
	    g = RLIFO->Pts[i+1];
	    xf = f % rs; yf = f / rs;
	    if (K[f] && K[g] && 
		(((I != NULL) && (!I[g] && !I[f])) || 
		 ((I == NULL) && (P[g] < priomax) && (P[f] < priomax)) ) )
	    {
	      xg = g % rs; yg = g / rs;
	      if (xf - xg) { direc = 0; if (xf > xg) orien = 0; else orien = 1; }
	      else         { direc = 1; if (yf > yg) orien = 0; else orien = 1; }
	      if ((DIM2D(xf,yf) == dim) && (direc == dir) && (orien == ori))
	      {
		K[g] = K[f] = VAL_NULLE; // COLLAPSE
		ncoll ++;
#ifdef TRACECOL
		printf("Collapse : %d,%d -> %d,%d\n", xg, yg, xf, yf);
#endif
		flow->v_sommets[g] = TF_HEAD;
#ifdef DESSINECOLSEQ
		flow->v_sommets[f] = -ncoll;//pour dessiner une col. seq.
#else
      		flow->v_sommets[f] = TF_TAIL;
#endif
		AjouteArcValue(flow, g, f, (TYP_VARC)1);
#ifdef TRACECONSGRAPH
		printf("Ajoute arc : %d [%d,%d] -> %d [%d,%d] (%g)\n", g, xg, yg, f, xf, yf, (TYP_VARC)1);
#endif
		Alphacarre2d(rs, cs, xf, yf, tab, &n);
		for (u = 0; u < n; u += 1)
		{
		  gg = tab[u];
		  xg = gg % rs; yg = gg / rs;
		  if (K[gg]) 
		  {
#ifdef TRACECONSGRAPH
		    printf("Ajoute arc : %d [%d,%d] -> %d [%d,%d] (%g)\n", f, xf, yf, gg, xg, yg, (TYP_VARC)1);
#endif
		    AjouteArcValue(flow, f, gg, (TYP_VARC)1);
		  }
		  if (K[gg] && 
		      (((I != NULL) && (!I[gg])) || ((I == NULL) && (P[gg] < priomax))) )
		  {
		    ff = PaireLibre2d(k, xg, yg);
		    if ((ff != -1) &&
			(((I != NULL) && (!I[ff])) || ((I == NULL) && (P[ff] < priomax))) )
		    {
		      if ((P[gg] <= p) && (P[ff] <= p) && !IsSet(gg, EN_RLIFO))
		      { // PrÅÈparation sous-ÅÈtapes suivantes
			RlifoPush(&RLIFOb, ff);
			RlifoPush(&RLIFOb, gg);
		      }
		      if (!IsSet(gg, EN_RBT))
		      { // PrÅÈparation ÅÈtape suivante
			pp = (TypRbtKey)(mcmax(P[gg],P[ff]));
			mcrbt_RbtInsert(&RBT, pp, gg);
			Set(gg, EN_RBT);
		      }
		    } // if (ff != -1)
		  } // if K[gg] && ...
		} // for u
	      } // if ((DIM2D(xf,yf) == dim) &&...
	    } // if (K[f] && K[g])
	  } // for (i = 0; i < RLIFO->Sp; i += 2)
	  while (!RlifoVide(RLIFOb))
	  { 
	    g = RlifoPop(RLIFOb);
	    f = RlifoPop(RLIFOb);
	    RlifoPush(&RLIFO, f); 
	    RlifoPush(&RLIFO, g); 
	  }
      } // for (dim = 2; dim >= 1; dim--)
    } // for for

    RlifoFlush(RLIFO);

  } /* while (!mcrbt_RbtVide(RBT)) */

  for (g = 0; g < N; g++)
    if (K[g]) flow->v_sommets[g] = TF_PERMANENT;

#ifdef VERBOSE
  fprintf(stderr, "%s: Fin traitement\n", F_NAME);
#endif

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  RlifoTermine(RLIFOb);
  return(flow);

} /* l2dtopoflow_f() */

//#define DESSINECOLSEQ
#define SHOWGRAPHS
//#define SHOWCONFLUENTPOINTS
#define SHOWIMAGES

#define TESTE_PROPRIETE

#define FS_ALPHA 0.1
#define MARGEX1 5
#define MARGEY1 5
#define MARGEX2 25
#define MARGEY2 25
#define FONTSIZE 12

#ifdef SHOWGRAPHS
static void ShowGraphe(graphe * g, char *filename, double s, double r, double t, int noms_sommets, int v_sommets, int col_sommets, int all_arcs, uint8_t *K, int rs, boolean *head) 
/* ====================================================================== */
#undef F_NAME
#define F_NAME "ShowGraphe"
{
  int i, j, xx, yy, n = g->nsom;
  double xmin, xmax, ymin, ymax;
  double x1, y1, x2, y2, x3, y3, x, y, a, b, d;
  pcell p;
  FILE * fd = NULL;
  char buf[80];
  
  if (g->gamma == NULL) 
  {  fprintf(stderr, "%s: representation successeurs absente\n", F_NAME);
     return;
  }
  
  if (g->x == NULL) 
  {  fprintf(stderr, "%s: coordonnees des sommets absentes\n", F_NAME);
     return;
  }

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    return;
  }

  /* determine le rectangle englobant et genere le header */
  xmin = xmax = s*g->x[0];
  ymin = ymax = s*g->y[0];
  for (i = 1; i < n; i++) 
  {
    if (s*g->x[i] < xmin) xmin = s*g->x[i]; else if (s*g->x[i] > xmax) xmax = s*g->x[i];
    if (s*g->y[i] < ymin) ymin = s*g->y[i]; else if (s*g->y[i] > ymax) ymax = s*g->y[i];
  }
  EPSHeader(fd, xmax - xmin + MARGEX1 + MARGEX2, ymax - ymin + MARGEY1 + MARGEY2, 1.0, FONTSIZE);
  
  /* dessine le fond */
  PSSetColor (fd, 1.0);
  PSDrawRect (fd, 0, 0, xmax - xmin + MARGEX1 + MARGEX2, ymax - ymin + MARGEY1 + MARGEY2);
  PSSetColor (fd, 0.0);

  /* dessine le complexe */
  PSSetColor (fd, 0.80);
  PSSetLineWidth (fd, 5);    
  for (i = 0; i < n; i++) 
    if (K[i])
    {
      xx = i%rs;
      yy = i/rs;
      if ((xx%2 == 0) && (yy%2 == 0))
      {
	PSDrawdisc(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, 5);
	if ((xx < rs-2) && K[i+2])
	  PSLine(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, s*g->x[i+2]-xmin+MARGEX1, s*g->y[i+2]-ymin+MARGEY1);
	if ((i < n-(rs+rs)) && K[i+rs+rs])
	  PSLine(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, s*g->x[i+rs+rs]-xmin+MARGEX1, s*g->y[i+rs+rs]-ymin+MARGEY1);
      }
    }
  PSSetColor (fd, 0.0);
  PSSetLineWidth (fd, 1);

  if (noms_sommets && g->nomsommet)
    for (i = 0; i < n; i++) 
      if (K[i])
	PSString(fd, s*g->x[i]-xmin+MARGEX1+2*r, s*g->y[i]-ymin+MARGEY1-2*r, g->nomsommet[i]);

  /* dessine les arcs */
  PSSetColor (fd, 0.50);
  for (i = 0; i < n; i++)
    if (all_arcs || head[i])
      for (p = g->gamma[i]; p != NULL; p = p->next)
      {
	j = p->som;
	PSLine(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, s*g->x[j]-xmin+MARGEX1, s*g->y[j]-ymin+MARGEY1);
      }

  /* dessine les fleches sur les arcs */
  if (t > 0.0)
  {
    for (i = 0; i < n; i++) 
    if (all_arcs || head[i])
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      x1 = s*g->x[i]-xmin+MARGEX1;
      y1 = s*g->y[i]-ymin+MARGEY1;
      x2 = s*g->x[j]-xmin+MARGEX1;
      y2 = s*g->y[j]-ymin+MARGEY1;
      x = (x2 + x1) / 2; // milieu de l'arc
      y = (y2 + y1) / 2;
      a = x2 - x1;
      b = y2 - y1;             /* (a,b) est un vecteur directeur de l'arc */
      d = sqrt(a * a + b * b); /* longueur de l'arc */
      if (d > 1) // sinon on ne dessine pas la fleche
      { 
        a /= d; b /= d;          /* norme le vecteur */
        x1 = x + 2 * t * a;
        y1 = y + 2 * t * b;      /* pointe de la fleche */
        x2 = x - 2 * t * a;
        y2 = y - 2 * t * b;      /* base de la fleche */
        x3 = x2 + t * -b;        /* (-b,a) est normal a (a,b) */
        y3 = y2 + t * a;
        x2 = x2 - t * -b;
        y2 = y2 - t * a;
        PSLine(fd, x1, y1, x2, y2);
        PSLine(fd, x2, y2, x3, y3);
        PSLine(fd, x3, y3, x1, y1);
      }
    }
  }
  PSSetColor (fd, 0.0);

  /* dessine les sommets */
  for (i = 0; i < n; i++) 
    if (K[i])
    {
      if (col_sommets && (g->v_sommets[i] == 1)) 
	PSDrawdisc(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, 8*r);
      else if (col_sommets && (g->v_sommets[i] == 2)) 
      {
	PSSetColor(fd, 1.0);
	PSDrawdisc(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, 8*r);
	PSSetColor(fd, 0.0);
	PSDrawcircle(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, 8*r);
      }
      else
	PSDrawcircle(fd, s*g->x[i]-xmin+MARGEX1, s*g->y[i]-ymin+MARGEY1, r);
    }
  if (v_sommets)
    for (i = 0; i < n; i++)
    { 
#ifdef DESSINECOLSEQ
      if (K[i] && !head[i] && (g->v_sommets[i] < 0))
      { //pour dessiner une col. seq.
	sprintf(buf, "%g", -(double)(g->v_sommets[i]));
	PSString(fd, s*g->x[i]-xmin+MARGEX1+2*r, s*g->y[i]-ymin+MARGEY1+2*r, buf);
      }
#else
      if (K[i])
      {
	sprintf(buf, "%.1f", (double)(g->v_sommets[i]));
	PSString(fd, s*g->x[i]-xmin+MARGEX1+2*r, s*g->y[i]-ymin+MARGEY1+2*r+2, buf);
      }
#endif
    }

  PSFooter(fd);
  fclose(fd);
} // ShowGraphe()
#endif

#ifdef TESTE_PROPRIETE
struct  CheckPropRes{
  int32_t npoints;
  double max, sum;
};
static void CheckProperty(graphe *H,
			  int32_t s, int32_t x, uint32_t *distimage,
			  index_t rs, index_t cs, 
			  int32_t *TabIndDec, int32_t nval, 
			  Coordinates *ListDecs,
			  ListDPoint2D Aval,
			  struct CheckPropRes *res
			  )
{
  int32_t y, i, j, n, card_aval;
  pcell p;
  double d, mind;

  p = H->gamma[x];
  if (p == NULL) return;
  for (; p != NULL; p = p->next)
  {
    y = p->som;
    i = y % rs;
    j = y / rs;
    card_aval = lmedialaxis_ExtendedDownstream(i, j, distimage, rs, cs, TabIndDec, nval, ListDecs, Aval);	  
    for (mind = (double)(rs*cs), n = 0; n < card_aval; n++)
    {
      d = dist2((double)(s%rs), (double)(s/rs), Aval[n].xCoor, Aval[n].yCoor);
      if (d < mind) mind = d;
    }
    mind -= 1;
    //    printf("%d,%d : mind = %g\n", i, j, mind);
    res->npoints += 1;
    res->sum += mind;
    if (mind > res->max) res->max = mind;
    CheckProperty(H, s, y, distimage, rs, cs, TabIndDec, nval, ListDecs, Aval, res);
  }
} // CheckProperty()
#endif

/* =============================================================== */
int32_t l2dflowskeleton(struct xvimage * k, int32_t mode, double level, struct xvimage * func) 
/* =============================================================== */
#undef F_NAME
#define F_NAME "l2dflowskeleton"
{
  struct xvimage * kk;
  struct xvimage * dist;
  struct xvimage * lambda;
  uint8_t *K;
  index_t rs, cs, ds=1, N, i, j;
  float * FUNC;
  float * LAMBDA;
  graphe * flow;
  boolean * perm;
  boolean * head;
  TYP_VSOM vmax;
  TYP_VSOM alpha = FS_ALPHA;

  ONLY_2D(k);
  COMPARE_SIZE(k, func);
  ACCEPTED_TYPES1(k, VFF_TYP_1_BYTE);
  ACCEPTED_TYPES1(func, VFF_TYP_FLOAT);

  rs = rowsize(k);
  cs = colsize(k);
  N = rs*cs;
  K = UCHARDATA(k);

  dist = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  assert(dist != NULL);
  if (! lsedt_meijster(k, dist))
  {
    fprintf(stderr, "%s: lsedt_meijster failed\n", F_NAME);
    return 0;
  }

  lambda = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
  assert(lambda != NULL);
  if (!llambdamedialaxis(dist, lambda))
  {
    fprintf(stderr, "%s: llambdamedialaxis failed\n", F_NAME);
    return 0;
  }

#ifdef SHOWIMAGES
  writeimage(lambda, "_lambda");
#endif

  // -----------------------------------------------------------
  // 1ERE ETAPE : CALCULE LE FLOW GRAPH
  // -----------------------------------------------------------
  kk = copyimage(k); // sauve k car l2dtopoflow modifie le complexe
  assert(kk != NULL);
#define PRIO_LAMBDA
#ifdef PRIO_LAMBDA
  if (! (flow = l2dtopoflow_f(kk, lambda, NULL, FLT_MAX)))
  {
    fprintf(stderr, "%s: function l2dtopoflow_f failed\n", F_NAME);
    return 0;
  }
#endif
//#define PRIO_DIST
#ifdef PRIO_DIST
  if (! (flow = l2dtopoflow_l(kk, dist, NULL, INT32_MAX)))
  {
    fprintf(stderr, "%s: function l2dtopoflow_f failed\n", F_NAME);
    return 0;
  }
#endif
//#define PRIO_UNIFORM
#ifdef PRIO_UNIFORM
  razimage(dist);
  if (! (flow = l2dtopoflow_l(kk, dist, NULL, INT32_MAX)))
  {
    fprintf(stderr, "%s: function l2dtopoflow_f failed\n", F_NAME);
    return 0;
  }
  razimage(dist);
  if (! lsedt_meijster(k, dist))
  {
    fprintf(stderr, "%s: lsedt_meijster failed\n", F_NAME);
    return 0;
  }
#endif

  freeimage(kk);

  perm = (boolean *)calloc(N, sizeof(boolean)); assert(perm != NULL);
  head = (boolean *)calloc(N, sizeof(boolean)); assert(head != NULL);
  for (i = 0; i < N; i++)
    if (flow->v_sommets[i] == TF_PERMANENT)
      perm[i] = TRUE;
    else if (flow->v_sommets[i] == TF_HEAD)
      head[i] = TRUE;

#ifdef SHOWGRAPHS
#ifdef DESSINECOLSEQ
  ShowGraphe(flow, "_flow", 30, 1, 3, 0, 1, 0, 0, K, rs, head);//pour dessiner une col. seq.
#else
  ShowGraphe(flow, "_flow1", 30, 1, 3, 0, 0, 0, 1, K, rs, head);
#endif
#ifdef SHOWCONFLUENTPOINTS
  {
    graphe * flow_s = Symetrique(flow);
    int32_t a;
    char buf[1024];
    //    struct xvimage *img = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    //    uint8_t *I = UCHARDATA(img);
    //    for (i = 0; i < N; i++) I[i] = (D[i]?255:0);
    //    writeimage(img, "_amont");
    //    freeimage(img);
    assert(N < 1000);
    for (a = 0; a < N; a++) if (K[a])
    {
      boolean *D = Descendants(flow_s, a);
      for (i = 0; i < N; i++) 
	if (D[i]) 
	  flow->v_sommets[i] = (TYP_VSOM)1; 
	else 
	  flow->v_sommets[i] = (TYP_VSOM)0;
      flow->v_sommets[a] = (TYP_VSOM)2; 
      sprintf(buf, "_amont%d_%d", a%rs, a/rs);
      printf("showing: %s\n", buf);
      ShowGraphe(flow, buf, 30, 1, 3, 0, 0, 1, 1, K, rs, head);
      free(D);
    }
    TermineGraphe(flow_s);
  }
#endif
#endif

#ifdef FOREST
  {   
    graphe * flow_s = Symetrique(flow);
    graphe * forest_s = ForetPCC(flow_s);
    TermineGraphe(flow);
    flow = Symetrique(forest_s);
    TermineGraphe(flow_s);
    TermineGraphe(forest_s);
  }
#endif

#ifdef TESTE_PROPRIETE
/*
   on considÅËre le sous-graphe H de flow dont les sommets 
   correspondent aux facettes (2-faces) et aux 1-faces participant 
   Å‡ un 2-collapse ie. ayant un successeur facette
   soit x un tel sommet
   soit y l'unique sommet source de H tel que x est un descendant de y  
   alors x est dans la region de Voronoi de y
*/
  {
    graphe * H = InitGraphe(flow->nsom, flow->nmaxarc);
    graphe * flow_s = Symetrique(flow);
    int32_t a, b, ib, jb, xg, yg, g;
    pcell p;
    struct CheckPropRes res;
    uint8_t *KK;
  
    FILE *fd=NULL;
    int32_t nval, npoints, npointsmax;
    int32_t *TabIndDec;
    Coordinates *ListDecs;
    ListDPoint2D Aval;
    char tablefilename[512];
    uint32_t distmax, *D; 
    uint32_t nsom_H, nsour_H;
    
    D = ULONGDATA(dist);
    distmax = 0;  // calcule la distance max dans l'image de distance
    for (i = 0; i < N; i++)
      if ((D[i] > distmax)) distmax = D[i];
    distmax++;
    Aval= (struct DPoint2D *)calloc(N, sizeof(struct DPoint2D)); assert(Aval != NULL);
    sprintf(tablefilename, "%s/src/tables/TabBisector_1.txt", getenv("PINK"));
    fd = fopen (tablefilename, "r"); assert(fd != NULL); 
    /* Reading the bisector table from the files */
    sprintf(tablefilename, "%s/src/tables/TabBisector_1.txt", getenv("PINK"));
    fd = fopen (tablefilename, "r"); assert(fd != NULL);
    fscanf(fd,"%d",&nval); assert(distmax < nval);
    TabIndDec = (int32_t *)calloc(1,(distmax+2) * sizeof(int32_t)); assert(TabIndDec != NULL) ;
    for (i = 0; i <= distmax+1; i++) fscanf(fd,"%d", &TabIndDec[i]);
    npointsmax = TabIndDec[distmax];
    fclose(fd);
    sprintf(tablefilename, "%s/src/tables/TabBisector_2.txt", getenv("PINK"));
    fd = fopen (tablefilename, "r"); assert(fd != NULL); 
    fscanf(fd,"%d",&npoints);
    ListDecs = (Coordinates *)calloc(1,npointsmax * sizeof(Coordinates)); assert(ListDecs != NULL);
    for (i = 0; i < npointsmax; i++) fscanf(fd,"%d%d", &ListDecs[i].x, &ListDecs[i].y);
    fclose (fd);

    kk = copyimage(k); assert(kk != NULL); razimage(kk);
    KK = UCHARDATA(kk); // pour reperer les sources

    for (yg = 0; yg < cs; yg++) // coordonnÅÈes des sommets
    for (xg = 0; xg < rs; xg++)
    {
      g = yg*rs + xg;
      H->x[g] = xg; // coordonnÅÈes des sommets
      H->y[g] = yg;
      if (!K[g]) H->v_sommets[g] = TF_NOT_IN_I;
    }

    nsom_H  = 0;
    nsour_H = 0;
    for (j = 0; j < cs; j++)
    for (i = 0; i < rs; i++)
    {
      a = j*rs + i;
      if (K[a] && INTER(i,j))
      {
	for (p = flow->gamma[a]; p != NULL; p = p->next)
	{
	  b = p->som; ib = b % rs; jb = b / rs;
	  if CARRE(ib,jb) break;
	}
	if (p != NULL) // 'a' possede un successeur 2D 'b' dans flow
	{
	  AjouteArcValue(H, a, b, (TYP_VARC)1);
	  nsom_H += 2;
	  if (flow_s->gamma[a] == NULL) // 'a' est une source de flow
          { KK[a] = 2; nsour_H += 1; } 
	  else
	    KK[a] = 1;
	}
      }
    }

    for (j = 0; j < cs; j++)
    for (i = 0; i < rs; i++)
    {
      a = j*rs + i;
      if (K[a] && CARRE(i,j))
      {
	for (p = flow->gamma[a]; p != NULL; p = p->next)
	{
	  b = p->som;
	  if (KK[b])
	    AjouteArcValue(H, a, b, (TYP_VARC)2);
	}
      }
    }

    ShowGraphe(H, "_H", 30, 1, 3, 0, 0, 0, 1, K, rs, head);
    printf("nsom_H = %d nsour_H = %d\n", nsom_H, nsour_H);

    res.npoints = 0;
    res.sum = 0;
    res.max = 0;
    for (a = 0; a < N; a++)
    {
      if (KK[a] == 2) // source
      {

	CheckProperty(H, a, a, D, rs, cs, TabIndDec, nval, ListDecs, Aval, &res);
	
      }
    }

    printf("CheckProperty : npoints = %d, max = %g, mean = %g\n", res.npoints, res.max, res.sum / res.npoints);

    TermineGraphe(H);
    TermineGraphe(flow_s);
    freeimage(kk);
  }
#endif
  
  // -------------------------------------------------------------------
  // 2EME ETAPE : CALCULE LA FONCTION DE POIDS INITIALE POUR LES SOMMETS
  // -------------------------------------------------------------------
  LAMBDA = FLOATDATA(lambda);
  FUNC = FLOATDATA(func);

  if (mode == 0)
  { // fonction uniformÅÈment nulle 
    razimage(func);
  }
  else if (mode == 1)
  { // fonction uniforme (unitÅÈ)
    for (i = 0; i < N; i++) 
      if (K[i])
	FUNC[i] = (float)1;
      else
	FUNC[i] = (float)0;
  }
  else if ((mode == 2) || (mode == 3))
  { // fonction uniforme sur la frontiÅËre, nulle Å‡ l'intÅÈrieur 
    struct xvimage * border = copyimage(k);
    uint8_t *B;
    assert(border != NULL);
    if (! l2dborder(border))
    {
      fprintf(stderr, "%s: function l2dborder failed\n", F_NAME);
      return 0;
    }
#ifdef SHOWIMAGES
    writeimage(border, "_border");
#endif
    B = UCHARDATA(border);
    for (i = 0; i < N; i++) 
      if (B[i]) 
	FUNC[i] = (float)1;
      else
	FUNC[i] = (float)0;
    freeimage(border);
  }
  else if (mode == 4)
  { // fonction d'ouverture inversÅÈe
    int32_t ret;
    uint32_t *OF, maxof;
    struct xvimage *of = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    assert(of != NULL);
    ret = lopeningfunction(k, of, 0); assert(ret == 1);
    OF = ULONGDATA(of);
    maxof = OF[0];
    for (i = 0; i < N; i++) if (OF[i] > maxof) maxof = OF[i];
    for (i = 0; i < N; i++) FUNC[i] = (float)(maxof - OF[i]) + 0.1;
    freeimage(of);
  }
  else if (mode == 5)
  { // fonction bisectrice
    if (! lmedialaxis_lbisector(dist, k, func))
    {
      fprintf(stderr, "%s: lmedialaxis_lbisector failed\n", F_NAME);
      return 0;
    }
    MaxAlpha2d(func); // fermeture (en ndg)
  }
  else if (mode == 6)
  { // distance map
    uint32_t *D = ULONGDATA(dist);
    for (i = 0; i < N; i++) FUNC[i] = (float)(1.0/sqrt(D[i]));
  }
  else if (mode == 7)
  { // lambda function
    copy2image(func, lambda);
    MaxAlpha2d(func); // fermeture (en ndg)
  }
  else if (mode == 8)
  { // fonction uniforme (unitÅÈ) sur les facettes
    for (j = 0; j < cs; j++) 
    for (i = 0; i < rs; i++) 
      if (K[j* rs + i] && CARRE(i,j))
	FUNC[j* rs + i] = (float)1;
      else
	FUNC[j* rs + i] = (float)0;
  }
  else if (mode == 9)
  { // fonction uniforme sur les facettes de la frontiÅËre
    struct xvimage * border = copyimage(k);
    uint8_t *B;
    assert(border != NULL);
    if (! l2dborder(border))
    {
      fprintf(stderr, "%s: function l2dborder failed\n", F_NAME);
      return 0;
    }
    B = UCHARDATA(border);
    for (j = 0; j < cs; j++) 
    for (i = 0; i < rs; i++) 
      if (B[j* rs + i] && INTER(i,j)) 
	FUNC[j* rs + i] = (float)1;
      else
	FUNC[j* rs + i] = (float)0;
    freeimage(border);
  }
  else
  {
    fprintf(stderr, "%s: bad mode: %d\n", F_NAME, mode);
    return 0;
  }

  for (i = 0; i < N; i++)
    flow->v_sommets[i] = (TYP_VSOM)FUNC[i];

#ifdef SHOWGRAPHS
  ShowGraphe(flow, "_flow2", 30, 1, 3, 0, 1, 0, 1, K, rs, head);
#endif

  // -----------------------------------------------------------
  // 3EME ETAPE : INTEGRE LA FONCTION DE POIDS 
  // -----------------------------------------------------------

  IntegreGSC(flow);

#ifdef SHOWGRAPHS
  ShowGraphe(flow, "_flow3", 30, 1, 3, 0, 1, 0, 1, K, rs, head);
#endif
#ifdef SHOWIMAGES
  for (i = 0; i < N; i++)
    FUNC[i] = (float)flow->v_sommets[i];
  writeimage(func, "_func");
#endif
    
  if (mode == 3)
  {
#ifdef ANCIEN1
    MaxAlpha3d(lambda); // fermeture (en ndg)
    for (i = 0; i < N; i++)
      flow->v_sommets[i] = flow->v_sommets[i] - (TYP_VSOM)LAMBDA[i];
#endif
#ifdef ANCIEN2
    int32_t *D = SLONGDATA(dist);
    for (i = 0; i < N; i++)
      if (D[i]) 
	flow->v_sommets[i] = flow->v_sommets[i] / (TYP_VSOM)sqrt(D[i]);
#endif
    int32_t ret;
    uint32_t *OF;
    struct xvimage *of = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    assert(of != NULL);
    ret = lopeningfunction(k, of, 0); assert(ret == 1);
    OF = ULONGDATA(of);
    for (i = 0; i < N; i++)
      if (OF[i]) 
	flow->v_sommets[i] = flow->v_sommets[i] / (TYP_VSOM)OF[i];
    freeimage(of);
  }

  // -----------------------------------------------------------
  // 4EME ETAPE : TRANSFORME LA FONCTION SUR LES SOMMETS EN 
  // FONCTION DE MORSE (INVERSÅ…E) SUR LE COMPLEXE
  // -----------------------------------------------------------  
  AlphaTopologicalMap(flow, head, alpha);

  // met Å‡ vmax (infini) les sommets "permanents" (non collapsÅÈs)
  vmax = flow->v_sommets[0];
  for (i = 0; i < N; i++)
    if (flow->v_sommets[i] > vmax) vmax = flow->v_sommets[i];
  for (i = 0; i < N; i++)
    if (perm[i])
      flow->v_sommets[i] = vmax;

#ifdef SHOWGRAPHS
  ShowGraphe(flow, "_flow4", 30, 1, 3, 0, 1, 0, 1, K, rs, head);
#endif
  
  if (level >= 0)
  {
    uint8_t *KK;
    graphe * flow_s;
    uint32_t j;

    for (i = 0; i < N; i++)
      K[i] = (flow->v_sommets[i] >= level ? 255 : 0);
    writeimage(k, "_seuil_level");

    // detects end points
    if (!l2dseltype(k, 0, 0, 0, 0, 1, 1))
    {
      fprintf(stderr, "%s: function l2dseltype failed\n", F_NAME);
      return 0;
    }
    writeimage(k, "_endpoints");    

    kk = copyimage(k); 
    assert(kk != NULL);
    razimage(kk);

    // foreach end point, compute the upstream
    KK     = UCHARDATA(kk);
    flow_s = Symetrique(flow);
    
    for (i = 0; i < N; i++)
      if (K[i])
      {
	boolean *D = Descendants(flow_s, i);
	for (j = 0; j < N; j++) if (D[j]) KK[j] = 255;
	free(D);
      }
    writeimage(kk, "_upstreams");    
    TermineGraphe(flow_s);

    // computes a new weighting function:
    // uniform on upstreams,
    // null elsewhere
    for (i = 0; i < N; i++)
      if (KK[i])
	flow->v_sommets[i] = (TYP_VSOM)1;
      else
	flow->v_sommets[i] = (TYP_VSOM)0;
    
    // integrates and "morsifies" this function
    IntegreGSC(flow);
    AlphaTopologicalMap(flow, head, alpha);
    vmax = flow->v_sommets[0];
    for (i = 0; i < N; i++)
      if (flow->v_sommets[i] > vmax) vmax = flow->v_sommets[i];
    for (i = 0; i < N; i++)
      if (perm[i])
	flow->v_sommets[i] = vmax;
  }

  for (i = 0; i < N; i++)
    FUNC[i] = (float)flow->v_sommets[i];

  freeimage(dist);
  freeimage(lambda);
  free(perm);
  free(head);
  TermineGraphe(flow);

  return 1;
} // l2dflowskeleton()
