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
/*! \file eden.cxx

\brief growth and/or shrinking of an 2D binary image - Eden process

<B>Usage:</B> eden in.pgm niter grow shrink topo out.pgm

<B>Description:</B>

Algorithm is as follows.

For growth:
  \li start from known set
  \li take pixels from the border into a queue
  \li choose randomly one such pixel
  \li set it to 1
  \li update queues
  \li repeat

For shrinking:
  \li same thing but set to 0

For adding boundary noise
  \li alternate growth and shrinking

With topological constraints
  \li make sure the point to add or remove is simple

Parameters:
  \li niter (positive integer): number of iterations
  \li grow (1/0): perform growing or not
  \li shrink (1/0): perform shrinking or not
  \li topo (26/6/8/4/0): connectivity for the object - 0: no topological constraint

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup geo

\author Hugues Talbot
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <leden.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * image1;
  index_t rs, cs, ds;
  int32_t niter, grow, shrink, topo;
  uint8_t *In;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in1.pgm niter grow shrink topo out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  In = UCHARDATA(image1);
  niter = atoi(argv[2]);
  grow = atoi(argv[3]);
  shrink = atoi(argv[4]);
  topo = atoi(argv[5]);

  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);

  if (! ledengrowth(In, rs, cs, ds, niter, grow, shrink, topo))
  {
    fprintf(stderr, "%s: function ledengrowth failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


