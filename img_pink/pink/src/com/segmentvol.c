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
/*! \file segmentvol.c

\brief segmentation by filtering components on a volume criterion

<B>Note:</B> in the python front-end 'm' is mandatory, set 0 without maximization
and 1 with maximization.

<B>Usage:</B> segmentvol in.pgm connex vol out.pgm

<B>Description:</B>
Segmentation by selection of cross-section components, based on a 
volume criterion. 
Connexity = <B>connex</B> ; volume threshold = <B>vol</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup  connect

\author Michel Couprie
*/

/*
%TEST segmentvol %IMAGES/2dbyte/gray/g2gel.pgm 4 200 %RESULTS/segmentvol_g2gel_4_200.pgm
%TEST segmentvol %IMAGES/2dbyte/gray/g2gel.pgm 8 200 %RESULTS/segmentvol_g2gel_8_200.pgm
%TEST segmentvol %IMAGES/3dbyte/gray/g3a.pgm 6 200 %RESULTS/segmentvol_g3a_6_200.pgm
%TEST segmentvol %IMAGES/3dbyte/gray/g3a.pgm 18 200 %RESULTS/segmentvol_g3a_18_200.pgm
%TEST segmentvol %IMAGES/3dbyte/gray/g3a.pgm 26 200 %RESULTS/segmentvol_g3a_26_200.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribvol.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, param;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm connex vol [m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && (strcmp(argv[4], "m") != 0))
  {
    fprintf(stderr, "usage: %s filein.pgm connex vol [m] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segmentvol: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (argc == 5)
  {
    if (! lsegmentvol(image, connex, param, 0))
    {
      fprintf(stderr, "segmentvol: lsegmentvol failed\n");
      exit(1);
    }
  }
  else
  {
    if (! lsegmentvol(image, connex, param, 1))
    {
      fprintf(stderr, "segmentvol: lsegmentvol failed\n");
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



