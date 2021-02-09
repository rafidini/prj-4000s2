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
exec vtk $0 $1 $2 $3 $4

# This example shows how to combine data from both the imaging
# and graphics pipelines. The vtkMergeData filter is used to
# merge the data from each together.

package require vtk
package require vtkinteraction

set PINK "$env(PINK)"
source "$PINK/tcl/my_write.tcl"
source "$PINK/tcl/my_read.tcl"
source "$PINK/tcl/my_dialog.tcl"

# get input image file names as first and second argument
if {($argc != 2) && ($argc != 4)} { 
    puts stderr "usage: relief lumin.pgm color.ppm [width height]"
  exit 
}
set RELIEF(lumfilename) [lindex $argv 0]
set RELIEF(colfilename) [lindex $argv 1]
set RELIEF(tmpfilename) "_relief_lum.ppm"
set RELIEF(indexframe) 0
if {$argc == 4} { 
    set RELIEF(rs) [lindex $argv 2]
    set RELIEF(cs) [lindex $argv 3]
} else {
    set RELIEF(rs) 600
    set RELIEF(cs) 600
}

catch { exec pgm2ppm $RELIEF(lumfilename) $RELIEF(lumfilename) $RELIEF(lumfilename) $RELIEF(tmpfilename)} mes

proc RELIEF_savepos {} {
  global ren1
  global RELIEF
  set pos [[ren1 GetActiveCamera] GetPosition]
  set foc [[ren1 GetActiveCamera] GetFocalPoint]
  set vup [[ren1 GetActiveCamera] GetViewUp]
  set sca [[ren1 GetActiveCamera] GetParallelScale]
  set ret [Dialog_Prompt_OK_NOK "do you want to save camera settings?"]
  if {$ret == 1} {
    puts "saving camera settings"
    my_write_text "_relief_coord_camera" "$pos\n$foc\n$vup\n$sca"
  }
}

proc RELIEF_setpos {} {
  global ren1
  global renWin
  if {[file exists "_relief_coord_camera"]} {
    puts "loading camera settings"
    set pos [my_read_text "_relief_coord_camera"]
    [ren1 GetActiveCamera] SetPosition [lindex $pos 0] [lindex $pos 1] [lindex $pos 2]
    [ren1 GetActiveCamera] SetFocalPoint [lindex $pos 3] [lindex $pos 4] [lindex $pos 5]
    [ren1 GetActiveCamera] SetViewUp [lindex $pos 6] [lindex $pos 7] [lindex $pos 8]
    [ren1 GetActiveCamera] SetParallelScale [lindex $pos 9]
    renWin Render
  }
}

proc RELIEF_savepng {} {
  global RELIEF
  set i $RELIEF(indexframe)
  set ii [format "%04d" $i]
  vtkWindowToImageFilter w2i$i
  w2i$i SetInput renWin

  vtkPNGWriter writer$i
  writer$i SetInput [w2i$i GetOutput]
  writer$i SetFileName "render_save$ii.png"
  writer$i Write
  set RELIEF(indexframe) [expr $i+1]
}

proc RELIEF_exit {} {
  global ren1
  global RELIEF
  RELIEF_savepos
  exec rm -f $RELIEF(tmpfilename)
  vtkCommand DeleteAllObjects
  exit
}

# Read in an image and compute a luminance value. The image is extracted
# as a set of polygons (vtkImageDataGeometryFilter). We then will
# warp the plane using the scalar (luminance) values.
#
vtkPNMReader reader1
  reader1 SetFileName $RELIEF(tmpfilename)
vtkPNMReader reader2
  reader2 SetFileName $RELIEF(colfilename)
vtkImageLuminance luminance
  luminance SetInput [reader1 GetOutput]
vtkImageDataGeometryFilter geometry
  geometry SetInput [luminance GetOutput]
vtkWarpScalar warp
  warp SetInput [geometry GetOutput]
  warp SetScaleFactor -0.25

# Use vtkMergeFilter to combine the original image with the warped geometry.
#
vtkMergeFilter merge
  merge SetGeometry [warp GetOutput]
  merge SetScalars  [reader2 GetOutput]
vtkDataSetMapper mapper
  mapper SetInput [merge GetOutput]
  mapper SetScalarRange 0 255
  mapper ImmediateModeRenderingOff
vtkActor actor
  actor SetMapper mapper

# Create renderer stuff
#
vtkRenderer ren1
vtkRenderWindow renWin
    renWin AddRenderer ren1
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin
vtkInteractorStyleTrackballCamera style
    iren SetInteractorStyle style

# Add the actors to the renderer, set the background and size
#
ren1 AddActor actor

#[ren1 GetActiveCamera] Azimuth 20
#[ren1 GetActiveCamera] Elevation 30
[ren1 GetActiveCamera] SetPosition 458.265 -173.666 -681.507 
[ren1 GetActiveCamera] SetFocalPoint 103.175 137.833 -48.5939 
[ren1 GetActiveCamera] SetViewUp -0.720751 0.369748 -0.586348 
[ren1 GetActiveCamera] SetParallelScale 1
#ren1 SetBackground 0.1 0.2 0.4
ren1 SetBackground 0.0 0.0 0.0
ren1 ResetCameraClippingRange

# then binds rendering window to a tk widget
set vtkw [vtkTkRenderWidget .ren \
        -width $RELIEF(rs) \
        -height $RELIEF(cs) \
        -rw renWin]
BindTkRenderWidget $vtkw

# command zone
frame .buttons
set quitbutton [button .buttons.quit -text "Quit" -command RELIEF_exit\
                -activebackground white]
set saveposbutton [button .buttons.savepos -text "SavePos" -command RELIEF_savepos\
                -activebackground white]
set setposbutton [button .buttons.setpos -text "SetPos" -command RELIEF_setpos\
                -activebackground white]
set savepngbutton [button .buttons.savepng -text "Snapshot" -command RELIEF_savepng\
                -activebackground white]
pack $quitbutton -side right
pack $saveposbutton -side right
pack $setposbutton -side right
pack $savepngbutton -side right
pack $vtkw .buttons -side top -fill both -expand yes

# what to do in case of window closing:
wm protocol . WM_DELETE_WINDOW RELIEF_exit
bind . "q" RELIEF_exit

RELIEF_setpos

# render the image
#
iren AddObserver UserEvent {wm deiconify .vtkInteract}
set cam1 [ren1 GetActiveCamera]
$cam1 ParallelProjectionOff
$cam1 Zoom 1.4
renWin Render


