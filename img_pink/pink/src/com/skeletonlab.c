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
/*! \file skeletonlab.c

\brief ultimate binary skeleton on label image guided by a priority image (see [BC07])

<B>Usage:</B> skeletonlab in.pgm prio connex [inhibit] out.pgm

<B>Description:</B>
Ultimate binary skeleton on label image guided by a priority image.
For simple point testing, each label is treated as if all other labels are turned to 0 (background).
For computing distance maps, all labels are turned to 1 (foregroung).
The lowest values of the priority image correspond to the highest priority.

The parameter \b prio is either an image (byte, int32_t, float or double), or a numerical code
indicating that a distance map will be used as a priority image; 
the possible choices are:
\li 0: approximate euclidean distance
\li 1: approximate quadratic euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

The parameter \b connex indicates the connectivity of the binary object.
Possible choices are 4, 8 in 2d and 6, 26 in 3d.

If the parameter \b inhibit is given and is an integer different from -1,
then the points which correspond to this priority value will be left unchanged. 
If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

Let X be the set corresponding to the input image \b in.pgm.
Let P be the function corresponding to the priority image.
Let I be the set corresponding to the inhibit image, if given, or the empty
set otherwise.
The algorithm is the following:

\verbatim
Repeat until stability
    Select a point x in X \ I such that P[x] is minimal
    If x is simple for X then
        X = X \ {x}
Result: X
\endverbatim

Reference:<BR> 
[BC07] G. Bertrand and M. Couprie: <A HREF="http://www.esiee.fr/~coupriem/Pdf/chapitre_topo.pdf">"Transformations topologiques discretes"</A>, in <I>G&eacute;om&eacute;trie discr&egrave;te et images num&eacute;riques</I>, D. Coeurjolly and A. Montanvert and J.M. Chassery, pp.&nbsp;187-209, Herm&egrave;s, 2007.<BR>
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
  struct xvimage * imagebin;
  struct xvimage * prio;
  int32_t connex;
  int32_t ret, priocode;
  struct xvimage * inhibimage = NULL;
  int32_t inhibvalue;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm prio connex [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  ret = sscanf(argv[2], "%d", &priocode);
  if (ret == 0) // priorité : image 
  {
    prio = readimage(argv[2]);
    if (prio == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }
  else  // priorité : carte de distance (à calculer)
  {
    int32_t i, N;
    int32_t *F;
    uint8_t *B;
    prio = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE); assert(prio != NULL);
    imagebin = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE); assert(imagebin != NULL);
    N = rowsize(image) * colsize(image) * depth(image);
    F = SLONGDATA(image);
    B = UCHARDATA(imagebin);
    for (i = 0; i < N; i++) // inverse l'image
      if (F[i]) B[i] = 0; else B[i] = NDG_MAX;
  
    if (priocode == 0)
    {
      if (depth(image) == 1)
      {
        if (! ldisteuc(imagebin, prio))
        {
          fprintf(stderr, "%s: ldisteuc failed\n", argv[0]);
          exit(1);
        }
      }
      else
      {
        if (! ldisteuc3d(imagebin, prio))
        {
          fprintf(stderr, "%s: ldisteuc3d failed\n", argv[0]);
          exit(1);
        }
      }
    }
    else
    if (priocode == 1)
    {
      if (depth(image) == 1)
      {
        if (! ldistquad(imagebin, prio))
        {
          fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
          exit(1);
        }
      }
      else
      {
        if (! ldistquad3d(imagebin, prio))
        {
          fprintf(stderr, "%s: ldistquad3d failed\n", argv[0]);
          exit(1);
        }
      }
    }
    else
    if (priocode == 2)
    {
      if (! lchamfrein(imagebin, prio))
      {
        fprintf(stderr, "%s: lchamfrein failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priocode == 3)
    {
      if (! lsedt_meijster(imagebin, prio))
      {
	fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      if (! ldist(imagebin, priocode, prio))
      {
        fprintf(stderr, "%s: ldist failed\n", argv[0]);
        exit(1);
      }
    }
  }
  freeimage(imagebin);

  connex = atoi(argv[3]);

  if (argc == 6) 
  {
    int32_t ret = sscanf(argv[4], "%d", &inhibvalue);
    if (ret == 0)
    {
      inhibimage = readimage(argv[4]);
      if (inhibimage == NULL)
      {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
      }
    }
  }
  else inhibvalue = -1;

  if (depth(imagebin) == 1)
  {
    fprintf(stderr, "%s: 2D not yet implemented\n", argv[0]);
    exit(1);
#ifdef WHEN_IMPLEMENTED
    if (inhibimage || (inhibvalue == -1))
    {
      if (! lskelubp2lab(image, prio, connex, inhibimage))
      {
        fprintf(stderr, "%s: lskelubp2 failed\n", argv[0]);
        exit(1);
      }
    }
    else
    {
      if (! lskelubplab(image, prio, connex, inhibvalue))
      {
        fprintf(stderr, "%s: lskelubp failed\n", argv[0]);
        exit(1);
      }
    }
#endif
  }
  else // 3D
  {
    if (inhibimage || (inhibvalue == -1))
    {
      if (! lskelubp3d2lab(image, prio, connex, inhibimage))
      {
        fprintf(stderr, "%s: lskelubp3d2 failed\n", argv[0]);
        exit(1);
      }
    }
    else
    {
      fprintf(stderr, "%s: inhibvalue not yet implemented\n", argv[0]);
      exit(1);
#ifdef WHEN_IMPLEMENTED
      if (! lskelubp3dlab(image, prio, connex, inhibvalue))
      {
        fprintf(stderr, "%s: lskelubp3d failed\n", argv[0]);
        exit(1);
      }
#endif
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(prio);
  if (inhibimage) freeimage(inhibimage);

  return 0;
} /* main */
