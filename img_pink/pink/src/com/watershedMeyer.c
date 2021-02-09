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
/*! \file watershedMeyer.c

\brief watershed transformation (Meyer's algorithm)

<B>Usage:</B> watershedMeyer in mark {bgmark|null} {roi|null} connex out

<B>Description:</B>
Performs the watershed transformation on the image <B>in</B>, taking the
set of markers in <B>mark</B>. 
If this parameter is present, <B>bgmark</B>
is used as a set of markers for the background.
If this parameter is present, <B>roi</B>
indicates the region of interest on which the operation is performed.
The parameter <B>connex</B> gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) 
for the makers.

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
  struct xvimage * marqueursfond;
  struct xvimage * masque;
  int32_t connex;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in mark {bgmark|null} {roi|null} connex out\n", argv[0]);
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
    marqueursfond = NULL;
  else
  {
    marqueursfond = readimage(argv[3]);
    if (marqueursfond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (strcmp(argv[4],"null") == 0) 
    masque = NULL;
  else
  {
    masque = readimage(argv[4]);
    if (masque == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  connex = atoi(argv[5]);

  if (! llpemeyer(image, marqueurs, marqueursfond, masque, connex))
  {
    fprintf(stderr, "%s: llpemeyer failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  freeimage(marqueurs);
  if (marqueursfond) freeimage(marqueursfond);
  if (masque) freeimage(masque);

  return 0;
} /* main */
