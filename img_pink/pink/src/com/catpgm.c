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
/*! \file catpgm.c

\brief catenates a sequence of 2d (resp. 1d) images into a 3d (resp. 2d) image

<B>Usage:</B> catpgm name_prefix index_begin index_end [xdim ydim zdim] name_out

<B>Description:</B>
Original file names must be of the form: <B>name_prefix</B>nnnn.pgm, 
where nnnn is a four digit decimal integer. The integers <B>index_begin</B>
and <B>index_end</B> specify respectively the first and the last index of the series.

<B>Types supported:</B> sequence of byte 1d or 2d images

<B>Category:</B>  convert
\ingroup   convert

\author Michel Couprie
*/

/* 
  Michel Couprie - mars 2001
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
  int32_t i, j, debut, fin, seqsize;
  char bufname[1024];
  int32_t namelen;
  struct xvimage * image_in;
  struct xvimage * image_out;
  int32_t rs, cs, N;
  uint8_t *I;
  uint8_t *O;

  if ((argc != 5) && (argc != 8))
  {
    fprintf(stderr, "usage: %s name_prefix index_begin index_end [xdim ydim zdim] name_out\n", argv[0]);
    exit(1);
  }

  strcpy(bufname, argv[1]);
  debut = atoi(argv[2]);  
  fin = atoi(argv[3]);  
  seqsize = fin - debut + 1;
  namelen = strlen(argv[1]);
  
  bufname[namelen] =   '0' + (debut / 1000) % 10;
  bufname[namelen+1] = '0' + (debut / 100) % 10;
  bufname[namelen+2] = '0' + (debut / 10) % 10;
  bufname[namelen+3] = '0' + (debut / 1) % 10;
  bufname[namelen+4] = '.';
  bufname[namelen+5] = 'p';
  bufname[namelen+6] = 'g';
  bufname[namelen+7] = 'm';
  bufname[namelen+8] = '\0';

  image_in = readimage(bufname);
  if (image_in == NULL)
  {
    fprintf(stderr, "%s: readimage failed: %s\n", argv[0], bufname);
    exit(1);
  }
  rs = rowsize(image_in);   /* taille ligne */
  cs = colsize(image_in);   /* taille colonne */
  N = rs * cs;              /* taille image */
  I = UCHARDATA(image_in);

  if (cs > 1)
    image_out = allocimage(NULL, rs, cs, seqsize, VFF_TYP_1_BYTE);
  else
    image_out = allocimage(NULL, rs, seqsize, 1, VFF_TYP_1_BYTE);

  if (image_out == NULL)
  {   
    fprintf(stderr,"%s : allocimage failed\n", argv[0]);
    exit(1);
  }
  O = UCHARDATA(image_out);

  for (i = 0; i < N; i++) O[i] = I[i];
  freeimage(image_in);

  for (j = 1; j < seqsize; j++)
  {  
    bufname[namelen] =   '0' + ((debut+j) / 1000) % 10;
    bufname[namelen+1] = '0' + ((debut+j) / 100) % 10;
    bufname[namelen+2] = '0' + ((debut+j) / 10) % 10;
    bufname[namelen+3] = '0' + ((debut+j) / 1) % 10;
    bufname[namelen+4] = '.';
    bufname[namelen+5] = 'p';
    bufname[namelen+6] = 'g';
    bufname[namelen+7] = 'm';
    bufname[namelen+8] = '\0';

    image_in = readimage(bufname);
    if (image_in == NULL)
    {
      fprintf(stderr, "%s: readimage failed: %s\n", argv[0], bufname);
      exit(1);
    }
    I = UCHARDATA(image_in);

    for (i = 0; i < N; i++) O[j * N + i] = I[i];
    freeimage(image_in);

  } /* for j */

  if (argc == 8)
  {
    image_out->xdim = atof(argv[4]);
    image_out->ydim = atof(argv[5]);
    image_out->zdim = atof(argv[6]);
  }
  else
    image_out->xdim = image_out->ydim = image_out->zdim = 0.0;

  writeimage(image_out, argv[argc-1]);
  freeimage(image_out);
  return 0;
} /* main() */
