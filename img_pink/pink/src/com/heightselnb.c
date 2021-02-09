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
/*! \file heightselnb.c

\brief height based segmentation by selection of a given number or components

<B>Usage:</B> heightselnb in.pgm connex nb out.pgm

<B>Description:</B>
Segmentation by selection of a given number or components, based on a 
height (contrast) criterion. 
Connexity = <B>connex</B> ; desired number of components = <B>nb</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

/*
%TEST heightselnb %IMAGES/2dbyte/gray/g2gel.pgm 4 3 %RESULTS/heightselnb_g2gel_4_3.pgm
%TEST heightselnb %IMAGES/2dbyte/gray/g2gel.pgm 8 3 %RESULTS/heightselnb_g2gel_8_3.pgm
%TEST heightselnb %IMAGES/3dbyte/gray/g3a.pgm 6 3 %RESULTS/heightselnb_g3a_6_3.pgm
%TEST heightselnb %IMAGES/3dbyte/gray/g3a.pgm 18 3 %RESULTS/heightselnb_g3a_18_3.pgm
%TEST heightselnb %IMAGES/3dbyte/gray/g3a.pgm 26 3 %RESULTS/heightselnb_g3a_26_3.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribheight.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, param;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex nb fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "heightselnb: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  if (! lheightselnb(image, connex, param))
  {
    fprintf(stderr, "heightselnb: lheightselnb failed\n");
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */



