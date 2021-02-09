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
/*! \file mhd2pgm.c

\brief converts from mhd/raw to pgm

<B>Usage:</B> mhd2pgm prefix_name

<B>Description:</B>

Reads the file <B>prefix_name.mhd</B>. A file named <B>prefix_name.pgm</B> will be generated.

<B>Types supported:</B> mdh file

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - novembre 2017
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/*
Example mhd file :

ObjectType = Image
NDims = 3
BinaryData = True
BinaryDataByteOrderMSB = False
CompressedData = False
TransformMatrix = 1 0 0 0 1 0 0 0 1
Offset = -154.691 -321.691 -95
CenterOfRotation = 0 0 0
AnatomicalOrientation = RAI
ElementSpacing = 0.617188 0.617188 2
DimSize = 512 512 104
ElementSize = 2 2 1
ElementType = MET_SHORT
ElementDataFile = Axial_Arterial_3mm_ST_130f_2.raw
*/

#define TAILLEBUF 4096

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rs, cs, ds, N, i, ndims;
  int littleendian = 0, datatype = 1, t; 
  char *ret, *s;
  char buf[TAILLEBUF];
  char infilename[TAILLEBUF];
  char outfilename[TAILLEBUF];
  char rawfilename[TAILLEBUF];
  FILE *fd = NULL;
  double xdim = 1.0, ydim = 1.0, zdim = 1.0;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s prefix_name\n", argv[0]);
    exit(1);
  }

  strcpy(infilename, argv[1]);
  strcat(infilename, ".mhd");
  strcpy(outfilename, argv[1]);
  strcat(outfilename, ".pgm");
#ifdef UNIXIO
  fd = fopen(infilename,"r");
#endif
#ifdef DOSIO
  fd = fopen(infilename,"rb");
#endif
  if (!fd)
  {
    fprintf(stderr, "%s: file not found: %s\n", argv[0], infilename);
    exit(1);
  }

  while(1)
  {
    ret = fgets(buf, TAILLEBUF, fd);
    if (ret == NULL) break;
    s = "NDims ="; t = strlen(s);
    if (strncasecmp(buf, s, t) == 0)
    {
      sscanf(buf+t, "%d\n", &ndims);
      if (ndims != 3)
      {
        fprintf(stderr, "%s expecting 3D image\n", argv[0]);
        exit(1);
      }
    }
    s = "DimSize ="; t = strlen(s);
    if (strncasecmp(buf, s, t) == 0)
    {
      sscanf(buf+t, "%d %d %d\n", &rs, &cs, &ds);
    }

    s = "ElementSpacing ="; t = strlen(s);
    if (strncasecmp(buf, s, t) == 0)
    {
      sscanf(buf+t, "%lf %lf %lf\n", &xdim, &ydim, &zdim);
    }

    s = "BinaryDataByteOrderMSB ="; t = strlen(s);
    if (strncasecmp(buf, s, t) == 0)
    {
       if (strcasestr(buf+t, "True") != NULL)
         littleendian = 1;
    }

    s = "ElementType ="; t = strlen(s);
    if (strncasecmp(buf, s, t) == 0)
    {
       if (strcasestr(buf+t, "MET_CHAR") != NULL) datatype = 1;
       if (strcasestr(buf+t, "MET_UCHAR") != NULL) datatype = 1;
       if (strcasestr(buf+t, "MET_BYTE") != NULL) datatype = 1;
       if (strcasestr(buf+t, "MET_UBYTE") != NULL) datatype = 1;
       if (strcasestr(buf+t, "MET_USHORT") != NULL) datatype = 2;
       if (strcasestr(buf+t, "MET_SHORT") != NULL) datatype = 3;
       if (strcasestr(buf+t, "MET_LONG") != NULL) datatype = 4;
       if (strcasestr(buf+t, "MET_FLOAT") != NULL) datatype = 5;
    }

    s = "ElementDataFile ="; t = strlen(s);
    if (strncasecmp(buf, s, t) == 0)
    {
      sscanf(buf+t, "%s\n", rawfilename);
    }

  } // while(1)
  fclose(fd);

#ifdef UNIXIO
  fd = fopen(rawfilename,"r");
#endif
#ifdef DOSIO
  fd = fopen(rawfilename,"rb");
#endif
  if (!fd)
  {
    fprintf(stderr, "%s: file not found: %s\n", argv[0], rawfilename);
    exit(1);
  }

#ifdef DEBUG
  printf("ndims, rs, cs, ds = %d %d %d %d\n", ndims, rs, cs, ds);
  printf("xdim, ydim, zdim = %g %g %g\n", xdim, ydim, zdim);
  printf("littleendian, datatype = %d %d\n", littleendian, datatype);
#endif

  N = rs * cs * ds;

  if (datatype == 1)
  {
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    assert (image != NULL);
    fread(UCHARDATA(image), sizeof(char), N, fd);
  }
  else if (datatype == 2)
  {
    int32_t * I;
    uint16_t tmp;
    uint8_t tmp1;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    assert (image != NULL);
    I = SLONGDATA(image);
    for (i = 0; i < N; i++) 
    { 
      fread(&tmp, 2 * sizeof(char), 1, fd);
      if (littleendian)
      {
        tmp1 = tmp & 0x00ff;
        tmp = tmp >> 8;
        tmp = tmp | (((uint32_t)tmp1) << 8);      
      }
      I[i] = (int32_t)tmp; 
    }
  }
  else if (datatype == 3)
  {
    struct xvimage *image_S;
    int16_t * S;
    int32_t * I;
    int16_t tmp;
    uint8_t tmp1;
    int16_t Smin;
    image_S = allocimage(NULL, rs, cs, ds, VFF_TYP_2_BYTE);
    assert (image_S != NULL);
    S = SSHORTDATA(image_S);
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    assert (image != NULL);
    I = SLONGDATA(image);

    for (i = 0; i < N; i++) 
    { 
      fread(&tmp, 2 * sizeof(char), 1, fd);
      if (littleendian)
      {
        tmp1 = tmp & 0x00ff;
        tmp = tmp >> 8;
        tmp = tmp | (((uint32_t)tmp1) << 8);      
      }
      S[i] = tmp; 
    }
    Smin = S[0];
    for (i = 0; i < N; i++) 
      if (S[i] < Smin) Smin = S[i];
    if (Smin < 0)
    {
      for (i = 0; i < N; i++) 
	I[i] = S[i] - Smin;
    }
    else
    {
      for (i = 0; i < N; i++) 
	I[i] = S[i];
    }
    freeimage(image_S);
  }
  else if (datatype == 4)
  {
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    assert (image != NULL);
    if (littleendian)
    {
      int32_t * I;
      uint32_t tmp;
      uint8_t tmp1, tmp2, tmp3;
      I = SLONGDATA(image);
      for (i = 0; i < N; i++) 
      { 
	fread(&tmp, 4 * sizeof(char), 1, fd);
        tmp1 = tmp & 0xff; tmp = tmp >> 8;
        tmp2 = tmp & 0xff; tmp = tmp >> 8;
        tmp3 = tmp & 0xff; tmp = tmp >> 8;
        tmp = tmp | (((uint32_t)tmp3) << 8);
        tmp = tmp | (((uint32_t)tmp2) << 16);
        tmp = tmp | (((uint32_t)tmp1) << 24);      
      }
      I[i] = (int32_t)tmp; 
    }
    else
    {
      fread(SLONGDATA(image), sizeof(float), N, fd);
    }
  }
  else if (datatype == 5)
  {
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
    assert (image != NULL);
    if (littleendian)
    {
      float *I;
      uint32_t tmp;
      uint8_t tmp1, tmp2, tmp3;
      I = FLOATDATA(image);
      assert(sizeof(uint32_t) == sizeof(float));
      for (i = 0; i < N; i++) 
      { 
	fread(&tmp, sizeof(uint32_t), 1, fd);
        tmp1 = tmp & 0xff; tmp = tmp >> 8;
        tmp2 = tmp & 0xff; tmp = tmp >> 8;
        tmp3 = tmp & 0xff; tmp = tmp >> 8;
        tmp = tmp | (((uint32_t)tmp3) << 8);
        tmp = tmp | (((uint32_t)tmp2) << 16);
        tmp = tmp | (((uint32_t)tmp1) << 24);      
        I[i] = *((float *)(&tmp)); 
      }
    }
    else
    {
      fread(FLOATDATA(image), sizeof(float), N, fd);
    }
  }

  image->xdim = xdim;
  image->ydim = ydim;
  image->zdim = zdim;

  writeimage(image, outfilename);
  freeimage(image);
  fclose(fd);
  return 0;
}

