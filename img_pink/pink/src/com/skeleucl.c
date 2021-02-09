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
/*! \file skeleucl.c

\brief Euclidean binary skeleton

<B>Usage:</B> skeleucl in.pgm connex [inhibit] out.pgm

<B>Description:</B>
Euclidean binary skeleton.

The parameter \b connex indicates the connectivity of the binary object.
Possible choices are 4, 8 in 2d and 6, 26 in 3d.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image (constraint set) will be left unchanged. 

This operator is usually used with a <I>constraint set</I> (parameter \b inhibit) that is a subset of the exact Euclidean medial axis (see operator <I>medialaxis</I>). 

Here is an example using the whole medial axis as constraint set:

\verbatim
medialaxis test.pgm 3 _1
threshold _1 1 _2
skeleucl test.pgm 8 _2 result.pgm
\endverbatim

Intersesting subsets are obtained by filtering the medial axis, either based on the ball radiuses, or based on the bisector angle (see operator <I>bisector</I>). Below is a script showing how to proceed:

\verbatim
#!/bin/sh
USAGE="Usage: $0 in seuilR (in [1..infnty[) seuilA (in [0.001..pi]) out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi
medialaxis $1 3 /tmp/skel2_tmp_m
threshold /tmp/skel2_tmp_m 1 /tmp/skel2_tmp_m1
skeleucl $1 8 /tmp/skel2_tmp_m1 /tmp/skel2_tmp_s
threshold /tmp/skel2_tmp_m $2 /tmp/skel2_tmp_ms
distc $1 3 /tmp/skel2_tmp_d
bisector /tmp/skel2_tmp_d /tmp/skel2_tmp_ms /tmp/skel2_tmp_a
threshold /tmp/skel2_tmp_a $3 /tmp/skel2_tmp_i
skeleton /tmp/skel2_tmp_s /tmp/skel2_tmp_d 8 /tmp/skel2_tmp_i $4
rm -f /tmp/skel2_tmp_*
\endverbatim

References:<BR> 
[CCZ07] M. Couprie, D. Coeurjolly and R. Zrour: <A HREF="http://www.esiee.fr/~coupriem/Pdf/ccz07.pdf">"Discrete bisector function and Euclidean skeleton in 2D and 3D"</A>, <I>Image and Vision Computing</I>, Vol.&nbsp;25, No.&nbsp;10, pp.&nbsp;1543-1556, 2007.<BR>

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lskeletons.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * inhibit = NULL;
  int32_t connex;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm connex [inhibit] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  if (argc == 5)
  {
    inhibit = readimage(argv[3]);
    if (inhibit == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lskeleucl(image, connex, inhibit))
  {
    fprintf(stderr, "%s: lskeleucl failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
