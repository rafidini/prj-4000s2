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
/* \file 2dtopoflow.c

\brief computes the topological flow (see [Cou10]) of a 2D complex

<B>Usage:</B> 2dtopoflow in.pgm prio [inhibit] out.pgm

<B>Description:</B>

EXPERIMENTAL - DO NOT USE IN APPLICATIONS

TODO

The lowest values of the priority image correspond to the highest priority.

The parameter \b prio is either an image (byte or int32_t), or a numerical code
indicating that a distance map will be used as a priority image; 
the possible choices are:
\li 0: approximate euclidean distance
\li 1: approximate quadratic euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 
If the parameter \b inhibit is given and is a number I,
then the points with priority greater than or equal to I will be left unchanged.

References:<BR> 
[Cou10] M. Couprie: "Topological flows, maps and skeletons", in preparation.<BR>

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky2d.h>
#include <mcgeo.h>
#include <mcdrawps.h>
#include <mcgraphe.h>
#include <ldist.h>
#include <l2dcollapse.h>

#define SHOWGRAPHS
//#define SHOWIMAGES

#define FS_EPSILON 0.1

#ifdef SHOWGRAPHS
void ShowGraphe(graphe * g, char *filename, double s, double r, double t, double marge, int noms_sommets, int v_sommets, int col_sommets, int all_arcs, uint8_t *K, int rs, boolean *head) 
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
  EPSHeader(fd, xmax - xmin + 2.0 * marge, ymax - ymin + 2.0 * marge, 1.0, 14);
  
  /* dessine le fond */
  PSSetColor (fd, 1.0);
  PSDrawRect (fd, 0, 0, xmax - xmin + 2.0 * marge, ymax - ymin + 2.0 * marge);
  PSSetColor (fd, 0.0);

  /* dessine le complexe */
  PSSetColor (fd, 0.75);
  PSSetLineWidth (fd, 5);    
  for (i = 0; i < n; i++) 
    if (K[i])
    {
      xx = i%rs;
      yy = i/rs;
      if ((xx%2 == 0) && (yy%2 == 0))
      {
	PSDrawdisc(fd, s*g->x[i]-xmin+marge, s*g->y[i]-ymin+marge, 5);
	if ((xx < rs-2) && K[i+2])
	  PSLine(fd, s*g->x[i]-xmin+marge, s*g->y[i]-ymin+marge, s*g->x[i+2]-xmin+marge, s*g->y[i+2]-ymin+marge);
	if ((i < n-(rs+rs)) && K[i+rs+rs])
	  PSLine(fd, s*g->x[i]-xmin+marge, s*g->y[i]-ymin+marge, s*g->x[i+rs+rs]-xmin+marge, s*g->y[i+rs+rs]-ymin+marge);
      }
    }
  PSSetColor (fd, 0.0);
  PSSetLineWidth (fd, 1);

  /* dessine les sommets */
  for (i = 0; i < n; i++) 
    if (K[i])
    {
      if (col_sommets && (g->v_sommets[i] != 0)) 
	PSDrawdisc(fd, s*g->x[i]-xmin+marge, s*g->y[i]-ymin+marge, r);
      else
	PSDrawcircle(fd, s*g->x[i]-xmin+marge, s*g->y[i]-ymin+marge, r);
    }

  if (noms_sommets && g->nomsommet)
    for (i = 0; i < n; i++) 
      if (K[i])
	PSString(fd, s*g->x[i]-xmin+marge+2*r, s*g->y[i]-ymin+marge-2*r, g->nomsommet[i]);
  if (v_sommets)
    for (i = 0; i < n; i++) 
#ifdef DESSINECOLSEQ
      if (K[i] && !head[i] && (g->v_sommets[i] < 0))
      { //pour dessiner une col. seq.
	sprintf(buf, "%g", -(double)(g->v_sommets[i]));
	PSString(fd, s*g->x[i]-xmin+marge+2*r, s*g->y[i]-ymin+marge+2*r, buf);
      }
#else
      if (K[i])
      {
	sprintf(buf, "%.1f", (double)(g->v_sommets[i]));
	PSString(fd, s*g->x[i]-xmin+marge+2*r, s*g->y[i]-ymin+marge+2*r, buf);
      }
#endif

  /* dessine les arcs */
  for (i = 0; i < n; i++)
    if (all_arcs || head[i])
      for (p = g->gamma[i]; p != NULL; p = p->next)
      {
	j = p->som;
	PSLine(fd, s*g->x[i]-xmin+marge, s*g->y[i]-ymin+marge, s*g->x[j]-xmin+marge, s*g->y[j]-ymin+marge);
      }

  /* dessine les fleches sur les arcs */
  if (t > 0.0)
  {
    for (i = 0; i < n; i++) 
    if (all_arcs || head[i])
    for (p = g->gamma[i]; p != NULL; p = p->next)
    {
      j = p->som;
      x1 = s*g->x[i]-xmin+marge;
      y1 = s*g->y[i]-ymin+marge;
      x2 = s*g->x[j]-xmin+marge;
      y2 = s*g->y[j]-ymin+marge;
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
  
  PSFooter(fd);
  fclose(fd);
} // ShowGraphe()
#endif

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  struct xvimage * prio;
  struct xvimage * prio2;
  struct xvimage * inhibimage = NULL;
  uint8_t *K;
  int32_t ret, priocode;
  int32_t rs, cs, N, i;
  float priomax_f;
  int32_t priomax_l;
  float * PRIO_F;
  int32_t * PRIO_L;
  graphe * flow;
  graphe * flow_s;
  graphe * forest;
  graphe * forest_s;
  boolean * perm;
  boolean * head;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm prio [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(k);
  cs = colsize(k);
  N = rs*cs;
  K = UCHARDATA(k);;

  ret = sscanf(argv[2], "%d", &priocode);
  if (ret == 0) // priorité : image 
  {
    prio2 = readimage(argv[2]);
    if (prio2 == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    if ((rowsize(prio2) != rs) || (colsize(prio2) != cs) || (depth(prio2) != 1))
    {
      fprintf(stderr, "%s: incompatible image sizes\n", argv[0]);
      exit(1);
    }
    if (datatype(prio2) == VFF_TYP_1_BYTE)
    {
      uint8_t *B = UCHARDATA(prio2);
      int32_t *L;
      int32_t x;
      prio = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
      if (prio == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", argv[0]);
        exit(1);
      }
      L = SLONGDATA(prio);
      for (x = 0; x < N; x++) L[x] = (int32_t)B[x];
      freeimage(prio2);
    }
    else if (datatype(prio2) == VFF_TYP_4_BYTE)
    {
      prio = prio2;
    }
    else if (datatype(prio2) == VFF_TYP_FLOAT)
    {
      prio = prio2;
    }
    else
    {
      fprintf(stderr, "%s: bad datatype for prio\n", argv[0]);
      exit(1);
    }
  }
  else  // priorité : carte de distance (à calculer)
  {
    int32_t i;
    prio = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
    if (prio == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    for (i = 0; i < N; i++) // inverse l'image
      if (K[i]) K[i] = 0; else K[i] = NDG_MAX;
  
    if (priocode == 0)
    {
      if (! ldisteuc(k, prio))
      {
        fprintf(stderr, "%s: ldisteuc failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priocode == 1)
    {
      if (! ldistquad(k, prio))
      {
        fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priocode == 2)
    {
      if (! lchamfrein(k, prio))
      {
        fprintf(stderr, "%s: lchamfrein failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priocode == 3)
    {
      if (! lsedt_meijster(k, prio))
      {
	fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      if (! ldist(k, priocode, prio))
      {
        fprintf(stderr, "%s: ldist failed\n", argv[0]);
        exit(1);
      }
    }
    for (i = 0; i < N; i++) // re-inverse l'image
      if (K[i]) K[i] = 0; else K[i] = NDG_MAX;
  }

  if (argc == 6) 
  {
    ret = sscanf(argv[4], "%f", &priomax_f);
    if (ret == 0) // inhibit : image
    {
      inhibimage = readimage(argv[3]);
      if (inhibimage == NULL)
      {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
      }
    }
  }
  else 
  {
    priomax_l = INT32_MAX; 
    priomax_f = FLT_MAX; 
  }

  if (datatype(prio) == VFF_TYP_4_BYTE)
  {
    PRIO_L = SLONGDATA(prio);
    if (! (flow = l2dtopoflow_l(k, prio, inhibimage, priomax_l)))
    {
      fprintf(stderr, "%s: function l2dtopoflow_l failed\n", argv[0]);
      exit(1);
    }
  }
  else
  if (datatype(prio) == VFF_TYP_FLOAT)
  {
    PRIO_F = FLOATDATA(prio);
    if (! (flow = l2dtopoflow_f(k, prio, inhibimage, priomax_f)))
    {
      fprintf(stderr, "%s: function l2dtopoflow_f failed\n", argv[0]);
      exit(1);
    }

    perm = (boolean *)calloc(N, sizeof(boolean)); assert(perm != NULL);
    head = (boolean *)calloc(N, sizeof(boolean)); assert(head != NULL);
    for (i = 0; i < N; i++)
      if (flow->v_sommets[i] == TF_PERMANENT)
	perm[i] = TRUE;
      else if (flow->v_sommets[i] == TF_HEAD)
	head[i] = TRUE;

#ifdef SHOWGRAPHS
    ShowGraphe(flow, "_flow1", 30, 1, 3, 20, 0, 0, 0, 1, K, rs, head);
#endif

    flow_s = Symetrique(flow);
    BellmanSCmax(flow_s);

    forest_s = ForetPCC(flow_s);
    
    forest = Symetrique(forest_s);

    for (i = 0; i < N; i++)
    { 
      forest->x[i] = flow->x[i];
      forest->y[i] = flow->y[i];
    }

#ifdef SHOWGRAPHS
    ShowGraphe(forest, "_forest", 30, 1, 3, 20, 0, 0, 0, 1, K, rs, head);
#endif
  }
  
  SaveGraphe(flow, argv[argc-1]);

  //  writeimage(k, argv[argc-1]);
  freeimage(k);
  free(perm);
  free(head);

  return 0;
} /* main */
