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

# A simple renderer - Michel Couprie 2002
package require vtk
package require vtkinteraction

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
    renWin AddRenderer ren1 

# load geometry
vtkPolyDataReader obj1
    obj1 SetFileName $infilename
vtkPolyDataNormals normals1
    normals1 SetInput [obj1 GetOutput]
    normals1 FlipNormalsOn
vtkPolyDataMapper obj1Mapper
    obj1Mapper SetInput [normals1 GetOutput]
vtkActor obj1Actor
    obj1Actor SetMapper obj1Mapper

ren1 AddProp obj1Actor
ren1 ResetCamera

# then binds rendering window to a tk widget
set vtkw [vtkTkRenderWidget .ren \
        -width 600 \
        -height 600 \
        -rw renWin]
BindTkRenderWidget $vtkw
# command zone
frame .commands
frame .buttons
set quitbutton [button .buttons.quit -text "Quit" -command RENDER_bye\
                -activebackground white]
# what to do in case of window closing:
wm protocol . WM_DELETE_WINDOW RENDER_bye
bind . "q" RENDER_bye
 
proc RENDER_bye {} {
    vtkCommand DeleteAllObjects
    exit
}
set wireframe 0
set wirebutton [checkbutton .buttons.wire -text "Wireframe" \
                -variable wireframe -command RENDER_togglewire\
                -activebackground white]
proc RENDER_togglewire {} {
  global wireframe
  if ($wireframe) {
    eval [obj1Actor GetProperty] SetRepresentationToWireframe
  } else {
    eval [obj1Actor GetProperty] SetRepresentationToSurface
  }
  renWin Render
}
set opacity [scale .commands.opacity \
        -from 0.0 -to 1.0 -res 0.05 \
        -orient horizontal \
        -label "Opacity:" \
        -command setOpacity]
$opacity set 1.0
proc setOpacity {opa} {
    eval [obj1Actor GetProperty] SetOpacity $opa
    renWin Render
}
pack $wirebutton -side left
pack $quitbutton -side right
pack $opacity .buttons -side top -anchor nw -fill both
pack $vtkw .commands -side top -fill both -expand yes
