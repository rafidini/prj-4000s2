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
/*! \file watershedwithoutlinelab.c

\brief watershed transformation without line of separation from labelled marker

<B>Usage:</B> watershedwithoutlinelab in mark {roi|null} connex out

<B>Description:</B>
Performs the watershed transformation on the image <B>in</B>, taking the
labelled marker in <B>mark</B>. 
If this parameter is present, <B>roi</B>
indicates the region of interest on which the operation is performed.
The parameter <B>connex</B> gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) 
for the makers.

The image mark is a label image (int32_t)

The image out is a label image (int32_t)

This version does not create any line to separate the catchment basins. 

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie and Laurent Najman
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
  int32_t connex;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in mark {roi|null} connex out\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  marqueurs = readimage(argv[2]);
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

  if ((connex == 4) || (connex == 8))
  {
    if (! llpemeyersanslignelab(image, marqueurs, masque, connex))
    {
      fprintf(stderr, "%s: llpemeyersanslignelab failed\n", argv[0]);
      exit(1);
    }
  }
  else if ((connex == 6) || (connex == 18) || (connex == 26))
  {
    if (! llpemeyer3dsanslignelab(image, marqueurs, masque, connex))
    {
      fprintf(stderr, "%s: llpemeyer3dsanslignelab failed\n", argv[0]);
      exit(1);
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
