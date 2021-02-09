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
/* \file gettree.c

\brief Get the structure of a tree

<B>Usage:</B> gettree in.pgm connex [ext] out.pgm

<B>Description:</B>
Level of the branch section defined by its level in the tree. Returns the
structure of the tree.

The distance d used depends on the parameter \b connex :
\li 26: 26-distance in 3d

The parameter <B>ext</B> is the size of the authorized extension of an intersection
(ext > 0; by default: ext = 1).

<B>Types supported:</B> byte 3D

<B>Category:</B> morpho
\ingroup   morpho

\author C�dric All�ne
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgettree.h>

/* =============================================================== */
int main(int argc, char **argv)
/* /include/lgettree.h */
/* /src/lib/lgettree.c */
/* /src/com/gettree.c */
/* =============================================================== */
{
  int32_t connex, extension;
  struct xvimage * image;
  struct xvimage * result;
  
  struct noeudArbre * racine;
  
  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm connex [ext] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8) && (connex != 6) && (connex != 18) && (connex != 26))
  {
    fprintf(stderr, "usage: %s in.pgm {4|8|6|18|26} [ext] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    extension = atoi (argv [3]);
    if (extension <= 0)
    {
      fprintf(stderr, "usage: %s in.pgm connex [ext] fileout.pgm\t(ext > 0)\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    extension = 1;
  }
  
  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  racine = lgettree(image, connex, extension, result);
  if (racine == NULL)
  {
    fprintf(stderr, "%s: lgettree failed\n", argv[0]);
    exit(1);
  }
  
  result->name = NULL;
  writeimage(result, argv[argc-1]);
  
  printf ("\nResultats obtenus:\n");
  printf ("  Niveau de branche max: %3d\n", racine->levelMax);
  printf ("  Niveau de branche min: %3d\n", racine->levelMin);
  printf ("  Nombre de feuilles   : %3d\n", NbFeuilles (racine, 1));
  
  Choix1 (racine, result);
  
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */


