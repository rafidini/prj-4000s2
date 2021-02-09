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
/*! \file list2pgm.c

\brief converts from point list representation to pgm

<B>Usage:</B> list2pgm in.list {in.pgm|rs cs ds} [scale] out.pgm

<B>Description:</B>

Reads the file <B>in.list</B>. This file must have one of the following formats:
<pre>  
  e &lt;n&gt;       s &lt;n&gt;         b &lt;n&gt;         n &lt;n&gt;            B &lt;n&gt;            N &lt;n&gt;    
  x1          x1 v1         x1 y1         x1 y1 v1         x1 y1 z1         x1 y1 z1 v1
  x2    or    x2 v2   or    x2 y2   or    x2 y2 v2   or    x2 y2 z2   or    z2 x2 y2 v2
  ...         ...           ...           ...              ...              ...
  xn          xn vn         xn yn         xn yn vn         xn yn zn         zn xn yn vn
</pre>
The formats <B>e</B>, <B>s</B>, <B>b</B>, <B>n</B>, <B>B</B>, and <B>N</B>, correspond respectively
to binary 1D, graylevel 1D, binary 2D, graylevel 2D, binary 3D and graylevel 3D images.
If a file name <B>in.pgm</B> is given, then the points read in <B>in.list</B> are 
inserted (if possible) into the image read in <B>in.pgm</B>. Else, they are inserted in
a new image, the dimensions of which are <B>rs</B>, <B>cs</B>, and <B>d</B>.

The optional parameter \b scale allows to scale the coordinates.

<B>Types supported:</B> list 1D, 2D, 3D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - septembre 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, ds, ps, N, x, y, z, n, i;
  double xx, yy, zz, vv, scale;
  uint8_t *F;
  float *FF;
  char type;

  if ((argc != 4) && (argc != 6) && (argc != 5) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.list {in.pgm|rs cs ds} [scale] out.pgm\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type != 'e') && (type != 's') && (type != 'b') && (type != 'n') && (type != 'B') && (type != 'N'))
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  if ((argc == 4) || (argc == 5))
  {
    image = readimage(argv[2]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    rs = rowsize(image);
    cs = colsize(image);
    ds = depth(image);
    ps = rs * cs;
    N = ps * ds;
    if ((type == 'e') || (type == 'b') || (type == 'B'))
    {
      F = UCHARDATA(image);
      assert(datatype(image) == VFF_TYP_1_BYTE);
    }
    else
    {
      FF = FLOATDATA(image);
      assert(datatype(image) == VFF_TYP_FLOAT);
    }
  }
  else
  {
    rs = atoi(argv[2]);
    cs = atoi(argv[3]);
    ds = atoi(argv[4]);
    ps = rs * cs;
    N = ps * ds;
    if ((type == 'e') || (type == 'b') || (type == 'B'))
    {
      image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE); assert(image != NULL);
      F = UCHARDATA(image);
    }
    else
    {
      image = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT); assert(image != NULL);
      FF = FLOATDATA(image);
    }
    razimage(image);
  }

  if ((argc == 5) || (argc == 7)) scale = atof(argv[argc-2]); else scale = 1.0;

  if (type == 'e') 
  {
    if ((cs != 1) || (ds != 1))
    {
      fprintf(stderr, "%s: type %c is for 1D images\n", argv[0], type);
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf\n", &xx);
      xx *= scale;
      x = arrondi(xx);
      if ((x >= 0) && (x < rs))
        F[x] = 255;
    }
  }
  else if (type == 's') 
  {
    if ((cs != 1) || (ds != 1))
    {
      fprintf(stderr, "%s: type %c is for 1D images\n", argv[0], type);
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf\n", &xx, &vv);
      xx *= scale;
      x = arrondi(xx);
      if ((x >= 0) && (x < rs))
        FF[x] = (float)vv;
    }
  }
  else if (type == 'b') 
  {
    if (ds != 1)
    {
      fprintf(stderr, "%s: type %c is for 2D images\n", argv[0], type);
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf\n", &xx, &yy);
      xx *= scale;
      yy *= scale;
      x = arrondi(xx);
      y = arrondi(yy);
      if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs))
        F[y * rs + x] = 255;
    }
  }
  else if (type == 'n') 
  {
    if (ds != 1)
    {
      fprintf(stderr, "%s: type %c is for 2D images\n", argv[0], type);
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf %lf\n", &xx, &yy, &vv);
      xx *= scale;
      yy *= scale;
      x = arrondi(xx);
      y = arrondi(yy);
      if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs))
        FF[y * rs + x] = (float)vv;
    }
  } else if (type == 'B') 
  {
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf %lf\n", &xx, &yy, &zz);
      xx *= scale;
      yy *= scale;
      zz *= scale;
      x = arrondi(xx);
      y = arrondi(yy);
      z = arrondi(zz);
      if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < ds))
        F[z * ps + y * rs + x] = 255;
    }
  }
  else if (type == 'N') 
  {
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf %lf %lf\n", &xx, &yy, &zz, &vv);
      xx *= scale;
      yy *= scale;
      zz *= scale;
      x = arrondi(xx);
      y = arrondi(yy);
      z = arrondi(zz);
      if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < ds))
        FF[z * ps + y * rs + x] = (float)vv;
    }
  }

  fclose(fd);
  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return 0;
}

