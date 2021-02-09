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
/*! \file seuilauto.c

\brief automatic threshold

<B>Usage:</B> seuilauto in.pgm L {min|max} [n] out.pgm

<B>Description:</B>
Determine automatically a threshold thanks to the smoothed histogram
of the picture (\b L : smoothing coefficient, 0<L<255).
If n is given, it indicates the number of peaks on the histogram to be kept.
If n isn't given, the threshold separates the picture in two approximatively equivalent groups of points.
The argument <B><min|max></B> selects the part of the picture
to be kept:

\li \b min: Every pixel < \b smin gets value \b vmin and every pixel >= \b smax gets value \b vmax 
\li \b max: Every pixel > \b smin gets value \b vmin and every pixel <= \b smax gets value \b vmax 

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> arith
\ingroup  arith

\author Cedric Allene 2003
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseuil.h>
#include <lhisto.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t seuil;
  struct xvimage * image;
  struct xvimage * imagebin;
  index_t rs;          /* taille ligne */
  index_t cs;          /* taille colonne */
  index_t d;           /* nb. plans */
  index_t N;           /* taille image */
  uint8_t *F;
  int32_t *FL;
  index_t * histo;
  index_t * histolisse, somme;
  int32_t x, y, diff, compteur, lissage;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm L {min|max} [n] fileout.pgm\n", argv[0]);
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

  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d; 


/* Initialisation de l'histogramme */
  histo = (index_t *)calloc((NDG_MAX - NDG_MIN + 1) * sizeof(index_t), 1);
  if (histo == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }


/* Calcul de l'histogramme */
  if (! lhisto(image, NULL, histo))
  {
    fprintf(stderr, "%s: function lhisto failed\n", argv[0]);
    exit(1);
  }

  
/* Initialisation de l'histogramme lisse*/
  histolisse = (index_t *)calloc((NDG_MAX - NDG_MIN + 1) * sizeof(index_t), 1);
  if (histolisse == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }


/* Lissage de l'histogramme */
  for (x=0; x<=255; x++)
  {
    somme = 0;
    for (y=-lissage/2; y<=lissage/2; y++)
      if ((x-y)>=0) somme = somme + histo[x-y];
    histolisse [x]=(index_t)(somme/(lissage+1));
  }


/* Recherche du minimum */
  seuil=0;

  if (argc == 5)
  {
    somme=histo[0]+histo[1]-histo[254]-histo[255];
    diff=N;
    for (x=2; x<254; x++)
    {
      somme = somme + histo [x];   /* points déjà considérés */
      if ((histolisse[x-1]>=histolisse[x]) && (histolisse[x]<histolisse[x+1]))
        if (abs(N-2*somme) < diff)
        {
          seuil=x;
          diff = abs(N-2*somme);
        }
    }
  }
  else
  {
    compteur=atoi (argv[4]);
    x=2;
    while ((x<254) && (compteur!=0))
    {
      if ((histolisse[x-1]>=histolisse[x]) && (histolisse[x]<histolisse[x+1]))
      {
        compteur--;
        if (compteur==0) seuil=x;
      }
      x++;
    }
  }


/* Seuillage */
  if (strcmp(argv[3], "max") == 0)
  {
    if (! lseuil(image, seuil))
    {
      fprintf(stderr, "%s: function lseuil failed\n", argv[0]);
      exit(1);
    }
  }
  else if (strcmp(argv[3], "min") == 0)
  {
    if (! lseuil3(image, 0, seuil))
    {
      fprintf(stderr, "%s: function lseuil3 failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "usage: %s filein.pgm L {min|max} fileout.pgm\n", argv[0]);
    exit(1);
  }


/* Ecriture de l'image résultat */
  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    imagebin = allocimage(image->name, rs, cs, d, VFF_TYP_1_BYTE);
    if (imagebin == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    F = UCHARDATA(imagebin);
    FL = SLONGDATA(image);
    for (x = 0; x < N; x++) F[x] = (uint8_t)FL[x];
    writeimage(imagebin, argv[argc-1]);
    freeimage(imagebin);
    freeimage(image);
  }
  else
  {
    writeimage(image, argv[argc-1]);
    freeimage(image);
  }
  return seuil;
} /* main */
