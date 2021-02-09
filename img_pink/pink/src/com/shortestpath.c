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
/*! \file shortestpath.c

\brief finds a shortest path from x to y in image 

<B>Usage:</B> shortestpath in.pgm connex mode S1 S2 [S3] D1 D2 [D3] out.lst

<B>Description:</B>
A weighted graph G = (V,E) is defined with V = set of pixels of \b in.pgm 
and E = {(P,Q) in VxV ; P and Q are \b connex- adjacent}. 
Let F(P) be the value of pixel P in the image \b in.pgm. 
A weight W(P,Q) is assigned to each edge, according to the value of \b mode:

\li max : W(P,Q) = max{F(P),F(Q)} 
\li min : W(P,Q) = min{F(P),F(Q)} 
\li avg : W(P,Q) = (F(P) + F(Q)) / 2 

This operator finds a shortest path from (\b S1, \b S2) to (\b D1, \b D2)
in this graph. The result is given as a list of vertices (pixels) 
in \b out.lst.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgraphe.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, mode, S1, S2, S3, D1, D2, D3;
  int32_t debut, fin;
  int32_t rs, cs, ds, ps;
  graphe * G;
  int32_t n, y, x;
  pcell p;
  FILE *fd = NULL;

  if ((argc != 9) && (argc != 11))
  {
    fprintf(stderr, "usage: %s in.pgm connex mode S1 S2 [S3] D1 D2 [D3] out.lst\n", argv[0]);
    fprintf(stderr, "mode = <min|max|avg>\n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;

  connex = atoi(argv[2]);

  if ((argc == 11) && (ds == 1))
  {
    fprintf(stderr, "%s: 3D coordinates and 2D image\n", argv[0]);
    exit(1);
  }

  if ((argc == 9) && (ds > 1))
  {
    fprintf(stderr, "%s: 2D coordinates and 3D image\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[3], "avg") == 0) mode = SP_AVG; else
  if (strcmp(argv[3], "min") == 0) mode = SP_MIN; else
  if (strcmp(argv[3], "max") == 0) mode = SP_MAX; else
  {
    fprintf(stderr, "mode = <min|max|avg>\n");
    exit(1);
  }

  if (argc == 10)
  {
    S1 = atoi(argv[4]);
    S2 = atoi(argv[5]);
    S3 = atoi(argv[6]);
    D1 = atoi(argv[7]);
    D2 = atoi(argv[8]);
    D3 = atoi(argv[9]);
    debut = S3*ps + S2*rs + S1;
    fin = D3*ps + D2*rs + D1;
  }
  else
  {
    S1 = atoi(argv[4]);
    S2 = atoi(argv[5]);
    D1 = atoi(argv[6]);
    D2 = atoi(argv[7]);
    debut = S2*rs + S1;
    fin = D2*rs + D1;
  }

  G = Image2Graphe(image, mode, connex);

  Dijkstra(G, fin);

  //  AfficheSuccesseurs(G);

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  
  x = debut;
  n = 1; // pour le point debut
  while (x != fin)
  {
    for (p = G->gamma[x]; p != NULL; p = p->next)
    { /* pour tout y voisin de x */
      y = p->som;
      if ((G->v_sommets[x]-G->v_sommets[y]) == p->v_arc) 
      {
        n++;
        x = y;
        break;
      }
    } // for p
    if (p == NULL)
    {
      fprintf(stderr, "%s: no shortest path found\n", argv[0]);
      exit(1);
    }
  }

  fprintf(fd, "b %d\n", n); 
  
  x = debut;
  fprintf(fd, "%d %d\n", x%rs, x/rs);
  while (x != fin)
  {
    for (p = G->gamma[x]; p != NULL; p = p->next)
    { /* pour tout y voisin de x */
      y = p->som;
      if ((G->v_sommets[x]-G->v_sommets[y]) == p->v_arc) 
      {
	fprintf(fd, "%d %d\n", y%rs, y/rs);
        x = y;
        break;
      }
    } // for p
  }

  fclose(fd);
  freeimage(image);
  TermineGraphe(G);

  return 0;
} /* main */


