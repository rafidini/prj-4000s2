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
/* operateur de reconstruction par dilatation de g sous f */
/* Camille Couprie - novembre 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mclifo.h>
#include <mcutil.h>
#include <lselcomp.h>

/* ==================================== */
int32_t lselcomp(struct xvimage *f, int32_t connex, int32_t x, int32_t y, int32_t z) 
/* ==================================== */
{
  int32_t i, j, k, h, couleurobjet, couleurfond;                 /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(f); /* l'image originale */
  Lifo * LIFO;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "lselcomp: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  LIFO = CreeLifoVide(N);
  if (LIFO[0] == NULL)
  {   fprintf(stderr,"lselcomp() : CreeLifoVide failed\n");
      return(0);
  }
  
  i = y*rs + x;
  couleurobjet = F[i];
  if (couleurobjet != 0) couleurfond = 0; else couleurfond = 255;
  Lifopush (LIFO, i);
  if (connex == 4) p = 2;
  if (connex == 8) p = 1;
  
  while ( ! LifoVide(LIFO))
  {
    i = LifoPop(LIFO);
    F[i] = 100;
    for (k = 0; k < 8; k = k + p)
    {
      j = voisin(i, k, rs, N);
      if ((j != -1) && (couleurinitiale == F[j])) LifoPush(LIFO, j);
    }  
  }
  
  for ( h = 0; h < N; h++ )
  {
    if ( F[h] == couleurobjet) F[h] = couleurfond;
    if ( F[h] == 100 )         F[h] = couleurobjet;
  }
  
  LifoTermine(LIFO);
  return 1;
}











