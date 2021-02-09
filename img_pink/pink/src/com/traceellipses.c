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
/* \file traceellipses.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  Trace des ellipses dont les specifications 
  (coord centre, extremite grand axe (coord. relatives au centre), extremite petit axe (idem)) 
  sont lues dans un fichier.
  Format du fichier: 
  <nb ellipse>
  <ell1>
  .
  .
  .


  Utilise l'algorithme de Bresenham generalise (voir lbresen)

  Michel Couprie - nov. 1998
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lbresen.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t i, x1, y1, x2, y2, xc, yc, x, y;
  uint8_t *Im;
  int32_t rs, cs, d, N;
  FILE *fd = NULL;
  int32_t nellipse;
  ellipse *ell;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm ellipses.txt [fill] out.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 5) && strcmp(argv[3], "fill"))
  {
    fprintf(stderr, "usage: %s in.pgm ellipses.txt [fill] out.pgm\n", argv[0]);
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
  d = depth(image);
  if (d > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }
  N = rs * cs;
  Im = UCHARDATA(image);

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  ell = AllocEllipseList(rs, cs);
  
  fscanf(fd, "%d", &nellipse);

  for (i = 0; i < nellipse; i++)
  {
    fscanf(fd, "%d%d%d%d%d%d", &xc, &yc, &x1, &y1, &x2, &y2);
    if (argc == 4)
      lellipse(Im, rs, cs, x1, y1, x2, y2, xc, yc);
    else
    {
      lellipselist(ell, rs, cs, x1, y1, x2, y2, xc, yc);
      for (y = 0; y < cs; y++) 
      {
        for (x = ell->xmin[y]; x <= ell->xmax[y]; x++)
          Im[y*rs + x] = NDG_MAX;
      }
    }
  }
  fclose(fd);
  FreeEllipseList(ell);
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

