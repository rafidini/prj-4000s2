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
/* \file waterfall.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Jean Cousty
*/
/* 
NAME

<B>waterfall</B> - Waterfall transformation on edges

SYNOPSIS

<B>Lwshed</B> image.pgm GAout.pgm

DESCRIPTION

Performs the waterfall transformation on the edge-weighted graph (V,E,F) such that each pixel of <B>image.pgm</B> is a vertex of G, there is an edge beetween x and y in V whenever the corresponding pixel are 4-adjacent and for any (x,y) in E, F(x,y) is equal to the difference of intensity between the two corresponding pixels. The result is a salliency weighted GA F', such that the edge (x,y) disapear from the cuts at the iteration F'(x,y) of the waterfall algorithm.  
Types supported: byte 2D.

CLASS 

morph

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lhierarchie.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * ga;
  struct xvimage * image;
  if ((argc != 4)){
    fprintf(stderr, "usage: %s image.pgm type (0 gradient en entree, 1 image brute en entree) GAout.pgm\n", argv[0]);
    exit(1);
  }
  if ((image =readimage(argv[1])) == NULL ){
    fprintf(stderr, "%s: ne peut lire %s \n", argv[0], argv[1]);
    exit(1);
  }
  if( (ga = allocGAimage(argv[3], image->row_size , image->col_size, 
		       depth(image), VFF_TYP_GABYTE )) == NULL ){
    fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
    freeimage(image);
    exit(1);
  }
  
  if(!main_cascade(image, ga, atoi(argv[2]))){
    fprintf(stderr, "%s: main_cascade failed \n", argv[0]);
    freeimage(ga); freeimage(image);  
    exit(1);
  }
  writerawGAimage(ga,argv[3]);
  freeimage(ga); freeimage(image);
  return 0;
}
