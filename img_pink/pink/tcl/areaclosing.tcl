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
wm title . "Area closing"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   AREACLOSE(infilename)
#   AREACLOSE(outfilename)
#   AREACLOSE(im)
#   AREACLOSE(im_rs)
#   AREACLOSE(im_cs)
set AREACLOSE(param1) 0
set AREACLOSE(param2) 0
set AREACLOSE(param3) 0
set AREACLOSE(param4) 0
set AREACLOSE(name) areaclose
set AREACLOSE(connex) 4

if {$argc != 2} { 
  puts stderr "usage: areaclose in out"
  exit 
}

proc tmpfile {tmpname} {
  global AREACLOSE
  return [file join "/tmp" "$AREACLOSE(name)_$tmpname"]
}

# get input image file name as first argument
set AREACLOSE(infilename) [lindex $argv 0]

# get output image file name as second argument
set AREACLOSE(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side left

# create a frame for a row of buttons
frame .but.row -borderwidth 5
pack .but.row -side top

# create the quit button
button .but.row.quit -text Quit -command areaclose_quit
pack .but.row.quit -side right

# create the connex buttons
radiobutton .but.row.connex4 -variable AREACLOSE(connex) -text "4" -value 4
pack .but.row.connex4 -side left
radiobutton .but.row.connex8 -variable AREACLOSE(connex) -text "8" -value 8
pack .but.row.connex8 -side left

# create the area buttons
scale .but.area1 -label "x 1" -from 0 -to 10 -length 200 -variable AREACLOSE(param1) \
  -orient horizontal -tickinterval 1 -showvalue false -command areaclose_run
scale .but.area2  -label "x 10" -from 0 -to 10 -length 200 -variable AREACLOSE(param2) \
  -orient horizontal -tickinterval 1 -showvalue false -command areaclose_run
scale .but.area3  -label "x 100" -from 0 -to 10 -length 200 -variable AREACLOSE(param3) \
  -orient horizontal -tickinterval 1 -showvalue false -command areaclose_run
scale .but.area4  -label "x 1000" -from 0 -to 10 -length 200 -variable AREACLOSE(param4) \
  -orient horizontal -tickinterval 1 -showvalue false -command areaclose_run
pack .but.area1 -side top
pack .but.area2 -side top
pack .but.area3 -side top
pack .but.area4 -side top

# create an image and load contents from file 
set AREACLOSE(im) [image create photo imname -file "$AREACLOSE(infilename)"]

# get image size
set AREACLOSE(im_cs) [image height $AREACLOSE(im)]
set AREACLOSE(im_rs) [image width $AREACLOSE(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $AREACLOSE(im_rs) -height $AREACLOSE(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $AREACLOSE(im_rs) - 2] -height [expr $AREACLOSE(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $AREACLOSE(im)

# action associated to area change
proc areaclose_run {dummy} {
  global AREACLOSE
  set area [expr $AREACLOSE(param1) + 10*$AREACLOSE(param2) + 100*$AREACLOSE(param3) + 1000*$AREACLOSE(param4)]
  my_exec areaclosing $AREACLOSE(infilename) $AREACLOSE(connex) $area [tmpfile 3]
  $AREACLOSE(im) read [tmpfile 3]
}

# action associated to quit widget
proc areaclose_quit {} {
  global AREACLOSE
  my_exec cp [tmpfile 3] $AREACLOSE(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW areaclose_quit
bind . "q" areaclose_quit
