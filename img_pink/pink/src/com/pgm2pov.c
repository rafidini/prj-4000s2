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
/*! \file pgm2pov.c

\brief generates a povray file from a 3D image

<B>Usage:</B> pgm2pov in.pgm mode out.pov

<B>Description:</B>

<B>mode = </B>
   0 (none)
   1 (between points)
   2 (everywhere)
   3 (6-links)
   4 (26-links)
   5 (cubes)
   6 (triangles)
   10-14: idem 0-4, khalimsky grid
   15: colored khalimsky 

<B>Types supported:</B> byte 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

#define GENHEADER
#define GENFOOTER

#define SCALEX 1
#define SCALEY 1
#define SCALEZ 1

#define HEAD1 "#include \"colors.inc\""
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#define HEAD2 "#include \"shapes.inc\""
#define HEAD3 "#include \"textures.inc\""
#define HEAD4 "#include \"stones.inc\""

#define DECL1  "#declare ColSphere = color red 1.0 green 1.0 blue 0.0;"
#define DECL2  "#declare ColCylinder = color red 0.0 green 0.0 blue 1.0;"
#define DECL3  "#declare RadSphere = 0.15;"
#define DECL4  "#declare RadCylinder = 0.02;"
#define DECL5  "#declare ColCube  = color red 1.0 green 0.0 blue 0.0;"
#define DECL6  "#declare ColCarre = color red 0.0 green 1.0 blue 0.0;"
#define DECL7  "#declare ColInter = color red 0.0 green 1.0 blue 1.0;"
#define DECL8  "#declare ColSingl = color red 1.0 green 1.0 blue 0.0;"
#define DECL9  "#declare RadCube  = 0.15;"
#define DECL10 "#declare RadCarre = 0.13;"
#define DECL11 "#declare RadInter = 0.11;"
#define DECL12 "#declare RadSingl = 0.10;"

#define DECL1VOX  "#declare texture1 = T_Stone17"

#define LIGHT0 "light_source { <0,0,-20> color White }"
#define LIGHT1 "light_source { <20,40,-30> color White }"
#define LIGHT2 "light_source { <-20,-40,-15> color Gray80 }"
#define LIGHT3 "light_source { <20,-10,15> color Gray80 }"
#define LIGHT4 "light_source { <-20,0,0> color Gray80 }"

#define LIGHT10 "light_source { <0,0,-400> color White }"
#define LIGHT11 "light_source { <400,800,-600> color White }"
#define LIGHT12 "light_source { <-400,-800,-300> color Gray80 }"
#define LIGHT13 "light_source { <400,-200,300> color Gray80 }"
#define LIGHT14 "light_source { <-400,0,0> color Gray80 }"

#define DEBIMAGE "#declare Image = union {"
#define FINIMAGE "} // Image"

#include <stdio.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>

/* =============================================================== */
void camera (FILE * fd, double x, double y, double z, double lx, double ly, double lz)
/* =============================================================== */
{
  fprintf(fd, "camera { location <%g,%g,%g> up <0,1,0> right <1,0,0> look_at <%g,%g,%g> }\n", x, y, z, lx, ly, lz);
}

/* =============================================================== */
void object (FILE * fd, double x, double y, double z)
/* =============================================================== */
{
  /*  fprintf(fd, "object { Image translate <%g,%g,%g> rotate <clock,clock,clock> }\n", x, y, z); */
  fprintf(fd, "object { Image translate <%g,%g,%g> rotate <10,10,0> }\n", x, y, z);
}

/* =============================================================== */
void sphere (FILE * fd, double x, double y, double z, char * r, char *color)
/* =============================================================== */
{
  fprintf(fd, "sphere { <%g,%g,%g>, %s pigment {color %s} }\n", x, y, z, r, color);
}

/* =============================================================== */
void cylinder (FILE * fd, double x1, double y1, double z1,
                          double x2, double y2, double z2, char * r, char *color)
/* =============================================================== */
{
  fprintf(fd, "cylinder { <%g,%g,%g>, <%g,%g,%g>, %s open pigment {color %s} }\n", 
              x1, y1, z1, x2, y2, z2, r, color);
}

/* =============================================================== */
void voxcube (FILE * fd, double x, double y, double z, char *texture)
/* =============================================================== */
{
  fprintf(fd, "box { <%g,%g,%g>, <%g,%g,%g> %s }\n", 
              SCALEX*(x-0.5), SCALEY*(y-0.5), SCALEZ*(z-0.5), 
              SCALEX*(x+0.5), SCALEY*(y+0.5), SCALEZ*(z+0.5), texture);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  char * filename;
  FILE *fd = NULL;
  int32_t rs, cs, ps, d, N, x, y, z, k;
  int32_t mode, khalimsky = 0;
  uint8_t * F;
  int32_t xmin, xmax, ymin, ymax, zmin, zmax;
  int32_t tabdir[13];                /* tableau des "directions positives" */
  double bx, by, bz, npoints;    /* pour le calcul du barycentre global */

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s image.pgm mode out.pov \n", argv[0]);
    fprintf(stderr, "       mode = 0 (none), 1 (between points), 2 (everywhere), \n");
    fprintf(stderr, "              3 (6-links), 4 (26-links), 5 (cubes)\n");
    fprintf(stderr, "              10-14: idem 0-4, khalimsky grid\n");
    fprintf(stderr, "              15: colored khalimsky grid\n");
    exit(1);
  }

  mode = atoi(argv[2]);

  if (mode == 15) 
  {
    image = readimage(argv[1]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    rs = image->row_size;
    cs = image->col_size;
    ps = rs * cs;
    d = depth(image);
    N = d * ps;
    F = UCHARDATA(image);
    filename = argv[argc - 1];
    fd = fopen(filename, "w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], filename);
      exit(1);
    }
    /* calcule le barycentre */
    npoints = bx = by = bz = 0;
      for (z = 0; z < d; z++)
        for (y = 0; y < cs; y++)
          for (x = 0; x < rs; x++)
            if (F[z * ps + y * rs + x]) 
	    {
              bx += x; by += y; bz += z; npoints++;
            }
    bx /= npoints;
    by /= npoints;
    bz /= npoints;

#ifdef GENHEADER
    fprintf(fd, "//Generated by pgm2pov (M. Couprie 1999) from file : %s ;\n", argv[1]);
    fprintf(fd, "//Image : %s\n\n", image->name);

    fprintf(fd, "%s\n%s\n%s\n%s\n", HEAD1, HEAD2, HEAD3, HEAD4);

    fprintf(fd, "#declare Cube = box { <-15,-15,-15>, <15,15,15> pigment {color Red} }\n");
    fprintf(fd, "#declare CarreXY = box { <-15,-15,-4>, <15,15,4> pigment {color Magenta} }\n");
    fprintf(fd, "#declare CarreYZ = box { <-4,-15,-15>, <4,15,15> pigment {color Magenta} }\n");
    fprintf(fd, "#declare CarreXZ = box { <-15,-4,-15>, <15,4,15> pigment {color Magenta} }\n");
    fprintf(fd, "#declare SegmentX = box { <-15,-4,-4>, <15,4,4> pigment {color Green} }\n");
    fprintf(fd, "#declare SegmentY = box { <-4,-15,-4>, <4,15,4> pigment {color Green} }\n");
    fprintf(fd, "#declare SegmentZ = box { <-4,-4,-15>, <4,4,15> pigment {color Green} }\n");
    fprintf(fd, "#declare Point = sphere { <0,0,0>, 6 pigment {color Blue} }\n");

    camera(fd, 0, 0, -600, 0, 0, 0);
    fprintf(fd, "%s\n%s\n%s\n%s\n%s\n", LIGHT10, LIGHT11, LIGHT12, LIGHT13, LIGHT14);
    fprintf(fd, "background {color Gray50} \n");
    fprintf(fd, "%s\n", DEBIMAGE);
#endif

    for (z = 0; z < d; z++)
      for (y = 0; y < cs; y++)
        for (x = 0; x < rs; x++)
          switch (F[z * ps + y * rs + x]) 
	  {
            case 0: break;
            case NDG_SINGL3D: fprintf(fd, "object{ Point translate <%d,%d,%d>}\n", 
                                      50*(x-(int32_t)bx), 50*(y-(int32_t)by), 50*(z-(int32_t)bz)); break;
            case NDG_INTER3DX: fprintf(fd, "object{ SegmentX translate <%d,%d,%d>}\n", 
                                      50*(x-(int32_t)bx), 50*(y-(int32_t)by), 50*(z-(int32_t)bz)); break;
            case NDG_INTER3DY: fprintf(fd, "object{ SegmentY translate <%d,%d,%d>}\n", 
                                      50*(x-(int32_t)bx), 50*(y-(int32_t)by), 50*(z-(int32_t)bz)); break;
            case NDG_INTER3DZ: fprintf(fd, "object{ SegmentZ translate <%d,%d,%d>}\n", 
                                      50*(x-(int32_t)bx), 50*(y-(int32_t)by), 50*(z-(int32_t)bz)); break;
            case NDG_CARRE3DXY: fprintf(fd, "object{ CarreXY translate <%d,%d,%d>}\n", 
                                      50*(x-(int32_t)bx), 50*(y-(int32_t)by), 50*(z-(int32_t)bz)); break;
            case NDG_CARRE3DYZ: fprintf(fd, "object{ CarreYZ translate <%d,%d,%d>}\n", 
                                      50*(x-(int32_t)bx), 50*(y-(int32_t)by), 50*(z-(int32_t)bz)); break;
            case NDG_CARRE3DXZ: fprintf(fd, "object{ CarreXZ translate <%d,%d,%d>}\n", 
                                      50*(x-(int32_t)bx), 50*(y-(int32_t)by), 50*(z-(int32_t)bz)); break;
            case NDG_CUBE3D: fprintf(fd, "object{ Cube translate <%d,%d,%d>}\n", 
                                      50*(x-(int32_t)bx), 50*(y-(int32_t)by), 50*(z-(int32_t)bz)); break;
	  }

#ifdef GENFOOTER
    fprintf(fd, "%s\n", FINIMAGE);
    fprintf(fd, "object { Image translate <0,0,0> rotate <150,0,0> }\n");
#endif

    fclose(fd);
    exit(1);
  }

  if (mode > 9) 
  {
    mode = mode - 10;
    khalimsky = 1;
  }

  if ((mode < 0) || (mode > 5))
  {
    fprintf(stderr, "usage: %s image.pgm mode out.pov \n", argv[0]);
    fprintf(stderr, "       mode = 0 (none), 1 (between points), 2 (everywhere) \n");
    fprintf(stderr, "              3 (6-links), 4 (26-links), 5 (cubes)\n");
    fprintf(stderr, "              10-14: idem 0-4, khalimsky grid\n");
    fprintf(stderr, "              15: colored khalimsky grid\n");
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = image->row_size;
  cs = image->col_size;
  ps = rs * cs;
  d = depth(image);
  N = d * ps;
  F = UCHARDATA(image);

  filename = argv[argc - 1];
  fd = fopen(filename, "w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], filename);
    exit(1);
  }

  /* calcule le barycentre */
  npoints = bx = by = bz = 0;
    for (z = 0; z < d; z++)
      for (y = 0; y < cs; y++)
        for (x = 0; x < rs; x++)
          if (F[z * ps + y * rs + x]) 
	  {
            bx += x; by += y; bz += z; npoints++;
          }
  bx /= npoints;
  by /= npoints;
  bz /= npoints;

#ifdef GENHEADER
  fprintf(fd, "//Generated by pgm2pov (M. Couprie 1999) from file : %s ;\n", argv[1]);
  fprintf(fd, "//Image : %s\n\n", image->name);

  fprintf(fd, "%s\n%s\n%s\n%s\n", HEAD1, HEAD2, HEAD3, HEAD4);
  if (mode == 5)
  {
    fprintf(fd, "%s\n", DECL1VOX);
  }
  else
  {
    fprintf(fd, "%s\n%s\n%s\n%s\n", DECL1, DECL2, DECL3, DECL4);
    fprintf(fd, "%s\n%s\n%s\n%s\n", DECL5, DECL6, DECL7, DECL8);
    fprintf(fd, "%s\n%s\n%s\n%s\n", DECL9, DECL10, DECL11, DECL12);
  }
  camera(fd, 0, 0, -15, 0, 0, 0);
  fprintf(fd, "%s\n%s\n%s\n%s\n%s\n", LIGHT0, LIGHT1, LIGHT2, LIGHT3, LIGHT4);
  fprintf(fd, "%s\n", DEBIMAGE);
#endif

  if (mode == 5)
  {
      for (z = 0; z < d; z++)
        for (y = 0; y < cs; y++)
          for (x = 0; x < rs; x++)
            if (F[z * ps + y * rs + x] == 1) 
              voxcube(fd, x, y, z, (char *)"texture {texture1}");
	    else if (F[z * ps + y * rs + x] == 2) 
              voxcube(fd, x, y, z, (char *)"texture {texture2}");
  }

  if (mode != 5)
  {
    /* dessine les spheres */
    if (khalimsky)
    {
      for (z = 0; z < d; z++)
        for (y = 0; y < cs; y++)
          for (x = 0; x < rs; x++)
            if (F[z * ps + y * rs + x]) 
 	    {
              if (CUBE3D(x,y,z))
                sphere(fd, x, y, z, (char *)"RadCube", (char *)"ColCube");
              else if (CARRE3D(x,y,z))
                sphere(fd, x, y, z, (char *)"RadCarre", (char *)"ColCarre");
              else if (INTER3D(x,y,z))
                sphere(fd, x, y, z, (char *)"RadInter", (char *)"ColInter");
              else if (SINGL3D(x,y,z))
                sphere(fd, x, y, z, (char *)"RadSingl", (char *)"ColSingl");
	    }
    }
    else
    {  
      for (z = 0; z < d; z++)
        for (y = 0; y < cs; y++)
          for (x = 0; x < rs; x++)
            if (F[z * ps + y * rs + x]) sphere(fd, x, y, z, (char *)"RadSphere", (char *)"ColSphere");
    }
  }

  /* dessine la "cage a poules" */
  if ((mode == 1) || (mode == 2))
  {
    /* trace les barreaux a y,z constant (horizontaux en largeur)  */
    for (z = 0; z < d; z++)
      for (y = 0; y < cs; y++)
      {
        xmin = 0; 
        xmax = rs - 1; 
        if (mode == 1) 
        {
          xmax = 0;
          for (x = 0; x < rs; x++) if (F[z * ps + y * rs + x]) { xmin = x ; break; }
          for (x = rs - 1; x >= 0; x--) if (F[z * ps + y * rs + x]) { xmax = x ; break; }
        }
        if (xmax > xmin) 
          for (x = xmin; x < xmax; x++) 
            cylinder(fd, x, y, z, x+1, y, z, (char *)"RadCylinder", (char *)"ColCylinder");
      }

    /* trace les barreaux a x,z constant (verticaux)  */
    for (z = 0; z < d; z++)
      for (x = 0; x < rs; x++)
      {
        ymin = 0; 
        ymax = cs - 1; 
        if (mode == 1) 
        {
          ymax = 0;
          for (y = 0; y < cs; y++) if (F[z * ps + y * rs + x]) { ymin = y ; break; }
          for (y = cs - 1; y >= 0; y--) if (F[z * ps + y * rs + x]) { ymax = y ; break; }
        }
        if (ymax > ymin) 
          for (y = ymin; y < ymax; y++) 
            cylinder(fd, x, y, z, x, y+1, z, (char *)"RadCylinder", (char *)"ColCylinder");
      }

    /* trace les barreaux a x,y constant (horizontaux en profondeur)  */
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
      {
        zmin = 0; 
        zmax = d - 1; 
        if (mode == 1) 
        {
          zmax = 0;
          for (z = 0; z < d; z++) if (F[z * ps + y * rs + x]) { zmin = z ; break; }
          for (z = d - 1; z >= 0; z--) if (F[z * ps + y * rs + x]) { zmax = z ; break; }
        }
        if (zmax > zmin) 
          for (z = zmin; z < zmax; z++) 
            cylinder(fd, x, y, z, x, y, z+1, (char *)"RadCylinder", (char *)"ColCylinder");
      }
  }

  if (mode == 3)
  {
    int32_t y1, y2, y3, x1, x2, x3, n;

    /* calcule les trois "directions positives" */
    x = 9 + 3 + 1;           /* point (1,1,1) dans un cube 3x3x3 */
    n = 0;
    for (k = 0; k <= 10; k += 2)
    {
      y = voisin6(x, k, 3, 9, 27);
      y1 = y % 3;
      y2 = (y % 9) / 3;
      y3 = y / 9;
      if (y1 + y2 + y3 - 3 > 0) tabdir[n++] = k;
    }
    if (n != 3) { fprintf(stderr, "BUG! BUG! BUG! n = %d\n", n); exit(1); }

    /* trace les voisinages */
    for (x = 0; x < N; x++)
      if (F[x]) 
      {
        x1 = x % rs;
        x2 = (x % ps) / rs;
        x3 = x / ps;
        for (k = 0; k < n; k ++)
	{
          y = voisin6(x, tabdir[k], rs, ps, N);
          if ((y != -1) && (F[y]))
	  {
            y1 = y % rs;
            y2 = (y % ps) / rs;
            y3 = y / ps;
            cylinder(fd, x1, x2, x3, y1, y2, y3, (char *)"RadCylinder", (char *)"ColCylinder");
	  }
	}
      }    
  } /* if (mode == 3) */

  if (mode == 4)
  {
    int32_t y1, y2, y3, x1, x2, x3, n;

    /* calcule les treize "directions positives" */
    x = 9 + 3 + 1;           /* point (1,1,1) dans un cube 3x3x3 */
    n = 0;
    for (k = 0; k < 26; k++)
    {
      y = voisin26(x, k, 3, 9, 27);
      if (y == -1) { fprintf(stderr, "BUG: erreur voisin = -1 : k = %d\n", k); exit(1); }
      y1 = y % 3;
      y2 = (y % 9) / 3;
      y3 = y / 9;
      if (y1 + y2 + y3 - 3 > 0)
        tabdir[n++] = k;
      if ( (y1 + y2 + y3 - 3 == 0) && (3 * (y1-1) + 2 * (y2-1) + y3-1 > 0) )
        tabdir[n++] = k;
    }
    if (n != 13) { fprintf(stderr, "BUG! BUG! BUG! n = %d\n", n); exit(1); }

    /* trace les voisinages */
    for (x = 0; x < N; x++)
      if (F[x]) 
      {
        x1 = x % rs;
        x2 = (x % ps) / rs;
        x3 = x / ps;
        for (k = 0; k < n; k ++)
	{
          y = voisin26(x, tabdir[k], rs, ps, N);
          if ((y != -1) && (F[y]))
	  {
            y1 = y % rs;
            y2 = (y % ps) / rs;
            y3 = y / ps;
            cylinder(fd, x1, x2, x3, y1, y2, y3, (char *)"RadCylinder", (char *)"ColCylinder");
	  }
	}
      }    
  } /* if (mode == 4) */

#ifdef GENFOOTER
  fprintf(fd, "%s\n", FINIMAGE);
  object(fd, -bx, -by, -bz);
#endif

  fclose(fd);
  freeimage(image);
  return 0;
}

