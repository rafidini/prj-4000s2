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

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "mccodimage.h"
#include "mcimage.h"
#include "mcutil.h"
#include "mcgraphe.h"
#include "mcskelcurv.h"
#include "lskelcurv.h"
#include "lskel2graph.h"

double pink_round(double number)
{
#ifdef _WINDOWS
  return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
#else /* _WINDOWS */
  return round(number);
#endif /* _WINDOWS */
} // pink_round

double pink_trunc( double number )
{
#ifdef _WINDOWS
  return (((number) < 0) ? ceil((number)) : floor((number)));
#else /* _WINDOWS */
  return round(number);
#endif /* _WINDOWS */
} // pink_trunc


/* ====================================================================== */
graphe * 
lskel2graph( skel * S, int32_t mode, double param )
/* ====================================================================== */
{
#undef F_NAME
#define F_NAME "lskel2graph"

    graphe * g = NULL;

    if (mode == 0)
      g = lskel2graph0(S);
    else if (mode == 1)
      g = lskel2graph1(S);
    else if (mode == 2)
      g = lskel2graph2(S, param);
    else if (mode == 3)
      g = lskel2graph3(S, param);
    else
    {
      printf("bad mode");
      return NULL;
    }

    return g;
} // lskel2graph()

/* ====================================================================== */
graphe * lskel2graph0(skel * S)
/* ====================================================================== */
// Les sommets du graphe sont les points isolés,  les extrémités, les arcs et les jonctions.
// Pour les jonctions, et les arcs, on prend pour coordonnées le barycentre des points.
// Graphe symétrique.
{
#undef F_NAME
#define F_NAME "lskel2graph0"
  graphe * G;
  int32_t i, rs, ps, v, n;
  double x, y, z;
  int32_t nsom, ncurv;
  SKC_pt_pcell p;
  SKC_adj_pcell a;

  rs = S->rs;
  ps = rs * S->cs;
  ncurv = S->e_curv - S->e_end;
  nsom  = S->e_junc;

  G = InitGraphe(nsom, ncurv * 4);

  // pts isolés  
  for (i = 0; i < S->e_isol; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  }
  // pts extrémités  
  for (i = S->e_isol; i < S->e_end; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  }
  // pts de courbe
  for (i = S->e_end; i < S->e_curv; i++)
  {
    a = S->tskel[i].adj; 
    if (a != NULL)
    {
      v = a->val;
      AjouteArc(G, i, v);
      AjouteArc(G, v, i);

      a = a->next;
      assert(a != NULL);
      v = a->val;
      AjouteArc(G, i, v);
      AjouteArc(G, v, i);

      a = a->next;
      assert(a == NULL);

      x = y = z = 0.0;
      for (p = S->tskel[i].pts, n = 0; p != NULL; p = p->next, n++)
      {
	v = p->val;
	x += (double)(v % rs);
	y += (double)((v % ps) / rs);
	z += (double)(v / ps);
      }
      assert(n > 0);
      G->x[i] = x / n;
      G->y[i] = y / n;
      G->z[i] = z / n;    
    } // if (a != NULL)
    else //  arc ferme : un sommet isolé
    {
      p = S->tskel[i].pts;
      v = p->val;
      x = (double)(v % rs);
      y = (double)((v % ps) / rs);
      z = (double)(v / ps);
      G->x[i] = x;
      G->y[i] = y;
      G->z[i] = z;    
    }
  }
  // pts de jonction
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    x = y = z = 0.0;
    for (p = S->tskel[i].pts, n = 0; p != NULL; p = p->next, n++)
    {
      v = p->val;
      x += (double)(v % rs);
      y += (double)((v % ps) / rs);
      z += (double)(v / ps);
    }
    assert(n > 0);
    G->x[i] = x / n;
    G->y[i] = y / n;
    G->z[i] = z / n;
  }

  return G;
} // lskel2graph0()

/* ====================================================================== */
graphe * lskel2graph1(skel * S)
/* ====================================================================== */
// Les sommets du graphe sont les points isolés,  les extrémités et les jonctions.
// Pour les jonctions on prend pour coordonnées le barycentre des points.
// Graphe symétrique.
{
#undef F_NAME
#define F_NAME "lskel2graph1"
  graphe * G;
  int32_t i, rs, ps, v, v2, n;
  double x, y, z;
  int32_t nsom, ncurv;
  SKC_pt_pcell p;
  SKC_adj_pcell a;

  rs = S->rs;
  ps = rs * S->cs;
  ncurv = S->e_curv - S->e_end;
  nsom  = S->e_junc - ncurv;

  G = InitGraphe(nsom, ncurv * 2);

  // pts isolés  
  for (i = 0; i < S->e_isol; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  }
  // pts extrémités  
  for (i = S->e_isol; i < S->e_end; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  }
  // pts de courbe
  for (i = S->e_end; i < S->e_curv; i++)
  {
    a = S->tskel[i].adj; 
    if (a != NULL)
    {
      v = a->val;
      if (IS_JUNC(v)) v = v - ncurv;

      a = a->next;
      assert(a != NULL);
      v2 = a->val;
      if (IS_JUNC(v2)) v2 = v2 - ncurv;
      AjouteArc(G, v, v2);
      AjouteArc(G, v2, v);

      a = a->next;
      assert(a == NULL);
    } // if (a != NULL)
    // else: on ne fait rien
  }
  // pts de jonction
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    x = y = z = 0.0;
    for (p = S->tskel[i].pts, n = 0; p != NULL; p = p->next, n++)
    {
      v = p->val;
      x += (double)(v % rs);
      y += (double)((v % ps) / rs);
      z += (double)(v / ps);
    }
    assert(n > 0);
    G->x[i-ncurv] = x / n;
    G->y[i-ncurv] = y / n;
    G->z[i-ncurv] = z / n;
  }

  return G;
} // lskel2graph1()

/* ====================================================================== */
graphe * lskel2graph2(skel * S, double param)
/* ====================================================================== */
// Les sommets du graphe sont les points isolés, les extrémités, les jonctions, et des sommets résultant de la subdivision des courbes.
// Pour les jonctions on prend pour coordonnées le barycentre des points.
// Graphe symétrique.
// each curve of the skeleton (not including its ends) is represented by N edges and N-1 vertices of the graph, where N is set to param
{
#undef F_NAME
#define F_NAME "lskel2graph2"
  graphe * G;
  int32_t i, j, rs, ps, v, n, E, E1, E2, pt;
  double x, y, z, inter;
  int32_t nsom, nextrasom, newsom, newsom2, ncurv, len;
  int32_t Nd = (int32_t)pink_round(param);
  SKC_pt_pcell p, pp;
  SKC_adj_pcell a;

  assert(Nd >= 1);
  rs = S->rs;
  ps = rs * S->cs;
  ncurv = S->e_curv - S->e_end;
  nsom  = S->e_junc - ncurv;
  
  nextrasom = 0;
  for (i = S->e_end; i < S->e_curv; i++) // pour toutes les courbes
  {
    a = S->tskel[i].adj; // si arc fermé rajoute un sommet
    if (a == NULL) nextrasom += 1;
  } // for (i = S->e_end; i < S->e_curv; i++)

  G = InitGraphe(nsom + nextrasom + ((Nd-1)*ncurv), ncurv * Nd * 2);

  // pts isolés  
  for (i = 0; i < S->e_isol; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  } // for (i = 0; i < S->e_isol; i++)
  // pts extrémités  
  for (i = S->e_isol; i < S->e_end; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  } // for (i = S->e_isol; i < S->e_end; i++)
  // pts de courbe
  for (i = S->e_end; i < S->e_curv; i++)
  {    
    // parcourt la liste des points pour trouver les emplacements des sommets
    // et ajouter les arcs
    a = S->tskel[i].adj;
    if (a != NULL) // si arc non fermé
    {
      assert(a->next != NULL); // soit 0, soit 2 adjacences
      assert(a->next->next == NULL);
      E1 = a->val; // Let E1, E2 be the vertices adjacent to ind
      E2 = a->next->val;
      pt = point_at_head(S, i);
      if (IS_JUNC(E1))
      {
	if (!adj_point_junc(S, pt, E1)) { E = E1; E1 = E2; E2 = E; }
      }
      else
      {
	assert(IS_END(E1));
	p = S->tskel[E1].pts;
	if (p->val != pt) { E = E1; E1 = E2; E2 = E; }
      }      

      // trouve la longueur de la courbe
      for (len = 0, p = S->tskel[i].pts; p != NULL; p = p->next) len++;
      if (IS_END(E1)) len--; // les points extrémités sont dans la courbe,
      if (IS_END(E2)) len--; // les points de jonction non
      // trouve le nombre de points moyen entre 2 sommets
      inter = ((double)len) / Nd;

      if (IS_JUNC(E1)) E1 = E1 - ncurv;
      if (IS_JUNC(E2)) E2 = E2 - ncurv;
      if (Nd == 1)
      {
	AjouteArc(G, E1, E2);
	AjouteArc(G, E2, E1);
      }
      else // if (Nd == 1)
      {
	// attribue un index au premier nouveau sommet 
	newsom = nsom;
	nsom++;
	// recherche le point de la courbe correspondant à ce sommet
	for (j = 0, p = S->tskel[i].pts; p != NULL; p = p->next)
	{
	  j += 1;
	  if (j > inter) { pp = p; p = p->next; break; }
	}
	v = pp->val;
	x = (double)(v % rs);
	y = (double)((v % ps) / rs);
	z = (double)(v / ps);
	G->x[newsom] = x;
	G->y[newsom] = y;
	G->z[newsom] = z;
	AjouteArc(G, newsom, E1);
	AjouteArc(G, E1, newsom);
	// pour les sommets suivants
	for (n = 1; n < Nd-1; n++)
        { 
	  newsom2 = newsom;
	  // attribue un index au nouveau sommet 
	  newsom = nsom;
	  nsom++;
	  for (j = 0; p != NULL; p = p->next)
	  {
	    j += 1;
	    if (j > inter) { pp = p; p = p->next; break; }
	  }
	  v = pp->val;
	  x = (double)(v % rs);
	  y = (double)((v % ps) / rs);
	  z = (double)(v / ps);
	  G->x[newsom] = x;
	  G->y[newsom] = y;
	  G->z[newsom] = z;
	  AjouteArc(G, newsom, newsom2);
	  AjouteArc(G, newsom2, newsom);
        } // for (n = 1; n < nextrasom-1; n++)
	AjouteArc(G, newsom, E2);
	AjouteArc(G, E2, newsom);
      } // else [if (Nd == 1)]
    } // if (a != NULL)
    else
    // a == NULL, arc fermé
    {
      // ajoute un sommet
      newsom = nsom;
      nsom++;
      p = S->tskel[i].pts;
      v = p->val;
      x = (double)(v % rs);
      y = (double)((v % ps) / rs);
      z = (double)(v / ps);
      G->x[newsom] = x;
      G->y[newsom] = y;
      G->z[newsom] = z;
      // trouve la longueur de la courbe
      for (len = 0, p = S->tskel[i].pts; p != NULL; p = p->next) len++;
      // trouve le nombre de points moyen entre 2 sommets
      inter = ((double)len) / Nd;
      E1 = newsom;
      if (Nd == 1)
      {
	AjouteArc(G, E1, E1);
      }
      else
      {
	// attribue un index au premier nouveau sommet 
	newsom = nsom;
	nsom++;
	// recherche le point de la courbe correspondant à ce sommet
	for (j = 0, p = S->tskel[i].pts; p != NULL; p = p->next)
	{
	  j += 1;
	  if (j > inter) { pp = p; p = p->next; break; }
	}
	v = pp->val;
	x = (double)(v % rs);
	y = (double)((v % ps) / rs);
	z = (double)(v / ps);
	G->x[newsom] = x;
	G->y[newsom] = y;
	G->z[newsom] = z;
	AjouteArc(G, newsom, E1);
	AjouteArc(G, E1, newsom);
	// pour les sommets suivants
	for (n = 1; n < Nd-1; n++)
        { 
	  newsom2 = newsom;
	  // attribue un index au nouveau sommet 
	  newsom = nsom;
	  nsom++;
	  for (j = 0; p != NULL; p = p->next)
	  {
	    j += 1;
	    if (j > inter) { pp = p; p = p->next; break; }
	  }
	  v = pp->val;
	  x = (double)(v % rs);
	  y = (double)((v % ps) / rs);
	  z = (double)(v / ps);
	  G->x[newsom] = x;
	  G->y[newsom] = y;
	  G->z[newsom] = z;
	  AjouteArc(G, newsom, newsom2);
	  AjouteArc(G, newsom2, newsom);
        } // for (n = 1; n < nextrasom-1; n++)
	if (Nd > 2)
	{
	  AjouteArc(G, newsom, E1);
	  AjouteArc(G, E1, newsom);
	}
      } // else [if (nextrasom == 0)]
    } // else [if (a != NULL)]
  } // for (i = S->e_end; i < S->e_curv; i++)
  // pts de jonction
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    x = y = z = 0.0;
    for (p = S->tskel[i].pts, n = 0; p != NULL; p = p->next, n++)
    {
      v = p->val;
      x += (double)(v % rs);
      y += (double)((v % ps) / rs);
      z += (double)(v / ps);
    }
    assert(n > 0);
    G->x[i-ncurv] = x / n;
    G->y[i-ncurv] = y / n;
    G->z[i-ncurv] = z / n;
  } // for (i = S->e_curv; i < S->e_junc; i++)

  return G;
} // lskel2graph2()

/* ====================================================================== */
graphe * lskel2graph3(skel * S, double param)
/* ====================================================================== */
// Les sommets du graphe sont les points isolés, les extrémités, les jonctions, et des sommets résultant de la subdivision des courbes.
// Pour les jonctions on prend pour coordonnées le barycentre des points.
// Graphe symétrique.
// each curve of the skeleton (not including its end) is represented by N edges and N-1 vertices of the graph, where N is set to max(1, trunc(len / \b param)), where len is the number of points of the curve.
{
#undef F_NAME
#define F_NAME "lskel2graph3"
  graphe * G;
  int32_t i, j, rs, ps, v, n, E, E1, E2, pt;
  double x, y, z, inter;
  int32_t nsom, newsom, newsom2, nextrasom, ncurv, len;
  SKC_pt_pcell p, pp;
  SKC_adj_pcell a;

  rs = S->rs;
  ps = rs * S->cs;
  ncurv = S->e_curv - S->e_end;
  nsom  = S->e_junc - ncurv;
  nextrasom = 0;

  // pour toutes les courbes
  for (i = S->e_end; i < S->e_curv; i++)
  {
    // trouve la longueur de la courbe
    for (len = 0, p = S->tskel[i].pts; p != NULL; p = p->next) len++;
    nextrasom += mcmax(1, (int32_t)pink_trunc((double)len / param)) - 1;
    a = S->tskel[i].adj; // si arc fermé rajoute un sommet
    if (a == NULL) nextrasom += 1;
  } // for (i = S->e_end; i < S->e_curv; i++)
  
  G = InitGraphe(nsom+nextrasom, (ncurv+nextrasom) * 2);

  // pts isolés  
  for (i = 0; i < S->e_isol; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  } // for (i = 0; i < S->e_isol; i++)
  // pts extrémités  
  for (i = S->e_isol; i < S->e_end; i++)
  {
    p = S->tskel[i].pts;
    assert(p != NULL);
    v = p->val;
    G->x[i] = (double)(v % rs);
    G->y[i] = (double)((v % ps) / rs);
    G->z[i] = (double)(v / ps);
    assert(p->next == NULL);
  } // for (i = S->e_isol; i < S->e_end; i++)
  // pts de courbe
  for (i = S->e_end; i < S->e_curv; i++)
  {    
    // parcourt la liste des points pour trouver les emplacements des sommets
    // et ajouter les arcs
    a = S->tskel[i].adj;
    if (a != NULL) // si arc non fermé
    {
      assert(a->next != NULL); // soit 0, soit 2 adjacences
      assert(a->next->next == NULL);
      E1 = a->val; // Let E1, E2 be the vertices adjacent to ind
      E2 = a->next->val;
      pt = point_at_head(S, i);
      if (IS_JUNC(E1))
      {
	if (!adj_point_junc(S, pt, E1)) { E = E1; E1 = E2; E2 = E; }
      }
      else
      {
	assert(IS_END(E1));
	p = S->tskel[E1].pts;
	if (p->val != pt) { E = E1; E1 = E2; E2 = E; }
      }      

      // trouve la longueur de la courbe
      for (len = 0, p = S->tskel[i].pts; p != NULL; p = p->next) len++;
      if (IS_END(E1)) len--; // les points extrémités sont dans la courbe,
      if (IS_END(E2)) len--; // les points de jonction non
      // trouve le nombre de sommets intermédiaires
      nextrasom = mcmax(1, (int32_t)pink_trunc((double)len / param)) - 1;
      // trouve le nombre de points moyen entre 2 sommets
      inter = ((double)len) / (nextrasom+1);
//printf("len = %d nextrasom = %d inter = %g\n", len, nextrasom, inter);

      if (IS_JUNC(E1)) E1 = E1 - ncurv;
      if (IS_JUNC(E2)) E2 = E2 - ncurv;
      if (nextrasom == 0)
      {
	AjouteArc(G, E1, E2);
	AjouteArc(G, E2, E1);
      }
      else
      {
	// attribue un index au premier nouveau sommet 
	newsom = nsom;
	nsom++;
	// recherche le point de la courbe correspondant à ce sommet
	for (j = 0, p = S->tskel[i].pts; p != NULL; p = p->next)
	{
	  j += 1;
	  if (j > inter) { pp = p; p = p->next; break; }
	}
	v = pp->val;
	x = (double)(v % rs);
	y = (double)((v % ps) / rs);
	z = (double)(v / ps);
	G->x[newsom] = x;
	G->y[newsom] = y;
	G->z[newsom] = z;
	AjouteArc(G, newsom, E1);
	AjouteArc(G, E1, newsom);
	// pour les sommets suivants
	for (n = 1; n < nextrasom; n++)
        { 
	  newsom2 = newsom;
	  // attribue un index au nouveau sommet 
	  newsom = nsom;
	  nsom++;
	  for (j = 0; p != NULL; p = p->next)
	  {
	    j += 1;
	    if (j > inter) { pp = p; p = p->next; break; }
	  }
	  v = pp->val;
	  x = (double)(v % rs);
	  y = (double)((v % ps) / rs);
	  z = (double)(v / ps);
	  G->x[newsom] = x;
	  G->y[newsom] = y;
	  G->z[newsom] = z;
	  AjouteArc(G, newsom, newsom2);
	  AjouteArc(G, newsom2, newsom);
        } // for (n = 1; n < nextrasom; n++)
	AjouteArc(G, newsom, E2);
	AjouteArc(G, E2, newsom);
      } // else [if (nextrasom == 0)]
    } // if (a != NULL)
    else // arc ferme
    {
      newsom = nsom;
      nsom++;
      p = S->tskel[i].pts;
      v = p->val;
      x = (double)(v % rs);
      y = (double)((v % ps) / rs);
      z = (double)(v / ps);
      G->x[newsom] = x;
      G->y[newsom] = y;
      G->z[newsom] = z;
      // trouve la longueur de la courbe
      for (len = 0, p = S->tskel[i].pts; p != NULL; p = p->next) len++;
      // trouve le nombre de sommets intermédiaires
      nextrasom = mcmax(1, (int32_t)pink_trunc((double)len / param)) - 1;
      // trouve le nombre de points moyen entre 2 sommets
      inter = ((double)len) / (nextrasom+1);

      E1 = newsom;
      if (nextrasom == 0)
      {
	AjouteArc(G, E1, E1);
      }
      else
      {
	// attribue un index au premier nouveau sommet 
	newsom = nsom;
	nsom++;
	// recherche le point de la courbe correspondant à ce sommet
	for (j = 0, p = S->tskel[i].pts; p != NULL; p = p->next)
	{
	  j += 1;
	  if (j > inter) { pp = p; p = p->next; break; }
	}
	v = pp->val;
	x = (double)(v % rs);
	y = (double)((v % ps) / rs);
	z = (double)(v / ps);
	G->x[newsom] = x;
	G->y[newsom] = y;
	G->z[newsom] = z;
	AjouteArc(G, newsom, E1);
	AjouteArc(G, E1, newsom);
	// pour les sommets suivants
	for (n = 1; n < nextrasom; n++)
        { 
	  newsom2 = newsom;
	  // attribue un index au nouveau sommet 
	  newsom = nsom;
	  nsom++;
	  for (j = 0; p != NULL; p = p->next)
	  {
	    j += 1;
	    if (j > inter) { pp = p; p = p->next; break; }
	  }
	  v = pp->val;
	  x = (double)(v % rs);
	  y = (double)((v % ps) / rs);
	  z = (double)(v / ps);
	  G->x[newsom] = x;
	  G->y[newsom] = y;
	  G->z[newsom] = z;
	  AjouteArc(G, newsom, newsom2);
	  AjouteArc(G, newsom2, newsom);
        } // for (n = 1; n < nextrasom; n++)
	if (nextrasom > 1)
	{
	  AjouteArc(G, newsom, E1);
	  AjouteArc(G, E1, newsom);
	}
      } // else [if (nextrasom == 0)]
    } // else [if (a != NULL)]
  } // for (i = S->e_end; i < S->e_curv; i++)
  // pts de jonction
  for (i = S->e_curv; i < S->e_junc; i++)
  {
    x = y = z = 0.0;
    for (p = S->tskel[i].pts, n = 0; p != NULL; p = p->next, n++)
    {
      v = p->val;
      x += (double)(v % rs);
      y += (double)((v % ps) / rs);
      z += (double)(v / ps);
    }
    assert(n > 0);
    G->x[i-ncurv] = x / n;
    G->y[i-ncurv] = y / n;
    G->z[i-ncurv] = z / n;
  } // for (i = S->e_curv; i < S->e_junc; i++)

  return G;
} // lskel2graph3()

