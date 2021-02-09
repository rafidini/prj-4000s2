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
/* \file htkern3dbin.c

\brief homotopic thinning or kernel of a 3D binary image

<B>Usage:</B> htkern3dbin im.pgm connex nimax out.pgm

<B>Description:</B>
Homotopic thinning or kernel of a 3D binary image. 
Uses a breadth-first strategy. 
The connectivity of the object is given by <B>connex</B>.
The maximum number of iterations is given by <B>nimax</B>,
if equal to -1 stability is reached.

<B>Types supported:</B> byte 3d

<B>Category:</B> topo
\ingroup  topo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;
  int32_t nimax;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm connex nimax fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 6) && (connex != 26))
  {
    fprintf(stderr, "%s : connex = <6|26>\n", argv[0]);
    exit(1);
  }

  nimax = atoi(argv[3]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! mctopo3d_lhtkern3dbin(image, connex, nimax))
  {
    fprintf(stderr, "%s: lhtkern3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
