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
/* 
  Generation d'un maillage triangulaire d'un ensemble de surfels  
  Michel Couprie  -  Juillet 2001
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcrbtp.h>
#include <mcmesh.h>
#include <mciomesh.h>

#define CSTVOL

/*
#define DEBUG
*/

/* =============================================================== */
int32_t lsurfels(struct xvimage * f, uint8_t v, 
             int32_t obj_id, FILE *fileout)
/* =============================================================== */
{
  int32_t i, j, k;
  int32_t rs, cs, ps, ds, N;
  uint8_t * F;
  uint8_t s;
  double g = 0.5;
  double h = 0.45;

  /* v est la valeur de seuil. si v == 0 alors il s'agit d'une image binaire */
  if (v == 0) s = 1; else s = v;

  rs = rowsize(f);
  cs = colsize(f);
  ds = depth(f);
  ps = rs * cs;
  N = ps * ds;
  F = UCHARDATA(f);

  for (k = 0; k < ds-1; k++)
  for (j = 0; j < cs-1; j++)
  for (i = 0; i < rs-1; i++)
  {
    v = F[k*ps + j*rs + i];
    if (v < s)
    {
      if (F[k*ps + j*rs + i+1] >= s) 
        gencarrePOV(fileout, i+g, j-h, k-h, i+g, j+h, k-h, i+g, j+h, k+h, i+g, j-h, k+h);
      if (F[k*ps + (j+1)*rs + i] >= s)
        gencarrePOV(fileout, i-h, j+g, k-h, i-h, j+g, k+h, i+h, j+g, k+h, i+h, j+g, k-h);
      if (F[(k+1)*ps + j*rs + i] >= s)
        gencarrePOV(fileout, i-h, j-h, k+g, i+h, j-h, k+g, i+h, j+h, k+g, i-h, j+h, k+g);
    }
    else /* v >= s */
    {
      if (F[k*ps + j*rs + i+1] < s) 
        gencarrePOV(fileout, i+g, j-h, k+h, i+g, j+h, k+h, i+g, j+h, k-h, i+g, j-h, k-h);
      if (F[k*ps + (j+1)*rs + i] < s)
        gencarrePOV(fileout, i+h, j+g, k-h, i+h, j+g, k+h, i-h, j+g, k+h, i-h, j+g, k-h);
      if (F[(k+1)*ps + j*rs + i] < s)
        gencarrePOV(fileout, i-h, j+h, k+g, i+h, j+h, k+g, i+h, j-h, k+g, i-h, j-h, k+g);
    }
  }

  return 1;

} /* lsurfels() */

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * f;
  FILE *fileout = NULL;
  uint8_t v;
  int32_t obj_id;
  
  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm threshold obj_id out.txt \n", argv[0]);
    exit(0);
  }

  f = readimage(argv[1]);  
  if (f == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(0);
  }

  v = (uint8_t)atoi(argv[2]);
  obj_id = atoi(argv[3]);

  fileout = fopen(argv[argc - 1],"w");
  if (!fileout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(0);
  }

  if (! lsurfels(f, v, obj_id, fileout))
  {
    fprintf(stderr, "%s: function lsurfels failed\n", argv[0]);
    exit(0);
  } 

  freeimage(f);
  fclose(fileout);
  return 0;
} /* main */
