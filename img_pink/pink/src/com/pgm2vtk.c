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
/*! \file pgm2vtk.c

\brief generates a vtk file from a 3D image

<B>Usage:</B> pgm2vtk in.pgm mode out.vtk

<B>Description:</B>
Create a vtk image from a 3D pgm image.
If <B>mode</B> = 
\li     1: surfels (2-faces) from a binary khalimsky grid
\li     2: triangulated surfels (2-faces) from a binary khalimsky grid
\li     3: voxels (cubes) from a binary image in Z3
\li     4: minimal grid from a binary image in Z3
\li     5: voxels (octaedrons) from a binary image in Z3
\li     6: linels (1-faces) from a binary khalimsky grid
\li     7: pointels (0-faces) from a binary khalimsky grid

<B>Types supported:</B> byte 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie (2005)
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>

//#define PARANO

#define RADIUSCUBE 0.2
#define RADIUSOCTAEDRON 0.2
#define RADIUSLINE 0.03
#define RADIUSDOT 0.08

#define NBPOINTS_CUBE 8
#define NBFACES_CUBE 6
#define SIZE_FACE_CUBE 5

#define NBPOINTS_OCTAEDRON 6
#define NBFACES_OCTAEDRON 8
#define SIZE_FACE_OCTAEDRON 4

static double xscale = 1.0;
static double yscale = 1.0;
static double zscale = 1.0;

/* =============================================================== */
void add_points_line_z (FILE * fd, double x, double y, double z1, double z2, double r)
/* =============================================================== */
{
  fprintf(fd, "%g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g\n", 
	  xscale*(x-r), yscale*(y-r), zscale*(z1),  
	  xscale*(x+r), yscale*(y-r), zscale*(z1), 
	  xscale*(x+r), yscale*(y+r), zscale*(z1),  
	  xscale*(x-r), yscale*(y+r), zscale*(z1),  
	  xscale*(x-r), yscale*(y-r), zscale*(z2),  
	  xscale*(x+r), yscale*(y-r), zscale*(z2),  
	  xscale*(x+r), yscale*(y+r), zscale*(z2),  
	  xscale*(x-r), yscale*(y+r), zscale*(z2)
	 );
}

/* =============================================================== */
void add_points_line_y (FILE * fd, double x, double y1, double y2, double z, double r)
/* =============================================================== */
{
  fprintf(fd, "%g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g\n", 
	  xscale*(x-r), yscale*(y1), zscale*(z-r),  
	  xscale*(x+r), yscale*(y1), zscale*(z-r), 
	  xscale*(x+r), yscale*(y2), zscale*(z-r),  
	  xscale*(x-r), yscale*(y2), zscale*(z-r),  
	  xscale*(x-r), yscale*(y1), zscale*(z+r),  
	  xscale*(x+r), yscale*(y1), zscale*(z+r),  
	  xscale*(x+r), yscale*(y2), zscale*(z+r),  
	  xscale*(x-r), yscale*(y2), zscale*(z+r)
	 );
}

/* =============================================================== */
void add_points_line_x (FILE * fd, double x1, double x2, double y, double z, double r)
/* =============================================================== */
{
  fprintf(fd, "%g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g\n", 
	  xscale*(x1), yscale*(y-r), zscale*(z-r),  
	  xscale*(x2), yscale*(y-r), zscale*(z-r), 
	  xscale*(x2), yscale*(y+r), zscale*(z-r),  
	  xscale*(x1), yscale*(y+r), zscale*(z-r),  
	  xscale*(x1), yscale*(y-r), zscale*(z+r),  
	  xscale*(x2), yscale*(y-r), zscale*(z+r),  
	  xscale*(x2), yscale*(y+r), zscale*(z+r),  
	  xscale*(x1), yscale*(y+r), zscale*(z+r)
	 );
}

/* =============================================================== */
void add_points_cube (FILE * fd, int32_t x, int32_t y, int32_t z, double r)
/* =============================================================== */
{
  fprintf(fd, "%g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g\n", 
	  xscale*(x-r), yscale*(y-r), zscale*(z-r),  
	  xscale*(x+r), yscale*(y-r), zscale*(z-r), 
	  xscale*(x+r), yscale*(y+r), zscale*(z-r),  
	  xscale*(x-r), yscale*(y+r), zscale*(z-r),  
	  xscale*(x-r), yscale*(y-r), zscale*(z+r),  
	  xscale*(x+r), yscale*(y-r), zscale*(z+r),  
	  xscale*(x+r), yscale*(y+r), zscale*(z+r),  
	  xscale*(x-r), yscale*(y+r), zscale*(z+r)
	 );
}

/* =============================================================== */
void add_cells_cube (FILE * fd, int32_t x)
/* =============================================================== */
{
  fprintf(fd, "4   %5d %5d %5d %5d\n", 8*x+0, 8*x+1, 8*x+2, 8*x+3);
  fprintf(fd, "4   %5d %5d %5d %5d\n", 8*x+4, 8*x+5, 8*x+6, 8*x+7);
  fprintf(fd, "4   %5d %5d %5d %5d\n", 8*x+0, 8*x+1, 8*x+5, 8*x+4);
  fprintf(fd, "4   %5d %5d %5d %5d\n", 8*x+2, 8*x+3, 8*x+7, 8*x+6);
  fprintf(fd, "4   %5d %5d %5d %5d\n", 8*x+0, 8*x+4, 8*x+7, 8*x+3);
  fprintf(fd, "4   %5d %5d %5d %5d\n", 8*x+1, 8*x+2, 8*x+6, 8*x+5);
}

/* =============================================================== */
void add_points_octaedron (FILE * fd, int32_t x, int32_t y, int32_t z, double r)
/* =============================================================== */
{
  fprintf(fd, "%g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g    %g %g %g\n", 
	  x-r, (double)y, (double)z,  
	  x+r, (double)y, (double)z, 
	  (double)x, y-r, (double)z,  
	  (double)x, y+r, (double)z,  
	  (double)x, (double)y, z-r,  
	  (double)x, (double)y, z+r
	 );
}

/* =============================================================== */
void add_cells_octaedron (FILE * fd, int32_t x)
/* =============================================================== */
{
  fprintf(fd, "3   %5d %5d %5d\n", 6*x+0, 6*x+3, 6*x+4);
  fprintf(fd, "3   %5d %5d %5d\n", 6*x+4, 6*x+3, 6*x+1);
  fprintf(fd, "3   %5d %5d %5d\n", 6*x+1, 6*x+3, 6*x+5);
  fprintf(fd, "3   %5d %5d %5d\n", 6*x+5, 6*x+3, 6*x+0);
  fprintf(fd, "3   %5d %5d %5d\n", 6*x+4, 6*x+2, 6*x+0);
  fprintf(fd, "3   %5d %5d %5d\n", 6*x+1, 6*x+2, 6*x+4);
  fprintf(fd, "3   %5d %5d %5d\n", 6*x+5, 6*x+2, 6*x+1);
  fprintf(fd, "3   %5d %5d %5d\n", 6*x+0, 6*x+2, 6*x+5);
}

/* =============================================================== */
void add_points_xy (FILE * fd, int32_t x, int32_t y, int32_t z)
/* =============================================================== */
{
  fprintf(fd, "%g %g %g    %g %g %g    %g %g %g    %g %g %g\n", 
	  xscale*(x-1), yscale*(y-1), zscale*(z),        
	  xscale*(x+1), yscale*(y-1), zscale*(z),     
	  xscale*(x+1), yscale*(y+1), zscale*(z),   
	  xscale*(x-1), yscale*(y+1), zscale*(z)
);
}

/* =============================================================== */
void add_points_xz (FILE * fd, int32_t x, int32_t y, int32_t z)
/* =============================================================== */
{
  fprintf(fd, "%g %g %g    %g %g %g    %g %g %g    %g %g %g\n", 
	  xscale*(x-1), yscale*(y), zscale*(z-1),        
	  xscale*(x+1), yscale*(y), zscale*(z-1),
	  xscale*(x+1), yscale*(y), zscale*(z+1),
	  xscale*(x-1), yscale*(y), zscale*(z+1)
	  );
}

/* =============================================================== */
void add_points_yz (FILE * fd, int32_t x, int32_t y, int32_t z)
/* =============================================================== */
{
  fprintf(fd, "%g %g %g    %g %g %g    %g %g %g    %g %g %g\n", 
	  xscale*(x), yscale*(y-1), zscale*(z-1),
	  xscale*(x), yscale*(y+1), zscale*(z-1),
	  xscale*(x), yscale*(y+1), zscale*(z+1),
	  xscale*(x), yscale*(y-1), zscale*(z+1)
	  );
}

/* =============================================================== */
void add_cell (FILE * fd, int32_t x)
/* =============================================================== */
{
  fprintf(fd, "4   %5d %5d %5d %5d\n", 4*x, 4*x+1, 4*x+2, 4*x+3);
}

/* =============================================================== */
void add_cell_triang (FILE * fd, int32_t x)
/* =============================================================== */
{
  fprintf(fd, "3   %5d %5d %5d\n", 4*x, 4*x+1, 4*x+2);
  fprintf(fd, "3   %5d %5d %5d\n", 4*x, 4*x+2, 4*x+3);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t mode;
  char * filename;
  FILE *fd = NULL;
  int32_t rs, cs, ps, ds, x, y, z;
  uint8_t * K;
  int32_t nb_cells = 0;
  int32_t nb_points = 0;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s image.pgm mode out.vtk \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  K = UCHARDATA(image);
  rs = image->row_size;
  cs = image->col_size;
  ps = rs * cs;
  ds = depth(image);
  if (image->xdim != 0.0)
  {
    xscale = image->xdim;
    yscale = image->ydim;
    zscale = image->zdim;
  }
  mode = atoi(argv[2]);
  
  filename = argv[argc - 1];
  fd = fopen(filename, "w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], filename);
    exit(1);
  }

  if ((mode == 1) || (mode == 2))
  {

    fprintf (fd, "# vtk DataFile Version 2.0\n");
    fprintf (fd, "pgm2vtk output\n");
    fprintf (fd, "ASCII\n\n");
    fprintf (fd, "DATASET POLYDATA\n");

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	{
	  if (K[z * ps + y * rs + x] && CARRE3D(x,y,z)) 
	    nb_cells++;
	}

    nb_points = 4 * nb_cells;
    fprintf (fd, "POINTS %d float\n", nb_points);

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x]) 
 	  {
#ifdef PARANO
	    if (CUBE3D(x,y,z))
	    {
	      fprintf(stderr, "%s: warning: 3D element found\n", argv[0]);
	    }
#endif
	    if (CARRE3DXY(x,y,z)) add_points_xy(fd, x, y, z);
	    else if (CARRE3DXZ(x,y,z)) add_points_xz(fd, x, y, z);
	    else if (CARRE3DYZ(x,y,z)) add_points_yz(fd, x, y, z);
	  }
  
    if (mode == 1)
    { 
      fprintf (fd, "\nPOLYGONS %d %d\n", nb_cells, 5 * nb_cells);
      for (x=0; x < nb_cells; x++) add_cell(fd, x);
    }
    if (mode == 2) 
    {  
      fprintf (fd, "\nPOLYGONS %d %d\n", 2 * nb_cells, 8 * nb_cells);
      for (x=0; x < nb_cells; x++) add_cell_triang(fd, x);
    }
  }
  else

  if (mode == 3)
  {
    fprintf (fd, "# vtk DataFile Version 2.0\n");
    fprintf (fd, "pgm2vtk output\n");
    fprintf (fd, "ASCII\n\n");
    fprintf (fd, "DATASET POLYDATA\n");

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x]) 
	    nb_cells++;

    nb_points = NBPOINTS_CUBE * nb_cells;
    fprintf (fd, "POINTS %d float\n", nb_points);

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x])
	    add_points_cube(fd, x, y, z, RADIUSCUBE);

    fprintf (fd, "\nPOLYGONS %d %d\n", 
	     NBFACES_CUBE * nb_cells, NBFACES_CUBE * SIZE_FACE_CUBE * nb_cells);
    for (x=0; x < nb_cells; x++) add_cells_cube(fd, x);

  }
  else
  if (mode == 4)
  {
    int imin, imax;

    fprintf (fd, "# vtk DataFile Version 2.0\n");
    fprintf (fd, "pgm2vtk output\n");
    fprintf (fd, "ASCII\n\n");
    fprintf (fd, "DATASET POLYDATA\n");

    for (x = 0; x < rs; x++)
      for (y = 0; y < cs; y++)
      {
	for (z = 0; z < ds; z++)
	  if (K[z * ps + y * rs + x]) 
	  { nb_cells++; break; }
	  
      }
    for (x = 0; x < rs; x++)
      for (z = 0; z < ds; z++)
      {
	for (y = 0; y < cs; y++)
	  if (K[z * ps + y * rs + x]) 
	  { nb_cells++; break; }
	  
      }
    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
      {
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x]) 
	  { nb_cells++; break; }
	
      }

    nb_points = NBPOINTS_CUBE * nb_cells;
    fprintf (fd, "POINTS %d float\n", nb_points);

    for (x = 0; x < rs; x++)
      for (y = 0; y < cs; y++)
      {
	imin = imax = -1;
	for (z = 0; z < ds; z++)
	  if (K[z * ps + y * rs + x]) 
	  { imin = z; break; }
	for (z = ds - 1; z >= 0; z--)
	  if (K[z * ps + y * rs + x]) 
	  { imax = z; break; }
	if (imin != -1)
	  add_points_line_z(fd, x, y, imin, imax, RADIUSLINE);
      }
    for (x = 0; x < rs; x++)
      for (z = 0; z < ds; z++)
      {
	imin = imax = -1;
	for (y = 0; y < cs; y++)
	  if (K[z * ps + y * rs + x]) 
	  { imin = y; break; }
	for (y = cs - 1; y >= 0; y--)
	  if (K[z * ps + y * rs + x]) 
	  { imax = y; break; }
	if (imin != -1)
	  add_points_line_y(fd, x, imin, imax, z, RADIUSLINE);
      }
    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
      {
	imin = imax = -1;
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x]) 
	  { imin = x; break; }
	for (x = rs - 1; x >= 0; x--)
	  if (K[z * ps + y * rs + x]) 
	  { imax = x; break; }
	if (imin != -1)
	  add_points_line_x(fd, imin, imax, y, z, RADIUSLINE);
      }
  
    fprintf (fd, "\nPOLYGONS %d %d\n", 
	     NBFACES_CUBE * nb_cells, NBFACES_CUBE * SIZE_FACE_CUBE * nb_cells);
    for (x=0; x < nb_cells; x++) add_cells_cube(fd, x);

  }
  else
  if (mode == 5)
  {
    fprintf (fd, "# vtk DataFile Version 2.0\n");
    fprintf (fd, "pgm2vtk output\n");
    fprintf (fd, "ASCII\n\n");
    fprintf (fd, "DATASET POLYDATA\n");

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x]) 
	    nb_cells++;

    nb_points = NBPOINTS_OCTAEDRON * nb_cells;
    fprintf (fd, "POINTS %d float\n", nb_points);

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x])
	    add_points_octaedron(fd, x, y, z, RADIUSOCTAEDRON);

    fprintf (fd, "\nPOLYGONS %d %d\n", 
	     NBFACES_OCTAEDRON * nb_cells, NBFACES_OCTAEDRON * SIZE_FACE_OCTAEDRON * nb_cells);
    for (x=0; x < nb_cells; x++) add_cells_octaedron(fd, x);

  }
  else

  if (mode == 6)
  {

    fprintf (fd, "# vtk DataFile Version 2.0\n");
    fprintf (fd, "pgm2vtk output\n");
    fprintf (fd, "ASCII\n\n");
    fprintf (fd, "DATASET POLYDATA\n");

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	{
	  if (K[z * ps + y * rs + x] && INTER3D(x,y,z)) 
	    nb_cells++;
	}

    nb_points = NBPOINTS_CUBE * nb_cells;
    fprintf (fd, "POINTS %d float\n", nb_points);

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x]) 
 	  {
#ifdef PARANO
	    if (CUBE3D(x,y,z) || CARRE3D(x,y,z))
	    {
	      fprintf(stderr, "%s: warning: 2D or 3D element found\n", argv[0]);
	    }
#endif
	    if (INTER3DX(x,y,z))      add_points_line_x(fd, x-1, x+1, y, z, RADIUSLINE);
	    else if (INTER3DY(x,y,z)) add_points_line_y(fd, x, y-1, y+1, z, RADIUSLINE);
	    else if (INTER3DZ(x,y,z)) add_points_line_z(fd, x, y, z-1, z+1, RADIUSLINE);
	  }
  
    fprintf (fd, "\nPOLYGONS %d %d\n", 
	     NBFACES_CUBE * nb_cells, NBFACES_CUBE * SIZE_FACE_CUBE * nb_cells);
    for (x=0; x < nb_cells; x++) add_cells_cube(fd, x);
  }
  else

  if (mode == 7)
  {

    fprintf (fd, "# vtk DataFile Version 2.0\n");
    fprintf (fd, "pgm2vtk output\n");
    fprintf (fd, "ASCII\n\n");
    fprintf (fd, "DATASET POLYDATA\n");

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	{
	  if (K[z * ps + y * rs + x] && SINGL3D(x,y,z)) 
	    nb_cells++;
	}

    nb_points = NBPOINTS_OCTAEDRON * nb_cells;
    fprintf (fd, "POINTS %d float\n", nb_points);

    for (z = 0; z < ds; z++)
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (K[z * ps + y * rs + x] && SINGL3D(x,y,z)) 
 	  {
	    add_points_octaedron(fd, x, y, z, RADIUSDOT);
	  }
  
    fprintf (fd, "\nPOLYGONS %d %d\n", 
	     NBFACES_OCTAEDRON * nb_cells, NBFACES_OCTAEDRON * SIZE_FACE_OCTAEDRON * nb_cells);
    for (x=0; x < nb_cells; x++) add_cells_octaedron(fd, x);
  }

  else
  {
    fprintf(stderr, "%s: bad mode: %d\n", argv[0], mode);
    exit(1);
  }

  fclose(fd);
  freeimage(image);
  return 0;
}

