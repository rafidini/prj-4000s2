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
/* Michel Couprie -  juillet 2001 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mcindic.h>
#include <mcrbt.h>
#include <mctopo3d.h>
#include <mcfifo.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lskeletons.h>

#define EN_RBT        0
#define EN_FIFO       1
#define CONTRAINTE    2
#define EN_RBT1       0
#define EN_RBT2       1
#define PARANO
//#define DEBUG

/* ==================================== */
static int32_t typedir(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t y;
  y = voisin(x, NORD, rs, N); if ((y!=-1) && (F[y]==0)) return 0;
  y = voisin(x, SUD, rs, N); if ((y!=-1) && (F[y]==0)) return 1;
  y = voisin(x, EST, rs, N); if ((y!=-1) && (F[y]==0)) return 2;
  y = voisin(x, OUEST, rs, N); if ((y!=-1) && (F[y]==0)) return 3;
  return 4;
} /* typedir() */

/* ==================================== */
static int32_t typedir3d(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
{
  int32_t y;
  y = voisin6(x, NORD, rs, ps, N); if ((y!=-1) && (F[y]==0)) return 0;
  y = voisin6(x, SUD, rs, ps, N); if ((y!=-1) && (F[y]==0)) return 1;
  y = voisin6(x, EST, rs, ps, N); if ((y!=-1) && (F[y]==0)) return 2;
  y = voisin6(x, OUEST, rs, ps, N); if ((y!=-1) && (F[y]==0)) return 3;
  y = voisin6(x, DEVANT, rs, ps, N); if ((y!=-1) && (F[y]==0)) return 4;
  y = voisin6(x, DERRIERE, rs, ps, N); if ((y!=-1) && (F[y]==0)) return 5;
  return 6;
} /* typedir3d() */

/* ==================================== */
static int32_t testabaisse4bin(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple4(F, x, rs, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse4bin() */

/* ==================================== */
static int32_t testabaisse8bin(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple8(F, x, rs, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse8bin() */

/* ==================================== */
static int32_t testabaisse6bin(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple6(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse6bin() */

/* ==================================== */
static int32_t testabaisse26bin(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple26(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse26bin() */

/* ==================================== */
int32_t lskelubp(struct xvimage *image, 
              struct xvimage *imageprio, 
              int32_t connex, 
              int32_t val_inhibit)
/* ==================================== */
/* 
Squelette ultime 2D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.
Si le parametre 'inhibit' a une valeur differente de -1,
il indique une valeur de priorite telle que les points affectes de
cette valeur ne seront jamais selectionnes. 

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points ayant une valeur specifiee VAL_INHIBIT ne seront
pas selectionnes.

On definit l'operateur Ultimate Thinning UT(F,P) : 
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que P[x] != VAL_INHIBIT 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelubp"
{ 
  int32_t i, k;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;

  IndicsInit(N);

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != 1))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }
  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (P[x] != val_inhibit) && bordext8(F, x, rs, N))
    {
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse4bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] != val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse8bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] != val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }


  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
  return(1);
} /* lskelubp() */

/* ==================================== */
int32_t lskelubp2(struct xvimage *image, 
               struct xvimage *imageprio, 
               int32_t connex, 
               struct xvimage *imageinhib)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelubp2"
{ 
  int32_t i, k;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = UCHARDATA(imageinhib); /* l'image d'inhibition */
  uint32_t *P = NULL;  /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;

  IndicsInit(N);

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != 1))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }
  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (!I[x]) && bordext8(F, x, rs, N))
    {
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse4bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I[y]) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse8bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I[y]) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
  return(1);
} /* lskelubp2() */

/* ==================================== */
int32_t lskelubp3d(struct xvimage *image, 
                 struct xvimage *imageprio, 
                 int32_t connex, 
                 int32_t val_inhibit)
/* ==================================== */
/* 
Squelette ultime 3D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.
Si le parametre 'inhibit' a une valeur differente de -1,
il indique une valeur de priorite telle que les points affectes de
cette valeur ne seront jamais selectionnes. 

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points ayant une valeur specifiee VAL_INHIBIT ne seront
pas selectionnes.

On definit l'operateur Ultimate Thinning UT(F,P) : 
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que P[x] != VAL_INHIBIT 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelubp3d"
{ 
  int32_t i, k;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t d = depth(image);
  int32_t N = d * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint32_t *P = NULL;  /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;

  IndicsInit(N);

  init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != d))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }
  taillemaxrbt = 2 * rs * cs +  2 * rs * d +  2 * d * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (P[x] != val_inhibit) && bordext26(F, x, rs, ps, N))
    {
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse6bin(F, x, rs, ps, N))      /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] != val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse26bin(F, x, rs, ps, N))         /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] != val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse26bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  termine_topo3d();
  IndicsTermine();
  RbtTermine(RBT);
  return(1);
} /* lskelubp3d() */

/* ==================================== */
int32_t lskelubp3d2(struct xvimage *image, 
                 struct xvimage *imageprio, 
                 int32_t connex, 
                 struct xvimage *imageinhib)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelubp3d2"
{ 
  int32_t i, k;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t d = depth(image);
  int32_t N = d * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = UCHARDATA(imageinhib); /* l'image d'inhibition */
  uint32_t *P = NULL;  /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;

  IndicsInit(N);

  init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != d))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }
  taillemaxrbt = 2 * rs * cs +  2 * rs * d +  2 * d * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (!I[x]) && bordext26(F, x, rs, ps, N))
    {
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse6bin(F, x, rs, ps, N))      /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I[y]) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse26bin(F, x, rs, ps, N))         /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I[y]) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse26bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  termine_topo3d();
  IndicsTermine();
  RbtTermine(RBT);
  return(1);
} /* lskelubp3d2() */

/* ==================================== */
int32_t lskelcurv(struct xvimage *image, 
              struct xvimage *imageprio, 
              struct xvimage *inhibit, 
              int32_t connex)
/* ==================================== */
/* 
Squelette curviligne 2D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.

On definit l'operateur Curvilinear Thinning CT(F,P) : 
C = empty image
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que C[x] == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
  pour tout y dans gamma(x)
    si T(y) > 1 alors C[y] = 1
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelcurv"
{ 
  int32_t i, k, t, tb;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  Fifo * FIFO1;
  Fifo * FIFO2;
  int32_t prio, oldprio;
  int32_t taillemaxrbt;

  IndicsInit(N);

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != 1))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s() : bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit);
    else 
    {
      fprintf(stderr, "%s() : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
    for (x = 0; x < N; x++)
      if (I[x]) Set(x,CONTRAINTE);
  }

  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  FIFO1 = CreeFifoVide(N);
  if (FIFO1 == NULL)
  {
    fprintf(stderr, "%s() : CreeFifoVide failed\n", F_NAME);
    return(0);
  }
  FIFO2 = CreeFifoVide(N);
  if (FIFO2 == NULL)
  {
    fprintf(stderr, "%s() : CreeFifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 4)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext4(F, x, rs, N))
      {
#define PRIODIR
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir(F, x, rs, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
#ifdef DEBUG
printf("init: push %d,%d (%d)\n", x%rs, x/rs, P[x]*10 + typedir(F, x, rs, N));
#endif
      }
    }
  }
  else if (connex == 8)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext8(F, x, rs, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir(F, x, rs, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
#ifdef DEBUG
printf("init: push %d,%d (%d)\n", x%rs, x/rs, P[x]*10 + typedir(F, x, rs, N));
#endif
      }
    }
  }
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && testabaisse4bin(F, x, rs, N))
        {
          for (k = 0; k < 8; k += 2)          /* parcourt les voisins en 4-connexite */
          {                                              /* pour empiler les voisins */
            y = voisin(x, k, rs, N);                             /* non deja empiles */
            if ((y != -1) && (F[y]))
            {
              if (! IsSet(y,EN_FIFO))
	      {
                FifoPush(FIFO2, y);
                Set(y, EN_FIFO);
	      }
              if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
                RbtInsert(&RBT, P[y]*10 + typedir(F, y, rs, N), y);
#else
                RbtInsert(&RBT, P[y], y);
#endif
                Set(y, EN_RBT);
	      }
            } /* if y */
          } /* for k */      
        } /* if (testabaisse4bin(F, x, rs, N)) */
      } // while (!FifoVide(FIFO1))

      while (!FifoVide(FIFO2))
      {
        x = FifoPop(FIFO2);
        UnSet(x,EN_FIFO);
        if ((! IsSet(x, CONTRAINTE)) && (nonbord(x,rs,N)))
        {
          top4(F, x, rs, N, &t, &tb);
          if (tb > 1) Set(x, CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */
  else if (connex == 8)
  {
    while (!RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && testabaisse8bin(F, x, rs, N))
        {
          for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
          {                                              /* pour empiler les voisins */
            y = voisin(x, k, rs, N);                             /* non deja empiles */
            if ((y != -1) && (F[y]))
            {
              if (! IsSet(y,EN_FIFO))
	      {
                FifoPush(FIFO2, y);
                Set(y, EN_FIFO);
	      }
              if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
                RbtInsert(&RBT, P[y]*10 + typedir(F, y, rs, N), y);
#else
                RbtInsert(&RBT, P[y], y);
#endif
                Set(y, EN_RBT);
	      }
            } /* if y */
          } /* for k */      
        } /* if (testabaisse8bin(F, x, rs, N)) */
      } // while (!FifoVide(FIFO1))

      while (!FifoVide(FIFO2))
      {
        x = FifoPop(FIFO2);
        UnSet(x,EN_FIFO);
        if ((! IsSet(x, CONTRAINTE)) && (nonbord(x,rs,N)))
        {
          top8(F, x, rs, N, &t, &tb);
          if (tb > 1) Set(x, CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
  FifoTermine(FIFO1);
  FifoTermine(FIFO2);
  return(1);
} /* lskelcurv() */

/* ==================================== */
int32_t lskelcurv3d(struct xvimage *image, 
              struct xvimage *imageprio, 
              struct xvimage *inhibit, 
              int32_t connex)
/* ==================================== */
/* 
Squelette curviligne 3D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.

De facon tres schematique,
on definit l'operateur Curvilinear Thinning CT(F,P) : 
C = empty image
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que C[x] == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
  pour tout y dans gamma(x)
    si T(y) > 1 alors C[y] = 1
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelcurv3d"
{ 
  int32_t i, k, t, tb;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t prio, oldprio;
  int32_t taillemaxrbt;

  IndicsInit(N);
  init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }
  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != ds))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s() : bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit);
    else 
    {
      fprintf(stderr, "%s() : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
    for (x = 0; x < N; x++)
      if (I[x]) Set(x,CONTRAINTE);
  }

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 6)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext6(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext26(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    Fifo * FIFO1;
    Fifo * FIFO2;
    FIFO1 = CreeFifoVide(N/2);
    if (FIFO1 == NULL)
    {
      fprintf(stderr, "%s() : CreeFifoVide failed\n", F_NAME);
      return(0);
    }
    FIFO2 = CreeFifoVide(N/2);
    if (FIFO2 == NULL)
    {
      fprintf(stderr, "%s() : CreeFifoVide failed\n", F_NAME);
      return(0);
    }

    while (!RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && testabaisse6bin(F, x, rs, ps, N))
        {
          for (k = 0; k <= 10; k += 2)        /* parcourt les voisins en 6-connexite */
          {                                              /* pour empiler les voisins */
            y = voisin6(x, k, rs, ps, N);                        /* non deja empiles */
            if ((y != -1) && (F[y]))
            {
              if (! IsSet(y,EN_FIFO))
	      {
                FifoPush(FIFO2, y);
                Set(y, EN_FIFO);
	      }
              if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
                RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
                RbtInsert(&RBT, P[y], y);
#endif
                Set(y, EN_RBT);
	      }
            } /* if y */
          } /* for k */      
        } /* if (testabaisse4bin(F, x, rs, N)) */
      } // while (!FifoVide(FIFO1))

      while (!FifoVide(FIFO2))
      {
        x = FifoPop(FIFO2);
        UnSet(x,EN_FIFO);
        if ((! IsSet(x, CONTRAINTE)) && (nonbord3d(x,rs,ps,N)))
        {
          top6(F, x, rs, ps, N, &t, &tb);
          if (t > 1) Set(x, CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!RbtVide(RBT)) */
    FifoTermine(FIFO1);
    FifoTermine(FIFO2);
  } /* if (connex == 6) */
  else if (connex == 26)  // NOTE : en 26 connexite pas besoin de la strategie a 2 passes (FIFO)
  {           
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if ((! IsSet(x, CONTRAINTE)) && testabaisse26bin(F, x, rs, ps, N))
      {
        for (k = 0; k < 26; k += 1)         /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]))
          {
            if ((! IsSet(y, CONTRAINTE)) && (nonbord3d(y,rs,ps,N)))
	    {
              top26(F, y, rs, ps, N, &t, &tb);
              if (t > 1) 
	      {
                Set(y, CONTRAINTE);
	      }
	    }
            if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	    {
#ifdef PRIODIR
              RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
              RbtInsert(&RBT, P[y], y);
#endif
              Set(y, EN_RBT);
	    }
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  termine_topo3d();
  RbtTermine(RBT);
  return(1);
} /* lskelcurv3d() */

/* ==================================== */
int32_t lskelsurf3d(struct xvimage *image, 
              struct xvimage *imageprio, 
              struct xvimage *inhibit, 
              int32_t connex)
/* ==================================== */
/* 
Squelette surfacique 3D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.

De facon tres schematique,
on definit l'operateur Surfacic Thinning CT(F,P) : 
C = empty image
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que C[x] == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
  pour tout y dans gamma(x)
    si Tb(y) > 1 alors C[y] = 1
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelsurf3d"
{ 
  int32_t i, k, t, tb;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t prio, oldprio;
  int32_t taillemaxrbt;

  IndicsInit(N);
  init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != ds))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s() : bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit);
    else 
    {
      fprintf(stderr, "%s() : datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
    for (x = 0; x < N; x++)
      if (I[x]) Set(x,CONTRAINTE);
  }

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */


  if (connex == 6)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext6(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext26(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    Fifo * FIFO1;
    Fifo * FIFO2;
    FIFO1 = CreeFifoVide(N/2);
    if (FIFO1 == NULL)
    {
      fprintf(stderr, "%s() : CreeFifoVide failed\n", F_NAME);
      return(0);
    }
    FIFO2 = CreeFifoVide(N/2);
    if (FIFO2 == NULL)
    {
      fprintf(stderr, "%s() : CreeFifoVide failed\n", F_NAME);
      return(0);
    }

    while (!RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && testabaisse6bin(F, x, rs, ps, N))
        {
          for (k = 0; k <= 10; k += 2)        /* parcourt les voisins en 6-connexite */
          {                                              /* pour empiler les voisins */
            y = voisin6(x, k, rs, ps, N);                        /* non deja empiles */
            if ((y != -1) && (F[y]))
            {
              if (! IsSet(y,EN_FIFO))
	      {
                FifoPush(FIFO2, y);
                Set(y, EN_FIFO);
	      }
              if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
                RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
                RbtInsert(&RBT, P[y], y);
#endif
                Set(y, EN_RBT);
	      }
            } /* if y */
          } /* for k */      
        } /* if (testabaisse4bin(F, x, rs, N)) */
      } // while (!FifoVide(FIFO1))

      while (!FifoVide(FIFO2))
      {
        x = FifoPop(FIFO2);
        UnSet(x,EN_FIFO);
        if ((! IsSet(x, CONTRAINTE)) && (nonbord3d(x,rs,ps,N)))
        {
          top6(F, x, rs, ps, N, &t, &tb);
          if (tb > 1) Set(x, CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!RbtVide(RBT)) */
    FifoTermine(FIFO1);
    FifoTermine(FIFO2);
  } /* if (connex == 6) */
  else if (connex == 26)  // NOTE : en 26 connexite pas besoin de la strategie a 2 passes (FIFO)
  {           
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if ((! IsSet(x, CONTRAINTE)) && testabaisse26bin(F, x, rs, ps, N))
      {
        for (k = 0; k < 26; k += 1)         /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]))
          {
            if ((! IsSet(y, CONTRAINTE)) && (nonbord3d(y,rs,ps,N)))
	    {
              top26(F, y, rs, ps, N, &t, &tb);
              if (tb > 1) 
	      {
                Set(y, CONTRAINTE);
	      }
	    }
            if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	    {
#ifdef PRIODIR
              RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
              RbtInsert(&RBT, P[y], y);
#endif
              Set(y, EN_RBT);
	    }
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  termine_topo3d();
  RbtTermine(RBT);
  return(1);
} /* lskelsurf3d() */

/* ==================================== */
int32_t ltoposhrink(struct xvimage *image, 
              struct xvimage *imageprio, 
              int32_t connex, 
              int32_t tmin, 
              int32_t tmax, 
              int32_t tbmin, 
              int32_t tbmax, 
              struct xvimage *imageinhibit)
/* ==================================== */
/* 
Amincissement 2D binaire avec controle topologique.

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points de l'image I (imageinhibit) ne seront
pas selectionnes.

repeter jusqu'a stabilite
  choisir un point x de F \ I tel que (tmin <= T(x) <= tmax) et (tbmin <= Tb(x) <= tbmax)
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "ltoposhrink"
{ 
  int32_t i, k;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;
  int32_t t, tb;

  IndicsInit(N);

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != 1))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }
  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  if (imageinhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(imageinhibit) != rs) || (colsize(imageinhibit) != cs) || (depth(imageinhibit) != 1))
    {
      fprintf(stderr, "%s() : bad size for imageinhibit\n", F_NAME);
      return(0);
    }
    if (datatype(imageinhibit) != VFF_TYP_1_BYTE) 
    {
      fprintf(stderr, "%s() : bad type for imageinhibit\n", F_NAME);
      return(0);
    }
    I = UCHARDATA(imageinhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x, CONTRAINTE);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && !IsSet(x, CONTRAINTE) && bordext8(F, x, rs, N))
    {
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top4(F, x, rs, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top8(F, x, rs, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }


  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
  return(1);
} /* ltoposhrink() */

/* ==================================== */
int32_t ltoposhrink3d(struct xvimage *image, 
                 struct xvimage *imageprio, 
                 int32_t connex, 
                 int32_t tmin, 
                 int32_t tmax, 
                 int32_t tbmin, 
                 int32_t tbmax, 
                 struct xvimage *imageinhibit)
/* ==================================== */
/* 
Amincissement 3D binaire avec controle topologique.

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points de l'image I (imageinhibit) ne seront
pas selectionnes.

repeter jusqu'a stabilite
  choisir un point x de F \ I tel que (tmin <= T(x) <= tmax) et (tbmin <= Tb(x) <= tbmax)
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "ltoposhrink3d"
{ 
  int32_t i, k;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t d = depth(image);
  int32_t N = d * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint32_t *P = NULL;  /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;
  int32_t t, tb;

  IndicsInit(N);

  init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s() : imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != d))
  {
    fprintf(stderr, "%s() : bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s() : datatype(imageprio) must be uint32_t\n", F_NAME);
    return(0);
  }
  taillemaxrbt = 2 * rs * cs +  2 * rs * d +  2 * d * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  if (imageinhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(imageinhibit) != rs) || (colsize(imageinhibit) != cs) || (depth(imageinhibit) != d))
    {
      fprintf(stderr, "%s() : bad size for imageinhibit\n", F_NAME);
      return(0);
    }
    if (datatype(imageinhibit) != VFF_TYP_1_BYTE) 
    {
      fprintf(stderr, "%s() : bad type for imageinhibit\n", F_NAME);
      return(0);
    }
    I = UCHARDATA(imageinhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x, CONTRAINTE);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && !IsSet(x, CONTRAINTE) && bordext26(F, x, rs, ps, N))
    {
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top6(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 18)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top18(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 18) */
  else
  if (connex == 26)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top26(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  termine_topo3d();
  IndicsTermine();
  RbtTermine(RBT);
  return(1);
} /* ltoposhrink3d() */

/* ==================================== */
int32_t lskeleucl(struct xvimage *image, 
              int32_t connex, 
              struct xvimage *imageinhib)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskeleucl"
{ 
  int32_t i, k;
  int32_t x, y, z;                 /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  struct xvimage *imageprio;
  uint32_t *P = NULL;     /* l'image de priorites */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = UCHARDATA(imageinhib); /* l'image d'inhibition */
  Rbt *RBT1, *RBT2;
  int32_t taillemaxrbt;
  double prio, sqrt_2 = sqrt(2.0);
  uint32_t mini;

  if (depth(image) != 1)
  {
    fprintf(stderr, "%s(): only for 2D images\n", F_NAME);
    return 0;
  }

  IndicsInit(N);

  imageprio = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  if (imageprio == NULL)
  {   
    fprintf(stderr, "%s(): allocimage failed\n", F_NAME);
    return 0;
  }
  P = SLONGDATA(imageprio); 

  if (!ldistquadSaito(image, imageprio))
  {   
    fprintf(stderr, "%s(): ldistquadSaito failed\n", F_NAME);
    return 0;
  }

  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT1 = CreeRbtVide(taillemaxrbt);
  RBT2 = CreeRbtVide(taillemaxrbt);
  if ((RBT1 == NULL) || (RBT2 == NULL))
  {
    fprintf(stderr, "%s() : CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (!I[x]) && bordext8(F, x, rs, N))
    {
      RbtInsert(&RBT1, sqrt(P[x]), x);
      Set(x, EN_RBT1);
    }
    if (F[x] && I[x])
      for (k = 0; k < 8; k += 1)
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && F[y] && (!I[y]) && (P[y] > P[x]))
	{
          if (k%2) prio = sqrt(P[x]) + (sqrt(P[y]) - sqrt(P[x])) / sqrt_2;
          else prio = sqrt(P[y]);
          RbtInsert(&RBT2, prio, y);
          Set(y, EN_RBT2);
	}
      } // for k
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while ((!RbtVide(RBT1)) || (!RbtVide(RBT2)))
    {
      if (RbtVide(RBT1))
      { x = RbtPopMin(RBT2); UnSet(x, EN_RBT2); }
      else if (RbtVide(RBT2))
      { x = RbtPopMin(RBT1); UnSet(x, EN_RBT1); }
      else 
      {
        if (RbtMinLevel(RBT2) <= RbtMinLevel(RBT1) )
        { x = RbtPopMin(RBT2); UnSet(x, EN_RBT2); }
        else 
        { x = RbtPopMin(RBT1); UnSet(x, EN_RBT1); }
      }
      if (I[x]) goto finwhile;
      if (testabaisse8bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && F[y] && (!I[y]) && (!IsSet(y, EN_RBT1)))
          {
            prio = sqrt(P[x]) + (sqrt(P[y]) - sqrt(P[x])
            RbtInsert(&RBT1, prio, y);
            Set(y, EN_RBT1);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
      else 
      {
        I[x] = 1; // le point x ne sera plus considéré, il fait partie du squelette
	mini = P[x]; z = x;
        for (k = 0; k < 8; k += 1) // recherche le point de squelette le plus bas
        {
          y = voisin(x, k, rs, N);
          if ((y != -1) && F[y] && I[y] && (P[y] < mini))
	  {
	    mini = P[y];
	    z = y;
	  }
	}
        for (k = 0; k < 8; k += 1)
        {
          y = voisin(x, k, rs, N);
          if ((y != -1) && F[y] && (!I[y]) && (P[y] > P[x]))
	  {
            prio = sqrt(P[z]) + (sqrt(P[y]) - sqrt(P[z])) / dist2((double)(y%rs), (double)(y/rs), (double)(z%rs), (double)(z/rs));
            RbtInsert(&RBT2, prio, y);
            Set(y, EN_RBT2);
	  } // if y
        } // for k
      } // else
    finwhile:;
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  {
    fprintf(stderr, "%s() : bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT1);
  RbtTermine(RBT2);
  freeimage(imageprio);
  return(1);
} /* lskeleucl() */




#ifdef DEBUG
	    if ((x%rs >= 460) && (x%rs <= 464) && (x/rs >= 239) && (x/rs <= 243))
	printf("Push RBT2: y=%d,%d; P[y]=%d; prio=%g\n", y%rs, y/rs, P[y], prio);
#endif
