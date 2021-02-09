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
wm title . "Threshold"

if {$argc != 2} { 
  puts stderr "usage: gaussianfilter in out"
  exit 
}

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

# globals
#   GAUSSIANFILTER(infilename)
#   GAUSSIANFILTER(outfilename)
#   GAUSSIANFILTER(im)
#   GAUSSIANFILTER(im_rs)
#   GAUSSIANFILTER(im_cs)
set GAUSSIANFILTER(param) 1
set GAUSSIANFILTER(name) gaussianfilter

# get input image file name as first argument
set GAUSSIANFILTER(infilename) [lindex $argv 0]

# get output image file name as second argument
set GAUSSIANFILTER(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command gaussianfilter_quit
pack .top.quit -side right

# create the param button
scale .top.param -from 1 -to 50 -length 200 -variable GAUSSIANFILTER(param) \
  -orient horizontal -tickinterval 10 -showvalue true -command gaussianfilter_run
pack .top.param -side left

# create an image and load contents from file 
set GAUSSIANFILTER(im) [image create photo imname -file "$GAUSSIANFILTER(infilename)"]

# get image size
set GAUSSIANFILTER(im_cs) [image height $GAUSSIANFILTER(im)]
set GAUSSIANFILTER(im_rs) [image width $GAUSSIANFILTER(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $GAUSSIANFILTER(im_rs) -height $GAUSSIANFILTER(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $GAUSSIANFILTER(im_rs) - 2] -height [expr $GAUSSIANFILTER(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $GAUSSIANFILTER(im)

# action associated to param. change
proc gaussianfilter_run {param} {
  global GAUSSIANFILTER
  global PINK
    my_exec_f $PINK/linux/bin/gaussianfilter $GAUSSIANFILTER(infilename) [expr 1.0 / $param] $GAUSSIANFILTER(outfilename)
  $GAUSSIANFILTER(im) read $GAUSSIANFILTER(outfilename)
}

# action associated to quit widget
proc gaussianfilter_quit {} {
  global GAUSSIANFILTER
  foreach file [glob -nocomplain "/tmp/$GAUSSIANFILTER(name)_?"] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW gaussianfilter_quit
bind . "q" gaussianfilter_quit
