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
/* \file hthinbin.c

OBSOLETE - see skeleton, skelcurv

\brief homotopic skeleton of a 2d binary image

<B>Usage:</B> hthinbin in {constr|null} connex ne ni out

<B>Description:</B>
Homotopic curvilinear thinning by iterative removal of simple,
non-end points. Breadth-first strategy. 
If given, the binary image \b constr indicates the points that must not
be removed during the thinning.
The number of iterations is set to \b ni . It \b ni = -1, 
the iterations will continue until stability.
During the first \b ne iterations (default 0), the end points
are removed as well.
If \b ne = -1, the end points are always removed.

<B>Types supported:</B> byte 2d

<B>Category:</B> topo
\ingroup  topo

\author Michel Couprie 1998
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsquelbin.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * contr = NULL;
  int32_t connex;
  int32_t niseuil;
  int32_t niter;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in {constr|null} connex ne ni out\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "hthinbin: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[3]);
  niseuil = atoi(argv[4]);
  niter = atoi(argv[5]);

  if (strcmp(argv[2],"null") == 0) 
  {
    if (! lhthinbin(image, connex, niseuil, niter))
    {
      fprintf(stderr, "hthinbin: lhthinbin failed\n");
      exit(1);
    }
  }
  else
  {
    contr = readimage(argv[2]);
    if (contr == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    if (! lhthinbincontr(image, contr, connex, niseuil, niter))
    {
      fprintf(stderr, "hthinbin: lhthinbincontr failed\n");
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (contr) freeimage(contr);

  return 0;
} /* main */
