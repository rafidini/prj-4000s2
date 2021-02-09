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
/*! \file curvetangents.c

\brief computes the tangents of a digital curve

<B>Usage:</B> curvetangents curve.list [mode masksize] tangents.list

<B>Description:</B>

Computes the tangents of a curve which is specified by the ordered list of its points. The output file is a list of normalized tangent vectors, all with origin 0.

<B>Types supported:</B> curve 2D, curve 3D

<B>Category:</B> geo
\ingroup geo

\author Michel Couprie
\author Paulin Sanselme
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ltangents.h>

//#define DEBUG
//#define DEBUG_2

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t i,j;
  FILE *fd = NULL;
  int32_t npoints, mode;
  char type;

  if ((argc != 3) && (argc != 5))
  {
    fprintf(stderr, "usage: %s curve.list [mode [masksize]] out.list \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  if (argc == 3) mode = 1;
  else mode = atoi(argv[2]);
  
  fscanf(fd, "%c", &type);
  if (type == 'b')
  {

    // TODO : call lcurvetangents2D instead (MC)

    int32_t *X, *Y;
    int32_t *end;
    double *angle;
    double *mstd;

    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);

    for (i = 0; i < npoints; i++)
      fscanf(fd, "%d %d", &(X[i]), &(Y[i]));
    fclose(fd);
  
    if (mode == 1)
    {

    end = (int32_t *)malloc(npoints * sizeof(int32_t)); assert(end != NULL);
    angle = (double *)malloc(npoints * sizeof(double)); assert(angle != NULL);
    mstd = (double *)malloc(npoints * sizeof(double)); assert(mstd != NULL);

    ExtractDSSs(npoints, X, Y, 1, end, angle);

#ifdef DEBUG
    printf("npoints = %d\n", npoints);
    for (i = 0; i < npoints; i++)
      if (end[i] != -1)
	printf("DSS %d-%d, angle %g (%g)\n", i, end[i], angle[i], angle[i]*180/M_PI);
#endif

    LambdaMSTD(npoints, end, angle, mstd);

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
      exit(1);
    }
    fprintf(fd, "b %d\n", npoints);
    for (i = 0; i < npoints; i++)
      fprintf(fd, "%g %g\n", cos(mstd[i]), sin(mstd[i]));
    fclose(fd);

    free(end);
    free(angle);
    free(mstd);
    }

    else if (mode == 2)
      {
	char tablefilename[512];
	int32_t n, nbis;
	int32_t decalage;
	uint64_t *tab_combi;
        char buf[25];
	double coef, normalisateur;
	double *dX, *dY;
	int32_t *deltaX, *deltaY;

	n = atoi(argv[3]);
	nbis = 2*n-1;

	dX = (double *)malloc(npoints * sizeof(double)); assert(dX != NULL);
	dY = (double *)malloc(npoints * sizeof(double)); assert(dY != NULL);
	deltaX = (int32_t *)malloc(npoints * sizeof(int32_t)); assert(deltaX != NULL);
	deltaY = (int32_t *)malloc(npoints * sizeof(int32_t)); assert(deltaY != NULL);
	tab_combi = (uint64_t*)malloc( (nbis+1)*sizeof( uint64_t ));

	deltaX[0] = 0;
	deltaY[0] = 0;
	for (i = 1; i < npoints; i++)
	  {
	    deltaX[i] = X[i]-X[i-1];
	    deltaY[i] = Y[i]-Y[i-1];
	  }

	sprintf(tablefilename, "%s/src/tables/TabTriPascal.txt", getenv("PINK"));
	fd = fopen (tablefilename, "r");
	if (fd == NULL) 
	  {
	    fprintf(stderr, "%s : table not found\n", argv[0]);
	    exit(1);
	  }

	decalage = 21*(nbis*(nbis+1))/2+nbis+3;
	fseek(fd, decalage, SEEK_SET);

	for (i = 0; i < nbis+1 ; i++)
	  {
 	    fread( buf, 21, 1, fd);
	    tab_combi[i] = atol(buf);
	  }


	coef = pow(2,-nbis);

	for (i = 0; i < npoints; i++)
	  {
	    dX[i] = 0;
	    dY[i] = 0;
	    for (j = mcmax(0,n-i); j < mcmin(nbis+1,npoints+n-i-1); j++)
	      {
		dX[i] += (int64_t)(tab_combi[j]*deltaX[i+j-n+1]);
		dY[i] += (int64_t)(tab_combi[j]*deltaY[i+j-n+1]);
	      }
	    dX[i] = dX[i]*coef;
	    dY[i] = dY[i]*coef;
	    normalisateur = pow(pow(dX[i],2)+pow(dY[i],2),0.5);
	    dX[i] = dX[i]/normalisateur;
	    dY[i] = dY[i]/normalisateur;    
	  }


	fd = fopen(argv[argc-1],"w");
	if (!fd)
	  {
	    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
	    exit(1);
	  }
	fprintf(fd, "b %d\n", npoints);
	for (i = 0; i < npoints; i++)
	  fprintf(fd, "%10g %10g\n", dX[i], dY[i]);
	fclose(fd);

	free(tab_combi);
	free(dX);
	free(dY);
	free(deltaX);
	free(deltaY);
      }

    free(X);
    free(Y);
  }
  else if (type == 'B')
  {
    int32_t *X, *Y, *Z;
    double *Xmstd, *Ymstd, *Zmstd;

    fscanf(fd, "%d", &npoints);
    X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
    Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);
    Z = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Z != NULL);
    for (i = 0; i < npoints; i++)
      fscanf(fd, "%d %d %d", &(X[i]), &(Y[i]), &(Z[i]));
    fclose(fd);

    if (mode ==1)
      {
    Xmstd = (double *)malloc(npoints * sizeof(double)); assert(Xmstd != NULL);
    Ymstd = (double *)malloc(npoints * sizeof(double)); assert(Ymstd != NULL);
    Zmstd = (double *)malloc(npoints * sizeof(double)); assert(Zmstd != NULL);

  
    if (!lcurvetangents3D(mode, 0, NULL, npoints, X, Y, Z, Xmstd, Ymstd, Zmstd))
    {
      fprintf(stderr, "%s: procedure lcurvetangents3D failed\n", argv[0]);
      exit(1);
    }

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
      exit(1);
    }
    fprintf(fd, "B %d\n", npoints);
    for (i = 0; i < npoints; i++)
      fprintf(fd, "%g %g %g\n", Xmstd[i], Ymstd[i], Zmstd[i]);
    fclose(fd);

    free(Xmstd);
    free(Ymstd);
    free(Zmstd);
      }
    else
      {
	char tablefilename[512];
	int32_t n, nbis;
	int32_t decalage;
	uint64_t *tab_combi;
        char buf[25];
	double coef, normalisateur;
	double *dX, *dY, *dZ;
	int32_t *deltaX, *deltaY, *deltaZ;

	n = atoi(argv[3]);
	nbis = 2*n-1;

	dX = (double *)malloc(npoints * sizeof(double)); assert(dX != NULL);
	dY = (double *)malloc(npoints * sizeof(double)); assert(dY != NULL);
	dZ = (double *)malloc(npoints * sizeof(double)); assert(dZ != NULL);
	deltaX = (int32_t *)malloc(npoints * sizeof(int32_t)); assert(deltaX != NULL);
	deltaY = (int32_t *)malloc(npoints * sizeof(int32_t)); assert(deltaY != NULL);
	deltaZ = (int32_t *)malloc(npoints * sizeof(int32_t)); assert(deltaZ != NULL);
	tab_combi = (uint64_t*)malloc( (nbis+1)*sizeof( uint64_t ));

	deltaX[0] = 0;
	deltaY[0] = 0;
	deltaZ[0] = 0;
	for (i = 1; i < npoints; i++)
	  {
	    deltaX[i] = X[i]-X[i-1];
	    deltaY[i] = Y[i]-Y[i-1];
	    deltaZ[i] = Z[i]-Z[i-1];
	  }

	sprintf(tablefilename, "%s/src/tables/TabTriPascal.txt", getenv("PINK"));
	fd = fopen (tablefilename, "r");
	if (fd == NULL) 
	  {
	    fprintf(stderr, "%s : table not found\n", argv[0]);
	    exit(1);
	  }

	decalage = 21*(nbis*(nbis+1))/2+nbis+3;
	fseek(fd, decalage, SEEK_SET);

	for (i = 0; i < nbis+1 ; i++)
	  {
 	    fread( buf, 21, 1, fd);
	    tab_combi[i] = atol(buf);
	  }
	fclose(fd);

	coef = pow(2,-nbis);

	for (i = 0; i < npoints; i++)
	  {
	    dX[i] = 0;
	    dY[i] = 0;
	    dZ[i] = 0;
	    for (j = mcmax(0,n-i); j < mcmin(nbis+1,npoints+n-i-1); j++)
	      {
		dX[i] += (int64_t)(tab_combi[j]*deltaX[i+j-n+1]);
		dY[i] += (int64_t)(tab_combi[j]*deltaY[i+j-n+1]);
		dZ[i] += (int64_t)(tab_combi[j]*deltaZ[i+j-n+1]);
	      }
	    dX[i] = dX[i]*coef;
	    dY[i] = dY[i]*coef;
	    dZ[i] = dZ[i]*coef;
	    normalisateur = pow(pow(dX[i],2)+pow(dY[i],2)+pow(dZ[i],2),0.5);
	    dX[i] = dX[i]/normalisateur;
	    dY[i] = dY[i]/normalisateur;
	    dZ[i] = dZ[i]/normalisateur;
	  }


	fd = fopen(argv[argc-1],"w");
	if (!fd)
	  {
	    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
	    exit(1);
	  }
	fprintf(fd, "B %d\n", npoints);
	for (i = 0; i < npoints; i++)
	  fprintf(fd, "%10g %10g %10g\n", dX[i], dY[i], dZ[i]);
	fclose(fd);

    free(tab_combi);
    free(dX);
    free(dY);
    free(dZ);
    free(deltaX);
    free(deltaY);
    free(deltaZ);
    }

    free(X);
    free(Y);
    free(Z);
  }
  else
  {
    fprintf(stderr, "usage: %s: bad file format: %c \n", argv[0], type);
    exit(1);
  }

  return 0;
} /* main */
