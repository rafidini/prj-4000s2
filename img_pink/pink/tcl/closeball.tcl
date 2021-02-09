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
wm title . "Ball close"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   CLOSEBALL(infilename)
#   CLOSEBALL(outfilename)
#   CLOSEBALL(im)
#   CLOSEBALL(im_rs)
#   CLOSEBALL(im_cs)
set CLOSEBALL(combine) 0
set CLOSEBALL(param) 0
set CLOSEBALL(name) closeball

if {$argc != 2} { 
  puts stderr "usage: closeball.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global CLOSEBALL
  return [file join "/tmp" "$CLOSEBALL(name)_$tmpname"]
}

# get input image file name as first argument
set CLOSEBALL(infilename) [lindex $argv 0]

# get output image file name as second argument
set CLOSEBALL(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command closeball_quit
pack .top.quit -side right

# create the combine button
set CLOSEBALL(combine) 0
checkbutton .top.combine -text combine -variable CLOSEBALL(combine) -command closeball_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from -50 -to 50 -length 200 -variable CLOSEBALL(param) \
  -orient horizontal -tickinterval 1 -showvalue true -command closeball_run
pack .top.radius -side left

# create an image and load contents from file 
set CLOSEBALL(im) [image create photo imname -file "$CLOSEBALL(infilename)"]

# get image size
set CLOSEBALL(im_cs) [image height $CLOSEBALL(im)]
set CLOSEBALL(im_rs) [image width $CLOSEBALL(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $CLOSEBALL(im_rs) -height $CLOSEBALL(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $CLOSEBALL(im_rs) - 2] -height [expr $CLOSEBALL(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $CLOSEBALL(im)

# action associated to radius change
proc closeball_run {radius} {
  global CLOSEBALL
  global PINK
  if {$radius < 0} {
    my_exec $PINK/linux/bin/erosball $CLOSEBALL(infilename) [expr -$radius]  [tmpfile 1]
    my_exec $PINK/linux/bin/dilatball [tmpfile 1] [expr -$radius] [tmpfile 3]
  } else {
    my_exec $PINK/linux/bin/dilatball $CLOSEBALL(infilename) $radius [tmpfile 1]
    my_exec $PINK/linux/bin/erosball [tmpfile 1] $radius [tmpfile 3]
  }
  if {$CLOSEBALL(combine) == 0} {
    $CLOSEBALL(im) read [tmpfile 3]
  } else {
    my_exec $PINK/linux/bin/scale $CLOSEBALL(infilename) 0.5 [tmpfile 1]
    my_exec $PINK/linux/bin/scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec $PINK/linux/bin/add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $CLOSEBALL(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc closeball_combine {} {
  global CLOSEBALL
  global PINK
  if {$CLOSEBALL(combine) == 0} {
    $CLOSEBALL(im) read [tmpfile 3]
  } else {
    my_exec $PINK/linux/bin/scale $CLOSEBALL(infilename) 0.5 [tmpfile 1]
    my_exec $PINK/linux/bin/scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec $PINK/linux/bin/add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $CLOSEBALL(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc closeball_quit {} {
  global CLOSEBALL
  my_exec cp [tmpfile 3] $CLOSEBALL(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW closeball_quit
bind . "q" closeball_quit
