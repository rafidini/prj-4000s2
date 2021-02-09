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
/*! \file zoom.c

\brief zoom (shrink or expand) an image

<B>Usage:</B> zoom in.pgm {f | x rs | y cs | z ds | fx fy fz} out.pgm

<B>Description:</B> 
There are 3 modes, depending on the number of arguments.

1 argument: the same scale factor <B>f</B> is applied to both dimensions 
  x and y (and z in 3D)

2 arguments: if the parameter <B>x</B> is used, 
  followed by an integer number <B>rs</B>, the zoom factor <B>f</B> 
  is computed by dividing <B>rs</B> by the rowsize of <B>in.pgm</B>.
  If the parameter <B>y</B> is used, followed by an integer number <B>cs</B>, 
  <B>f</B> is computed by dividing <B>cs</B> by the colsize of <B>in.pgm</B>.
  If the parameter <B>z</B> is used, followed by an integer number <B>ds</B>, 
  <B>f</B> is computed by dividing <B>ds</B> by the depth of <B>in.pgm</B>.

3 arguments: different zoom factors <B>fx</B>, <B>fy</B>, <B>fz</B>
  are given for directions x, y, z.

<B>Types supported:</B> byte 1d, byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - decembre 1996 */

/*
%TEST zoom %IMAGES/2dbyte/binary/b2hebreu.pgm 0.7 %RESULTS/zoom_b2hebreu_07.pgm
%TEST zoom %IMAGES/2dbyte/binary/b2hebreu.pgm 1.7 %RESULTS/zoom_b2hebreu_17.pgm
%TEST zoom %IMAGES/2dbyte/binary/b2hebreu.pgm x 100 %RESULTS/zoom_b2hebreu_x_100.pgm
%TEST zoom %IMAGES/2dbyte/binary/b2hebreu.pgm 0.5 0.25 1 %RESULTS/zoom_b2hebreu_05_025_1.pgm
%TEST zoom %IMAGES/3dbyte/binary/b3a.pgm 0.7 %RESULTS/zoom_b3a_07.pgm
%TEST zoom %IMAGES/3dbyte/binary/b3a.pgm 1.7 %RESULTS/zoom_b3a_17.pgm
%TEST zoom %IMAGES/3dbyte/binary/b3a.pgm x 100 %RESULTS/zoom_b3a_x_100.pgm
%TEST zoom %IMAGES/3dbyte/binary/b3a.pgm 0.5 0.25 0.9 %RESULTS/zoom_b3a_05_025_09.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lzoom.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * out;
  index_t rs, cs;
  double z, fx, fy, fz;
  index_t newdim;

  if ((argc != 4) && (argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm {f | x rs | y cs | z ds | fx fy fz} out.pgm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = in->row_size;
  cs = in->col_size;

  if (argc == 4) 
  { 
    z = atof(argv[2]);
    if (! lzoom(in, &out, z, z, z))
    {
      fprintf(stderr, "%s: function lzoom failed\n", argv[0]);
      exit(1);
    }
  }
  else if (argc == 5) 
  {
    newdim = atoi(argv[3]);
    if (! lzoom2(in, &out, newdim, argv[2][0]))
    {
      fprintf(stderr, "%s: function lzoom2 failed\n", argv[0]);
      exit(1);
    }
  }
  else
  { 
    fx = atof(argv[2]);
    fy = atof(argv[3]);
    fz = atof(argv[4]);
    if (! lzoom(in, &out, fx, fy, fz))
    {
      fprintf(stderr, "%s: function lzoom failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(out, argv[argc-1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
