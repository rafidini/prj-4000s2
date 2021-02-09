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
/*! \file segmentarea.c

\brief segmentation by filtering components on an area criterion

<B>Usage:</B> segmentarea in.pgm connex area [m] out.pgm

<B>Note:</B> in the python front-end 'm' is mandatory, set 0 without maximization
and 1 with maximization.

<B>Description:</B>
Segmentation by selection of cross-section components, based on an area criterion. 
Connexity = <B>connex</B> ; area threshold = <B>area</B>.

Segmentation is followed by a maximization if option <B>m</B> is set.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B>  connect
\ingroup  connect

\author Michel Couprie
*/

/*
%TEST segmentarea %IMAGES/2dbyte/gray/g2gel.pgm 4 100 %RESULTS/segmentarea_g2gel_4_100.pgm
%TEST segmentarea %IMAGES/2dbyte/gray/g2gel.pgm 8 100 %RESULTS/segmentarea_g2gel_8_100.pgm
%TEST segmentarea %IMAGES/3dbyte/gray/g3a.pgm 6 100 %RESULTS/segmentarea_g3a_6_100.pgm
%TEST segmentarea %IMAGES/3dbyte/gray/g3a.pgm 18 100 %RESULTS/segmentarea_g3a_18_100.pgm
%TEST segmentarea %IMAGES/3dbyte/gray/g3a.pgm 26 100 %RESULTS/segmentarea_g3a_26_100.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribarea.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, param;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm connex area [m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && (strcmp(argv[4], "m") != 0))
  {
    fprintf(stderr, "usage: %s filein.pgm connex area [m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segmentarea: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (argc == 5)
  {
    if (! lsegmentarea(image, connex, param, 0))
    {
      fprintf(stderr, "segmentarea: lsegmentarea failed\n");
      exit(1);
    }
  }
  else
  {
    if (! lsegmentarea(image, connex, param, 1))
    {
      fprintf(stderr, "segmentarea: lsegmentarea failed\n");
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



