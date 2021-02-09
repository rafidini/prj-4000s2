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

/*! \file MSF.c

\brief 
Compute a minimum spanning forest of a GA from a set of markers

<B>Usage:</B> MSF GAin.ga marqueurs.pgm typeResul Result.?? \n


<B>Description:</B>

Compute a minimum spanning forest of <B>GAin.ga</B> (a 4-connected 2D
or 6-connected 3D edge-weighted graph) relative to the connected
components of the non-zero pixels of <B>marqueurs.pgm</B>. If
TypeResult = 0, then Result.?? is a GA whose non-zero edges form the
induced MSF cut, otherwise Result.?? is a label image that represents
the vertex partition induced by the MSF (the connected components of
the MSF are marked with distinct labels).

See [COUSTYetAl-PAMI2009] and [COUSTYetAl-PAMI2010] for more details.

<B>Types supported:</B> GA byte 2D, GA byte 3D

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
#include <llabelextrema.h>
#include <lMSF.h>


/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * ga;
  struct xvimage * marqueurs;
  struct xvimage * labels;
  int32_t nblabels;

  if ((argc != 5))
  {
    fprintf(stderr, "usage: %s GAin.ga marqueurs.pgm typeResul GAout.ga\n TypeResult = 0 result is GA otherwise it is a label image\n", argv[0]);
    exit(1);
  }
  ga = readGAimage(argv[1]);
  marqueurs = readimage(argv[2]);
  if ((ga == NULL) || (marqueurs == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  labels = allocimage(NULL, rowsize(marqueurs), colsize(marqueurs), depth(marqueurs), VFF_TYP_4_BYTE);
  if (labels == NULL){   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  /* Il doit y avoir mieux dans Pink pour labeler des composantes connexes */ 
  if(depth(marqueurs) == 1){
    if (! llabelextrema(marqueurs, 4, 0, labels, &nblabels)){
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }
  }
  else 
    if (! llabelextrema(marqueurs, 6, 0, labels, &nblabels)){
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }
  
  if (! MSF(ga,labels)){
    fprintf(stderr, "%s: lLlpe failed\n", argv[0]);
    exit(1);
  }
  
  /*Il faut surement tester la valeur de retour de writeimage() */
  if(atoi(argv[3]) == 0)
    writerawGAimage(ga,argv[4]); 
  else writeimage(labels,argv[4]);
  
  freeimage(labels);
  freeimage(ga); freeimage(marqueurs); 
  return 0;
}


