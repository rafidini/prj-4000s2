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
/*! \file diZenzo.c

\brief diZenzo gradient pour les images couleurs

<B>Usage:</B> diZenzo imageRVB.ppm alpha [mode] out.pgm

<B>Description:</B>
Le gradient de diZenzo est défini par la donnée de p, q, et t:

p = Rx*Rx + Vx*Vx + Bx*Bx

q = Ry*Ry + Vy*Vy + By*By

t = Rx*Ry + Vx*Vy + Bx*By

(ou Rx = dérivée en x de la bande rouge, Ry est la dérivée en y de la bande rouge, etc.)

et le module est donnée par

G = sqrt(1/2*(p+q+sqrt((p+q)*(p+q) -4(pq-t*t))))

La direction est donnée par 1/2*atan(2*t/(p-q))

Si le mode est égale à 0 (valeur défaut) alors l'image de sortie est le gradient, 
sinon l'image de sortie est une int32_t entre 0 et 360.

Les gradients sont calculés par les filtres de Deriche, de paramètre alpha

<B>Types supported:</B> byte 2D

<B>Category:</B> signal
\ingroup  signal

\author Laurent Najman
*/
/* Gradient couleur de diZenzo basé sur le filtre lineaire general recursif de Deriche */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldiZenzo.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * imageR;
  struct xvimage * imageV;
  struct xvimage * imageB;
  struct xvimage * result;
  double alpha;
  int32_t mode;

  if ((argc < 4) || (argc > 5))
  {
    fprintf(stderr, "usage: %s inRVB.ppm alpha [mode] out.pgm \n", 
            argv[0]);
    exit(1);
  }

  if (readrgbimage(argv[1], &imageR, &imageV, &imageB) == 0)
  {
    fprintf(stderr, "diZenzo: readrgbimage failed\n");
    exit(1);
  }

  alpha = atof(argv[2]);
  if (argc == 5) {
    mode = atoi(argv[3]);
  } else mode = 0;
  
  if (mode == 0) {
    if (! ldiZenzoGradient(imageR, imageV, imageB, alpha))
      {
	fprintf(stderr, "deriche: function lderiche failed\n");
	exit(1);
      }
    writeimage(imageR, argv[argc-1]);
  } else {
    result = allocimage(NULL, rowsize(imageR), colsize(imageR), depth(imageR), VFF_TYP_4_BYTE);
    if (result == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    ldiZenzoDirection(imageR, imageV, imageB, result, alpha);
    writeimage(result, argv[argc-1]);
    freeimage(result);
  }


  freeimage(imageR);
  freeimage(imageV);
  freeimage(imageB);

  return 0;
} /* main */


