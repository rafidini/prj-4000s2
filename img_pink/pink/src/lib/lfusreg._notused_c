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
/****************************************************************
*
* Routine Name: lfusreg - library call for fusreg
*
* Purpose: fusion de plateaux a partir de marqueurs
*
* Input:
* Output:
* Written By: Michel Couprie - aout 1997
*
* Revision juillet 1999: marqueurs non ponctuels
*
****************************************************************/

/*
Version 0.1 du 25/08/97
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcfusion.h>
#include <mcfahdouble.h>
#include <mcgraphe.h>
#include <lfusreg.h>
#include <mcutil.h>
#include <mcindic.h>
#ifdef PERF
#include <mcchrono.h>
#endif

#define MARQUE     0

/*
#define DEBUG
#define VERBOSE
*/


#define PARANO

/* ========================================== */
static void erreur(char * mess)
/* ========================================== */
{
  fprintf(stderr, "%s\n", mess);
  exit(0);
} /* erreur() */

/* ==================================== */
int32_t lfusreg(
  struct xvimage *img,    /* image en niveaux de gris */
  struct xvimage *mrq,    /* marqueurs (image d'etiquettes) */
  struct xvimage *result, /* resultat : etiquettes sur 4 octets */
  int32_t memoire
)
/* ==================================== */
{
  uint8_t *F = UCHARDATA(img);
  uint32_t *ML = SLONGDATA(mrq);
  uint32_t *R = SLONGDATA(result);
  uint32_t rs, cs, N;
  Fahd * FAHD;
  Fus * FUS;
  graphe * g;
  pcell p;
  int32_t i, j, k, l, x, y, z, w;
  uint32_t ml;
  int32_t maxmrq;
  int32_t label, labmrq, labnm, lab, nblab;
  int32_t *tabr;
  uint8_t *tmrq;         /* booleen: represente une region marqueur */

  /* ---------------------------------------------------------- */
  /* recupere et verifie les operandes */
  /* ---------------------------------------------------------- */

  if (depth(img) != 1) 
  {
    fprintf(stderr, "lfusreg: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  rs = rowsize(img);
  cs = colsize(img);
  N = rs * cs;
  if ((rowsize(mrq) != rs) || (colsize(mrq) != cs))
  {
    fprintf(stderr, "lfusreg: incompatible image sizes\n");
    return 0;
  }

  IndicsInit(N);

  if (datatype(mrq) != VFF_TYP_4_BYTE)
  { 
    fprintf(stderr, "lfusreg: marker image datatype must be longint\n");
    return 0;
  }

  FAHD = CreeFahdVide(2 * N);
  if (FAHD == NULL)
  {   fprintf(stderr, "lfusreg() : CreeFahd failed\n");
      return(0);
  }

  /* recherche l'etiquette max */
  maxmrq = ML[0];
  for (x = 0; x < N; x++) if (ML[x] > maxmrq) maxmrq = ML[x];
  label = maxmrq;

  /* etiquette les plateaux non marques avec des etiquettes > maxmrq */  
  for (x = 0; x < N; x++)
  {
    if (ML[x] == 0)
    {
      label += 1;
      ML[x] = label;
      FahdPush(FAHD, x, 0, 0); /* la fah est utilisee en simple pile */
      while (! FahdVide(FAHD))
      {
        FahdPop(FAHD, &z, &lab);
        for (k = 0; k < 8; k += 2) /* parcourt les voisins en 4-connexite */
        {
          y = voisin(z, k, rs, N);
          if ((y != -1) && (ML[y] == 0) && (F[y] == F[z]))
          {
            ML[y] = label;
            FahdPush(FAHD, y, 0, 0);
          } /* if y ... */
        } /* for k ... */
      } /* while (! FahdVide(FAHD)) */
    } /* if (ML[x] == 0) */
  } /* for (x = 0; x < N; x++) */
  nblab = label + 1;

  FUS = CreeFus(nblab);
  if (FUS == NULL)
  {   fprintf(stderr, "lfusreg() : CreeFus failed\n");
      return(0);
  }
  
  g = initgraphe(nblab, nblab * 10); /* ATTENTION: le facteur 10 n'est pas theoriquement justifie */
  if (g == NULL)
  {   fprintf(stderr, "lfusreg() : initgraph failed\n");
      return(0);
  }

#ifdef VERBOSE
printf("PLATEAUX ETIQUETES\n");
#endif
#ifdef DEBUG
printf("maxmrq = %d, nblab = %d\n", maxmrq, nblab);
for (j = 0; j < cs; j++)
{
  for (i = 0; i < rs; i++)
  {
    x = j * rs + i;
    printf("%3d ", ML[x]);
  }
  printf("\n");
}
printf("\n");
#endif

  /* 
     Construit le graphe initial des regions marquees et des plateaux

     On adoptera la representation par listes d'adjacence. 
     Les sommets du graphe sont les etiquettes (ML) des regions.
     La table 'tabr' associe, a chaque label, un pixel de la region.

     pour tous les x parcourus dans l'ordre raster
       si x non marque faire          // x est le representant d'une nouvelle region
         label = ML[x]
         marque x
         tabr[label] = x
         pour tous les y non marques tq ML[y] == label faire
           marque y
           pour tous les z 4-voisins de y tq ML[z] != label
           ajoutearete(label,ML[z])
         finpour
      finsi             
    finpour
  */

  tabr = (int32_t *)calloc(1,nblab * sizeof(int32_t));
  if (tabr == NULL)
  {   fprintf(stderr,"lfusreg() : malloc failed for tabr\n");
      return 0;
  }

  tmrq = (uint8_t *)calloc(nblab, sizeof(char));
  if (tmrq == NULL)
  {   fprintf(stderr,"lfusreg() : malloc failed for tmrq\n");
      return 0;
  }
  for (lab = 1; lab <= maxmrq; lab++) tmrq[lab] = 1;

  for (x = 0; x < N; x++)
  {
    if (!IsSet(x,MARQUE)) /* x est le representant d'une nouvelle region */
    {
      label = ML[x];
      Set(x,MARQUE);
      tabr[label] = x;
      FahdPush(FAHD, x, 0, 0);
      while (! FahdVide(FAHD)) /* la fah est utilisee en simple pile */
      {
        FahdPop(FAHD, &w, &lab);        
        for (k = 0; k < 8; k += 2) /* parcourt les voisins en 4-connexite */
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (ML[y] == label) && (!IsSet(y,MARQUE)))
          {
            Set(y,MARQUE);
            FahdPush(FAHD, y, 0, 0);
          } /* if y ... */
          else if ((y != -1) && (ML[y] != label))
          {
            ajoutearete(g, label, ML[y]);
          }
        } /* for k ... */
      } /* while (! FahdVide(FAHD)) */
    } /* if (!IsSet(x,MARQUE)) */
  } /* for (x = 0; x < N; x++) */

  /* nettoie le graphe pour eliminer les aretes dupliquees */
  nettoiegraphe(g);

#ifdef VERBOSE
printf("GRAPHE INITIALISE\n");
#endif
#ifdef DEBUG
printgraphe(g);
#endif

  /* 
     Initialise la FAHD en vue de la propagation des regions initialement marquees
     Les regions initialement marquees (labmrq) sont indiquees par le booleen tmrq.
     Pour chaque labmrq, on parcourt la liste des sommets voisins (dans g) qui ne sont pas 
       inialement marques labnm et on empile dans la fahd les couples
       labmrq,labnm au niveau : abs(F[tabr[labmrq]] - F[tabr[labnm]]).
  */

  for (labmrq = 1; labmrq <= maxmrq; labmrq++)
  {
    for (p = *(g->listar + labmrq); p != NULL; p = p->next)
    {
      labnm = p->val;          
      if (labnm > maxmrq)
      {
        FahdPush(FAHD, labmrq, labnm, abs(F[tabr[labmrq]] - F[tabr[labnm]]));
#ifdef DEBUG
printf("PUSH init %d,%d (niveau %d) \n", labmrq, labnm, abs(F[tabr[labmrq]] - F[tabr[labnm]]));
#endif
      } /* if (labnm > maxmrq) */
    } /* for (p = *(g->listar + labmrq); p != NULL; p = p->next) */
  } /* for (labmrq = 1; labmrq <= maxmrq; labmrq++) */
#ifdef VERBOSE
printf("INITIALISATION FAHD TERMINEE\n");
#endif

  /* 
     Boucle principale : 
       - retire l'arete de plus forte priorite de la FAHD
       - si cette arete separe une region marquee d'une region non marquee, opere la fusion
  */
  while (!FahdVide(FAHD))
  {
    FahdPop(FAHD, &lab, &label);

#ifdef DEBUG
printf("pop %d %d\n", lab, label);
#endif

    lab = FusF(FUS, lab);
    label = FusF(FUS, label);

#ifdef DEBUG
printf("representants %d %d\n", lab, label);
#endif

    if (tmrq[lab])
    {
      if (!tmrq[label]) { labmrq = lab; labnm = label; }
      else goto finboucle;  /* deux regions marquees : rien a faire */
    }
    else   /* !tmrq[lab] */
    {
      if (tmrq[label]) { labmrq = label; labnm = lab; }
      else erreur("fusreg : deux regions non marquees !");
    }
#ifdef DEBUG
printf("reg. marquee %d, non marquee %d \n", labmrq, labnm);
#endif
  
    retirearete(g, labmrq, labnm);
    retirearete(g, labnm, labmrq);
    for (p = *(g->listar + labnm); p != NULL; p = p->next)
    {  /* pour tous les sommets voisins du sommet non marque */
      lab = p->val;
      lab = FusF(FUS, lab);
      if (!tmrq[lab]) 
      {
        ajoutearete(g, labmrq, lab);
        if (memoire)
        {      /* c'est l'ancien qui restera le representant de la fusion */
          FahdPush(FAHD, labmrq, lab, abs(F[tabr[labmrq]] - F[tabr[lab]]));
#ifdef DEBUG
printf("PUSH %d,%d (niveau %d) \n", labmrq, lab, abs(F[tabr[labmrq]] - F[tabr[lab]]));
#endif
        }
        else
	{    /* c'est le nouveau qui sera le representant de la fusion */
          FahdPush(FAHD, labnm, lab, abs(F[tabr[labnm]] - F[tabr[lab]]));
#ifdef DEBUG
printf("PUSH %d,%d (niveau %d) \n", labnm, lab, abs(F[tabr[labnm]] - F[tabr[lab]]));
#endif
        }
      } /* if (!tmrq[lab]) */
    } /* for (p = *(g->listar + labnm); p != NULL; p = p->next) */
  
    if (memoire)
    {
      Fusion1rep(FUS, labmrq, labnm); /* le representant ne change pas */
#ifdef DEBUG
printf("Fusion1rep %d, %d \n", labmrq, labnm);
#endif
    }
    else
    {
#ifdef DEBUG
printf("Fusion1rep %d, %d \n", labnm, labmrq);
#endif
      Fusion1rep(FUS, labnm, labmrq); /* le nouveau plateau fusionne' devient */
      tmrq[labnm] = 1;                 /* le representant */
    }

finboucle:;

  } /* while (!FahdVide(FAHD)) */

#ifdef VERBOSE
printf("BOUCLE PRINCIPALE TERMINEE\n");
#endif

  (void)FusNormalise(FUS);
  for (x = 0; x < N; x++) R[x] = FusF(FUS, ML[x]);

#ifdef VERBOSE
printf("RECUPERATION RESULTAT TERMINEE\n");
#endif

  FusTermine(FUS);
  FahdTermine(FAHD);
  terminegraphe(g);
  IndicsTermine();
  free(tabr);
  free(tmrq);
  return(1);
}
/* -library_code_end */


