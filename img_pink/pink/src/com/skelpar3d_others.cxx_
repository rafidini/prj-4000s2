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
/*! \file skelpar3d_others.cxx

\brief parallel 3D binary skeleton
DirectionalSkeletonizer
<B>Usage:</B> skelpar3d in.pgm algorithm nsteps [inhibit] out.pgm

<B>Description:</B>
Parallel 3D binary thinning or skeleton. The parameter \b nsteps gives,
if positive, the number of parallel thinning steps to be processed.
If the value given for \b nsteps equals -1, the thinning is continued
until stability.

The parameter \b algorithm is a numerical code
indicating which method will be used for the thinning.
The possible choices are:
\li 0: Palagyi (curvilinear, 6-subiterations directional, 1998)
\li 1: Palagyi (curvilinear, sequential, 2006)
\li 2: Palagyi (surface, parallel directional, 2002)
\li 3: Palagyi (surface, fully parallel, 2008)
\li 4: Raynal  (curvilinear, directional, 2010)
\li 5: Raynal  (surface, directional, 2010)
\li 6: Lohou-Bertrand  (curvilinear, symmetric, 2007)
\li 7: Ma-Wan-Chang (curvilinear, 2 subfields, 2002)
\li 8: Tsao-Fu (curvilinear, 6-subiterations directional, 1982)
\li 9: Ma-Sonka (curvilinear, fully parallel, does not preserve topology 1996)
\li 10: Ma-Wan (curvilinear (18/6) 6 subiterations, CVIU 2000)
\li 11: Lohou-Bertrand (curvilinear 6 subiterations, DAM 2005)
\li 12: Lohou-Bertrand (curvilinear 12 subiterations, DAM 2004)
\li 13: ACK3a
\li 14: CKSC
\li 15: Ma-Wan-Lee (curvilinear, 4 subfields, 2002)
\li 16: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 1)
\li 17: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 2)
\li 18: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 3)
\li 19: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 1)
\li 20: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 2)
\li 21: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 3)
\li 22: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 1)
\li 23: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 2)
\li 24: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 3)
\li 25: She et al. (curvilinear, symmetric, DICTA 2009)
\li 26: Tsao-Fu (surface, 6-subiterations directional, 1981)
\li 27: Tsao-Fu (curvilinear, 6-subiterations directional, 1981)
\li 28: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 0)
\li 29: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 0)
\li 30: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 0)
\li 31: Lohou-Bertrand  (surface, symmetric, 2007)
\li 32: Manzanera et al. (surface, symmetric, 1999)

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged.

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie, Benjamin Raynal, John Chaussard
*/

// I'm confused, this should be a C++ file...
// Hugues Talbot	18 Aug 2010

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lskelpar3d_others.h>
#include <lmedialaxis.h>
#include <lskeletons.h>
#include <lskelpar3d.h>
#include <DirectionalSkeletonizer.hpp>
#include <FIFO.hpp>

/* =============================================================== */
int main(int32_t argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * inhibit = NULL;
  int32_t mode, nsteps;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm algorithm nsteps [inhibit] out.pgm\n", argv[0]);
    fprintf(stderr, "   0: Palagyi (curvilinear, parallel directional, 1998)\n");
    fprintf(stderr, "   1: Palagyi (curvilinear, sequential, 2006)\n");
    fprintf(stderr, "   2: Palagyi (surface, parallel directional, 2002)\n");
    fprintf(stderr, "   3: Palagyi (surface, fully parallel, 2008)\n");
    fprintf(stderr, "   4: Raynal (curvilinear, directional, 2010)\n");
    fprintf(stderr, "   5: Raynal (surface, directional, 2010)\n");
    fprintf(stderr, "   6: Lohou-Bertrand (curvilinear, symmetric, 2007)\n");
    fprintf(stderr, "   7: Ma-Wan-Chang (curvilinear, 2 subfields, 2002)\n");
    fprintf(stderr, "   8: Tsao-Fu (curvilinear, 6-subiterations directional, 1982)\n");
    fprintf(stderr, "   9: Ma-Sonka (curvilinear, fully parallel, does not preserve topology 1996)\n");
    fprintf(stderr, "   10: Ma-Wan (curvilinear (18/6) 6 subiterations, CVIU 2000)\n");
    fprintf(stderr, "   11: Lohou-Bertrand (curvilinear 6 subiterations, DAM 2005)\n");
    fprintf(stderr, "   12: Lohou-Bertrand (curvilinear 12 subiterations, DAM 2004)\n");
    fprintf(stderr, "   13: ACK3a\n");
    fprintf(stderr, "   14: CKSC\n");
    fprintf(stderr, "   15: Ma-Wan-Lee (curvilinear, 4 subfields, 2002)\n");
    fprintf(stderr, "   16: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 1)\n");
    fprintf(stderr, "   17: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 2)\n");
    fprintf(stderr, "   18: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 3)\n");
    fprintf(stderr, "   19: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 1)\n");
    fprintf(stderr, "   20: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 2)\n");
    fprintf(stderr, "   21: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 3)\n");
    fprintf(stderr, "   22: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 1)\n");
    fprintf(stderr, "   23: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 2)\n");
    fprintf(stderr, "   24: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 3)\n");
    fprintf(stderr, "   25: She et al. (curvilinear, symmetric, DICTA 2009)\n");
    fprintf(stderr, "   27: Tsao-Fu (curvilinear, 6-subiterations directional, 1981)\n");
    fprintf(stderr, "   28: Nemeth-Kardos-Palagyi (curvilinear, 2 subfields, 2010, var. 0)\n");
    fprintf(stderr, "   29: Nemeth-Kardos-Palagyi (curvilinear, 4 subfields, 2010, var. 0)\n");
    fprintf(stderr, "   30: Nemeth-Kardos-Palagyi (curvilinear, 8 subfields, 2010, var. 0)\n");    
    fprintf(stderr, "   31: Lohou-Bertrand (surface, symmetric, 2007)\n");
    fprintf(stderr, "   32: Manzanera et al. (surface, symmetric, 1999)\n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  DirectionalSkeletonizer dskel1(image->row_size, image->col_size, image->depth_size);

  mode = atoi(argv[2]);
  nsteps = atoi(argv[3]);

  if (argc == 6)
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
      if (argc == 6)
		fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
	  else if (nsteps!=-1)
		fprintf(stderr, "%s: warning: step mode not implemented for algo %d\n", argv[0], mode);
      (void)palagyi_skelpar_curv_98(image);
      break;
    case 1:
      if (argc == 6)
		fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
	  else if (nsteps!=-1)
		fprintf(stderr, "%s: warning: step mode not implemented for algo %d\n", argv[0], mode);
      (void)palagyi_skelpar_curv_06(image);
      break;
    case 2:
      if (argc == 6)
		fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
	  else if (nsteps!=-1)
		fprintf(stderr, "%s: warning: step mode not implemented for algo %d\n", argv[0], mode);
      (void)palagyi_skelpar_surf_02(image);
      break;
    case 3:
      if (argc == 6)
		fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
	  else if (nsteps!=-1)
		fprintf(stderr, "%s: warning: step mode not implemented for algo %d\n", argv[0], mode);
      (void)palagyi_skelpar_surf_08(image);
      break;
    case 4:
      if (nsteps==-1) nsteps=0;
      if(inhibit!=NULL)
      dskel1.skeletonize((unsigned char*)(image->image_data), true, nsteps, (unsigned char*)(inhibit->image_data));
      else
      dskel1.skeletonize((unsigned char*)(image->image_data), true, nsteps);
      break;
    case 5:
      if (nsteps==-1) nsteps=0;
      if(inhibit!=NULL)
      dskel1.skeletonize((unsigned char*)(image->image_data), false, nsteps, (unsigned char*)(inhibit->image_data));
      else
      dskel1.skeletonize((unsigned char*)(image->image_data), false, nsteps);
      break;
    case 6:
      if (!llohoubertrandsymcurv2007(image, inhibit, nsteps))
      {
	fprintf(stderr, "%s: llohoubertrandsymcurv2007 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 7:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lmawanchangcurv2subfields2002(image, nsteps))
      {
	fprintf(stderr, "%s: lmawanchangcurv2subfields2002 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 8:
      if (!ltsaofu6dircurv1982(image, inhibit, nsteps))
      {
	fprintf(stderr, "%s: ltsaofu6dircurv1982 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 9:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lmasonka1996(image, nsteps))
      {
	fprintf(stderr, "%s: lmasonka1996 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 10:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lmawan2000(image, nsteps))
      {
	fprintf(stderr, "%s: lmawan2000 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 11:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!llohoubertrand6dir2005(image, nsteps))
      {
	fprintf(stderr, "%s: llohoubertrand6dir2005 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 12:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!llohoubertrand12dir2004(image, nsteps))
      {
	fprintf(stderr, "%s: llohoubertrand12dir2004 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 13:
      if (! lskelACK3a(image, nsteps, 0, inhibit))
      {
	fprintf(stderr, "%s: lskelACK3a failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 14:
      if (! lskelCKSC3(image, inhibit, nsteps))
      {
	fprintf(stderr, "%s: lskelCKSC3 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 15:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lmawanleecurv4subfields2002(image, nsteps))
      {
	fprintf(stderr, "%s: lmawanleecurv4subfields2002 failed\n", argv[0]);
	exit(1);
      } 
    case 16:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv2subfields2010(image, nsteps, 1))
      {
	fprintf(stderr, "%s: lnemethetalcurv2subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 17:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv2subfields2010(image, nsteps, 2))
      {
	fprintf(stderr, "%s: lnemethetalcurv2subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 18:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv2subfields2010(image, nsteps, 3))
      {
	fprintf(stderr, "%s: lnemethetalcurv2subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 19:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv4subfields2010(image, nsteps, 1))
      {
	fprintf(stderr, "%s: lnemethetalcurv4subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 20:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv4subfields2010(image, nsteps, 2))
      {
	fprintf(stderr, "%s: lnemethetalcurv4subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 21:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv4subfields2010(image, nsteps, 3))
      {
	fprintf(stderr, "%s: lnemethetalcurv4subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 22:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv8subfields2010(image, nsteps, 1))
      {
	fprintf(stderr, "%s: lnemethetalcurv8subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 23:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv8subfields2010(image, nsteps, 2))
      {
	fprintf(stderr, "%s: lnemethetalcurv8subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 24:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv8subfields2010(image, nsteps, 3))
      {
	fprintf(stderr, "%s: lnemethetalcurv8subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 25:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lsheetalcurvsym2009(image, nsteps))
      {
	fprintf(stderr, "%s: lsheetalcurvsym2009 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 26:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!ltsaofu6dirsurf1981(image, nsteps))
      {
	fprintf(stderr, "%s: ltsaofu6dirsurf1981 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 27:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!ltsaofu6dirsurf1981(image, nsteps)) // 1st pass: surface thinning
      {
	fprintf(stderr, "%s: ltsaofu6dirsurf1981 failed\n", argv[0]);
	exit(1);
      } 
      if (!ltsaofu6dircurv1981(image, nsteps)) // 2nd pass: curvilinear thinning
      {
	fprintf(stderr, "%s: ltsaofu6dircurv1981 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 28:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv2subfields2010(image, nsteps, 0))
      {
	fprintf(stderr, "%s: lnemethetalcurv2subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 29:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv4subfields2010(image, nsteps, 0))
      {
	fprintf(stderr, "%s: lnemethetalcurv4subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 30:
      if (argc == 6)
	fprintf(stderr, "%s: warning: inhibit mode not implemented for algo %d\n", argv[0], mode);
      if (!lnemethetalcurv8subfields2010(image, nsteps, 0))
      {
	fprintf(stderr, "%s: lnemethetalcurv8subfields2010 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 31:
      if (!llohoubertrandsymsurf2007(image, inhibit, nsteps))
      {
	fprintf(stderr, "%s: llohoubertrandsymsurf2007 failed\n", argv[0]);
	exit(1);
      } 
      break;
    case 32:
      if (!lmanzanerasurf1999(image, inhibit, nsteps))
      {
	fprintf(stderr, "%s: lmanzanerasurf1999 failed\n", argv[0]);
	exit(1);
      } 
      break;
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

  if (inhibit!=NULL) freeimage(inhibit);
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */








