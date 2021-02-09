/*
Table for the 3D bisector function 

format : 

Table 1:
NVAL 
TabIndDec[NVAL+1] 

Table 2:
NPOINTS 
ListDecs[NPOINTS]

For each i between 0 and NVAL-1, 
TabNbDec[i] gives the index of the first decomposition (if any) 
of the integer i into a sum of two squares.

For each i between 0 and NVAL-1, 
TabNbDec[i+1] - TabNbDec[i] gives the number of different 
decompositions of the integer i into a sum of three squares.

For each p between 0 and NPOINTS-1, 
ListDecs[NPOINTS] is a triple of integers x, y, z which is 
a decomposition of an integer into a sum of three squares.

Michel Couprie - juillet 2004
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VERBOSE

typedef struct {
  int x, y, z;
} Coordinates;

/* =============================================================== */
void fprint_table1(char *filename, int *TabIndDec, int nval)
/* =============================================================== */
{
  int i, j, k, n;
  FILE *fd;

  fd = fopen(filename, "w");

  fprintf(fd, "%d\n\n", nval); 
  for (i = 0; i <= nval; i++) fprintf(fd, "%d\n", TabIndDec[i]);  

  fclose(fd);
} // fprint_table1()

/* =============================================================== */
void fprint_table2(char *filename, Coordinates *ListDecs, int npoints)
/* =============================================================== */
{
  int i, j, k, n;
  FILE *fd;

  fd = fopen(filename, "w");

  fprintf(fd, "\n%d\n\n", npoints); 
  for (i = 0; i < npoints; i++) fprintf(fd, "%d %d %d\n", 
					ListDecs[i].x, ListDecs[i].y, ListDecs[i].z);  

  fclose(fd);
} // fprint_table2()

void print_table(int *TabIndDec, int nval, Coordinates *ListDecs, int npoints)
{
  int i, j, k, n;
  for (i = 0; i < nval; i++) 
  {
    j = TabIndDec[i];
    n = TabIndDec[i+1] - TabIndDec[i];
    printf("NbDec[%d] = %d  ", i, n);  
    for (k = 0; k < n; k++) 
      printf("%d(%d,%d,%d) ", j+k, ListDecs[j+k].x, ListDecs[j+k].y, ListDecs[j+k].z);  
    printf("\n"); 
  } 
} // fprint_table()

/* =============================================================== */
void print_table2(int *TabIndDec, int nval, Coordinates *ListDecs, int npoints)
/* =============================================================== */
{
  int i, j, k, n;
  for (i = 0; i < nval; i++) 
  {
    j = TabIndDec[i];
    n = TabIndDec[i+1] - TabIndDec[i];
    if (n > 0)
    {
      printf("%d  ", i);  
      for (k = 0; k < n; k++) 
	printf("(%d,%d,%d), ", ListDecs[j+k].x, ListDecs[j+k].y, ListDecs[j+k].z);  
      printf("\n"); 
    }
  } 
} // print_table2()

/* =============================================================== */
void print_ndec(int *TabIndDec, int nval)
/* =============================================================== */
{
  int i, j, k, n;
  j = k = 0;
  for (i = 0; i <= nval; i++) 
  {
    n = TabIndDec[i+1] - TabIndDec[i];
    if (n != 0) { k += n; j++; }
    if (i % 10000 == 0) printf("i = %d ; j = %d ; k = %d ; ratio = %g\n", 
			      i, j, k, (double)k/(double)j);  
  } 
} // print_ndec()

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int i, j, k, x, y, z;
  int *TabIndDec;
  int *TabInd;
  int NPOINTS, NVAL, L1;
  Coordinates* ListDecs;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s nval filename1 filename2 \n", argv[0]);
    exit(1);
  }

  NVAL = atoi(argv[1]);

  TabIndDec = (int *)malloc((NVAL+1) * sizeof(int));
  TabInd = (int *)malloc((NVAL+1) * sizeof(int));

  for (i = 0; i <= NVAL; i++) TabIndDec[i] = 0;
  for (i = 0; i <= NVAL; i++) TabInd[i] = 0;

  L1 = (int)(sqrt(NVAL)) + 1;
  for (x = 0; x <= L1; x++)
  {
    for (y = 0; y <= x; y++)
    {
      for (z = 0; z <= y; z++)
      {
	i = x*x + y*y + z*z;
	if (i <= NVAL) TabIndDec[i] += 1;
      }
    }
  }

  k = TabIndDec[0]; TabIndDec[0] = 0;
  for (i = 1; i <= NVAL; i++) { j = TabIndDec[i]; TabIndDec[i] = k; k += j; } 
  NPOINTS = k; 

  ListDecs = (Coordinates *)malloc(NPOINTS * sizeof(Coordinates));

  for (x = 0; x <= L1; x++)
  {
    for (y = 0; y <= x; y++)
    {
      for (z = 0; z <= y; z++)
      {
	i = x*x + y*y + z*z;
	if (i <= NVAL) 
	{
	  j = TabIndDec[i] + TabInd[i];
	  ListDecs[j].x = x;
	  ListDecs[j].y = y;
	  ListDecs[j].z = z;
	  TabInd[i] += 1;
	}
      }
    }
  }


  fprint_table1(argv[argc-2], TabIndDec, NVAL);
  fprint_table2(argv[argc-1], ListDecs, NPOINTS);
#ifdef VERBOSE
  print_table2(TabIndDec, NVAL, ListDecs, NPOINTS);
#endif
  //  print_ndec(TabIndDec, NVAL);

  return 0;
} // main()
