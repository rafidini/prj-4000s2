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
/*! \file dilatballnum.c

\brief morphological grayscale dilation by an Euclidean ball

<B>Usage:</B> dilatballnum in.pgm r out.pgm

<B>Description:</B>
Morphological grayscale dilation by an Euclidean ball.
The parameter \b r must a positive number (int or float), 
the structuring element for this dilation is a ball (or a disc) of radius \b r.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2006

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>
//#include <mcconvert.h>
//#include <ldraw.h>    EN LOCAL POUR FACILITER L'EXPORT

/* ==================================== */
void ldrawdisk(struct xvimage * image1, double r, double xc, double yc)
/* ==================================== */
/* draws a euclidean disk */
#undef F_NAME
#define F_NAME "ldrawdisk"
{
  int32_t i, j, rs, cs;
  uint8_t *F;
  double r2, x, y;
  
  rs = rowsize(image1);
  cs = colsize(image1);
  F = UCHARDATA(image1);
  
  r2 = r * r;

  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    x = xc - i; 
    y = yc - j; 

    if (x * x + y * y <= r2)
      F[j * rs + i] = NDG_MAX;
  }
} // ldrawdisk()

/* ==================================== */
void ldrawball(struct xvimage * image1, double r, double xc, double yc, double zc)
/* ==================================== */
/* draws a euclidean ball */
#undef F_NAME
#define F_NAME "ldrawball"
{
  int32_t i, j, k, rs, cs, ds, ps;
  uint8_t *F;
  double r2, x, y, z;
  
  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);
  ps = rs*cs;
  F = UCHARDATA(image1);
  
  r2 = r * r;
  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    x = xc - i; 
    y = yc - j; 
    z = zc - k; 
    if (x * x + y * y + z * z <= r2)
      F[k * ps + j * rs + i] = NDG_MAX;
  }
} // ldrawball()

/* ==================================== */
int32_t image2coords(
        struct xvimage *f,
        int32_t **tabx,
        int32_t **taby,
        int32_t *nbpoints)
/* ==================================== */
#undef F_NAME
#define F_NAME "image2coords"
{
  register int32_t i, j, nbpts;
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(f);
  int32_t *tab_x;               /* liste des coord. x des points */
  int32_t *tab_y;               /* liste des coord. y des points */

  if (depth(f) != 1) 
  {
    fprintf(stderr, "%s: not for 3d images\n", F_NAME);
    return 0;
  }

  nbpts = 0;
  for (i = 0; i < N; i++) if (F[i]) nbpts ++;

  tab_x = (int32_t *)calloc(1, nbpts * sizeof(int32_t));
  tab_y = (int32_t *)calloc(1, nbpts * sizeof(int32_t));
  if ((tab_x == NULL) || (tab_y == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab\n", F_NAME);
     return(0);
  }

  nbpts = 0;
  for (j = 0; j < cs; j++)
    for (i = 0; i < rs; i++)
      if (F[j * rs + i])
      {
        tab_x[nbpts] = i;
        tab_y[nbpts] = j;
        nbpts++;
      }

  *tabx = tab_x;
  *taby = tab_y;
  *nbpoints = nbpts;
  return 1;
} // image2coords()

/* ==================================== */
int32_t image2coords3(
        struct xvimage *f,
        int32_t **tabx,
        int32_t **taby,
        int32_t **tabz,
        int32_t *nbpoints)
/* ==================================== */
#undef F_NAME
#define F_NAME "image2coords3"
{
  register int32_t i, j, k, nbpts;
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(f);
  int32_t *tab_x;               /* liste des coord. x des points */
  int32_t *tab_y;               /* liste des coord. y des points */
  int32_t *tab_z;               /* liste des coord. z des points */

  nbpts = 0;
  for (i = 0; i < N; i++) if (F[i]) nbpts++;

  tab_x = (int32_t *)calloc(1, nbpts * sizeof(int32_t));
  tab_y = (int32_t *)calloc(1, nbpts * sizeof(int32_t));
  tab_z = (int32_t *)calloc(1, nbpts * sizeof(int32_t));
  if ((tab_x == NULL) || (tab_y == NULL) || (tab_z == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab\n", F_NAME);
     return(0);
  }

  nbpts = 0;
  for (k = 0; k < ds; k++)
    for (j = 0; j < cs; j++)
      for (i = 0; i < rs; i++)
	if (F[k*ps + j*rs + i])
	{
	  tab_x[nbpts] = i;
	  tab_y[nbpts] = j;
	  tab_z[nbpts] = k;
	  nbpts++;
	}

  *tabx = tab_x;
  *taby = tab_y;
  *tabz = tab_z;
  *nbpoints = nbpts;
  return 1;
} // image2coords3()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * es;
  double r;
  int32_t xc, yc, zc, rs, cs, ds, nbpoints;
  int32_t *tabx, *taby, *tabz;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s f.pgm r out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: bad image type (byte assumed)\n", argv[0]);
    exit(1);
  }

  r = atof(argv[2]);
  if (r < 0)
  {
    fprintf(stderr, "%s: error: r must be >= 0\n", argv[0]);
    exit(1);
  }

  if (depth(image) == 1) // 2D
  {
    cs = rs = 2 * ((int)r) + 3;
    yc = xc = ((int)r) + 1;
    es = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
    if (es == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }

    ldrawdisk(es, r, (double)xc, (double)yc);

    if (! image2coords(es, &tabx, &taby, &nbpoints))
    {
      fprintf(stderr, "%s: function image2coords failed\n", argv[0]);
      exit(1);
    }

    if (! ldilat2(image, nbpoints, tabx, taby, xc, yc))
    {
      fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
      exit(1);
    }
  }
  else // 3D
  {
    ds = cs = rs = 2 * ((int)r) + 3;
    zc = yc = xc = ((int)r) + 1;
    es = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (es == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }

    ldrawball(es, r, (double)xc, (double)yc, (double)zc);

    if (! image2coords3(es, &tabx, &taby, &tabz, &nbpoints))
    {
      fprintf(stderr, "%s: function image2coords3 failed\n", argv[0]);
      exit(1);
    }

    if (! ldilat3(image, nbpoints, tabx, taby, tabz, xc, yc, zc))
    {
      fprintf(stderr, "%s: function ldilat3 failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(es);
  return 0;
} /* main */
