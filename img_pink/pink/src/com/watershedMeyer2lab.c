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
/*! \file watershedMeyer2lab.c

\brief watershed transformation (Meyer's algorithm) with labelled markers  

<B>Usage:</B> watershedMeyer2lab marker image {roi|null} connex [mode] out

<B>Description:</B>
Performs the watershed transformation on the image <B>image</B>, taking the
set of markers in <B>marker</B>, in the form of a label image where all the pixels
sharing the same label (even if not connected) belong to the same marker.
If this parameter is present, <B>roi</B>
indicates the region of interest on which the operation is performed.
The parameter <B>connex</B> gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) 
for the makers.

The result is a label image. If the original markers are labelled by
numbers 1,...,n then the regions (catchment basins) of the result will
be labelled with the same numbers.

If the optional parameter <B>mode</B> is 0 (default value), then the
separation (watershed) will be labelled with n+1. Otherwise, a
separating point that is neighbour of exactly two regions i and j will be
labelled by j*(n+1)+i, with i<j, and a separating point that is neighbour 
of more than two regions be labelled by n+1.
Thus, from any label L > n+1, one can recover the labels of the two
regions i,j in contact by doing: i = L%(n+1); j = L/(n+1).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpemeyer.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * marqueurs;
  struct xvimage * masque;
  int32_t connex, mode;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s marker image {roi|null} connex [mode] out\n", argv[0]);
    exit(1);
  }

  marqueurs = readimage(argv[1]);
  image = readimage(argv[2]);
  if ((image == NULL) || (marqueurs == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[3],"null") == 0) 
    masque = NULL;
  else
  {
    masque = readimage(argv[3]);
    if (masque == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  connex = atoi(argv[4]);

  if (argc == 7) mode = atoi(argv[5]); else mode = 0;

  if ((connex == 4) || (connex == 8))
  {
    if (mode == 0)
    {
      if (! llpemeyer2(image, marqueurs, masque, connex))
      {
	fprintf(stderr, "%s: llpemeyer2 failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      fprintf(stderr, "%s: this mode is not yet implemented in 2D\n", argv[0]);
      exit(1);
    }
  }
  else if ((connex == 6) || (connex == 18) || (connex == 26))
  {
    if (mode == 0)
    {
      if (! llpemeyer3d2(image, marqueurs, masque, connex))
      {
	fprintf(stderr, "%s: llpemeyer3d2 failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      if (! llpemeyer3d2b(image, marqueurs, masque, connex))
      {
	fprintf(stderr, "%s: llpemeyer3d2b failed\n", argv[0]);
	exit(1);
      }
    }
  }
  else    
  {
    fprintf(stderr, "%s: bad connexity: %d\n", argv[0], connex);
    exit(1);
  }

  writeimage(marqueurs, argv[argc - 1]);
  freeimage(image);
  freeimage(marqueurs);
  if (masque) freeimage(masque);

  return 0;
} /* main */
