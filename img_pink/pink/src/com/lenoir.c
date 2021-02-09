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
/* 

\brief Calcul d'une géodésique dans un espace discret (2D ou 3D)

<B>Usage:</B> Lenoir in.pgm source_point_x source_point_y [source_point_z] target_point_x target_point_y [target_point_z] connex out.pgm

<B>Description:</B> Calcul de la géodésique entre le point source et le point target

<B>Types supported:</B> byte 3d

<B>Category:</B> 
\ingroup  

\author Massieu Julien
\author Parly Sébastien
*/
/* Massieu Julien & Parly Sébastien - Février 2005 */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include "llenoir.h"

int main(int argc, char **argv) 
{
	struct xvimage *image, *out; //xvimages
	int32_t xb,yb,zb,xe,ye,ze;
	int32_t x,y;//Points entre lesquels on applique l'algorithme Lenoir1
	int32_t rs, cs, ds, connex; //row-size col-size depth_size et connexité

	if ((argc != 8) && (argc != 10))
	{
	  fprintf(stderr, "usage: %s in.pgm xb yb [zb] xe ye [ze] connex out.pgm \n", argv[0]);
	  exit(0);
  	}

	if (argc == 8)
	  connex = atoi(argv[6]);
	else connex = atoi(argv[8]);

	if ((argc == 8) && ( (connex != 4) && (connex != 8))) {
	  fprintf(stderr, "%s: connexity for 2D images should be 4 or 8\n", argv[0]);
	  exit(0);
	}
	if ((argc == 10) && ( (connex != 6) && (connex != 18) && (connex != 26))) {
	  fprintf(stderr, "%s: connexity for 3D images should be 6, 18 or 26\n", argv[0]);
	  exit(0);
	}

	if (argc == 8) {
	  xb = atoi(argv[2]);
	  yb = atoi(argv[3]);
	  zb = 0;
	  xe = atoi(argv[4]);
	  ye = atoi(argv[5]);
	  ze = 0;
	} else {
	  xb = atoi(argv[2]);
	  yb = atoi(argv[3]);
	  zb = atoi(argv[4]);
	  xe = atoi(argv[5]);
	  ye = atoi(argv[6]);
	  ze = atoi(argv[7]);
	}

  	image = readimage(argv[1]); //lecture de l'image d'entrée  
  	if (image == NULL)
  	{
    		fprintf(stderr, "Lenoir: readimage failed\n");
    		exit(0);
  	}
	
	rs = rowsize(image);
	cs = colsize(image);
	ds = depth(image);
	x = xb + yb*rs + zb*rs*cs;
	y = xe + ye*rs + ze*rs*cs;
	out = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);

	Lenoir(image, x, y, connex, out);//Application de l'algorithme
	if (argc == 8)
	  writeascimage(out, argv[7]);
	else
	  writeascimage(out, argv[9]);
	freeimage(image);
	freeimage(out);
	
	return 1;
}/* Main */
