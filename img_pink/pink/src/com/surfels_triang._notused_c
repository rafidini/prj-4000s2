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
/* 
  Generation d'un maillage triangulaire d'un ensemble de surfels  
  Michel Couprie  -  Juillet 2001
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcmesh.h>
#include <mciomesh.h>

#define CSTVOL

/*
#define DEBUG
*/

/* =============================================================== */
int32_t lsurfels(struct xvimage * f, uint8_t v, 
                   int32_t nregul, int32_t obj_id, FILE *fileout)
/* =============================================================== */
{
  int32_t i, j, k, nbfac, fac;
  int32_t rs, cs, ps, ds, N;
  uint8_t * F;
  uint8_t s;
  meshvertex V;
  double x1, y1, z1, x2, y2, z2, x3, y3, z3 ;
  double xoff, yoff, zoff;
  double h = 0.5;
  double v0, v1, fc;
  meshbox MB0, MB1; 
  double shrink;

  /* v est la valeur de seuil. si v == 0 alors il s'agit d'une image binaire */
  if (v == 0) s = 1; else s = v;

  rs = rowsize(f);
  cs = colsize(f);
  ds = depth(f);
  ps = rs * cs;
  N = ps * ds;
  F = UCHARDATA(f);

  for (k = 0; k < ds-1; k++)
  for (j = 0; j < cs-1; j++)
  for (i = 0; i < rs-1; i++)
  {
    v = F[k*ps + j*rs + i];
    if (v < s)
    {
      if (F[k*ps + j*rs + i+1] >= s) 
      {
        (void)AddFace(i+h, j-h, k-h, i+h, j+h, k-h, i+h, j-h, k+h);
        (void)AddFace(i+h, j-h, k+h, i+h, j+h, k-h, i+h, j+h, k+h);
      }
      if (F[k*ps + (j+1)*rs + i] >= s)
      {
        (void)AddFace(i-h, j+h, k-h, i-h, j+h, k+h, i+h, j+h, k-h);
        (void)AddFace(i-h, j+h, k+h, i+h, j+h, k+h, i+h, j+h, k-h);
      }
      if (F[(k+1)*ps + j*rs + i] >= s)
      {
        (void)AddFace(i-h, j-h, k+h, i+h, j-h, k+h, i-h, j+h, k+h);
        (void)AddFace(i+h, j-h, k+h, i+h, j+h, k+h, i-h, j+h, k+h);
      }
    }
    else /* v >= s */
    {
      if (F[k*ps + j*rs + i+1] < s) 
      {
        (void)AddFace(i+h, j-h, k+h, i+h, j+h, k-h, i+h, j-h, k-h);
        (void)AddFace(i+h, j+h, k+h, i+h, j+h, k-h, i+h, j-h, k+h);
      }
      if (F[k*ps + (j+1)*rs + i] < s)
      {
        (void)AddFace(i+h, j+h, k-h, i-h, j+h, k+h, i-h, j+h, k-h);
        (void)AddFace(i+h, j+h, k-h, i+h, j+h, k+h, i-h, j+h, k+h);
      }
      if (F[(k+1)*ps + j*rs + i] < s)
      {
        (void)AddFace(i-h, j+h, k+h, i+h, j-h, k+h, i-h, j-h, k+h);
        (void)AddFace(i-h, j+h, k+h, i+h, j+h, k+h, i+h, j-h, k+h);
      }
    }
  }

  IsobarMesh(&x1, &y1, &z1);
  TranslateMesh(-x1, -y1, -z1);
  v0 = VolMesh();
  printf("Volume original = %g\n", v0);
  BoundingBoxMesh(&MB0);
  printf("xmin = %g ; xmax = %g\n", MB0.bxmin, MB0.bxmax);
  printf("ymin = %g ; ymax = %g\n", MB0.bymin, MB0.bymax);
  printf("zmin = %g ; zmax = %g\n", MB0.bzmin, MB0.bzmax);
  for (i = 0; i < nregul; i++)
  {
    RegulMesh(); 
    RestoreCoords(); 
    IsobarMesh(&x2, &y2, &z2);
    TranslateMesh(-x2, -y2, -z2);
    x1 += x2; y1 += y2; z1 += z2; 
#ifdef CSTVOL 
    BoundingBoxMesh(&MB1);
    shrink = (MB1.bxmax - MB1.bxmin) / (MB0.bxmax - MB0.bxmin); ZoomMeshX(1/shrink);
    shrink = (MB1.bymax - MB1.bymin) / (MB0.bymax - MB0.bymin); ZoomMeshY(1/shrink);
    shrink = (MB1.bzmax - MB1.bzmin) / (MB0.bzmax - MB0.bzmin); ZoomMeshZ(1/shrink);
    v1 = VolMesh();
    printf("Regul. iter. %d, Volume = %g\n", i+1, v1);
    fc = pow(v0 / v1, 1.0/3); 
    printf("facteur correctif = %g\n", fc);
    ZoomMesh(fc);
    printf("rect. Volume = %g\n", VolMesh());
#endif
  }
  TranslateMesh(x1, y1, z1);

  genheaderPOV(fileout, obj_id, MB0);

  xoff = 0.5-rs/2.0;
  yoff = 0.5-cs/2.0;
  zoff = 0.5-ds/2.0;

  for (i = 0; i < Faces->cur; i++)
  {
    V = Vertices->v[ Faces->f[i].vert[0] ];
    x1 = V.x; y1 = V.y; z1 = V.z;
    V = Vertices->v[ Faces->f[i].vert[1] ];
    x2 = V.x; y2 = V.y; z2 = V.z;
    V = Vertices->v[ Faces->f[i].vert[2] ];
    x3 = V.x; y3 = V.y; z3 = V.z;
    genfacePOV(fileout, xoff+x1, yoff+y1, zoff+z1, xoff+x2, yoff+y2, zoff+z2, xoff+x3, yoff+y3, zoff+z3);
  }

  genfooterPOV(fileout);
  return 1;

} /* lsurfels() */

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * f;
  FILE *fileout = NULL;
  uint8_t v;
  int32_t nregul;
  int32_t obj_id;
  
  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in.pgm threshold nregul obj_id out.txt \n", argv[0]);
    exit(0);
  }

  f = readimage(argv[1]);  
  if (f == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(0);
  }

  v = (uint8_t)atoi(argv[2]);
  nregul = atoi(argv[3]);
  obj_id = atoi(argv[4]);

  fileout = fopen(argv[argc - 1],"w");
  if (!fileout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(0);
  }

  InitMesh(1000); /* reallocation automatique en cas de besoin */

  if (! lsurfels(f, v, nregul, obj_id, fileout))
  {
    fprintf(stderr, "%s: function lsurfels failed\n", argv[0]);
    exit(0);
  } 

  TermineMesh();
  freeimage(f);
  fclose(fileout);

} /* main */
