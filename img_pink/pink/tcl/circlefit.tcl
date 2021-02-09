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
# Circle fitting
# Michel couprie - sept 2002

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Circle Fitting"
menu .menubar
. config -menu .menubar
foreach m {File Edit Filter Help} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$File      add command -label "Quit"             -command circlefit_quit
$File      add command -label "Load"             -command circlefit_load
$File      add command -label "Save"             -command circlefit_save
$Edit      add command -label "Undo"             -command circlefit_undo
$Edit      add command -label "Reset"            -command circlefit_reset
$Filter    add command -label "Smooth"           -command circlefit_smooth

if {$argc != 2} { 
  puts stderr "usage: circlefit in out"
  exit 
}

# declare globals
global CIRCLEFIT
# infilename  : input file name
# outfilename : output file name
# x  : pixel position in row
# y  : pixel position in column
# r  : current circle radius
# s  : current smoothing factor
# im : image structure
# rs : row size
# cs : col size
# name : command name
set CIRCLEFIT(x) 0
set CIRCLEFIT(y) 0 
set CIRCLEFIT(r) 80 
set CIRCLEFIT(s) 0.7
set CIRCLEFIT(name) "circlefit"

# external command call
#-----------------------------------
proc my_exec {command args} {
  set call "catch \{ exec $command $args \} result"
  eval $call
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message $result
    return 1
  }
  return 0
}

# external command call with fatal error test
#-----------------------------------
proc my_exec_f {command args} {
  set call "catch \{ exec $command $args \} result"
  eval $call
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message "Fatal error: $result"
    circlefit_quit
  }
}

# get input image file name as first argument
set CIRCLEFIT(infilename) [lindex $argv 0]
if ![file exists $CIRCLEFIT(infilename)] {
  tk_messageBox -type ok -icon error -message "File not found: $CIRCLEFIT(infilename)"
  exit
}
my_exec cp $CIRCLEFIT(infilename) "/tmp/$CIRCLEFIT(name)_0" 

# get output image file name as second argument
set CIRCLEFIT(outfilename) [lindex $argv 1]

# create an image and load contents from file 
set CIRCLEFIT(im) [image create photo imname -file $CIRCLEFIT(infilename)]

# get image size
set CIRCLEFIT(cs) [image height $CIRCLEFIT(im)]
set CIRCLEFIT(rs) [image width $CIRCLEFIT(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $CIRCLEFIT(rs) -height $CIRCLEFIT(cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $CIRCLEFIT(rs) - 2] -height [expr $CIRCLEFIT(cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $CIRCLEFIT(im)

# bind actions to mouse events
# temp. images: 
# _0: grayscale image
# _1: last circle
# _2: all circles but the last one
# _3: all circles
# _4: surimpression of _3 and original image
my_exec genimage $CIRCLEFIT(rs) $CIRCLEFIT(cs) 1 0 "/tmp/$CIRCLEFIT(name)_1"
my_exec cp "/tmp/$CIRCLEFIT(name)_1"  "/tmp/$CIRCLEFIT(name)_2" 
my_exec cp "/tmp/$CIRCLEFIT(name)_1"  "/tmp/$CIRCLEFIT(name)_3" 
bind .imframe.c <Button> { 
  set CIRCLEFIT(x) %x 
  set CIRCLEFIT(y) %y
  my_exec max "/tmp/$CIRCLEFIT(name)_1" "/tmp/$CIRCLEFIT(name)_2" "/tmp/$CIRCLEFIT(name)_2" 
  my_exec fitcircle "/tmp/$CIRCLEFIT(name)_0" %x %y $CIRCLEFIT(r) "/tmp/$CIRCLEFIT(name)_1" 
  my_exec max "/tmp/$CIRCLEFIT(name)_1"  "/tmp/$CIRCLEFIT(name)_2"  "/tmp/$CIRCLEFIT(name)_3" 
  my_exec surimp "/tmp/$CIRCLEFIT(name)_0" "/tmp/$CIRCLEFIT(name)_3"  "/tmp/$CIRCLEFIT(name)_4" 
  $CIRCLEFIT(im) read "/tmp/$CIRCLEFIT(name)_4"
}

# action associated to reset menu option
proc circlefit_reset {} {
  global CIRCLEFIT
  my_exec cp $CIRCLEFIT(infilename) "/tmp/$CIRCLEFIT(name)_0" 
  my_exec genimage $CIRCLEFIT(rs) $CIRCLEFIT(cs) 1 0 "/tmp/$CIRCLEFIT(name)_1"
  my_exec cp "/tmp/$CIRCLEFIT(name)_1"  "/tmp/$CIRCLEFIT(name)_2" 
  my_exec cp "/tmp/$CIRCLEFIT(name)_1"  "/tmp/$CIRCLEFIT(name)_3" 
  $CIRCLEFIT(im) read "/tmp/$CIRCLEFIT(name)_0"
}

# action associated to undo menu option
proc circlefit_undo {} {
  global CIRCLEFIT
  my_exec genimage $CIRCLEFIT(rs) $CIRCLEFIT(cs) 1 0 "/tmp/$CIRCLEFIT(name)_1"
  my_exec cp "/tmp/$CIRCLEFIT(name)_2"  "/tmp/$CIRCLEFIT(name)_3" 
  my_exec surimp "/tmp/$CIRCLEFIT(name)_0" "/tmp/$CIRCLEFIT(name)_3" "/tmp/$CIRCLEFIT(name)_4"
  $CIRCLEFIT(im) read "/tmp/$CIRCLEFIT(name)_4"
}

# action associated to smooth menu option
proc circlefit_smooth {} {
  global CIRCLEFIT
  my_exec deriche "/tmp/$CIRCLEFIT(name)_0" $CIRCLEFIT(s) 4 "/tmp/$CIRCLEFIT(name)_0"
  my_exec surimp "/tmp/$CIRCLEFIT(name)_0" "/tmp/$CIRCLEFIT(name)_3" "/tmp/$CIRCLEFIT(name)_4"
  $CIRCLEFIT(im) read "/tmp/$CIRCLEFIT(name)_4"
}

# action associated to quit menu option
proc circlefit_quit {} {
  global CIRCLEFIT
  my_exec mv "/tmp/$CIRCLEFIT(name)_3" $CIRCLEFIT(outfilename)
  foreach file [glob -nocomplain "/tmp/$CIRCLEFIT(name)_?"] {
    my_exec rm $file
  }
  exit
}
wm protocol . WM_DELETE_WINDOW circlefit_quit
bind . "q" circlefit_quit
