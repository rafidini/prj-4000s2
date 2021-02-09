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
/*! \file meshoffset.c

\brief applies an offset to all points of a mesh (translation)

<B>Usage:</B> meshoffset in [ox oy oz] out

<B>Description:</B>

Applies an offset to all points of a mesh (translation). 
Available input formats: vtk.
Available output formats: vtk.

<B>Types supported:</B> mesh 3d

<B>Category:</B> mesh3d
\ingroup  mesh3d

\author Michel Couprie
*/

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

#define VERBOSE
#define TOOMUCH

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  FILE *filein = NULL;
  FILE *fileout = NULL;
  int32_t i;
  int32_t formatin, formatout;
  double  resolution;
  
  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in [resolution] out\n", argv[0]);
    exit(0);
  }
  if (argc == 4) resolution = atof(argv[2]);

  formatin = UNKNOWN;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".MCM") == 0) formatin = T_MCM;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".mcm") == 0) formatin = T_MCM;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".VTK") == 0) formatin = T_VTK;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".vtk") == 0) formatin = T_VTK;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".IFS") == 0) formatin = T_IFS;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".ifs") == 0) formatin = T_IFS;
  if (strcmp(argv[1]+strlen(argv[1])-5, ".CGAL") == 0) formatin = T_CGAL;
  if (strcmp(argv[1]+strlen(argv[1])-5, ".cgal") == 0) formatin = T_CGAL;
  if (formatin == UNKNOWN)
  {
    fprintf(stderr, "%s: bad input file format\n", argv[0]);
    exit(0);
  }
  formatout = UNKNOWN;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".MCM") == 0) formatout = T_MCM;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".mcm") == 0) formatout = T_MCM;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".VTK") == 0) formatout = T_VTK;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".vtk") == 0) formatout = T_VTK;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".PGM") == 0) formatout = T_PGM;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".pgm") == 0) formatout = T_PGM;
  if (formatout == UNKNOWN)
  {
    fprintf(stderr, "%s: bad output file format\n", argv[0]);
    exit(0);
  }

  filein = fopen(argv[1],"r");
  if (filein == NULL)
  {
    fprintf(stderr, "%s: cannot open %s\n", argv[0], argv[1]);
    exit(0);
  }
  if (formatin == T_MCM) LoadMeshMCM(filein);
  if (formatin == T_IFS) LoadMeshIFS(filein);
  if (formatin == T_VTK) LoadBuildVTK(filein);
  if (formatin == T_CGAL) LoadMeshCGAL(filein);
  fclose(filein);

  fileout = fopen(argv[argc-1],"w");
  if (formatout == T_MCM) SaveMeshMCM(fileout);
  if (formatout == T_VTK) 
  {
    genheaderVTK(fileout, (char *)"meshoffset output");    
    SaveMeshVTK(fileout);
  }
  if (formatout == T_PGM) 
  {
    // 0 1 2     ...     rs-2 rs-1
    //   |bxmin  ...  bxmax|
    meshbox B;
    meshvertex V;
    double xrange, yrange, zrange;
    int32_t rs, cs, ds, ps, N, x, y, z;
    struct xvimage *image;
    uint8_t *F;

    BoundingBoxMesh(&B);
#ifdef VERBOSE    
    printf("bxmin=%g bxmax=%g\n", B.bxmin, B.bxmax);
    printf("bymin=%g bymax=%g\n", B.bymin, B.bymax);
    printf("bzmin=%g bzmax=%g\n", B.bzmin, B.bzmax);
#endif
    xrange = B.bxmax - B.bxmin;
    yrange = B.bymax - B.bymin;
    zrange = B.bzmax - B.bzmin;
    rs = (int32_t)(xrange / resolution) + 2;
    cs = (int32_t)(yrange / resolution) + 2;
    ds = (int32_t)(zrange / resolution) + 2;
#ifdef VERBOSE    
    printf("rs=%d cs=%d ds=%d \n", rs, cs, ds);
#endif
    ps = rs * cs;
    N = ps * ds;
#ifdef TOOMUCH    
    if (N > 10000000) 
    {
      printf("maximum size exceeded, exit\n");
      exit(0);
    }
#endif
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(0);
    }
    F = UCHARDATA(image);
    memset(F, NDG_MIN, N);
    for (i = 0; i < Vertices->cur; i++)
    {                              /* pour chaque sommet de la grille */
      V = Vertices->v[i];
      x = (int32_t)((V.x - B.bxmin) / resolution) + 1;
      y = (int32_t)((V.y - B.bymin) / resolution) + 1;
      z = (int32_t)((V.z - B.bzmin) / resolution) + 1;
      F[z*ps + y*rs + x] = NDG_MAX;
    } /* for i */
    writeimage(image,argv[argc-1]);
  }
  fclose(fileout);
  return 0;
} /* main */
