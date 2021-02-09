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
/*! \file pgmascmode.c

\brief converts a pgm file to ascii mode

<B>Usage:</B> pgmascmode filename [outfilename | "wh" outfilename]

<B>Description:</B> Converts a pgm file to ascii mode.
If only parameter \b filename is given, then the result
is also named \b filename .
If the keyword "wh" is used as second parameter (stands for "without header"), then 
an ascii file without header is produced.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* ==================================== */
void writeascimage_without_header(struct xvimage * image, char *filename)
/* ==================================== */
#undef F_NAME
#define F_NAME "writeascimage"
{
  FILE *fd = NULL;
  int32_t rs, cs, ps, d, N, i;

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    exit(0);
  }

  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  ps = rs * cs;
  N = ps * d;

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    if (N > 8000) // grandes images : pas de padding (blancs)
    { 
      for (i = 0; i < N; i++)
      {
        if (i % rs == 0) fprintf(fd, "\n");
        if (i % ps == 0) fprintf(fd, "\n");
        fprintf(fd, "%d ", (int32_t)(UCHARDATA(image)[i]));
      } /* for i */
    }
    else
    { 
      for (i = 0; i < N; i++)
      {
        if (i % rs == 0) fprintf(fd, "\n");
        if (i % ps == 0) fprintf(fd, "\n");
        fprintf(fd, "%3d ", (int32_t)(UCHARDATA(image)[i]));
      } /* for i */
    }
    fprintf(fd, "\n");
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    for (i = 0; i < N; i++)
    {
      if (i % rs == 0) fprintf(fd, "\n");
      if (i % ps == 0) fprintf(fd, "\n");
      fprintf(fd, "%ld ", (long int)(SLONGDATA(image)[i]));
    } /* for i */
    fprintf(fd, "\n");
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    for (i = 0; i < N; i++)
    {
      if (i % rs == 0) fprintf(fd, "\n");
      if (i % ps == 0) fprintf(fd, "\n");
      fprintf(fd, "%8g ", FLOATDATA(image)[i]);
    } /* for i */
    fprintf(fd, "\n");
  }
  else if (datatype(image) == VFF_TYP_DOUBLE)
  {
    for (i = 0; i < N; i++)
    {
      if (i % rs == 0) fprintf(fd, "\n");
      if (i % ps == 0) fprintf(fd, "\n");
      fprintf(fd, "%8g ", DOUBLEDATA(image)[i]);
    } /* for i */
    fprintf(fd, "\n");
  }
  fclose(fd);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if ((argc != 2) && (argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filename [outfilename | 'wh' outfilename] \n", argv[0]);
    exit(1);
  }

  if ((argc == 4) && ((argv[2][0] != 'w') || (argv[2][1] != 'h')))
  {
    fprintf(stderr, "usage: %s filename [outfilename | 'wh' outfilename] \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {   fprintf(stderr,"%s: readimage failed\n", argv[0]);
      exit(1);
  }

  if (argc == 4)
    writeascimage_without_header(image, argv[argc-1]);
  else
    writeascimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
