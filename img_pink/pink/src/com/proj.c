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
/*! \file proj.c

\brief projection parallel to one of the main axes

<B>Usage:</B> proj in.pgm dir mode out.pgm

<B>Description:</B>

Projection parallel to one of the main axes.

<B>dir</B> = x | y | z

<B>mode</B> = 0 (mean) | 1 (max)  | 2 (sum)

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* 
   Michel Couprie - avril 2001 
   update juillet 2012 - mode somme
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * result;
  struct xvimage * image;
  int32_t i, j, k;
  uint8_t *R;
  float *F;
  uint8_t *I;
  int32_t rs, cs, ds, ps, N;
  char dir;
  int32_t mode;
  double tmp;
  uint8_t t;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm dir mode out.pgm \n", argv[0]);
    fprintf(stderr, "       dir = x|y|z ; mode = 0 (mean) | 1 (max) | 2 (sum)\n");
    exit(1);
  }

  image = readimage(argv[1]);

  rs = rowsize(image);   /* taille ligne */
  cs = colsize(image);   /* taille colonne */
  ds = depth(image);     /* nb plans */
  ps = rs * cs;          /* taille plan */
  N = ps * ds;           /* taille image */
  I = UCHARDATA(image);

  dir = argv[2][0];
  mode = atoi(argv[3]);

  switch(dir)
  {
    case 'x':
      if (mode == 2)
      {
	result = allocimage(NULL, cs, ds, 1, VFF_TYP_FLOAT);
	assert(result != NULL);
	F = FLOATDATA(result);
      }
      else
      {
	result = allocimage(NULL, cs, ds, 1, VFF_TYP_1_BYTE);
	assert(result != NULL);
	R = UCHARDATA(result);
      }

      for (k = 0; k < ds; k++)
      for (j = 0; j < cs; j++)
      {
        tmp = 0.0;
        if (mode == 0)
	{
          for (i = 0; i < rs; i++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for i */
          R[k*cs + j] = (uint8_t)arrondi((tmp/rs));
	}
        else if (mode == 1)
	{
          t = 0;
          for (i = 0; i < rs; i++)
          {
            t = mcmax(t,I[k*ps+j*rs+i]);
          } /* for i */
          R[k*cs + j] = t;
	}
        else if (mode == 2)
	{
          for (i = 0; i < rs; i++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for i */
          F[k*cs + j] = (float)rs;
	}
      } /* for k, j */
      break;

    case 'y':
      if (mode == 2)
      {
	result = allocimage(NULL, rs, ds, 1, VFF_TYP_FLOAT);
	assert(result != NULL);
	F = FLOATDATA(result);
      }
      else
      {
	result = allocimage(NULL, rs, ds, 1, VFF_TYP_1_BYTE);
	assert(result != NULL);
	R = UCHARDATA(result);
      }

      for (k = 0; k < ds; k++)
      for (i = 0; i < rs; i++)
      {
        tmp = 0.0;
        if (mode == 0)
	{
          for (j = 0; j < cs; j++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for j */
          R[k*rs + i] = (uint8_t)arrondi((tmp/cs));
	}
        else if (mode == 1)
	{
          t = 0;
          for (j = 0; j < cs; j++)
          {
            t = mcmax(t,I[k*ps+j*rs+i]);
          } /* for j */
          R[k*rs + i] = t;
	}
        else if (mode == 2)
	{
          for (j = 0; j < cs; j++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for j */
          F[k*rs + i] = (float)tmp;
	}
      } /* for k, i */
      break;

    case 'z':
      if (mode == 2)
      {
	result = allocimage(NULL, rs, cs, 1, VFF_TYP_FLOAT);
	assert(result != NULL);
	F = FLOATDATA(result);
      }
      else
      {
	result = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
	assert(result != NULL);
	R = UCHARDATA(result);
      }

      for (j = 0; j < cs; j++)
      for (i = 0; i < rs; i++)
      {
        tmp = 0.0;
        if (mode == 0)
	{
          for (k = 0; k < ds; k++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for k */
          R[j*rs + i] = (uint8_t)arrondi((tmp/ds));
	}
        else if (mode == 1)
	{
          t = 0;
          for (k = 0; k < ds; k++)
          {
            t = mcmax(t,I[k*ps+j*rs+i]);
          } /* for k */
          R[j*rs + i] = t;
	}
        else if (mode == 2)
	{
          for (k = 0; k < ds; k++)
          {
            tmp += (double)(I[k * ps + j*rs + i]);
          } /* for k */
          F[j*rs + i] = (float)tmp;
	}
      } /* for j, i */
      break;
    
    default:
      fprintf(stderr, "dir = x|y|z \n");
      exit(1);

  } /* switch(dir) */

  writeimage(result, argv[argc - 1]);
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */

