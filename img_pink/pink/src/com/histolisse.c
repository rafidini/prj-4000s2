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
/*! \file histolisse.c

\brief smoothed histogram of the gray values

<B>Usage:</B> histolisse in.pgm L [mask.pgm] out.pgm

<B>Description:</B>
Generates a binary image which contains the graphical representation
of the smoothed histogram of \b in.pgm (masked by the binary image
\b mask.pgm, if given). \b L : smoothing coefficient, 0<L<255.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> histo
\ingroup  histo

\author Cedric Allene 2003
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhisto.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imhist;
  struct xvimage * mask = NULL;
  index_t * histo, somme;
  int32_t x, y, z, lissage;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm L [mask.pgm] out.pgm\n", argv[0]);
    exit(1);
  }

  lissage = atoi (argv[2]);
  if ((lissage < 1) || (lissage > 254))
  {
    fprintf(stderr, "%s: 0<L<255 \n", argv[0]);
    exit(1);
  }
  
  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  histo = (index_t *)calloc((NDG_MAX - NDG_MIN + 1) * sizeof(index_t), 1);
  if (histo == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    mask = readimage(argv[3]);
    if (mask == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lhisto(image, mask, histo))
  {
    fprintf(stderr, "%s: function lhisto failed\n", argv[0]);
    exit(1);
  }

  
/* Lissage de l'histogramme */
  for (x=0; x<=NDG_MAX - NDG_MIN; x++)
  {
    somme = 0;
    z=0;
    for (y=-lissage/2; y<=lissage/2; y++)
    {
      z++;
      if ((x-y)>=0) somme = somme + histo[x-y];
    }
    histo [x]=(index_t)(somme/z);
  }


/* Ecriture de l'image histogramme */
  if (! lhisto2image(histo, NDG_MAX - NDG_MIN + 1, &imhist))
  {
    fprintf(stderr, "%s: function lhisto2image failed\n", argv[0]);
    exit(1);
  }

  writeimage(imhist, argv[argc - 1]);
  free(histo);
  freeimage(image);
  freeimage(imhist);
  if (mask) freeimage(mask);
  return 0;
} /* main */
