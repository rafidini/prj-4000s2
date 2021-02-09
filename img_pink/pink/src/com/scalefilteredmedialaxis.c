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
/*! \file scaleaxis.c

\brief discrete scale filtered medial axis transform

<B>Usage:</B> scalefilteredmedialaxis in.pgm s out.pgm

<B>Description:</B>
Discrete scale filtered medial axis, as defined in [PCJ13], 
of the binary image \b X contained in \b in.pgm.

References:<BR> 
[PCJ13] "Scale Filtered Euclidean Medial Axis", Michal Postolski, Michel Couprie, Marcin Janaszewski, DGCI 2013

<B>Types supported:</B> byte 2d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

/*
%TEST scalefilteredmedialaxis %IMAGES/2dbyte/binary/b2hebreu.pgm 1.5 %RESULTS/scalefilteredmedialaxis_b2hebreu.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * res;
  double scale;
  uint32_t rs, cs, ds, N;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm scale fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  N = rs * cs * ds;

  res = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if (res == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  scale = atof(argv[2]);

  if (!lmedialaxis_scalefilteredmedialaxis(image, scale, res))
  {
    fprintf(stderr, "%s: lmedialaxis_scalefilteredmedialaxis failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(res, argv[argc - 1]);

  freeimage(res);
  freeimage(image);

  return 0;
} /* main */


