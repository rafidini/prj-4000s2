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
/*! \file graph2pgm.c

\brief converts from graph representation to pgm

<B>Usage:</B> graph2pgm in.graph {in.pgm|rs cs ds} out.pgm

<B>Description:</B>

Reads the file <B>in.graph</B>. Each vertex of this graph must have integer coordinates, and 
represents a pixel/voxel of a 2D/3D image.
If a file name <B>in.pgm</B> is given, then the points read in <B>in.graph</B> are 
inserted (if possible) into the image read in <B>in.pgm</B>. Else, they are inserted in
a new image, the dimensions of which are <B>rs</B>, <B>cs</B>, and <B>ds</B>.

<B>Types supported:</B> graph

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - juillet 2009
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcgraphe.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rs, cs, ds, ps, N, i;
  char type;
  graphe *g;

  if ((argc != 4) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.graph {in.pgm|rs cs ds} out.pgm\n", argv[0]);
    exit(1);
  }

  g = ReadGraphe(argv[1]);
  if (g == NULL)
  {
    fprintf(stderr, "%s: ReadGraphe failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    image = readimage(argv[2]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    rs = rowsize(image);
    cs = colsize(image);
    ds = depth(image);
    ps = rs * cs;
    N = ps * ds;
    if (N != g->nsom)
    {
      fprintf(stderr, "%s: nb. of vertices must be equal to nb. of image points\n", argv[0]);
      exit(1);
    }
    type = datatype(image);
  }
  else
  {
    rs = atoi(argv[2]);
    cs = atoi(argv[3]);
    ds = atoi(argv[4]);
    ps = rs * cs;
    N = ps * ds;
    if (N != g->nsom)
    {
      fprintf(stderr, "%s: nb. of vertices must be equal to nb. of image points\n", argv[0]);
      exit(1);
    }
    type = VFF_TYP_1_BYTE;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
  }

  switch(type)
  {
    case VFF_TYP_1_BYTE:
    {
      uint8_t *F = UCHARDATA(image);
      for (i = 0; i < N; i++)  
        F[i] = (uint8_t)(g->v_sommets[i]); 
      break;
    }
    case VFF_TYP_4_BYTE:
    {
      int32_t *F = SLONGDATA(image);
      for (i = 0; i < N; i++)  
        F[i] = (uint8_t)(g->v_sommets[i]); 
      break;
    }
    case VFF_TYP_FLOAT:
    {
      float *F = FLOATDATA(image);
      for (i = 0; i < N; i++)  
        F[i] = (float)(g->v_sommets[i]); 
      break;
    }
    default:
    {
      fprintf(stderr, "%s: bad data type %d\n", argv[0], type);
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return 0;
}

