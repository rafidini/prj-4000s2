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
/*! \file saliency.c

\brief salient watershed transformation

<B>Usage:</B> saliency in connex out

<B>Description:</B>
Performs the salient watershed transformation on the image <B>in</B>, 
The parameter <B>connex</B> gives the adjacency relation (4 or 6b in 2D).

The image out is a int32_t image (int32_t)

In 4 connectivity, the output image is a line graph in the khalimsky
grid. In 6b connectivity, the result is on the pixels themselves, with
the gammab neighborhood.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Laurent Najman
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpemeyer.h>
#include <lsaliency.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * masque;
  struct xvimage * result;
  int32_t connex;

  //if (argc != 5)
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in connex out\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);

  if (!strcmp(argv[2], "4")) {
    result = allocimage(NULL, 2*rowsize(image)+1, 2*colsize(image)+1, depth(image), VFF_TYP_4_BYTE);
    if (result == NULL)
      {   
	fprintf(stderr, "%s: allocimage failed\n", argv[0]);
	exit(1);
      }
    /*
      if (strcmp(argv[2],"null") == 0) 
      masque = NULL;
      else
      {
      masque = readimage(argv[2]);
      if (masque == NULL)
      {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
      }
      }
    */
    masque = NULL;
    //connex = atoi(argv[3]);
    connex = 4;

    if ((connex == 4) || (connex == 8))
      {
	if (! lsaliency(image, masque, result, connex))
	  {
	    fprintf(stderr, "%s: lsaliency failed\n", argv[0]);
	    exit(1);
	  }
      }
    /*
      else if ((connex == 6) || (connex == 18) || (connex == 26))
      {
      if (! llpemeyer3dsansligne(image, marqueurs, marqueursfond, masque, connex, result))
      {
      fprintf(stderr, "%s: llpemeyer3d failed\n", argv[0]);
      exit(1);
      }
      }*/
  } else if (!strcmp(argv[2], "6b")) {
    result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
    if (result == NULL)
      {   
	fprintf(stderr, "%s: allocimage failed\n", argv[0]);
	exit(1);
      }
    masque = NULL;
    if (! lsaliency6b(image, masque, result, 0))
	  {
	    fprintf(stderr, "%s: lsaliency6b failed\n", argv[0]);
	    exit(1);
	  }
  }
  else    
  {
    fprintf(stderr, "%s: bad connexity: %d\n", argv[0], connex);
    exit(1);
  }


  writeimage(result, argv[argc - 1]);
  freeimage(image);
  freeimage(result);
  if (masque) freeimage(masque);

  return 0;
} /* main */
