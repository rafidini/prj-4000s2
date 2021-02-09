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
/*! \file pgm2raw.c

\brief converts a pgm file to mhd format

<B>Usage:</B> pgm2raw in.pgm out.mhd out.raw

<B>Description:</B> converts a pgm file to mhd format
<B>Types supported:</B> byte 2d, byte 3d, short 2d, short 3d, long 2d, long 3d, float 2d, float 3d, double 2d, double 3d

<B>Category:</B> convert
\ingroup  convert

*/

/* Imen Melki , Mai 2014 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

#define VERBOSE

#define UINTDATA unsigned int *
/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  FILE *fh = NULL;
  int32_t rs, cs, ds, N, ret;

  struct xvimage * image;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm out.mhd out.raw \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ( (datatype(image) != VFF_TYP_1_BYTE) && (datatype(image) != VFF_TYP_2_BYTE) && (datatype(image) != VFF_TYP_4_BYTE) && (datatype(image) != VFF_TYP_FLOAT) && (datatype(image) != VFF_TYP_DOUBLE))
  {
    fprintf(stderr, "%s: image type not supported (%d)\n", argv[0], datatype(image));
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  N = rs * cs * ds;

#ifdef VERBOSE
  printf("rs = %d ; cs = %d ; ds = %d ; N = rs * cs * ds = %d\n", rs, cs, ds, N);
#endif



  //write the raw file
#ifdef UNIXIO
  fd = fopen(argv[argc - 1],"w");
#endif
#ifdef DOSIO
  fd = fopen(argv[argc - 1],"wb");
#endif

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
	ret = fwrite(UCHARDATA(image), sizeof(uint8_t), N, fd);
  }
  else if (datatype(image) == VFF_TYP_2_BYTE)
  {
  	ret = fwrite(SSHORTDATA(image), sizeof(int16_t), N, fd);
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
  	ret = fwrite(SLONGDATA(image), sizeof(int32_t), N, fd);
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
  	ret = fwrite(FLOATDATA(image), sizeof(float), N, fd);
  }
  else if (datatype(image) == VFF_TYP_DOUBLE)
  {
  	ret = fwrite(DOUBLEDATA(image), sizeof(double), N, fd);
  }
  if (ret != N)
  {
    fprintf(stderr, "%s: only %d items written\n", argv[0], ret);
    exit(1);
  }

  fclose(fd);

  //write the mhd header
#ifdef UNIXIO
  fh = fopen(argv[argc - 2],"w");
#endif
#ifdef DOSIO
  fh = fopen(argv[argc - 2],"wb");
#endif

  if (datatype(image) == VFF_TYP_1_BYTE)
    {
      fprintf(fh, "NDims = %d \n", 3);
      fprintf(fh, "DimSize = %d %d %d  \n", rs, cs, ds);
      fprintf(fh, "ElementType = %s\n", "MET_UCHAR");
      fprintf(fh, "Offset = %d %d %d \n", 0,0,0);
      fprintf(fh, "ElementSpacing = %f %f %f \n",image->xdim, image->ydim, image->zdim);
      fprintf(fh, "ElementByteOrderMSB = %s \n", "False");
      fprintf(fh, "ElementNumberOfChannels = %d \n", 1);
      fprintf(fh, "ElementDataFile = %s \n", argv[argc - 1] );
      
    }

  else if (datatype(image) == VFF_TYP_2_BYTE)
    {
      fprintf(fh, "NDims = %d \n", 3);
      fprintf(fh, "DimSize = %d %d %d  \n", rs, cs, ds);
      fprintf(fh, "ElementType = %s\n", "MET_USHORT");
      fprintf(fh, "Offset = %d %d %d \n", 0,0,0);
      fprintf(fh, "ElementSpacing = %f %f %f \n",image->xdim, image->ydim, image->zdim);
      fprintf(fh, "ElementByteOrderMSB = %s \n", "False");
      fprintf(fh, "ElementNumberOfChannels = %d \n", 1);
      fprintf(fh, "ElementDataFile = %s \n", argv[argc - 1] );
    }
  
  else if (datatype(image) == VFF_TYP_4_BYTE)
    {
      fprintf(fh, "NDims = %d \n", 3);
      fprintf(fh, "DimSize = %d %d %d  \n", rs, cs, ds);
      fprintf(fh, "ElementType = %s\n", "MET_INT");
      fprintf(fh, "Offset = %d %d %d \n", 0,0,0);
      fprintf(fh, "ElementSpacing = %f %f %f \n",image->xdim, image->ydim, image->zdim);
      fprintf(fh, "ElementByteOrderMSB = %s \n", "False");
      fprintf(fh, "ElementNumberOfChannels = %d \n", 1);
      fprintf(fh, "ElementDataFile = %s \n", argv[argc - 1] );
    }
  else if (datatype(image) == VFF_TYP_FLOAT)
    {
      fprintf(fh, "NDims = %d \n", 3);
      fprintf(fh, "DimSize = %d %d %d  \n", rs, cs, ds);
      fprintf(fh, "ElementType = %s\n", "MET_FLOAT");
      fprintf(fh, "Offset = %d %d %d \n", 0,0,0);
      fprintf(fh, "ElementSpacing = %f %f %f \n",image->xdim, image->ydim, image->zdim);
      fprintf(fh, "ElementByteOrderMSB = %s \n", "False");
      fprintf(fh, "ElementNumberOfChannels = %d \n", 1);
      fprintf(fh, "ElementDataFile = %s \n", argv[argc - 1] );
    }
  else if (datatype(image) == VFF_TYP_DOUBLE)
    {
      fprintf(fh, "NDims = %d \n", 3);
      fprintf(fh, "DimSize = %d %d %d  \n", rs, cs, ds);
      fprintf(fh, "ElementType = %s\n", "MET_DOUBLE");
      fprintf(fh, "Offset = %d %d %d \n", 0,0,0);
      fprintf(fh, "ElementSpacing = %f %f %f \n",image->xdim, image->ydim, image->zdim);
      fprintf(fh, "ElementByteOrderMSB = %s \n", "False");
      fprintf(fh, "ElementNumberOfChannels = %d \n", 1);
      fprintf(fh, "ElementDataFile = %s \n", argv[argc - 1] );
    }
  
  
  fclose(fh);
  return 0;
} /* main */


