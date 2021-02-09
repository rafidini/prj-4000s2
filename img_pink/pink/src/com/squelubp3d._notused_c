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
OBSOLETE - voir squelubp.c

/* \file squelubp3d.c

\brief homotopic skeleton of a 3D binary image guided by a priority function 

<B>Usage:</B> squelubp3d im.pgm imprio.pgm connex inhibit out.pgm

<B>Description:</B> 
Homotopic skeleton of a 3D binary image guided by a priority function.
The grayscale image <B>imprio.pgm</B> specifies the order in which simple
points should be deleted (the lower the level, the higher the priority).
The connectivity of the object is given by <B>connex</B>.
Points having the value <B>inhibit</B> will never be deleted.

<B>Types supported:</B> byte 3d

<B>Category:</B> topo
\ingroup  topo

\author Michel Couprie
*/
/* 

Squelette binaire ultime 3d guide par une image de priorites.
Michel Couprie -  juillet 2001

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points ayant une valeur specifiee VAL_INHIBIT ne seront
pas selectionnes.

On definit l'operateur Ultimate Thinning UT(F,P) : 
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que P[x] != VAL_INHIBIT 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsquelubp.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imageprio;
  int32_t connex;
  int32_t inhibit;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s im.pgm imprio.pgm connex inhibit fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  imageprio = readimage(argv[2]);
  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }   

  connex = atoi(argv[3]);
  if ((connex != 6) && (connex != 26))
  {
    fprintf(stderr, "%s : connex = <6|26>\n", argv[0]);
    exit(1);
  }

  inhibit = atoi(argv[4]);

  if (! lsquelubp3d(image, imageprio, connex, inhibit))
  {
    fprintf(stderr, "%s: lsquelubp3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(imageprio);

  return 0;
} /* main */
