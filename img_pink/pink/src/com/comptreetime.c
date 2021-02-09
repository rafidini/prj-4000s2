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
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

#include <mcchrono.h>


//#define IMGLONG
#define IMGCHAR

//#define _SALEMBIER_
//#define _NC_

#ifdef _NC_
#include <mcunionfind.h>
#include <mccomptree.h>
#endif


#ifdef _SALEMBIER_
#define ATTR_SURF
#include <mclifo.h>
#include <mcfahsalembier.h>
#include <ldynamique.h>

#include "../lib/lattrib.c"


uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                              /* en sortie, contient le numero de la composante de niveau h */
                              /* qui contient le pixel (avec h = valeur du pixel) */
uint32_t *number_nodes;   /* nombre de composantes par niveau */
uint8_t *node_at_level; /* tableau de booleens */
CompTree * TREE;              /* resultat : l'arbre des composantes */


#ifdef IMGCHAR
#define MAXGREY 256
#else
#define MAXGREY 65536
#endif

#ifdef IMGCHAR
int32_t salembier(uint8_t *F, int32_t rs, int32_t cs, int32_t ds, int32_t ps, int32_t N, int32_t connex, chrono *Chrono)
#endif
#ifdef IMGLONG
int32_t salembier(uint32_t *F, int32_t rs, int32_t cs, int32_t ds, int32_t ps, int32_t N, int32_t connex, chrono *Chrono)
#endif
#undef F_NAME
#define F_NAME "salembier"
{
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  int32_t i,k;

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

  STATUS = (uint32_t *)malloc(N * sizeof(int32_t));
  if (STATUS == NULL)
  {   fprintf(stderr, "%s : malloc failed for STATUS\n", F_NAME);
      return(0);
  }

  number_nodes = (uint32_t *)calloc(MAXGREY, sizeof(int32_t));
  if (number_nodes == NULL)
  {   fprintf(stderr, "%s : calloc failed for number_nodes\n", F_NAME);
      return(0);
  }

  node_at_level = (uint8_t *)calloc(MAXGREY, sizeof(char));
  if (node_at_level == NULL)
  {   fprintf(stderr, "%s : calloc failed for node_at_level\n", F_NAME);
      return(0);
  }
  
  TREE = InitCompTree(N);
  if (TREE == NULL)
  {   fprintf(stderr, "%s : InitCompTree failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /* INITIALISATIONS                                  */
  /* ================================================ */

  for (i = 0; i < N; i++) STATUS[i] = NOT_ANALYZED;
  k = 0;             /* recherche un pixel k de niveau de gris minimal dans l'image */
  for (i = 1; i < N; i++) if (F[i] < F[k]) k = i;
  FahPush(FAH, k, F[k]);

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood((int32_t)F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  /*else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);
  */
  FahTermine(FAH);
  TermineCompTree(TREE);

  //printf("SALEMBIER: nb de usec ecoulees = %d\n", read_chrono(Chrono));

  free(STATUS);
  free(number_nodes);
  free(node_at_level);

  return 0;
}

#endif // _SALEMBIER_

int main(int argc, char **argv)
{
  chrono Chrono;
  int32_t i;

  struct xvimage * image;
  int32_t connex, param;
  int32_t rs;      /* taille ligne */
  int32_t cs;      /* taille colonne */
  int32_t ds;        /* nb plans */
  int32_t ps;
  int32_t N;
#ifdef IMGCHAR
  uint8_t *F;
#endif
#ifdef IMGLONG
  uint32_t *F;
#endif

#ifdef _NC_
  ctree *CT; /* Najman-Couprie */
  int32_t *CM;
#endif


  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein.pgm connex\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

#ifdef IMGCHAR
  if (datatype(image) != VFF_TYP_1_BYTE) {
    fprintf(stderr, "%s: Image not 1 Byte\n", argv[0]);
    exit(1);
  }
#endif
#ifdef IMGLONG
  if (datatype(image) != VFF_TYP_4_BYTE) {
    fprintf(stderr, "%s: Image not 4 Byte\n", argv[0]);
    exit(1);
  }
#endif
  connex = atoi(argv[2]);

  rs = rowsize(image);      /* taille ligne */
  cs = colsize(image);      /* taille colonne */
  ds = depth(image);        /* nb plans */
  ps = rs * cs;             /* taille plan */
  N = ps * ds;              /* taille image */

#ifdef IMGCHAR
  F = UCHARDATA(image);
#endif
#ifdef IMGLONG
  F = SLONGDATA(image);
#endif

  /* Najman and Couprie */
  start_chrono(&Chrono);
#ifdef _NC_
  ComponentTree(F, rs, ps, connex, &CT, &CM);
  printf("NC: nb de usec ecoulees = %d\n", read_chrono(&Chrono));

  ComponentTreeFree(CT);
  free(CM);
#endif

#ifdef _SALEMBIER_
  /* Salembier */
  start_chrono(&Chrono);
  salembier(F, rs, cs, ds, ps, N, connex, &Chrono);
  printf("SALEMBIER: nb de usec ecoulees = %d\n", read_chrono(&Chrono));
#endif // _SALEMBIER_  


  freeimage(image);
  return 0;
}
