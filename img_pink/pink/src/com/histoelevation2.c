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
/* \file histoelevation2.c

\brief 

<B>Usage:</B> histoelevation2 in.list nbins fileout

<B>Description:</B> Orientations are given as vectors in the input file \b "in.list", in the form :<br>
 V "n"<br>
 x1 y1 z1<br>
 x2 y2 z2<br>
 ...<br>
 xn yn zn<br>

Elevation angles (in degrees between 0 and 180) are computed and their histogram, divided into \b "nbins" bins, are written into output file "fileout", in the form :<br>
 s "nbins"<br>
 0 "histo[0]"<br>
 1 "histo[1]"<br>
 ...<br>
 n-1 "histo[n-1]"<br>

<B>Types supported:</B> 3D vector list

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhisto.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  index_t * histo;
  int32_t i, nbins, nvec;
  FILE *fdin = NULL;
  FILE *fdout = NULL;
  double *Vx, *Vy, *Vz;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.list nbins fileout\n", argv[0]);
    exit(1);
  }

  fdin = fopen(argv[1],"r");
  if (!fdin)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fdin, "%c", &type);
  if (type != 'V')
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fdin, "%d\n", &nvec);
  Vx = (double *)malloc(nvec * sizeof(double)); assert(Vx != NULL);
  Vy = (double *)malloc(nvec * sizeof(double)); assert(Vy != NULL);
  Vz = (double *)malloc(nvec * sizeof(double)); assert(Vz != NULL);
  for (i = 0; i < nvec; i++)
  {
    fscanf(fdin, "%lf %lf %lf\n", &(Vx[i]), &(Vy[i]), &(Vz[i]));
  }

  nbins = atoi(argv[2]);

  if (! lhistoelevationlist(nvec, Vx, Vy, Vz, nbins, &histo))
  {
    fprintf(stderr, "%s: function lhistoelevation failed\n", argv[0]);
    exit(1);
  }

  fdout = fopen(argv[argc-1],"w");
  if (!fdout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(1);
  }
 
  fprintf(fdout, "s %d\n", nbins);
  for (i = 0; i < nbins; i++) 
#ifdef MC_64_BITS
    fprintf(fdout, "%4d %lld\n", i, histo[i]);
#else
    fprintf(fdout, "%4d %d\n", i, histo[i]);
#endif

  fclose(fdin);
  fclose(fdout);
  free(histo);
  free(Vx);
  free(Vy);
  free(Vz);

  return 0;
} /* main */
