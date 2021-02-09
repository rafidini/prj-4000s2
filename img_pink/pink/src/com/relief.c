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
/* \file relief.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Sauvegarde dans un fichier le profil 3D d'une portion d'image */
/* inscrite dans une fenetre autour d'un point de coord. x,y     */
/* entree: image x y taille_fenetre nom_fichier_sortie           */
/* sortie: fichier du profil 3D (coord. x y z)                   */

/* Regis VINCIGUERRA - mai 98 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>  

int main(int argc, char **argv)
{
  struct xvimage * image;
  uint8_t *I;
  int32_t rs, cs, N;
  int32_t xp, yp;
  int32_t x,y;
  int32_t tf;
  FILE *profil;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in1.pgm xmax ymax taille_fenetre out \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  I = UCHARDATA(image);
  rs = rowsize(image);
  cs = colsize(image);
  N = rs * cs;
  
  xp = atoi(argv[2]);
  yp = atoi(argv[3]);
  tf = atoi(argv[4])/2;
  profil = fopen(argv[5],"w");

  for (y=yp-tf;y<yp+tf;y++)
    for (x=xp-tf;x<xp+tf;x++)
      if ((y>=0) && (y<=cs) && (x>=0) && (x<=rs))
	fprintf(profil,"%d %d %d\n",x,y,I[x+y*rs]);
  
  fclose(profil);
  freeimage(image);
  
  return 0;
} /* main */
