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
/* \file topotypes.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* calcul des types topologiques des points d' une image */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ltopotypes.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * out;
  int32_t rs, cs, bin, connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm bin connex out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "topotypes: readimage failed\n");
    exit(1);
  }

  bin = atoi(argv[2]);
  connex = atoi(argv[3]);

  rs = in->row_size;
  cs = in->col_size;
  out = allocimage(NULL,rs, cs, 1, VFF_TYP_1_BYTE);
  if (out == NULL)
  {
    fprintf(stderr, "topotypes: allocimage failed\n");
    exit(1);
  }

  if (! ltopotypes(in, out, connex, bin))
  {
    fprintf(stderr, "topotypes: function ltopotypes failed\n");
    exit(1);
  }
  
  writeimage(out, argv[argc - 1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
