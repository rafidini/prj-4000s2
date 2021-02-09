/*
  from the article: 
  E. Rémy, E. Thiel: "Exact Medial Axis With Euclidean Distance"

  Algoritm to compute the table and store it in the file table.txt

  parameter to adjust the table MAXLUTENTRY, Rtarget1 and L1

  Note: the Columns are adjusted to put the right DT in face of the correct 
  row of the table

  Written by: Rita Zrour
  created: 18/04/2004
  last modified: 29/04/2004
  revision by Michel Couprie: 10/08/2004
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x, y, w, RR;
} Weighting;

#define MAXWEIGHTING 200 // max. number of vg (neighbors) that must be entered

typedef struct {
  Weighting vg[MAXWEIGHTING];
  int ng;
} MaskG;

//#define DEBUG

/* utility */
static int min(int x, int y) { if (x<y) return x; else return y; }

/* Function prototypes */
void ComputeCTg (int L, long int * CTg);
int AddWeighting (MaskG *M, int x, int y, int w, int RR);
void ComputeLutCol (long int * CTg, int L, Weighting *vg, int ivg, int Rmaxx, long int * Lutnew, long int * CTgNew, long int * Lut);
void CompLutMask (int L, MaskG * MgL, long int * LutColumn, int Rknown, int Rtarget);
int IsMAg (int x, int y, MaskG *MgL, long int * LutColumn, long int * DTg, int L, int radiusmax);
void PrintLut (FILE *f, MaskG *MgL, long int * Lut, int Rknown);
void PrintMgLut (FILE *f, MaskG *M, int radius);
void adjustCTg (int L, long int * CTgg, long int * CTgNew, int Rmaxx);
void CompSEDT(long int * D, int rs, int cs);

/* Main Functions */

/* =============================================================== */
void ComputeCTg (int L, long int * CTg)
/* =============================================================== */
/* Function that compute CTgs*/
{
  int x, y;
  
  for (x = 0; x < L; x++)
    for (y = 0; y <= x ; y++)
      CTg[ y*L + x ] = x*x + y*y;
} // ComputeCTg ()


/* =============================================================== */
int AddWeighting (MaskG *M, int x, int y, int w,int RR)
/* =============================================================== */
/*
* Add a weigthing in the mask M.
* Input:  M the generator of the mask, (x,y,w) the weighting.
* Output: the number of the weighting in the mask.
*/
{
  int i = M->ng;

  if (i >= MAXWEIGHTING)
  {
    printf ("AddWeighting: weighting number limited to MAXWEIGHTING = %d\n",MAXWEIGHTING);
    return 0;
  }

  if (! (0 <= y && y <= x && 0 < x && 0 < w))
  {
    printf ("AddWeighting: (x = %d, y = %d, w = %d)\n",x, y, w);
    printf ("  does not verify 0 <= y <= x, 0 < x, 0 < w\n");
    return 0;
  }
  M->vg[i].x = x;
  M->vg[i].y = y;
  M->vg[i].w = w;
  M->vg[i].RR = RR;

  printf ("AddWeighting: (x = %d, y = %d, w = %d) added with index = %d\n",x, y, w, i);

  M->ng++;
  return i;
}

/* =============================================================== */
void ComputeLutCol (long int * CTg, int L, Weighting *vg, int ivg, int Rmaxx, 
		    long int * Lutnew, long int * CTgNew, long int * Lut)
/* =============================================================== */
/* 
   Function that computes one Column of lut
   Warning: the table "Lut" must have been allocated with a size at least equal to Rmaxx
*/

{
  int x, y, r1, r2, r, ra, rb, indice;
  int count=0; //this counter is used to put correct CTg in row of the column

  //Initializes Lut[ivg] and lut column to 0
  for (r = 0; r < Rmaxx; r++) Lut[r] = 0;

  for (x = 0; x < L - vg->x; x++)
  {
    for (y = 0; y <= x; y++)
    {
      //Radius of the ball where p1 is located
      r1 = CTg[ y*L + x ]+1 ;
      // Same for p2
      r2 = CTg[ (y+vg->y)*L + x+vg->x ] + 1;
      if ((r1 < Rmaxx) && (r2 > Lut[r1])) Lut[r1] = r2;
    }
  }
  rb = 0;
  for (ra = 0; ra < Rmaxx; ra++)
  {
    if (Lut[ra]!=0)
    {
      if (Lut[ra] > rb) rb = Lut[ra]; else Lut[ra] = rb;
    }
  }
  for (r = 0; r < Rmaxx; r++) Lutnew[ivg*Rmaxx+r] = 0;
  for (r = 0; r < Rmaxx; r++)
  {
    if (Lut[r] != 0)
    {
      count = count + 1;
      indice = CTgNew[count];
      Lutnew[ivg*Rmaxx + indice] = Lut[r];
    }
  }
} // ComputeLutCol()

/* =============================================================== */
int IsMAg (int x, int y, MaskG *MgL, long int * Lut, long int * DTg, int L, int radiusmax)
/* =============================================================== */
/* This function requires DTg and LutColumn values; x et y are the coordinates of p */
{
  int xx, yy, val, i;
  val = DTg[ y*L + x ];

  for (i = 0; i < MgL->ng ; i++)
  {
    xx = x-MgL->vg[i].x;
    yy = y-MgL->vg[i].y;
    if (0 <= yy && yy <= xx)
    {
      if ( DTg[ yy*L + xx ] >= Lut[i*radiusmax + val] ) 
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
  int x, y, p, pp, R, i;
  long int *CTg, *DTg, *CTgNew, *Lut;

  CTg = (long int *)malloc(L * L * sizeof(long int));
  DTg = (long int *)malloc(L * L * sizeof(long int));
  CTgNew = (long int *)malloc(Rtarget * sizeof(long int));
  Lut = (long int *)malloc(Rtarget * sizeof(long int));

  if ((CTg == NULL) || (DTg == NULL) || (CTgNew == NULL))
  {
    printf ("CompLutMask: malloc failed\n");
    exit(0);
  }

printf ("CompLutMask running\n");
  ComputeCTg (L, CTg);
printf ("CompLutMask: ComputeCTg done\n");
  adjustCTg (L, CTg, CTgNew, Rtarget);
printf ("CompLutMask: adjustCTg done\n");

#ifdef DEBUG
    printf("CTg\n");
    for (y = 0; y < L; y++)
      {
	for (x = 0; x < L; x++) printf("%6d", CTg[y*L+x]);
	printf("\n");
      }
    printf("\n");
#endif			

  for (R = Rknown+1; R < Rtarget; R++)
  {
if (R%100 == 0) printf ("R = %d\n", R);
    for (x = 0; x < L; x++)
      for (y = 0; y <= x; y++)
      {
	p = y*L + x; pp = x*L + y;
	if (CTg[p] <= R) DTg[p] = DTg[pp] = 1; else DTg[p] = DTg[pp] = 0;
      }

#ifdef DEBUG
    printf("avant CopmSEDT\n");
    for (y = 0; y < L; y++)
      {
	for (x = 0; x < L; x++) printf("%6d", DTg[y*L+x]);
	printf("\n");
      }
    printf("\n");
#endif			

      // distance map
      CompSEDT (DTg,L,L);

#ifdef DEBUG
    printf("apres CopmSEDT\n");
    for (y = 0; y < L; y++)
      {
	for (x = 0; x < L; x++) printf("%6d", DTg[y*L+x]);
	printf("\n");
      }
    printf("\n");
#endif			

      for (x = 1; x < L; x++)
        for (y = 0; y <= x; y++)
	{
	  p = y*L + x;
	  if ( DTg[p] > 0 && IsMAg (x, y, MgL, LutColumn, DTg, L,Rtarget) )
	  {
	    printf ("(%2d, %2d, %3d) added for R = %d\n",x, y, CTg[p], R);

	    /* Add a new weighting to MgL */
	    i = AddWeighting (MgL, x, y, CTg[p], R);
	    
	    /* New column in Lut */
	    ComputeLutCol (CTg, L, MgL->vg+i, i, Rtarget, LutColumn, CTgNew, Lut);

	    if (IsMAg(x, y, MgL, LutColumn, DTg, L,Rtarget))
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
  free(Lut);
} // CompLutMask()

/* ======================================================== */
/* ======================================================== */
// Functions for the exact Squared Euclidean Distance Transform (Meijster et al.)
// Linear algorithm
/* ======================================================== */
/* ======================================================== */

#define F_2_2d(y,yp,f,i) (f[y*rs+i]+(yp-y)*(yp-y))
#define Sep_2_2d(v,u,f,i) (((u*u)-(v*v)+f[u*rs+i]-f[v*rs+i])/(2*(u-v)))

/* ======================================================== */
static void SEDT_line(unsigned char *f, long int *g, int rs, int cs)
/* ======================================================== */
{
  int i, j;
  for (j = 0; j < cs; j++)
  {
    if (f[0 + rs*j] == 0) g[0 + rs*j] = 0; else g[0 + rs*j] = rs*cs; // infinity
    for (i = 1; i < rs; i++)
      if (f[i + rs*j] == 0) g[i + rs*j] = 0; 
      else g[i + rs*j] = 1 + g[i-1 + rs*j]; 
    for (i = rs-2; i >= 0; i--)
      if (g[i+1 + rs*j] < g[i + rs*j]) g[i + rs*j] = 1 + g[i+1 + rs*j];
    for (i = 0; i < rs; i++) g[i + rs*j] = g[i + rs*j] * g[i + rs*j];
  }
} //  SEDT_line()

/* ======================================================== */
static void SEDT_column(long int *f, long int *g, int rs, int cs)
/* ======================================================== */
{
  int i, u, q;
  long int w;
  long int *s, *t;
  s = (long int *)malloc(cs * sizeof(long int));
  t = (long int *)malloc(cs * sizeof(long int));

  for (i = 0; i < rs; i++)
  {
    q = 0; s[0] = 0; t[0] = 0;
    for (u = 1; u < cs; u++)
    {
      while ( (q >= 0) && (F_2_2d(s[q],t[q],f,i) > F_2_2d(u,t[q],f,i)) ) q--;
      if (q < 0)
      {
        q = 0;
        s[0] = u;
      }
      else
      {
        w = 1 + Sep_2_2d(s[q],u,f,i);
        if (w < cs)
        {
          q++; s[q] = u; t[q] = w;
        }
      }
    } 
    for (u = cs-1; u >= 0; u--)
    {
      g[rs*u + i] = F_2_2d(s[q],u,f,i);
      if (u == t[q]) q--;
    }
  }
  free(s); free(t);
} //  SEDT_column()

/* =============================================================== */
void CompSEDT(long int * D, int rs, int cs)
/* =============================================================== */
/* Function for the distance map */
{
  int i, N = rs * cs;
  unsigned char *F = (unsigned char *)malloc(N * sizeof(char));
  long int *T = (long int *)malloc(N * sizeof(long int));
  for (i = 0; i < N; i++) F[i] = (unsigned char)D[i];
  SEDT_line(F, T, rs, cs);
  SEDT_column(T, D, rs, cs);
} // CompSEDT()

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
    fprintf (f, "%d %d %d\n",M->vg[i].x, M->vg[i].y, M->vg[i].RR);
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
void adjustCTg (int LL, long int * CTgg,long int * CTgNew,int Rmaxx)
/* =============================================================== */
/* Function that puts CTgs values in an increasing order letting each CTg 
   appear just once 
*/
{
  int x, y, i;
  int count = -1;

  for (i = 0; i <= Rmaxx; i++)
  {
if (i%100 == 0) printf ("i = %d\n", i);
    for (x = 0; x < LL; x++)
    {
      for (y = 0; y <= x; y++)
      {
	if (CTgg[ y*LL + x ] == i )
	{
	  count = count + 1;
	  CTgNew[count] = i;
	  goto stoploops;
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
  long int * LutColumn1;

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
