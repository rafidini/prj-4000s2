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
/*! \file sceneconvert.c

\brief converts a 3D scene into another format

<B>Usage:</B> sceneconvert in.3sc format out

<B>Description:</B>
The file \b scene.3sc contains a scene under the 3Dscene format.

The parameter \b format indicate the format of the output file 
(choices are POV, POVB).
The keyword POVB corresponds to a bare Povray mesh: a header and a footer must be
catenated in order to make a full Povray scene. 

<B>Types supported:</B> 3D scene

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mcgeo.h>
#include <mcrbtp.h>
#include <mcmesh.h>
#include <mciomesh.h>

#define RADIUS 9
#define TOLERANCE 1.0e-1

/* =============================================================== */
void genheaderscenePOV(FILE *fileout, meshbox MB)
/* =============================================================== */
{
  fprintf(fileout, "#include \"colors.inc\"\n");
  fprintf(fileout, "#include \"shapes.inc\"\n");
  fprintf(fileout, "#include \"textures.inc\"\n");
  fprintf(fileout, "#include \"stones.inc\"\n");

  fprintf(fileout, "\n");
  fprintf(fileout, "camera { location <0,0,%g> up <0,1,0> right <1,0,0> look_at <0,0,0> }\n",
                   2.7*MB.bzmax);
  fprintf(fileout, "\n");
  fprintf(fileout, "light_source { <%g,%g,%g> color White }\n", 
                   2*MB.bxmin, 2*MB.bymax, 2*MB.bzmax);
  fprintf(fileout, "light_source { <%g,%g,%g> color Gray80 }\n", 
                   2*MB.bxmax, 2*MB.bymax, 2*MB.bzmax);
  fprintf(fileout, "light_source { <%g,%g,%g> color Gray80 }\n", 
                   2*MB.bxmin, 2*MB.bymin, 2*MB.bzmax);
  fprintf(fileout, "light_source { <%g,%g,%g> color Gray80 }\n", 
                   2*MB.bxmax, 2*MB.bymin, 2*MB.bzmax);
  fprintf(fileout, "light_source { <%g,%g,%g> color White }\n", 
                   2*MB.bxmax, 2*MB.bymax, 2*MB.bzmin);
  fprintf(fileout, "light_source { <%g,%g,%g> color White }\n", 
	  (double)0, (double)0, 3*MB.bzmax);
  fprintf(fileout, "\n");
  fprintf(fileout, "#declare mytexture = Bright_Bronze\n");
  fprintf(fileout, "#declare mytolerance = %g;\n", TOLERANCE);
  fprintf(fileout, "#declare myradius = %d;\n", RADIUS);
  fprintf(fileout, "\n");
  fprintf(fileout, "#declare Image = union {\n");
} /* genheaderscenePOV() */

/* =============================================================== */
void genfooterscenePOV(FILE *fileout, meshbox MB)
/* =============================================================== */
{
  fprintf(fileout, "} // Image\nobject { Image translate <%g,%g,%g> rotate <0,0,0> }\n",
	  -(MB.bxmax-MB.bxmin)/2, -(MB.bymax-MB.bymin)/2, -(MB.bzmax-MB.bzmin)/2);
} /* genfooterscenePOV() */

/* =============================================================== */
void genmeshbox(scene *s, meshbox *MB)
/* =============================================================== */
{
  int32_t i, j, npoints;
  double x, y, z;
  MB->bxmin = MB->bxmax = s->tabobj[0]->points[0].x;
  MB->bymin = MB->bymax = s->tabobj[0]->points[0].y;
  MB->bzmin = MB->bzmax = s->tabobj[0]->points[0].z;
  for (i = 0; i < s->nobj; i++)
  {
    npoints = s->tabobj[i]->npoints;
    for (j = 0; j < npoints; j++)
    {
      x = s->tabobj[i]->points[j].x;
      y = s->tabobj[i]->points[j].y;
      z = s->tabobj[i]->points[j].z;
      if (MB->bxmin > x) MB->bxmin = x; else if (MB->bxmax < x) MB->bxmax = x;
      if (MB->bymin > y) MB->bymin = y; else if (MB->bymax < y) MB->bymax = y;
      if (MB->bzmin > z) MB->bzmin = z; else if (MB->bzmax < z) MB->bzmax = z;
    }
  } // for (i = 0; i < nobj; i++)
} // genmeshbox()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t i, j, npoints, format, type;
  FILE *fdout = NULL;
  double x, y, z;
  scene *scene_in;
  meshbox MB;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.3sc format out \n", argv[0]);
    exit(1);
  }

  if ((strcmp(argv[2], "pov") == 0) || (strcmp(argv[2], "POV") == 0))
    format = T_POV;
  else if ((strcmp(argv[2], "povb") == 0) || (strcmp(argv[2], "POVB") == 0))
    format = T_POVB;
  else 
  {
    fprintf(stderr, "%s: available formats: POV, POVB\n", argv[0]);
    exit(0);
  }

  scene_in = readscene(argv[1]);
  if (scene_in == NULL)
  {
    fprintf(stderr, "%s: readscene failed\n", argv[0]);
    exit(1);
  }

  fdout = fopen(argv[argc-1], "w");
  if (!fdout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

  if (format == T_POV) 
  {
    genmeshbox(scene_in, &MB);
    genheaderscenePOV(fdout, MB);
  }

  for (i = 0; i < scene_in->nobj; i++)
  {
    npoints = scene_in->tabobj[i]->npoints;
    type = scene_in->tabobj[i]->objtype;
    if (type == OBJTYPE_LINE)
    {
      fprintf(fdout, "sphere_sweep { linear_spline %d\n", npoints);
      for (j = 0; j < npoints-1; j++)
      {
	x = scene_in->tabobj[i]->points[j].x;
	y = scene_in->tabobj[i]->points[j].y;
	z = scene_in->tabobj[i]->points[j].z;
	fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      }
      x = scene_in->tabobj[i]->points[npoints-1].x;
      y = scene_in->tabobj[i]->points[npoints-1].y;
      z = scene_in->tabobj[i]->points[npoints-1].z;
      fprintf(fdout, "<%g,%g,%g>, myradius\n", x, y, z);
    }
    else if (type == OBJTYPE_CLOSEDLINE)
    {
      fprintf(fdout, "sphere_sweep { linear_spline %d\n", npoints+1);
      for (j = 0; j < npoints; j++)
      {
	x = scene_in->tabobj[i]->points[j].x;
	y = scene_in->tabobj[i]->points[j].y;
	z = scene_in->tabobj[i]->points[j].z;
	fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      }
      x = scene_in->tabobj[i]->points[0].x;
      y = scene_in->tabobj[i]->points[0].y;
      z = scene_in->tabobj[i]->points[0].z;
      fprintf(fdout, "<%g,%g,%g>, myradius\n", x, y, z);
      fprintf(fdout, "  tolerance mytolerance\n  texture{mytexture}\n}\n");
    }
    else if (type == OBJTYPE_SPLINE)
    {
      fprintf(fdout, "sphere_sweep { cubic_spline %d\n", npoints+2);
      x = scene_in->tabobj[i]->points[0].x;
      y = scene_in->tabobj[i]->points[0].y;
      z = scene_in->tabobj[i]->points[0].z;
      fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      for (j = 1; j < npoints-1; j++)
      {
	x = scene_in->tabobj[i]->points[j].x;
	y = scene_in->tabobj[i]->points[j].y;
	z = scene_in->tabobj[i]->points[j].z;
	fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      }
      x = scene_in->tabobj[i]->points[npoints-1].x;
      y = scene_in->tabobj[i]->points[npoints-1].y;
      z = scene_in->tabobj[i]->points[npoints-1].z;
      fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      fprintf(fdout, "<%g,%g,%g>, myradius\n", x, y, z);
      fprintf(fdout, "  tolerance mytolerance\n  texture{mytexture}\n}\n");
    }
    else if (type == OBJTYPE_CLOSEDSPLINE)
    {
      fprintf(fdout, "sphere_sweep { cubic_spline %d\n", npoints+3);
      for (j = 0; j < npoints; j++)
      {
	x = scene_in->tabobj[i]->points[j].x;
	y = scene_in->tabobj[i]->points[j].y;
	z = scene_in->tabobj[i]->points[j].z;
	fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      }
      x = scene_in->tabobj[i]->points[0].x;
      y = scene_in->tabobj[i]->points[0].y;
      z = scene_in->tabobj[i]->points[0].z;
      fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      x = scene_in->tabobj[i]->points[1].x;
      y = scene_in->tabobj[i]->points[1].y;
      z = scene_in->tabobj[i]->points[1].z; 
      fprintf(fdout, "<%g,%g,%g>, myradius,\n", x, y, z);
      x = scene_in->tabobj[i]->points[2].x;
      y = scene_in->tabobj[i]->points[2].y;
      z = scene_in->tabobj[i]->points[2].z;
      fprintf(fdout, "<%g,%g,%g>, myradius\n", x, y, z);
      fprintf(fdout, "  tolerance mytolerance\n  texture{mytexture}\n}\n");
    }
    else fprintf(stderr, "%s : warning : i = %d ; unsupported object type : %d\n", 
		 argv[0], i, scene_in->tabobj[i]->objtype);
  } // for (i = 0; i < nobj; i++)

  if (format == T_POV) 
  {
    genfooterscenePOV(fdout, MB);
  }

  fclose(fdout);

  return 0;
} /* main */


