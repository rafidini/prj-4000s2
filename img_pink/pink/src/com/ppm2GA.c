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

/*! \file ppm2GA.c

\brief Computes an edge-weighted graph from an color .ppm image

<B>Usage:</B> ppm2GA im.ppm param out.ga \n

<B>Description:</B>

Computes a GA (an edge-weighted graph) from a color image. The values
of an edge linking two pixels {x,y} is computed according to the
parameter <B>param</B>.
 
If param = 0, the maximum, over the RGB chanels, of absolute
differences of intensities between x and y is used.

If param = 1, the Euclidean distance between the values of x and y is
used.

If param = 2, fuzzy affinity between x and y is used.

If im.ppm is a 2D (resp. 3D) image, then <B>out.ga</B> is a 2D (resp
3D GA), that is a 2D 4-connected edge-weighted graph (resp. a 2D
6-connected edge-weighted graph).


<B>Types supported:</B> ppm 2D

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
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * r;
  struct xvimage * v;
  struct xvimage * b;
  struct xvimage * ga;
  int32_t param;
  if ( (argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s im.pgm param  out.ppm \nparam = 0 max (sur RGB) des differences entre pixel extremite, param = 1 distance euclidienne entre pixels extremités, param = 2 affinité floue.\n", 
            argv[0]);
    exit(1);
  }
  if ( readrgbimage(argv[1], &r,&v,&b) == 0 )
  {
    fprintf(stderr, "%s: ne peut lire %s \n", argv[0], argv[1]);
    exit(1);
  }
  if( (ga = allocGAimage(argv[2], r->row_size , r->col_size, depth(r), VFF_TYP_GABYTE )) == NULL )
  {
      fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
      freeimage(r); freeimage(v); freeimage(b);
      exit(1);
  }
  param = atoi(argv[2]);

  if(depth(r) == 1)
    {
      if( lppm2ga(r,v,b,ga,param) != 1 )
      {
	fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
	exit(1);
      }
    }
  else
    {
      fprintf(stderr, "%s: Ne traite pas les images 3D couleurs\n", argv[0]);
      exit(1);
    }
  writerawGAimage(ga, argv[3]); 
  freeimage(r); freeimage(v); freeimage(b);freeimage(ga);
  return 0;
}
