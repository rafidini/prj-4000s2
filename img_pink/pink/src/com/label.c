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
/*! \file label.c

\brief labeling of some flat zones of a grayscale or a binary image

<B>Usage:</B> label in.pgm connex {fgd|bgd|min|max|pla} out.pgm

<B>Description:</B>
The argument \b connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).
The argument <B><fgd|bgd|min|max|pla></B> selects the kind of flat zone
to be labeled:
\li \b fgd selects foreground components for a binary image
\li \b bgd selects background components for a binary image
\li \b min selects regional minima
\li \b max selects regional maxima
\li \b pla selects all flat zones (plateaux).
The output image \b out.pgm has the type "int32_t".

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> connect
\ingroup  connect

\author Michel Couprie
*/

/*
%TEST label %IMAGES/2dbyte/binary/b2hebreu.pgm 8 fgd %RESULTS/label_b2hebreu_8_fgd.pgm
%TEST label %IMAGES/2dbyte/binary/b2hebreu.pgm 4 fgd %RESULTS/label_b2hebreu_4_fgd.pgm
%TEST label %IMAGES/2dbyte/binary/b2hebreu.pgm 8 bgd %RESULTS/label_b2hebreu_8_bgd.pgm
%TEST label %IMAGES/2dbyte/binary/b2hebreu.pgm 4 bgd %RESULTS/label_b2hebreu_4_bgd.pgm
%TEST label %IMAGES/2dbyte/gray/g2hebreu.pgm 4 min %RESULTS/label_g2hebreu_4_min.pgm
%TEST label %IMAGES/2dbyte/gray/g2hebreu.pgm 4 max %RESULTS/label_g2hebreu_4_max.pgm
%TEST label %IMAGES/2dbyte/gray/g2hebreu.pgm 4 pla %RESULTS/label_g2hebreu_4_pla.pgm
%TEST label %IMAGES/3dbyte/gray/g3a.pgm 6 pla %RESULTS/label_g3a_6_pla.pgm
%TEST label %IMAGES/3dbyte/gray/g3a.pgm 18 pla %RESULTS/label_g3a_18_pla.pgm
%TEST label %IMAGES/3dbyte/gray/g3a.pgm 26 pla %RESULTS/label_g3a_26_pla.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>
#include <llabelextrema.h>
#include <llabelplateaux.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t nblabels, connex;
  struct xvimage * image;
  struct xvimage * result;
  int32_t function;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex {fgd|bgd|min|max|pla} fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  if (strcmp(argv[3], "fgd") == 0) function = LABFGD; else
  if (strcmp(argv[3], "bgd") == 0) function = LABBGD; else
  if (strcmp(argv[3], "min") == 0) function = LABMIN; else
  if (strcmp(argv[3], "max") == 0) function = LABMAX; else
  if (strcmp(argv[3], "pla") == 0) function = LABPLA; else
  {
    fprintf(stderr, "usage: %s filein.pgm connex {fgd|bgd|min|max|pla} fileout.pgm\n", argv[0]);
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  switch (function)
  {
  case LABMIN:
  case LABMAX:
    if (! llabelextrema(image, connex, function, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }
    break;
  case LABPLA:
    if (! llabelplateaux(image, connex, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelplateaux failed\n", argv[0]);
      exit(1);
    }
    break;
  case LABFGD:
    if (! llabelbin(image, connex, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelbin failed\n", argv[0]);
      exit(1);
    }    
    break;
  case LABBGD:
    (void)linverse(image);
    if (! llabelbin(image, connex, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelbin failed\n", argv[0]);
      exit(1);
    }    
    break;
  } // switch (function)

  writeimage(result, argv[argc-1]);
  freeimage(result);
  freeimage(image);

  return 0;
} /* main */


