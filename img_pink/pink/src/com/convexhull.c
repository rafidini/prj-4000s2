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
/*! \file convexhull.c

\brief convex hull of a set of points in the 2D plane

<B>Usage:</B> convewhull in.pgm mode out

<B>Description:</B> Computes the convex hull of the set of non-null points
in the image <B>in.pgm</B>, by the Jarvis's algorithm as described in 
"Introduction to algorithms", T. Cormen, C. Leiserson, R. Rivest, MIT Press.

The parameter \b mode selects the format of the result: 
\li 0: list of the support points of the convex hull
\li 1: support points of the convex hull, embedded in the image frame
\li 2: contour of the convex hull, embedded in the image frame
\li 3: full convex hull, embedded in the image frame

<B>Types supported:</B> byte 2D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/* 
  Enveloppe convexe - algorithme de Jarvis (paquet-cadeau)

  D'apres "Introduction a l'algorithmique", 
    T. Cormen, C. Leiserson, R. Rivest, pp. 890, Dunod Ed. 

  Michel Couprie - avril 1998
*/


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lconvexhull.h>
#include <ldraw.h>
#include <lgeodesic.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t * listepoints;
  int32_t npoints;
  int32_t i, rs, cs, N, mode;
  uint8_t *F;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm mode out (mode : 0=point list, 1=points, 2=contour, 3=full)\n", argv[0]);
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
  N = rs * cs;
  F = UCHARDATA(image);

  //  POUR ACCELERER: RETIRER LES POINTS INTERIEURS

  if (! lconvexhull(image, &listepoints, &npoints))
  {
    fprintf(stderr, "%s: function lconvexhull failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[2]);
  if ((mode < 0) || (mode > 3))
  {
    fprintf(stderr, "%s: bad value for mode\n", argv[0]);
    fprintf(stderr, "usage: %s in.pgm mode out (mode : 0=point list, 1=points, 2=contour, 3=full)\n", argv[0]);
    exit(1);
  }

  if (mode == 0)
  {
    FILE * fd = NULL;
    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[3]);
      exit(1);
    }
    fprintf(fd, "b %d\n", npoints);
    for (i = 0; i < npoints; i++)
      fprintf(fd, "%d %d\n", listepoints[i]%rs, listepoints[i]/rs);
    fclose(fd);
  }
  else if (mode == 1)
  {
    writeimage(image, argv[argc-1]);
  }
  else if (mode == 2)
  {
    for (i = 0; i < npoints-1; i++)
    {
      ldrawline(image, listepoints[i]%rs, listepoints[i]/rs, 
                        listepoints[i+1]%rs, listepoints[i+1]/rs);
    }    
    ldrawline(image, listepoints[npoints-1]%rs, listepoints[npoints-1]/rs, 
                      listepoints[0]%rs, listepoints[0]/rs);
    writeimage(image, argv[argc-1]);
  }
  else if (mode == 3)
  {
    struct xvimage * mark = copyimage(image);
    uint8_t *G = UCHARDATA(mark);
    memset(G, 0, N);
    for (i = 0; i < npoints-1; i++)
    {
      ldrawline(image, listepoints[i]%rs, listepoints[i]/rs, 
                        listepoints[i+1]%rs, listepoints[i+1]/rs);
    }    
    ldrawline(image, listepoints[npoints-1]%rs, listepoints[npoints-1]/rs, 
                      listepoints[0]%rs, listepoints[0]/rs);
    for (i = 0; i < rs; i++) if (!F[i]) G[i] = NDG_MAX; // construit cadre
    for (i = 0; i < rs; i++) if (!F[(cs - 1) * rs + i]) G[(cs - 1) * rs + i] = NDG_MAX;
    for (i = 1; i < cs - 1; i++) if (!F[i * rs]) G[i * rs] = NDG_MAX;
    for (i = 1; i < cs - 1; i++) if (!F[i * rs + rs - 1]) G[i * rs + rs - 1] = NDG_MAX;
    for (i = 0; i < N; i++) F[i] = NDG_MAX - F[i]; // inverse image
    if (! lgeodilat(mark, image, 4, -1))
    {
      fprintf(stderr, "%s: function lgeodilat failed\n", argv[0]);
      exit(1);
    }
    for (i = 0; i < N; i++) F[i] = NDG_MAX - G[i];    
    writeimage(image, argv[argc-1]);
    free(mark);
  }

  free(listepoints);
  freeimage(image);

  return 0;
} /* main */


