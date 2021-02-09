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
/* \file segmentnum.c

\brief tool for multiclass image segmentation

<B>Usage:</B> segmentnum in.pgm measure threshold out.pgm

<B>Description:</B>
Segmentation by selection of components in the component tree, computed on the 
lineal gradient of the original image.

The parameter "measure" specifies one of the following measures: <B>surf</B> (area), 
<B>prof</B> (depth), or <B>vol</B> (volume). The parameter "threshold" is an integer 
value. 

<B>Types supported:</B> byte 2D

<B>Category:</B> connect
\ingroup connect

\warning experimental - do not use in applications

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsegmentnum.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * result;
  int32_t trace;
  char * nameout;
  int32_t mesure;
  int32_t seuil;

  if ((argc < 5) || (argc > 6) || ((argc == 6) && (strcmp(argv[4], "trace") != 0)))
  {
    fprintf(stderr, "usage: %s filein.pgm {surf|prof|vol} seuil [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2], "surf") == 0) mesure = SURFACE;
  else if (strcmp(argv[2], "prof") == 0) mesure = PROFONDEUR;
  else if (strcmp(argv[2], "vol") == 0) mesure = VOLUME;
  else 
  {
    fprintf(stderr, "usage: %s filein.pgm {surf|prof|vol} seuil [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  seuil = atoi(argv[3]);

  if (argc == 5)
  {
    trace = 0;
    nameout = argv[4];
  }
  else
  {
    trace = 1;
    nameout = argv[5];
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segmentnum: readimage failed\n");
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "segmentnum: allocimage failed\n");
    exit(1);
  }

  if (! lsegmentnum(image, mesure, seuil, trace, result))
  {
    fprintf(stderr, "segmentnum: lsegmentnum failed\n");
    exit(1);
  }

  writelongimage(result, nameout);
  freeimage(image);
  freeimage(result);
  return 0;
} /* main */



