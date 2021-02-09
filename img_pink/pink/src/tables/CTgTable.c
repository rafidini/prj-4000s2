/*
Table for the bisector function 

Author: Rita Zrour (2004)
Revised: Michel Couprie (august 2004)
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef int * TabCTg;

/* =============================================================== */
void ComputeCTg (int L, TabCTg CTg)
/* =============================================================== */
{
  int x1,x2;
  
  for (x1 = 0; x1 < L; x1++)
    for (x2 = 0; x2 <= x1 ; x2++)
      CTg[ x2*L + x1 ] = x1 * x1 + x2 * x2;
} // ComputeCTg ()


/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int x, y, L1;
  FILE *fd;
  int Rmax, a;
  int count = 0;
  TabCTg CTg1;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s rmax filename \n", argv[0]);
    exit(1);
  }

  Rmax = atoi(argv[1]);
  L1 = (int)sqrt((double)Rmax);

  CTg1 = (int *)malloc(L1*L1, sizeof(int));

  ComputeCTg(L1, CTg1);

  fd = fopen (argv[argc-1], "w");
  if (fd == NULL)
  {
    fprintf (stderr, "%s : Error while opening file %s\n", argv[0], argv[argc-1]);
    exit(0);
  }

  for (x = 0; x < L1; x++) // counts the number of 
  {
    for (y = 0; y <= x; y++)
      if (CTg1[x + y*L1] <= Rmax)
      {
	count++;
	a = CTg1[x + y*L1]; //+1
	fprintf(fd,"%d\t%d\t%d\t\n",a,y,x);
      }
  }

  fprintf (fd,"27714\n\n");

  for (x = 0; x < L1; x++)
  {
    for (y = 0; y <= x; y++)
      if (CTg1[x + y*L1] <= Rmax)
      {
	count++;
	a = CTg1[x + y*L1]; //+1
	fprintf(fd,"%d\t%d\t%d\t\n",a,y,x);
      }
  }

  fclose (fd);
  free(CTg1);

  printf("nb of CTg printed=%d\n",count);//this number must be put in the begining of the file BigTableCTg
  return 0;
} // main()
