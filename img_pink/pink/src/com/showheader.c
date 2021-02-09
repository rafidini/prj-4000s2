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
/*! \file showheader.c

\brief prints image information

<B>Usage:</B> showheader in.pgm

<B>Description:</B>
Prints all informations contained in the image file header: type, header, size and comments.

<B>Types supported:</B> all pgm and ppm types

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - septembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <mcimage.h>

#define BUFFERSIZE 1024

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s file.pgm \n", argv[0]);
    exit(1);
  }

  if (!showheader(argv[1]))
  {
    fprintf(stderr, "%s: function showheader failed \n", argv[0]);
    exit(1);
  }

  return 0;
} /* main */
