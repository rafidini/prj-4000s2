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
      Librairie mcgraphe :

      Gestion de graphes de regions

      Michel Couprie, aout 1997

      Principe du codage :
        - a chaque sommet (region) est associe une liste de codes d'aretes
        - les regions "atomiques" sont les pixels de l'image originale
        - dans une image a N pixels, pour un pixel i, on code par:
            i     l'arete liant le pixel i a son voisin est i+1,
            N+i   l'arete liant le pixel i a son voisin sud,
          et de plus, pour le 8-voisinage:
            2N+i  l'arete liant le pixel i a son voisin sud-est,
            3N+i  l'arete liant le pixel i a son voisin sud-ouest.
        - les regions sont obtenues par fusion de regions (voir mcfus).
*/
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <mcsort.h>
#include <mcgraphe.h>

#define DEBUGGRAPHE

/* ================================================ */
/* ================================================ */
/* fonctions privees */
/* ================================================ */
/* ================================================ */

/* ========================================== */
static void erreur(char * mess)
/* ========================================== */
{
  fprintf(stderr, "%s\n", mess);
  exit(0);
} /* erreur() */

#ifdef DEBUGGRAPHE
/* ====================================================================== */
void printliste(pcell p)
/* ====================================================================== */
{
  for (; p != NULL; p = p->next)
    printf("%d ", p->val);
  printf("\n");
} /* printliste() */

/* ====================================================================== */
void printgraphe(graphe * g)
/* ====================================================================== */
{
  uint32_t i;
  
  for (i = 0; i < g->nbsom; i++)
  {
    printf("[%d] ", i);
    printliste(*(g->listar+i));
  }
  printf("\n");
} /* printgraphe() */
#endif

/* ================================================ */
/* ================================================ */
/* fonctions publiques */
/* ================================================ */
/* ================================================ */

/* ====================================================================== */
graphe * initgraphe(uint32_t nbsom, uint32_t nbmaxar)
/* ====================================================================== */
{
  graphe * g;
  uint32_t i;
  
  g = (graphe *)calloc(1,sizeof(graphe));
  if (g == NULL)
  {   fprintf(stderr, "initgraph : malloc failed for g\n");
      return(0);
  }
  g->tasar = (cell *)calloc(1,nbmaxar * sizeof(cell));
  if (g->tasar == NULL)
  {   fprintf(stderr, "initgraph : malloc failed for g->tasar\n");
      return(0);
  }
  g->listar = (pcell *)calloc(nbsom, sizeof(pcell));
  if (g->listar == NULL)
  {   fprintf(stderr, "initgraph : calloc failed for g->listar\n");
      return(0);
  }

  g->nbsom = nbsom;
  g->nbmaxar = nbmaxar;
  g->nbar = 0;
  for (i = 0; i < nbmaxar - 1; i++)
    (g->tasar+i)->next = g->tasar+i+1;
  (g->tasar+i)->next = NULL;
  g->librear = g->tasar;  
  
  return g;
} /* initgraphe() */

/* ====================================================================== */
void terminegraphe(graphe * g)
/* ====================================================================== */
{
  free(g->tasar);
  free(g->listar);
  free(g);
} /* terminegraphe() */

/* ====================================================================== */
pcell allouecell(graphe * g)
/* ====================================================================== */
{
  pcell p;
  if (g->librear == NULL) erreur("allouecell : plus de cellules libres");
  p = g->librear;
  g->librear = g->librear->next;
  return p;
} /* allouecell() */

/* ====================================================================== */
void liberecell(graphe * g, pcell p)
/* ====================================================================== */
{
  p->next = g->librear;
  g->librear = p;
} /* liberecell() */

/* ====================================================================== */
void retiretete(graphe * g, pcell * pliste)
/* ====================================================================== */
{
  pcell p;
  p = (*pliste)->next;
  liberecell(g, *pliste);
  *pliste = p;
} /* retiretete() */

/* ====================================================================== */
int32_t estarete(graphe * g, uint32_t som, uint32_t a)
/* ====================================================================== */
{
  pcell liste;
  liste = *(g->listar + som);
  while (liste != NULL)
    if (liste->val == a) return 1;
    else liste = liste->next;
  return 0;
} /* estarete() */

/* ====================================================================== */
void retirearete(graphe * g, uint32_t som, uint32_t a)
/* ====================================================================== */
{
  pcell * pliste;
  pliste = g->listar + som;
  while ((*pliste != NULL) && (((*pliste)->val) != a))
    pliste = &((*pliste)->next);
  if (*pliste != NULL) retiretete(g, pliste);
  g->nbar--;
} /* retirearete() */

/* ====================================================================== */
void ajoutearete(graphe * g, uint32_t som, uint32_t a)
/* ====================================================================== */
{
  pcell p;
  p = allouecell(g);
  p->next = *(g->listar + som);
  p->val = a;
  *(g->listar + som) = p;
  g->nbar++;
} /* ajoutearete() */

/* ====================================================================== */
void maille4graphe(graphe * g, uint32_t rs, uint32_t cs)
/* ====================================================================== */
{
  uint32_t i, j;
  uint32_t N = rs * cs;
  
  for (j = 0; j < cs; j++)
    for (i = 0; i < rs; i++)
    {
      if (i < rs - 1)
      {
        ajoutearete(g, j * rs + i, j * rs + i);
        ajoutearete(g, j * rs + i + 1, j * rs + i);
      }
      if (j < cs - 1)
      {
        ajoutearete(g, j * rs + i, j * rs + i + N);
        ajoutearete(g, (j+1) * rs + i, j * rs + i + N);
      }
    }
} /* maille4graphe() */

/* ====================================================================== */
void nettoiegraphe(graphe * g)
/* ====================================================================== */
/* retire les aretes dupliquees dans les listes d'aretes */
{
  int32_t nbsom = g->nbsom;
  int32_t nbar = g->nbar;
  int32_t *tmpar;
  int32_t s, a;
  pcell * pliste;

  /* alloue la place pour le tableau d'aretes */
  tmpar = (int32_t *)calloc(1,nbar * sizeof(int32_t));
  if (tmpar == NULL)
  {   fprintf(stderr, "nettoiegraphe : malloc failed for tmpar\n");
      exit(0);
  }
  
  for (s = 0; s < nbsom; s++)
  {
    /* transfere la liste d'aretes adjacentes au sommet s dans le tableau tmpar */
    nbar = 0;
    pliste = g->listar + s;
    while (*pliste != NULL)
    {
      tmpar[nbar] = (*pliste)->val;
      nbar++;
      retiretete(g, pliste);
      g->nbar--;      
    }

    /* tri de tmpar et elimination des dupliques */
    TriRapideStochastique (tmpar, 0, nbar-1);
    nbar = ElimineDupliques(tmpar, nbar);

    /* reconstruction de la liste des aretes adjacentes a sommet s */
    for (a = 0; a < nbar; a++) ajoutearete(g, s, tmpar[a]);
  } /* for (s = 0; s < nbsom; s++) */

  free(tmpar);

} /* nettoiegraphe() */

/* ====================================================================== */
void aretesommets(uint32_t a, uint32_t N, uint32_t rs, uint32_t * s1, uint32_t * s2)
/* ====================================================================== */
{
  if (a < N)          { *s1 = a; *s2 = a + 1; }  
  else if (a < 2 * N) { *s1 = a - N; *s2 = a - N + rs; }  
  else if (a < 3 * N) { *s1 = a - 2*N; *s2 = a - 2*N + rs + 1; }  
  else                { *s1 = a - 3*N; *s2 = a - 3*N + rs - 1; }  
} /* aretesommets() */

/* ================================================ */
/* ================================================ */
/* fonctions publiques pour la vision 'liste d'adjacence' */
/* (a chaque sommet est associee la liste des sommets adjacents) */
/* ================================================ */
/* ================================================ */

/* ====================================================================== */
int32_t estsuccesseur(graphe * g, uint32_t som, uint32_t a)
/* ====================================================================== */
{ /* idem estarete, mais c'est la vision 'liste d'adjacence' */
  return estarete(g, som, a);
} /* estsuccesseur() */

/* ====================================================================== */
int32_t estsymetrique(graphe * g)
/* ====================================================================== */
{
  uint32_t i, j;
  
  for (i = 0; i < g->nbsom - 1; i++)
    for (j = i+1; j < g->nbsom; j++)
    {
      if (estsuccesseur(g, i, j))
      {
        if (!estsuccesseur(g, j, i)) return 0;
      }
      else
      {
        if (estsuccesseur(g, j, i)) return 0;
      }
    }
  return 1;
} /* estsymetrique() */

#ifdef TESTGRAPHE
void main()
{
  graphe * g;
  uint32_t s1, s2;
  
  g = initgraphe(16, 64);
  maille4graphe(g, 4, 4);
  printgraphe(g);
  nettoiegraphe(g);
  printgraphe(g);
  ajoutearete(g,12,24);
  ajoutearete(g,12,12);
  ajoutearete(g,12,24);
  ajoutearete(g,15,27);
  ajoutearete(g,15,14);
  ajoutearete(g,15,27);
  printgraphe(g);
  nettoiegraphe(g);
  printgraphe(g);
  aretesommets(0, 16, 4, &s1, &s2);
  printf("%d %d\n", (int32_t)s1, (int32_t)s2);
  aretesommets(1, 16, 4, &s1, &s2);
  printf("%d %d\n", (int32_t)s1, (int32_t)s2);
  aretesommets(21, 16, 4, &s1, &s2);
  printf("%d %d\n", (int32_t)s1, (int32_t)s2);
  printf("estarete 0 16 -> %d\n", estarete(g, 0, 16));
  printf("estarete 0 2 -> %d\n", estarete(g, 0, 2));
  terminegraphe(g);
} /* main() */
#endif
