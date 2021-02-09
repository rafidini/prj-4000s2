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
/*! \file longestplateau.c

\brief finds the location of the longest plateau in 1D sequence

<B>Usage:</B> longestplateau in.list [tolerance] out.list

<B>Description:</B>

Reads the sequence S from the file <B>in.list</B>. 
This file must have the following format:
<pre>  
  e &lt;n&gt;
  x1
  x2
  ...
  xn
</pre>
The tolerance t (float) is given by the parameter <B>tolerance</B>
(default value is 0).
A <i>plateau</i> is a subsequence P of S formed by consecutive elements of P, 
between indices i and j, and such that max{P[k];i<=k<=j} - min{P[k];i<=k<=j} <= t.
The program returns the base index and length of the first occurence of a plateau with 
maximal length in S.

<B>Types supported:</B> list 1D

<B>Category:</B> signal
\ingroup signal

\author Michel Couprie
*/

/* 
  Michel Couprie - avril 2006
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int32_t longest(double *tab, int32_t n, int32_t i, double tol)
/* =============================================================== */
{
  double minv, maxv; 
  int32_t j = i;
  minv = maxv = tab[i];
  while ((j < n) && ((maxv - minv) <= tol)) 
  {
    j++;
    if (tab[j] < minv) minv = tab[j];
    else if (tab[j] > maxv) maxv = tab[j];
  }
  return j - i;
} // longest()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t n, i, l, imax, lmax;
  char type;
  double v, tol, *tab;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.list [tolerance] out.list\n", argv[0]);
    exit(1);
  }

  if (argc == 4) tol = atof(argv[2]); else tol = 0.0;

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'e')
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  tab = (double *)malloc(n * sizeof(double));
  if (tab == NULL)
  {
    fprintf(stderr, "%s: malloc failed \n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf\n", &v);
    tab[i] = v;
  }
  fclose(fd);

  imax = 0; lmax = longest(tab, n, 0, tol);
  for (i = 1; i < n; i++)  
  {
    l = longest(tab, n, i, tol);
    if (l > lmax) { imax = i; lmax = l; }
  }
  free(tab);

  fd = fopen(argv[argc-1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(1);
  }
  fprintf(fd, "e 2\n");
  fprintf(fd, "%d %d\n", imax, lmax);
  fclose(fd);

  return 0;
}

