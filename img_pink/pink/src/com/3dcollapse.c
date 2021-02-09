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
/*! \file 3dcollapse.c

\brief ultimate constrained collapse guided by a priority image

<B>Usage:</B> 3dcollapse in.pgm prio [inhibit] out.pgm

<B>Description:</B>
Ultimate constrained collapse guided by a priority image.
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

If the parameter \b inhibit is given and is a binary image name,
then the elements of this image will be left unchanged. 
If the parameter \b inhibit is given and is a number I,
then the elements with priority greater than or equal to I will be left unchanged. 

\warning The result makes sense only if the input image is a complex.

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/*
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k 0 %RESULTS/3dcollapse_b3a_0.k
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k 1 %RESULTS/3dcollapse_b3a_1.k
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k 2 %RESULTS/3dcollapse_b3a_2.k
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k 3 %RESULTS/3dcollapse_b3a_3.k
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k 4 %RESULTS/3dcollapse_b3a_4.k
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k 8 %RESULTS/3dcollapse_b3a_8.k
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k 0 3 %RESULTS/3dcollapse_b3a_0_3.k
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k %IMAGES/3dfloat/f3alam.k 10 %RESULTS/3dcollapse_b3a_alam_10.k
%TEST 3dcollapse %IMAGES/3dbyte/binary/b3a.k 0 %IMAGES/3dbyte/binary/b3a2.k %RESULTS/3dcollapse_b3a_0_i.k
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>
#include <mcgeo.h>
#include <mcgraphe.h>
#include <ldist.h>
#include <l3dcollapse.h>

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  struct xvimage * prio;
  struct xvimage * prio2;
  struct xvimage * inhibimage = NULL;
  int32_t ret, priocode;
  int32_t rs, cs, ds, N;
  float priomax_f;
  int32_t priomax_l;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm prio [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  N = rs*cs*ds;

  ret = sscanf(argv[2], "%d", &priocode);
  if (ret == 0) // priorité : image 
  {
    prio2 = readimage(argv[2]);
    if (prio2 == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    if ((rowsize(prio2) != rs) || (colsize(prio2) != cs) || (depth(prio2) != ds))
    {
      fprintf(stderr, "%s: incompatible image sizes\n", argv[0]);
      exit(1);
    }
    if (datatype(prio2) == VFF_TYP_1_BYTE)
    {
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
    else if (datatype(prio2) == VFF_TYP_FLOAT)
    {
      prio = prio2;
    }
    else
    {
      fprintf(stderr, "%s: bad datatype for prio\n", argv[0]);
      exit(1);
    }
  }
  else  // priorité : carte de distance (à calculer)
  {
    int32_t i;
    uint8_t *K;
    prio = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    if (prio == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    K = UCHARDATA(k);;
    for (i = 0; i < N; i++) // inverse l'image
      if (K[i]) K[i] = 0; else K[i] = NDG_MAX;
  
    if (priocode == 0)
    {
      if (ds == 1)
      {
        if (! ldisteuc(k, prio))
        {
          fprintf(stderr, "%s: ldisteuc failed\n", argv[0]);
          exit(1);
        }
      }
      else
      {
        if (! ldisteuc3d(k, prio))
        {
          fprintf(stderr, "%s: ldisteuc3d failed\n", argv[0]);
          exit(1);
        }
      }
    }
    else
    if (priocode == 1)
    {
      if (ds == 1)
      {
        if (! ldistquad(k, prio))
        {
          fprintf(stderr, "%s: ldistquad failed\n", argv[0]);
          exit(1);
        }
      }
      else
      {
        if (! ldistquad3d(k, prio))
        {
          fprintf(stderr, "%s: ldistquad3d failed\n", argv[0]);
          exit(1);
        }
      }
    }
    else
    if (priocode == 2)
    {
      if (! lchamfrein(k, prio))
      {
        fprintf(stderr, "%s: lchamfrein failed\n", argv[0]);
        exit(1);
      }
    }
    else
    if (priocode == 3)
    {
      if (! lsedt_meijster(k, prio))
      {
	fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
	exit(1);
      }
    }
    else
    {
      if (! ldist(k, priocode, prio))
      {
        fprintf(stderr, "%s: ldist failed\n", argv[0]);
        exit(1);
      }
    }
    for (i = 0; i < N; i++) // re-inverse l'image
      if (K[i]) K[i] = 0; else K[i] = NDG_MAX;
  }

  if (argc == 5) 
  {
    ret = sscanf(argv[3], "%f", &priomax_f);
    if (ret == 0) // inhibit : image
    {
      inhibimage = readimage(argv[3]);
      if (inhibimage == NULL)
      {
        fprintf(stderr, "%s: readimage failed\n", argv[0]);
        exit(1);
      }
    }
  }
  else 
  {
    priomax_l = INT32_MAX; 
    priomax_f = FLT_MAX; 
  }

  if (datatype(prio) == VFF_TYP_4_BYTE)
  {
    if ((argc == 5) && (inhibimage == NULL)) priomax_l = (int32_t)floorf(priomax_f);
    if (! l3dpardircollapse_l(k, prio, inhibimage, priomax_l))
    {
      fprintf(stderr, "%s: function l3dpardircollapse_l failed\n", argv[0]);
      exit(1);
    }
  }
  else
  if (datatype(prio) == VFF_TYP_FLOAT)
  {
    if (! l3dpardircollapse_f(k, prio, inhibimage, priomax_f))
    {
      fprintf(stderr, "%s: function l3dpardircollapse_f failed\n", argv[0]);
      exit(1);
    }
  }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
