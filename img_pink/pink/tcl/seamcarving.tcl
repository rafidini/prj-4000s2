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
# 2D Seam Carver
# Michel couprie - aout 2012

# the current label image is stored in the file: /tmp/$SEAMCARV(name)_l
# the displayed image is stored in the file: /tmp/$SEAMCARV(name)_d
# the original color (ppm) image is stored in the file: /tmp/$SEAMCARV(name)_c.ppm

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

if {$argc != 2} { 
  puts stderr "usage: seamcarving.tcl in.color out.ppm"
  exit 
}

wm title . "Seam Carver"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_dialog.tcl"
source "$PINK/tcl/my_feedback.tcl"
source "$PINK/tcl/my_stroke.tcl"
source "$PINK/tcl/my_write.tcl"

# ============================================================
# GLOBAL VARIABLES
# ============================================================

global SEAMCARV
# infilename  : input file name
# outfilename : output file name
# im : image structure
# can : canvas structure
# rs : row size
# cs : col size
# name : command name
# label
set SEAMCARV(name) "seamcarving"
append SEAMCARV(name) [clock seconds]
set SEAMCARV(label) 1
set SEAMCARV(color_1) blue
set SEAMCARV(color_2) red
set SEAMCARV(color_3) green
set SEAMCARV(alpha) 1.0
set SEAMCARV(markers) 1
set SEAMCARV(gradient) 0
set SEAMCARV(surimp) 1
set SEAMCARV(infilename) [lindex $argv 0]
set SEAMCARV(outfilename) [lindex $argv 1]
if ![file exists $SEAMCARV(infilename)] {
  puts stderr "cannot find input file: $SEAMCARV(infilename)"
  exit   
}

proc tmpfile {tmpname} {
  global SEAMCARV
  return [file join "/tmp" "$SEAMCARV(name)_$tmpname"]
}

my_exec_f convert $SEAMCARV(infilename) [tmpfile c.ppm] 
my_exec rgb2hls [tmpfile c.ppm] [tmpfile h] [tmpfile o] [tmpfile s]
my_exec cp [tmpfile o] [tmpfile d] 
my_exec genimage [tmpfile o] 0 [tmpfile t] 
my_exec genimage [tmpfile o] 0 [tmpfile l] 
my_exec byte2long [tmpfile l] [tmpfile l] 
my_exec gradientcd [tmpfile o] $SEAMCARV(alpha) [tmpfile g]

# ============================================================
# MENU DEFINITION
# ============================================================

set modifier Alt
menu .menubar -tearoff 0
. config -menu .menubar
foreach m {File Action Gradient Help} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$File      add command -label "Quit" -command seg2d_quit -underline 0

$Action    add command -label "Reset" -command seg2d_reset -underline 0

$Gradient  add radio   -label "alpha=0.7" -variable SEAMCARV(alpha) -value 0.7 -command seg2d_gradient
$Gradient  add radio   -label "alpha=1.0" -variable SEAMCARV(alpha) -value 1.0 -command seg2d_gradient
$Gradient  add radio   -label "alpha=1.5" -variable SEAMCARV(alpha) -value 1.5 -command seg2d_gradient

# ============================================================
# MAIN WINDOW DEFINITION
# ============================================================

# create the toplevel frame
frame .top -borderwidth 10
pack .top -side top -fill x

# create the image
set SEAMCARV(im) [image create photo imxyname -file [tmpfile d]]

# get image size
set SEAMCARV(cs) [image height $SEAMCARV(im)]
set SEAMCARV(rs) [image width $SEAMCARV(im)]

# create frame for the image
frame .top.imframe -borderwidth 10 -width $SEAMCARV(rs) -height $SEAMCARV(cs)
pack .top.imframe -side top

# create canvas with the right size
set SEAMCARV(can) [canvas .top.imframe.can -width $SEAMCARV(rs) -height $SEAMCARV(cs)]
pack .top.imframe.can -fill both -expand true

# put the images into the canvas
$SEAMCARV(can) create image 1 1 -anchor nw -image $SEAMCARV(im)

# buttons
frame .top.buttons
frame .top.buttons.labels
radiobutton .top.buttons.labels.1 -variable SEAMCARV(label) -text "1" -value 1 -fg $SEAMCARV(color_1)
radiobutton .top.buttons.labels.2 -variable SEAMCARV(label) -text "2" -value 2 -fg $SEAMCARV(color_2)
radiobutton .top.buttons.labels.3 -variable SEAMCARV(label) -text "3" -value 3 -fg $SEAMCARV(color_3)
pack .top.buttons.labels.1 .top.buttons.labels.2 .top.buttons.labels.3 -side left
pack .top.buttons.labels -side top
pack .top.buttons -side top

#===========================================
# bind actions to mouse events
#===========================================

bind $SEAMCARV(can) <Button-1> { 
  StrokeBegin $SEAMCARV(can) %x %y
}

bind $SEAMCARV(can) <B1-Motion> { 
  Stroke $SEAMCARV(can) %x %y $SEAMCARV(color_$SEAMCARV(label))
}

bind $SEAMCARV(can) <ButtonRelease-1> { 
  set coords [StrokeEnd $SEAMCARV(can) %x %y $SEAMCARV(color_$SEAMCARV(label))]
  my_write_curve [tmpfile lst] $coords
  my_exec genimage [tmpfile o] 0 [tmpfile v]
  my_exec drawlines [tmpfile v] [tmpfile lst] [tmpfile v]
  my_exec seuil2 [tmpfile v] 0 1 0 $SEAMCARV(label) [tmpfile v]
  my_exec max [tmpfile v] [tmpfile l] [tmpfile l]
  my_exec watershedMeyer2 [tmpfile g] [tmpfile l] null $SEAMCARV(connex) [tmpfile t]
  my_exec seuil [tmpfile t] 1 [tmpfile t]
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEAMCARV(im) read [tmpfile d]
}

#===========================================
# PROCEDURES
#===========================================

proc seg2d_reset {} {
  global SEAMCARV
  my_exec genimage [tmpfile o] 0 [tmpfile t] 
  my_exec genimage [tmpfile o] 0 [tmpfile l] 
  my_exec byte2long [tmpfile l] [tmpfile l] 
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEAMCARV(im) read [tmpfile d]
  $SEAMCARV(can) delete line
}

proc seg2d_gradient {} {
  global SEAMCARV
  my_exec gradientcd [tmpfile o] $SEAMCARV(alpha) [tmpfile g]
  my_exec watershedMeyer2 [tmpfile g] [tmpfile l] null $SEAMCARV(connex) [tmpfile t]
  my_exec seuil [tmpfile t] 1 [tmpfile t]
  my_exec surimp [tmpfile o] [tmpfile t] [tmpfile d]
  $SEAMCARV(im) read [tmpfile d]
}

proc seg2d_quit {} {
  global SEAMCARV
  my_exec mv [tmpfile t] $SEAMCARV(outfilename)
  foreach file [glob -nocomplain [tmpfile *]] {
    exec rm $file
  }
  exit
}
wm protocol . WM_DELETE_WINDOW seg2d_quit
bind . "q" seg2d_quit
