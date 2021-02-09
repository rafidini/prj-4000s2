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
/*! \file histodist.c

\brief computes the distance between two histograms

<B>Usage:</B> histodist h1.list h2.list [type] out.list

<B>Description:</B>
Computes the distance between the two histograms given in \b h1.list and \b h2.list. The result is written in \b out.list.

The distance is an edit distance, it is defined as the minimal number of elementary operations (moving one unit from a bin to the neighboring one) that is necessary to transform h1 into h2.

Parameter \b type (default 0) indicates whether the histogram must be interpreted as circular (type = 1) of not (type = 0). Circular interpretation implies that first and last bins are considered as neighbors.

<B>Types supported:</B> list

<B>Category:</B> histo
\ingroup  histo

\author Michel Couprie, Mohamed Amine Salem
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhisto.h>

#define INDEX_FIRST
/*
*/

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  index_t *histo1, *histo2;
  char tag;
  int32_t i, x, y, n1, n2, type = 0;
  double dist;
  FILE *fd = NULL;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s h1.list h2.list [type] out.list\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r"); // lecture histo 1
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &tag);
  if (tag != 's')
  {
    fprintf(stderr, "%s: bad file format for %s: %c \n", argv[0], argv[1], tag);
    exit(1);
  }

  fscanf(fd, "%d\n", &n1);
  histo1 = (index_t *)calloc(n1, sizeof(index_t)); assert(histo1 != NULL);
  histo2 = (index_t *)calloc(n1, sizeof(index_t)); assert(histo2 != NULL);

  for (i = 0; i < n1; i++)
  {
    fscanf(fd, "%d %d\n", &x, &y);
    assert(x == i);
    histo1[x] = y;
  }

  fclose(fd);

  fd = fopen(argv[2],"r"); // lecture histo 2
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  fscanf(fd, "%c", &tag);
  if (tag != 's')
  {
    fprintf(stderr, "%s: bad file format for %s: %c \n", argv[0], argv[2], tag);
    exit(1);
  }

  fscanf(fd, "%d\n", &n2);
  if (n2 != n1)
  {
    fprintf(stderr, "%s: histograms must have the same size\n", argv[0]);
    exit(1);
  }
  
  for (i = 0; i < n2; i++)
  {
    fscanf(fd, "%d %d\n", &x, &y);
    assert(x == i);
    histo2[x] = y;
  }

  fclose(fd);

  if (argc == 5)
    type = atoi(argv[3]);

  if (type == 0)
    dist = lhisto_distance_ordinal (histo1, histo2, n1);
  else
    dist = lhisto_distance_modulo (histo1, histo2, n1);


  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "s %d\n", 1); 
  fprintf(fd, "0 %g\n", dist); 
  fclose(fd);

  return 0;
} /* main */
