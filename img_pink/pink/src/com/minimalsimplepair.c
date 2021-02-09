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
/*! \file minimalsimplepair.c

\brief detects minimal simple pairs in a 3D binary image

<B>Usage:</B> minimalsimplepair in.pgm [all] out.pgm

<B>Description:</B>
Detects minimal simple pairs in a 3D binary image. 
If the optional parameter \b all is used, then all 
minimal simple pairs are searched.
Otherwise, only one pair (if any) is given.

If the input image contains no simple point, consider using "simplepair" (faster)
instead of "minimalsimplepair".

Reference:<BR> 
[PCB08] Nicolas Passat, Michel Couprie, and Gilles Bertrand:
<A HREF="http://igm.univ-mlv.fr/LabInfo/rapportsInternes/2007/04.pdf">"Minimal simple pairs in the 3D cubic grid"</A>, in
<I>Journal of Mathematical Imaging and Vision</I>, 32 (3), pp.&nbsp;239–249, November 2008.<BR>

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2008
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseltopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.pgm [all] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 4) && (strcmp(argv[2], "all") != 0))
  {
    fprintf(stderr, "usage: %s filein.pgm [all] fileout.pgm\n", argv[0]);
    exit(1);
  }


  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    if (! lminimalsimplepair(image, 0))
    {
      fprintf(stderr, "%s: function lminimalsimplepair failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lminimalsimplepair(image, 1))
    {
      fprintf(stderr, "%s: function lminimalsimplepair failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


