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
/*! \file delaunay.c

\brief delaunay triangulation

<B>Usage:</B> delaunay in.list [mask.pgm] out.list

<B>Description:</B>
Reads a point list in file <B>in.list</B> under the following format:
<pre>  
    b &lt;n&gt;         n &lt;n&gt;    
    x1 y1         x1 y1 v1
    x2 y2   ou    x2 y2 v2
    ...           ...
    xn yn         xn yn vn
</pre>  

Computes a Delaunay triangulation and stores the resulting graph 
into file <B>out.graph</B> under the following format:
<pre>
    G &lt;n&gt;
    x1 y1 v1 ec1 ns1 s11 s12 ... s1ns1
    x2 y2 v2 ec2 ns2 s21 s22 ... s1ns2
    ...
    xn yn vn ecn nsn sn1 sn2 ... s1nsn

</pre>  
where xi, yi are the coordinates of the ith vertex, vi is the associated 
value (if given in the input file), eci is a int32_t which indicates 
whether the vertex i belongs to the convex hull, nsi denotes the number
of adjacent vertices, and si1 si2 ... sins1 is the list of the indexes of
the adjacent vertices (counted from 0).

If the optional parameter <B>mask.pgm</B> is given, then it must be a 
binary byte image. Any edge of the triangulation which intersects the
mask will be discarded.  

<B>Types supported:</B> byte 2D

<B>See also:</B> drawtriangulation.c

<B>Category:</B> geo
\ingroup  geo

\warning algorithm in O(n^2)

\author Michel Couprie, Laurent Mercier
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <lvoronoi.h>
#include <lbresen.h>
#define isnotinmask(x,y) (!M[y*rss+x])

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t n, i, j, k, rss, css, N;
  double xx, yy, vv;
  char type;
  mcgeo_point * S;
  double * V;
  int32_t **v; /* la table des listes de voisins (reperes par leur indice dans S) */
  int32_t *nv; /* la table des nombres de voisins */
  int32_t *ec; /* table de booleens indiquant les points de l'enveloppe convexe */
  struct xvimage * mask;	
  int32_t Cx, Cy, Dx, Dy, vois,longlpmax;
  uint8_t *M;
  int32_t npoints;
  int32_t newnv;
  int32_t *del, *lx, *ly;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.list [mask.pgm] out.graph\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type != 'b') && (type != 'n'))
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  S = (mcgeo_point *)calloc(1,n * sizeof(mcgeo_point));
  V = (double *)calloc(1,n * sizeof(double));

  if (type == 'b') 
  {
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf\n", &xx, &yy);
      S[i].x = xx;
      S[i].y = yy;
      V[i] = 255.0;
    }
  }
  else
  {
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf %lf\n", &xx, &yy, &vv);
      S[i].x = xx;
      S[i].y = yy;
      V[i] = vv;
    }
  }
  fclose(fd);

  if (!ldelaunay(S, n, &v, &nv, &ec))
  {
    fprintf(stderr, "%s : ldelaunay failed \n", argv[0]);
    exit(1);
  }

  if (!ltriang_delaunay(S, n, v, nv, ec))
  {
    fprintf(stderr, "%s : ltriang_delaunay failed \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[argc-1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[3]);
    exit(1);
  }

  if (argc == 3)
  {
    fprintf(fd, "G %d\n", n);
    for (i = 0; i < n; i++)
    {
      fprintf(fd, "%g %g %g %d %d ", S[i].x, S[i].y, V[i], ec[i], nv[i]);
      for (j = 0; j < nv[i]; j++) fprintf(fd, "%d ", v[i][j]);
      fprintf(fd, "\n");
    }
    fclose(fd);
    return 0;
  }
  // else (argc == 4)
  mask = readimage(argv[2]);
  if (mask == NULL)
  {
    fprintf(stderr, "%s: lecture mask échoué\n", argv[0]);
    exit(1);
  }   
  M = UCHARDATA(mask);
  rss = mask->row_size;
  css = mask->col_size;
  N = rss * css; 
  longlpmax=mcmax(rss,css);
  lx = (int32_t*)malloc(longlpmax*sizeof(int32_t));
  ly = (int32_t*)malloc(longlpmax*sizeof(int32_t));
  del = (int32_t*)malloc(longlpmax*sizeof(int32_t));
  npoints = 0;

  fprintf(fd, "G %d\n", n);

  for (i = 0; i < n; i++)
  {
    Cx = arrondi(S[i].x);
    Cy = arrondi(S[i].y);
    if (isnotinmask(Cx,Cy))
    {
      fprintf(fd, "%g %g %g %d ", S[i].x, S[i].y, V[i], ec[i]);	
      newnv=nv[i];
      for (k = 0; k < nv[i] ; k++) del[k]=0;
      for (j = 0; j < nv[i]; j++) 
      {
	vois = v[i][j];
	Dx = arrondi(S[vois].x);
	Dy = arrondi(S[vois].y);
	npoints = longlpmax;
	lbresenlist(Cx, Cy, Dx, Dy, lx, ly, &npoints);
	for (k = 0; k < npoints; k++)
	{
	  if (M[ly[k]*rss + lx[k]]) // si le point du segment est dans le masque
	  {
	    del[j]=1;
	    newnv--;
	    break;
	  }
	}
      }
      fprintf(fd, "%d ", newnv);
      for (k = 0; k < nv[i]; k++)
      {
	if (del[k]==0)
	  fprintf(fd, "%d ", v[i][k]);
      }
      fprintf(fd, "\n");
    }
    else
      fprintf(fd, "%g %g %g %d %d\n", S[i].x, S[i].y, V[i], ec[i], 0);	
  }
  fclose(fd);
  for (i = 0; i < n; i++) free(v[i]);
  free(v);
  free(nv);
  free(ec);
  free(lx);
  free(ly);
  free(del);
  free(S);
  free(V);
  return 0;
}
