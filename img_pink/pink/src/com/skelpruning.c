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
/*! \file skelpruning.c

\brief pruning of "short end branches" in a curvilinear skeleton

<B>Usage:</B> skelpruning in.skel length out.pgm

<B>Description:</B>

The skeleton found in \in.skel is searched for "small" branches which satisfy the following criteria:
\li Branch has exactly one end
\li Branch length is less than or equal to \b length parameter.

Parameter \b length is a number pixels.

Matching arcs are written in \b out.pgm.

<B>Types supported:</B> skel 2d, skel 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2011
*/

/*
%TEST skelpruning %IMAGES/2dskel/s2skel4.skel 35 %RESULTS/skelpruning_s2skel4.skel
%TEST skelpruning %IMAGES/2dskel/s2skel8.skel 35 %RESULTS/skelpruning_s2skel8.skel
%TEST skelpruning %IMAGES/3dskel/s3skel6.skel 35 %RESULTS/skelpruning_s3skel6.skel
%TEST skelpruning %IMAGES/3dskel/s3skel26.skel 35 %RESULTS/skelpruning_s3skel26.skel
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>
#include <lsquelbin.h>
#include <lskeletons.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  skel * S;
  double length;
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.skel length out.pgm\n", argv[0]);
    exit(1);
  }

  S = readskel(argv[1]);
  if (S == NULL)
  {
    fprintf(stderr, "%s: readskel failed\n", argv[0]);
    exit(1);
  }
  
  length = atof(argv[2]);

  if (!lskelpruning(S, length))
  {
    fprintf(stderr, "%s: function lskelpruning failed\n", argv[0]);
    exit(1);
  }

  if (! (image = lskelnotmarked2image(S)))
  {
    fprintf(stderr, "%s: function lskelnotmarked2image failed\n", argv[0]);
    exit(1);
  }

  if (depth(image) == 1) // 2D
  {
    if (! lsquelbin(image, S->connex, -1))
    {
      fprintf(stderr, "%s: lsquelbin failed\n", argv[0]);
      exit(1);
    }
  }
  else // 3D
  {
    if (! lskelendcurv3d(image, S->connex, -1))
    {
      fprintf(stderr, "%s: lskelendcurv3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  termineskel(S);
  freeimage(image);

  return 0;
} /* main */
