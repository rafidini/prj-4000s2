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
/* \file catgif.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>catgif</B> - catenates .gif files into an animated gif

SYNOPSIS

<B>catgif</B> name_in index_begin index_end delay [i] name_out

DESCRIPTION

Original .gif file names must be of the form: <B>name_in</B>nnnn.gif, 
where nnnn is a four digit decimal integer. The integers <B>index_begin</B>
and <B>index_end</B> specify respectively the first and the last index of the series.
The integer <B>delay</B> specifies the delay, in s/100, between the display of two 
consecutive images. The optional parameter <B>i</B> must be used for an interactive 
control.

Types supported: gif

CLASS 

convert

*/

/* 
  Michel Couprie - avril 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

//#define DEBUG

/* =============================================================== */
void read_global_color_table(FILE *fd, int32_t size, FILE *fdout)
/* =============================================================== */
{
  int32_t ret;
  char * color_table;

#ifdef DEBUG
  printf("reading global color table : size = %d\n", size);
#endif
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
  fwrite(color_table, sizeof(char), size, fdout);
  free(color_table);
} /* read_global_color_table() */

/* =============================================================== */
void skip_global_color_table(FILE *fd, int32_t size)
/* =============================================================== */
{
  int32_t ret;
  char * color_table;

#ifdef DEBUG
  printf("reading global color table : size = %d\n", size);
#endif
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
} /* skip_global_color_table() */

/* =============================================================== */
void read_local_color_table(FILE *fd, int32_t size, FILE *fdout)
/* =============================================================== */
{
  int32_t ret;
  char * color_table;

#ifdef DEBUG
  printf("reading local color table : size = %d\n", size);
#endif
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
  fwrite(color_table, sizeof(char), size, fdout);
  free(color_table);
} /* read_local_color_table() */

/* =============================================================== */
void read_header(FILE *fd, FILE *fdout)
/* =============================================================== */
{
  char signature[7];
  int32_t i;
  signature[6] = '\0';
  for (i = 0; i < 6; i++) signature[i] = (char)getc(fd);
  fwrite(signature, sizeof(char), 6, fdout);
} /* read_header() */

/* =============================================================== */
void skip_header(FILE *fd)
/* =============================================================== */
{
  char signature[7];
  int32_t i;
  signature[6] = '\0';
  for (i = 0; i < 6; i++) signature[i] = (char)getc(fd);
} /* skip_header() */

/* =============================================================== */
void read_logical_screen(FILE *fd, FILE *fdout)
/* =============================================================== */
{
  int32_t h, l;
  int32_t lswidth , lsheight;
  uint8_t flags, bgcolindex, pixasprat;

  l = getc(fd); h = getc(fd); lswidth = h * 256 + l;
  fputc(l, fdout); fputc(h, fdout);
  l = getc(fd); h = getc(fd); lsheight  = h * 256 + l;
  fputc(l, fdout); fputc(h, fdout);
#ifdef DEBUG
  printf("lswidth = %ld ; lsheight = %ld\n", (long int)lswidth, (long int)lsheight);
#endif
  flags = (uint8_t)getc(fd);
  fputc(flags, fdout);
  bgcolindex = (uint8_t)getc(fd);  
  fputc(bgcolindex, fdout);
  pixasprat = (uint8_t)getc(fd);  
  fputc(pixasprat, fdout);

  if (flags & 0x80)
  {
    int32_t size;
    flags &= 0x7;
    size = 3 * (1 << (flags + 1));
    read_global_color_table(fd, size, fdout);
  }
} /* read_logical_screen() */

/* =============================================================== */
void skip_logical_screen(FILE *fd)
/* =============================================================== */
{
  int32_t h, l;
  int32_t lswidth , lsheight;
  uint8_t flags, bgcolindex, pixasprat;

  l = getc(fd); h = getc(fd); lswidth = h * 256 + l;
  l = getc(fd); h = getc(fd); lsheight  = h * 256 + l;
#ifdef DEBUG
  printf("lswidth = %ld ; lsheight = %ld\n", (long int)lswidth, (long int)lsheight);
#endif
  flags = (uint8_t)getc(fd);
  bgcolindex = (uint8_t)getc(fd);  
  pixasprat = (uint8_t)getc(fd);  

  if (flags & 0x80)
  {
    int32_t size;
    flags &= 0x7;
    size = 3 * (1 << (flags + 1));
    skip_global_color_table(fd, size);
  }
} /* skip_logical_screen() */

/* =============================================================== */
void read_graphic_extension(FILE *fd, FILE *fdout)
/* =============================================================== */
{
  int32_t h, l, c;
  int32_t delay;
  uint8_t flags, transcolindex;

#ifdef DEBUG
  printf("reading graphic extension\n");
#endif
  c = getc(fd); /* get block size */
  if (c != 4)
  {
    fprintf(stderr, "read_graphic_extension: FORMAT ERROR\n");
    exit(1);
  }
  flags = (uint8_t)getc(fd);  

  l = getc(fd); h = getc(fd); delay = h * 256 + l;

  transcolindex = (uint8_t)getc(fd);  
#ifdef DEBUG
  printf("delay = %d, transp. col. index = %d\n", delay, transcolindex);
#endif

  c = getc(fd); /* swallow terminator */
  if (c != 0)
  {
    fprintf(stderr, "read_graphic_extension: FORMAT ERROR\n");
    exit(1);
  }

} /* read_graphic_extension() */

/* =============================================================== */
void write_graphic_extension(int32_t delay, FILE *fdout)
/* =============================================================== */
{
  fputc(0x21, fdout); /* extension introducer */
  fputc(0xF9, fdout); /* graphic control label */
  fputc(4, fdout);    /* set block size */
  fputc(0x00, fdout); /* set flags */

  fputc(delay%256, fdout); /* low byte */
  fputc(delay/256, fdout); /* high byte */

  fputc(0, fdout); /* transp. col index */

  fputc(0, fdout); /* terminator */

} /* write_graphic_extension() */

/* =============================================================== */
void write_application_extension(FILE *fdout)
/* =============================================================== */
{
  /* add extension for NETSCAPE to loop over the animation */

  fputc(0x21, fdout); /* extension introducer */
  fputc(0xFF, fdout); /* application extension label */

  fputc(11, fdout);   /* set block size */

  fputc('N', fdout);  /* appl. name */
  fputc('E', fdout);
  fputc('T', fdout);
  fputc('S', fdout);
  fputc('C', fdout);
  fputc('A', fdout);
  fputc('P', fdout);
  fputc('E', fdout);
  fputc(0x32, fdout); /* appl. code */
  fputc(0x2E, fdout);
  fputc(0x30, fdout);

  fputc(3, fdout);    /* set data block size */

  fputc(1, fdout);    /* block data */
  fputc(0, fdout);
  fputc(0, fdout);

  fputc(0, fdout); /* terminator */

} /* write_graphic_extension() */

/* =============================================================== */
void read_comment_extension(FILE *fd, FILE *fdout)
/* =============================================================== */
{
  char buf[256];
  int32_t c, ret;

#ifdef DEBUG
  printf("reading comment extension\n");
#endif

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
#ifdef DEBUG
    printf("comment: %s", buf);
#endif
    c = getc(fd); /* get next block size */
  }

} /* read_comment_extension() */

/* =============================================================== */
void read_application_extension(FILE *fd, FILE *fdout)
/* =============================================================== */
{
  char buf[256];
  int32_t c, ret;

#ifdef DEBUG
  printf("reading application extension\n");
#endif

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
  buf[8] = '\0';
#ifdef DEBUG
  printf("application name: %s\n", buf);
#endif

  c = getc(fd); /* get data block size */
  while (c != 0)
  {
#ifdef DEBUG
    printf("reading data block, length = %d\n", c);
#endif
    ret = fread(buf, sizeof(char), c, fd);
    if (ret != c)
    {
      fprintf(stderr, "read_application_extension: FORMAT ERROR, %ld byte read\n", (long int)ret);
      exit(1);
    }
    c = getc(fd); /* get next block size */
  }
  
} /* read_application_extension() */

/* =============================================================== */
void read_plain_text_extension(FILE *fd, FILE *fdout)
/* =============================================================== */
{
#ifdef DEBUG
  printf("reading plain text extension\n");
#endif
} /* read_plain_text_extension() */

/* =============================================================== */
void read_extension(FILE *fd, FILE *fdout)
/* =============================================================== */
{
  int32_t c = getc(fd);
  if (c == 0xF9)
    read_graphic_extension(fd, fdout);
  else if (c == 0xFE)
    read_comment_extension(fd, fdout);
  else if (c == 0xFF)
    read_application_extension(fd, fdout);
  else if (c == 0x01)
    read_plain_text_extension(fd, fdout);
  else 
  {
    fprintf(stderr, "read_extension: FORMAT ERROR\n");
    exit(1);
  }
} /* read_extension() */

/* =============================================================== */
void read_image_data(FILE *fd, FILE *fdout)
/* =============================================================== */
{
  int32_t LZWmincodesize, c;
  char buf[256];
  int32_t ret;

  LZWmincodesize = getc(fd);
  fputc(LZWmincodesize, fdout);
#ifdef DEBUG
  printf("reading image data, LZWmincodesize = %d\n", LZWmincodesize);
#endif

  c = getc(fd); /* get block size */
  fputc(c, fdout);
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
    fwrite(buf, sizeof(char), c, fdout);
    c = getc(fd); /* get next block size */
    fputc(c, fdout);
  }
} /* read_image_data() */

/* =============================================================== */
void read_data(FILE *fd, FILE *fdout)
/* =============================================================== */
{
  int32_t h, l, c;

  c = getc(fd);
  while (c != 0)
  {
    if (c == 0x3B) 
    {
#ifdef DEBUG
      printf("end of GIF data stream\n");
#endif
      return;
    }
    else if (c == 0x21)
    {
      read_extension(fd, fdout);
      c = getc(fd); /* get next tag */
    }
    else if (c == 0x2C)
    { 
      int32_t leftpos, toppos, width, height;
      uint8_t flags;
#ifdef DEBUG
      printf("reading image descriptor\n");
#endif
      fputc(c, fdout);

      l = getc(fd); h = getc(fd); leftpos = h * 256 + l;
      fputc(l, fdout); fputc(h, fdout);
      l = getc(fd); h = getc(fd); toppos = h * 256 + l;
      fputc(l, fdout); fputc(h, fdout);
      l = getc(fd); h = getc(fd); width = h * 256 + l;
      fputc(l, fdout); fputc(h, fdout);
      l = getc(fd); h = getc(fd); height = h * 256 + l;
      fputc(l, fdout); fputc(h, fdout);
#ifdef DEBUG
      printf("leftpos = %ld ; toppos = %ld ; width = %ld ; height = %ld\n", (long int)leftpos, (long int)toppos, (long int)width, (long int)height);
#endif
      flags = (uint8_t)getc(fd);  
      fputc(flags, fdout);

      if (flags & 0x80)
      {
        int32_t size;
        flags &= 0x7;
        size = 3 * (1 << (flags + 1));
        read_local_color_table(fd, size, fdout);
      }
      read_image_data(fd, fdout);
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
void read_trailer(FILE *fd, FILE *fdout)
/* =============================================================== */
{
} /* read_trailer()*/

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  FILE *fdout = NULL;
  char bufname[256];
  int32_t i, debut, fin, delai;
  int32_t namelen;
  int32_t interact = 0;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s name_in index_begin index_end delay [i] name_out\n", argv[0]);
    fprintf(stderr, "       optional argument i is for interactive mode\n");
    exit(1);
  }

  strcpy(bufname, argv[1]);
  debut = atoi(argv[2]);  
  fin = atoi(argv[3]);  
  delai = atoi(argv[4]);  
  namelen = strlen(argv[1]);
  if (argc == 7) interact = 1;
  
  bufname[namelen] = '0' + (debut / 1000) % 10;
  bufname[namelen+1] = '0' + (debut / 100) % 10;
  bufname[namelen+2] = '0' + (debut / 10) % 10;
  bufname[namelen+3] = '0' + (debut / 1) % 10;
  bufname[namelen+4] = '.';
  bufname[namelen+5] = 'g';
  bufname[namelen+6] = 'i';
  bufname[namelen+7] = 'f';
  bufname[namelen+8] = '\0';

  fd = fopen(bufname,"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], bufname);
    exit(1);
  }

  fdout = fopen(argv[argc-1],"w");
  if (!fdout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

  read_header(fd, fdout);
  read_logical_screen(fd, fdout);
  if (interact)
  {
    long int d;
    printf("delay for image %d (-1 to quit interactive mode)?\n", debut);
    scanf("%ld", &d);
    if (d == -1) interact = 0; else delai = d;
  }
  write_graphic_extension(delai, fdout);
  if (interact)
  {
    char rep[64];
    printf("loop ? (y/n)\n");
    scanf("%s", rep);
    if (rep[0] == 'y')
      write_application_extension(fdout);
  }
  else
    write_application_extension(fdout);
  read_data(fd, fdout);
  fclose(fd);

  for (i = debut + 1; i <= fin; i++)
  {
    bufname[namelen] = '0' + (i / 1000) % 10;
    bufname[namelen+1] = '0' + (i / 100) % 10;
    bufname[namelen+2] = '0' + (i / 10) % 10;
    bufname[namelen+3] = '0' + (i / 1) % 10;
    bufname[namelen+4] = '.';
    bufname[namelen+5] = 'g';
    bufname[namelen+6] = 'i';
    bufname[namelen+7] = 'f';
    bufname[namelen+8] = '\0';
    fd = fopen(bufname,"r");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], bufname);
      exit(1);
    }
    skip_header(fd);
    skip_logical_screen(fd);
    if (interact)
    {
      long int d;
      printf("delay for image %d (-1 to quit interactive mode)?\n", i);
      scanf("%ld", &d);
      if (d == -1) interact = 0; else delai = d;
    }
    write_graphic_extension(delai, fdout);
    read_data(fd, fdout);
    fclose(fd); 
  } /* for i */

  fputc(0x3B, fdout);
  fclose(fdout);
  return 0;
}
