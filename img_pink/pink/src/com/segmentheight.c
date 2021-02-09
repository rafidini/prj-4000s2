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
/*! \file segmentheight.c

\brief segmentation based on a height criterion

<B>Usage:</B> segmentheight in.pgm connex height [r] out.pgm

<B>Description:</B>
Height segmentation with connexity <B>connex</B> and depth <B>height</B>, 
followed by a reconstruction (2D only) if option <B>r</B> is set (see reference: to appear)
or a maximization if option <B>m</B> is set.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

/*
%TEST segmentheight %IMAGES/2dbyte/gray/g2gel.pgm 4 20 %RESULTS/segmentheight_g2gel_4_20.pgm
%TEST segmentheight %IMAGES/2dbyte/gray/g2gel.pgm 8 20 %RESULTS/segmentheight_g2gel_8_20.pgm
%TEST segmentheight %IMAGES/3dbyte/gray/g3a.pgm 6 20 %RESULTS/segmentheight_g3a_6_20.pgm
%TEST segmentheight %IMAGES/3dbyte/gray/g3a.pgm 18 20 %RESULTS/segmentheight_g3a_18_20.pgm
%TEST segmentheight %IMAGES/3dbyte/gray/g3a.pgm 26 20 %RESULTS/segmentheight_g3a_26_20.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribheight.h>
#include <lsegreconsheight.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, param;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm connex height [r|m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && (strcmp(argv[4], "r") != 0) && (strcmp(argv[4], "m") != 0))
  {
    fprintf(stderr, "usage: %s filein.pgm connex height [r|m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (argc == 5)
  {
    if (! lsegmentheight(image, connex, param, 0))
    {
      fprintf(stderr, "%s: lsegmentheight failed\n", argv[0]);
      exit(1);
    }
  }
  else
  if (strcmp(argv[4], "m") == 0)
  {
    if (! lsegmentheight(image, connex, param, 1))
    {
      fprintf(stderr, "%s: lsegmentheight failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lsegreconsheight(image, connex, param))
    {
      fprintf(stderr, "%s: lsegreconsheight failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



