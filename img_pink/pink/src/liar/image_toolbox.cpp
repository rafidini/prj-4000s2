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

/*! \defgroup visu Visualization

\file image_toolbox.c
\brief Implements several functions usefull for results visualization

\ingroup visu

\author Camille Couprie
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "mcimage.h"
#include "mccodimage.h"
#include "larith.h"
#include "image_toolbox.h"
#include "cccodimage.hxx"

#define MAXSIZEFILENAME 200

/* =============================== */
char * AllocFileName(char * name)
/* =============================== */
{
  name = (char *)calloc(sizeof(char),MAXSIZEFILENAME);
  return name;
}

/* =============================================================================== */
void ComputeSeedsSurfRec(char * image_name, struct graph *G) 
/* =============================================================================== */
  /* IN : distance image and the structure Graph 
     OUT : two arrays of the structure graph filled from the seed image
- the array G->SeededNodes contains the vertices indexes of seeded nodes, 
- the array G->Labels contains the associated values to those seeds. 
  */
{
  int i, j, rs, cs, ds;
  struct xvimage * seeds;
  unsigned char * s;
  seeds = readimage(image_name);
  if (seeds == NULL) { fprintf(stderr, "msf_rw: readimage failed\n"); exit(1); }
  s = UCHARDATA(seeds);
  rs=rowsize(seeds);
  cs=colsize(seeds);
  ds = depth(seeds);
  j=0;
 
  // Foreground seeds
  for (i=0;i<G->N;i++)
    {
      if(s[i]>128) 
	{
	  G->SeededNodes[j] = i;
	  G->Labels[j]=1;
	  j++;
	}
    }
 
 // Background seeds
  if(ds>1) //set of 3D points 
    {
      for (i=0;i<rs*cs;i++)
	{
	  G->SeededNodes[j] = i;
	  G->Labels[j]=2;
	  j++;
	}
      for (i=(ds-1)*rs*cs;i<rs*cs*ds;i++)
	{
	  G->SeededNodes[j] = i;
	  G->Labels[j]=2;
	  j++;
	  }

      G->SeededNodes[j] = 0;
      G->Labels[j]=2;
      j++;
      G->SeededNodes[j] = rs*cs*ds-1;
      G->Labels[j]=2;
      j++;
    }
  else  //set of 2D points 
    {
      for (i=0;i<rs;i++)
	{
	  G->SeededNodes[j] = i;
	  G->Labels[j]=2;
	  j++;
	}
      for (i=(cs-1)*rs;i<rs*cs;i++)
	{
	  G->SeededNodes[j] = i;
	  G->Labels[j]=2;
	  j++;
	}
    }

  G->S = j; 
  freeimage(seeds);
  G->P = 1;


}




/* =============================================================================== */
void StoresGrabCutFormatSeeds(char * image_name, struct graph *G) 
/* =============================================================================== */
  /* IN : image name and the structure Graph 
     OUT : two arrays of the structure graph filled from the seed image
- the array G->SeededNodes contains the vertices indexes of seeded nodes, 
- the array G->Labels contains the associated values to those seeds. 

A seed image from the GrabCut database contains 
- white pixel values as foreground seeds
- black pixel values as background seeds
- grey pixel values as unseeded pixels
  */
{
  int i, j;
  struct xvimage * seeds;
  unsigned char * s;
  seeds = readimage(image_name);
  if (seeds == NULL) { fprintf(stderr, "msf_rw: readimage failed\n"); exit(1); }
  s = UCHARDATA(seeds);
  j=0;
 
  for (i=0;i<G->N;i++)
    {
      if(s[i]>155) 
	{
	  G->SeededNodes[j] = i;
	  G->Labels[j]=2;
	  j++;
	}
      else if(s[i]<100) 
	{
	  G->SeededNodes[j] = i;
	  G->Labels[j]=1;
	  j++;
	}
    }
  G->S = j; 
  freeimage(seeds);
  G->P = 1;
}


int StoresMultiSeeds(struct xvimage *seeds,
                      struct graph *G) 
/* =============================================================================== */
  /* IN : image structure and the structure Graph 
     OUT : two arrays of the structure graph filled from the seed image 
- the array G->SeededNodes contains the vertices indexes of seeded nodes, 
- the array G->Labels contains the associated values to those seeds.

The multiseed format for a seed image contains 
- value 0 for unseeded pixels
- values 1, 2, ... as labeled values for seeded points. 
  */
{
    int i, j, retval=0;
    unsigned char * s;

    assert(seeds != NULL);
  
    s = UCHARDATA(seeds);
    j=0;
    G->P = 0;
    for (i=0;i<G->N;i++)
        if(s[i]>0)
        {
            G->SeededNodes[j]=i;
            G->Labels[j]=s[i];
            j++;
            if(s[i]>G->P) {G->P = s[i]; } 
        }
    G->S = j;
    printf("%d \n", G->S);
    G->P = G->P-1;

    return retval;
}


/* =============================================================================== */
int StoresMultiSeeds(char * image_name,
		     struct graph *G) 
/* =============================================================================== */
  /* IN : image name and the structure Graph 
     OUT : two arrays of the structure graph filled from the seed image 
- the array G->SeededNodes contains the vertices indexes of seeded nodes, 
- the array G->Labels contains the associated values to those seeds.

The multiseed format for a seed image contains 
- value 0 for unseeded pixels
- values 1, 2, ... as labeled values for seeded points. 
  */
{
  struct xvimage * seeds;

  seeds = readimage(image_name);
  if (seeds == NULL) {
      return(1);
  }

  // call function just above
  StoresMultiSeeds(seeds, G);

  freeimage(seeds);

  return (0);
}



/* =============================================================================== */
void ArgMax (DBL_TYPE ** array, int nb_arg, int size_array,  unsigned int * result) 
/* =============================================================================== */
  /* IN : two-dimentional array of doubles representing nb_arg arrays of size size_array
     OUT : result : one-dimentional array taking the argmax values of array.
  */
{
  int argmax;
  double val;
  double maxi;
  int i,j;
     for (j = 0; j < size_array; j++)
     {
       maxi=0; argmax = 0; val =1;
       for(i=0; i< nb_arg-1;i++)
	 {
	   if(array[i][j]> maxi) 
	     { 
	       maxi = array[i][j] ;
	       argmax = i;
	     }
	   val = val - array[i][j];
	 
	 }  
       if (val>maxi) argmax = i;
       result[j] = argmax; // no normalization // ((argmax)*255)/(nb_arg-1);
     } 
}

/* ======================================================================== */
void print_gradient( int ** edges, /* array of node indexes composing edges */ 
		     uint32_t * weights, /* weights values on the edges */
		     int rs, /* rowsize of the image */
		     int cs, /* colsize of the image */
		     int ds) /* depth of the image */
		    
/* ======================================================================== */
/* Prints the weight function in a image "gradient.pgm" */
{
  int i,j, k, n;
  int N = rs*cs*ds;
 int numvoisins = 4;
 if (ds>1) numvoisins = 6;
  
 struct xvimage * gradient = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  unsigned char *Temp = UCHARDATA(gradient);
      
  double w = 0;
  for (j = 0; j < N; j++)
    {
      i=0;
      w=0;
      for (k=1;k<=numvoisins;k++)
	{
	  n = neighbor_node_edge(j, k, rs, cs, ds);
	  if (n != -1)
	    {
	      w = w + weights[n];
	      i++;
	    }
	}
      Temp[j] = (uint8_t)((w*2)/i);
    }
  writeimage(gradient, (char*)"gradient.pgm");
  freeimage(gradient);
}



/* ======================================================================== */
void print_gradient_double( int ** edges, /* array of node indexes composing edges */ 
		     double * weights, /* weights values on the edges */
		     int rs, /* rowsize of the image */
		     int cs, /* colsize of the image */
		     int ds) /* depth of the image */
		    
/* ======================================================================== */
/* Prints the weight function in a image "gradient.pgm" */
{
  int i,j, k, n;
  int N = rs*cs*ds;
 int numvoisins = 4;
 if (ds>1) numvoisins = 6;
  
 struct xvimage * gradient = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  unsigned char *Temp = UCHARDATA(gradient);
      
  double w = 0;
  for (j = 0; j < N; j++)
    {
      i=0;
      w=0;
      for (k=1;k<=numvoisins;k++)
	{
	  n = neighbor_node_edge(j, k, rs, cs, ds);
	  if (n != -1)
	    {
	      w = w + weights[n];
	      i++;
	    }
	}
      Temp[j] = (uint8_t)(255*(w/i));
    }
  writeimage(gradient, (char*)"gradient.pgm");
  freeimage(gradient);
}


/* ================================================================ */
void printweights_double(DBL_TYPE * weights, /* weights value on the edges */ 
			 int rs, /* row size */
			 int cs) /*col size */
/* ================================================================ */
{ 
  int V = (cs-1)*rs;
  int i,l,j,k;
  k=0;
  int nb_edges = V+ (rs-1)*cs;
  for (i = V;i<nb_edges;i=i+(rs-1))
    {
  for (j = k;j<k+cs;j++) printf("  |      ");
      printf("\n   ");
      for (l = i ; l<i+rs-1 ; l++ )
	{
	  printf(" -- %f --", weights[l]);
	}
      printf("\n");
        for (j = k;j<k+cs;j++) printf("  |      ");
      printf("\n");
      if (k>=V) break;
      for (j = k;j<k+cs;j++)
	{
	  printf("%f       ", weights[j]);
	}
      printf("\n");
      k=j; 
      
    }
}

/* ================================================================ */
void printweights(uint32_t * weights, /* weights value on the edges */ 
		  int rs, /* row size */
		  int cs) /*col size */
/* ================================================================ */
{ 
  int V = (cs-1)*rs;
  int i,l,j,k;
  k=0;
  int nb_edges = V+ (rs-1)*cs;
  for (i = V;i<nb_edges;i=i+(rs-1))
    {
  for (j = k;j<k+cs;j++) printf("  |      ");
      printf("\n   ");
      for (l = i ; l<i+rs-1 ; l++ )
	{
	  printf(" -- %d --", weights[l]);
	}
      printf("\n");
        for (j = k;j<k+cs;j++) printf("  |      ");
      printf("\n");
      if (k>=V) break;
      for (j = k;j<k+cs;j++)
	{
	  printf("%d       ", weights[j]);
	}
      printf("\n");
      k=j; 
      
    }
}

/*=====================================================*/
struct xvimage * mult_image_value(struct xvimage * image, 
				  double n)
/*=====================================================*/
//multiply image values by a constant number n
{
    struct xvimage * output;
    int rs = rowsize(image);
    int cs = colsize(image);
    int ds = depth(image);
    int t = datatype(image);
 
    // fprintf(stderr, "data type = %d \n", t);
    int i;
    unsigned char *img, *out;
    uint32_t *imgl, *outl;
    output = allocimage(NULL, rs, cs, ds, t);
    switch (t)
    {
    case VFF_TYP_1_BYTE:       
        img = UCHARDATA(image);  
        out = UCHARDATA(output);
        for (i=0; i<rs*cs*ds;i++)
            out[i] = static_cast<unsigned char>(n*img[i]);
        break;
    case VFF_TYP_4_BYTE: 
        imgl = ULONGDATA(image); //UINT32DATA(image);
        outl = ULONGDATA(output); //UINT32DATA(output);
        for (i=0; i<rs*cs*ds;i++)
            outl[i] = static_cast<uint32_t>(n*imgl[i]);
        break;
    default: fprintf(stderr,"bad data type %d\n", t);
        return NULL;
    } /* switch (t) */


    return output; 
}





/*==============================================*/
struct xvimage * add_cross(struct xvimage * image)
/*==============================================*/
//return the inner boundary of a white object in an image 
{
  struct xvimage * output;
  unsigned char *img = UCHARDATA(image);
  int rs = rowsize(image);
  int cs = colsize(image);
  output = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  unsigned char *out = UCHARDATA(output);
  for (int i=0; i<rs*cs;i++)
    out[i] = 255;

  for (int i=2; i<rs-1;i++)
    for (int j=2; j<cs-1;j++)
      {
	if (img[i+j*rs] == 0)
	  {
	    out[i+j*rs] = 0;
	    out[i+1+j*rs] = 0;
	    out[i-1+j*rs] = 0;
	    out[i+(j-1)*rs] = 0;
	    out[i+(j+1)*rs] = 0;
	    out[i+2+j*rs] = 0;
	    out[i-2+j*rs] = 0;
	    out[i+(j-2)*rs] = 0;
	    out[i+(j+2)*rs] = 0;
	  }
      }
  return output; 
}


/*==============================================*/
struct xvimage * boundary(struct xvimage * image)
/*==============================================*/
//return the inner boundary of a white object in an image 
{
  struct xvimage * output;
  unsigned char *img = UCHARDATA(image);
  int rs = rowsize(image);
  int cs = colsize(image);
  int ds = depth(image);
  int y, i, j, k;
  
  output = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  unsigned char *out = UCHARDATA(output);
  if(ds==1){ 
    k=0;
    for (i=2; i<rs-1;i++)
      for (j=2; j<cs-1;j++)
	{
	  y = img[i+j*rs+k*rs*cs];
	  if ((img[i-1+j*rs+k*rs*cs]!= y) ||  (img[i+1+j*rs+k*rs*cs]!= y)  || (img[i+(j-1)*rs+k*rs*cs]!= y) || (img[i+(j+1)*rs+k*rs*cs]!= y) || 
	      (img[i-1+(j-1)*rs+k*rs*cs]!= y) ||  (img[i+1+(j+1)*rs+k*rs*cs]!= y)  || (img[(i-1)+(j-1)*rs+k*rs*cs]!= y) || (img[(i+1)+(j+1)*rs+k*rs*cs]!= y))
	    out[i+j*rs+k*rs*cs] = 255; 
	  
	  /*if (img[i-1+j*rs+k*rs*cs]!=img[i+j*rs+k*rs*cs] || img[i+1+j*rs+k*rs*cs]!=img[i+j*rs+k*rs*cs]  || img[i+(j-1)*rs+k*rs*cs]!=img[i+j*rs+k*rs*cs] || img[i+(j+1)*rs+k*rs*cs]!=img[i+j*rs+k*rs*cs])
	    {
	    out[i+j*rs+k*rs*cs] = 255;
	    out[i+1+j*rs+k*rs*cs] = 255;
	    out[i-1+j*rs+k*rs*cs] = 255;
	    out[i+(j-1)*rs+k*rs*cs] = 255;
	    out[i+(j+1)*rs+k*rs*cs] = 255;
	    }*/
	  else out[i+j*rs+k*rs*cs] = 0;
	}
  }
  else {
    for (i=2; i<rs-1;i++)
      for (j=2; j<cs-1;j++)
	for (k=2; k<ds-1;k++)      
	  {
	    y = img[i+j*rs+k*rs*cs];
	    if ((img[i-1+j*rs+k*rs*cs]!= y) ||  (img[i+1+j*rs+k*rs*cs]!= y)  || (img[i+(j-1)*rs+k*rs*cs]!= y) || (img[i+(j+1)*rs+k*rs*cs]!= y) || 
		(img[i-1+(j-1)*rs+k*rs*cs]!= y) ||  (img[i+1+(j+1)*rs+k*rs*cs]!= y)  || (img[(i-1)+(j-1)*rs+k*rs*cs]!= y) || (img[(i+1)+(j+1)*rs+k*rs*cs]!= y))
	      out[i+j*rs+k*rs*cs] = 255; 
	    
	    else out[i+j*rs+k*rs*cs] = 0;
	  }
  }
  return output; 
}



/* ===================================================================================================== */
void crossoverlay(struct xvimage * image_r, /* red componant of the image */
	     struct xvimage * image_v, /* green componant of the image */
	     struct xvimage * image_b, /* blue componant of the image */
	     struct xvimage * output,  /* segmentation mask image */
	     bool color,               /* true if the image is in color, false if image is in grey levels*/
	     char * name)              /* name of the overlay image */
/* ===================================================================================================== */
/* TO COMMENT */
{
  struct xvimage * tmp_r;
  struct xvimage * tmp_g;
  struct xvimage * tmp_b;
  int  rs, cs;
  rs = rowsize(image_r);
  cs = colsize(image_r);
 
  tmp_r = copyimage(image_r);
  if (color)
    {
      tmp_g = copyimage(image_v);
      tmp_b = copyimage(image_b);
    }
  else {
    tmp_g = copyimage(image_r);
    tmp_b = copyimage(image_r);}
  
  
  image_r = add_cross(output);
  lmin(image_r,tmp_r);
  image_v = add_cross(output);
  lmin(image_v,tmp_g);
  image_b = add_cross(output);
  lmin(image_b,tmp_b);
  
  writergbimage(image_r, image_v, image_b,  name);
  freeimage(image_r);
  freeimage(image_v);
  freeimage(image_b);
  
  freeimage(tmp_r);
  freeimage(tmp_g);
  freeimage(tmp_b);
    
}

/* ===================================================================================================== */
void overlay(int algo,                 /* algorithm index (1, 2, or 3)*/
	     struct xvimage * image_r, /* red componant of the image */
	     struct xvimage * image_v, /* green componant of the image */
	     struct xvimage * image_b, /* blue componant of the image */
	     struct xvimage * output,  /* segmentation mask image */
	     bool color,               /* true if the image is in color, false if image is in grey levels*/
	     char * name)              /* name of the overlay image */
/* ===================================================================================================== */
/* from an input segmentation mask and the original 2D image, prints the overlay in "overlay.pgm" */
{
  struct xvimage * tmp_r;
  struct xvimage * tmp_g;
  struct xvimage * tmp_b;
  int i, rs, cs;
  rs = rowsize(image_r);
  cs = colsize(image_r);
 
  tmp_r = copyimage(image_r);
  if (color)
    {
      tmp_g = copyimage(image_v);
      tmp_b = copyimage(image_b);
    }
  else {
    tmp_g = copyimage(image_r);
    tmp_b = copyimage(image_r);
}
  
  if (algo == 2) // red
    {
     
      image_r = boundary(output);
      lmax(image_r,tmp_r);
      image_v = boundary(output);
      linverse(image_v);
      lmin(image_v,tmp_g);
      image_b = boundary(output);
      linverse(image_b);
      lmin(image_b,tmp_b);
      
      writergbimage(image_r, image_v, image_b,  name);
      freeimage(image_r);
      freeimage(image_v);
      freeimage(image_b);
    }

  else if (algo == 3) // orange
    { 
      struct xvimage * nimage;
      image_r = boundary(output);
      nimage = mult_image_value(image_r, 1);
      lmax(nimage,tmp_r);
      unsigned char *utmp_b = UCHARDATA(tmp_b);
      unsigned char *utmp_g = UCHARDATA(tmp_g);
      image_b = boundary(output);
      unsigned char *uimage_b = UCHARDATA(image_b);
      for (i=0;i<rs*cs;i++)
	if(uimage_b[i]==255) {uimage_b[i] = 40; utmp_g[i]=90;}
	else uimage_b[i] = utmp_b[i];
      writergbimage(nimage, tmp_g, image_b, name);
      freeimage(image_r);
      freeimage(image_b);
      freeimage(nimage);
      
    }
  else if (algo == 1) // pink
    { 
      struct xvimage * nimage;
      image_r = boundary(output);
      nimage = mult_image_value(image_r, 1);
      lmax(nimage,tmp_r);
      unsigned char *utmp_b = UCHARDATA(tmp_b);
      unsigned char *utmp_g = UCHARDATA(tmp_g);
      image_b = boundary(output);
      unsigned char *uimage_b = UCHARDATA(image_b);
      for (i=0;i<rs*cs;i++)
	if(uimage_b[i]==255) {uimage_b[i] = 200; utmp_g[i]=0;}
	else uimage_b[i] = utmp_b[i];
      writergbimage(nimage, tmp_g, image_b,  name);
      freeimage(nimage);
      freeimage(image_r);
      freeimage(image_b);
    }
  else if (algo == 5) // clear blue
    {
      /*  // dark blue
	  image_b = boundary(output);
	  lmax(image_b,tmp_b);
	  image_r = boundary(output);
	  linverse(image_r);
	  lmin(image_r,tmp_r);
	  image_v = boundary(output);
	  linverse(image_v);
	  lmin(image_v,tmp_g);
	  writergbimage(image_r, image_v, image_b,  (char*)"overlay.pgm");*/
      
      struct xvimage * nimage;
      image_b = boundary(output);
      nimage = mult_image_value(image_b, 1);
      lmax(nimage,tmp_b);
      unsigned char *utmp_r = UCHARDATA(tmp_r);
      unsigned char *utmp_g = UCHARDATA(tmp_g);
      image_r = boundary(output);
      unsigned char *uimage_r = UCHARDATA(image_r);
      for (i=0;i<rs*cs;i++)
	if(uimage_r[i]==255) {uimage_r[i] = 60; utmp_g[i]=100;}
	else uimage_r[i] = utmp_r[i];
      
      writergbimage(image_r, tmp_g, nimage, name);
      freeimage(nimage);
      freeimage(image_r);
      freeimage(image_b);
    } 
  else if (algo == 6) // yellow
    { 
      struct xvimage * nimage;
      image_r = boundary(output);
      nimage = mult_image_value(image_r, 1);
      lmax(nimage,tmp_r);
      unsigned char *utmp_b = UCHARDATA(tmp_b);
      unsigned char *utmp_g = UCHARDATA(tmp_g);
      image_b = boundary(output);
      unsigned char *uimage_b = UCHARDATA(image_b);
      for (i=0;i<rs*cs;i++)
	if(uimage_b[i]==255) {uimage_b[i] = 0; utmp_g[i]=255;} //180
	else uimage_b[i] = utmp_b[i];
      writergbimage(nimage, tmp_g, image_b, name);
      freeimage(nimage);
      freeimage(image_b);
      freeimage(image_r);
    }


  freeimage(tmp_r);
  freeimage(tmp_g);
  freeimage(tmp_b);
    
} 


/* ===================================================================================================== */
void overlay3D(struct xvimage * image_r, /* red componant of the image */
	     struct xvimage * image_v, /* green componant of the image */
	     struct xvimage * image_b, /* blue componant of the image */
	     struct xvimage * output,  /* segmentation mask image */
	     char * name)              /* name of the overlay image */
/* ===================================================================================================== */
/* from an input segmentation mask and the original 3D image, prints the overlay in "overlay.pgm" */
{
  struct xvimage * tmp_r;
  struct xvimage * tmp_g;
  struct xvimage * tmp_b;
  int rs, cs,ds;
  rs = rowsize(image_r);
  cs = colsize(image_r);
  ds = depth(image_r);

  tmp_r = copyimage(image_r);
  tmp_g = copyimage(image_r);
  tmp_b = copyimage(image_r);
  
  image_r = boundary(output);
  printf("%d %d %d \n",rs,cs,ds);
  printf("%d %d %d \n",rowsize(tmp_r),colsize(tmp_r),depth(tmp_r));
  writeimage(image_r, "_1.pgm");
  writeimage(tmp_r, "_2.pgm");
  lmax(image_r,tmp_r);
  image_v = boundary(output);
  linverse(image_v);
  lmin(image_v,tmp_g);
  image_b = boundary(output);
  linverse(image_b);
  lmin(image_b,tmp_b);
  /* writeimage(image_r, "boundary1.pgm");
   writeimage(image_v, "boundary2.pgm");
   writeimage(image_b, "boundary3.pgm");*/
  //writergbimage(image_r, image_v, image_b,  name);
   
  FILE * fd;
  int y;
  fd = fopen(name,"w+");
  if (fd == NULL)
	{
	  printf("erreur fichier\n");
	  exit(-1);
	}
  
  fprintf(fd,"P2 \n");
  fprintf(fd,"%d %d %d \n", rs, cs, ds);
  fprintf(fd,"255 \n");
  //int ret = fwrite(seeds, sizeof(unsigned char), rs*cs*ds, fd);

  freeimage(tmp_r);
  freeimage(tmp_g);
  freeimage(tmp_b);

 unsigned char *tr = UCHARDATA(image_r);
 unsigned char *tg = UCHARDATA(image_v);
 unsigned char *tb = UCHARDATA(image_b);
 unsigned char *file = ( unsigned char *) malloc(rs*cs*ds*4*sizeof(unsigned char));
 for( y=0; y<rs*cs*ds;y++)
   {   
     file[4*y]=tr[y];
     file[4*y+1]=tg[y];
     file[4*y+2]=tb[y];
     file[4*y+3]=tb[y];
      }

fwrite(file, sizeof(unsigned char), rs*cs*ds*4, fd);

  fflush(fd);
  fclose(fd);



  freeimage(image_r);
  freeimage(image_v);
  freeimage(image_b);
   

    
}
