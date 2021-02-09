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
/* \file 2dflowskeleton.c

\brief computes the flow skeleton (see [Cou10]) of a 2D complex

<B>Usage:</B> 2dflowskeleton in.pgm mode [level] out.pgm

<B>Description:</B>

Computes the flow skeleton (see [Cou10]) of a 2D complex.

The parameter \b mode selects the function to be integrated in order to build the topological map. The possible choices are:
\li 0: uniform null function 
\li 1: uniform unity function 
\li 2: border indicator function
\li 3: border indicator function and division of the integrated map by the opening function
\li 4: inverse opening function
\li 5: bisector function
\li 6: inverse Euclidean distance map
\li 7: lambda function
\li 8: uniform unity function on facets 
\li 9: border indicator function on facets 

\warning The input image \b in.pgm must be a complex, otherwise the result is meaningless (no verification is done)

References:<BR> 
[Cou10] M. Couprie: "Topological flows, maps and skeletons", in preparation.<BR>

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <l2dkhalimsky.h>
#include <l2dcollapse.h>

#define PARANO

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  struct xvimage * func;
  int32_t mode;
  double level;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm mode [level] out.pgm\n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]); 
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

#ifdef PARANO
  if (!l2d_is_complex(k))
  {
    fprintf(stderr, "%s: input image must be a complex\n", argv[0]);
    exit(1);
  }
#endif

  mode = atoi(argv[2]);

  if (argc == 4)
    level = -1;
  else
    level = atof(argv[3]);

  func = allocimage(NULL, rowsize(k), colsize(k), depth(k), VFF_TYP_FLOAT);
  if (func == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (!l2dflowskeleton(k, mode, level, func))
  {
    fprintf(stderr, "%s: l2dflowskeleton failed\n", argv[0]);
    exit(1);
  }

  writeimage(func, argv[argc-1]);

  freeimage(k);
  freeimage(func);

  return 0;
} /* main */
