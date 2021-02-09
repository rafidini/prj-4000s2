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
/*! \file pgm2skel2.c

\brief decomposition of a curvilinear skeleton into isolated points,
end points, curves and junctions

<B>Usage:</B> pgm2skel in.pgm junc.pgm connex out.skel

<B>Description:</B> 
The skeleton found in \b in.pgm is decomposed into isolated points,
end points, curves and junctions ; and its description is stored in 
\b out.skel .

The parameter \b connex sets the adjacency relation used for the object
(4, 8 (2d) or 6, 18, 26 (3d)).

The image given as parameter\b junc.pgm contains curve points that
will artificially considered as junction points.

\warning Points at the border of the image will be ignored.

\warning IMPORTANT LIMITATION: 
different junctions in the original image must not be in direct
contact with each other (i.e., connected) otherwise they will be
considered as a single junction. To prevent this to occur, one can
increase image resolution.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2009
*/

/*
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>
#include <pinktypes.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * junc;
  int32_t connex;
  skel * S;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm junc.pgm connex fileout.skel\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  junc = readimage(argv[2]);
  if (junc == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);

  if (! (S = limage2skel2(image, junc, connex)))
  {
    fprintf(stderr, "%s: function limage2skel2 failed\n", argv[0]);
    exit(1);
  }

  //printskel(S);

  writeskel(S, argv[argc-1]);
  termineskel(S);
  freeimage(image);
  freeimage(junc);

  return 0;
} /* main */
