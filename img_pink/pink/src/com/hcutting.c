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
/*! \file hcutting.c

\brief homotopic cutting (pseudo opening) by a ball

<B>Usage:</B> hcutting in.pgm r connex out.pgm

<B>Description:</B>
Homotopic cutting (pseudo opening) by a ball of radius \b r.

Reference:<BR> 
[CB04] M. Couprie and G. Bertrand:
<A HREF="http://www.esiee.fr/~coupriem/Pdf/cb04.pdf">Topology preserving alternating sequential filter for smoothing 
2D and 3D objects</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;13, No.&nbsp;4, pp.&nbsp;720-730, 2004.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho, topobin
\ingroup  morpho, topobin

\author Michel Couprie 2002

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lasft.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, r;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s f.pgm r connex out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  r = atoi(argv[2]);
  connex = atoi(argv[3]);

  if (depth(image) == 1)
  {
    if (! lhpopeningdisc(image, connex, r))
    {
      fprintf(stderr, "%s: lhpopeningdisc failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lhpopening3dball(image, connex, r))
    {
      fprintf(stderr, "%s: lhpopening3dball failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
