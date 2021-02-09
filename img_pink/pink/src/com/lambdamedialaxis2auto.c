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
/*! \file lambdamedialaxis2.c

\brief discrete lambda-medial axis transform

<B>Usage:</B> lambdamedialaxis2 in.pgm lambda opening.pgm out.pgm

<B>Description:</B>

lambda:
 -1: produce lambdamedialaxis map
  0: automaticly define lambda parameter
  >0: produce binary image based on defined lambda
 
Experimental

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * distimage;
  struct xvimage * lambdaimage;
  struct xvimage * opening;	
  double lambda;
  uint32_t rs, cs, ds, N, i;
  float max, *L, *O;
  uint8_t *I;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm lambda opening.pgm fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  opening = readimage(argv[3]);
	
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  N = rs * cs * ds;

  lambdaimage = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
  if (lambdaimage == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  distimage = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if (distimage == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lsedt_meijster(image, distimage))
  {
    fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
    exit(1);
  }

  if (!llambdamedialaxis(distimage, lambdaimage))
  {
    fprintf(stderr, "%s: llambdamedialaxis failed\n", argv[0]);
    exit(1);
  }

  I = UCHARDATA(image);
  L = FLOATDATA(lambdaimage);
  O = FLOATDATA(opening);
  
  lambda = atof(argv[2]);

	if (lambda == 0) {
		//try to select lambda automaticaly
		double e = 2.718281828;
		uint32_t nbLam = 0;
		float maxlam = 0;
		float minlam = 10000;
		float maxope = 0;
		float minope = 10000;
		
		for (i = 0; i < N; i++)
			if (L[i] > 0)
			{
				nbLam++;
				
				if(L[i]>maxlam) maxlam = L[i];
				if(L[i]<minlam) minlam = L[i];
				
				if(O[i]>maxope) maxope = O[i];
				if(O[i]<minope) minope = O[i];
			}
		
		float *Lam = (float *) malloc(nbLam * sizeof (float));
		float *Ope = (float *) malloc(nbLam * sizeof (float));
		char *idx = (char *) malloc(nbLam * sizeof (char));
		double *distTo1 = (double *) malloc(nbLam * sizeof (double));
		double *distTo2 = (double *) malloc(nbLam * sizeof (double));
		
		nbLam=0;
		for (i = 0; i < N; i++)
			if (L[i] > 0)
			{
				Lam[nbLam] = L[i];
				Ope[nbLam] = O[i];
				nbLam++;
			}
		
		//initialize centroids
		float centroids[] = {0, 0};
		
		centroids[0] = log(maxope+1);
		centroids[1] = log(maxope+1);
		centroids[0] = centroids[0]/maxlam;
		centroids[1] = centroids[1]/minlam;
		centroids[0] = pow(e,centroids[0]);
		centroids[1] = pow(e,centroids[1]);
		
		char objectMoved = 1; 
		char nbIteration = 0;
		
		//main Loop
		while (objectMoved) {
			nbIteration++;
			
			//1. calculate the distance between cluster centroid to each object
			for (i=0; i<nbLam;i++)
			{
				double y2 = (log(Ope[i]+1))/(log(centroids[0]));
				distTo1[i] = (y2-Lam[i])*(y2-Lam[i]);
				y2 = (log(Ope[i]+1))/(log(centroids[1]));
				distTo2[i] = (y2-Lam[i])*(y2-Lam[i]);
			}
			
			//2. assign each object based on the minimum distance
			objectMoved = 0;
			for (i=0; i<nbLam;i++)
			{
				if ((distTo1[i]<distTo2[i]) && (idx[i] != 1))
				{
					objectMoved = 1;
					idx[i] = 1;
				}
				if ((distTo1[i]>distTo2[i]) && (idx[i] != 2))
				{
					objectMoved = 1;
					idx[i] = 2;
				}
			}
			
			//3. compute the new centroid of each group based on new memberships
			uint32_t count1=0;
			uint32_t count2=0;
			float val1 = 0;
			float val2 = 0;
			
			for (i=0; i<nbLam;i++)
			{
				if (idx[i] == 1)
				{
					count1++;
					val1+=Lam[i]/Ope[i];
				}
				if (idx[i] == 2)
				{
					count2++;
					val2+=Lam[i]/Ope[i];
				}
			}
			
			centroids[0] = log(maxope/2+1);
			centroids[1] = log(maxope/2+1);
			centroids[0] = centroids[0]/(maxope/2*(val1/count1));
			centroids[1] = centroids[1]/(maxope/2*(val2/count2));
			centroids[0] = pow(e,centroids[0]);
			centroids[1] = pow(e,centroids[1]);
			
			if (nbIteration > 30)
				break;
		}
		
		//Calculate parameter which separate 2 sets
		lambda = log(maxope+1);
		lambda = lambda/(log(maxope+1)/log(centroids[1])+fabs(log(maxope+1)/log(centroids[0])-log(maxope+1)/log(centroids[1]))/2);
		lambda = pow(e,lambda);
		
		//finalize
		free(Lam);
		free(Ope);
		free(idx);
		free(distTo1);
		free(distTo2);
		
		lambda=(lambda-1.1)/0.01;
		
		printf("%f\n",lambda);
	}
	
  //max=0;
  //for (i = 0; i < N; i++)
  //{
  //  if (O[i] > max) max=O[i];	  
  //}
  //lambda = (lambda*M_PI_4)/100;
	  
  for (i = 0; i < N; i++)
  {
	if(L[i] > 0)
	{
	 if(L[i] <= log(O[i]+1)/log(1.1+lambda*0.01))
    //if (L[i] <= ((lowlambda/L[i])+((O[i]*lambda)/max))) // michal
    //if (L[i]*L[i] <= ((O[i]*lambda)/max))
    //if (L[i] <  O[i]*tan(lambda))
    //if (L[i]*L[i] <  (O[i]*tan(lambda)*O[i]*tan(lambda)))
    //if (L[i] < (O[i] - (max - ((lambda*max)/M_PI_4)))) pas bon
    
	{
      I[i] = NDG_MIN;
    }
	}
	  else {
		  I[i] = NDG_MIN;
	  }

	
  }

  if(lambda>-1)
  {
    writeimage(image, argv[argc - 1]);
  }
  else
    writeimage(lambdaimage, argv[argc - 1]);

  freeimage(lambdaimage);
  freeimage(distimage);
  freeimage(image);
  freeimage(opening);

  return 0;
} /* main */


