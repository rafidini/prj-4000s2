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
wm title . "Vol segmentation"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   SEGMENTVOL(infilename)
#   SEGMENTVOL(outfilename)
#   SEGMENTVOL(im)
#   SEGMENTVOL(im_rs)
#   SEGMENTVOL(im_cs)
set SEGMENTVOL(param1) 0
set SEGMENTVOL(param2) 0
set SEGMENTVOL(param3) 0
set SEGMENTVOL(param4) 0
set SEGMENTVOL(param5) 0
set SEGMENTVOL(name) segmentvol
set SEGMENTVOL(connex) 4
set SEGMENTVOL(combine) 1
set SEGMENTVOL(inverse) 0

if {$argc != 2} { 
  puts stderr "usage: segmentvol in out"
  exit 
}

proc tmpfile {tmpname} {
  global SEGMENTVOL
  return [file join "/tmp" "$SEGMENTVOL(name)_$tmpname"]
}

# get input image file name as first argument
set SEGMENTVOL(infilename) [lindex $argv 0]
file copy -force $SEGMENTVOL(infilename) [tmpfile 1]

# get output image file name as second argument
set SEGMENTVOL(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side left

# create a frame for a row of buttons
frame .but.row -borderwidth 5
pack .but.row -side top

# create a frame for a second row of buttons
frame .but.row2 -borderwidth 5
pack .but.row2 -side top

# create the watershed button
button .but.row.watershed -text Watershed -command segmentvol_watershed
pack .but.row.watershed -side right

# create the quit button
button .but.row.quit -text Quit -command segmentvol_quit
pack .but.row.quit -side right

# create the connex buttons
radiobutton .but.row2.connex4 -variable SEGMENTVOL(connex) -text "4" -value 4
pack .but.row2.connex4 -side left
radiobutton .but.row2.connex8 -variable SEGMENTVOL(connex) -text "8" -value 8
pack .but.row2.connex8 -side left

# create the combine button
checkbutton .but.row2.combine -text combine -variable SEGMENTVOL(combine) -command segmentvol_combine
pack .but.row2.combine -side left

# create the inverse button
checkbutton .but.row2.inverse -text inverse -variable SEGMENTVOL(inverse) -command segmentvol_inverse
pack .but.row2.inverse -side left

# create the gradient button
checkbutton .but.row2.gradient -text gradient -variable SEGMENTVOL(gradient) -command segmentvol_gradient
pack .but.row2.gradient -side left

# create the vol buttons
scale .but.vol1 -label "x 10" -from 0 -to 9 -length 200 -variable SEGMENTVOL(param1) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentvol_run
scale .but.vol2  -label "x 100" -from 0 -to 9 -length 200 -variable SEGMENTVOL(param2) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentvol_run
scale .but.vol3  -label "x 1000" -from 0 -to 9 -length 200 -variable SEGMENTVOL(param3) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentvol_run
scale .but.vol4  -label "x 10000" -from 0 -to 9 -length 200 -variable SEGMENTVOL(param4) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentvol_run
scale .but.vol5  -label "x 100000" -from 0 -to 9 -length 200 -variable SEGMENTVOL(param5) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentvol_run
pack .but.vol1 -side top
pack .but.vol2 -side top
pack .but.vol3 -side top
pack .but.vol4 -side top
pack .but.vol5 -side top

# create an image and load contents from file 
set SEGMENTVOL(im) [image create photo imname -file "$SEGMENTVOL(infilename)"]

# get image size
set SEGMENTVOL(im_cs) [image height $SEGMENTVOL(im)]
set SEGMENTVOL(im_rs) [image width $SEGMENTVOL(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $SEGMENTVOL(im_rs) -height $SEGMENTVOL(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $SEGMENTVOL(im_rs) - 2] -height [expr $SEGMENTVOL(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $SEGMENTVOL(im)

# action associated to vol change
proc segmentvol_run {dummy} {
  global SEGMENTVOL
  set vol [expr 10*$SEGMENTVOL(param1) + 100*$SEGMENTVOL(param2) + 1000*$SEGMENTVOL(param3) + 10000*$SEGMENTVOL(param4) + 100000*$SEGMENTVOL(param5)]
  my_exec segmentvol [tmpfile 1] $SEGMENTVOL(connex) $vol [tmpfile 3]

  if {$SEGMENTVOL(combine) == 1} {
    my_exec surimp $SEGMENTVOL(infilename) [tmpfile 3] [tmpfile 2]
    $SEGMENTVOL(im) read [tmpfile 2]
  } else {
    $SEGMENTVOL(im) read [tmpfile 3]
  }
}

# action associated to combine button
proc segmentvol_combine {} {
  global SEGMENTVOL
  if {$SEGMENTVOL(combine) == 1} {
    my_exec surimp $SEGMENTVOL(infilename) [tmpfile 3] [tmpfile 2]
    $SEGMENTVOL(im) read [tmpfile 2]
  } else {
    $SEGMENTVOL(im) read [tmpfile 3]
  }
}

# action associated to inverse button
proc segmentvol_inverse {} {
  global SEGMENTVOL
  my_exec inverse [tmpfile 1] [tmpfile 1]
  segmentvol_run 0
}

# action associated to gradient button
proc segmentvol_gradient {} {
  global SEGMENTVOL
  my_exec gradientcd [tmpfile 1] 1 [tmpfile 1]
  my_exec inverse [tmpfile 1] [tmpfile 1]
  segmentvol_run 0
}

# action associated to watershed widget
proc segmentvol_watershed {} {
  global SEGMENTVOL
  my_exec inverse [tmpfile 1] [tmpfile 1]
  my_exec watershed [tmpfile 1] [tmpfile 3] $SEGMENTVOL(connex) [tmpfile 2]
  my_exec surimp $SEGMENTVOL(infilename) [tmpfile 2] [tmpfile 2]
  $SEGMENTVOL(im) read [tmpfile 2]
  my_exec inverse [tmpfile 1] [tmpfile 1]
}

# action associated to quit widget
proc segmentvol_quit {} {
  global SEGMENTVOL
  my_exec cp [tmpfile 3] $SEGMENTVOL(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW segmentvol_quit
bind . "q" segmentvol_quit
