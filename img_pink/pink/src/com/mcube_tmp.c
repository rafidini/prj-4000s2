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
/*! \file mcube.c

\brief topologically correct "marching cubes"-like algorithm

<B>Usage:</B> mcube in.pgm threshold nregul obj_id format [connex] out

<B>Description:</B>
Generates a 3d mesh from the binary or grayscale image \b in.pgm .

The original image is first thresholded (parameter \b threshold, values 0 and 1
both fit for a binary image). Then the method described in [Lac96] is applied to
generate the 3d mesh. The resulting mesh is then smoothed by \b nregul steps of 
laplacian smoothing. The parameter \b obj_id is used to tag the generated mesh.

The parameter \b format indicate the format of the output file 
(choices are POV, POVB, COL, MCM, AC, GL, VTK, RAW).
The keyword POVB corresponds to a bare Povray mesh: 
a header and a footer must be catenated in order to make a full Povray scene. 
The keyword RAW is the exchange format for the "mesh" software: see http://mesh.berlios.de/

The optional parameter \b connex indicates the connexity used for the object.
Possible values are 6 and 26 (default).

[Lac96] J.-O. Lachaud, "Topologically defined iso-surfaces", DGCI'96, LNCS 1176 (245--256), Springer Verlag, 1996.

<B>Types supported:</B> byte 3d

<B>Category:</B> mesh3d
\ingroup  mesh3d

\author Michel Couprie
*/

/* ATTENTION: option non documentee (threshold = 255) pour les points fixes */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcrbtp.h>
#include <mcmesh.h>
#include <mciomesh.h>
#include <lmcube.h>


/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * f;
  FILE *fileout = NULL;
  uint8_t v;
  int32_t nregul;
  int32_t obj_id;
  int32_t format;
  
  if ((argc != 7) && (argc != 8))
  {
    fprintf(stderr, "usage: %s in.pgm threshold nregul obj_id format [connex] out.txt \n", argv[0]);
    exit(0);
  }

  f = readimage(argv[1]);  
  if (f == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(0);
  }

  if (datatype(f) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: bad data type\n", argv[0]);
    exit(0);
  }

  v = (uint8_t)atoi(argv[2]);
  nregul = atoi(argv[3]);
  obj_id = atoi(argv[4]);
  if ((strcmp(argv[5], "pov") == 0) || (strcmp(argv[5], "POV") == 0))
    format = T_POV;
  else if ((strcmp(argv[5], "povb") == 0) || (strcmp(argv[5], "POVB") == 0))
    format = T_POVB;
  else if ((strcmp(argv[5], "col") == 0) || (strcmp(argv[5], "COL") == 0))
    format = T_COL;
  else if ((strcmp(argv[5], "mcm") == 0) || (strcmp(argv[5], "MCM") == 0))
    format = T_MCM;
  else if ((strcmp(argv[5], "ac") == 0) || (strcmp(argv[5], "AC") == 0))
    format = T_AC;
  else if ((strcmp(argv[5], "gl") == 0) || (strcmp(argv[5], "GL") == 0))
    format = T_GL;
  else if ((strcmp(argv[5], "vtk") == 0) || (strcmp(argv[5], "VTK") == 0))
    format = T_VTK;
  else if ((strcmp(argv[5], "raw") == 0) || (strcmp(argv[5], "RAW") == 0))
    format = T_RAW;
  else 
  {
    fprintf(stderr, "%s: formats: POV, POVB, COL, MCM, AC, GL, VTK, RAW\n", argv[0]);
    exit(0);
  }

  if (argc == 8)
  {
    int32_t connex = atoi(argv[6]);
    if (connex == 6) // inverse l'image
    {
      uint8_t *pt;
      int32_t i, N = rowsize(f) * colsize(f) * depth(f);
      for (pt = UCHARDATA(f), i = 0; i < N; i++, pt++)
        if (*pt) *pt = 0; else *pt = NDG_MAX;  
    }
    else if (connex != 26)
    {
      fprintf(stderr, "%s: bad connexity\n", argv[0]);
      exit(0);
    }
  }

  fileout = fopen(argv[argc - 1],"w");
  if (!fileout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(0);
  }

  InitLUT();
  InitMesh(1000); /* reallocation automatique en cas de besoin */

  if (v == 255)
  { // version avec preservation de points fixes
    if (! lmarchingcubes2(f, nregul, obj_id, fileout, format))
    {
      fprintf(stderr, "%s: function lmarchingcubes2 failed\n", argv[0]);
      exit(0);
    }
  }
  else
  {
    if (! lmarchingcubes(f, v, nregul, obj_id, fileout, format))
    {
      fprintf(stderr, "%s: function lmarchingcubes failed\n", argv[0]);
      exit(0);
    }
  } 

  TermineMesh();
  freeimage(f);
  fclose(fileout);
  return 0;
} /* main */


