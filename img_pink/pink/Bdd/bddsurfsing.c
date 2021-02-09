/* $Id: bddsurfsing.c,v 1.1.1.1 2008-11-25 08:01:35 mcouprie Exp $ */
/* 
  Genere les configurations possibles des thetacarre pour les 
  types de points 3D:
    cube
    carre (3 sous-types)

  Genere du code booleen pour les BDD caracterisant les points terminaux de surface
  qui sont des singularites (correspondant a un arbre mais non a une courbe simple).

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

  Un autre codage est utilise pour la visualisation (editcube):

                               24          25          26
                             21          22          23
                           18          19          20

                               15          16          17
                             12          13          14
                            9          10          11

                                6           7           8
                              3           4           5
                            0           1           2
   
  La fonction CODK2V passe du premier au second.
   
   Michel COUPRIE, mai 2000
*/

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mckhalimsky3d.h>

#define CUBES
#define CARRES

unsigned int CODK2V(unsigned int d)
{
  unsigned int c = 0;
  if (d&0x1) c |= 1 << 12;
  if (d&0x2) c |= 1 << 14;
  if (d&0x4) c |= 1 << 22;
  if (d&0x8) c |= 1 << 4;
  if (d&0x10) c |= 1 << 10;
  if (d&0x20) c |= 1 << 16;
  if (d&0x40) c |= 1 << 21;
  if (d&0x80) c |= 1 << 3;
  if (d&0x100) c |= 1 << 23;
  if (d&0x200) c |= 1 << 5;
  if (d&0x400) c |= 1 << 9;
  if (d&0x800) c |= 1 << 15;
  if (d&0x1000) c |= 1 << 11;
  if (d&0x2000) c |= 1 << 17;
  if (d&0x4000) c |= 1 << 19;
  if (d&0x8000) c |= 1 << 1;
  if (d&0x10000) c |= 1 << 25;
  if (d&0x20000) c |= 1 << 7;
  if (d&0x40000) c |= 1 << 18;
  if (d&0x80000) c |= 1 << 0;
  if (d&0x100000) c |= 1 << 20;
  if (d&0x200000) c |= 1 << 2;
  if (d&0x400000) c |= 1 << 24;
  if (d&0x800000) c |= 1 << 6;
  if (d&0x1000000) c |= 1 << 26;
  if (d&0x2000000) c |= 1 << 8;
  return c;
}

void printboolean10(FILE * fd, 
                    int a, int b, int c, int d, int e, int f, int g, int h, int i, int j)
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
}

void printboolean26(FILE * fd, 
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
  unsigned int b, c;
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
  /* POUR LES CUBES */
  /* ======================================================================= */

#ifdef CUBES
  ns = n = 0;
  first_one = 1;
  fd = fopen("bddsurfsing.cube.res","w");
  if (fd == NULL)
  {
    fprintf(stderr, "cannot open file bddsurfsing.cube.res\n");
    exit(0);
  }

  /* cube : coord x, y, z impaires */
  x = 1; y = 1; z = 1;
  for (i1 = 0; i1 < 2; i1++)
  for (i2 = 0; i2 < 2; i2++)
  for (i3 = 0; i3 < 2; i3++)
  for (i4 = 0; i4 < 2; i4++)
  for (i5 = 0; i5 < 2; i5++)
  for (i6 = 0; i6 < 2; i6++)
  for (i7 = 0; i7 < 2; i7++)
  for (i8 = 0; i8 < 2; i8++)
  for (i9 = 0; i9 < 2; i9++)
  for (i10 = 0; i10 < 2; i10++)
  for (i11 = 0; i11 < 2; i11++)
  for (i12 = 0; i12 < 2; i12++)
  for (i13 = 0; i13 < 2; i13++)
  for (i14 = 0; i14 < 2; i14++)
  for (i15 = 0; i15 < 2; i15++)
  for (i16 = 0; i16 < 2; i16++)
  for (i17 = 0; i17 < 2; i17++)
  for (i18 = 0; i18 < 2; i18++)
  for (i19 = 0; i19 < 2; i19++)
  for (i20 = 0; i20 < 2; i20++)
  for (i21 = 0; i21 < 2; i21++)
  for (i22 = 0; i22 < 2; i22++)
  for (i23 = 0; i23 < 2; i23++)
  for (i24 = 0; i24 < 2; i24++)
  for (i25 = 0; i25 < 2; i25++)
  for (i26 = 0; i26 < 2; i26++)
  { 
    n++;
    c = 0;
    if (i1) c |= 1 << 0;
    if (i2) c |= 1 << 1;    
    if (i3) c |= 1 << 2;    
    if (i4) c |= 1 << 3;    
    if (i5) c |= 1 << 4;    
    if (i6) c |= 1 << 5;    
    if (i7) c |= 1 << 6;    
    if (i8) c |= 1 << 7;
    if (i9) c |= 1 << 8;
    if (i10) c |= 1 << 9;
    if (i11) c |= 1 << 10;
    if (i12) c |= 1 << 11;
    if (i13) c |= 1 << 12;
    if (i14) c |= 1 << 13;
    if (i15) c |= 1 << 14;
    if (i16) c |= 1 << 15;
    if (i17) c |= 1 << 16;
    if (i18) c |= 1 << 17;
    if (i19) c |= 1 << 18;
    if (i20) c |= 1 << 19;
    if (i21) c |= 1 << 20;
    if (i22) c |= 1 << 21;
    if (i23) c |= 1 << 22;
    if (i24) c |= 1 << 23;
    if (i25) c |= 1 << 24;
    if (i26) c |= 1 << 25;
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    if ((!CourbeSimpleOuverte3d(g)) && Arbre3d(g))
    { 
      ns++;
#ifdef DEBUG
printf("%x\n", CODK2V(c));
#else
      if (!first_one) fprintf(fd, "+ ");
      printboolean26(fd, i1,  i2,  i3,  i4,  i5,  i6,  i7,  i8,  i9,  i10, 
                         i11, i12, i13, i14, i15, i16, i17, i18, i19, i20,
                         i21, i22, i23, i24, i25, i26);
      fprintf(fd, "\n");
      first_one = 0;
#endif
    }
    if (n % 1000 == 0)
      fprintf(stderr, "%d configs traitees, %d simples\n", n, ns);    
  } /* for for ... */
  fprintf(fd, ".\n");
  fclose(fd);
#endif

  /* ======================================================================= */
  /* POUR LES CARRES */
  /* ======================================================================= */

#ifdef CARRES
  fd = fopen("bddsurfsing.carrex.res","w");
   if (fd == NULL)
  {
    fprintf(stderr, "cannot open file bddsurfsing.carrex.res\n");
    exit(0);
  }
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
  for (i9 = 0; i9 < 2; i9++)
  for (i10 = 0; i10 < 2; i10++)
  { 
    b = c = 0;
    if (i1) c |= 1 << 2;
    if (i2) c |= 1 << 3;    
    if (i3) c |= 1 << 4;    
    if (i4) c |= 1 << 5;    
    if (i5) c |= 1 << 14;    
    if (i6) c |= 1 << 15;    
    if (i7) c |= 1 << 16;    
    if (i8) c |= 1 << 17;
    if (i9) b |= 1 << 0;
    if (i10) b |= 1 << 1;
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    SetXBetacarre3d(b, G, 5, 5, 5, x, y, z);
    if ((!CourbeSimpleOuverte3d(g)) && Arbre3d(g))
    { if (!first_one) fprintf(fd, "+ ");
      printboolean10(fd, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10);
      fprintf(fd, "\n");
      first_one = 0;
    }
  } /* for for for for for for for for */
  fprintf(fd, ".\n");
  fclose(fd);

  first_one = 1;
  fd = fopen("bddsurfsing.carrey.res","w");
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
  for (i9 = 0; i9 < 2; i9++)
  for (i10 = 0; i10 < 2; i10++)
  { 
    b = c = 0;
    if (i1) c |= 1 << 0;    
    if (i2) c |= 1 << 1;    
    if (i3) c |= 1 << 4;    
    if (i4) c |= 1 << 5;    
    if (i5) c |= 1 << 10;    
    if (i6) c |= 1 << 11;    
    if (i7) c |= 1 << 12;    
    if (i8) c |= 1 << 13;
    if (i9) b |= 1 << 2;
    if (i10) b |= 1 << 3;
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    SetXBetacarre3d(b, G, 5, 5, 5, x, y, z);
    if ((!CourbeSimpleOuverte3d(g)) && Arbre3d(g))
    { if (!first_one) fprintf(fd, "+ ");
      printboolean10(fd, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10);
      fprintf(fd, "\n");
      first_one = 0;
    }
  } /* for for for for for for for for */
  fprintf(fd, ".\n");
  fclose(fd);

  first_one = 1;
  fd = fopen("bddsurfsing.carrez.res","w");
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
  for (i9 = 0; i9 < 2; i9++)
  for (i10 = 0; i10 < 2; i10++)
  { 
    b = c = 0;
    if (i1) c |= 1 << 0;
    if (i2) c |= 1 << 1;    
    if (i3) c |= 1 << 2;    
    if (i4) c |= 1 << 3;    
    if (i5) c |= 1 << 6;    
    if (i6) c |= 1 << 7;    
    if (i7) c |= 1 << 8;    
    if (i8) c |= 1 << 9;
    if (i9) b |= 1 << 4;
    if (i10) b |= 1 << 5;
    memset(G,0,N);
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    SetXBetacarre3d(b, G, 5, 5, 5, x, y, z);
    if ((!CourbeSimpleOuverte3d(g)) && Arbre3d(g))
    { if (!first_one) fprintf(fd, "+ ");
      printboolean10(fd, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10);
      fprintf(fd, "\n");
      first_one = 0;
    }
  } /* for for for for for for for for */
  fprintf(fd, ".\n");
  fclose(fd);
#endif
} /* main() */













