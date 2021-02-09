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
/*! \file ptWsimple.c

\brief detects W-simple points in a 2D binary image

<B>Usage:</B> ptWsimple in.pgm connex out.pgm

<B>Description:</B>
The argument \b connex selects the connectivity (4, 8 in 2D).

<B>Types supported:</B> byte 2d

<B>Category:</B> connect
\ingroup  connect

\author Jean Cousty (2007)
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>
#include <llabelextrema.h>

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  int32_t nblabels, connex, i,y,k;
  int32_t rs, cs, d, n, N, label;
  uint8_t *G;
  int32_t *LABEL;
  struct xvimage * image;
  struct xvimage * result;
  struct xvimage * result2;
  int32_t function;
  int32_t incr;
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm\n", argv[0]);
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
  n = rs * cs;          /* taille plan */
  N = n * d;            /* taille image */

  if(d !=1){
    fprintf(stderr,"Cette version ne traite que les images 2D\n");
    freeimage(image);
    exit(0); 
  }
  connex = atoi(argv[2]);
  if( (connex != 4) && (connex != 8)){
    fprintf(stderr,"Connexité inconnue en 2D\n");
    freeimage(image);
    exit(0);
  }
  if(connex == 4) incr = 2; else incr = 1;
  function = LABMIN; 
  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE); 
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! llabelextrema(image, connex, function, result, &nblabels))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
    exit(1);
  }
  
  result2 = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
  
  LABEL = SLONGDATA(result);
  G = UCHARDATA(result2);
  memset(G,0,N);
  for(i = 0; i < N; i++){
    if(LABEL[i] == 0){ // le points appartient à l'objet X
      G[i] = 255;
      label = -1;
      for(k = 0; k < 8; k += incr){
	y = voisin(i, k, rs, N);
	if (y != -1)
	{
	  if(LABEL[y] > 0) // y appartient à \Bar{X}
	  {
	    if(label != -1) 
	    {
	      if(label != LABEL[y]) G[i] = 0;
	    } 
	    else 
	      label = LABEL[y];
	  }
	}
      }//for(k = 0 ... )
    }// if(LABEL[i] ...)
  }// for(i = 0...)
  
  writeimage(result2, argv[argc-1]);
  freeimage(result);
  freeimage(result2);
  freeimage(image);

  return 0;
} /* main */


