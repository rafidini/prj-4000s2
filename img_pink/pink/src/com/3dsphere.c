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
/* NOT SUPPORTED - EXPERIMENTAL */
/* \file 3dsphere.c

\brief Builds the discretization of a sphere in the 3d Khalimsky space.

<B>Usage:</B> 3dsphere in.pgm x0 y0 z0 radius out.pgm

<B>Description:</B>
  Draws into the Khalimsky volume \b k, the discretized sphere of center 
  \b x0, \b y0, \b z0 and of radius \b r.

<B>Types supported:</B> byte 3d

<B>Category:</B> topo 
\ingroup  topo 

\author Michel Couprie
*/

/* Michel Couprie - novembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  int32_t x0, y0, z0;
  double radius;  

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm x0 y0 z0 radius out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  x0 = atoi(argv[2]);
  y0 = atoi(argv[3]);
  z0 = atoi(argv[4]);
  radius = atof(argv[5]);

  if (! l3dsphere(k, x0, y0, z0, radius))
  {
    fprintf(stderr, "%s: function l3dsphere failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc - 1]);
  freeimage(k);

  return 0;
} /* main */
