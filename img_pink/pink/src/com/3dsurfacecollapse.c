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
/*! \file 3dsurfacecollapse.c

\brief parallel directional constrained collapse with surface detection and preservation

<B>Usage:</B> 3dsurfacecollapse in.pgm nsteps [inhibit] out.pgm

<B>Description:</B>
Parallel directional constrained collapse with surface detection and preservation.

The parameter \b nsteps gives the number of "layers" to be removed, if
the value is -1 then the interations continue until stability.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>
#include <mcgraphe.h>
#include <l3dcollapse.h>

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  struct xvimage * inhibimage = NULL;
  int32_t nsteps;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm nsteps [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  nsteps = atoi(argv[2]);

  if (argc == 5) 
  {
    inhibimage = readimage(argv[3]);
    if (inhibimage == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    inhibimage = allocimage(NULL, rowsize(k), colsize(k), depth(k), VFF_TYP_1_BYTE);
    if (inhibimage == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! l3ddetectdyncollapse(k, nsteps, inhibimage, 2))
  {
    fprintf(stderr, "%s: function l3ddetectdyncollapse failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
