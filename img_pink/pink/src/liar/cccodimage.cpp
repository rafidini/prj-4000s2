/*Copyright ESIEE (2009) 

Author :
Camille Couprie (c.couprie@esiee.fr)

Contributors : 
Hugues Talbot (h.talbot@esiee.fr)
Leo Grady (leo.grady@siemens.com)
Laurent Najman (l.najman@esiee.fr)

This software contains some image processing algorithms whose purpose is to be
used primarily for research.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
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

/****************************************************************
*
* Purpose:      useful functions to compute neigbors of points, edges, and gradient of images 
*
* Written By:   Camille Couprie, avril 2009 
*
****************************************************************/

#include <complex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cccodimage.hxx"

/* ================================================================================================================ */
void read3Drgbimage(const char * image_name, struct xvimage **image_r, struct xvimage **image_g, struct xvimage **image_b )
/* ================================================================================================================ */
{
  int rs, cs, ds;
  FILE *fd = NULL;
  fd = fopen(image_name,"r");
  
  char * read;
  int c, d;
  char buffer[1000];
  read = fgets(buffer,1000, fd); // P5: raw byte bw  ; P2: ascii bw  
  
  if (!read)
    fprintf(stderr, "read_image: fgets returned without reading\n");
  
  if (buffer[0] != 'P')
    fprintf(stderr,"read_image : invalid image format\n");
  
  do
    { 
      fgets(buffer, 1000, fd);
    }
  while (!isdigit(buffer[0]));
  
  c = sscanf(buffer, "%d %d %d \n", &rs, &cs, &ds);
  c = fread(&d, sizeof(unsigned char), 4, fd);
  unsigned char *img = (unsigned char *)malloc(sizeof(unsigned char)* rs * cs * ds*3);
  
  int ret = fread(img, sizeof(unsigned char), cs * rs * ds*3, fd);
  if (ret != rs * cs * ds*3)
      fprintf(stderr,"fread failed : %d asked ; %d read\n", cs * rs * ds*3, ret);
    
  fclose(fd);
  *image_r  = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  *image_g  = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  *image_b  = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  unsigned char *ir = UCHARDATA(*image_r);
  unsigned char *ig = UCHARDATA(*image_g);
  unsigned char *ib = UCHARDATA(*image_b);
  int i;
  for (i=0;i<rs*cs*ds;i=i+1)
    {
      ir[i] = img[i*3];
      ig[i] = img[i*3+1];
      ib[i] = img[i*3+2];
    }
  free(img);
}




/* ================================================================== */
void print_incidence_graph(struct graph *G) 
/* =================================================================== */
{
  int i, k;
  
  for (i=0;i<G->N;i++)
    {
      for (k = 0; k < G->Neighbors[i]->Sp; k += 1) 
	printf("%d ", (G->Neighbors[i]->Pts[k]));
      printf("\n");
    }
  
}


/* ================================================================== */
void fill_incidence_graph(struct graph *G,
			  int rs,
			  int cs) 
/* =================================================================== */
{
  int i, k, y;

 for (i=0;i<G->N;i++)
    G->Neighbors[i] = CreeLifoVide(G->DegreMax);
 
  for (i=0;i<G->N;i++)
    for (k = 1; k <= G->DegreMax; k += 1) 
      {
	y = neighbor_node_edge(i, k, rs, cs, G->levels);
	if ((y!=-1)&&(y<G->M)) LifoPush(G->Neighbors[i],y);
      }

}



/*
% example of indexing of the edges in 3D

%        27      28
%       _____ ________
%     /      /       /|
%  12/    13/     14/ |
%   /__6___/___7__ /  | 23
%   |      |      |   |
%  0|     1|     2|17/|
%   |      |      | / |
%   |__8___|___9__|/  | 26
%   |      |      |   |
%  3|     4|     5|20/
%   |      |      | /
%   |__10__|__11__|/

*/


/* ================================================================== */
int neighbor_edge(int i,  /* edge index */
		  int k,  /* number of the desired neighbor of edge i */
		  int rs, /* rowsize of the image */
		  int cs, /* colsize of the image */
		  int ds) /* depth of the image */
/* =================================================================== */
/* return the index of the k_th neighbor 
(only works in 2D, a little faster than the neighbor_edge3D)
  
%      1       _|_          
%    2 0 6     _|_          2 1      _|_|_
%    3 0 5      |         3 0 0 6     | | 
%      4                    4 5 
%                           
% return -1 if the neighbor is outside the image


% indexing edges 2D
% 
%    _4_ _5_
%  0|  1|
%   |_6_|_7_
%  2|  3|
%   |   |

*/
{
  int V = (cs-1)*rs; // nb vertical edges 
  if (i >= V)
    {
      //horizontal
      switch(k)
	{
	case 2:
	  if ((i-V) < rs-1) return -1;
	  else return ((i-V)/(rs-1)-1)*rs + ((i - V)%(rs-1));break;
	case 3:
	  if ((i-V)%(rs-1)==0) return -1;
	  else return i-1 ;break;
	case 4:
	  if (i>(rs-1)*cs+ V -rs) return -1;
	  else return ((i-V)/(rs-1)-1)*rs + ((i - V)%(rs-1)) + rs;break;
	case 5:
	  if (i>(rs-1)*cs+ V - rs) return -1;
	  else return ((i-V)/(rs-1)-1)*rs + ((i - V)%(rs-1))  + rs +1;break;
	case 6:
	  if ((i-V)%(rs-1)==rs-2) return -1;
	  else return i+1;break;
	case 1:
	  if (i-V<rs-1) return -1;
	  else return ((i-V)/(rs-1)-1)*rs + ((i - V)%(rs-1))+1;break;
	}
    }
  else
    { //vertical
      switch(k)
	{
	case 6:
	  if (i %rs == rs-1) return -1;
	  else return (i+V)-(i/rs);break;
	case 1:
          if (i < rs) return -1;
          else return i-rs;break;
        case 2:
          if (i%rs==0) return -1;
          else return (i+V)-(i/rs)-1;break;
        case 3:
          if (i%rs==0) return -1;
          else  return (i+V)-(i/rs)-1+rs-1;break;
        case 4:
          if (i>=V-rs) return -1;
          else return i+rs;break;
        case 5:
	  if (i %rs == rs-1) return -1;
	  else return (i+V)-(i/rs)+rs-1;break;
	}
    }
  return -1; //never happens 
}


/* ============================================================================= */ 
int neighbor_node_edge( int i,  /* node index */
			int k,  /* number of the desired edge neighbor of node i */
			int rs, /* rowsize of the image */
			int cs, /* colsize of the image */
			int ds) /* depth of the image */
/* ============================================================================== */
/* return the index of the k_th edge neighbor of the node "i" 
    10 4 9  
     3 0 1   5 front slice,  6 back slice
     7 2 8
   return -1 if the neighbor is outside the image */
{
  int rs_cs = rs*cs;
  int zp = i % (rs_cs); 
  int z = i / (rs_cs);   
  int V = (cs-1)*rs;
  int H = (rs-1)*cs;
  switch(k)
    {
    case 1:
      if (zp % rs >= rs-1) return -1;
      else return (zp+V)-(zp/rs)+z*(V+H+rs_cs);break;
    case 3:
      if (zp % rs == 0) return -1;
      else return (zp+V)-(zp/rs)-1+z*(V+H+rs_cs);break;
    case 2:
      if (zp / rs >= cs-1) return -1;
      else return zp+z*(V+H+rs_cs);break;
    case 4:
      if (zp / rs == 0) return -1;
      else return zp-rs+z*(V+H+rs_cs);break;
    case 5:
      if (z == 0) return -1;
      else return z*(V+H)+zp+(z-1)*rs_cs;break;
    case 6:
      if (z >= ds-1) return -1;
      else return (z+1)*(V+H)+zp+z*rs_cs;break;
      // 8 connectivity
    case 7:
      if ((zp % rs ==0)||(zp / rs == 0)) return -1;
      else return zp-rs+(z+1)*(V+H+rs_cs)-(zp/rs); break; 
    case 9:
      if ((zp % rs == 0)||(zp / rs >= cs-1)) return -1;
      else return zp+(z+1)*(V+H+rs_cs)-(zp/rs)+(rs-1)*(cs-1)-1; break;  
    case 8:
      if ((zp / rs >= cs-1)||(zp % rs >= rs-1)) return -1;
      else return zp+(z+1)*(V+H+rs_cs)-(zp/rs);break; 
    case 10:
      if ((zp / rs == 0)||(zp % rs >= rs-1)) return -1;
      else  return zp-rs+(z+1)*(V+H+rs_cs)-(zp/rs)+1+(rs-1)*(cs-1);break;
    }
  return -1; //never happens 
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
void compute_edges(  struct graph *G,
		     int rs, 
		     int cs,
		     int ds)
/* ======================================================================= */
{
  int i,j,k,l,M;
  M=0; k=0;
  int rs_cs = rs*cs;

 
  /*  if (array)
    for (i=0;i<G->N;i++)
    G->Neighbors[i] = CreeLifoVide(G->DegreMax);*/
 
  for(k=0;k<ds;k++) 
    {
      for(i=0;i<cs;i++) 
	{
	  for(j=0;j<rs;j++)
	    {
	      if(i<(cs-1))
		{
		  G->Edges[0][M]=j+i*rs+k*rs_cs;
		  G->Edges[1][M]=j+(i+1)*rs+k*rs_cs;
		   LifoPush(G->Neighbors[G->Edges[0][M]],M);
		    LifoPush(G->Neighbors[G->Edges[1][M]],M);
		  M++;
		}
	    }
	}
      for(i=0;i<cs;i++) 
	{
	  for(j=0;j<rs;j++)
	    {
	      if(j<(rs-1))
		{
		  G->Edges[0][M]=j+i*rs+k*rs_cs;
		  G->Edges[1][M]=j+1+i*rs+k*rs_cs;
		  LifoPush(G->Neighbors[G->Edges[0][M]],M);
		    LifoPush(G->Neighbors[G->Edges[1][M]],M);
		  M++;
		}
	    }
	}
      if (k != ds-1)
	for(l=k*rs_cs;l<(k+1)*rs_cs;l++) 
	  {
	    G->Edges[0][M]=l;
	    G->Edges[1][M]=l+rs_cs;
	    LifoPush(G->Neighbors[G->Edges[0][M]],M);
	    LifoPush(G->Neighbors[G->Edges[1][M]],M);
	    M++;
	  }
    }
}









/* =============================================================== */
void grey_weights( char * image_name , /* image name */
		   bool geod,          /* if true, geodesic reconstruction is performed */
		   struct graph *G, 
		   int rs, 
		   int cs,
		   int ds)  
/* =============================================================== */
/* Computes weights inversely proportionnal to the image gradient for grey level images */
{
  int i;
  uint8_t *img;
  struct xvimage * image;
  image = readimage(image_name);
  if (image == NULL)
    {
      fprintf(stderr, "msf_rw: readimage failed\n");
      exit(1);
    }
 
  img = UCHARDATA(image); 
 
  if (geod==false)
    {
      for (i=0;i<G->M;i++)
	G->Weights[i]= 255-labs(img[G->Edges[0][i]]-img[G->Edges[1][i]]);
    }
  else 
    {
      int j,k,n;
      uint32_t * weights_tmp = (uint32_t*) malloc(G->M*sizeof(uint32_t));
      uint32_t * seeds_function =(uint32_t *)calloc(G->M,sizeof(uint32_t));
     
      for (i=0;i<G->M;i++)
	weights_tmp[i]=255-labs(img[G->Edges[0][i]]-img[G->Edges[1][i]]) ;
	
      for (j=0;j<G->S;j++)
	for (k=1;k<=G->DegreMax; k++)
	  {
	    n = neighbor_node_edge(G->SeededNodes[j], k, rs, cs, ds);
	    if (n != -1)
	      seeds_function[n]= weights_tmp[n];
	  } 
     
   
      geodilate_union_find<unsigned int>(seeds_function,G->Weights, G, 255, false);
     
      free(weights_tmp);
      free(seeds_function);
    }
  freeimage(image);
}

/* ======================================================================================================== */
void grey_weights_PW(struct xvimage *image, /* already read */  
			   struct graph *G, 
			   int rs, 
			   int cs,
			   int ds)  
/* ======================================================================================================== */
/* Computes weights inversely proportionnal to the image gradient for grey level (pgm) images 
   Returns the normal weights and computes the reconstructed weights in the array weights */

{
  int i, j,k,n;
  uint8_t *img;

  img = UCHARDATA(image); 

  for (i=0;i<G->M;i++)
     G->Weights[i]=  255-labs(img[G->Edges[0][i]]-img[G->Edges[1][i]]);

  for (j=0;j<G->S;j++)
    for (k=1;k<=G->DegreMax; k++)
      {
	n = neighbor_node_edge(G->SeededNodes[j], k, rs, cs, ds);
	if (n != -1)
	  G->RecWeights[n]=  G->Weights[n];
      }
  geodilate_union_find<unsigned int>( G->Weights, G->RecWeights, G, (uint32_t)255, false);
}


/* ======================================================================================================== */
void grey_weights_PW(char * image_name, /*IN : image name */  
			   struct graph *G, 
			   int rs, 
			   int cs,
			   int ds)  
/* ======================================================================================================== */
/* Computes weights inversely proportionnal to the image gradient for grey level (pgm) images 
   Returns the normal weights and computes the reconstructed weights in the array weights */

{
  struct xvimage * image;
  
  image = readimage(image_name);
  if (image == NULL)
    {
      fprintf(stderr, "grey_weights_PW: readimage failed\n");
      exit(1);
    }

  // call function just above
  grey_weights_PW(image, G, rs, cs, ds);
  
  
  
  freeimage(image);
}

/* ================================================================================================================= */
void color_standard_weights_PW( struct xvimage *image_r , /* image */
                                struct xvimage *image_v,
                                struct xvimage *image_b,
                                uint32_t * maxi,         /* OUT : the maximum weight value */
                                struct graph *G)
/* ================================================================================================================= */
/* Computes weights inversely proportionnal to the image gradient for 2D color (ppm) images 
   Returns the normal weights and computes the reconstructed weights in the array weights */
{

  int i, rs, cs, ds, j,k,n;
  uint8_t wr, wv, wb;

  uint8_t *img_r;
  uint8_t *img_v;
  uint8_t *img_b;
  // uint32_t * seeds_function =( uint32_t *)calloc(G->M,sizeof(uint32_t));
  int numvoisins = 4;
  int size_seeds = G->S;

  *maxi = 0;

  ds=1;//default
 
  rs = rowsize(image_r);
  cs = colsize(image_r);
  img_r = UCHARDATA(image_r);
  img_v = UCHARDATA(image_v);
  img_b = UCHARDATA(image_b);
  
  for (i=0;i<G->M;i++)
    {
      // PAMI
      wr = labs(img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]]) ;
      wv = labs(img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]]) ;
      wb = labs(img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]]) ;
      G->Weights[i] = wr*wr+wv*wv+wb*wb;
      if (G->Weights[i]> *maxi) *maxi = G->Weights[i];
      
      //ICCV
      /*   wr = fabs(img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]]);
      wv = fabs(img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]]);
      wb = fabs(img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]]);
     
      weights[i] = 255-wr;
      if (255-wv < weights[i]) weights[i] = 255-wv; 
      if (255-wb < weights[i]) weights[i] = 255-wb;
      *maxi = 255;*/
    } 
 
  if (ds>1) numvoisins = 6;
  for (i=0;i<G->M;i++)
    G->Weights[i] =  *maxi-G->Weights[i];

  printf("%d \n", G->S);
  for (j=0;j<size_seeds;j++)
    for (k=1;k<=numvoisins; k++)
      {
	n = neighbor_node_edge(G->SeededNodes[j], k, rs, cs, ds);
	if (n != -1)
	  G->RecWeights[n]= G->Weights[n]; //PAMI
      } 
 
  geodilate_union_find<unsigned int>(G->Weights,G->RecWeights, G, *maxi, false);
  

  //  for (j = 0; j < G->M; j++)
  //  fprintf(stderr, "weights[%d]=%d (%d)\n", j, normal_weights[j], weights[j]);
}



/* ================================================================================================================= */
void color_standard_weights_PW( char * image_name , /* IN : image name */
				      uint32_t * maxi,         /* OUT : the maximum weight value */
				      struct graph *G)
/* ================================================================================================================= */
/* Computes weights inversely proportionnal to the image gradient for 2D color (ppm) images 
   Returns the normal weights and computes the reconstructed weights in the array weights */
{
  struct xvimage * image_r;
  struct xvimage * image_v;
  struct xvimage * image_b;
 
  readrgbimage(image_name,  &image_r, &image_v, &image_b);

  color_standard_weights_PW(image_r, image_v, image_b, maxi, G);
  
  freeimage(image_r);
  freeimage(image_v);
  freeimage(image_b);
  //  for (j = 0; j < G->M; j++)
  //  fprintf(stderr, "weights[%d]=%d (%d)\n", j, normal_weights[j], weights[j]);
}


/* ================================================================================================================= */
void color_3D_weights_PW( struct xvimage *image_r , /* IN : image name */
                          struct xvimage *image_v,  // "vert" = green
                          struct xvimage *image_b,
			  uint32_t * maxi,         /* OUT : the maximum weight value */
			  struct graph *G)
/* ================================================================================================================= */
/* Computes weights inversely proportionnal to the image gradient for 2D color (ppm) images 
   Returns the normal weights and computes the reconstructed weights in the array weights */
{
    *maxi = 0;
    int i, rs, cs, ds, j,k,n;
    uint8_t wr, wv, wb;
    uint8_t *img_r;
    uint8_t *img_v;
    uint8_t *img_b;
    // uint32_t * seeds_function =( uint32_t *)calloc(G->M,sizeof(uint32_t));
    int numvoisins = 4;
    int size_seeds = G->S;
 
  
    rs = rowsize(image_r);
    cs = colsize(image_r);
    ds = depth(image_r);

    fprintf(stderr,"ds=%d, cs=%d, ds=%d\n",rs,cs,ds);
    img_r = UCHARDATA(image_r);
    img_v = UCHARDATA(image_v);
    img_b = UCHARDATA(image_b);
  
    i=-1;
    int mm;
    int rs_cs = rs*cs;
    for (k=0;k<ds;k++)
    {
        for (j=0;j<(rs-1)*cs+(cs-1)*rs;j++)
        {
            // weights used in PAMI
            i++;
            wr = labs(img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]]) ;
            wv = labs(img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]]) ;
            wb = labs(img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]]) ;
     
            G->Weights[i] = wr*wr+wv*wv+wb*wb;
            if (G->Weights[i]> *maxi) *maxi = G->Weights[i];
        }

        if (k != ds-1) // edges entre 2 images successives
            for(j=k*rs_cs;j<(k+1)*rs_cs;j++) 
            {
                i++;
                wr = labs(img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]]) ;
                wv = labs(img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]]) ;
                wb = labs(img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]]) ;
                mm = wr*wr; 
                G->Weights[i] =(mm*4);
                if (G->Weights[i]> *maxi) *maxi = G->Weights[i];
            }
        // weights used infor ICCV, not as effective perhaps.
        /*   wr = fabs(img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]]);
             wv = fabs(img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]]);
             wb = fabs(img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]]);
     
             weights[i] = 255-wr;
             if (255-wv < weights[i]) weights[i] = 255-wv; 
             if (255-wb < weights[i]) weights[i] = 255-wb;
             *maxi = 255;*/
    }
 
    if (ds>1) numvoisins = 6;
    for (i=0;i<G->M;i++){
        G->Weights[i] =  *maxi-G->Weights[i];
        G->RecWeights[i]=G->Weights[i];
    }

    fprintf(stderr,"------------------- \n");

    // re-arange weights into a limited range
    uint32_t * Es = (uint32_t*)malloc(G->M*sizeof(uint32_t));
    for (i=0;i<G->M;i++)
        Es[i] = i;

    Es = linsortimageup<uint32_t>(G->Weights, G->M, *maxi+1);
  
    j=0;
    for (i=0;i<G->M-1;i++)
    {
        G->RecWeights[Es[i]] = j;
        if(labs(G->Weights[Es[i]]-G->Weights[Es[i+1]])>epsilon)
            j++;
    }
    G->RecWeights[Es[G->M-1]] = j-1;
   

    for (i=0;i<G->M;i++)
    {G->Weights[i]=G->RecWeights[i];
        G->RecWeights[i]=0;
    }
  



    *maxi =j;
    fprintf(stderr,"%d \n", *maxi);
    for (j=0;j<size_seeds;j++)
        for (k=1;k<=numvoisins; k++)
        {
            n = neighbor_node_edge(G->SeededNodes[j], k, rs, cs, ds);
            if (n != -1)
                G->RecWeights[n]= G->Weights[n]; //PAMI
        } 
    //fprintf(stderr,"%d \n", *maxi);

    //--------------------------------------------------

    geodilate_union_find<unsigned int>(G->Weights,G->RecWeights, G, *maxi, false);
  
    //  for (j = 0; j < G->M; j++)
    //  fprintf(stderr, "weights[%d]=%d (%d)\n", j, normal_weights[j], weights[j]);
}

/* ================================================================================================================= */
void color_3D_weights_PW( char * image_name , /* IN : image name */
			  uint32_t * maxi,         /* OUT : the maximum weight value */
			  struct graph *G)
/* ================================================================================================================= */
/* Computes weights inversely proportionnal to the image gradient for 2D color (ppm) images 
   Returns the normal weights and computes the reconstructed weights in the array weights */
{
  struct xvimage * image_r;
  struct xvimage * image_v;
  struct xvimage * image_b;
 
  read3Drgbimage((const char *)image_name,  &image_r, &image_v, &image_b);

  // call function just above
  color_3D_weights_PW(image_r, image_v, image_b, maxi, G);
  
  freeimage(image_r);
  freeimage(image_v);
  freeimage(image_b);
  //  for (j = 0; j < G->M; j++)
  //  fprintf(stderr, "weights[%d]=%d (%d)\n", j, normal_weights[j], weights[j]);
}





/* ================================================================================================= */
int color_standard_weights( char * image_name , /* image name */
			    uint32_t * weights, /* array to store the values of weights on the edges */
			    uint32_t * seeds,        /* array of seeded nodes indexes */
			    int size_seeds,     /* nb of seeded nodes */
			    bool geod,          /* if true, geodesic reconstruction is performed */
			    struct graph *G)
/* ================================================================================================== */
/* Computes weights inversely proportionnal to the image gradient for 2D color (ppm) images */
{
  uint32_t maxi = 0;
  int i, rs, cs, ds;

  uint8_t wr, wv, wb;
  struct xvimage * image_r;
  struct xvimage * image_v;
  struct xvimage * image_b;
  uint8_t *img_r;
  uint8_t *img_v;
  uint8_t *img_b;
  
  ds=1;//default
 
  readrgbimage(image_name,  &image_r, &image_v, &image_b);
  rs = rowsize(image_r);
  cs = colsize(image_r);
  img_r = UCHARDATA(image_r);
  img_v = UCHARDATA(image_v);
  img_b = UCHARDATA(image_b);
 
  for (i=0;i<G->M;i++)
    {
      /*   wr = abs(img_r[edges[0][i]]-img_r[edges[1][i]]) ;
      wv = abs(img_v[edges[0][i]]-img_v[edges[1][i]]) ;
      wb = abs(img_b[edges[0][i]]-img_b[edges[1][i]]) ;
      weights[i] = wr*wr+wv*wv+wb*wb;
      if (weights[i]> maxi) (maxi) = weights[i];*/

      //ICCV
      wr = labs(img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]]);
      wv = labs(img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]]);
      wb = labs(img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]]);
      
      weights[i] = 255-wr;
      if (255-wv < (int)weights[i]) weights[i] = 255-wv; 
      if (255-wb < (int)weights[i]) weights[i] = 255-wb;
      maxi = 255;

    }
  if (geod==false)
    { 
      for (i=0;i<G->M;i++)
	//weights[i]= maxi-weights[i];
	weights[i]= weights[i];
    }
  else
    { 
      int j,k,n;
      uint32_t * weights_tmp = (uint32_t*) malloc(G->M*sizeof(uint32_t));
      uint32_t * seeds_function =(uint32_t *)calloc(G->M,sizeof(uint32_t));
      int numvoisins = 4;
      if (ds>1) numvoisins = 6;
      for (i=0;i<G->M;i++)
	weights_tmp[i]=maxi-weights[i];
	
      for (j=0;j<size_seeds;j++)
	for (k=1;k<=numvoisins; k++)
	  {
	    n = neighbor_node_edge(seeds[j], k, rs, cs, ds);
	    if (n != -1)
	      seeds_function[n]=weights_tmp[n];
	  } 
      geodilate_union_find<unsigned int>(weights,seeds_function, G, maxi, false);
      free(weights_tmp);
      free(seeds_function);
    }
  freeimage(image_r);
  freeimage(image_v);
  freeimage(image_b);
  return maxi;
}




/* =============================================================== */
void color_gaussian_weights_double( char * image_name ,
				  double beta, 
				  double  * weights, 
				    struct graph *G,
				  uint32_t * seeds,
				  int size_seeds )
/* =============================================================== */
{
  double maxi = 0;
  int i,M, rs, cs, ds;
  double wr, wv, wb;
  struct xvimage * image_r;
  struct xvimage * image_v;
  struct xvimage * image_b;
  uint8_t *img_r;
  uint8_t *img_v;
  uint8_t *img_b;
  ds=1;//default
 
  readrgbimage(image_name,  &image_r, &image_v, &image_b);
  rs = rowsize(image_r);
  cs = colsize(image_r);
  img_r = UCHARDATA(image_r);
  img_v = UCHARDATA(image_v);
  img_b = UCHARDATA(image_b);
  
  M = ds*rs*(cs-1)+ds*(rs-1)*cs+(ds-1)*cs*rs; 
	 
  for (i=0;i<M;i++)
    {
        wr = (img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]])*(img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]]) ;
      wv = (img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]])* (img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]]) ;
      wb = (img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]])*(img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]]) ;
      //  weights[i] =wr*wr+wv*wv+wb*wb;

      weights[i] = wr;
      if (wv > weights[i]) weights[i] = wv; 
      if (wb > weights[i]) weights[i] = wb;

      if (weights[i]> maxi) (maxi) = weights[i];
    }

  for (i=0;i<M;i++)
    weights[i]= exp(-beta*(weights[i]/maxi))+epsilon;

  freeimage(image_r);
  freeimage(image_v);
  freeimage(image_b);
}


/* =============================================================== */
void color_standard_weights_double( char * image_name ,
				  double beta, 
				  double * weights, 
				  struct graph *G,
				  int * seeds,
				  int size_seeds )
/* =============================================================== */
{
  double maxi = 0;
  int i,M, rs, cs, ds;
  double wr, wv, wb;
  struct xvimage * image_r;
  struct xvimage * image_v;
  struct xvimage * image_b;
  uint8_t *img_r;
  uint8_t *img_v;
  uint8_t *img_b;
  ds=1;//default
 
  readrgbimage(image_name,  &image_r, &image_v, &image_b);
  rs = rowsize(image_r);
  cs = colsize(image_r);
  img_r = UCHARDATA(image_r);
  img_v = UCHARDATA(image_v);
  img_b = UCHARDATA(image_b);
  
  M = ds*rs*(cs-1)+ds*(rs-1)*cs+(ds-1)*cs*rs; 
	 
  for (i=0;i<M;i++)
    {
      wr = labs(img_r[G->Edges[0][i]]-img_r[G->Edges[1][i]]) ;
      wv = labs(img_v[G->Edges[0][i]]-img_v[G->Edges[1][i]]) ;
      wb = labs(img_b[G->Edges[0][i]]-img_b[G->Edges[1][i]]) ;
      weights[i] =sqrt(wr*wr+wv*wv+wb*wb);
      if (weights[i]> maxi) (maxi) = weights[i];
    }

  for (i=0;i<M;i++)
    weights[i]= exp(-beta*(weights[i]/maxi))+epsilon;

  freeimage(image_r);
  freeimage(image_v);
  freeimage(image_b);
}



/* =============================================================== */
void grey_weights_double( char * image_name, 
			  double beta,
			  double * weights, 
			  struct graph *G,
			  uint32_t * seeds,
			  int size_seeds)
/* =============================================================== */
{
  int i,M, rs, cs, ds;
  uint8_t *img;
  struct xvimage * image;
  image = readimage(image_name);
  if (image == NULL)
    {
      fprintf(stderr, "msf_rw: readimage failed\n");
      exit(1);
    }
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  img = UCHARDATA(image); 
 
  M = ds*rs*(cs-1)+ds*(rs-1)*cs+(ds-1)*cs*rs;  /*number of G->Edges*/
  
   for (i=0;i<M;i++)
      weights[i]= exp(-beta*(labs(img[G->Edges[0][i]]-img[G->Edges[1][i]]))/255)+epsilon;
  
  freeimage(image);
}
