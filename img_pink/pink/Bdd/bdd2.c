/* $Id: bdd2.c,v 1.1.1.1 2008-11-25 08:01:35 mcouprie Exp $ */
/* 
  Genere les configurations possibles des alphacarre et betacarre pour les 
  types de points 3D:
    carre (3 sous-types)
    inter (3 sous-types)

  Genere du code booleen pour les BDD.

  Le codage du voisinage est le suivant: (compatible avec les fonctions de 
  mckhalimsky3d)

                               22          16          24
                              6           2           8
                           18          14          20

                               11           5          13
                              0           .           1
                           10           4          12

                               23          17          25
                              7           3           9
                           19          15          21
   
   Michel COUPRIE, novembre 1999
*/

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mckhalimsky3d.h>

void printboolean2(FILE *fd, int a, int b)
{
  if (a) fprintf(fd, "a "); else fprintf(fd, "a' ");
  if (b) fprintf(fd, "b "); else fprintf(fd, "b' ");
}

void printboolean8(FILE *fd, int a, int b, int c, int d, int e, int f, int g, int h)
{
  if (a) fprintf(fd, "a "); else fprintf(fd, "a' ");
  if (b) fprintf(fd, "b "); else fprintf(fd, "b' ");
  if (c) fprintf(fd, "c "); else fprintf(fd, "c' ");
  if (d) fprintf(fd, "d "); else fprintf(fd, "d' ");
  if (e) fprintf(fd, "e "); else fprintf(fd, "e' ");
  if (f) fprintf(fd, "f "); else fprintf(fd, "f' ");
  if (g) fprintf(fd, "g "); else fprintf(fd, "g' ");
  if (h) fprintf(fd, "h "); else fprintf(fd, "h' ");
}

void printboolean26(FILE *fd, 
  int a,  int b,  int c,  int d,  int e,  int f,  int g,  int h,  int i,  int j, 
  int k, int l, int m, int n, int o, int p, int q, int r, int s, int t, 
  int u, int v, int w, int x, int y, int z)
{
  if (a) fprintf(fd, "a "); else fprintf(fd, "a' ");
  if (b) fprintf(fd, "b "); else fprintf(fd, "b' ");
  if (c) fprintf(fd, "c "); else fprintf(fd, "c' ");
  if (d) fprintf(fd, "d "); else fprintf(fd, "d' ");
  if (e) fprintf(fd, "e "); else fprintf(fd, "e' ");
  if (f) fprintf(fd, "f "); else fprintf(fd, "f' ");
  if (g) fprintf(fd, "g "); else fprintf(fd, "g' ");
  if (h) fprintf(fd, "h "); else fprintf(fd, "h' ");
  if (i) fprintf(fd, "i "); else fprintf(fd, "i' ");
  if (j) fprintf(fd, "j "); else fprintf(fd, "j' ");
  if (k) fprintf(fd, "k "); else fprintf(fd, "k' ");
  if (l) fprintf(fd, "l "); else fprintf(fd, "l' ");
  if (m) fprintf(fd, "m "); else fprintf(fd, "m' ");
  if (n) fprintf(fd, "n "); else fprintf(fd, "n' ");
  if (o) fprintf(fd, "o "); else fprintf(fd, "o' ");
  if (p) fprintf(fd, "p "); else fprintf(fd, "p' ");
  if (q) fprintf(fd, "q "); else fprintf(fd, "q' ");
  if (r) fprintf(fd, "r "); else fprintf(fd, "r' ");
  if (s) fprintf(fd, "s "); else fprintf(fd, "s' ");
  if (t) fprintf(fd, "t "); else fprintf(fd, "t' ");
  if (u) fprintf(fd, "u "); else fprintf(fd, "u' ");
  if (v) fprintf(fd, "v "); else fprintf(fd, "v' ");
  if (w) fprintf(fd, "w "); else fprintf(fd, "w' ");
  if (x) fprintf(fd, "x "); else fprintf(fd, "x' ");
  if (y) fprintf(fd, "y "); else fprintf(fd, "y' ");
  if (z) fprintf(fd, "z "); else fprintf(fd, "z' ");
}

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  unsigned int c, d;
  int i1, i2, i3, i4, i5, i6, i7, i8, i9, i10;
  int i11, i12, i13, i14, i15, i16, i17, i18, i19, i20;
  int i21, i22, i23, i24, i25, i26;
  struct xvimage *g;
  unsigned char *G;
  int cs, rs, ds, ps, N, x, y, z;
  int first_one;
  int n, ns;
  FILE *fd; 

  if (argc != 1)
  {
    fprintf(stderr, "usage: %s\n", argv[0]);
    exit(0);
  }

  rs = cs = ds = 5;
  ps = rs * cs;
  N = ps * ds;

  g = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  G = UCHARDATA(g);

  InitPileGrilles3d();

  /* ======================================================================= */
  /* POUR LES ALPHAS */
  /* ======================================================================= */

  fd = fopen("bdd2.alphacarrex.res","w");
  first_one = 1;
  /* carre normal a l'axe des x : coord. x paire, coord y, z impaires */
  x = 2; y = 1; z = 1;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  for (i3 = 0; i3 < 2; i3++)
  for (i4 = 0; i4 < 2; i4++)
  for (i5 = 0; i5 < 2; i5++)
  for (i6 = 0; i6 < 2; i6++)
  for (i7 = 0; i7 < 2; i7++)
  for (i8 = 0; i8 < 2; i8++)
  { 
    c = 0;
    if (i1) c |= 1 << 2;
    if (i2) c |= 1 << 3;    
    if (i3) c |= 1 << 4;    
    if (i4) c |= 1 << 5;    
    if (i5) c |= 1 << 14;    
    if (i6) c |= 1 << 15;    
    if (i7) c |= 1 << 16;    
    if (i8) c |= 1 << 17;
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XAlphacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Alpha3Simple3d(g, x, y, z)) 
    { if (!first_one) fprintf(fd, "+\n");
      printboolean8(fd, i1, i2, i3, i4, i5, i6, i7, i8);
      first_one = 0;
    }
  } /* for for for for for for for for */
  fprintf(fd, ".\n");
  fclose(fd);

#ifdef ARETIRER
  fd = fopen("bdd2.alphacarrey.res","w");
  first_one = 1;
  /* carre normal a l'axe des y : coord. y paire, coord x, z impaires */
  x = 1; y = 2; z = 1;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  for (i3 = 0; i3 < 2; i3++)
  for (i4 = 0; i4 < 2; i4++)
  for (i5 = 0; i5 < 2; i5++)
  for (i6 = 0; i6 < 2; i6++)
  for (i7 = 0; i7 < 2; i7++)
  for (i8 = 0; i8 < 2; i8++)
  { 
    c = 0;
    if (i1) c |= 1 << 0;    
    if (i2) c |= 1 << 1;    
    if (i3) c |= 1 << 4;    
    if (i4) c |= 1 << 5;    
    if (i5) c |= 1 << 10;    
    if (i6) c |= 1 << 11;    
    if (i7) c |= 1 << 12;    
    if (i8) c |= 1 << 13;
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XAlphacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Alpha3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean8(fd, i1, i2, i3, i4, i5, i6, i7, i8);
      first_one = 0;
    }
  } /* for for for for for for for for */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.alphacarrez.res","w");
  first_one = 1;
  /* carre normal a l'axe des z : coord. z paire, coord x, y impaires */
  x = 1; y = 1; z = 2;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  for (i3 = 0; i3 < 2; i3++)
  for (i4 = 0; i4 < 2; i4++)
  for (i5 = 0; i5 < 2; i5++)
  for (i6 = 0; i6 < 2; i6++)
  for (i7 = 0; i7 < 2; i7++)
  for (i8 = 0; i8 < 2; i8++)
  { 
    c = 0;
    if (i1) c |= 1 << 0;
    if (i2) c |= 1 << 1;    
    if (i3) c |= 1 << 2;    
    if (i4) c |= 1 << 3;    
    if (i5) c |= 1 << 6;    
    if (i6) c |= 1 << 7;    
    if (i7) c |= 1 << 8;    
    if (i8) c |= 1 << 9;
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XAlphacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Alpha3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean8(fd, i1, i2, i3, i4, i5, i6, i7, i8);
      first_one = 0;
    }
  } /* for for for for for for for for */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.alphainterx.res","w");
  first_one = 1;
  /* intervalle parallele a l'axe des x : coord. x impaire, coord y, z paires */
  x = 1; y = 2; z = 2;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  { 
    c = 0;
    if (i1) c |= 1 << 0;
    if (i2) c |= 1 << 1;    
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XAlphacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Alpha3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean2(fd, i1, i2);
      first_one = 0;
    }
  } /* for for */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.alphaintery.res","w");
  first_one = 1;
  /* intervalle parallele a l'axe des y : coord. y impaire, coord x, z paires */
  x = 2; y = 1; z = 2;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  { 
    c = 0;
    if (i1) c |= 1 << 2;
    if (i2) c |= 1 << 3;    
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XAlphacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Alpha3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean2(fd, i1, i2);
      first_one = 0;
    }
  } /* for for */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.alphainterz.res","w");
  first_one = 1;
  /* intervalle parallele a l'axe des z : coord. z impaire, coord x, y paires */
  x = 2; y = 2; z = 1;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  { 
    c = 0;
    if (i1) c |= 1 << 4;
    if (i2) c |= 1 << 5;    
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XAlphacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Alpha3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean2(fd, i1, i2);
      first_one = 0;
    }
  } /* for for */
  fprintf(fd, ".\n");
  fclose(fd);

  /* ======================================================================= */
  /* POUR LES BETAS */
  /* ======================================================================= */

  fd = fopen("bdd2.betacarrex.res","w");
  first_one = 1;
  /* carre normal a l'axe des x : coord. x paire, coord y, z impaires */
  x = 2; y = 1; z = 1;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  { 
    c = 0;
    if (i1) c |= 1 << 0;
    if (i2) c |= 1 << 1;    
    memset(G,0,N);
    SetXBetacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XBetacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Beta3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean2(fd, i1, i2);
      first_one = 0;
    }
  } /* for for */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.betacarrey.res","w");
  first_one = 1;
  /* carre normal a l'axe des y : coord. y paire, coord x, z impaires */
  x = 1; y = 2; z = 1;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  { 
    c = 0;
    if (i1) c |= 1 << 2;    
    if (i2) c |= 1 << 3;    
    memset(G,0,N);
    SetXBetacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XBetacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Beta3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean2(fd, i1, i2);
      first_one = 0;
    }
  } /* for for */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.betacarrez.res","w");
  first_one = 1;
  /* carre normal a l'axe des z : coord. z paire, coord x, y impaires */
  x = 1; y = 1; z = 2;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  { 
    c = 0;
    if (i1) c |= 1 << 4;
    if (i2) c |= 1 << 5;    
    memset(G,0,N);
    SetXBetacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XBetacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Beta3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean2(fd, i1, i2);
      first_one = 0;
    }
  } /* for for */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.betainterx.res","w");
  first_one = 1;
  /* intervalle parallele a l'axe des x : coord. x impaire, coord y, z paires */
  x = 1; y = 2; z = 2;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  for (i3 = 0; i3 < 2; i3++)
  for (i4 = 0; i4 < 2; i4++)
  for (i5 = 0; i5 < 2; i5++)
  for (i6 = 0; i6 < 2; i6++)
  for (i7 = 0; i7 < 2; i7++)
  for (i8 = 0; i8 < 2; i8++)
  { 
    c = 0;
    if (i1) c |= 1 << 2;
    if (i2) c |= 1 << 3;    
    if (i3) c |= 1 << 4;    
    if (i4) c |= 1 << 5;    
    if (i5) c |= 1 << 14;    
    if (i6) c |= 1 << 15;    
    if (i7) c |= 1 << 16;    
    if (i8) c |= 1 << 17;
    memset(G,0,N);
    SetXBetacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XBetacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Beta3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean8(fd, i1, i2, i3, i4, i5, i6, i7, i8);
      first_one = 0;
    }
  } /* for for ... */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.betaintery.res","w");
  first_one = 1;
  /* intervalle parallele a l'axe des y : coord. y impaire, coord x, z paires */
  x = 2; y = 1; z = 2;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  for (i3 = 0; i3 < 2; i3++)
  for (i4 = 0; i4 < 2; i4++)
  for (i5 = 0; i5 < 2; i5++)
  for (i6 = 0; i6 < 2; i6++)
  for (i7 = 0; i7 < 2; i7++)
  for (i8 = 0; i8 < 2; i8++)
  { 
    c = 0;
    if (i1) c |= 1 << 0;
    if (i2) c |= 1 << 1;    
    if (i3) c |= 1 << 4;    
    if (i4) c |= 1 << 5;    
    if (i5) c |= 1 << 10;    
    if (i6) c |= 1 << 11;    
    if (i7) c |= 1 << 12;    
    if (i8) c |= 1 << 13;
    memset(G,0,N);
    SetXBetacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XBetacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Beta3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean8(fd, i1, i2, i3, i4, i5, i6, i7, i8);
      first_one = 0;
    }
  } /* for for ... */
  fprintf(fd, ".\n");
  fclose(fd);

  fd = fopen("bdd2.betainterz.res","w");
  first_one = 1;
  /* intervalle parallele a l'axe des z : coord. z impaire, coord x, y paires */
  x = 2; y = 2; z = 1;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  for (i3 = 0; i3 < 2; i3++)
  for (i4 = 0; i4 < 2; i4++)
  for (i5 = 0; i5 < 2; i5++)
  for (i6 = 0; i6 < 2; i6++)
  for (i7 = 0; i7 < 2; i7++)
  for (i8 = 0; i8 < 2; i8++)
  { 
    c = 0;
    if (i1) c |= 1 << 0;
    if (i2) c |= 1 << 1;    
    if (i3) c |= 1 << 2;    
    if (i4) c |= 1 << 3;    
    if (i5) c |= 1 << 6;    
    if (i6) c |= 1 << 7;    
    if (i7) c |= 1 << 8;    
    if (i8) c |= 1 << 9;
    memset(G,0,N);
    SetXBetacarre3d(c, G, 5, 5, 5, x, y, z);
    d = XBetacarre3d(G, rs, cs, ds, x, y, z);
    G[z*ps + y*rs + x] = VAL_OBJET;
    if (Beta3Simple3d(g, x, y, z))
    { if (!first_one) fprintf(fd, "+\n");
      printboolean8(fd, i1, i2, i3, i4, i5, i6, i7, i8);
      first_one = 0;
    }
  } /* for for ... */
  fprintf(fd, ".\n");
  fclose(fd);
#endif
} /* main() */


