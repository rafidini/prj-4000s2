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

#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif


  //  const double epsilon=0.00001;
  // #define MAX_DEGREE 3000
  // #define SIZE_MAX_PLATEAU 1800000

typedef float DBL_TYPE;

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mclifo.h>




/*!
  \file graph_utils.h
  \brief Generic graph structures

 Solution = argmin_x { sum_{e_{ij}=1...M} {w_{ij}^p|x_i-x_j|^q + sum_{v_i=1...N} {w_{ij}^p|x_i-y_i|^q} 
y_i 
 <B>
 Description: A graph structure is composed of 
 </B>
*/

struct graph {
  int       N;           /*! nb_vertices */
  int       M;           /*! nb_edges */
  int       S;           /*! nb of seeded nodes */
  int       P;           /*! Number of different problems to solve on the graph, 
			    i.e. the number of labels in a multilabel segmentation */
  int        levels;     /*! temporary usefull for anisotropic diffusion only, for the rest, equal to 1*/
 
  int        DegreMax;      /*! maximum number of neighbors for all vertices */
  uint32_t** Edges;         /*! array of node indexes composing edges */
  Lifo**     Neighbors;     /*! array of lists of edge neighbors of each node */
 
  uint32_t*  SeededNodes;   /*! list of seeded nodes*/
  uint8_t*   Labels;        /*! list of labels corresponding to seeded nodes */

  uint32_t*  Weights;       /*! edges weights (integers)*/
  DBL_TYPE*  DWeights;      /*! double or float edges weights */
  double*    dWeights;      /*! double edges weights */
  uint32_t*  RecWeights;    /*! integer reconstructed weights relatively to the seeds */
 
  DBL_TYPE** Solution;      /*! proba value assigned to each node */
};	

void Allocate_Graph_Solution( struct graph *G /*Graph structure*/
); 

void Allocate_Graph( struct graph *G, /*Graph structure*/
		     int nb_nodes,  /*Number of nodes*/ 
		     int nb_max_seeded_nodes, /*Max Number of seeded nodes*/
		     int nb_edges, /*Number of edges*/ 
		     int degree /*Max degree*/,
		     int doubleweights );/*0 : weights are integers */
                                        /*1 : weights are DBL_TYPE (may be float) */
                                        /*2 : weights are double */

void Print_Graph_Content( struct graph *G, /*Graph structure*/
			  int doubleweights); /*0 : weights are integers */
                                             /*1 : weights are DBL_TYPE (may be float) */
                                             /*2 : weights are double */



void Allocate_Minimum_Graph( struct graph *G, /*Graph structure*/
			     int nb_nodes,  /*Number of nodes*/ 
			     int nb_max_seeded_nodes, /*Max Number of seeded nodes*/
			     int nb_edges); /*Number of edges*/ 

void Free_Graph( struct graph *G, /*Graph structure*/
		 int nb_max_seeded_nodes, /*Max Number of seeded nodes*/
		 int doubleweights); 

void Free_Minimum_Graph( struct graph *G, /*Graph structure*/
			 int nb_max_seeded_nodes);


void AddEdgeInt( struct graph* G, int node1, int node2, uint32_t weight, int index_edge);
void Free_Partial_Graph( struct graph *G, int, /*Graph structure*/
			 int doubleweights);
#ifdef __cplusplus
}
#endif

    

#endif // GRAPH_UTILS_H
