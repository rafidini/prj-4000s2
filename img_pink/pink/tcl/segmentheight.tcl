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
wm title . "Height segmentation"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   SEGMENTHEIGHT(infilename)
#   SEGMENTHEIGHT(outfilename)
#   SEGMENTHEIGHT(im)
#   SEGMENTHEIGHT(im_rs)
#   SEGMENTHEIGHT(im_cs)
set SEGMENTHEIGHT(param1) 0
set SEGMENTHEIGHT(param2) 0
set SEGMENTHEIGHT(param3) 0
set SEGMENTHEIGHT(name) segmentheight
set SEGMENTHEIGHT(connex) 4
set SEGMENTHEIGHT(combine) 1
set SEGMENTHEIGHT(inverse) 0
set SEGMENTHEIGHT(gradient) 0
set SEGMENTHEIGHT(watershed) 0

# temp files : 
#   1: copie de l'image originale
#   3: maxima sélectionnés
#   2: surimpression de 1 et 3
#   4: inverse de 1
#   5: lpe de 4 avec 3 comme marqueurs
#   6: surimpression de 1 et 6

if {$argc != 2} { 
  puts stderr "usage: segmentheight in out"
  exit 
}

proc tmpfile {tmpname} {
  global SEGMENTHEIGHT
  return [file join "/tmp" "$SEGMENTHEIGHT(name)_$tmpname"]
}

# get input image file name as first argument
set SEGMENTHEIGHT(infilename) [lindex $argv 0]
file copy -force $SEGMENTHEIGHT(infilename) [tmpfile 1]
my_exec inverse [tmpfile 1] [tmpfile 4]

# get output image file name as second argument
set SEGMENTHEIGHT(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side left

# create a frame for a first row of buttons
frame .but.row1 -borderwidth 5
pack .but.row1 -side top

# create a frame for a second row of buttons
frame .but.row2 -borderwidth 5
pack .but.row2 -side top

# create the connex buttons
radiobutton .but.row1.connex4 -variable SEGMENTHEIGHT(connex) -text "4" -value 4
pack .but.row1.connex4 -side left
radiobutton .but.row1.connex8 -variable SEGMENTHEIGHT(connex) -text "8" -value 8
pack .but.row1.connex8 -side left

# create the combine button
checkbutton .but.row1.combine -text combine -variable SEGMENTHEIGHT(combine) -command segmentheight_combine_watershed
pack .but.row1.combine -side left

# create the inverse button
checkbutton .but.row1.inverse -text inverse -variable SEGMENTHEIGHT(inverse) -command segmentheight_inverse
pack .but.row1.inverse -side left

# create the watershed button
checkbutton .but.row2.watershed -text watershed -variable SEGMENTHEIGHT(watershed) -command segmentheight_combine_watershed
pack .but.row2.watershed -side left

# create the quit button
button .but.row2.quit -text Quit -command segmentheight_quit
pack .but.row2.quit -side right

# create the height buttons
scale .but.height1 -label "x 1" -from 0 -to 9 -length 200 -variable SEGMENTHEIGHT(param1) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentheight_run
scale .but.height2  -label "x 10" -from 0 -to 9 -length 200 -variable SEGMENTHEIGHT(param2) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentheight_run
scale .but.height3  -label "x 100" -from 0 -to 9 -length 200 -variable SEGMENTHEIGHT(param3) \
  -orient horizontal -tickinterval 1 -showvalue false -command segmentheight_run
pack .but.height1 -side top
pack .but.height2 -side top
pack .but.height3 -side top

# create an image and load contents from file 
set SEGMENTHEIGHT(im) [image create photo imname -file "$SEGMENTHEIGHT(infilename)"]

# get image size
set SEGMENTHEIGHT(im_cs) [image height $SEGMENTHEIGHT(im)]
set SEGMENTHEIGHT(im_rs) [image width $SEGMENTHEIGHT(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $SEGMENTHEIGHT(im_rs) -height $SEGMENTHEIGHT(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $SEGMENTHEIGHT(im_rs) - 2] -height [expr $SEGMENTHEIGHT(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $SEGMENTHEIGHT(im)

# action associated to height change
proc segmentheight_run {dummy} {
  global SEGMENTHEIGHT
  set height [expr $SEGMENTHEIGHT(param1) + 10*$SEGMENTHEIGHT(param2) + 100*$SEGMENTHEIGHT(param3)]
  my_exec segmentheight [tmpfile 1] $SEGMENTHEIGHT(connex) $height [tmpfile 3]

  segmentheight_combine_watershed
}

# action associated to watershed and to the combine button
proc segmentheight_combine_watershed {} {
  global SEGMENTHEIGHT
  if {$SEGMENTHEIGHT(watershed) == 1} {
    my_exec watershed [tmpfile 4] [tmpfile 3] $SEGMENTHEIGHT(connex) [tmpfile 5]
    if {$SEGMENTHEIGHT(combine) == 1} {
      my_exec surimp $SEGMENTHEIGHT(infilename) [tmpfile 5] [tmpfile 6]
      $SEGMENTHEIGHT(im) read [tmpfile 6]
    } else {
      $SEGMENTHEIGHT(im) read [tmpfile 5]
    }
  } else {
    if {$SEGMENTHEIGHT(combine) == 1} {
      my_exec surimp $SEGMENTHEIGHT(infilename) [tmpfile 3] [tmpfile 2]
      $SEGMENTHEIGHT(im) read [tmpfile 2]
    } else {
      $SEGMENTHEIGHT(im) read [tmpfile 3]
    }
  }
}

# action associated to inverse button
proc segmentheight_inverse {} {
  global SEGMENTHEIGHT
  my_exec inverse [tmpfile 1] [tmpfile 1]
  my_exec inverse [tmpfile 1] [tmpfile 4]
  segmentheight_run 0
}

# action associated to quit widget
proc segmentheight_quit {} {
  global SEGMENTHEIGHT
  if {$SEGMENTHEIGHT(watershed) == 1} {
    my_exec cp [tmpfile 5] $SEGMENTHEIGHT(outfilename)
  } else {
    my_exec cp [tmpfile 3] $SEGMENTHEIGHT(outfilename)
  }
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW segmentheight_quit
bind . "q" segmentheight_quit
