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
#!/bin/sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Area segmentation"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   SEGMENTAREA(infilename)
#   SEGMENTAREA(outfilename)
#   SEGMENTAREA(im)
#   SEGMENTAREA(im_rs)
#   SEGMENTAREA(im_cs)
set SEGMENTAREA(param1) 0
set SEGMENTAREA(param2) 0
set SEGMENTAREA(param3) 0
set SEGMENTAREA(param4) 0
set SEGMENTAREA(param5) 0
set SEGMENTAREA(name) segmentarea
set SEGMENTAREA(connex) 4
set SEGMENTAREA(combine) 1
set SEGMENTAREA(inverse) 0

if {$argc != 2} { 
  puts stderr "usage: segmentarea in out"
  exit 
}

proc tmpfile {tmpname} {
  global SEGMENTAREA
  return [file join "/tmp" "$SEGMENTAREA(name)_$tmpname"]
}

# get input image file name as first argument
set SEGMENTAREA(infilename) [lindex $argv 0]
file copy -force $SEGMENTAREA(infilename) [tmpfile 1]

# get output image file name as second argument
set SEGMENTAREA(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side left

# create a frame for a row of buttons
frame .but.row -borderwidth 5
pack .but.row -side top

# create the quit button
button .but.row.quit -text Quit -command segmentarea_quit
pack .but.row.quit -side right

# create the connex buttons
radiobutton .but.row.connex4 -variable SEGMENTAREA(connex) -text "4" -value 4
pack .but.row.connex4 -side left
radiobutton .but.row.connex8 -variable SEGMENTAREA(connex) -text "8" -value 8
pack .but.row.connex8 -side left

# create the combine button
checkbutton .but.row.combine -text combine -variable SEGMENTAREA(combine) -command segmentarea_combine
pack .but.row.combine -side left

# create the inverse button
checkbutton .but.row.inverse -text inverse -variable SEGMENTAREA(inverse) -command segmentarea_inverse
pack .but.row.inverse -side left

# create the area buttons
scale .but.area1 -label "x 1" -from 0 -to 9 -length 200 -variable SEGMENTAREA(param1) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentarea_run
scale .but.area2  -label "x 10" -from 0 -to 9 -length 200 -variable SEGMENTAREA(param2) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentarea_run
scale .but.area3  -label "x 100" -from 0 -to 9 -length 200 -variable SEGMENTAREA(param3) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentarea_run
scale .but.area4  -label "x 1000" -from 0 -to 9 -length 200 -variable SEGMENTAREA(param4) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentarea_run
scale .but.area5  -label "x 10000" -from 0 -to 9 -length 200 -variable SEGMENTAREA(param5) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentarea_run
pack .but.area1 -side top
pack .but.area2 -side top
pack .but.area3 -side top
pack .but.area4 -side top
pack .but.area5 -side top

# create an image and load contents from file 
set SEGMENTAREA(im) [image create photo imname -file "$SEGMENTAREA(infilename)"]

# get image size
set SEGMENTAREA(im_cs) [image height $SEGMENTAREA(im)]
set SEGMENTAREA(im_rs) [image width $SEGMENTAREA(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $SEGMENTAREA(im_rs) -height $SEGMENTAREA(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $SEGMENTAREA(im_rs) - 2] -height [expr $SEGMENTAREA(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $SEGMENTAREA(im)

# action associated to area change
proc segmentarea_run {dummy} {
  global SEGMENTAREA
  set area [expr $SEGMENTAREA(param1) + 10*$SEGMENTAREA(param2) + 100*$SEGMENTAREA(param3) + 1000*$SEGMENTAREA(param4) + 10000*$SEGMENTAREA(param5)]
  my_exec segmentarea [tmpfile 1] $SEGMENTAREA(connex) $area [tmpfile 3]

  if {$SEGMENTAREA(combine) == 1} {
    my_exec surimp $SEGMENTAREA(infilename) [tmpfile 3] [tmpfile 2]
    $SEGMENTAREA(im) read [tmpfile 2]
  } else {
    $SEGMENTAREA(im) read [tmpfile 3]
  }
}

# action associated to combine button
proc segmentarea_combine {} {
  global SEGMENTAREA
  if {$SEGMENTAREA(combine) == 1} {
    my_exec surimp $SEGMENTAREA(infilename) [tmpfile 3] [tmpfile 2]
    $SEGMENTAREA(im) read [tmpfile 2]
  } else {
    $SEGMENTAREA(im) read [tmpfile 3]
  }
}

# action associated to inverse button
proc segmentarea_inverse {} {
  global SEGMENTAREA
  my_exec inverse [tmpfile 1] [tmpfile 1]
  segmentarea_run 0
}

# action associated to quit widget
proc segmentarea_quit {} {
  global SEGMENTAREA
  my_exec cp [tmpfile 3] $SEGMENTAREA(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW segmentarea_quit
bind . "q" segmentarea_quit
