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
/* \file fusreg.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfusreg.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * marqueurs;
  struct xvimage * result;
  int32_t memoire;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s image.pgm marqueurs.pgm mem fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "fusreg: readimage failed\n");
    exit(1);
  }

  marqueurs = readimage(argv[2]);
  if (marqueurs == NULL)
  {
    fprintf(stderr, "fusreg: readimage failed\n");
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "fusreg: allocimage failed\n");
    exit(1);
  }

  memoire = atoi(argv[3]);
  if ((memoire != 0) && (memoire != 1))
  {   
    fprintf(stderr, "fusreg: parametre mem = [0|1]\n");
    exit(1);
  }

  if (! lfusreg(image, marqueurs, result, memoire))
  {
    fprintf(stderr, "fusreg: lfusreg failed\n");
    exit(1);
  }

  writelongimage(result, argv[4]);
  freeimage(image);
  freeimage(marqueurs);
  freeimage(result);
  return 0;
} /* main */



