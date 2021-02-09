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
/*! \file skel2pov.c

\brief generation of a 3d illustration from a curvilinear skeleton

<B>Usage:</B> skel2pov in.skel out.pov

<B>Description:</B>
Generation of a 3d illustration from a curvilinear skeleton.

<B>Types supported:</B> 2Dskel, 3Dskel

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2004
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>

#define GENHEADER
#define GENFOOTER

#define SCALEX 1
#define SCALEY 1
#define SCALEZ 1

#define HEAD1 (char*)"#include \"colors.inc\""
#define HEAD2 (char*)"#include \"shapes.inc\""
#define HEAD3 (char*)"#include \"textures.inc\""
#define HEAD4 (char*)"#include \"stones.inc\""

#define DECL1  (char*)"#declare ColIsol = color red 1.0 green 1.0 blue 0.0;"
#define DECL2  (char*)"#declare ColCurve = color red 0.0 green 0.0 blue 1.0;"
#define DECL3  (char*)"#declare ColEnd  = color red 1.0 green 0.0 blue 0.0;"
#define DECL4  (char*)"#declare ColJunc = color red 0.0 green 1.0 blue 0.0;"
#define DECL5  (char*)"#declare RadSphere = 5;"
#define DECL6  (char*)"#declare RadCylinder = 1;"

#define LIGHT0 (char*)"light_source { <0,0,-200> color White }"
#define LIGHT1 (char*)"light_source { <200,400,-300> color White }"
#define LIGHT2 (char*)"light_source { <-200,-400,-150> color Gray80 }"
#define LIGHT3 (char*)"light_source { <200,-100,150> color Gray80 }"
#define LIGHT4 (char*)"light_source { <-200,0,0> color Gray80 }"
#define DEBIMAGE (char*)"#declare Image = union {"
#define FINIMAGE (char*)"} // Image"

/* =============================================================== */
void camera (FILE * fd, double x, double y, double z, double lx, double ly, double lz)
/* =============================================================== */
{
  fprintf(fd, "camera { location <%g,%g,%g> up <0,1,0> right <1,0,0> look_at <%g,%g,%g> }\n", x, y, z, lx, ly, lz);
}

/* =============================================================== */
void object (FILE * fd, double x, double y, double z)
/* =============================================================== */
{
  /*  fprintf(fd, "object { Image translate <%g,%g,%g> rotate <clock,clock,clock> }\n", x, y, z); */
  fprintf(fd, "object { Image translate <%g,%g,%g> rotate <10,10,0> }\n", x, y, z);
}

/* =============================================================== */
void sphere (FILE * fd, double x, double y, double z, char * r, char *color)
/* =============================================================== */
{
  fprintf(fd, "sphere { <%g,%g,%g>, %s pigment {color %s} }\n", x, y, z, r, color);
}

/* =============================================================== */
void cylinder (FILE * fd, double x1, double y1, double z1,
                          double x2, double y2, double z2, char * r, char *color)
/* =============================================================== */
{
  fprintf(fd, "cylinder { <%g,%g,%g>, <%g,%g,%g>, %s open pigment {color %s} }\n", 
              x1, y1, z1, x2, y2, z2, r, color);
}
/* ====================================================================== */
static void barycentre(SKC_pt_pcell p, int32_t rs, int32_t ps, double *x, double *y, double *z)
/* ====================================================================== */
{
  uint32_t v; 
  double sx, sy, sz;
  int32_t n;

  sx = sy = sz = 0.0; n = 0;
  for (; p != NULL; p = p->next)
  {
    n++;
    v = p->val;
    sx += (double)(v % rs); 
    sy += (double)((v % ps) / rs);
    sz += (double)(v / ps);
  }
  *x = sx / n;
  *y = sy / n;
  *z = sz / n;
} /* barycentre() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  skel * S;
  FILE *fd = NULL;
  int32_t rs, ps, n, i, j, k;
  double x, y, z;
  int32_t npoints = 0;
  double bx, by, bz; //pour le calcul du barycentre global
  double *X, *Y, *Z; // tableaux pour les coordonnées
  SKC_adj_pcell a;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.skel out.pov\n", argv[0]);
    exit(1);
  }

  S = readskel(argv[1]);
  if (S == NULL)
  {
    fprintf(stderr, "%s: readskel failed\n", argv[0]);
    exit(1);
  }
  rs = S->rs;
  ps = rs * S->cs; 
  n = S->e_junc;

  X = (double *)calloc(1,n * sizeof(double));
  Y = (double *)calloc(1,n * sizeof(double));
  Z = (double *)calloc(1,n * sizeof(double));
  if ((X == NULL) || (Y == NULL) || (Z == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[argc - 1], "w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

#ifdef GENHEADER
  fprintf(fd, "//Generated by skel2pov (M. Couprie 2004) from file : %s ;\n", argv[1]);

  fprintf(fd, "%s\n%s\n%s\n%s\n", HEAD1, HEAD2, HEAD3, HEAD4);
  fprintf(fd, "%s\n%s\n%s\n%s\n%s\n%s\n", DECL1, DECL2, DECL3, DECL4, DECL5, DECL6);
  camera(fd, 0, 0, -120, 0, 0, 0);
  fprintf(fd, "%s\n%s\n%s\n%s\n%s\n", LIGHT0, LIGHT1, LIGHT2, LIGHT3, LIGHT4);
  fprintf(fd, "%s\n", DEBIMAGE);
#endif

  // génère les sommets

  for (i = 0; i < S->e_isol; i++)
  {
    barycentre(S->tskel[i].pts, rs, ps, &x, &y, &z);
    X[i] = x; Y[i] = y; Z[i] = z;
    sphere(fd, x, y, z, (char*)"RadSphere", (char*)"ColIsol");
    npoints++; bx += x; by += y; bz += z;
  }

  for (i = S->e_isol; i < S->e_end; i++)
  {
    barycentre(S->tskel[i].pts, rs, ps, &x, &y, &z);
    X[i] = x; Y[i] = y; Z[i] = z;
    sphere(fd, x, y, z, (char*)"RadSphere", (char*)"ColEnd");
    npoints++; bx += x; by += y; bz += z;
  }

  for (i = S->e_curv; i < S->e_junc; i++)
  {
    barycentre(S->tskel[i].pts, rs, ps, &x, &y, &z);
    X[i] = x; Y[i] = y; Z[i] = z;
    sphere(fd, x, y, z, (char*)"RadSphere", (char*)"ColJunc");    
    npoints++; bx += x; by += y; bz += z;
  }

  // génère les arêtes

  for (i = S->e_end; i < S->e_curv; i++)
  {
    a = S->tskel[i].adj; 
    if ((a == NULL) || (a->next == NULL))
    {
      fprintf(stderr, "%s: error in skelton structure\n", argv[0]);
      exit(1);
    }
    j = a->val;
    k = a->next->val;
    if (j > k)
    {
      cylinder(fd, X[j], Y[j], Z[j], X[k], Y[k], Z[k], (char*)"RadCylinder", (char*)"ColCurve");    
    }
  } // for i

#ifdef GENFOOTER
  fprintf(fd, "%s\n", FINIMAGE);
  bx = bx / npoints; by = by / npoints; bz = bz / npoints;
  object(fd, -bx, -by, -bz);
#endif

  free(X);
  free(Y);
  free(Z);
  fclose(fd);
  termineskel(S);
  return 0;
} /* main */
