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
/*! \file 3drecons.c

\brief geodesic reconstruction in a 3d khalimsky order 

<B>Usage:</B> 3drecons g.list f.pgm out.pgm

<B>Description:</B>
Geodesic reconstruction (in the sense of the neighborhood relation theta) 
of the set of points represented by the list \b g.list in the set \b f.pgm .

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/
/* Michel Couprie - novembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  FILE *fd;
  int32_t i, n;
  index_t *tab;  
  double xx, yy, zz;
  index_t x, y, z;
  index_t rs, cs, ds, ps, N;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s g.list f.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[2]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  ps = rs * cs;
  N = ps * ds;

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'B')
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  tab = (index_t *)calloc(1,n * sizeof(index_t));
  if (tab == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf %lf\n", &xx, &yy, &zz);
    x = arrondi(xx);
    y = arrondi(yy);
    z = arrondi(zz);
    if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < ds))
      tab[i] = x + y * rs + z * ps;
    else
    {      
#ifdef MC_64_BITS
      fprintf(stderr, "%s: point outside image: %lld %lld %lld\n", argv[0], x, y, z);
#else
      fprintf(stderr, "%s: point outside image: %d %d %d\n", argv[0], x, y, z);
#endif
      exit(1);
    }
  }
  fclose(fd);

  if (! l3drecons(k, tab, n))
  {
    fprintf(stderr, "%s: function l3drecons failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[3]);
  freeimage(k);
  free(tab);

  return 0;
} /* main */
