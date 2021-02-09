/*
\brief Building the MhLut

<B>Usage:</B> mhlut Rmax dims=(2|3) output

<B>Description:</B>

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  tables

\author Andre Vital Saude
*/

//#define _DEBUG
//#define __DEBUG
#define VERBOSE
//#define _VERBOSE


#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <avsimage.h>
#include <llut.h>
#include <lhma.h>

/* =============================================================== */
void twoExpansion(u_int8_t *B, int32_t rs, int32_t cs, int32_t ds) {
/* =============================================================== */
  int32_t x, y, z;
  int32_t zz, zmin, zmax;
  int32_t ps;

  if (ds > 1) {
    ps = rs*cs;
    zmin = -1; zmax = 1;
  } else
    ps = 0; zmin = zmax = 0;

  for (z = 0; z < ds; z+=2) {
    for (y = 0; y < cs; y+=2) {
      for (x = 0; x < rs; x+=2) {
	if (voxel(B, x, y, z, ps, rs)) {
	  for (zz = zmin; zz <= zmax; zz++) {
	    voxel(B, x-1, y-1, z+zz, ps, rs) = 255;
	    voxel(B, x-1, y, z+zz, ps, rs) = 255;
	    voxel(B, x-1, y+1, z+zz, ps, rs) = 255;
	    voxel(B, x, y-1, z+zz, ps, rs) = 255;
	    voxel(B, x, y, z+zz, ps, rs) = 255;
	    voxel(B, x, y+1, z+zz, ps, rs) = 255;
	    voxel(B, x+1, y-1, z+zz, ps, rs) = 255;
	    voxel(B, x+1, y, z+zz, ps, rs) = 255;
	    voxel(B, x+1, y+1, z+zz, ps, rs) = 255;
	  }
	}
      }
    }
  }
}

/* =============================================================== */
void createEdt(struct xvimage *sqedt, struct xvimage *white, Point c, int32_t ps, int32_t rs)
/* =============================================================== */
{
#undef F_NAME
#define F_NAME "createEdt"

  u_int32_t *D = ULONGDATA(sqedt);
  u_int8_t *W = UCHARDATA(white);


  voxel(W, c.x, c.y, c.z, ps, rs) = 0;
#ifdef __DEBUG
  writeimage(white, "tmp/_point.pgm");
#endif

  //build square Euclidean distance
  if (! lsedt_meijster(white, sqedt)) // calcule la fonction distance
  {
    fprintf(stderr, "%s: lsedt_meijster failed\n", F_NAME);
    exit(1);
  }
	
  //restablish center to white
  voxel(W, c.x, c.y, c.z, ps, rs) = 255;

#ifdef __DEBUG
  writeimage(sqedt, "tmp/_sqedt.pgm");
#endif
}

/* =============================================================== */
int32_t addVectorsToMLut(MLut *mlut, MLut *mglut, u_int32_t *HMA, int32_t radius, Point c, int32_t is3D, int32_t ps, int32_t rs) {
/* =============================================================== */
#undef F_NAME
#define F_NAME "addVectorsToMLut"

#define permute(a,b) (a)+=(b);(b)=(a)-(b);(a)-=(b)
  int32_t x,y,z;
  int32_t i,j;
  int32_t count = 0;

  //find vectors
  for (x = c.x; x >= 0; x--) {
    for(y = c.y; y >= 0; y--) {
      for(z = is3D*c.z; z >= 0; z--) {
	if ((voxel(HMA, x, y, z, ps, rs) > 0) && 
	    ((x != c.x) || (y != c.y) || (z != is3D*c.z))) //ignore center
	{
	  count++;
	  Coordinates v;
	  v.x = c.x-x;
	  v.y = c.y-y;
	  v.z = is3D*(c.z-z);

	  //count
	  if (count <= 1) {
#ifdef VERBOSE
	    printf("\n(%d,%d,%d) vector found at radius=%d\n", v.x, v.y, v.z, radius);
#endif
	    //put v in the cone x>=y>=z>=0
	    if (is3D) {
	      if (v.x < v.y) {
		permute(v.x, v.y);
	      }
	      if (v.y < v.z) {
		permute(v.y, v.z);
	      }
	      if (v.x < v.y) {
		permute(v.x, v.y);
	      }
	    } else {
	      if (v.x < v.y) {
		permute(v.x, v.y);
	      }
	    }

	    //stock vector
	    mlut->vec[mlut->numd] = v;
	    mlut->RR[mlut->numd] = radius;

            // find v in mglut
	    for (i=0; i<mglut->numd; i++) {
		Coordinates m = mglut->vec[i];
		if ((m.x == v.x) && (m.y == v.y) && (is3D*m.z == is3D*v.z)) {
		    mlut->indmap[mlut->numd] = i*mglut->rknown;
		    printf("breaking\n");
		    break;
		}
	    }
	    if (i == mglut->numd) {
		fprintf(stderr,
			"%s: the RTLut is not sufficient to calculate this MhLut!!! \nFatal error at radius %d\n",
			F_NAME, radius);
		exit(1);
	    }

	    mlut->numd++; //stock only one vector
	  } else {
#ifdef VERBOSE
	    printf("(%d,%d,%d) vector ignored at radius=%d\n", v.x, v.y, v.z, radius);
#endif
	  }
	}
      }
    }
  }
  return count;
}


/* =============================================================== */
int32_t updateMLut(int32_t radius, int32_t ei, Point c,
		   u_int32_t *D, struct xvimage *ball,
		   MLut *mlut, MLut *mglut, RTLutCol Lut, SQDLut SQDn,
		   int32_t rs, int32_t cs, int32_t ds, int32_t ps, int32_t N, int32_t is3D) {
/* =============================================================== */
  /*
    B is passed as parameter to avoid reallocating memory
  */

  //images
  struct xvimage *hma;
  u_int32_t *H;
  u_int8_t *B = UCHARDATA(ball);

  //Ei gaps
  int32_t addx, addy, addz;
  addx = addy = addz = 0;

  //auxiliar
  int32_t x, y, z;
  int32_t expandedRadius;
  static char filename[512];

  //return
  int32_t modified = 0;

  //put center in ei
  if (ei > 0) {
    addx++; c.x++;
    if (ei > 1) {
      addy++; c.y++;
      if (ei > 2) {
	addz++; c.z++;
      }
    }
  }

#ifdef _VERBOSE
  printf("CENTER in (%d,%d,%d), E%d\n", c.x, c.y, c.z, ei);
  fflush(stdout);
#endif

  //create ball from sqedt and transform to H
  memset(B, 0, N);
  for (z = 0; z < ds-is3D; z++)
    for (y = 0; y < cs-1; y++)
      for (x = 0; x < rs-1; x++)
	voxel(B, x + addx, y + addy, z + addz, ps, rs) = (voxel(D, x, y, z, ps, rs) < radius) ? 255 : 0;
  twoExpansion(B, rs, cs, ds);

#ifdef __DEBUG
  sprintf(filename, "tmp/_ballE%d_%d%d.pgm", ei, radius / 10, radius % 10);
  writeimage(ball, filename);
#endif

  //extract hma
  hma = lhma_givenTables(ball, *mlut, Lut, SQDn, &expandedRadius);
  H = ULONGDATA(hma);
#ifdef _VERBOSE
  printf("\tradius\texpandedRadius\n\t%d\t%d\n", radius, expandedRadius);
  fflush(stdout);
  //getchar();
#endif

#ifdef __DEBUG
  {
    int32_t i, count = 0, N=rs*cs*ds;

    sprintf(filename, "tmp/_hma_ball%d%d.pgm", radius / 10, radius % 10);
    writeimage(hma, filename);
    
    for (i = 0; i < N; i++)
      if(H[i]>0) count++;
    
    printf("number of faults: %d\n", count-1);

  }
#endif


  //add vectors
  /*
    The best would be to stock the expandedRadius with
      modified = addVectorsToMLut(mlut, H, expandedRadius, c, is3D, ps, rs);
    but it would be difficult to keep track of the order of appearence of the vectors.
  */
  modified = addVectorsToMLut(mlut, mglut, H, radius, c, is3D, ps, rs);

#ifdef _VERBOSE
  if (modified) lprint_MLut(*mlut);
#endif

  //free
  freeimage(hma);

  return modified;
}

/* =============================================================== */
void mhlut_print_usage(char **argv)
/* =============================================================== */
{
#undef F_NAME
#define F_NAME "mhlut"
  fprintf(stderr, "\nusage: %s rmax dims [output]\n", F_NAME);
  fprintf(stderr, "\nARGUMENTS:\n");
  fprintf(stderr, "\n\trmax:\tthe desired maximal radius or 0 for all known radii;\n");
  fprintf(stderr, "\t\t%s uses the RT Lut Column and that Lut indicates the maximal known radius\n", F_NAME);
  fprintf(stderr, "\n\tdims:\t2 (or even) for 2D\n\t\t3 (or odd) for 3D\n");
  fprintf(stderr, "\n\toutput:\twhere the table will be written\n");
}

/* =============================================================== */
int32_t main(int32_t argc, char **argv) 
/* =============================================================== */
{

  //images
  struct xvimage *ball, *white;
  struct xvimage *sqedt, *result;
  u_int8_t *B, *W;
  u_int32_t *D, *R;

  //arguments
  int32_t Rmax, center;
  Point c;
  int32_t is3D;

  //dimensions and coordinates
  int32_t radius;
  int32_t rs, cs, ds, ps, N;
  int32_t x,y,z;

  //tables
  SQDLut SQDn;
  MLut mlut, mglut;
  RTLutCol Lut;

  //output
  char filename[512];
  char outfile[512];
  FILE *fd;

#ifdef VERBOSE
  int32_t oldrad = 0;
#endif

  //read arguments
  if ((argc < 3) || (argc > 4)) {
    mhlut_print_usage(argv);
    exit(1);
  }
  Rmax = atol(argv[1]);
  is3D = atoi(argv[2]) % 2;

  if (argc == 4) {
      sprintf(outfile, argv[argc-1]);
  } else {
      sprintf(outfile, "%s/src/tables/%s", getenv("PINK"), TAB_MHLUT(ds));
  }

  fd = fopen(outfile, "r");
  if (fd != NULL) 
  {
      char c = 'c';
      while ((c != 'y') && (c != 'n') && (c != 'Y') && (c != 'N')) {
	  printf("\n\nOverride file %s? [y|n]: ", outfile);
	  c = getchar();
      }
      if ((c == 'n') || (c == 'N')) {
	  printf("\n\nAborted!\n\n");
	  exit(0);
      }
      fclose(fd);
  }



  //Read tables
  if (! lread_SQD_RT_Luts(&SQDn, &mglut, &Lut, Rmax, 1+is3D)) {
    fprintf(stderr, "%s: lread_SQD_RT_Luts failed. This is fatal error.\n", F_NAME);
    exit(1);
  }
  if (Rmax == 0) Rmax = mglut.rknown;


  //image center
  center = (int32_t)(ceil(sqrt((double)Rmax)));
  center = (center / 2) * 2 + 1; //center in E3
  c.x = c.y = c.z = center;

  //calculate image dimensions
  rs = cs = 2 + 2*center; //to allow center in E_{(j+k) mod n} in nD, k in Nset
  if (is3D) {
    ds = rs; ps = rs*cs; N = ds*ps;
  } else {
    ds = 1; ps = 0; N = rs*cs;
  }

  //alloc images
  sqedt = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  ball = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  white = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if ((!sqedt) || (!white) || (!ball)) {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  D = ULONGDATA(sqedt);
  B = UCHARDATA(ball);

  //Read tables
  if (! lread_SQD_RT_Luts(&SQDn, &mglut, &Lut, Rmax, ds)) {
    fprintf(stderr, "%s: lread_SQD_RT_Luts failed. This is fatal error.\n", F_NAME);
    exit(1);
  }
  if (Rmax == 0) Rmax = mglut.rknown;

  //create mlut; mlut.rknown is initialized to the greatest known radius from RTLut
  if (!lcopy_MLut(&mlut, &mglut)) {
    fprintf(stderr, "%s: lcopy_MLut failed\n", argv[0]);
    exit(1);
  }
  mlut.numd = 0;


  //create distance map from white image and center
  memset(B, 255, N);
#ifdef __DEBUG
  writeimage(ball, "tmp/_white.pgm");
#endif
  createEdt(sqedt, ball, c, ps, rs);

  //for all radii
  for (radius = 4; radius < Rmax; radius++) {
    int32_t ei;

#ifdef VERBOSE
    if (radius >= 1000) {
      if (radius >= 5000) {
	if (radius >= 8000) {
	  if (radius >= 12000) {
	    if (radius >= 20000) {
	    } else { //12000 <= radius < 20000
	      if (radius - oldrad >= 50) {
		printf("\nTreating radius =%d\n", radius);
		oldrad = radius;
	      }
	    }
	  } else { //8000 <= radius < 12000
	    if (radius - oldrad >= 200) {
	      printf("\nTreating radius =%d\n", radius);
	      oldrad = radius;
	    }
	  }
	} else { //5000 <= radius < 8000
	  if (radius - oldrad >= 500) {
	    printf("\nTreating radius =%d\n", radius);
	    oldrad = radius;
	  }
	}
      } else { //1000 <= radius < 5000
	if (radius - oldrad >= 1000) {
	  printf("\nTreating radius =%d\n", radius);
	  oldrad = radius;
	}
      }
    }
#endif

    //for each i in Ei
    for (ei = 0; ei <= 2+is3D; ei++) {
      if (updateMLut(radius, ei, c,
		     D, ball,
		     &mlut, &mglut, Lut, SQDn,
		     rs, cs, ds, ps, N, is3D)) {
	fflush(stdout);
	radius--; //redo for the same radius with the new vector added
	break;
      }
    }
    //getchar();
  }

  //output
  mlut.rknown = Rmax; //finally
  lprint_MLut(mlut);
  lfprint_MLut(mlut, outfile);

  //final free
  freeimage(ball);
  freeimage(sqedt);
  freeimage(white);
  lfree_MLut(&mlut);
  lfree_MLut(&mglut);

  return 0;
} /* main */

