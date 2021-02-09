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
/*! \file skelsurf.c

\brief surfacic binary skeleton guided by a priority image

<B>Usage:</B> skelsurf in.pgm prio connex [inhibit] out.pgm

<B>Description:</B>
Surfacic binary skeleton guided by a priority image.
The lowest values of the priority image correspond to the highest priority.

The parameter \b prio is either an image (byte or int32_t), or a numerical code
indicating that a distance map will be used as a priority image; 
the possible choices are:
\li 0: approximate euclidean distance
\li 1: approximate quadratic euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

The parameter \b connex indicates the connectivity of the binary object.
Possible choices are 6, 26.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

Let X be the set corresponding to the input image \b in.pgm.
Let P be the function corresponding to the priority image.
Let I be the set corresponding to the inhibit image, if given, or the empty
set otherwise.
The algorithm is the following:

\verbatim
C = {y in F | Tb(y) > 1}
Repeat until stability
  choose a point x in X, simple for X, such that C[x] == 0 
    and such that P[x] is minimal
  X = X \ {x}
  for all y in gamma(x)
    if Tb(y) > 1 then C[y] = 1
Result: X
\endverbatim

where Tb(y) refers to the second connectivity number of y, that is, 
(informally) the number of connected components of gamma(y) inter complement(X).

References:<BR> 
[BC07] G. Bertrand and M. Couprie: <A HREF="http://www.esiee.fr/~coupriem/Pdf/chapitre_topo.pdf">"Transformations topologiques discretes"</A>, in <I>G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques</I>, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.<BR>

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/

/*
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 0 26 %RESULTS/skelsurf_b3a_0_26.pgm
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 1 26 %RESULTS/skelsurf_b3a_1_26.pgm
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 2 26 %RESULTS/skelsurf_b3a_2_26.pgm
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 3 26 %RESULTS/skelsurf_b3a_3_26.pgm
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 6 26 %RESULTS/skelsurf_b3a_6_26.pgm
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 18 26 %RESULTS/skelsurf_b3a_18_26.pgm
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 26 26 %RESULTS/skelsurf_b3a_26_26.pgm
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 0 6 %RESULTS/skelsurf_b3a_0_6.pgm
%TEST skelsurf %IMAGES/3dbyte/binary/b3a.pgm 0 26 %IMAGES/3dbyte/binary/b3a2.pgm %RESULTS/skelsurf_b3a_0_26_i.pgm
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
int main(int32_t argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * prio;
  struct xvimage * prio2;
  struct xvimage * inhibit = NULL;
  int32_t connex;
  int32_t ret, priovalue;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm prio connex [inhibit] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (depth(image) == 1)
  {
    fprintf(stderr, "%s: 3d images only\n", argv[0]);
    exit(1);
  }

  ret = sscanf(argv[2], "%d", &priovalue);
  if (ret == 0)
  {
    prio2 = readimage(argv[2]);
    if (prio2 == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    if (datatype(prio2) == VFF_TYP_1_BYTE)
    {
      int32_t rs = rowsize(prio2);
      int32_t cs = colsize(prio2);
      int32_t ds = depth(prio2);
      int32_t N = rs * cs * ds;
      uint8_t *B = UCHARDATA(prio2);
      int32_t *L;
      int32_t x;
      prio = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
      if (prio == NULL)
      {
        fprintf(stderr, "%s: allocimage failed\n", argv[0]);
        exit(1);
      }
      L = SLONGDATA(prio);
      for (x = 0; x < N; x++) L[x] = (int32_t)B[x];
      freeimage(prio2);
    }
    else if (datatype(prio2) == VFF_TYP_4_BYTE)
    {
      prio = prio2;
    }
    else
    {
      fprintf(stderr, "%s: bad datatype for prio\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    int32_t i, N;
    uint8_t *F;
    prio = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
    if (prio == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    N = rowsize(image) * colsize(image) * depth(image);
    F = UCHARDATA(image);;
    for (i = 0; i < N; i++) // inverse l'image
      if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
  
    if (priovalue == 0)
    {
      if (! ldisteuc3d(image, prio))
      {
        fprintf(stderr, "%s: ldisteuc3d failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priovalue == 1)
    {
      if (! ldistquad3d(image, prio))
      {
        fprintf(stderr, "%s: ldistquad3d failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priovalue == 2)
    {
      if (! lchamfrein(image, prio))
      {
        fprintf(stderr, "%s: lchamfrein failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priovalue == 3)
    {
      if (! lsedt_meijster(image, prio))
      {
	fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      if (! ldist(image, priovalue, prio))
      {
        fprintf(stderr, "%s: ldist failed\n", argv[0]);
        exit(1);
      }
    }
    for (i = 0; i < N; i++) // re-inverse l'image
      if (F[i]) F[i] = 0; else F[i] = NDG_MAX;
  }

  connex = atoi(argv[3]);

  if (argc == 6)
  {
    inhibit = readimage(argv[4]);
    if (inhibit == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lskelsurf3d(image, prio, inhibit, connex))
  {
    fprintf(stderr, "%s: lskelsurf3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(prio);

  return 0;
} /* main */
