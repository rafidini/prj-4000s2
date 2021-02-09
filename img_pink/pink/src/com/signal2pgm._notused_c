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
/*! \file signal2pgm.c

\brief converts from signal representation to pgm

<B>Usage:</B> signal2pgm in.list [scale] out.pgm

<B>Description:</B>

Reads the file <B>in.list</B>. This file must have one of the following formats:
<pre>  
    s &lt;n&gt; 
    x1 y1 
    x2 y2 
    ...   
    xn yn 
</pre>

The optional parameter \b scale allows to scale the y coordinates.

<B>Types supported:</B> signal 1D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - decembre 2004
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, N, x, y, v, n, i;
  double xx, yy, vv, scale;
  uint8_t *F;
  char type;
  int32_t *signal;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.list [scale] out.pgm\n", argv[0]);
    exit(1);
  }

  if (argc == 4) scale = atof(argv[2]); else scale = 1.0;

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 's')
  {
    fprintf(stderr, "usage: %s : bat file format : %c \n", type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  signal = (int32_t *)calloc(1,n * sizeof(int32_t));
  if (signal == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }









PAS FINI !!!!!!!!!







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

  fclose(fd);



    rs = atoi(argv[2]);
    cs = atoi(argv[3]);
    ds = atoi(argv[4]);
    ps = rs * cs;
    N = ps * ds;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    F = UCHARDATA(image);
    memset(F, 0, N);




  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return 0;
}

