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
exec wish $0 $1 $2 $3

# set window title
wm title . "Insert"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# globals
#   INSERT(infilename)
#   INSERT(infilename2)
#   INSERT(outfilename)
#   INSERT(im)
#   INSERT(im2)
#   INSERT(im_rs)
#   INSERT(im_cs)
#   INSERT(im_rs2)
#   INSERT(im_cs2)
#   INSERT(delta_x)
#   INSERT(delta_y)
set INSERT(x) 0
set INSERT(y) 0
set INSERT(name) insert

if {$argc != 3} { 
  puts stderr "usage: insert.tcl in1 in2 out"
  exit 
}

proc tmpfile {tmpname} {
  global INSERT
  return [file join "/tmp" "$INSERT(name)_$tmpname"]
}

# get input image file names as first two arguments
set INSERT(infilename) [lindex $argv 0]
set INSERT(infilename2) [lindex $argv 1]

# get output image file name as second argument
set INSERT(outfilename) [lindex $argv 2]

# create two images and load contents from files and get image sizes
set INSERT(im) [image create photo imname -file "$INSERT(infilename)"]
set INSERT(im_cs) [image height $INSERT(im)]
set INSERT(im_rs) [image width $INSERT(im)]

set INSERT(im2) [image create photo imname -file "$INSERT(infilename2)"]
set INSERT(im_cs2) [image height $INSERT(im2)]
set INSERT(im_rs2) [image width $INSERT(im2)]

set INSERT(delta_x) [expr $INSERT(im_rs2) - $INSERT(im_rs)]
set INSERT(delta_y) [expr $INSERT(im_cs2) - $INSERT(im_cs)]

puts "$INSERT(delta_x) $INSERT(delta_y)"

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command insert_quit
pack .top.quit -side right

# create the posx button
scale .top.posx -from 0 -to $INSERT(delta_x) -length $INSERT(delta_x) \
  -variable INSERT(x) -orient horizontal -tickinterval 50 \
  -showvalue true -command insert_run
pack .top.posx -side top

# create the posy button
scale .top.posy -from 0 -to $INSERT(delta_y) -length $INSERT(delta_y) \
  -variable INSERT(y) -orient horizontal -tickinterval 50 \
  -showvalue true -command insert_run
pack .top.posy -side top

# create a frame for the combined image (image 2)
frame .imframe -borderwidth 10 -width $INSERT(im_rs2) -height $INSERT(im_cs2)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $INSERT(im_rs2) - 2] -height [expr $INSERT(im_cs2) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $INSERT(im2)

# perform insertion
my_exec $PINK/linux/bin/insert $INSERT(infilename) $INSERT(infilename2) $INSERT(x) $INSERT(y) 0 [tmpfile 3]
$INSERT(im2) read [tmpfile 3]

# action associated to position change
proc insert_run {dummy} {
  global INSERT
  global PINK
  my_exec $PINK/linux/bin/insert $INSERT(infilename) $INSERT(infilename2) $INSERT(x) $INSERT(y) 0 [tmpfile 3]
  $INSERT(im2) read [tmpfile 3]
}

# action associated to quit widget
proc insert_quit {} {
  global INSERT
  my_exec cp [tmpfile 3] $INSERT(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW insert_quit
bind . "q" insert_quit
