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
/*! \file skelpar3d.c

\brief parallel 3D binary skeleton

<B>Usage:</B> skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm

<B>Description:</B>

Parallel 3D binary thinning or skeleton. 

The parameter \b nsteps gives, if positive, the number of parallel
thinning steps to be processed.  If the value given for \b nsteps
equals -1, the thinning is continued until stability.

For some algorithms (16 to ??), the parameter \b nsteps has a
different meaning which is related to the notion of persistence (see
[REF A AJOUTER]). It plays the role of a filtering parameter.

The parameter \b algorithm is a numerical code
indicating which method will be used for the thinning.
The possible choices are:
\li 0: ultimate, symmetric, without constraint (MK3a)
\li 1: curvilinear, symmetric, based on 1D isthmus (CK3a)
\li 2: medial axis preservation (AK3) - parameter inhibit represents the minimal radius of medial axis balls which are considered
\li 3: ultimate, symmetric (MK3) - if nsteps = -2, returns the topological distance
\li 4: curvilinear, symmetric, based on ends (EK3)
\li 5: curvilinear, symmetric, based on ends, with end reconstruction (CK3b)
\li 6: topological axis (not homotopic)
\li 7: curvilinear, symmetric, based on residual points and 2D isthmus (CK3)
\li 8: ultimate, asymmetric (AMK3)
\li 9: curvilinear, asymmetric, based on thin 1D isthmus (ACK3a)
\li 10: curvilinear, asymmetric, based on 3D and 2D residuals (ACK3)
\li 11: surface, symmetric, based on residual points (RK3)
\li 12: surface, symmetric, based on 2D isthmuses (SK3)
\li 13: ultimate, directional, (DK3)
\li 14: surface, directional, based on residual points (DRK3)
\li 15: surface, directional, based on 2D isthmuses (DSK3)
\li 16: curvilinear, asymmetric, based on thin 1D isthmus with persistence (ACK3p)
\li 17: surface, asymmetric, based on thin 2D isthmus with persistence (ASK3p)
\li 18: curvilinear, symmetric, based on 1D isthmus with persistence (CK3p)
\li 19: surface, symmetric, based on 2D isthmus with persistence (SK3p)
\li 20: surface and curvilinear, symmetric, based on 1D and 2D isthmus with persistence (SCK3p)
\li 21: surface, symmetric, based on residual points (RK3), variant (uses 26-connectivity to define residual points)
\li 22: surface and curvilinear, asymmetric, based on 1D and 2D isthmus with persistence (ASCK3p)
\li 23: curvilinear, asymmetric, based on ends (AEK3)
\li 24: surface, symmetric, based on 2D and 1D isthmuses (SCK3)
\li 25: surface, directional, based on 2D and 1D isthmuses (DSCK3)

In modes other than 2, if the parameter \b inhibit is given and is a
binary image name, then the points of this image will be left
unchanged.

The following codes give access to auxiliary functions, for isthmus detection. 
Parameter \b nsteps will not be used in this case (any value can be given).

\li 100: 1D isthmus points

<B>Warning:</B> The object must not have any point on the frame of the image.

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/

/*
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 0 -1 %RESULTS/skelpar3d_b3a_0__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 1 -1 %RESULTS/skelpar3d_b3a_1__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 2 -1 %RESULTS/skelpar3d_b3a_2__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 3 -1 %RESULTS/skelpar3d_b3a_3__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 4 -1 %RESULTS/skelpar3d_b3a_4__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 5 -1 %RESULTS/skelpar3d_b3a_5__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 6 -1 %RESULTS/skelpar3d_b3a_6__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 7 -1 %RESULTS/skelpar3d_b3a_7__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 8 -1 %RESULTS/skelpar3d_b3a_8__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 9 -1 %RESULTS/skelpar3d_b3a_9__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 10 -1 %RESULTS/skelpar3d_b3a_10__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 11 -1 %RESULTS/skelpar3d_b3a_11__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 12 -1 %RESULTS/skelpar3d_b3a_12__1.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 3 4 %RESULTS/skelpar3d_b3a_0_4.pgm
%TEST skelpar3d %IMAGES/3dbyte/binary/b3a.pgm 3 -1 %IMAGES/3dbyte/binary/b3a2.pgm %RESULTS/skelpar3d_b3a_0__1_i.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lskelpar3d.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * inhibit = NULL;
  int32_t mode, nsteps;
  int32_t filter = 0;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm algorithm nsteps [inhibit] out.pgm\n", argv[0]);
    fprintf(stderr, "   0: ultimate, symmetric, without constraint (MK3a)\n");
    fprintf(stderr, "   1: curvilinear, symmetric, based on 1D isthmus (CK3a)\n");
    fprintf(stderr, "   2: medial axis preservation (AK3) - parameter inhibit represents the minimal radius of medial axis balls which are considered\n");
    fprintf(stderr, "   3: ultimate (MK3) - if nsteps = -2, returns the topological distance\n");
    fprintf(stderr, "   4: curvilinear, symmetric, based on ends (EK3)\n");
    fprintf(stderr, "   5: curvilinear, symmetric, based on ends, with end reconstruction (CK3b)\n");
    fprintf(stderr, "   6: topological axis (not homotopic)\n");
    fprintf(stderr, "   7: curvilinear, symmetric, based on residual points and 2D isthmus (CK3)\n");
    fprintf(stderr, "   8: ultimate, asymmetric (AMK3)\n");
    fprintf(stderr, "   9: curvilinear, asymmetric, based on thin 1D isthmus (ACK3a)\n");
    fprintf(stderr, "  10: curvilinear, asymmetric, based on 3D and 2D residuals (ACK3)\n");
    fprintf(stderr, "  11: surface, symmetric, based on residual points (RK3)\n");
    fprintf(stderr, "  12: surface, symmetric, based on 2D isthmuses (SK3)\n");
    fprintf(stderr, "  13: ultimate, directional (DRK3)\n");
    fprintf(stderr, "  14: surface, directional, based on residual points (DRK3)\n");
    fprintf(stderr, "  15: surface, directional, based on 2D isthmuses (DSK3)\n");
    fprintf(stderr, "  16: curvilinear, asymmetric, based on thin 1D isthmus with persistence (ACK3p)\n");
    fprintf(stderr, "  17: surface, asymmetric, based on thin 2D isthmus with persistence (ASK3p)\n");
    fprintf(stderr, "  18: curvilinear, symmetric, based on 1D isthmus with persistence (CK3p)\n");
    fprintf(stderr, "  19: surface, symmetric, based on 2D isthmus with persistence (SK3p)\n");
    fprintf(stderr, "  20: surface and curvilinear, symmetric, based on 1D and 2D isthmus with persistence (SCK3p)\n");
    fprintf(stderr, "  21: surface, symmetric, based on residual points (RK3), variant (uses 26-connectivity to define residual points)\n");
    fprintf(stderr, "  22: surface and curvilinear, asymmetric, based on 1D and 2D isthmus with persistence (ASCK3p)\n");
    fprintf(stderr, "  23: curvilinear, asymmetric, based on ends (AEK3)\n");
    fprintf(stderr, "  24: surface, symmetric, based on 2D and 1D isthmuses (SCK3)\n");
    fprintf(stderr, "  25: surface, directional, based on 2D and 1D isthmuses (DSCK3)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, " 100: 1D isthmus points detection\n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[2]);
  nsteps = atoi(argv[3]);

  if ((argc == 6) && (mode == 2))
    filter = atoi(argv[4]);
  else if (argc == 6)
  {
    inhibit = readimage(argv[4]);
    if (inhibit == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (depth(image) != 1)
  {
    switch (mode)
    {
    case 0:
      if (! lskelMK3a(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelMK3a failed\n", argv[0]);
	exit(1);
      } break;
    case 1:
      if (! lskelCK3a(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelCK3a failed\n", argv[0]);
	exit(1);
      } break;
    case 2:
      if (! lskelAK3(image, nsteps, inhibit, filter))
      {
	fprintf(stderr, "%s: lskelAK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 3:
      if (nsteps == -2)
      {
	struct xvimage *disttopo = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
	if (disttopo == NULL)
	{   
	  fprintf(stderr, "%s(): allocimage failed\n", argv[0]);
	  return 0;
	}
	if (! ldisttopo3(image, inhibit, disttopo))
        {
	  fprintf(stderr, "%s: ldistaxetopo3 failed\n", argv[0]);
	  exit(1);
	} 
	writeimage(disttopo, argv[argc-1]);
	freeimage(image);
	freeimage(disttopo);
	return 0;
      }
      else
      {
        if (! lskelMK3(image, nsteps, inhibit))
        {
	  fprintf(stderr, "%s: lskelMK3c failed\n", argv[0]);
	  exit(1);
        } break;
      }
    case 4:
      if (! lskelEK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelEK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 5:
      if (! lskelCK3b(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelCK3b failed\n", argv[0]);
	exit(1);
      } break;
    case 6:
      {
	struct xvimage *disttopo = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
	if (disttopo == NULL)
	{   
	  fprintf(stderr, "%s(): allocimage failed\n", argv[0]);
	  return 0;
	}
	if (! ldistaxetopo3(image, inhibit, disttopo))
        {
	  fprintf(stderr, "%s: ldistaxetopo3 failed\n", argv[0]);
	  exit(1);
	} 
	freeimage(disttopo);
      } break;
    case 7:
      if (! lskelCK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelCK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 8:
      if (! lskelAMK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelAMK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 9:
      if (! lskelACK3a_old(image, nsteps, 0, inhibit))
      {
	fprintf(stderr, "%s: lskelACK3a_old failed\n", argv[0]);
	exit(1);
      } break;
    case 10:
      if (! lskelACK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelACK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 11:
      if (! lskelRK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelRK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 12:
      if (! lskelSK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelSK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 13:
      if (! lskelDK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelDK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 14:
      if (! lskelDRK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelDRK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 15:
      if (! lskelDSK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelDSK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 16:
      if (! lskelACK3p(image, -1, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelACK3p failed\n", argv[0]);
	exit(1);
      } break;
    case 17:
      if (! lskelASK3p(image, -1, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelASK3p failed\n", argv[0]);
	exit(1);
      } break;
    case 18:
      if (! lskelCK3p(image, -1, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelCK3p failed\n", argv[0]);
	exit(1);
      } break;
    case 19:
      if (! lskelSK3p(image, -1, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelSK3p failed\n", argv[0]);
	exit(1);
      } break;
    case 20:
      if (! lskelSCK3p(image, -1, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelSCK3p failed\n", argv[0]);
	exit(1);
      } break;
    case 21:
      if (! lskelRK3_26(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelRK3_26 failed\n", argv[0]);
	exit(1);
      } break;
    case 22:
    if (! lskelASCK3p(image, -1, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelASCK3p failed\n", argv[0]);
	exit(1);
      } break;
    case 23:
      if (! lskelAEK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelAEK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 24:
      if (! lskelSCK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelSCK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 25:
      if (! lskelDSCK3(image, nsteps, inhibit))
      {
	fprintf(stderr, "%s: lskelDSCK3 failed\n", argv[0]);
	exit(1);
      } break;
    case 100:
      if (! lskel1Disthmuspoints(image))
      {
	fprintf(stderr, "%s: lskel1Disthmuspoints failed\n", argv[0]);
	exit(1);
      } break;
    default: 
      fprintf(stderr, "%s: mode %d not implemented\n", argv[0], mode);
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "%s: image must be 3D\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
