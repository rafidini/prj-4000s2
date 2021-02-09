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
/*! \file taf.c

\brief topological alternating filter

<B>Usage:</B> taf in.pgm connexmin r [lambdap [lambdaw]] out.pgm

<B>Description:</B>
Topological alternating filter. Performs alternatively topological upper filter (tuf)
and topological lower filter (tlf) with increasing radius until radius <B>r</B>.

If given, parameter <B>lambdap</B> (resp. <B>lambdaw</B>) is an integer which 
indicates that peaks (resp. wells) of height greater than this value must be 
preserved. 

<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
/* 
   Filtre topologique alterne 

   Michel Couprie - janvier 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltrestopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rayon;
  int32_t connexmin;
  int32_t lambdapics = 255;
  int32_t lambdapuits = 255;

  if ((argc != 5) && (argc != 6)  && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm connexmin rayon [lambdapics [lambdapuits]] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connexmin = atoi(argv[2]);
  rayon = atoi(argv[3]);

  if (argc >= 6) lambdapics = lambdapuits = atoi(argv[4]);
  if (argc == 7) lambdapuits = atoi(argv[5]);

  if (! ltaflambda(image, connexmin, rayon, lambdapics, lambdapuits))
  {
    fprintf(stderr, "%s: function ltaflambda failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
