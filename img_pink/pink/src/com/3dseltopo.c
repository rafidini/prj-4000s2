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
/*! \file 3dseltopo.c

\brief selects points in the 3d khalimsky grid according to a local topological criterion 

<B>Usage:</B> 3dseltopo in d- d+ a- a+ b- b+ out

<B>Description:</B>
  Selects the elements x of the Khalimsky volume K
  which satisfy the following inequalities : 

  \b d1 <= d(x) <= \b d2

  \b a1 <= a(x) <= \b a2

  \b b1 <= b(x) <= \b b2

  where

  d(x) = dimension of x

  a(x) = number of elements under x of dimension d(x) - 1

  b(x) = number of elements over x of dimension d(x) + 1

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup orders

\author Michel Couprie
*/

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
  struct xvimage * image;
  int32_t dm, dp, am, ap, bm, bp;

  if (argc != 9)
  {
    fprintf(stderr, "usage: %s in d- d+ a- a+ b- b+ out\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  dm = atoi(argv[2]);
  dp = atoi(argv[3]);
  am = atoi(argv[4]);
  ap = atoi(argv[5]);
  bm = atoi(argv[6]);
  bp = atoi(argv[7]);

  if (! l3dseltype(image, dm, dp, am, ap, bm, bp))
  {
    fprintf(stderr, "%s: function l3dseltype failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
