
/*! \file powerwatFiltering.c
\defgroup aniso Anisotropic diffusion

\brief Implements anisotropic diffusion 

<B>Usage:</B> ./ perona_malik.exe image_name.pgm out.pgm

<B>Description:</B>
<B>Types supported:</B> byte 2d, int32_t 2d

\ingroup aniso

\author Camille Couprie
*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>

// Pink-specific
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>

//#include "cccodimage.h"
//#include <ccsort.h>
//#include <lMSF.h>
//#include <MSF_RW.h>
//#include <SPF.h>
//#include <graph.h>

//#include <image_toolbox.h>

//#include <argv.h>
//#include <filtering.h>

#include "perona_malik.h"

double sqr_diff(DBL_TYPE * x, DBL_TYPE  * y, int N)
{
  int i;
  double difference=0;
  for (i=0;i<N;i++)
  difference = difference + (x[i]-y[i])*(x[i]-y[i]);
  
  return difference;
}

double energy(int ** edges, double * x, int N, int M, double alpha)
{
  int i;
  double E=0;
  for (i=0;i<M;i++)
    E = E + (1- exp(-alpha*(x[edges[0][i]]-x[edges[1][i]])*(x[edges[0][i]]-x[edges[1][i]])));
  return E;
}

/* =================================================================== */ 
int neighbor( int i,  /* node index */
	      int k,  /* number of the desired neighbor node of node i */
	      int rs, /* rowsize of the image */
	      int cs, /* colsize of the image */
	      int ds) /* depth of the image */
/* =================================================================== */
/* 
  return the index of the k_th neighbor 
       5         From the top :
     3 0 1       4: slice below,  2 : slice above
       6 
  return -1 if the neighbor is outside the image */
 
{
  int rs_cs=rs*cs;
  int z = i / (rs_cs);  
  int x = i / (rs*ds);
  int y = i % (rs);
  
  switch(k)
    {
    case 1:
      if (y >= rs-1) return -1;
      else return i+1;break;	
    case 3:
      if (y == 0) return -1;
      else return i-1;break;
    case 2:
      if (x >= cs-1) return -1;
      else return i+rs;break;
    case 4:
      if (x == 0) return -1;
      else return i-rs;break;
    case 5:
      if (z >= ds-1) return -1;
      else return i+rs_cs;break;
    case 6:
      if (z == 0) return -1;
      else return i-rs_cs;break;
    case 0:
       return rs_cs;break;
    case -1:
      return rs_cs+1; break;
    }
   return -1; //never happens 
}


/* ======================================================================= */
void compute_edges(  int ** edges, /*array of node indexes composing edges */
		     struct graph G)
/* ======================================================================= */
{
  int i,j,k,l,M;
  M=0; k=0;
  int rs_cs = G.rs*G.cs;

 /*initializing the incidence array T*/

  for(k=0;k<G.ds;k++) 
    {
      for(i=0;i<G.cs;i++) 
	{
	  for(j=0;j<G.rs;j++)
	    {
	      if(i<(G.cs-1))
		{
		  edges[0][M]=j+i*G.rs+k*rs_cs;
		  edges[1][M]=j+(i+1)*G.rs+k*rs_cs;
		  M++;
		}
	    }
	}
      for(i=0;i<G.cs;i++) 
	{
	  for(j=0;j<G.rs;j++)
	    {
	      if(j<(G.rs-1))
		{
		  edges[0][M]=j+i*G.rs+k*rs_cs;
		  edges[1][M]=j+1+i*G.rs+k*rs_cs;
		  M++;
		}
	    }
	}
      if (k != G.ds-1)
	for(l=k*rs_cs;l<(k+1)*rs_cs;l++) 
	  {
	    edges[0][M]=l;
	    edges[1][M]=l+rs_cs;
	    M++;
	  }
    }
}



/* ==================================================================================================== */
void update_x(double * x ,
	      int rs, 
	      int cs, 
	      int ** edges,
	      double dt, 
	      double alpha)
/* ==================================================================================================== */
/* Computes x(k+1) from x(k) for the anisotropic diffusion*/
 
{
  int i,j, k,N;
  // double K2 = 1.0/alpha; /// 20*20;
  double sum;
  double nb;
  N = rs*cs;
  for (i=0;i<N;i++)
    {
      nb=0;
      sum = 0;
      for (k=1;k<=4;k++)
	{
	  j = neighbor(i, k, rs, cs,1);
	  sum = sum+ exp(-alpha*(x[i]-x[j])*(x[i]-x[j]))*(x[j]-x[i]);
	  nb++;
	}
      x[i] = x[i] + (dt/nb) *(sum);
    }
}




/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{

  int t1=clock();

  if (argc < 5)
    {
      fprintf(stderr, "usage: %s image_name.pgm dt (0.12 good fit) nbitermax (i.e. 100) alpha (around 0.0005) out.pgm [GT.pgm] \n", argv[0]);
      exit(1);
  }
 
  int i,k;
  struct xvimage * image_r;
 
  int rs, cs, N, M;  
   
 //lecture de l'image 

  image_r = readimage(argv[1]);
 
  rs = rowsize(image_r);
  cs = colsize(image_r);
 
  uint8_t *img = UCHARDATA(image_r);

  double dt = atof(argv[2]);
  double alpha = atof(argv[4]);
  N = rs * cs;
  M = rs*(cs-1)+(rs-1)*cs;  /*number of edges*/
 
  double  * x = (double  *)malloc(N*sizeof(double));
 
   for(i=0;i<N;i++)
     x[i]=img[i];

  int ** edges;

  edges = (int**)malloc(2*sizeof(int*));
  for(k=0;k<2;k++) edges[k] = (int*)malloc((M)*sizeof(int));
  int cpt = 0;
  struct graph G;
  
  G.rs = rs;
  G.cs = cs;
  G.ds = 1;
  compute_edges(edges, G);
  
  fprintf(stderr, "init ok \n");


  do 
    {
      update_x( x, rs, cs, edges, dt, alpha);
      cpt ++;
      //  printf("%f \n", energy(edges, x, N, M, alpha));
    }
  while (!(cpt>atoi(argv[3])));
    
     fprintf(stderr,"%d iterations \n", cpt);
   
     for(i=0;i<N;i++)
       img[i]=x[i];

     writeimage(image_r, argv[5]);

 int t2=clock();
   
  printf("Temps total : %.6lf seconds elapsed\n", ((double)t2-t1)/CLOCKS_PER_SEC);
    

   freeimage(image_r);
  
   for (i=0;i<2;i++) 
     free(edges[i]); free(edges);
  
  return 0;
} 


