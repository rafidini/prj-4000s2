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
/*! \file htkernu.c

\brief grayscale ultimate homotopic thickening

<B>Usage:</B> htkernu in.pgm {imcond.pgm|null} connex out.pgm

<B>Description:</B>
Grayscale ultimate homotopic thickening (refs. [BEC97, CBB01]).
The parameter \b connex gives the connectivity used for the minima;
possible choices are 4, 8 in 2D and 6, 26 in 3D.
Let F be the function corresponding to the input image \b in.pgm.
Let G be the function corresponding to the input image 
\b imcond.pgm, or the null function if the keyword \b null is used.
The algorithm is the following:

\verbatim
Repeat until stability:
    select p constructible for F such that F(p) is maximal
    F(p) = min{ delta+(p,F), G(p) }    
Result: F
\endverbatim

References:<BR> 
[BEC97] G. Bertrand, J. C. Everat and M. Couprie: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag97.ps.gz">"Image segmentation through operators based upon topology"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;6, No.&nbsp;4, pp.&nbsp;395-405, 1997.<BR>
[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
grayscale image processing"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.

<B>Types supported:</B> byte 2D, byte 3D.

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern.h>
#include <lhtkern3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm {imcond.pgm|null} connex fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8) && (connex != 6) && (connex != 26))
  {
    fprintf(stderr, "%s : connex = <4|8|6|26>\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
    imagecond = NULL;
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if ((connex == 4) || (connex == 8))
  {
    if (! lhtkernu(image, imagecond, connex))
    {
      fprintf(stderr, "%s: lhtkernu failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lhtkernu3d(image, imagecond, connex))
    {
      fprintf(stderr, "%s: lhtkernu3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
