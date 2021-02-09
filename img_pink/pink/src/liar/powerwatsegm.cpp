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

/*! \file powerwatsegm.c
\defgroup segm Segmentation

\brief Implements several segmentation algorithms optimizing power watershed energies for 2D and 3D images 

<B>Usage:</B> ./ powerwatsegm.exe -a algo_number -i image_name -s seed_name

<B>Description: see the segmentation page </B>

\ingroup segm

\author Camille Couprie
\changes Hugues Talbot, Python version
*/


#include <sys/types.h>
#ifndef _WINDOWS
# include <sys/time.h>
#endif /* _WINDOWS */
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#ifndef _WINDOWS
# include <unistd.h>
#endif /* _WINDOWS */

#include "graph_utils.h"
#include "mccodimage.h"
#include "mcimage.h"
#include "cccodimage.hxx"
#include "lMSF.h"
#include "powerwatsegm.h"
#include "MSF_RW.h"
// Shortest-path forest, not needed perhaps
// #include "SPF.h"
// Graph-cut library, not needed either
// #include "graph.h"
#include "image_toolbox.h"
// #include "argv.h"
// #include "random_walker.h"



// colour Power Watershed
int compute_power_watershed_col(struct xvimage *image_r,
                                struct xvimage *image_g,
                                struct xvimage *image_b,
                                struct xvimage *seeds,
                                bool geod, // perform geodesic reconstruction or not ?
                                bool mult, // multiple or binary seeds, grabcut style ?
                                struct xvimage *output // must be already allocated when calling
    )
{
    struct graph *G;
    bool          quicksort = false, color=false, color3D=false;
    int           rs, cs, ds, degreMax, doubleweights ;
    int           i, j;
    double        beta;

    // image dimensions
    rs = rowsize(image_r);
    cs = colsize(image_r);
    ds = depth(image_r);

    beta= 500;       // to be documented
    doubleweights=0; // weights are integer or double.
        
    //Building the graph
    G = (struct graph*)malloc(sizeof(struct graph));
    if (ds == 1) degreMax = 4; else degreMax = 6; 
    Allocate_Graph(G, 
                   rs*cs*ds,/*Number of nodes*/ 
                   rs*cs*ds, /*Max Number of seeded nodes*/ 
                   ds*rs*(cs-1)+ds*(rs-1)*cs+(ds-1)*cs*rs, /*Number of edges*/ 
                   degreMax, 
                   doubleweights);
  
    // G->levels = ds;
    compute_edges(G, rs, cs, ds);

    // colour image ?
    if ((image_g != NULL) && (image_b != NULL)) {
        color = true;
        // does this make sense (HT) ?
        if (ds > 1)
            color3D = true;
    }
    

    // input the seed image
    if (mult)
        StoresMultiSeeds(seeds, G);
    
    // The PWSH algorithm

    G->RecWeights = (uint32_t *)calloc(G->M,sizeof(uint32_t));
    uint32_t max_weight = 255;

    // this needs to be changed.
    // 
    if (color == true) 
	color_standard_weights_PW( image_r, image_g, image_b , &max_weight, G);
    else if (color3D== true)
        color_3D_weights_PW( image_r, image_g, image_b , &max_weight,  G);
    else grey_weights_PW(image_r,  G, rs, cs, ds);
    
      // for (i=0;i<G->M;i++)
      //	 printf("%d poids %d poids reconstruits %d \n",i,  G->Weights[i], G->RecWeights[i]); 
    Allocate_Graph_Solution(G);
     
    for (i=0;i<G->S;i++)
	for (j=0;j<G->P;j++)
        {
	    if (G->Labels[i]==j+1)
                G->Solution[j][G->SeededNodes[i]] = 1;
	    else G->Solution[j][G->SeededNodes[i]] = 0;
        }
    
    if (geod ==true)
	PowerWatershed_q2( G->RecWeights, max_weight, G, false);
    else
	PowerWatershed_q2( G->Weights,max_weight,G, false);
    
      // print_gradient(G->Edges, G->RecWeights, rs, cs, ds );
    //writing results
    //*output= allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    unsigned int *Temp = ULONGDATA(output); // this is really 32-bit unsigned int
    ArgMax(G->Solution, G->P+1, G->N, Temp) ;
    
    free(G->RecWeights);
    free(G->Weights);
    for (i=0;i<2;i++) 
	free(G->Edges[i]); 
    free(G->Edges);

    return 0;
}


int compute_power_watershed_bw(struct xvimage *image_bw,
                               struct xvimage *seeds,
                               bool geod, // perform geodesic reconstruction or not ?
                               bool mult, // multiple or binary seeds, grabcut style ?
                               struct xvimage *output // must be already allocated when calling
    )
{
    return compute_power_watershed_col(image_bw, NULL, NULL, seeds, geod, mult, output);
}

/// This is old code for reference only at this stage
///
/// Hugues Talbot 2011-10-06

#if 0


void ProcessArgs(int ,char **, char *, char *, char *, char *, char *, int*, bool*, bool*);

/* =============================================================== */
int powerwatsegm_main(int argc, char **argv) 
/* =============================================================== */
{
  char *image_name, *seeds_name, *multi_seeds_name, *overlay_name, *output_name;
  int algo, rs, cs, ds, degreMax;
  bool mult, geod, quicksort = false, color=false, color3D=false;
  int doubleweights;
  double beta;
  int i,j, size;
  struct xvimage *image_r, *image_v, *image_b, *output;
  struct graph *G;
  int t1=clock();
  
  image_name       = AllocFileName(image_name); 
  seeds_name       = AllocFileName(seeds_name); 
  multi_seeds_name = AllocFileName(multi_seeds_name); 
  overlay_name     = AllocFileName(overlay_name); 
  output_name      = AllocFileName(output_name);
  ProcessArgs(argc, argv, image_name, seeds_name, multi_seeds_name, overlay_name, output_name, &algo, &mult, &geod);
  beta= 500;
  doubleweights=0;
  if ((algo ==5)||(algo ==6)) doubleweights = 2;

  output = readheader(seeds_name);
  rs = rowsize(output);
  cs = colsize(output);
  ds = depth(output);
  fprintf(stderr,"ds=%d, cs=%d, ds=%d\n",rs,cs,ds);

  //Building the graph
  G = (struct graph*)malloc(sizeof(struct graph));
  if (ds == 1) degreMax = 4; else degreMax = 6; 
  Allocate_Graph(G, 
		 rs*cs*ds,/*Number of nodes*/ 
		 rs*cs*ds, /*Max Number of seeded nodes*/ 
		 ds*rs*(cs-1)+ds*(rs-1)*cs+(ds-1)*cs*rs, /*Number of edges*/ 
		 degreMax, 
		 doubleweights);
  
  // G->levels = ds;
  compute_edges(G, rs, cs, ds);

  // Reading the image
  size = strlen(image_name);
  if (strcmp(&image_name[size-3], "pgm")==0) // grey levels image
    image_r = readimage(image_name);
  else if (strcmp(&image_name[size-3], "ppm")==0) // color 2D image
    {
      color = true;
      readrgbimage(image_name, &image_r, &image_v, &image_b);
    }
  else
    {
      color3D = true;
      read3Drgbimage(image_name, &image_r, &image_v, &image_b);
    }
  

  // Reading the seed image 
  if (mult == true) StoresMultiSeeds(seeds_name, G);
  else StoresGrabCutFormatSeeds(seeds_name, G);

  

  // Execution of an algorithm
  
   if (algo == 1) // Kruskal
    {
      uint32_t max_weight = 255;
      if (color == true) 
	max_weight = color_standard_weights( image_name , G->Weights, G->SeededNodes, G->S, geod,  G);
      //else if (color3D== true)
	//	max_weight = color_3D_weights( image_name , G->Weights, G->SeededNodes, G->S, geod,  G);
      else
	grey_weights(image_name, geod, G, rs, cs, ds);
      output = MSF_Kruskal(G, max_weight, rs, cs, ds);
      free(G->Weights);
    }
 
  else if (algo == 3) // Prim RB tree
    {  
      if (color == true) 
        color_standard_weights( image_name , G->Weights, G->SeededNodes, G->S, geod, G);
      // else if (color3D== true)
       //	max_weight = color_3D_weights( image_name , G->Weights, G->SeededNodes, G->S, geod,  G);
      else
	grey_weights(image_name, geod,  G, rs, cs, ds);
      output = MSF_Prim(G,rs, cs, ds, false);
      free(G->Weights);
    }

 else if (algo == 2) // Kruskal & RW on plateaus multiseeds linear time
    {
      struct xvimage * img_proba;     
      G->RecWeights = (uint32_t *)calloc(G->M,sizeof(uint32_t));
      uint32_t max_weight = 255;
      if (color == true) 
	color_standard_weights_PW( image_name , &max_weight, G);
      else if (color3D== true)
        color_3D_weights_PW( image_name , &max_weight,  G);
      else grey_weights_PW(image_name,  G, rs, cs, ds);
    
      // for (i=0;i<G->M;i++)
      //	 printf("%d poids %d poids reconstruits %d \n",i,  G->Weights[i], G->RecWeights[i]); 
      Allocate_Graph_Solution(G);
     
      for (i=0;i<G->S;i++)
	for (j=0;j<G->P;j++)
	  {
	    if (G->Labels[i]==j+1)
	      G->Solution[j][G->SeededNodes[i]] = 1;
	    else G->Solution[j][G->SeededNodes[i]] = 0;
	  }
      
      if (geod ==true)
	PowerWatershed_q2( G->RecWeights, max_weight, G, false);
      else
	PowerWatershed_q2( G->Weights,max_weight,G, false);

      // print_gradient(G->Edges, G->RecWeights, rs, cs, ds );
 //writing results
      output= allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  unsigned char *Temp = UCHARDATA(output);
  ArgMax(G->Solution, G->P+1, G->N, Temp) ;
  img_proba = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  unsigned char *Temp2 = UCHARDATA(img_proba);
   
  for (j = 0; j < G->N; j++)
    Temp2[j] = (unsigned char)(255-255*G->Solution[0][j]); 
  for (i=0; i<G->P; i++)  free(G->Solution[i]);
  free(G->Solution);

      writeimage(img_proba, (char*)"proba.pgm");
      freeimage(img_proba);
      free(G->RecWeights);
      free(G->Weights);
      for (i=0;i<2;i++) 
	free(G->Edges[i]); 
      free(G->Edges);
    }
 else if (algo == 4) // random walker with equal weights
    {
       output = RandomWalkerComplet(G->Edges, rs,cs,ds, G->SeededNodes, G->Labels, G->S, G->P+1);
    } 


 else if (algo == 5) // graph cuts 
    {
      beta = 500;
      // double * G->Weights = (double *)malloc(sizeof(double)*G->M);
      if (color == true) 
	//	color_gaussian_weights_double(image_name , beta, weights, G->Edges, G->SeededNodes, G->S);
	color_gaussian_weights_double(image_name , beta, G->dWeights, G, G->SeededNodes, G->S);
      else 
	grey_weights_double(image_name, beta, G->dWeights, G,G->SeededNodes, G->S);
      //print_gradient_double(G->Edges, G->DWeights, rs, cs, ds);
      output = Graph_Cuts(G->Edges,G->dWeights, G->SeededNodes, G->Labels, G->S,rs, cs, ds);
      free(G->dWeights);
    } 

    else if (algo == 6) // SPF 
    { 
      beta= 1000;
      //double * G->Weights = (double *)malloc(sizeof(double)*G->M);
      if (color == true) 
        color_gaussian_weights_double( image_name , beta, G->dWeights, G, G->SeededNodes, G->S);
      else  grey_weights_double(image_name, beta, G->dWeights, G, G->SeededNodes, G->S);
      for (i=0;i<G->M;i++)
	G->dWeights[i] = (1.0/G->dWeights[i]);
      //  printweights_double(weights, rs, cs, 0); 
   
      output = SPF_double(G->Edges,G->dWeights, G->SeededNodes, G->Labels, G->S, rs, cs, ds);
      free(G->DWeights);
      }

  int t2=clock();
  assert(output != NULL);
 
  free(G->SeededNodes);
  free(G->Labels);
  if (strcmp(output_name, "\0")==0)
      output_name =(char*)"mask.pgm"; 
  writeimage(output, output_name);

  // overlay for 2D images only
  if (ds==1){
     if (strcmp(overlay_name , "\0")==0)
       overlay_name =(char*)"overlay.ppm"; 
     overlay(algo, image_r, image_v, image_b, output, color, overlay_name);
  }
  else {
     if (strcmp(overlay_name , "\0")==0)
       overlay_name =(char*)"overlay3D.ppm"; 
     overlay3D(image_r, image_v, image_b, output, overlay_name);
  }
  if (color) 
    {
      freeimage(image_v);
      freeimage(image_b);
    }
  
  freeimage(image_r);
  freeimage(output);
 
  printf("Computation time : %.6lf seconds elapsed\n", ((double)t2-t1)/CLOCKS_PER_SEC);
  return 0;
} 


	
	
/* ============================================================================================ */
void ProcessArgs(int argc,
		 char **argv, 
		 char *image_name,
		 char *seeds_name,
		 char *multi_seeds_name,  
		 char *overlay_name,
		 char *output_name, 
		 int  *algo, 
		 bool *mult,
		 bool *geod)
/* ============================================================================================ */
{
 
  char *image_name_t, *seeds_name_t, *multi_seeds_name_t, *overlay_name_t, *output_name_t;

 image_name_t = AllocFileName(image_name_t); 
 seeds_name_t = AllocFileName(seeds_name_t); 
 multi_seeds_name_t = AllocFileName(multi_seeds_name_t); 
 overlay_name_t = AllocFileName(overlay_name_t); 
 output_name_t = AllocFileName(output_name_t);

  *algo =-1;
  *mult = false;
  *geod = false;
  argv_t args[] = {
    { 'a', 0L, ARGV_INT, &(*algo), (char*)" 1|2|3", (char*)"algo: Kruskal(1) PW q=2(2) Prim(3)" },
    { 'i', 0L, ARGV_CHAR_P, &image_name_t, (char*)" *.pgm |*.ppm",  (char*)"image_name" },
    { 's', 0L, ARGV_CHAR_P, &seeds_name_t, (char*)" *.pgm",  (char*)"seed_name (see README)" },
    { 'm', 0L, ARGV_CHAR_P, &multi_seeds_name_t, (char*)" *.pgm",  (char*)"multi_seed_name (see README)" },
    { 'g', 0L, ARGV_BOOL, &(*geod), (char*)" 0|1", (char*)"geod: reconstruction of the weights(1)" },
    { 'o', 0L, ARGV_CHAR_P, &output_name_t, (char*)" *.pgm",  (char*)"output mask name" },
    { 'v', 0L, ARGV_CHAR_P, &overlay_name_t, (char*)" *.ppm",  (char*)"output overlay name" },
    { ARGV_LAST, 0L, 0, 0L, 0L, 0L }
  };
  argv_process(args, argc, argv);
  
  strcpy(image_name, image_name_t);
  strcpy(seeds_name, seeds_name_t);
  strcpy(multi_seeds_name , multi_seeds_name_t);
  strcpy(output_name, output_name_t);
  strcpy(overlay_name, overlay_name_t);
  if(strcmp(seeds_name_t, "\0")==0) 
    {
      strcpy (seeds_name, multi_seeds_name_t);
      *mult = true;
    }

  if ((*algo==-1) ||(strcmp(image_name_t, "\0")==0)||(strcmp(seeds_name, "\0")==0))
    {
      fprintf(stderr, "usage: %s -a algo -i image(.pgm or .ppm) <-s seeds_2_labels.pgm | -m seeds_m_labels.pgm > \n", argv[0]);
      fprintf(stderr, "options : [-g geod] [-o output mask name] [-v image overlay name]\n");
      fprintf(stderr, "algo : 1 : Kruskal \n");
      fprintf(stderr, "       2 : PW q=2 \n");
      fprintf(stderr, "       3 : Prim \n");
      fprintf(stderr, "       5 : Graph cuts \n");
      fprintf(stderr, "       6 : Shortest Paths \n");
      fprintf(stderr, "seeds[MULT].pgm  (see the README for more precision) \n");
      fprintf(stderr, "geod : 1 : geodesic reconstruction \n");
      fprintf(stderr, "       0 : no reconstruction \n");
      exit(1);
    }
}
#endif // 0 : dead code
