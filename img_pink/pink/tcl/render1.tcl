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
#sh
# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1

# example from the tutorial by W.J. Shroeder & al
package require vtk

# set window title
wm title . "Simple Renderer"

# get input image file name as first argument
if {$argc != 1} { 
  puts stderr "usage: render in"
  exit 
}
set infilename [lindex $argv 0]

# create renderer and window
vtkRenderer ren1
    ren1 SetViewport 0 0 1 1
    ren1 SetBackground 0 0 0.7
vtkRenderWindow renWin
    renWin SetSize 600 600
    renWin AddRenderer ren1 

# load geometry
vtkPolyDataReader reader
    reader SetFileName $infilename

# smoothing (Taubin method)
vtkWindowedSincPolyDataFilter smooth
    smooth SetInput [reader GetOutput]
    smooth SetNumberOfIterations 15
    smooth BoundarySmoothingOn
    smooth SetFeatureAngle 120
    smooth SetEdgeAngle 90
    smooth SetPassBand 0.1

# compute normals
vtkPolyDataNormals normals
    normals SetInput [smooth GetOutput]
    normals FlipNormalsOn
# create mapper and actor
vtkPolyDataMapper mapper
    mapper SetInput [normals GetOutput]
vtkActor actor
    actor SetMapper mapper
    eval [actor GetProperty] SetInterpolationToPhong
#    eval [actor GetProperty] SetColor 1.0 0.59 0.35

ren1 AddProp actor
ren1 ResetCamera

# add interactor to the rendering window
vtkRenderWindowInteractor iren
iren SetRenderWindow renWin
iren Initialize
# add this for a normal exit
iren AddObserver ExitEvent {exit} 

renWin Render

# 
# Hide the default . widget
#
wm withdraw .
