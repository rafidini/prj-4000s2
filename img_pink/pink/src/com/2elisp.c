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
/* \file 2elisp.c

\brief

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#define HEAD1 "(defun zc (d f) (hilit-region-set-face d f 'purple-bold))"
#define HEAD2 "(defun zd (d f) (hilit-region-set-face d f 'purple))"
#define HEAD3 "(defun za (d f) (hilit-region-set-face d f 'bold))"

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
  struct xvimage * bold = NULL;
  struct xvimage * underl = NULL;
  FILE *fd = NULL;
  int32_t rs, cs, x, y;

  if ((argc < 3) || (argc > 5))
  {
    fprintf(stderr, "usage: %s image.pgm [bold.pgm [underl.pgm]] out.tex \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "2elisp: readimage failed\n");
    exit(1);
  }
  rs = image->row_size;
  cs = image->col_size;
  if ((rs > 20) || (cs > 20))
  {
    fprintf(stderr, "2elisp: image too big\n");
    exit(1);
  }

  if (argc > 3)
  {
     bold = readimage(argv[2]);
     if (bold == NULL)
     {
       fprintf(stderr, "2elisp: readimage failed\n");
       exit(1);
     }
     if ((bold->row_size != rs) || (bold->col_size != cs))
     {
       fprintf(stderr, "2elisp: incompatible image sizes\n");
       exit(1);
     }
  }

  if (argc > 4)
  {
     underl = readimage(argv[3]);
     if (underl == NULL)
     {
       fprintf(stderr, "2elisp: readimage failed\n");
       exit(1);
     }
     if ((underl->row_size != rs) || (underl->col_size != cs))
     {
       fprintf(stderr, "2elisp: incompatible image sizes\n");
       exit(1);
     }
  }

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "2elisp: cannot open file: %s\n", argv[argc - 1]);
    exit(1);
  }

  for (y = 0; y < cs; y++)
  {
    for (x = 0; x < rs; x++)
    {
      fprintf(fd, "%4d", *(UCHARDATA(image) + y * rs + x));
    } /* for x */
    fprintf(fd, "\n\n");
  } /* for y */

  fprintf(fd, ";%s\n\n", image->name);    
  fprintf(fd, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  fprintf(fd, "%s\n%s\n%s\n", HEAD1, HEAD2, HEAD3);

  for (y = 0; y < cs; y++)
  {
    for (x = 0; x < rs; x++)
    {
      if (bold && *(UCHARDATA(bold) + y * rs + x))
      {
        if (underl && *(UCHARDATA(underl) + y * rs + x))
          fprintf(fd, "(zc %d %d)\n", (y*rs+x)*4 + 2*y + 1, (y*rs+x)*4 + 2*y + 5);
        else
          fprintf(fd, "(za %d %d)\n", (y*rs+x)*4 + 2*y + 1, (y*rs+x)*4 + 2*y + 5);
      }
      else
      {
        if (underl && *(UCHARDATA(underl) + y * rs + x))
          fprintf(fd, "(zd %d %d)\n", (y*rs+x)*4 + 2*y + 1, (y*rs+x)*4 + 2*y + 5);
      }
    } /* for x */
  } /* for y */

  fclose(fd);
  freeimage(image);
  if (argc > 4) freeimage(underl);
  if (argc > 3) freeimage(bold);
  return 0;
}

