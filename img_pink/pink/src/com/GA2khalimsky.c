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

/*! \file GA2khalimsky.c

\brief Convert a 4-connected edge-weighted graph (a GA) into its
representation in the Khalimsky grid

<B>Usage:</B> GA2khalimsky GA.pgm type out.pgm \n

<B>Description:</B>

Convert a 4-connected (6-connected, in 3D) edge-weighted graph
(<B>graph.ga</B>) into its Khalimsky grid representation (depending on
the parameter <B>type</B>), stored as a pgm image
(<B>out.pgm</B>). The vertices of the graph are associated to the
square of the Khalimsky grid, and the edges of the graphs are
associated to the line segments.

If type = 0, then the closure of the set of weighted-edges is used
(usefull for representing contours).  

If type = 1, then the dual of the closure of the set of weighted edges
is used (usefull for representing regions).


<B>Types supported:</B> ga 2d, ga 3d

<B>Category:</B> GA
\ingroup GA

\author Jean Cousty
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lga2khalimsky.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * ga;
  struct xvimage * out;
  int32_t bar;                    /* booleen indiquant si l'on traite un objet ou un complementaire */
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s GA.pgm type (0 pour un objet ou 1 pour un complementaire) out.pgm \n", 
            argv[0]);
    exit(1);
  }
  
  if( (ga = readGAimage(argv[1])) == NULL )
  {
     fprintf(stderr, "%s : ne peut lire %s \n", 
	     argv[0], argv[1]);
     exit(1);
  }
  if(datatype(ga) != VFF_TYP_GABYTE)
  {
    fprintf(stderr, "%s : mauvais format pour %s \n", 
            argv[0], argv[1]);
    freeimage(ga);
    exit(1);
  }
  bar = atoi(argv[2]);
  if(depth(ga) == 1){
    out = allocimage(argv[3], rowsize(ga)*2, colsize(ga)*2, 1, VFF_TYP_1_BYTE);
    if(!lga2khalimsky(ga, out, bar))
    {
      fprintf(stderr, "%s : echec de lga2khalimsky \n", 
	      argv[0]);
      freeimage(ga); freeimage(out);
      exit(1);
    }
  }
  else
  {
    out = allocimage(argv[3], rowsize(ga)*2, colsize(ga)*2, depth(ga)*2, VFF_TYP_1_BYTE);
    if(!lga2khalimsky3d(ga, out, bar))
    {
      fprintf(stderr, "%s : echec de lga2khalimsky \n", 
	      argv[0]);
      freeimage(ga); freeimage(out);
      exit(1);
    }
  }
  writeimage(out, argv[3]);
  freeimage(ga); freeimage(out);
  return 0;
}
