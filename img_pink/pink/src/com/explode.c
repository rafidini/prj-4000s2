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
/*! \file explode.c

\brief converts single 3D pgm file into a series of 2D pgm files

<B>Usage:</B> explode in.pgm [begin end step] name_prefix

<B>Description:</B>
Generated file names are of the form: <B>name_prefix</B>nnnn.pgm, 
where nnnn is a four digit decimal integer. 

<B>Types supported:</B> byte 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t i, k;
  char bufname[1024];
  int32_t namelen, begin, end, step;
  struct xvimage * image_in;
  struct xvimage * image_out;
  int32_t rs, cs, ds, ps, N;
  uint8_t *I;
  uint8_t *O;

  if ((argc != 3) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm [begin end step] name_prefix\n", argv[0]);
    exit(1);
  }

  image_in = readimage(argv[1]);
  if (image_in == NULL)
  {
    fprintf(stderr, "%s: readimage failed: %s\n", argv[0], argv[1]);
    exit(1);
  }
  rs = rowsize(image_in);   /* taille ligne */
  cs = colsize(image_in);   /* taille colonne */
  ds = depth(image_in);     /* nb plans */
  ps = rs * cs;             /* taille plan */
  N = ps * ds;              /* taille image */
  I = UCHARDATA(image_in);

  if (argc == 6)
  {
    begin = atoi(argv[2]);
    end = atoi(argv[3]);
    step = atoi(argv[4]);
  }
  else
  {
    begin = 0;
    end = ds-1;
    step = 1;
  }

  strcpy(bufname, argv[argc - 1]);
  namelen = strlen(argv[argc - 1]);
  
  image_out = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if (image_out == NULL)
  {   
    fprintf(stderr,"%s : allocimage failed\n", argv[0]);
    exit(1);
  }
  O = UCHARDATA(image_out);

  for (k = begin; k <= end; k += step)
  {  
    bufname[namelen] =   '0' + (k / 1000) % 10;
    bufname[namelen+1] = '0' + (k / 100) % 10;
    bufname[namelen+2] = '0' + (k / 10) % 10;
    bufname[namelen+3] = '0' + (k / 1) % 10;
    bufname[namelen+4] = '.';
    bufname[namelen+5] = 'p';
    bufname[namelen+6] = 'g';
    bufname[namelen+7] = 'm';
    bufname[namelen+8] = '\0';

    for (i = 0; i < ps; i++) O[i] = I[k * ps + i];
    writeimage(image_out, bufname);

  } /* for k */

  freeimage(image_in);
  freeimage(image_out);
  return 0;
} /* main() */
