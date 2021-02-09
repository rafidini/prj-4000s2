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
/* \file saliencyGa.c

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

<B>saliencyGA</B> - saliency on 4-connected edge-weighted graph

SYNOPSIS

<B>saliencyGA</B> GAin.pgm type[0 surface, 1 dynamic, 2 volume 3 alphaOmegaCC] GAOut.pgm [annexe.pgm]

DESCRIPTION

Compute an edge weighted graph which is the saliency of the input for
a give type of filter. Type may take a value in {0, 1, 2}, where 0
stands for area closing, 1 for dynamic closing and 2 for simple volume
closing. We warn that simple volume closing corresponds to the
saliency by area closing of the M-border watershed.

Compute an edge weighted graph which is the saliency of the input for
a give type of filter. Type may take a value in {0, 1, 2, 3}, where 0
stands for area closing, 1 for dynamic closing and 2 for simple volume
closing and 3 for alpha-omega-CC. We warn that simple volume closing
corresponds to the saliency by area closing of the M-border watershed.

In the case of alpha-omega-CC, the annexe.pgm is the original image from which the gradient GAin.pgm is computed.

Types supported: GA byte 2D.

CLASS 

connect

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
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
  struct xvimage * ga, *annexe;
  int32_t param;
  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s GAin.pgm type[0 surface, 1 dynamic, 2 volumeSimple, 3 alphaOmega] GAOut.pgm [annexe.pgm]\n", argv[0]);
    exit(1); 
  }
  ga = readGAimage(argv[1]);
  if (ga == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  fprintf(stderr,"Avant le calcul de la saillance\n");

  switch(atoi(argv[2]))
  {
  case 0: param = SURFACE; break;
  case 1: param = DYNAMIC; break;
  case 2: param = VOLUME; break; 
  case 3: param = OMEGA; break; 
  default: fprintf(stderr, "%s: Mauvais parametre\n",argv[0]); exit(0);
  }
  if (param == OMEGA) {
    annexe = readimage(argv[4]);
    if (annexe == NULL)
      {
	fprintf(stderr, "%s: readimage failed\n", argv[0]);
	exit(1);
      }
  } else 
    annexe = NULL;
  if (saliencyGa(ga, param, annexe) != 1)
  {
    fprintf(stderr, "%s: flowLPEAttributOpenning failed\n", argv[0]);
    exit(1);
  }
  fprintf(stderr,"Aprèes le calcul de la saillance\n");
  writerawGAimage(ga, argv[3]);
  freeimage(ga);  
  return 0;
}
