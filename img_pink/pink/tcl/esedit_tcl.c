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
/*! \file esedit.tcl

\brief structuring element editor

<B>Usage:</B> esedit.tcl <width height filename | filename>

<B>Description:</B>
If given alone, parameter \b filename indicates a file which contains the 
structuring element to be edited.

Otherwise, \b width and \b height indicate the dimensions of the
element to be created.

Check \b origin to specify the origin of the structuring element.

Check \b revert to set all blue points in the structuring element,
and all red points in its complementary.

Use the \b Abort button to quit without saving.

Use the \b Save/Quit button to save and quit.

<B>Types supported:</B> byte 2d

<B>Category:</B> inter
\ingroup inter

\author Michel Couprie
*/
