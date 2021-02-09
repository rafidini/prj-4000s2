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
/* \file pgm2list1d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  Traduit un fichier image 1D (pgm) en fichier contenant le signal 1D correspondant,
  sous forme d'une liste de couples  x f(x)

  Michel Couprie - mars 2001
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
  FILE *fd = NULL;
  int32_t rs, cs, ds, x, y;
  uint8_t *F;
  uint32_t *G;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s image.pgm out.list \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ((datatype(image) != VFF_TYP_1_BYTE) && (datatype(image) != VFF_TYP_4_BYTE))
  {
    fprintf(stderr, "%s: bad image type(s)\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  
  if ((ds != 1) || (cs != 1))
  {
    fprintf(stderr, "%s: cette version ne traite que les signaux 1D\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    F = UCHARDATA(image);
    for (x = 0; x < rs; x++)
      fprintf(fd, "%d %d\n", x, F[x]); 
  }
  else
  {
    G = SLONGDATA(image);
    for (x = 0; x < rs; x++)
      fprintf(fd, "%d %d\n", x, G[x]); 
  }

  fclose(fd);
  freeimage(image);
  return 0;
}
