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

/*! \file skel2graph.c

\brief generation of a graph from a curvilinear skeleton

<B>Usage:</B> skel2graph in.skel [mode [param]] out.graph

<B>Description:</B>
Generation of a graph from a curvilinear skeleton.

The parameter \b mode has the following meaning (default is 0):

\li 0: vertices of the graphs are elements (ends, curves, junctions, isolated points) of the skeleton

\li 1: vertices of the graph are only ends, isolated points and junctions of the skeleton

\li 2: each curve of the skeleton (not including its ends) is represented by N edges and N-1 vertices of the graph, where N is set to \b param

\li 3: each curve of the skeleton (not including its end) is represented by N edges and N-1 vertices of the graph, where N is set to max(1, trunc(len / \b param)), where len is the number of points of the curve.

<B>Types supported:</B> 2Dskel, 3Dskel

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2009
*/
// UPDATE MC 13/07/2012: adding modes 2 and 3

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include "mccodimage.h"
#include "mcimage.h"
#include "mcgraphe.h"
#include "mcskelcurv.h"
#include "lskelcurv.h"
#include "lskel2graph.h"

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  skel * s; 
  graphe * g; 
  int32_t mode = 0;
  double param = 1.0;

  if ((argc != 3) && (argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.skel [mode [param]] fileout.graph\n", argv[0]);
    exit(1);
  }

  s = readskel(argv[1]);
  assert(s != NULL);

  if (argc >= 4)
    mode = atoi(argv[2]);

  if (argc == 5)
    param = atof(argv[3]);

  g = lskel2graph(s, mode, param);
    
  assert(g != NULL);

  SaveGraphe(g, argv[argc-1]);  

  return 0;
} /* main */
