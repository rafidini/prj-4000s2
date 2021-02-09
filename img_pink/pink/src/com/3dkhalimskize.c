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
data to be ensured and, more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file 3dkhalimskize.c

\brief converts a 3D image into a 3D Khalimsky order

<B>Usage:</B> 3dkhalimskize in.pgm {6|26|h|m|M|a|R|b} out.pgm

<B>Description:</B>
Converts a 3D image into a 3D Khalimsky order.

Modes <B>6</B>, <B>26</B> and <B>h</B> are for binary images. 

Modes <B>m</B>, <B>M</B> and <B>a</B> are for grayscale images. 

Mode = 
\li    6 : emulates 6-connectivity, idem "miss", 
\li    26 : emulates 26-connectivity ("hit" transform followed by a kernel transform),   
\li    h : "hit",
\li    m : min, 
\li    M : max,
\li    a : average,
\li    R : reverse,
\li    b : border.

<B>Types supported:</B> byte 3d, long 3d, float 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/*
%TEST 3dkhalimskize %IMAGES/3dbyte/binary/b3a.pgm 6 %RESULTS/3dkhalimskize_b3a_6.k
%TEST 3dkhalimskize %IMAGES/3dbyte/binary/b3a.pgm 26 %RESULTS/3dkhalimskize_b3a_26.k
%TEST 3dkhalimskize %IMAGES/3dbyte/binary/b3a.pgm h %RESULTS/3dkhalimskize_b3a_h.k
%TEST 3dkhalimskize %IMAGES/3dbyte/gray/g3a.pgm m %RESULTS/3dkhalimskize_g3a_m.k
%TEST 3dkhalimskize %IMAGES/3dlong/l3a.pgm M %RESULTS/3dkhalimskize_l3a_M.k
%TEST 3dkhalimskize %IMAGES/3dfloat/f3a.pgm a %RESULTS/3dkhalimskize_f3a_a.k
%TEST 3dkhalimskize %IMAGES/3dbyte/binary/b3a.k R %RESULTS/3dkhalimskize_b3a_R.pgm
*/

/* Michel Couprie - novembre 1999 */
// update octobre 2011 mode 'b'

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * o;
  struct xvimage * k;
  int32_t mode;
  
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm {6|26|h|m|M|a|R|b} out.pgm \n", argv[0]);
    exit(1);
  }

  o = readimage(argv[1]);
  if (o == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = argv[2][0];
  if ((mode != '6') && (mode != '2') && (mode != 'h') && (mode != 'm') && (mode != 'M') && (mode != 'a') && (mode != 'R'))
  {
    fprintf(stderr, "usage: %s in.pgm {6|26|h|m|M|a|R} out.pgm \n", argv[0]);
    exit(1);
  }
  if ((mode == '2') && (argv[2][1] != '6'))
  {
    fprintf(stderr, "usage: %s in.pgm {6|26|h|m|M|a|R} out.pgm \n", argv[0]);
    exit(1);
  }
  if (mode == 'h') mode = 0; else 
  if (mode == 'm') mode = 1; else 
  if (mode == 'M') mode = 2; else 
  if (mode == '2') mode = 3; else 
  if (mode == '6') mode = 4; else
  if (mode == 'a') mode = 5; else 
  if (mode == 'R') mode = 6; else 
  if (mode == 'b') mode = 7;

  if (! l3dkhalimskize(o, &k, mode))
  {
    fprintf(stderr, "%s: function l3dkhalimskize failed\n", argv[0]);
    exit(1);
  }

  k->xdim = o->xdim;
  k->ydim = o->ydim;
  k->zdim = o->zdim;

  writeimage(k, argv[argc-1]);
  freeimage(k);
  freeimage(o);

  return 0;
} /* main */
