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

/*! \file graph2ps.c

\brief draws a planar graph into a ps file

<B>Usage:</B> graph2ps in.graph out.ps

<B>Description:</B>
Draws a planar graph into a ps file.

<B>Types supported:</B> graph

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie 2009
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcgraphe.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  graphe * g; 
  int i;
  double scale;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage 1: %s filein.graph [scale] fileout.ps\n", argv[0]);
    exit(1);
  }

  g = ReadGraphe(argv[1]);             /* lit le graphe a partir du fichier */

  if (argc == 3) 
  {
    scale = 1;
  }
  else
    scale = atof(argv[2]);

  for (i = 0; i < g->nsom; i++)
  {
    g->x[i] *= scale;
    g->y[i] *= -scale; // - : pour suivre la convention image (y de haut en bas)
  }

  EPSGraphe(g, argv[argc-1], 
	    2, // facteur d'échelle à appliquer aux coordonnées des sommets
	    10, // rayon des cercles qui représentent les sommets
	    0, // taille (demi-longueur) des flèches pour les arcs
	    10, // marge en x et en y
	    0, // booléen indiquant s'il faut écrire les noms des sommets
	    0, // booléen indiquant s'il faut écrire les valeurs des sommets
	    1, // booléen indiquant s'il faut colorier les sommets
	    0  // booléen indiquant s'il faut écrire les valeurs des arcs
	   ) ;

  TermineGraphe(g);

  return 0;
} /* main */
