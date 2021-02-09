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
#ifndef CCCODIMAGE_H
#define CCCODIMAGE_H



#include <gageodesic.hxx>
#include <image_toolbox.h>
#include <graph_utils.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#ifndef _WINDOWS
# include <unistd.h>
#endif /* _WINDOWS */

#include "mccodimage.h"
#include "mcimage.h"
#include "mcindic.h"
#include "mclifo.h"
#include "mcutil.h"
#ifndef _WINDOWS
# include "stdbool.h"
#endif /* _WINDOWS */
#include "lMSF.h"
#include "union_find.h"

void print_incidence_graph(struct graph *G) ;
void fill_incidence_graph(struct graph *G,
			  int rs,
			  int cs) ;
int neighbor_edge(int i,  /* edge index */
		  int k,  /* number of the desired neighbor of edge i */
		  int rs, /* rowsize of the image */
		  int cs, /* colsize of the image */
		  int ds); /* depth of the image */
int neighbor_node_edge( int i,  /* node index */
			int k,  /* number of the desired edge neighbor of node i */
			int rs, /* rowsize of the image */
			int cs, /* colsize of the image */
			int ds); /* depth of the image */

int neighbor( int i,  /* node index */
	      int k,  /* number of the desired neighbor node of node i */
	      int rs, /* rowsize of the image */
	      int cs, /* colsize of the image */
	      int ds); /* depth of the image */
void compute_edges(  struct graph *G,
		     int rs, 
		     int cs,
		     int ds);
void grey_weights( char * image_name , /* image name */
		   bool geod,          /* if true, geodesic reconstruction is performed */
		   struct graph *G, 
		   int rs, 
		   int cs,
		   int ds)  ;

void grey_weights_PW(struct xvimage *glimage, /*IN : image name */  
			   struct graph *G, 
			   int rs, 
			   int cs,
			   int ds)  ;
void grey_weights_PW(char * image_name, /*IN : image name */  
			   struct graph *G, 
			   int rs, 
			   int cs,
			   int ds)  ;
void color_standard_weights_PW( char * image_name , /* IN : image name */
				      uint32_t * maxi,         /* OUT : the maximum weight value */
				      struct graph *G);

void color_standard_weights_PW( struct xvimage *image_r , /* IN : image name */
                                struct xvimage *image_g,
                                struct xvimage *image_b,
				      uint32_t * maxi,         /* OUT : the maximum weight value */
                                struct graph *G);

int color_standard_weights( char * image_name , /* image name */
			    uint32_t * weights, /* array to store the values of weights on the edges */
			    
			    uint32_t * seeds,        /* array of seeded nodes indexes */
			    int size_seeds,     /* nb of seeded nodes */
			    bool geod,          /* if true, geodesic reconstruction is performed */
			    struct graph *G);

void color_gaussian_weights_double( char * image_name ,
				  double beta, 
				  double * weights, 
				    struct graph *G,
				  uint32_t * seeds,
				    int size_seeds );

void grey_weights_double( char * image_name, 
			  double beta,
			  double * weights, 
			  struct graph *G,
			  uint32_t * seeds,
			  int size_seeds);

void read3Drgbimage(const char * image_name, struct xvimage **image_r, struct xvimage **image_g, struct xvimage **image_b );

void color_3D_weights_PW( struct xvimage *image_R , /* IN : image name */
                          struct xvimage *image_G,
                          struct xvimage *image_B,
			  uint32_t * maxi,         /* OUT : the maximum weight value */
		     struct graph *G);
void color_3D_weights_PW( char * image_name , /* IN : image name */
			  uint32_t * maxi,         /* OUT : the maximum weight value */
                          struct graph *G);

/*
void print_incidence_graph( int ** edges, 
			    struct graph G) ;
extern int neighbor_node_edge( int indice, int num, int rs, int cs, int ds);
extern  int neighbor( int indice, int num, int rs, int cs, int ds);
extern int neighbor_edge(int indice, int num, int rs, int cs, int ds);

extern uint32_t * color_standard_weights_PW( char * image_name , uint32_t* weights, int ** edges, int * seeds,  int size_seeds, uint32_t * max_weight , bool quicksort, struct graph G );
extern int color_standard_weights( char * image_name ,  uint32_t * weights, int ** edges, int * seeds, int size_seeds,   bool geod, bool quicksort,  struct graph G );
extern uint32_t * grey_weights_PW( char * image_name , int ** edges, int * seeds, int size_seeds,  uint32_t * weights , bool quicksort, struct graph G);
extern void grey_weights( char * image_name ,  uint32_t * weights, int ** edges, int * seeds,   int size_seeds,  bool geod, bool quicksort, struct graph G);
 
void color_standard_weights_double( char * image_name , double beta, double * weights, int ** edges,int * seeds,int size_seeds);
void color_gaussian_weights_double( char * image_name , double beta, double * weights, int ** edges,int * seeds,int size_seeds);
extern void grey_weights_double( char * image_name , double beta,double * weights, int ** edges,int * seeds, int size_seeds);
void compute_edges(int ** edges,  struct graph G, bool);
void print_gradient(int ** edges,  uint32_t * weights,  int rs,  int cs,  int ds);
extern struct xvimage * mult_image_value(struct xvimage * image, double n);
extern struct xvimage * boundary(struct xvimage * image);




void fill_incidence_graph( int ** edges, 
struct graph G) ;*/
#endif
