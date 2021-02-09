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
/*! \file areaopening.c

\brief area opening

<B>Usage:</B> areaopening in.pgm connex area out.pgm

<B>Note: </B> in C main passes an argument 'area+1', whereas python passes the 'area'
as argument (LuM)

<B>Description:</B>
Area opening with connexity <B>connex</B> and area <B>area</B>.
Deletes the components of the upper cross-sections 
which have an area not higher than <B>area</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

/*
%TEST areaopening %IMAGES/2dbyte/gray/g2gel.pgm 4 20 %RESULTS/areaopening_g2gel_4_20.pgm
%TEST areaopening %IMAGES/2dbyte/gray/g2gel.pgm 8 20 %RESULTS/areaopening_g2gel_8_20.pgm
%TEST areaopening %IMAGES/3dbyte/gray/g3a.pgm 6 20 %RESULTS/areaopening_g3a_6_20.pgm
%TEST areaopening %IMAGES/3dbyte/gray/g3a.pgm 18 20 %RESULTS/areaopening_g3a_18_20.pgm
%TEST areaopening %IMAGES/3dbyte/gray/g3a.pgm 26 20 %RESULTS/areaopening_g3a_26_20.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
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

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex area fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "areaopening: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (! lareaopening(image, connex, param+1))
  {
    fprintf(stderr, "areaopening: lareaopening failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */



