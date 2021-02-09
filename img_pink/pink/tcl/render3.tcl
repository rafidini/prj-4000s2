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
exec vtk $0 $1 $2 $3

# example from the tutorial by W.J. Shroeder & al
package require vtk

# set window title
wm title . "Simple Renderer"

# get input image file name as first argument
if {$argc != 3} { 
  puts stderr "usage: render2 in1 in2 in3"
  exit 
}
set infilename1 [lindex $argv 0]
set infilename2 [lindex $argv 1]
set infilename3 [lindex $argv 2]

# create renderer and window
vtkRenderer ren1
    ren1 SetViewport 0 0 1 1
    ren1 SetBackground 0 0 0.7
vtkRenderWindow renWin
    renWin SetSize 600 600
    renWin AddRenderer ren1 

# load geometry
vtkPolyDataReader obj1
    obj1 SetFileName $infilename1
vtkPolyDataReader obj2
    obj2 SetFileName $infilename2
vtkPolyDataReader obj3
    obj3 SetFileName $infilename3

# compute normals
vtkPolyDataNormals normals1
    normals1 SetInput [obj1 GetOutput]
    normals1 FlipNormalsOn
vtkPolyDataNormals normals2
    normals2 SetInput [obj2 GetOutput]
    normals2 FlipNormalsOn
vtkPolyDataNormals normals3
    normals3 SetInput [obj3 GetOutput]
    normals3 FlipNormalsOn

vtkPolyDataMapper obj1Mapper
    obj1Mapper SetInput [normals1 GetOutput]
vtkPolyDataMapper obj2Mapper
    obj2Mapper SetInput [normals2 GetOutput]
vtkPolyDataMapper obj3Mapper
    obj3Mapper SetInput [normals3 GetOutput]

vtkActor obj1Actor
    obj1Actor SetMapper obj1Mapper
    eval [obj1Actor GetProperty] SetOpacity 0.5
vtkActor obj2Actor
    obj2Actor SetMapper obj2Mapper
    eval [obj2Actor GetProperty] SetColor 1.0 0.49 0.25
vtkActor obj3Actor
    obj3Actor SetMapper obj3Mapper
    eval [obj3Actor GetProperty] SetColor 0.25 0.25 0.25

ren1 AddProp obj1Actor
ren1 AddProp obj2Actor
ren1 AddProp obj3Actor
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

