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

/*! \file pgm2GA.c

\brief Computes an edge-weighted graph from an image

<B>Usage:</B> pgm2GA im.pgm param [alpha] out.ga \n

<B>Description:</B>

Computes a GA (an edge-weighted graph) from an image. The values of an
edge linking two pixels {x,y} is computed according to the parameter
<B>param</B>.

If param = 0, the absolute difference of intensity between x and y is used.

If param = 1, the maximum between the intensities of x and y is used.

If param = 2, the minimum between the intensities of x and y is used.

If param = 3, a Deriche-like gradient is used, the optional parameter
alpha specifies the spatial extention of the filter (by default alpha
is set to  1]),

If im.pgm is a 2D (resp. 3D) image, then <B>out.ga</B> is a 2D (resp
3D GA), that is a 2D 4-connected edge-weighted graph (resp. a 2D
6-connected edge-weighted graph).


<B>Types supported:</B> GA byte 2D, GA byte 3D, GA float 2D

<B>Category:</B> 
\ingroup  GA

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
int main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  double alpha;
  struct xvimage * im;
  struct xvimage * ga;
  int32_t param;
  if ( (argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s im.pgm param [alpha] out.ppm \nparam = 0 difference entre \
pixel extremite, param = 1 max entre pixel extremite, param = 2 min \
entre pixel extremite, param = 3 Deriche default for alpha is 1,\n", 
            argv[0]);
    exit(1);
  }
  if ((im =readimage(argv[1])) == NULL )
  {
    fprintf(stderr, "%s: ne peut lire %s \n", argv[0], argv[1]);
    exit(1);
  }
  //printf("taille de l'image 3d: %d\n", depth(im));
  printf("Datatype %d (et devrait etre %d ou %d)\n",datatype(im), VFF_TYP_FLOAT,VFF_TYP_1_BYTE);
  switch(datatype(im)){
  case VFF_TYP_1_BYTE:
    if( (ga = allocGAimage(argv[2], im->row_size , im->col_size, depth(im), VFF_TYP_GABYTE )) == NULL )
    {
      fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
      freeimage(im);
      exit(1);
    }
    break;
  case VFF_TYP_FLOAT:
    if( (ga = allocGAimage(argv[2], im->row_size , im->col_size, depth(im), VFF_TYP_GAFLOAT )) == NULL )
    {
      fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
      freeimage(im);
      exit(1);
    }
    break;
  default: fprintf(stderr,"Ne traite pas ce type d'image\n");
    exit(0);
  }
  param = atoi(argv[2]);

  if(argc == 5)
    alpha = atof(argv[3]);
  else alpha = 1;
  if(depth(im) == 1)
    {
      switch(datatype(im)){
      case VFF_TYP_1_BYTE:
	//fprintf(stderr,"Type de l'image vaut 1\n");
	if( lpgm2ga(im, ga, param, alpha) != 1 )
	{
	  fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
	  exit(1);
	}
	break;
      case VFF_TYP_FLOAT:
	if( lpgm2gafloat(im, ga, param, alpha) != 1 )
	{
	  fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
	  exit(1);
	}
	break;
      }
    }
  else
    if( lpgm2ga3d(im,ga,param) != 1 )
    {
      fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
      exit(1);
    }
  if(argc == 5) 
    writerawGAimage(ga, argv[4]);
  else
    writerawGAimage(ga, argv[3]); 

  freeimage(im); 
  freeimage(ga);
  return 0;
}
