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
/*! \file genfield.c

\brief generates a null vector field of given size

<B>Usage:</B> genfield {in.pgm|rs cs ds} out.pgm

<B>Description:</B>
The size of the result field <B>out.pgm</B> is taken from image
<B>in.pgm</B>, or given by the parameters <B>rs</B>, <B>cs</B>, <B>ds</B>. 

<B>Types supported:</B> float 2d, float 3d

<B>Category:</B> arith, geo
\ingroup  arith, geo

\author Michel Couprie
*/

/* 
   Michel Couprie - juillet 2010
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * header;
  struct xvimage * field;
  int32_t rs, cs, ds;

  if ((argc != 3) && (argc != 5))
  {
    fprintf(stderr, "usage: %s {in.pgm|rowsize colsize depth} out.pgm \n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    rs = atoi(argv[1]);
    cs = atoi(argv[2]);
    ds = atoi(argv[3]);
  }
  else
  {
    header = readheader(argv[1]);
    rs = rowsize(header);
    cs = colsize(header);
    ds = depth(header);
  }
  assert(rs > 0);
  assert(cs > 0);
  assert(ds > 0);

  if (ds > 1)
    field = allocmultimage(NULL, rs, cs, ds, 1, 3, VFF_TYP_FLOAT);
  else
    field = allocmultimage(NULL, rs, cs, 1, 1, 2, VFF_TYP_FLOAT);

  if (field == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  razimage(field);

  writeimage(field, argv[argc-1]);
  freeimage(field);

  return 0;
} /* main */
