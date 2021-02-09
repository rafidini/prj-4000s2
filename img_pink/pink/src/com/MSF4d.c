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
/* \file MSE.c

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

<B>MSE</B> - Compute a minimum spanning extension of an image from a
set of markers

SYNOPSIS

<B>MSE4d</B> GAin.pgm MarqueurPrefix first last TypeResult
LabelImageResult

DESCRIPTION

Compute a minimum spanning forest relative to the opening of the the
set of markers (vertices) in <B>ImageMarqueur</B> in the edge-weighted
graph <B>GAin.pgm</B>, as defined in ??. The marker sequence should be
a sequence of 3D byte images formated MarqueurPrefixXXXX.pgm. If
TypeResult is set to 0 then a 4DGA is outputed, otherwise if
TypeResult = 1 a sequence of 3D byte label images are outputed.

Types supported: byte 4D.

CLASS 

connect

*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lMSF.h>


/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct GA4d * ga;
  struct xvimage4D * marqueurs;

  /* A priori il n'y a (quasi?) rien à modifier pour passer à des
     marqueurs codés en uint32_t */

  if ((argc != 7))
  {
    fprintf(stderr, "usage: %s GAin.pgm MarqueurPrefix first last TypeResult LabelImageResult \n TypeResult = 0 result is GA otherwise it is a label image\n", argv[0]);
    exit(1);
  }
  if( (ga = readGA4d(argv[1])) == NULL)
  {
    fprintf(stderr, "%s: readGA4d failed\n", argv[0]);
    exit(1);
  }
 
  if( (marqueurs = readimage4D(argv[2], atoi(argv[3]), atoi(argv[4]))) == NULL)
  {
    fprintf(stderr, "%s: readimage4d failed\n", argv[0]);
    exit(1);
  } 
  
  if (! MSF4d(ga,marqueurs)){
    fprintf(stderr, "%s: MSF4d failed\n", argv[0]);
    exit(1);
  }
  printf("Calcule de la MSE effectué \n");
  /* Il faut surement tester la valeur de retour de writeimage() */
  if(atoi(argv[3]) == 0)
    writeGA4d(ga,argv[4]); 
  else 
    writeimage4D(marqueurs, argv[argc - 1], atoi(argv[3]), atoi(argv[4]));
  
  freeGA4d(ga); freeimage4D(marqueurs); 
  return 0;
}
