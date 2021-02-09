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
# 2D Segmenter
# Michel couprie - aout 2002

# the current label image is stored in the file: /tmp/$SEG2D(name)_l
# the current segmentation is stored in the file: /tmp/$SEG2D(name)_t
# the displayed image is stored in the file: /tmp/$SEG2D(name)_d
# the original color (ppm) image is stored in the file: /tmp/$SEG2D(name)_c.ppm
# the original grayscale image is stored in the file: /tmp/$SEG2D(name)_o
# the gradient image is stored in the file: /tmp/$SEG2D(name)_g

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

if {$argc != 2} { 
  puts stderr "usage: 2dsegment.tcl in.color out.binary"
  exit 
}

wm title . "2D segmenter"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_dialog.tcl"
source "$PINK/tcl/my_feedback.tcl"
source "$PINK/tcl/my_stroke.tcl"
source "$PINK/tcl/my_write.tcl"

# ============================================================
# GLOBAL VARIABLES
# ============================================================

global SEG2D
# infilename  : input file name
# outfilename : output file name
# im : image structure
# can : canvas structure
# rs : row size
# cs : col size
# name : command name
# connex : adjacency relation (4,8)
# label
# alpha : parameter for the canny-deriche gradient
set SEG2D(name) "2dsegment"
append SEG2D(name) [clock seconds]
set SEG2D(connex) 4
set SEG2D(connexbar) 8
set SEG2D(label) 1
set SEG2D(color_1) blue
set SEG2D(color_2) red
set SEG2D(color_3) green
set SEG2D(alpha) 1.0
set SEG2D(markers) 1
set SEG2D(gradient) 0
set SEG2D(surimp) 1
set SEG2D(infilename) [lindex $argv 0]
set SEG2D(outfilename) [lindex $argv 1]
if ![file exists $SEG2D(infilename)] {
  puts stderr "cannot find input file: $SEG2D(infilename)"
  exit   
}

proc tmpfile {tmpname} {
  global SEG2D
  return [file join "/tmp" "$SEG2D(name)_$tmpname"]
}

my_exec_f convert $SEG2D(infilename) [tmpfile c.ppm] 
my_exec rgb2hls [tmpfile c.ppm] [tmpfile h] [tmpfile o] [tmpfile s]
my_exec cp [tmpfile o] [tmpfile d] 
my_exec genimage [tmpfile o] 0 [tmpfile t] 
my_exec genimage [tmpfile o] 0 [tmpfile l] 
my_exec byte2long [tmpfile l] [tmpfile l] 
my_exec gradientcd [tmpfile o] $SEG2D(alpha) [tmpfile g]

# ============================================================
# MENU DEFINITION
# ============================================================

set modifier Alt
menu .menubar -tearoff 0
. config -menu .menubar
foreach m {File Action Gradient Vue Help} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$File      add command -label "Quit" -command seg2d_quit -underline 0

$Action    add command -label "Reset" -command seg2d_reset -underline 0
$Action    add command -label "Add frame as marker" -command seg2d_frame -underline 0
$Action    add command -label "Smooth" -command {seg2d_vue_smooth 5} -underline 0
$Action    add command -label "Smooth more" -command {seg2d_vue_smooth 10} -underline 0
$Action    add command -label "Smooth very much" -command {seg2d_vue_smooth 20} -underline 0

$Gradient  add radio   -label "alpha=0.7" -variable SEG2D(alpha) -value 0.7 -command seg2d_gradient
$Gradient  add radio   -label "alpha=1.0" -variable SEG2D(alpha) -value 1.0 -command seg2d_gradient
$Gradient  add radio   -label "alpha=1.5" -variable SEG2D(alpha) -value 1.5 -command seg2d_gradient

$Vue       add command -label "Extract" -command seg2d_vue_extract -underline 0
$Vue       add command -label "Surimp" -command seg2d_vue_surimp -underline 0
$Vue       add command -label "Gradient" -command seg2d_vue_gradient -underline 0
$Vue       add check -label "Markers" -variable SEG2D(markers) -command seg2d_vue_markers -underline 0

# ============================================================
# MAIN WINDOW DEFINITION
# ============================================================

# create the toplevel frame
frame .top -borderwidth 10
pack .top -side top -fill x

# create the image
set SEG2D(im) [image create photo imxyname -file [tmpfile d]]

# get image size
set SEG2D(cs) [image height $SEG2D(im)]
set SEG2D(rs) [image width $SEG2D(im)]

# create frame for the image
frame .top.imframe -borderwidth 10 -width $SEG2D(rs) -height $SEG2D(cs)
pack .top.imframe -side top

# create canvas with the right size
set SEG2D(can) [canvas .top.imframe.can -width $SEG2D(rs) -height $SEG2D(cs)]
pack .top.imframe.can -fill both -expand true

# put the images into the canvas
$SEG2D(can) create image 1 1 -anchor nw -image $SEG2D(im)

# buttons
frame .top.buttons
frame .top.buttons.labels
radiobutton .top.buttons.labels.1 -variable SEG2D(label) -text "1" -value 1 -fg $SEG2D(color_1)
radiobutton .top.buttons.labels.2 -variable SEG2D(label) -text "2" -value 2 -fg $SEG2D(color_2)
radiobutton .top.buttons.labels.3 -variable SEG2D(label) -text "3" -value 3 -fg $SEG2D(color_3)
pack .top.buttons.labels.1 .top.buttons.labels.2 .top.buttons.labels.3 -side left
pack .top.buttons.labels -side top
pack .top.buttons -side top

#===========================================
# bind actions to mouse events
#===========================================

bind $SEG2D(can) <Button-1> { 
  StrokeBegin $SEG2D(can) %x %y
}

bind $SEG2D(can) <B1-Motion> { 
  Stroke $SEG2D(can) %x %y $SEG2D(color_$SEG2D(label))
}

bind $SEG2D(can) <ButtonRelease-1> { 
  set coords [StrokeEnd $SEG2D(can) %x %y $SEG2D(color_$SEG2D(label))]
  my_write_curve [tmpfile lst] $coords
  my_exec genimage [tmpfile o] 0 [tmpfile v]
  my_exec drawlines [tmpfile v] [tmpfile lst] [tmpfile v]
  my_exec seuil2 [tmpfile v] 0 1 0 $SEG2D(label) [tmpfile v]
  my_exec max [tmpfile v] [tmpfile l] [tmpfile l]
  my_exec watershedMeyer2 [tmpfile g] [tmpfile l] null $SEG2D(connex) [tmpfile t]
  my_exec seuil [tmpfile t] 1 [tmpfile t]
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEG2D(im) read [tmpfile d]
}

#===========================================
# PROCEDURES
#===========================================

proc seg2d_reset {} {
  global SEG2D
  my_exec genimage [tmpfile o] 0 [tmpfile t] 
  my_exec genimage [tmpfile o] 0 [tmpfile l] 
  my_exec byte2long [tmpfile l] [tmpfile l] 
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEG2D(im) read [tmpfile d]
  $SEG2D(can) delete line
}

proc seg2d_frame {} {
  global SEG2D
  my_exec cadre [tmpfile t] [tmpfile 1] 
  my_exec seuil2 [tmpfile 1] 1 1 0 $SEG2D(label) [tmpfile 1]
  my_exec byte2long [tmpfile 1] [tmpfile 1]
  my_exec max [tmpfile 1] [tmpfile l] [tmpfile l]
  my_exec watershedMeyer2 [tmpfile g] [tmpfile l] null $SEG2D(connex) [tmpfile t]
  my_exec seuil [tmpfile t] 1 [tmpfile t]
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEG2D(im) read [tmpfile d]
}

proc seg2d_gradient {} {
  global SEG2D
  my_exec gradientcd [tmpfile o] $SEG2D(alpha) [tmpfile g]
  my_exec watershedMeyer2 [tmpfile g] [tmpfile l] null $SEG2D(connex) [tmpfile t]
  my_exec seuil [tmpfile t] 1 [tmpfile t]
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEG2D(im) read [tmpfile d]
}

proc seg2d_vue_gradient {} {
  global SEG2D
  my_exec inverse [tmpfile g] [tmpfile d]
  my_exec surimp [tmpfile d] [tmpfile t] [tmpfile d]
  $SEG2D(im) read [tmpfile d]
}

proc seg2d_vue_markers {} {
  global SEG2D
  if {$SEG2D(markers)} {
    $SEG2D(can) itemconfigure $SEG2D(color_1) -fill $SEG2D(color_1)
    $SEG2D(can) itemconfigure $SEG2D(color_2) -fill $SEG2D(color_2)
    $SEG2D(can) itemconfigure $SEG2D(color_3) -fill $SEG2D(color_3)
  } else { 
    $SEG2D(can) itemconfigure line -fill ""
  }
  
}

proc seg2d_vue_extract {} {
  global SEG2D
  my_exec long2byte [tmpfile l] [tmpfile 1]
  my_exec selndg [tmpfile 1] $SEG2D(label) $SEG2D(label) [tmpfile 1]
  my_exec inverse [tmpfile t] [tmpfile 2]
  my_exec geodilat [tmpfile 1] [tmpfile 2] $SEG2D(connex) -1 [tmpfile 1]
  my_exec inverse [tmpfile 1] [tmpfile 2]
  my_exec min [tmpfile o] [tmpfile 1] [tmpfile d]
  my_exec max [tmpfile d] [tmpfile 2] [tmpfile d]
  my_exec hls2rgb [tmpfile h] [tmpfile d] [tmpfile s] [tmpfile d]
  $SEG2D(im) read [tmpfile d]
}

proc seg2d_vue_smooth {radius} {
  global SEG2D
  my_exec long2byte [tmpfile l] [tmpfile 1]
  my_exec selndg [tmpfile 1] $SEG2D(label) $SEG2D(label) [tmpfile 1]
  my_exec inverse [tmpfile t] [tmpfile 2]
  my_exec geodilat [tmpfile 1] [tmpfile 2] $SEG2D(connex) -1 [tmpfile 1]
  my_exec openball [tmpfile 1] $radius [tmpfile 1]
  my_exec closeball [tmpfile 1] $radius [tmpfile 1]
  my_exec border [tmpfile 1] $SEG2D(connexbar) [tmpfile t]
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEG2D(im) read [tmpfile d]
}

proc seg2d_vue_surimp {} {
  global SEG2D
  my_exec watershedMeyer2 [tmpfile g] [tmpfile l] null $SEG2D(connex) [tmpfile t]
  my_exec seuil [tmpfile t] 1 [tmpfile t]
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEG2D(im) read [tmpfile d]
}

proc seg2d_quit {} {
  global SEG2D
  my_exec mv [tmpfile t] $SEG2D(outfilename)
  foreach file [glob -nocomplain [tmpfile *]] {
    exec rm $file
  }
  exit
}
wm protocol . WM_DELETE_WINDOW seg2d_quit
bind . "q" seg2d_quit
