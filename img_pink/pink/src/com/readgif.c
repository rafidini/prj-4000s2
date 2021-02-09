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
/* \file readgif.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  Lecteur de format GIF

  Michel Couprie - avril 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
void read_global_color_table(FILE *fd, int32_t size)
/* =============================================================== */
{
  int32_t ret;
  char * color_table;

  printf("reading global color table : size = %d\n", size);
  color_table = (char *)calloc(1,size * sizeof(char));  
  if (color_table == NULL)
  {
    fprintf(stderr, "read_global_color_table: MALLOC ERROR\n");
    exit(1);
  }
  ret = fread(color_table, sizeof(char), size, fd);
  if (ret != size)
  {
    fprintf(stderr, "read_global_color_table: FORMAT ERROR\n");
    exit(1);
  }

  free(color_table);
} /* read_global_color_table() */


/* =============================================================== */
void read_local_color_table(FILE *fd, int32_t size)
/* =============================================================== */
{
  int32_t ret;
  char * color_table;

  printf("reading local color table : size = %d\n", size);
  color_table = (char *)calloc(1,size * sizeof(char));  
  if (color_table == NULL)
  {
    fprintf(stderr, "read_local_color_table: MALLOC ERROR\n");
    exit(1);
  }
  ret = fread(color_table, sizeof(char), size, fd);
  if (ret != size)
  {
    fprintf(stderr, "read_local_color_table: FORMAT ERROR\n");
    exit(1);
  }

  free(color_table);
} /* read_local_color_table() */

/* =============================================================== */
/* =============================================================== */
/* =============================================================== */

/* =============================================================== */
void read_header(FILE *fd)
/* =============================================================== */
{
  char signature[7];
  int32_t i;
  signature[6] = '\0';
  for (i = 0; i < 6; i++) signature[i] = (char)getc(fd);
  printf("signature = %s\n", signature);
} /* read_header() */

/* =============================================================== */
void read_logical_screen(FILE *fd)
/* =============================================================== */
{
  int32_t h, l;
  int32_t lswidth , lsheight;
  uint8_t flags, bgcolindex, pixasprat;

  l = getc(fd); h = getc(fd); lswidth = h * 256 + l;
  l = getc(fd); h = getc(fd); lsheight  = h * 256 + l;
  printf("lswidth = %ld ; lsheight = %ld\n", (long int)lswidth, (long int)lsheight);
  flags = (uint8_t)getc(fd);  
  bgcolindex = (uint8_t)getc(fd);  
  pixasprat = (uint8_t)getc(fd);  

  if (flags & 0x80)
  {
    int32_t size;
    flags &= 0x7;
    size = 3 * (1 << (flags + 1));
    read_global_color_table(fd, size);
  }
} /* read_logical_screen() */

/* =============================================================== */
void read_graphic_extension(FILE *fd)
/* =============================================================== */
{
  int32_t h, l, c;
  int32_t delay;
  uint8_t flags, transcolindex;

  printf("reading graphic extension\n");
  c = getc(fd); /* get block size */
  if (c != 4)
  {
    fprintf(stderr, "FORMAT ERROR 2\n");
    exit(1);
  }
  flags = (uint8_t)getc(fd);  

  l = getc(fd); h = getc(fd); delay = h * 256 + l;

  transcolindex = (uint8_t)getc(fd);  
  printf("delay = %d, transp. col. index = %d\n", delay, transcolindex);

  c = getc(fd); /* swallow terminator */
  if (c != 0)
  {
    fprintf(stderr, "FORMAT ERROR 3\n");
    exit(1);
  }

} /* read_graphic_extension() */

/* =============================================================== */
void read_comment_extension(FILE *fd)
/* =============================================================== */
{
  char buf[256];
  int32_t c, ret;

  printf("reading comment extension\n");

  c = getc(fd); /* get block size */
  while (c != 0)
  {
#ifdef DEBUG
    printf("reading data block, length = %d\n", c); 
#endif
    ret = fread(buf, sizeof(char), c, fd);
    if (ret != c)
    {
      fprintf(stderr, "read_comment_extension: FORMAT ERROR, %ld byte read\n", (long int)ret);
      exit(1);
    }
    printf("comment: %s", buf);
    c = getc(fd); /* get next block size */
  }

} /* read_comment_extension() */

/* =============================================================== */
void read_application_extension(FILE *fd)
/* =============================================================== */
{
  char buf[256];
  int32_t c, ret;

  printf("reading application extension\n");

  c = getc(fd); /* get block size */
  if (c != 11)
  {
    fprintf(stderr, "read_application_extension: FORMAT ERROR\n");
    exit(1);
  }
  ret = fread(buf, sizeof(char), c, fd); /* skip block */
  if (ret != c)
  {
    fprintf(stderr, "read_application_extension: FORMAT ERROR, %ld byte read\n", (long int)ret);
    exit(1);
  }
  printf("application tag : %x %x %x\n", buf[8], buf[9], buf[10]);
  buf[8] = '\0';
  printf("application name: %s\n", buf);

  c = getc(fd); /* get data block size */
  while (c != 0)
  {
    int32_t i;
    printf("reading data block, length = %d\n", c);
    ret = fread(buf, sizeof(char), c, fd);
    if (ret != c)
    {
      fprintf(stderr, "read_application_extension: FORMAT ERROR, %ld byte read\n", (long int)ret);
      exit(1);
    }
    printf("application specific data : \n");
    for (i = 0; i < c; i++) printf("%x\n", buf[i]);
    c = getc(fd); /* get next block size */
  }
  
} /* read_application_extension() */

/* =============================================================== */
void read_plain_text_extension(FILE *fd)
/* =============================================================== */
{
  printf("reading plain text extension\n");
} /* read_plain_text_extension() */

/* =============================================================== */
void read_extension(FILE *fd)
/* =============================================================== */
{
  int32_t c = getc(fd);
  if (c == 0xF9)
    read_graphic_extension(fd);
  else if (c == 0xFE)
    read_comment_extension(fd);
  else if (c == 0xFF)
    read_application_extension(fd);
  else if (c == 0x01)
    read_plain_text_extension(fd);
  else 
  {
    fprintf(stderr, "FORMAT ERROR 1\n");
    exit(1);
  }
} /* read_extension() */

/* =============================================================== */
void read_image_data(FILE *fd)
/* =============================================================== */
{
  int32_t LZWmincodesize, c;
  char buf[256];
  int32_t ret;

  LZWmincodesize = getc(fd);
  printf("reading image data, LZWmincodesize = %d\n", LZWmincodesize);

  c = getc(fd); /* get block size */
  while (c != 0)
  {
#ifdef DEBUG
    printf("reading data block, length = %d\n", c);
#endif
    ret = fread(buf, sizeof(char), c, fd);
    if (ret != c)
    {
      fprintf(stderr, "read_image_data: FORMAT ERROR, %ld byte read\n", (long int)ret);
      exit(1);
    }
    c = getc(fd); /* get next block size */
  }
} /* read_image_data() */

/* =============================================================== */
void read_data(FILE *fd)
/* =============================================================== */
{
  int32_t h, l, c;

  c = getc(fd);
  while (c != 0)
  {
    if (c == 0x3B) 
    {
      printf("end of GIF data stream\n");
      return;
    }
    else if (c == 0x21)
    {
      read_extension(fd);
      c = getc(fd); /* get next tag */
    }
    else if (c == 0x2C)
    { 
      int32_t leftpos, toppos, width, height;
      uint8_t flags;
      printf("reading image descriptor\n");

      l = getc(fd); h = getc(fd); leftpos = h * 256 + l;
      l = getc(fd); h = getc(fd); toppos = h * 256 + l;
      l = getc(fd); h = getc(fd); width = h * 256 + l;
      l = getc(fd); h = getc(fd); height = h * 256 + l;
      printf("leftpos = %ld ; toppos = %ld ; width = %ld ; height = %ld\n", (long int)leftpos, (long int)toppos, (long int)width, (long int)height);
      flags = (uint8_t)getc(fd);  

      if (flags & 0x80)
      {
        int32_t size;
        flags &= 0x7;
        size = 3 * (1 << (flags + 1));
        read_local_color_table(fd, size);
      }
      read_image_data(fd);
      c = getc(fd); /* get next tag */
    }  
    else 
    {
      fprintf(stderr, "read_data: FORMAT ERROR : c = %x\n", c);
      exit(1);
    }
  } /* while (c) */
} /* read_data() */

/* =============================================================== */
void read_trailer(FILE *fd)
/* =============================================================== */
{
} /* read_trailer()*/

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s in.gif\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  read_header(fd);
  read_logical_screen(fd);
  read_data(fd);

  fclose(fd);
  return 0;
}











