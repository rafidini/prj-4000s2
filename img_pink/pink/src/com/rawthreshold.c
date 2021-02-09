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
/*! \file rawthreshold.c

\brief converts from raw format into pgm format and applies a double threshold

<B>Usage:</B> rawthreshold in.raw rs cs ds headersize datatype littleendian thresh1 thresh2 [xdim ydim zdim] out.pgm

<B>Description:</B> Converts from raw format into pgm byte format, and selects only pixels which have values V such that thresh1 <= V <= thresh2

Parameters:
\li \b in.pgm : source file in raw format
\li \b rs (int32_t): row size (number of voxels in a row)
\li \b cs (int32_t): column size (number of voxels in a column)
\li \b ds (int32_t): number of planes
\li \b headersize (int32_t): size of the header in bytes (information in the header will be ignored)
\li \b datatype (int32_t): (1 for byte, 2 for short int, 4 for long int, 5 for float)
\li \b littleendian (int32_t) 1: littleendian, 0: bigendian. Usual choice is 0.
\li \b thresh1, thresh2 (float): threshold values
\li \b xdim (float, optional) : gap (in the real world) between two adjacent voxels in a row.  
\li \b ydim (float, optional) : gap (in the real world) between two adjacent voxels in a column.  
\li \b zdim (float, optional) : gap (in the real world) between two adjacent planes.  

This operator does not load the complete image into memory. 

<B>Types supported:</B> float

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - janvier 2011 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fdi = NULL, *fdo = NULL;
  int32_t nbytesperpix, littleendian;
  index_t N, rs, cs, ds, i, headersize;
  index_t n_w, n_r, blocksize = 1024*1024;
  double thresh1, thresh2, xdim = 0.0, ydim, zdim;
  uint8_t * B;

  if ((argc != 11) && (argc != 14))
  {
    fprintf(stderr, "usage: %s in.raw rs cs ds headersize nbytespervox littleendian thresh1 thresh2 [xdim ydim zdim] out.pgm \n", argv[0]);
    exit(1);
  }

#ifdef UNIXIO
  fdi = fopen(argv[1],"r");
  fdo = fopen(argv[argc-1],"w");
#endif
#ifdef DOSIO
  fdi = fopen(argv[1],"rb");
  fdo = fopen(argv[argc-1],"wb");
#endif

  if (fdi == NULL)
  {
    fprintf(stderr, "%s: fopen failed for %s\n", argv[0], argv[1]);
    exit(1);
  }

  if (fdo == NULL)
  {
    fprintf(stderr, "%s: fopen failed for %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

  rs = atoi(argv[2]);
  cs = atoi(argv[3]);
  ds = atoi(argv[4]);
  N = rs * cs * ds;
  headersize = atoi(argv[5]);
  nbytesperpix = atoi(argv[6]);
  littleendian = atoi(argv[7]);
  thresh1 = atof(argv[8]);
  thresh2 = atof(argv[9]);
  if (argc == 13)
  {
    xdim = atof(argv[10]);
    ydim = atof(argv[11]);
    zdim = atof(argv[12]);
  }

  if ((nbytesperpix != 1) && (nbytesperpix != 2) && (nbytesperpix != 4))
  {
    fprintf(stderr, "%s: bad value for pix size: %d\n", argv[0], nbytesperpix);
    exit(1);
  }

  assert(nbytesperpix == 4);

  fputs("P5\n", fdo);
  if ((xdim != 0.0) && (ds > 1))
    fprintf(fdo, "#xdim %g\n#ydim %g\n#zdim %g\n", xdim, ydim, zdim);
  if ((xdim != 0.0) && (ds == 1))
    fprintf(fdo, "#xdim %g\n#ydim %g\n", xdim, ydim);
  
  if (ds > 1) 
#ifdef MC_64_BITS
    fprintf(fdo, "%lld %lld %lld\n", rs, cs, ds); 
#else
    fprintf(fdo, "%d %d %d\n", rs, cs, ds); 
#endif
  else 
#ifdef MC_64_BITS
    fprintf(fdo, "%lld %lld\n", rs, cs);
#else
    fprintf(fdo, "%d %d\n", rs, cs);
#endif
  fprintf(fdo, "255\n");

  if (littleendian)
  {
    uint32_t tmp;
    uint8_t tmp1, tmp2, tmp3;
    float f;

    fseek(fdi, headersize, SEEK_CUR);

    for (i = 0; i < N; i++) 
    { 
#ifdef VERBOSE
      if ((i % 100000000) == 0) 
#ifdef MC_64_BITS
	printf("%lld written elements\n", i);
#else
	printf("%d written elements\n", i);
#endif
#endif
      fread(&tmp, sizeof(uint32_t), 1, fdi);
      tmp1 = tmp & 0xff; tmp = tmp >> 8;
      tmp2 = tmp & 0xff; tmp = tmp >> 8;
      tmp3 = tmp & 0xff; tmp = tmp >> 8;
      tmp = tmp | (((uint32_t)tmp3) << 8);
      tmp = tmp | (((uint32_t)tmp2) << 16);
      tmp = tmp | (((uint32_t)tmp1) << 24);      
      f = *((float *)(&tmp));
      if ((f >= thresh1) && (f <= thresh2)) tmp1 = NDG_MAX; else tmp1 = 0;
      fwrite(&tmp1, sizeof(uint8_t), 1, fdo);
    }
  }
  else
  {
    float * F;
    int32_t n_elts = blocksize/sizeof(float); assert(F != NULL);
    F = (float *)malloc(n_elts * sizeof(float)); assert(F != NULL);
    B = (uint8_t *)malloc(n_elts * sizeof(uint8_t)); assert(B != NULL);

    fseek(fdi, headersize, SEEK_CUR);
    n_w = 0;
    do 
    {
      n_r = (int32_t)fread(F, sizeof(float), n_elts, fdi); assert(n_r > 0);
      for (i = 0; i < n_r; i++)
      {
	if ((F[i] >= thresh1) && (F[i] <= thresh2)) B[i] = NDG_MAX; else B[i] = 0;
      }
      if (n_w + n_r > N) n_r = N - n_w;
      fwrite(B, sizeof(uint8_t), n_r, fdo);
      n_w = n_w + n_r;
    } while (n_w < N);
    
    free(F);
  }

  fclose(fdi);
  fclose(fdo);

  return 0;
} /* main */


