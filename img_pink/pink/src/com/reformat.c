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
/* \file reformat.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <mccodimage.h>
#include <mcimage.h>

/* ==================================== */
struct xvimage * readimage_old(char *filename)
/* ==================================== */
#undef F_NAME
#define F_NAME "readimage_old"
{
#define BUFFERSIZE 10000
  char buffer[BUFFERSIZE];
  FILE *fd = NULL;
  int32_t rs, cs, d, ndgmax, N, i;
  struct xvimage * image;
  int32_t ascii;  
  int32_t typepixel;
  int32_t c;
  double xdim=1.0, ydim=1.0, zdim=1.0;

#ifdef UNIXIO
  fd = fopen(filename,"r");
#endif
#ifdef DOSIO
  fd = fopen(filename,"rb");
#endif
  if (!fd)
  {
    fprintf(stderr, "%s: file not found: %s\n", F_NAME, filename);
    return NULL;
  }

  fgets(buffer, BUFFERSIZE, fd); /* P5: raw byte bw  ; P2: ascii bw */
                                 /* P6: raw byte rgb ; P3: ascii rgb */
                                 /* P7: raw byte 3d  ; P4: ascii 3d  ==  extensions MC */
                                 /* P8: raw int32_t 2d-3d  ==  extension MC */
                                 /* P9: raw float 2d-3d  ==  extension MC */
                                 /* PA: ascii float 2d-3d  ==  extension LN */
                                 /* PB: ascii int32_t 2d-3d  ==  extension MC */

  if (buffer[0] != 'P')
  {   fprintf(stderr,"%s : invalid image format\n", F_NAME);
      return NULL;
  }
  switch (buffer[1])
  {
    case '2':
    case '4': ascii = 1; typepixel = VFF_TYP_1_BYTE; break;
    case '5':
    case '7': ascii = 0; typepixel = VFF_TYP_1_BYTE; break;
    case '8': ascii = 0; typepixel = VFF_TYP_4_BYTE; break;
    case '9': ascii = 0; typepixel = VFF_TYP_FLOAT; break;
    case 'A': ascii = 1; typepixel = VFF_TYP_FLOAT; break;
    case 'B': ascii = 1; typepixel = VFF_TYP_4_BYTE; break;
    default:
      fprintf(stderr,"%s : invalid image format\n", F_NAME);
      return NULL;
  } /* switch */

  do 
  {
    fgets(buffer, BUFFERSIZE, fd); /* commentaire */
    if (strncmp(buffer, "#xdim", 5) == 0)
      sscanf(buffer+5, "%lf", &xdim);
    else if (strncmp(buffer, "#ydim", 5) == 0)
      sscanf(buffer+5, "%lf", &ydim);
    else if (strncmp(buffer, "#zdim", 5) == 0)
      sscanf(buffer+5, "%lf", &zdim);
  } while (!isdigit(buffer[0]));

  c = sscanf(buffer, "%d %d %d", &rs, &cs, &d);
  if (c == 2) d = 1;
  else if (c != 3)
  {   fprintf(stderr,"%s : invalid image format\n", F_NAME);
      return NULL;
  }

  fgets(buffer, BUFFERSIZE, fd);

  sscanf(buffer, "%d", &ndgmax);
  N = rs * cs * d;

  image = allocimage(NULL, rs, cs, d, typepixel);
  if (image == NULL)
  {   fprintf(stderr,"%s : alloc failed\n", F_NAME);
      return(NULL);
  }
  image->xdim = xdim;
  image->ydim = ydim;
  image->zdim = zdim;

  if (typepixel == VFF_TYP_1_BYTE)
  {
    if (ascii)
    {
      if (ndgmax == 255)
        for (i = 0; i < N; i++)
        {
          fscanf(fd, "%d", &c);
          UCHARDATA(image)[i] = (uint8_t)c;
        } /* for i */
      else if (ndgmax == 65535)
        for (i = 0; i < N; i++)
        {
          fscanf(fd, "%d", &c);
          UCHARDATA(image)[i] = (uint8_t)(c/256);
        } /* for i */
      else
      {
        fprintf(stderr,"%s : wrong ndgmax = %d\n", F_NAME, ndgmax);
        return(NULL);
      }
    }
    else
    {
      int32_t ret = fread(UCHARDATA(image), sizeof(char), N, fd);
      if (ret != N)
      {
        fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, N, ret);
        return(NULL);
      }
    }
  } /* if (typepixel == VFF_TYP_1_BYTE) */
  else
  if (typepixel == VFF_TYP_4_BYTE)
  {
    if (ascii)
    {
      long int tmp;
      for (i = 0; i < N; i++)
      {
        fscanf(fd, "%ld", &tmp); SLONGDATA(image)[i] = (uint32_t)tmp;
      } /* for i */
    }
    else 
    {
      int32_t ret = fread(SLONGDATA(image), sizeof(int32_t), N, fd);
      if (ret != N)
      {
        fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, N, ret);
        return(NULL);
      }
    }
  } /* if (typepixel == VFF_TYP_4_BYTE) */
  else
  if (typepixel == VFF_TYP_FLOAT)
  {
    if (ascii)
    {
      for (i = 0; i < N; i++)
      {
        fscanf(fd, "%f", &(FLOATDATA(image)[i]));
      } /* for i */
    }
    else 
    {
      int32_t ret = fread(FLOATDATA(image), sizeof(float), N, fd);
      if (ret != N)
      {
        fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, N, ret);
        return(NULL);
      }
    }
  } /* if (typepixel == VFF_TYP_FLOAT) */

  fclose(fd);

  return image;
} /* readimage_old() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s file.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage_old(argv[1]);
  if (image == NULL)
  {   fprintf(stderr,"reformat : readimage failed\n");
      exit(1);
  }

  writeimage(image, argv[1]);
  freeimage(image);

  return 0;
} /* main */
