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
exec wish $0 $1 $2

# set window title
wm title . "Rotation"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   ROTATION(infilename)
#   ROTATION(outfilename)
#   ROTATION(im)
#   ROTATION(im_rs)
#   ROTATION(im_cs)
set ROTATION(mode) 1
set ROTATION(param) 0
set ROTATION(name) rotation

if {$argc != 2} { 
  puts stderr "usage: rotation.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global ROTATION
  return [file join "/tmp" "$ROTATION(name)_$tmpname"]
}

# get input image file name as first argument
set ROTATION(infilename) [lindex $argv 0]

# get output image file name as second argument
set ROTATION(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command rotation_quit
pack .top.quit -side right

# create the mode buttons
radiobutton .top.modeinter -variable ROTATION(mode) -text "inter" -value 1\
    -command rotation_changemode
pack .top.modeinter -side right
radiobutton .top.modetrunc -variable ROTATION(mode) -text "trunc" -value 0\
  -command rotation_changemode
pack .top.modetrunc -side right

# create the angle button
scale .top.angle -from 0 -to 359 -length 300 -variable ROTATION(param) \
  -orient horizontal -tickinterval 45 -showvalue true -command rotation_run
pack .top.angle -side left

# create an image and load contents from file 
set ROTATION(im) [image create photo imname -file "$ROTATION(infilename)"]

# get image size
set ROTATION(im_cs) [image height $ROTATION(im)]
set ROTATION(im_rs) [image width $ROTATION(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $ROTATION(im_rs) -height $ROTATION(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $ROTATION(im_rs) - 2] -height [expr $ROTATION(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $ROTATION(im)

# action associated to angle change
proc rotation_run {angle} {
  global ROTATION
  if {$ROTATION(mode) == 1} {
    my_exec rotate $ROTATION(infilename) $ROTATION(param) [expr $ROTATION(im_rs)/2] [expr $ROTATION(im_cs)/2] [tmpfile 3]
  } else {
    my_exec rotatebin $ROTATION(infilename) $ROTATION(param) [expr $ROTATION(im_rs)/2] [expr $ROTATION(im_cs)/2] [tmpfile 3]
  }
  $ROTATION(im) read [tmpfile 3]
}

# action associated to mode change
proc rotation_changemode {} {
  global ROTATION
  rotation_run $ROTATION(param)
}

# action associated to quit widget
proc rotation_quit {} {
  global ROTATION
  my_exec cp [tmpfile 3] $ROTATION(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW rotation_quit
bind . "q" rotation_quit
