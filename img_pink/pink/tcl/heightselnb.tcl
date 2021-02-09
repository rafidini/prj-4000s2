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
wm title . "Height segmentation"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   HEIGHTSELNB(infilename)
#   HEIGHTSELNB(outfilename)
#   HEIGHTSELNB(im)
#   HEIGHTSELNB(im_rs)
#   HEIGHTSELNB(im_cs)
set HEIGHTSELNB(param1) 2
set HEIGHTSELNB(param2) 0
set HEIGHTSELNB(param3) 0
set HEIGHTSELNB(name) heightselnb
set HEIGHTSELNB(connex) 4
set HEIGHTSELNB(combine) 1
set HEIGHTSELNB(inverse) 0

if {$argc != 2} { 
  puts stderr "usage: heightselnb in out"
  exit 
}

proc tmpfile {tmpname} {
  global HEIGHTSELNB
  return [file join "/tmp" "$HEIGHTSELNB(name)_$tmpname"]
}

# get input image file name as first argument
set HEIGHTSELNB(infilename) [lindex $argv 0]
file copy -force $HEIGHTSELNB(infilename) [tmpfile 1]

# get output image file name as second argument
set HEIGHTSELNB(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side left

# create a frame for a row of buttons
frame .but.row -borderwidth 5
pack .but.row -side top

# create the quit button
button .but.row.quit -text Quit -command heightselnb_quit
pack .but.row.quit -side right

# create the connex buttons
radiobutton .but.row.connex4 -variable HEIGHTSELNB(connex) -text "4" -value 4
pack .but.row.connex4 -side left
radiobutton .but.row.connex8 -variable HEIGHTSELNB(connex) -text "8" -value 8
pack .but.row.connex8 -side left

# create the combine button
checkbutton .but.row.combine -text combine -variable HEIGHTSELNB(combine) -command heightselnb_combine
pack .but.row.combine -side left

# create the inverse button
checkbutton .but.row.inverse -text inverse -variable HEIGHTSELNB(inverse) -command heightselnb_inverse
pack .but.row.inverse -side left

# create the height buttons
scale .but.height1 -label "x 1" -from 0 -to 10 -length 200 -variable HEIGHTSELNB(param1) \
  -orient horizontal -tickinterval 1 -showvalue false -command heightselnb_run
scale .but.height2  -label "x 10" -from 0 -to 10 -length 200 -variable HEIGHTSELNB(param2) \
  -orient horizontal -tickinterval 1 -showvalue false -command heightselnb_run
scale .but.height3  -label "x 100" -from 0 -to 10 -length 200 -variable HEIGHTSELNB(param3) \
  -orient horizontal -tickinterval 1 -showvalue false -command heightselnb_run
pack .but.height1 -side top
pack .but.height2 -side top
pack .but.height3 -side top

# create an image and load contents from file 
set HEIGHTSELNB(im) [image create photo imname -file "$HEIGHTSELNB(infilename)"]

# get image size
set HEIGHTSELNB(im_cs) [image height $HEIGHTSELNB(im)]
set HEIGHTSELNB(im_rs) [image width $HEIGHTSELNB(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $HEIGHTSELNB(im_rs) -height $HEIGHTSELNB(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $HEIGHTSELNB(im_rs) - 2] -height [expr $HEIGHTSELNB(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $HEIGHTSELNB(im)

# action associated to height change
proc heightselnb_run {dummy} {
  global HEIGHTSELNB
  set height [expr $HEIGHTSELNB(param1) + 10*$HEIGHTSELNB(param2) + 100*$HEIGHTSELNB(param3)]
  my_exec heightselnb [tmpfile 1] $HEIGHTSELNB(connex) $height [tmpfile 3]

  if {$HEIGHTSELNB(combine) == 1} {
    my_exec surimp $HEIGHTSELNB(infilename) [tmpfile 3] [tmpfile 2]
    $HEIGHTSELNB(im) read [tmpfile 2]
  } else {
    $HEIGHTSELNB(im) read [tmpfile 3]
  }
}

# action associated to combine button
proc heightselnb_combine {} {
  global HEIGHTSELNB
  if {$HEIGHTSELNB(combine) == 1} {
    my_exec surimp $HEIGHTSELNB(infilename) [tmpfile 3] [tmpfile 2]
    $HEIGHTSELNB(im) read [tmpfile 2]
  } else {
    $HEIGHTSELNB(im) read [tmpfile 3]
  }
}

# action associated to inverse button
proc heightselnb_inverse {} {
  global HEIGHTSELNB
  my_exec inverse [tmpfile 1] [tmpfile 1]
  heightselnb_run 0
}

# action associated to quit widget
proc heightselnb_quit {} {
  global HEIGHTSELNB
  my_exec cp [tmpfile 3] $HEIGHTSELNB(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW heightselnb_quit
bind . "q" heightselnb_quit
