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
/*! \file lambdamedialaxis.c

\brief discrete lambda-medial axis transform

<B>Usage:</B> lambdamedialaxis in.pgm [lambda] out.pgm

<B>Description:</B>
Discrete lambda-medial axis, as defined in [CCT09], 
of the binary image \b X contained in \b in.pgm.

By default, the output is a float image representing the function x->lambda(x).

If the parameter \b lambda is given (bash interface only, not python), the output is a binary image obtained by thresholding the aforementioned function at the value \b lambda .

If the parameter \b lambda is given and set to 0, the output is a binary image obtained by thresholding the aforementioned function at the automaticaly defined (k-means) value \b lambda . 

 References:<BR> 
[CCT09] John Chaussard, Michel Couprie, and Hugues Talbot. A discrete lambda-medial axis. 15th Discrete Geometry for Computer Imagery (DGCI'09). Lecture Notes in Computer Science.   2009.  pp. 1–12. To appear.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

/*
%TEST lambdamedialaxis %IMAGES/2dbyte/binary/b2hebreu.pgm %RESULTS/lambdamedialaxis_b2hebreu.pgm
%TEST lambdamedialaxis %IMAGES/3dbyte/binary/b3a.pgm %RESULTS/lambdamedialaxis_b3a.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * lambdaimage;
  double lambda;
  uint32_t rs, cs, ds, N, i;
  uint8_t *I;
  float *L;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.pgm [lambda] fileout.pgm\n", argv[0]);
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
  N = rs * cs * ds;

  lambdaimage = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
  if (lambdaimage == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (!lmedialaxis_lambdamedialaxis(image, lambdaimage))
  {
    fprintf(stderr, "%s: lmedialaxis_lambdamedialaxis failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    I = UCHARDATA(image);
    L = FLOATDATA(lambdaimage);
    lambda = atof(argv[2]);
    
	if (lambda == 0) {
		//try to select lambda automaticaly
		uint32_t nbLam = 0;
		float maxlam = 0;
		float minlam = 10000;
		
		for (i = 0; i < N; i++)
			if (L[i] > 0)
			{
				nbLam++;
				
				if(L[i]>maxlam) maxlam = L[i];
				if(L[i]<minlam) minlam = L[i];
			}
		float *Lam = (float *) malloc(nbLam * sizeof (float));
		char *idx = (char *) malloc(nbLam * sizeof (char));
		double *distTo1 = (double *) malloc(nbLam * sizeof (double));
		double *distTo2 = (double *) malloc(nbLam * sizeof (double));
		
		nbLam=0;
		for (i = 0; i < N; i++)
			if (L[i] > 0)
			{
				Lam[nbLam] = L[i];
				nbLam++;
			}
		
		//initialize centroids
		float centroids[] = {0, 0};
		centroids[0] = maxlam;
		centroids[1] = minlam;
		
		char objectMoved = 1; 
		char nbIteration = 0;
		
		//main Loop
		while (objectMoved) {
			nbIteration++;
			
			//1. calculate the distance between cluster centroid to each object
			for (i=0; i<nbLam;i++)
			{
				distTo1[i] = (centroids[0]-Lam[i])*(centroids[0]-Lam[i]);
				distTo2[i] = (centroids[1]-Lam[i])*(centroids[1]-Lam[i]);
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
					val1+=Lam[i];
				}
				if (idx[i] == 2)
				{
					count2++;
					val2+=Lam[i];
				}
			}
			
			
			centroids[0] = val1/count1;
			centroids[1] = val2/count2;
			if (nbIteration > 30)
				break;
		}
		
		//Calculate lambda parameter which separate 2 sets
		lambda = centroids[1] + (centroids[0] - centroids[1])/2;
		
		//finalize
		free(Lam);
		free(idx);
		free(distTo1);
		free(distTo2);
		
		printf("%f\n",lambda);
	}
	
	for (i = 0; i < N; i++)
      if (L[i] <= lambda) 
	I[i] = NDG_MIN;
    writeimage(image, argv[argc - 1]);
  }
  else
    writeimage(lambdaimage, argv[argc - 1]);

  freeimage(lambdaimage);
  freeimage(image);

  return 0;
} /* main */