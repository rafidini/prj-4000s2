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
/*! \file extractplane.c

\brief extracts a plane normal to one of the directions x,y,z from a 3D image

<B>Usage:</B> extractplane in.pgm n plane out.pgm

<B>Description:</B>
The original 3D image is <B>in.pgm</B>. The parameter <B>n</B> is an integer.
The parameter <B>plane</B> is set to <B>xy</B>, <B>yx</B>, 
<B>xz</B>, <B>zx</B>, <B>yz</B>, <B>zy</B>.
The order of the letters x, y, z is meaningfull: for example, zy means that 
each row of the output image will correspond to a z-line of the original image, 
each column of the output image will correspond to a y-line of the original image.
The output image <B>out.pgm</B> contains the <B>n</B>th plane of the given form extracted
from <B>in.pgm</B>.

<B>Types supported:</B> byte 3d, long 3d, float 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imgres;
  int32_t i, j, k, n, t, offset, rs, cs, ds;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm n plane fileout.pgm (plane=xy|yx|xz|zx|yz|zy)\n", argv[0]);
    exit(1);
  }

  if (! (((argv[3][0] == 'x') && (argv[3][1] == 'y')) ||
         ((argv[3][0] == 'y') && (argv[3][1] == 'x')) ||
         ((argv[3][0] == 'x') && (argv[3][1] == 'z')) ||
         ((argv[3][0] == 'z') && (argv[3][1] == 'x')) ||
         ((argv[3][0] == 'y') && (argv[3][1] == 'z')) ||
         ((argv[3][0] == 'z') && (argv[3][1] == 'y'))))
  {
    fprintf(stderr, "usage: %s filein.pgm n plane fileout.pgm (plane=xy|yx|xz|zx|yz|zy)\n", argv[0]);
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
  ds  = depth(image);

if (datatype(image) == VFF_TYP_1_BYTE)
{
  uint8_t *I, *R;
  I = UCHARDATA(image);

  n = atoi(argv[2]);

  if ((argv[3][0] == 'x') && (argv[3][1] == 'y'))
  {
    imgres = allocimage(NULL, rs, cs, 1, datatype(image));
    R = UCHARDATA(imgres);
    if ((n < 0) || (n >= ds))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    offset = n * t;
    for (i = 0; i < t; i++) R[i] = I[i + offset];    
  } 
  else if ((argv[3][0] == 'y') && (argv[3][1] == 'x'))
  {
    imgres = allocimage(NULL, cs, rs, 1, datatype(image));
    R = UCHARDATA(imgres);
    if ((n < 0) || (n >= ds))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (j = 0; j < cs ; j++)
      for (i = 0; i < rs ; i++)
        R[i*cs + j] = I[n*t + j*rs + i];
  } 
  else if ((argv[3][0] == 'x') && (argv[3][1] == 'z'))
  {
    imgres = allocimage(NULL, rs, ds, 1, datatype(image));
    R = UCHARDATA(imgres);
    if ((n < 0) || (n >= cs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (i = 0; i < rs ; i++)
        R[k*rs + i] = I[k*t + n*rs + i];
  }
  else if ((argv[3][0] == 'z') && (argv[3][1] == 'x'))
  {
    imgres = allocimage(NULL, ds, rs, 1, datatype(image));
    R = UCHARDATA(imgres);
    if ((n < 0) || (n >= cs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (i = 0; i < rs ; i++)
        R[i*ds + k] = I[k*t + n*rs + i];
  }
  else if ((argv[3][0] == 'y') && (argv[3][1] == 'z'))
  {
    imgres = allocimage(NULL, cs, ds, 1, datatype(image));
    R = UCHARDATA(imgres);
    if ((n < 0) || (n >= rs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (j = 0; j < cs ; j++)
        R[k*cs + j] = I[k*t + j*rs + n];
  }
  else if ((argv[3][0] == 'z') && (argv[3][1] == 'y'))
  {
    imgres = allocimage(NULL, ds, cs, 1, datatype(image));
    R = UCHARDATA(imgres);
    if ((n < 0) || (n >= rs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (j = 0; j < cs ; j++)
        R[j*ds + k] = I[k*t + j*rs + n];
  }
  else
  {
    fprintf(stderr, "usage: %s filein.pgm n plane fileout.pgm (plane=xy|yx|xz|zx|yz|zy)\n", argv[0]);
    exit(1);
  }
} 
else if (datatype(image) == VFF_TYP_4_BYTE)
{
  uint32_t *I, *R;
  I = ULONGDATA(image);

  n = atoi(argv[2]);

  if ((argv[3][0] == 'x') && (argv[3][1] == 'y'))
  {
    imgres = allocimage(NULL, rs, cs, 1, datatype(image));
    R = ULONGDATA(imgres);
    if ((n < 0) || (n >= ds))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    offset = n * t;
    for (i = 0; i < t; i++) R[i] = I[i + offset];    
  } 
  else if ((argv[3][0] == 'y') && (argv[3][1] == 'x'))
  {
    imgres = allocimage(NULL, cs, rs, 1, datatype(image));
    R = ULONGDATA(imgres);
    if ((n < 0) || (n >= ds))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (j = 0; j < cs ; j++)
      for (i = 0; i < rs ; i++)
        R[i*cs + j] = I[n*t + j*rs + i];
  } 
  else if ((argv[3][0] == 'x') && (argv[3][1] == 'z'))
  {
    imgres = allocimage(NULL, rs, ds, 1, datatype(image));
    R = ULONGDATA(imgres);
    if ((n < 0) || (n >= cs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (i = 0; i < rs ; i++)
        R[k*rs + i] = I[k*t + n*rs + i];
  }
  else if ((argv[3][0] == 'z') && (argv[3][1] == 'x'))
  {
    imgres = allocimage(NULL, ds, rs, 1, datatype(image));
    R = ULONGDATA(imgres);
    if ((n < 0) || (n >= cs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (i = 0; i < rs ; i++)
        R[i*ds + k] = I[k*t + n*rs + i];
  }
  else if ((argv[3][0] == 'y') && (argv[3][1] == 'z'))
  {
    imgres = allocimage(NULL, cs, ds, 1, datatype(image));
    R = ULONGDATA(imgres);
    if ((n < 0) || (n >= rs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (j = 0; j < cs ; j++)
        R[k*cs + j] = I[k*t + j*rs + n];
  }
  else if ((argv[3][0] == 'z') && (argv[3][1] == 'y'))
  {
    imgres = allocimage(NULL, ds, cs, 1, datatype(image));
    R = ULONGDATA(imgres);
    if ((n < 0) || (n >= rs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (j = 0; j < cs ; j++)
        R[j*ds + k] = I[k*t + j*rs + n];
  }
  else
  {
    fprintf(stderr, "usage: %s filein.pgm n plane fileout.pgm (plane=xy|yx|xz|zx|yz|zy)\n", argv[0]);
    exit(1);
  }
} 
else if (datatype(image) == VFF_TYP_FLOAT)
{
  float *I, *R;
  I = FLOATDATA(image);

  n = atoi(argv[2]);

  if ((argv[3][0] == 'x') && (argv[3][1] == 'y'))
  {
    imgres = allocimage(NULL, rs, cs, 1, datatype(image));
    R = FLOATDATA(imgres);
    if ((n < 0) || (n >= ds))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    offset = n * t;
    for (i = 0; i < t; i++) R[i] = I[i + offset];    
  } 
  else if ((argv[3][0] == 'y') && (argv[3][1] == 'x'))
  {
    imgres = allocimage(NULL, cs, rs, 1, datatype(image));
    R = FLOATDATA(imgres);
    if ((n < 0) || (n >= ds))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (j = 0; j < cs ; j++)
      for (i = 0; i < rs ; i++)
        R[i*cs + j] = I[n*t + j*rs + i];
  } 
  else if ((argv[3][0] == 'x') && (argv[3][1] == 'z'))
  {
    imgres = allocimage(NULL, rs, ds, 1, datatype(image));
    R = FLOATDATA(imgres);
    if ((n < 0) || (n >= cs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (i = 0; i < rs ; i++)
        R[k*rs + i] = I[k*t + n*rs + i];
  }
  else if ((argv[3][0] == 'z') && (argv[3][1] == 'x'))
  {
    imgres = allocimage(NULL, ds, rs, 1, datatype(image));
    R = FLOATDATA(imgres);
    if ((n < 0) || (n >= cs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (i = 0; i < rs ; i++)
        R[i*ds + k] = I[k*t + n*rs + i];
  }
  else if ((argv[3][0] == 'y') && (argv[3][1] == 'z'))
  {
    imgres = allocimage(NULL, cs, ds, 1, datatype(image));
    R = FLOATDATA(imgres);
    if ((n < 0) || (n >= rs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (j = 0; j < cs ; j++)
        R[k*cs + j] = I[k*t + j*rs + n];
  }
  else if ((argv[3][0] == 'z') && (argv[3][1] == 'y'))
  {
    imgres = allocimage(NULL, ds, cs, 1, datatype(image));
    R = FLOATDATA(imgres);
    if ((n < 0) || (n >= rs))
    {
      fprintf(stderr, "%s: bad plane number\n", argv[0]);
      exit(1);
    }
    t = rs * cs;
    for (k = 0; k < ds ; k++)
      for (j = 0; j < cs ; j++)
        R[j*ds + k] = I[k*t + j*rs + n];
  }
  else
  {
    fprintf(stderr, "usage: %s filein.pgm n plane fileout.pgm (plane=xy|yx|xz|zx|yz|zy)\n", argv[0]);
    exit(1);
  }
} 
else 
{
  fprintf(stderr, "%s: bad data type\n", argv[0]);
  exit(1);
}

  writeimage(imgres, argv[argc-1]);
  freeimage(imgres);
  freeimage(image);

  return 0;
} /* main */
