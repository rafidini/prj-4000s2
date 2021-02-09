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
/* \file deriche3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Filtre lineaire general recursif de Deriche en 3D */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lderiche.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  double alpha;
  int32_t function;
  double l;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in1.pgm alpha function [l] out.pgm \n", 
            argv[0]);
    fprintf(stderr, "function = 0 : module du gradient lisse'\n");
    fprintf(stderr, "           1 : direction du gradient lisse'\n");
    fprintf(stderr, "           2 : laplacien lisse'\n");
    fprintf(stderr, "           3 : f - l * laplacien(f)\n");
    fprintf(stderr, "           4 : lisseur\n");
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "deriche3d: readimage failed\n");
    exit(1);
  }
  alpha = atof(argv[2]);
  function = atoi(argv[3]);
  if (function == 3) 
  { 
    if (argc != 6)
    {
      fprintf(stderr, "fonction 3: argument necessaire\n");
      exit(1);
    }
    l = atof(argv[4]);
  }

  if (function == 4)
  { 
    if (! llisseurrec3d(image1, alpha))
    {
      fprintf(stderr, "deriche3d: function llisseurrec3d failed\n");
      exit(1);
    }
  }
  else
  { 
    if (! lderiche3d(image1, alpha, function, l))
    {
      fprintf(stderr, "deriche3d: function lderiche3d failed\n");
      exit(1);
    }
  }

  if (function == 3) writeimage(image1, argv[5]);
  else               writeimage(image1, argv[4]);
  freeimage(image1);

  return 0;
} /* main */


