/*
  from the article: 
  E. Rémy, E. Thiel: "Exact Medial Axis With Euclidean Distance"

  Note: the Columns are adjusted to put the right DT in face of the correct 
  row of the table

  Written by: Rita Zrour, Michel Couprie 20/08/2004
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x, y, z, w, RR;
} Weighting;

#define MAXWEIGHTING 400 // max. number of vg (neighbors) that must be entered

typedef struct {
  Weighting vg[MAXWEIGHTING];
  int ng;
} MaskG;

//#define DEBUG

/* utility */
static int min(int x, int y) { if (x<y) return x; else return y; }

/* Function prototypes */
void ComputeCTg (int L, long int * CTg);
int AddWeighting (MaskG *M, int x, int y, int z, int w, int RR);
void ComputeLutCol (long int * CTg, int L, Weighting *vg, int ivg, int Rmaxx, long int * Lutnew, long int * CTgNew);
void CompLutMask (int L, MaskG * MgL, long int * LutColumn, int Rknown, int Rtarget);
int IsMAg (int x, int y, int z, MaskG *MgL, long int * LutColumn, long int * DTg, int L, int radiusmax);
void PrintLut (FILE *f, MaskG *MgL, long int * Lut, int Rknown);
void PrintMgLut (FILE *f, MaskG *M, int radius);
void adjustCTg (int L, long int * CTgg, long int * CTgNew, int Rmaxx);
void CompSEDT3D(long int * D, int rs, int cs, int ds);

/* Main Functions */

/* =============================================================== */
void ComputeCTg (int L, long int * CTg)
/* =============================================================== */
/* Function that compute CTgs*/
{
  int x, y, z, L2 = L*L;
  
  for (x = 0; x < L; x++)
    for (y = 0; y <= x ; y++)
      for (z = 0; z <= y ; z++)
	CTg[ z*L2 + y*L + x ] = x*x + y*y + z*z;
} // ComputeCTg ()

/* =============================================================== */
int AddWeighting (MaskG *M, int x, int y, int z, int w, int RR)
/* =============================================================== */
/*
* Add a weigthing in the mask M.
* Input:  M the generator of the mask, (x,y,z,w) the weighting.
* Output: the number of the weighting in the mask.
*/
{
  int i = M->ng;

  if (i >= MAXWEIGHTING)
  {
    printf ("AddWeighting: weighting number limited to MAXWEIGHTING = %d\n",MAXWEIGHTING);
    return 0;
  }

  if (! (0 <= z && z <= y && y <= x && 0 < x && 0 < w))
  {
    printf ("AddWeighting: (x = %d, y = %d, z = %d, w = %d)\n", x, y, z, w);
    printf ("  does not verify 0 <= z <= y <= x, 0 < x, 0 < w\n");
    return 0;
  }
  M->vg[i].x = x;
  M->vg[i].y = y;
  M->vg[i].z = z;
  M->vg[i].w = w;
  M->vg[i].RR = RR;

  printf ("AddWeighting: (x = %d, y = %d, z = %d, w = %d) added with index = %d\n",x, y, z, w, i);

  M->ng++;
  return i;
}

/* =============================================================== */
void ComputeLutCol (long int * CTg, int L, Weighting *vg, int ivg, int Rmaxx, 
		    long int * Lutnew, long int * CTgNew)
/* =============================================================== */
/* Function that computes one Column of lut*/
{
  int x, y, z, r1, r2, r, ra, rb, indice, L2 = L*L;
  int count = 0; //this counter is used to put correct CTg in row of the column
  long int * Lut = (long int *)malloc(Rmaxx * sizeof(long int));

  //Initializes Lut[ivg] and lut column to 0
  for (r = 0; r < Rmaxx; r++) Lut[r] = 0;

  for (x = 0; x < L - vg->x; x++)
  {
    for (y = 0; y <= x; y++)
    {
      for (z = 0; z <= y; z++)
      {
	//Radius of the ball where p1 is located
	r1 = CTg[ z*L2 + y*L + x ] + 1 ;
	// Same for p2
	r2 = CTg[ (z+vg->z)*L2 + (y+vg->y)*L + x+vg->x ] + 1;
	if ((r1 < Rmaxx) && (r2 > Lut[r1])) Lut[r1] = r2;
      }
    }
  }
  rb = 0;
  for (ra = 0; ra < Rmaxx; ra++)
  {
    if (Lut[ra] != 0)
    {
      if (Lut[ra] > rb) rb = Lut[ra]; else Lut[ra] = rb;
    }
  }
  for (r = 0; r < Rmaxx; r++) Lutnew[ivg*Rmaxx + r] = 0;
  for (r = 0; r < Rmaxx; r++)
  {
    if (Lut[r]!=0)
    {
      count = count + 1;
      indice = CTgNew[count];
      Lutnew[ivg*Rmaxx + indice] = Lut[r];
    }
  }
  free(Lut);
} // ComputeLutCol()

/* =============================================================== */
int IsMAg (int x, int y, int z, MaskG *MgL, long int * Lut, long int * DTg, int L, int radiusmax)
/* =============================================================== */
/* This function requires DTg and LutColumn values; x et y are the coordinates of p */
{
  int xx, yy, zz, val, i, L2 = L*L;
  val = DTg[ z*L2 + y*L + x ];

  for (i = 0; i < MgL->ng ; i++)
  {
    xx = x-MgL->vg[i].x;
    yy = y-MgL->vg[i].y;
    zz = z-MgL->vg[i].z;
    if (0 <= zz && zz <= yy && yy <= xx)
    {
      if ( DTg[ zz*L2 + yy*L + xx ] >= Lut[i*radiusmax + val] ) 
	return 0;
    }
  }
  return 1;
} // IsMAg()

/* =============================================================== */
void CompLutMask (int L, MaskG * MgL, long int * LutColumn, int Rknown, int Rtarget)
/* =============================================================== */
/*
* Full Lut Computation Algorithm with determination of MgLut.
*
* Input:  CTg and DTg two images, L the side length,
*	  MgL the generator of the Lut mask, LutColumn the look-up
*         table, Rknown the last verified radius, Rtarget the maximum radius
*         to be verified and CTgNew used to put the values of the lut column 
*         in the correct place to be read using distance transform Image
* Output: MgL and LutColumn are filled with the correct values.
*
* To compute MgL and Lut from beginning to the maximum radius allowed with L:
*/
{
  int x, y, z, p0, p1, p2, p3, p4, p5, R, i, L2 = L*L;
  long int *CTg, *DTg, *CTgNew;

  CTg = (long int *)malloc(L * L * L * sizeof(long int));
  DTg = (long int *)malloc(L * L * L * sizeof(long int));
  CTgNew = (long int *)malloc(Rtarget * sizeof(long int));

  ComputeCTg (L, CTg);
  adjustCTg (L, CTg, CTgNew, Rtarget);

  for (R = Rknown+1; R < Rtarget; R++)
  {
    printf ("R = %d Rtarget = %d\n", R, Rtarget);
    for (x = 0; x < L; x++)
      for (y = 0; y <= x; y++)
	for (z = 0; z <= y; z++)
        {
	  p0 = z*L2 + y*L + x; 	p1 = z*L2 + x*L + y; 
	  p2 = y*L2 + z*L + x; 	p3 = y*L2 + x*L + z; 
	  p4 = x*L2 + z*L + y; 	p5 = x*L2 + y*L + z; 
	  if (CTg[p0] <= R) 
	    DTg[p0] = DTg[p1] = DTg[p2] = DTg[p3] = DTg[p4] = DTg[p5] = 1; 
	  else 
	    DTg[p0] = DTg[p1] = DTg[p2] = DTg[p3] = DTg[p4] = DTg[p5] = 0; 
	}

#ifdef DEBUG
 { int LL = ((int)sqrt((double)R) + 2);
    printf("avant CompSEDT\n");
    for (z = 0; z < LL; z++)
    {
      for (y = 0; y < LL; y++)
      {
	for (x = 0; x < LL; x++) printf("%6d", DTg[z*L2 + y*L + x]);
	printf("\n");
      }
      printf("\n");
    }
    printf("\n");
 }
#endif			

      // distance map
    CompSEDT3D (DTg, L, L, L);

#ifdef DEBUG
 { int LL = ((int)sqrt((double)R) + 2);
    printf("apres CompSEDT\n");
    for (z = 0; z < LL; z++)
    {
      for (y = 0; y < LL; y++)
      {
	for (x = 0; x < LL; x++) printf("%6d", DTg[z*L2 + y*L + x]);
	printf("\n");
      }
      printf("\n");
    }
    printf("\n");
 }
#endif			

    for (x = 1; x < L; x++)
      for (y = 0; y <= x; y++)
	for (z = 0; z <= y; z++)
	{
	  p0 = z*L2 + y*L + x;
	  if ( DTg[p0] > 0 && IsMAg (x, y, z, MgL, LutColumn, DTg, L, Rtarget) )
	  {
	    printf ("(%2d, %2d, %2d, %3d) added for R = %d\n", x, y, z, CTg[p0], R);

	    /* Add a new weighting to MgL */
	    i = AddWeighting (MgL, x, y, z, CTg[p0], R);
	    
	    /* New column in Lut */
	    ComputeLutCol (CTg, L, MgL->vg+i, i, Rtarget, LutColumn, CTgNew);

	    if (IsMAg(x, y, z, MgL, LutColumn, DTg, L,Rtarget))
	    {
	      printf ("\nComputeAndVerifyLut: ERROR for R = %d\n", R);
	      return;
	    }
	  }
	}
  }
  free(CTg);
  free(CTgNew);
  free(DTg);
} // CompLutMask()

/* ======================================================== */
/* ======================================================== */
// Functions for the exact Squared Euclidean Distance Transform (Meijster et al.)
// Linear algorithm
/* ======================================================== */
/* ======================================================== */

#define F_2_2d(y,yp,f,i) (f[y*rs+i]+(yp-y)*(yp-y))
#define Sep_2_2d(v,u,f,i) (((u*u)-(v*v)+f[u*rs+i]-f[v*rs+i])/(2*(u-v)))

#define F_2_3d(y,yp,f,i,k) (f[k*ps+y*rs+i]+(yp-y)*(yp-y))
#define F_3_3d(z,zp,f,i,j) (f[z*ps+j*rs+i]+(zp-z)*(zp-z))
#define Sep_2_3d(v,u,f,i,k) (((u*u)-(v*v)+f[k*ps+u*rs+i]-f[k*ps+v*rs+i])/(2*(u-v)))
#define Sep_3_3d(v,u,f,i,j) (((u*u)-(v*v)+f[u*ps+j*rs+i]-f[v*ps+j*rs+i])/(2*(u-v)))

/* ======================================================== */
static void SEDT3d_line(long int *f, long int *g, int rs, int cs, int ds)
/* ======================================================== */
{
  int i, j, k, ps = rs*cs;
  for (k = 0; k < ds; k++)
    for (j = 0; j < cs; j++)
    {
      if (f[0 + rs*j + ps*k] == 0)
	g[0 + rs*j + ps*k] = 0; 
      else 
	g[0 + rs*j + ps*k] = rs*cs*ds; // infinity
      for (i = 1; i < rs; i++)
	if (f[i + rs*j + ps*k] == 0) g[i + rs*j + ps*k] = 0; 
	else g[i + rs*j + ps*k] = 1 + g[i-1 + rs*j + ps*k]; 
      for (i = rs-2; i >= 0; i--)
	if (g[i+1 + rs*j + ps*k] < g[i + rs*j + ps*k])
	  g[i + rs*j + ps*k] = 1 + g[i+1 + rs*j + ps*k];
      for (i = 0; i < rs; i++)
	g[i + rs*j + ps*k] = g[i + rs*j + ps*k] * g[i + rs*j + ps*k];
    }
} //  SEDT3d_line()

/* ======================================================== */
static void SEDT3d_column(long int *f, long int *g, int rs, int cs, int ds)
/* ======================================================== */
{
  int i, k, u, q, ps = rs*cs;
  long int w;
  long int *s, *t;
  s = (long int *)malloc(cs * sizeof(long int));
  t = (long int *)malloc(cs * sizeof(long int));

  for (k = 0; k < ds; k++)
    for (i = 0; i < rs; i++)
    {
      q = 0; s[0] = 0; t[0] = 0;
      for (u = 1; u < cs; u++)
      {
	while ( (q >= 0) && (F_2_3d(s[q],t[q],f,i,k) > F_2_3d(u,t[q],f,i,k)) ) q--;
	if (q < 0)
        {
	  q = 0;
	  s[0] = u;
	}
	else
        {
	  w = 1 + Sep_2_3d(s[q],u,f,i,k);
	  if (w < cs)
	  {
	    q++; s[q] = u; t[q] = w;
	  }
	}
      } 
      for (u = cs-1; u >= 0; u--)
      {
	g[ps*k + rs*u + i] = F_2_3d(s[q],u,f,i,k);
	if (u == t[q]) q--;
      }
    }
  free(s); free(t);
} //  SEDT3d_column()

/* ======================================================== */
static void SEDT3d_planes(long int *f, long int *g, int rs, int cs, int ds)
/* ======================================================== */
{
  int i, j, u, q, ps = rs*cs;
  long int w;
  long int *s, *t;
  s = (long int *)malloc(ds * sizeof(long int));
  t = (long int *)malloc(ds * sizeof(long int));

  for (j = 0; j < cs; j++)
    for (i = 0; i < rs; i++)
    {
      q = 0; s[0] = 0; t[0] = 0;
      for (u = 1; u < ds; u++)
      {
	while ( (q >= 0) && (F_3_3d(s[q],t[q],f,i,j) > F_3_3d(u,t[q],f,i,j)) ) q--;
	if (q < 0)
        {
	  q = 0;
	  s[0] = u;
	}
	else
        {
	  w = 1 + Sep_3_3d(s[q],u,f,i,j);
	  if (w < ds)
	  {
	    q++; s[q] = u; t[q] = w;
	  }
	}
      } 
      for (u = ds-1; u >= 0; u--)
      {
	g[ps*u + rs*j + i] = F_3_3d(s[q],u,f,i,j);
	if (u == t[q]) q--;
      }
    }
  free(s); free(t);
} //  SEDT3d_planes()

/* =============================================================== */
void CompSEDT3D(long int * D, int rs, int cs, int ds)
/* =============================================================== */
/* Function for the distance map */
{
  int i;
  static int N = 0;
  //static long int *F=NULL;
  static long int *T=NULL;
  
  if (N != rs*cs*ds)
  {
	N=rs*cs*ds;
	//if(F) free(F);
	if(T) free(T);
	if(N > 0)
	{
		//F = (long int *)malloc(N * sizeof(long int));
   		T = (long int *)malloc(N * sizeof(long int));
	}
  }
  
  if(N>0)
  { 
  	//memcpy(F, D, N * sizeof(long int));
	SEDT3d_line(D, D, rs, cs, ds);
  	SEDT3d_column(D, T, rs, cs, ds);
  	SEDT3d_planes(T, D, rs, cs, ds);
  }
} // CompSEDT3D()

/* =============================================================== */
void PrintMgLut (FILE *f, MaskG *M,int radius)
/* =============================================================== */
/* Print a mask M to a file.
   Input:  f a file, M the generator of a mask.
   Output: to file f
*/
{
  int i;
  fprintf (f,"%d\n\n",M->ng);
  for (i = 0; i < M->ng; i++)
    fprintf (f, "%d %d %d %d\n",M->vg[i].x, M->vg[i].y, M->vg[i].z, M->vg[i].RR);
  fprintf (f, "\n");
} // PrintMgLut()

/* =============================================================== */
void PrintLut (FILE *f, MaskG *MgL, long int * Lut, int Rknown)
/* =============================================================== */
/* Print the LUT to a file */
{
  int i,ra;
  fprintf (f, "%d\n\n", Rknown);
  for (i = 0; i < MgL->ng; i++)
  {
    for (ra = 0; ra < Rknown; ra++) fprintf(f,"%d\n", Lut[i*Rknown + ra]);  
    fprintf (f, "\n");
  }
} // PrintLut()

/* =============================================================== */
void adjustCTg (int L, long int * CTgg, long int * CTgNew, int Rmaxx)
/* =============================================================== */
/* Function that puts CTgs values in an increasing order letting each CTg 
   appear just once 
*/
{
  int x, y, z, i, L2 = L*L;
  int count = -1;

  for (i = 0; i <= Rmaxx; i++)
  {
    for (x = 0; x < L; x++)
    {
      for (y = 0; y <= x; y++)
      {
	for (z = 0; z <= y; z++)
        {
	  if (CTgg[ z*L2 + y*L + x ] == i )
	  {
	    count = count + 1;
	    CTgNew[count] = i;
	    goto stoploops;
	  }
	}
      }
    }
    stoploops: ;
  }
} // adjustCTg()

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  FILE *fd;
  int L1; // here L1 is the length of the image
  int Rknown1;
  int Rtarget1;
  MaskG MgL1;
  long int *LutColumn1;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s rmax filename \n", argv[0]);
    exit(1);
  }

  Rtarget1 = atoi(argv[1]);
  L1 = 2 * ((int)sqrt((double)Rtarget1) + 1);

  LutColumn1 = (long int *)malloc(Rtarget1 * MAXWEIGHTING * sizeof(long int));

  MgL1.ng = 0;
  Rknown1 = 0;
  CompLutMask (L1, &MgL1, LutColumn1, Rknown1, Rtarget1);
  CompSEDT3D(NULL, 0, 0, 0); //free memory
  Rknown1 = Rtarget1;

  fd = fopen (argv[argc-1], "w");
  if (fd == NULL)
  {
    fprintf (stderr, "%s : Error while opening file %s\n", argv[0], argv[argc-1]);
    exit(0);
  }

  PrintMgLut (fd, &MgL1, Rknown1);
  PrintLut (fd, &MgL1, LutColumn1, Rknown1);
  fclose (fd);
  free(LutColumn1);
  return 0;
} // main()
