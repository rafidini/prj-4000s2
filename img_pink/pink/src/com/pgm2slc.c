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
/* \file pgm2slc.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  Traduit un fichier image (pgm) en fichier SLC (format pour le visualisateur 3D VolVis)

  Michel Couprie - novembre 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>

#define C_SLICE_MAGIC 11111

void C_err_msg(char *string)
{
	fprintf(stderr, "ERROR: %s", string);
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, ds, ps, N;
  uint8_t *F;
  int32_t type;
  float	x_units;
  float	y_units;
  float	z_units;
  int32_t unit_type;
  int32_t data_origin;
  int32_t data_modification;
  int32_t bits_per_voxel;
  char tmp_msg[1024];
  int32_t done;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s image.pgm [i[nteractive]] out.slc \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  ps = rs * cs;
  N = ps * ds;
  F = UCHARDATA(image);
  type = datatype(image);

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

  switch (type)
  {
    case VFF_TYP_1_BYTE: bits_per_voxel = 8; break;
    default: fprintf(stderr,"%s : bad data type, must be VFF_TYP_1_BYTE\n", argv[0]);
             exit(1);
  } /* switch (type) */

  if (argc == 4)
  {
	do
	{
	    	printf(
		    "X Size of a voxel (distance between samples) in units : ");
	    	scanf("%s", tmp_msg );
		sscanf( tmp_msg, "%f", &x_units );

		if ( x_units <= 0.0 )
	    	{
		    	sprintf( tmp_msg, "Unreasonable x unit size : %f\n",
					   x_units );
		    	C_err_msg( tmp_msg );
			done = 0;
	    	}
		else	
			done = 1;
	} while ( !done );

	do
	{
	    	printf(
		    "Y Size of a voxel (distance between samples) in units : ");
	    	scanf("%s", tmp_msg );
		sscanf( tmp_msg, "%f", &y_units );

		if ( y_units <= 0.0 )
	    	{
		    	sprintf( tmp_msg, "Unreasonable y unit size : %f\n",
					   y_units );
		    	C_err_msg( tmp_msg );
			done = 0;
	    	}
		else	
			done = 1;
	} while ( !done );

	do
	{
	    	printf(
		    "Z Size of a voxel (distance between samples) in units : ");
	    	scanf("%s", tmp_msg );
		sscanf( tmp_msg, "%f", &z_units );

		if ( z_units <= 0.0 )
	    	{
		    	sprintf( tmp_msg, "Unreasonable z unit size : %f\n",
					   z_units );
		    	C_err_msg( tmp_msg );
			done = 0;
	    	}
		else	
			done = 1;
	} while ( !done );

	do
	{
  		printf("\t\t Units Types = \n");
  		printf("\t\t         0 Specifies Meter\n");
  		printf("\t\t         1 Specifies Millimeter\n");
  		printf("\t\t         2 Specifies Micrometer\n");
  		printf("\t\t         3 Specifies Foot\n");
	    	printf( "Unit type : ");
	    	scanf("%s", tmp_msg );
		sscanf( tmp_msg, "%d", &unit_type );

		if ( unit_type < 0 || unit_type > 3 )
	    	{
		    	sprintf( tmp_msg, "Unreasonable unit type : %d\n",
					   unit_type );
		    	C_err_msg( tmp_msg );
			done = 0;
	    	}
		else	
			done = 1;
	} while ( !done );

	do
	{
  		printf("\t\t Data Origin = \n");
  		printf("\t\t         0 Specifies BioRad Confocal Microscope\n");
  		printf("\t\t         1 Specifies Magnetic Resonance\n");
  		printf("\t\t         2 Specifies Computed Tomography\n");
  		printf("\t\t         3 Specifies Simulation\n");
  		printf("\t\t         4 Specifies Binary Voxelized Data\n");
  		printf("\t\t         5 Specifies Fuzzy Voxelized Data\n");
  		printf("\t\t         6 Specifies Other\n");
	    	printf( "Data origin : ");
	    	scanf("%s", tmp_msg );
		sscanf( tmp_msg, "%d", &data_origin );

		if ( data_origin < 0 || data_origin > 6 )
	    	{
		    	sprintf( tmp_msg, "Unreasonable data origin : %d\n",
					   data_origin );
		    	C_err_msg( tmp_msg );
			done = 0;
	    	}
		else	
			done = 1;
	} while ( !done );

	do
	{
  		printf("\t\t Data Modification = \n");
  		printf("\t\t         0 Specifies Original Data\n");
  		printf("\t\t         1 Specifies Resampled Data\n");
  		printf("\t\t         2 Specifies Filtered Data\n");
  		printf("\t\t         3 Specifies Resampled, Filtered Data\n");
  		printf("\t\t         4 Specifies Other Modification\n");
	    	printf( "Data modification : ");
	    	scanf("%s", tmp_msg );
		sscanf( tmp_msg, "%d", &data_modification );

		if ( data_modification < 0 || data_modification > 4 )
	    	{
		    	sprintf( tmp_msg, 
				"Unreasonable data modification : %d\n",
				 data_modification );
		    	C_err_msg( tmp_msg );
			done = 0;
	    	}
		else	
			done = 1;
	} while ( !done );
  } /* if (argc == 4) */
  else
  {
    x_units = 1;
    y_units = 1;
    z_units = 1;
    unit_type  = 1;
    data_origin = 6;
    data_modification = 4;
  }

  fprintf(fd, "%d\n", C_SLICE_MAGIC);
  fprintf(fd, "%d %d %d %d\n", rs, cs, ds, bits_per_voxel);
  fprintf(fd, "%f %f %f\n", x_units, y_units, z_units);
  fprintf(fd, "%d %d %d %d\n", unit_type, data_origin, data_modification, 0);
  done = fwrite(F, sizeof(char), N, fd);
  if (done != N)
  {
    fprintf(stderr, "%s: only %d items written\n", argv[0], done);
    exit(1);
  }

  fclose(fd);
  freeimage(image);
  return 0;
}
