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
/* \file recons.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* operateur de reconstruction binaire */
/* 
   entrees : img, une image en niveaux de gris
             seg, une image binaire (pre-segmentation topologiquement correcte,
                                     sur-ensemble de la segmentation desiree)
             connex, 4 ou 8
   sortie : seg, la segmentation (binaire) desiree
*/
/* Michel Couprie -  avril 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lrecons.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * img;
  struct xvimage * seg;
  int32_t connex;
  int32_t memoire;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s img.pgm seg.pgm connex mem out.pgm \n", argv[0]);
    exit(1);
  }

  img = readimage(argv[1]);
  seg = readimage(argv[2]);
  if ((img == NULL) || (seg == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8))
  {   
    fprintf(stderr, "%s: parametre connex = [4|8]\n", argv[0]);
    exit(1);
  }

  memoire = atoi(argv[4]);
  if ((memoire != 0) && (memoire != 1))
  {   
    fprintf(stderr, "%s: parametre mem = [0|1]\n", argv[0]);
    exit(1);
  }

  if (! lrecons(img, seg, connex, memoire))
  {
    fprintf(stderr, "recons: function lrecons failed\n");
    exit(1);
  }

  writeimage(seg, argv[5]);
  freeimage(img);
  freeimage(seg);

  return 0;
} /* main */
