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
/*! \file pgm2GA4d.c

\brief Create a 4D edge-weighted graph from a series of 3D images


<B>Usage:</B> pgm2GA4d prefix_in first last mode GA4d_out.ga

<B>Description:</B> Create a 4D GA (edge-weighted graph) from the 4D
image obtained by concatenation of the series of 3D images \<
<B>prefix_inxxxx.pgm</B> | xxxx is a four digit decimal integer of the
interval [ <B>first</B> , <B>last</B>] \> . The values of the edges
are computed according to the parameter <B>mode</B>. If <B>mode</B> is
set to 0, then the value of an edge { x,y } is the absolute diference
of intensity between the pixels x and y and if <B>mode</B> is set to 1
the maximum of intensity between x and y is used. The output
<B>GA4d_out.ga</B> is a 4D GA, that is a 4D, 8-connected,
edge-weighted grah, (i.e. the adjacency is the direct adjacency in
dimension 4).

<B>Types supported:</B> byte 2d, byte 3d, byte 4d

<B>Category:</B> convert
\ingroup convert

\author Jean Cousty - janvier 2006
*/


#include <stdio.h>
#include <stdint.h>
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
  struct xvimage4D * im;
  struct GA4d * ga;
  int32_t param;
  if (argc != 6)
  {
    fprintf(stderr, "usage: %s prefix_in first last mode GA4d_out.pgm \nparap = 0 difference entre pixel extremite, param = 1 max entre pixel extremite,\n", 
            argv[0]);
    exit(1);
  }
  if ((im = readimage4D(argv[1], atoi(argv[2]), atoi(argv[3]))) == NULL )
  {
    fprintf(stderr, "%s: ne peut lire %s \n", argv[0], argv[1]);
    exit(1);
  }
  //printf("taille de l'image 3d: %d\n", depth(im));
  if( (ga = allocGA4d(argv[2], rowsize(im->frame[0]) , colsize(im->frame[0]), depth(im->frame[0]), im->ss )) == NULL )
  {
    fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
    freeimage4D(im);
    exit(1);
  }
  param = atoi(argv[4]);
  if( lpgm2ga4d(im,ga,param) != 1 )
  {
    fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
    exit(1);
  }
  writeGA4d(ga, argv[5]);
  freeimage4D(im); freeGA4d(ga);
  return 0;
}
