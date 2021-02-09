/* $Id: bddtest.c,v 1.1.1.1 2008-11-25 08:01:35 mcouprie Exp $ */
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

    memset(G,0,N);
    c = 0x0cc1;
    x=1; y=1; z=1; ns=0;
    printf("%x\n", CODK2V(c));
    SetXAlphacarre3d(c, G, 5, 5, 5, x, y, z);
    if ((!CourbeSimpleOuverte3d(g)) && (Arbre3d(g)))
    { 
      ns++;

      printf("c'est une singularite\n");
    }
} /* main() */













