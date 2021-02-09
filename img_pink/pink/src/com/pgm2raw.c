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

\brief suppress the header from a pgm file

<B>Usage:</B> pgm2raw image_name[.pgm] [out.raw]

<B>Description:</B> suppress the header from a pgm file. If used with only one argument \b image_name, the operator searches for a file named \b image_name.pgm and generates a raw file named \b image_name.raw, a NRRD header file named \b image_name.nrrd (see http://teem.sourceforge.net/nrrd/format.html), and a MHD header file named \b image_name.mfd (see https://itk.org/Wiki/ITK/MetaIO/Documentation)

<B>Types supported:</B> byte 2d, byte 3d, short 2d, short 3d, long 2d, long 3d, float 2d, float 3d, double 2d, double 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* Michel Couprie - janvier 2000 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mcimage.h>
#include <mccodimage.h>

#define VERBOSE
#define TAILLEBUF 4096

#define UINTDATA unsigned int *
/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t rs, cs, ds, N, ret;
  struct xvimage * image;
  char pgmfilename[TAILLEBUF];
  char rawfilename[TAILLEBUF];
  char nrrdfilename[TAILLEBUF];
  char mhdfilename[TAILLEBUF];

  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s image_name[.pgm] [out.raw] \n", argv[0]);
    exit(1);
  }

  if (argc == 3)
  {
    strcpy((char *)pgmfilename, argv[1]);
    strcpy((char *)rawfilename, argv[2]);
  }
  else // argc == 2
  {
    strcpy((char *)pgmfilename, argv[1]);
    strcpy((char *)rawfilename, argv[1]);
    strcpy((char *)nrrdfilename, argv[1]);
    strcpy((char *)mhdfilename, argv[1]);
    strcat((char *)pgmfilename, ".pgm");
    strcat((char *)rawfilename, ".raw");
    strcat((char *)nrrdfilename, ".nrrd");
    strcat((char *)mhdfilename, ".mhd");
  }

  image = readimage(pgmfilename);
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

#ifdef UNIXIO
  fd = fopen(rawfilename,"w");
#endif
#ifdef DOSIO
  fd = fopen(rawfilename,"wb");
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

  if (argc == 2)
  {
    int nb;

#ifdef UNIXIO
    fd = fopen(nrrdfilename,"w");
#endif
#ifdef DOSIO
    fd = fopen(nrrdfilename,"wb");
#endif

    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], (char *)nrrdfilename);
      exit(0);
    }

    if (ds == 1) nb = 2; else nb = 3;
    fputs("NRRD0001\n", fd);
    fprintf(fd, "content: %s\n", argv[1]);
    fprintf(fd, "dimension: %d\n", nb);
    if (datatype(image) == VFF_TYP_1_BYTE)
      fprintf(fd, "type: uchar\n");
    else
      fprintf(fd, "type: unknown\n");
    if (ds == 1)
      fprintf(fd, "sizes: %d %d\n", rs, cs);
    else
      fprintf(fd, "sizes: %d %d %d\n", rs, cs, ds);
    if (ds > 1)
    {
      if (image->xdim != 0.0)
	fprintf(fd, "spacings: %g %g %g\n", image->xdim, image->ydim, image->zdim);
      else
	fprintf(fd, "spacings: 1 1 1\n");
    }
    fprintf(fd, "data file: ./%s.raw\n", argv[1]);
    fprintf(fd, "encoding: raw\n");

    fclose(fd);


#ifdef UNIXIO
    fd = fopen(mhdfilename,"w");
#endif
#ifdef DOSIO
    fd = fopen(mhdfilename,"wb");
#endif

    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], (char *)mhdfilename);
      exit(0);
    }

    if (ds == 1) nb = 2; else nb = 3;
    fputs("ObjectType = Image\n", fd);
    fprintf(fd, "NDims = %d\n", nb);
    fputs("BinaryData = True\n", fd);
    fputs("BinaryDataByteOrderMSB = False\n", fd);
    fputs("CompressedData = False\n", fd);
    if (nb == 3)
    {
      if (image->xdim != 0.0)
        fprintf(fd, "ElementSpacing = %g %g %g\n", image->xdim, image->ydim, image->zdim );
      fprintf(fd, "DimSize = %d %d %d\n", rs, cs, ds);
    }
    else
    {
      if (image->xdim != 0.0)
        fprintf(fd, "ElementSpacing = %g %g\n", image->xdim, image->ydim );
      fprintf(fd, "DimSize = %d %d\n", rs, cs);
    }
    if (datatype(image) == VFF_TYP_1_BYTE) fprintf(fd, "ElementType = MET_CHAR\n");
    if (datatype(image) == VFF_TYP_2_BYTE) fprintf(fd, "ElementType = MET_SHORT\n");
    if (datatype(image) == VFF_TYP_4_BYTE) fprintf(fd, "ElementType = MET_LONG\n");
    if (datatype(image) == VFF_TYP_FLOAT) fprintf(fd, "ElementType = MET_FLOAT\n");
    fprintf(fd, "ElementDataFile = %s.raw\n", argv[1]);

  }


  return 0;
} /* main */


