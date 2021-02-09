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
/*! \file skelsmoothing.c

\brief computes a smmothed version of a curvilinear skeleton

<B>Usage:</B> skelsmoothing in.skel mode param out.skel

<B>Description:</B>
Computes a smmothed version of a curvilinear skeleton.

The parameter \b mode specifies the smoothing method:
\li 0: straight lines segments
\li 1: splines

<B>Types supported:</B> 2Dskel, 3Dskel

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2011
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>

//#define DEBUG

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  skel * S, *SS;
  int32_t mode;
  double param;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.skel mode param fileout.skel\n", argv[0]);
    exit(1);
  }

  S = readskel(argv[1]);
  if (S == NULL)
  {
    fprintf(stderr, "%s: readskel failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[2]);
  param = atof(argv[3]);

  if (!(SS = lskelsmoothing(S, mode, param)))
  {
    fprintf(stderr, "%s: function lskelsmoothing failed\n", argv[0]);
    exit(1);
  }

  writeskel(SS, argv[argc-1]);
  termineskel(S);
  termineskel(SS);

  return 0;
} /* main */
