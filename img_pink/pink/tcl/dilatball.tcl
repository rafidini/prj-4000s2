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
wm title . "Ball dilate"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   DILATBALL(infilename)
#   DILATBALL(outfilename)
#   DILATBALL(im)
#   DILATBALL(im_rs)
#   DILATBALL(im_cs)
set DILATBALL(combine) 0
set DILATBALL(param) 0
set DILATBALL(name) dilatball

if {$argc != 2} { 
  puts stderr "usage: dilatball.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global DILATBALL
  return [file join "/tmp" "$DILATBALL(name)_$tmpname"]
}

# get input image file name as first argument
set DILATBALL(infilename) [lindex $argv 0]

# get output image file name as second argument
set DILATBALL(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command dilatball_quit
pack .top.quit -side right

# create the combine button
set DILATBALL(combine) 0
checkbutton .top.combine -text combine -variable DILATBALL(combine) -command dilatball_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from -20 -to 20 -length 200 -variable DILATBALL(param) \
  -orient horizontal -tickinterval 1 -showvalue true -command dilatball_run
pack .top.radius -side left

# create an image and load contents from file 
set DILATBALL(im) [image create photo imname -file "$DILATBALL(infilename)"]

# get image size
set DILATBALL(im_cs) [image height $DILATBALL(im)]
set DILATBALL(im_rs) [image width $DILATBALL(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $DILATBALL(im_rs) -height $DILATBALL(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $DILATBALL(im_rs) - 2] -height [expr $DILATBALL(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $DILATBALL(im)

# action associated to radius change
proc dilatball_run {radius} {
  global DILATBALL
  global PINK
  if {$radius < 0} {
    my_exec $PINK/linux/bin/erosball $DILATBALL(infilename) [expr -$radius] [tmpfile 3]
  } else {
    my_exec $PINK/linux/bin/dilatball $DILATBALL(infilename) $radius [tmpfile 3]
  }
  if {$DILATBALL(combine) == 0} {
    $DILATBALL(im) read [tmpfile 3]
  } else {
    my_exec scale $DILATBALL(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $DILATBALL(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc dilatball_combine {} {
  global DILATBALL
  if {$DILATBALL(combine) == 0} {
    $DILATBALL(im) read [tmpfile 3]
  } else {
    my_exec scale $DILATBALL(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $DILATBALL(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc dilatball_quit {} {
  global DILATBALL
  my_exec cp [tmpfile 3] $DILATBALL(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW dilatball_quit
bind . "q" dilatball_quit
