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
/*! \file skel2pgm.c

\brief generation of a labelled image from a curvilinear skeleton

<B>Usage:</B> skel2pgm in.skel [id] out.pgm

<B>Description:</B>
Generation of a labelled image from a curvilinear skeleton.

If the optional argument \b id is given and positive, then only the skeleton element having this index is considered.

If the optional argument \b id is given and is -1, then points of the skeleton are labelled with their index in the skeleton structure (warning: no more than 255 elements)

Otherwise, points of the skeleton are labelled with:
\li 1: isolated
\li 2: end
\li 3: curve
\li 4: junction

<B>Types supported:</B> 2Dskel, 3Dskel

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2004
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>

//#define DEBUG

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  skel * S;
  int32_t id;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.skel [id] fileout.pgm\n", argv[0]);
    exit(1);
  }

  S = readskel(argv[1]);
  if (S == NULL)
  {
    fprintf(stderr, "%s: readskel failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4) id = atoi(argv[2]); else id = -2;

#ifdef DEBUG
  printskel(S);
#endif

  if (! (image = lskel2image(S, id)))
  {
    fprintf(stderr, "%s: function lskel2image failed\n", argv[0]);
    exit(1);
  }


  writeimage(image, argv[argc-1]);
  termineskel(S);
  freeimage(image);

  return 0;
} /* main */
