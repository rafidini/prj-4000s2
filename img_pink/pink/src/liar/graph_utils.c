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

/*! \file graph_utils.c

\brief Generic graph structure basic functions

<B> Description : Allocation, initializations and Free operations for the graph elements 
 </B>

\author Camille Couprie
*/

#include <graph_utils.h>




/* =============================================================== */
void Allocate_Minimum_Graph( struct graph *G, /*Graph structure*/
			     int nb_nodes,  /*Number of nodes*/ 
			     int nb_max_seeded_nodes, /*Max Number of seeded nodes*/
			     int nb_edges) /*Number of edges*/ 
/* =============================================================== */
{
  G->N = nb_nodes;
  G->M = nb_edges;
  G->S = nb_max_seeded_nodes;
  G->SeededNodes = (uint32_t*)malloc(nb_max_seeded_nodes *sizeof(uint32_t));
  G->Labels = (uint8_t*)malloc(nb_max_seeded_nodes *sizeof(uint8_t));
}


/* =============================================================== */
void Free_Minimum_Graph( struct graph *G, /*Graph structure*/
			 int nb_max_seeded_nodes) /*Max Number of seeded nodes*/
/* =============================================================== */
{
  free(G->SeededNodes);
  free(G->Labels);
}


/* =============================================================== */
void Allocate_Graph( struct graph *G, /*Graph structure*/
		     int nb_nodes,  /*Number of nodes*/ 
		     int nb_max_seeded_nodes, /*Max Number of seeded nodes*/
		     int nb_edges, /*Number of edges*/ 
		     int degree, /*Max degree*/
		     int doubleweights) /*0 : weights are integers */
                                        /*1 : weights are DBL_TYPE (may be float) */
                                        /*2 : weights are double */
/* =============================================================== */
{
  int i;
  Allocate_Minimum_Graph(G, nb_nodes,  nb_max_seeded_nodes, nb_edges);

  G->DegreMax = degree;
  G->Neighbors =(Lifo **)malloc(G->N*sizeof(Lifo*));
  G->Edges =  (uint32_t**)malloc(2*sizeof(uint32_t*));
  for(i=0;i<2;i++) G->Edges[i] = (uint32_t*)malloc(G->M*sizeof(uint32_t));
  
  if (doubleweights==2) 
    G->dWeights = (double *)malloc(sizeof(double)*G->M);
  else if (doubleweights==1) 
    G->DWeights = (DBL_TYPE *)malloc(sizeof(DBL_TYPE)*G->M);
  else
    G->Weights = (uint32_t *)malloc(sizeof(uint32_t)*G->M);
 
  /*initializing the incidence array G->Neighbors*/
  for (i=0;i<G->N;i++)
    G->Neighbors[i] = CreeLifoVide(G->DegreMax);
}


/* =============================================================== */
void Free_Graph( struct graph *G, /*Graph structure*/
		 int nb_max_seeded_nodes, /*Max Number of seeded nodes*/
		 int doubleweights) /*0 : weights are integers */
                                    /*1 : weights are DBL_TYPE (may be float) */
                                    /*2 : weights are double */
/* =============================================================== */
{
  int i;
  Free_Minimum_Graph(G, nb_max_seeded_nodes);

  for(i=0;i<G->N;i++) 
  LifoTermine(G->Neighbors[i]); 
  free(G->Neighbors);

  for (i=0;i<2;i++) 
    free(G->Edges[i]);
  free(G->Edges);
 
  if (doubleweights==2) 
    free(G->dWeights);
  else if (doubleweights==1) 
    free(G->DWeights);
  else
    free(G->Weights);
}


/* =============================================================== */
void Free_Partial_Graph( struct graph *G, /*Graph structure*/
			 int nb_max_seeded_nodes,
			 int doubleweights) /*0 : weights are integers */
                                    /*1 : weights are DBL_TYPE (may be float) */
                                    /*2 : weights are double */
/* =============================================================== */
{
  int i;
 
  G->SeededNodes = (uint32_t*) realloc(G->SeededNodes ,sizeof(uint32_t)*G->S);
  G->Labels = (uint8_t*) realloc(G->Labels ,sizeof(uint8_t)*G->S);


   for(i=G->N+1; i<nb_max_seeded_nodes;i++) 
    G->Neighbors[i] = (Lifo *)realloc( G->Neighbors[i] ,0);
 
    G->Neighbors =(Lifo **)realloc(G->Neighbors, G->N*sizeof(Lifo*));
  
  for (i=0;i<2;i++) 
    G->Edges[i] = (uint32_t*)realloc(G->Edges[i] ,sizeof(uint32_t)* G->M);
  
 
  if (doubleweights==2) 
    G->dWeights = (double*) realloc(G->Weights ,sizeof(double)*G->M);
  else if (doubleweights==1) 
    G->DWeights = (DBL_TYPE*) realloc(G->Weights ,sizeof(DBL_TYPE)*G->M);
  else
   G->Weights = (uint32_t*) realloc(G->Weights ,sizeof(uint32_t)*G->M);
}





/* =============================================================== */
void Allocate_Graph_Solution( struct graph *G) /*Graph structure*/
			    
/* =============================================================== */
{
  int i, j;
  G->Solution = (DBL_TYPE **)malloc((G->P) *sizeof(DBL_TYPE*));
  for (i=0; i<G->P; i++) 
    {
      G->Solution[i]= (DBL_TYPE *)malloc(G->N *sizeof(DBL_TYPE));
      for (j=0; j<G->N; j++) G->Solution[i][j]=-1;
    }
}


/* =============================================================== */
void Print_Graph_Content( struct graph *G, /*Graph structure*/
			  int doubleweights) /*0 : weights are integers */
                                             /*1 : weights are DBL_TYPE (may be float) */
                                             /*2 : weights are double */
/* =============================================================== */
{
  int i, j;
  fprintf(stderr,"************************************\n");
  fprintf(stderr,"Nb of nodes %d \n", G->N);
  fprintf(stderr,"Nb of edges %d \n", G->M);
  fprintf(stderr,"Nb of seeded nodes %d \n", G->S);
  fprintf(stderr,"Maximum Degree %d \n\n", G->DegreMax);

  fprintf(stderr,"List of edges : \n");
  for(i=0;i<G->M;i++)
    fprintf(stderr, "edge %d : (node %d, node %d) \n", i, G->Edges[0][i], G->Edges[1][i] );
  fprintf(stderr,"************************************\n", G->N);

    fprintf(stderr,"List of edge neighbors for each node : \n");
  for(i=0;i<G->N;i++)
    {
      fprintf(stderr, "node %d has for neighbors edges ", i);
      for(j=0;j<G->Neighbors[i]->Sp;j++)
	fprintf(stderr, " %d, ", G->Neighbors[i]->Pts[j]);
      fprintf(stderr, "\n ");
    }
  fprintf(stderr,"************************************\n");

  fprintf(stderr,"Edge weights : \n");
  if (doubleweights==2) 
    for(i=0;i<G->M;i++)
      fprintf(stderr, "weight of edge %d = %f  \n", i, G->dWeights[i]);
  else if (doubleweights==1) 
    for(i=0;i<G->M;i++)
      fprintf(stderr, "weight of edge %d = %f  \n", i, G->DWeights[i]);
  else
    for(i=0;i<G->M;i++)
      fprintf(stderr, "weight of edge %d = %d  \n",i,  G->Weights[i]);
  fprintf(stderr,"************************************\n");

 fprintf(stderr,"List of seeded nodes : \n");
  for(i=0;i<G->S;i++)
    fprintf(stderr, "label ( node %d) = %d \n", G->SeededNodes[i], G->Labels[i]);
  fprintf(stderr,"************************************\n");

  
}



/* ======================================================================================================== */
void AddEdgeInt( struct graph* G, int node1, int node2, uint32_t weight, int index_edge)
/* ======================================================================================================== */
{
  G->Edges[0][index_edge]= node1;
  G->Edges[1][index_edge]= node2;
  LifoPush(G->Neighbors[node1],index_edge);
  LifoPush(G->Neighbors[node2],index_edge);
  G->Weights[index_edge] = weight;
}
