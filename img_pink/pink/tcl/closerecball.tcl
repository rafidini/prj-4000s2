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
wm title . "CloseRecBall"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   CLOSERECBALL(infilename)
#   CLOSERECBALL(outfilename)
#   CLOSERECBALL(im)
#   CLOSERECBALL(im_rs)
#   CLOSERECBALL(im_cs)
set CLOSERECBALL(combine) 0
set CLOSERECBALL(param) 0
set CLOSERECBALL(name) closerecball

if {$argc != 2} { 
  puts stderr "usage: closerecball in out"
  exit 
}

proc tmpfile {tmpname} {
  global CLOSERECBALL
  return [file join "/tmp" "$CLOSERECBALL(name)_$tmpname"]
}

# get input image file name as first argument
set CLOSERECBALL(infilename) [lindex $argv 0]

# get output image file name as second argument
set CLOSERECBALL(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command closerecball_quit
pack .top.quit -side right

# create the combine button
set CLOSERECBALL(combine) 0
checkbutton .top.combine -text combine -variable CLOSERECBALL(combine) -command closerecball_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from -20 -to 20 -length 200 -variable CLOSERECBALL(param) \
  -orient horizontal -tickinterval 1 -showvalue true -command closerecball_run
pack .top.radius -side left

# create an image and load contents from file 
set CLOSERECBALL(im) [image create photo imname -file "$CLOSERECBALL(infilename)"]

# get image size
set CLOSERECBALL(im_cs) [image height $CLOSERECBALL(im)]
set CLOSERECBALL(im_rs) [image width $CLOSERECBALL(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $CLOSERECBALL(im_rs) -height $CLOSERECBALL(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $CLOSERECBALL(im_rs) - 2] -height [expr $CLOSERECBALL(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $CLOSERECBALL(im)

# action associated to radius change
proc closerecball_run {radius} {
  global CLOSERECBALL
  if {$radius < 0} {
    my_exec erosball $CLOSERECBALL(infilename) [expr -$radius]  [tmpfile 1]
    my_exec geodilat [tmpfile 1] $CLOSERECBALL(infilename) 8 -1 [tmpfile 3]
  } else {
    my_exec dilatball $CLOSERECBALL(infilename) $radius [tmpfile 1]
    my_exec geoeros [tmpfile 1] $CLOSERECBALL(infilename) 8 -1 [tmpfile 3]
  }
  if {$CLOSERECBALL(combine) == 0} {
    $CLOSERECBALL(im) read [tmpfile 3]
  } else {
    my_exec scale $CLOSERECBALL(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $CLOSERECBALL(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc closerecball_combine {} {
  global CLOSERECBALL
  if {$CLOSERECBALL(combine) == 0} {
    $CLOSERECBALL(im) read [tmpfile 3]
  } else {
    my_exec scale $CLOSERECBALL(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $CLOSERECBALL(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc closerecball_quit {} {
  global CLOSERECBALL
  my_exec cp [tmpfile 3] $CLOSERECBALL(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW closerecball_quit
bind . "q" closerecball_quit
