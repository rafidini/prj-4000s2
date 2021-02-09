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

/*! \file GA2pgm.c

\brief Convert a 4-connected edge-weighted graph (a GA) into a pgm image 

<B>Usage:</B> GA2pgm graph.ga param out.pgm \n

<B>Description:</B>

Convert a 4-connected edge-weighted graph (<B>graph.ga</B>) into a pgm
image (<B>out.pgm</B>) where each pixel represents a vertex of the
input graph and the gray level of a pixel is obtained from the values
of its incident edges by means of an operation depending of the
parameter <B>param</B>.

If param = 1, then max is used, and if param = 2, min is used.

<B>Types supported:</B> ga 2d, ga 3d

<B>Category:</B> GA
\ingroup GA

\author Jean Cousty
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lppm2GA.h>

/* =============================================================== */
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * im;
  struct xvimage * ga;
  int32_t param;
  if(argc != 4) {
    fprintf(stderr, "usage: %s ga.ga param out.pgm \n param = 1 max des sommets contenus dans l'arete, param = 2 min des sommets contenus dans l'aretes\n", 
            argv[0]);
    exit(1);
  }
  param = atoi(argv[2]);
  if( (param!= 1) && (param != 2)){
    fprintf(stderr, "Bad parameter param\nusage: %s ga.ga param out.pgm \n param = 1 max des sommets contenus dans l'arete, param = 2 min des sommets contenus dans l'aretes\n", 
            argv[0]);
    exit(1);
  }

  ga = readGAimage(argv[1]);
  if (ga == NULL) {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if( (im = allocimage(argv[2], ga->row_size , ga->col_size, depth(ga), VFF_TYP_1_BYTE )) == NULL ) {
    fprintf(stderr, "%s: ne peut pas allouer d'image de sortie \n", argv[0]);
    freeimage(ga);
    exit(1);
  }
  if(depth(im) == 1) {
    
    if( lga2pgm(im,ga,param) != 1 ) {
      fprintf(stderr, "%s: erreur de lga2pgm \n", argv[0]);
      exit(1);
    }
  } else {
    if( lga2pgm3d(im,ga,param) != 1 ) {
      fprintf(stderr, "%s: erreur de lga2pgm3d \n", argv[0]);
      exit(1);
    }
  }
  
  writerawimage(im, argv[3]); 
  freeimage(im); 
  freeimage(ga);
  return 0;
}
