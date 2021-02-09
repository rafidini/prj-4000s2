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
/*! \file 2dmakecomplex.c

\brief makes a cellular complex from the given set

<B>Usage:</B> 2dmakecomplex in.pgm [i] out.pgm

<B>Note: </B> in python for the i option use function 'l2dclosebeta'

<B>Description:</B>
Makes a cellular complex from the given set, 
by performing the topological closure.

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - avril 2006 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky2d.h>
#include <l2dkhalimsky.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  
  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [i] out.pgm \n", argv[0]);
    exit(1);
  }
  
  if ((argc == 4) && (strcmp(argv[2],"i") != 0))
  {
    fprintf(stderr, "usage: %s in.pgm [i] out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 3)
  {
    if (! l2dmakecomplex(k))
    {
      fprintf(stderr, "%s: function l2dmakecomplex failed\n", argv[0]);
      exit(1);
    }
  }
  else
    if (! l2dclosebeta(k))
    {
      fprintf(stderr, "%s: function l2dclosebeta failed\n", argv[0]);
      exit(1);
    }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
