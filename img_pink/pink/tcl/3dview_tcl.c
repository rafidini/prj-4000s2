# Copyright ESIEE (2009) 
#
# m.couprie@esiee.fr
#
# This software is an image processing library whose purpose is to be
# used primarily for research and teaching.
#
# This software is governed by the CeCILL  license under French law and
# abiding by the rules of distribution of free software. You can  use, 
# modify and/ or redistribute the software under the terms of the CeCILL
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info". 
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability. 
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or 
# data to be ensured and,  more generally, to use and operate it in the 
# same conditions as regards security. 
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL license and that you accept its terms.
#
/*! \file 3dview.tcl

\brief interactive visualization / segmentation of a 3D image

<B>Usage:</B> 3dview.tcl in.pgm out.pgm [mark.pgm]<br>
3dview in.pgm out.pgm [mark.pgm]

<B>Description:</B>

Displays three cross sections of a 3D grayscale image, and allows for 
simple segmentation by thresholding. If given, the parameter \b mark.pgm
must be a binary image (to be superimposed with the input image \b in.pgm).
Segmentation result is saved in \b out.pgm.

Menus: 
\li File: choose \b Quit to save and exit.
\li Analyze: to compute the histogram of the full image (item \b Histogram)
or of the Region Of Interest specified by the current segmentation  
(item \b ROI Histogram).
\li Threshold: simple of double thresholding.
\li Visualize: choose \b Revert to inverse the gray levels, check \b Surimp to 
superimpose the binary image to the grayscale data, check \b Light for 
superimposition to be made with a light color, choose \b Render or 
\b Rendersmooth to launch a 3D rendering of the binary image (a smoothing of 
the surface is done with the last choice).

<B>Types supported:</B> byte 3d

<B>Category:</B> inter
\ingroup inter

\author Michel Couprie
*/
