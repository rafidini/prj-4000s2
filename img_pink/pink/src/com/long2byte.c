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
/*! \file long2byte.c

\brief converts a "int32_t" image to a "byte" image

<B>Usage:</B> long2byte in.pgm [mode] [n] out.pgm

<B>Description:</B>

Depending on the value given for the (optional) parameter <B>mode</B>:
\li   <B>mode</B> = 0 (default) : for all x, out[x] = min(255, in[x]).
\li   <B>mode</B> = 1 : for all x, out[x] = in[x] modulo 256.
\li   <B>mode</B> = 2 : scales values in the range 0-255.
\li   <B>mode</B> = 3 : sorts the values by decreasing number of occurence in the image.
Replaces the values by their order.
Only the <B>n</B> (default 255) first values are kept.
Useful for label images.
\li   <B>mode</B> = 4 : truncation of the square root in the range 0-255.
\li   <B>mode</B> = 5 : truncation of the log in the range 0-255.

<B>Types supported:</B> int32_t 2d, int32_t 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/*
%TEST long2byte %IMAGES/2dlong/l2fish1.pgm 0 %RESULTS/long2byte_l2fish1_0.pgm
%TEST long2byte %IMAGES/2dlong/l2fish1.pgm 1 %RESULTS/long2byte_l2fish1_1.pgm
%TEST long2byte %IMAGES/2dlong/l2fish1.pgm 2 %RESULTS/long2byte_l2fish1_2.pgm
%TEST long2byte %IMAGES/2dlong/l2fish1.pgm 3 3 %RESULTS/long2byte_l2fish1_3_3.pgm
%TEST long2byte %IMAGES/2dlong/l2fish1.pgm 4 %RESULTS/long2byte_l2fish1_4.pgm
%TEST long2byte %IMAGES/2dlong/l2fish1.pgm 5 %RESULTS/long2byte_l2fish1_5.pgm
*/

/*
   Michel Couprie - mai 1998

   Modif : decembre 1999 - mode 3 (trunchisto)
   Modif : decembre 2009 - trunchisto sorts in decreasing order
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <lhisto.h>
#include <math.h>

/* =============================================================== */
static int32_t Partitionner(int32_t *A, index_t *T, int32_t p, int32_t r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : les elements q tq T[A[q]] >= T[A[p]] et les autres.
*/
{
  int32_t t;
  index_t x = T[A[p]];
  int32_t i = p - 1;
  int32_t j = r + 1;
  while (1)
  {
    do j--; while (T[A[j]] < x);
    do i++; while (T[A[i]] > x);
    if (i < j) { t = A[i]; A[i] = A[j]; A[j] = t; }
    else return j;
  } /* while (1) */   
} /* Partitionner() */

/* =============================================================== */
static int32_t PartitionStochastique(int32_t *A, index_t *T, int32_t p, int32_t r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : les elements k tels que T[A[k]] >= T[A[q]] et les autres, 
  avec q tire au hasard dans [p,r].
*/
{
  int32_t t, q;

  q = p + (rand() % (r - p + 1));
  t = A[p];         /* echange A[p] et A[q] */
  A[p] = A[q]; 
  A[q] = t;
  return Partitionner(A, T, p, r);
} /* PartitionStochastique() */

/* =============================================================== */
static void TriRapideStochastique (int32_t * A, index_t *T, int32_t p, int32_t r)
/* =============================================================== */
/* 
  trie les valeurs du tableau A de l'indice p (compris) a l'indice r (compris) 
  par ordre décroissant 
*/
{
  int32_t q; 
  if (p < r)
  {
    q = PartitionStochastique(A, T, p, r);
    TriRapideStochastique (A, T, p, q) ;
    TriRapideStochastique (A, T, q+1, r) ;
  }
} /* TriRapideStochastique() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * imagelong;
  struct xvimage * imagebyte;
  int32_t *L;
  uint8_t *B;
  int32_t i;
  int32_t mode = 0;
  int32_t Max;
  index_t *histo;
  index_t *newvals;
  int32_t nbval, nbnewval;
  int32_t *index;
  double t;
  index_t x, rs, cs, d, N;

  if ((argc < 3) || (argc > 5))
  {
    fprintf(stderr, "usage: %s in1.pgm [mode] [n] out.pgm \n", argv[0]);
    fprintf(stderr, "mode = 0 (trunc) | 1 (modulo 256) | 2 (scale) | 3 (trunchisto)\n");
    fprintf(stderr, "       4 (square root) | 5 (log)\n");
    exit(1);
  }

  imagelong = readimage(argv[1]);  
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(imagelong) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "%s: image type must be int32_t\n", argv[0]);
    fprintf(stderr, "type found: %d\n", datatype(imagelong));
    exit(1);
  }


  if (argc > 3) mode = atoi(argv[2]);
  if (argc > 4) nbnewval = atoi(argv[3]); else nbnewval = 256;

  rs = rowsize(imagelong);
  cs = colsize(imagelong);
  d = depth(imagelong);
  N = rs * cs * d;
  L = SLONGDATA(imagelong);
  
  imagebyte = allocimage(imagelong->name, rs, cs, d, VFF_TYP_1_BYTE);
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  B = UCHARDATA(imagebyte);
  imagebyte->xdim = imagelong->xdim;
  imagebyte->ydim = imagelong->ydim;
  imagebyte->zdim = imagelong->zdim;

  switch(mode)
  {
    case 0:
      for (x = 0; x < N; x++)
        B[x] = (uint8_t)mcmax(mcmin(L[x],255),0);
      break;
    case 1:
      for (x = 0; x < N; x++)
        B[x] = (uint8_t)(L[x] % 256);
      break;
    case 2:
      Max = L[0];
      for (x = 0; x < N; x++) if (L[x] > Max) Max = L[x];
      if (Max > 255)
        for (x = 0; x < N; x++) 
          B[x] = (uint8_t)(L[x] * 255 / Max);
      else
        for (x = 0; x < N; x++) 
          B[x] = (uint8_t)L[x];
      break;
    case 3:
      if (! lhistolong(imagelong, NULL, &histo, &nbval))
      {
        fprintf(stderr, "%s: function lhistolong failed\n", argv[0]);
        exit(1);
      }
      index = (int32_t *)calloc(1, nbval * sizeof(int32_t));
      if (index == NULL)
      {
        fprintf(stderr, "%s: malloc failed\n", argv[0]);
        exit(1);
      }
      for (i = 0; i < nbval; i++) index[i] = i;
      TriRapideStochastique (index, histo, 1, nbval-1);
      newvals = histo; /* reutilisation de la place memoire allouee pour histo */
      for (i = 0; i < nbval; i++) newvals[i] = 0;
      for (i = 1; i < mcmin(nbval,nbnewval); i++) 
	newvals[index[i]] = i;
      for (x = 0; x < N; x++) 
        B[x] = (uint8_t)(newvals[L[x]]);
      free(histo);
      free(index);      
      break;
    case 4:
      for (x = 0; x < N; x++)
      {
        t = sqrt((double)(L[x]));
        if (t > 255) t = 255;
        B[x] = (uint8_t)arrondi(t);
      }
      break;
    case 5:
      for (x = 0; x < N; x++)
      {
        t = log((double)(L[x]));
        if (t > 255) t = 255;
        B[x] = (uint8_t)arrondi(t);
      }
      break;
    default:
      fprintf(stderr, "usage: %s in1.pgm [mode] [n] out.pgm \n", argv[0]);
      fprintf(stderr, "mode = 0 (trunc) | 1 (modulo 256) | 2 (scale) | 3 (trunchisto)\n");
      fprintf(stderr, "       4 (square root) | 5 (log)\n");
      exit(1);
  } /* switch(mode) */

  writeimage(imagebyte, argv[argc-1]);
  freeimage(imagelong);
  freeimage(imagebyte);

  return 0;
} /* main */
