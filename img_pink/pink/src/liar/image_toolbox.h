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
#include<graph_utils.h>
#include<powerwatsegm.h>
void StoresGrabCutFormatSeeds(char * image_name, struct graph*) ;

int StoresMultiSeeds(char * image_name,struct graph* ) ;
int StoresMultiSeeds(struct xvimage *seeds, struct graph *G);

void ComputeSeedsSurfRec(char * image_name, struct graph *G) ;
void ArgMax (DBL_TYPE ** array, int nb_arg, int size_array,  unsigned int * result) ;
 void printweights(uint32_t * weights, int rs, int cs);
 void printweights_double(DBL_TYPE * weights, int rs, int cs);
 void overlay(int algo, struct xvimage * image_r,  struct xvimage * image_v,  struct xvimage * image_b, struct xvimage * output,  bool color, char * name);
struct xvimage * mult_image_value(struct xvimage * image, 
				  double n);
void print_gradient_double( int ** edges, /* array of node indexes composing edges */ 
		     double * weights, /* weights values on the edges */
		     int rs, /* rowsize of the image */
		     int cs, /* colsize of the image */
			    int ds); /* depth of the image */
char * AllocFileName(char * name);
void print_gradient( int ** edges, /* array of node indexes composing edges */ 
		     uint32_t * weights, /* weights values on the edges */
		     int rs, /* rowsize of the image */
		     int cs, /* colsize of the image */
		     int ds); /* depth of the image */
void crossoverlay(struct xvimage * image_r, /* red componant of the image */
	     struct xvimage * image_v, /* green componant of the image */
	     struct xvimage * image_b, /* blue componant of the image */
	     struct xvimage * output,  /* segmentation mask image */
	     bool color,               /* true if the image is in color, false if image is in grey levels*/
		  char * name)  ;
void overlay3D(struct xvimage * image_r, /* red componant of the image */
	     struct xvimage * image_v, /* green componant of the image */
	     struct xvimage * image_b, /* blue componant of the image */
	     struct xvimage * output,  /* segmentation mask image */
	       char * name) ;
