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
wm title . "Homotopic Filling"

# globals
#   HFILLING(infilename)
#   HFILLING(outfilename)
#   HFILLING(im)
#   HFILLING(im_rs)
#   HFILLING(im_cs)
set HFILLING(connex) 4
set HFILLING(combine) 0
set HFILLING(param) 0
set HFILLING(name) hfilling

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
    segment3d_quit
  }
}

if {$argc != 2} { 
  puts stderr "usage: hfilling in out"
  exit 
}

# get input image file name as first argument
set HFILLING(infilename) [lindex $argv 0]

# get output image file name as second argument
set HFILLING(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side top -fill x

# create the quit button
button .but.quit -text Quit -command hfilling_quit
pack .but.quit -side right

# create the connex buttons
radiobutton .but.connex4 -variable HFILLING(connex) -text "4" -value 4
pack .but.connex4 -side left
radiobutton .but.connex8 -variable HFILLING(connex) -text "8" -value 8
pack .but.connex8 -side left

# create the combine button
set HFILLING(combine) 0
checkbutton .but.combine -text combine -variable HFILLING(combine) -command hfilling_combine
pack .but.combine -side right

# create the radius button
scale .but.radius -from -20 -to 20 -length 200 -variable HFILLING(param) \
  -orient horizontal -tickinterval 1 -showvalue true -command hfilling_run
pack .but.radius -side left

# create an image and load contents from file 
set HFILLING(im) [image create photo imname -file "$HFILLING(infilename)"]

# get image size
set HFILLING(im_cs) [image height $HFILLING(im)]
set HFILLING(im_rs) [image width $HFILLING(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $HFILLING(im_rs) -height $HFILLING(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $HFILLING(im_rs) - 2] -height [expr $HFILLING(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $HFILLING(im)

# action associated to radius change
proc hfilling_run {radius} {
  global HFILLING
  if {$radius < 0} {
    my_exec hcutting $HFILLING(infilename) [expr -$radius] $HFILLING(connex) $HFILLING(outfilename)
  } else {
    my_exec hfilling $HFILLING(infilename) [expr -$radius] $HFILLING(connex) $HFILLING(outfilename)
  }
  if {$HFILLING(combine) == 0} {
    $HFILLING(im) read $HFILLING(outfilename)
  } else {
    my_exec scale $HFILLING(infilename) 0.5 "/tmp/$HFILLING(name)1"
    my_exec scale $HFILLING(outfilename) 0.5 "/tmp/$HFILLING(name)2"
    my_exec add "/tmp/$HFILLING(name)1" "/tmp/$HFILLING(name)2" "/tmp/$HFILLING(name)2"
    $HFILLING(im) read "/tmp/$HFILLING(name)2"
  }
}

# action associated to combine button
proc hfilling_combine {} {
  global HFILLING
  if {$HFILLING(combine) == 0} {
    $HFILLING(im) read $HFILLING(outfilename)
  } else {
    my_exec scale $HFILLING(infilename) 0.5 "/tmp/$HFILLING(name)1"
    my_exec scale $HFILLING(outfilename) 0.5 "/tmp/$HFILLING(name)2"
    my_exec add "/tmp/$HFILLING(name)1" "/tmp/$HFILLING(name)2" "/tmp/$HFILLING(name)2"
    $HFILLING(im) read "/tmp/$HFILLING(name)2"
  }
}

# action associated to quit widget
proc hfilling_quit {} {
  global HFILLING
  foreach file [glob -nocomplain "/tmp/$HFILLING(name)?"] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW hfilling_quit
