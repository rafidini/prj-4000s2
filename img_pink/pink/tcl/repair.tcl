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
# 
# Michel couprie - aout 2004

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2 $3 $4

# set window title
wm title . "Repair"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_scanvas.tcl"

if {$argc != 4} { 
  puts stderr "usage: repair in1 in2 in3 out"
  exit 
}

# declare globals
global REPAIR
# x  : pixel position in row
# y  : pixel position in column
# im : image structure
# rs : row size
# cs : col size
# name : command name
set REPAIR(x) 0
set REPAIR(y) 0 
set REPAIR(name) "repair"
set REPAIR(width) 5 

set REPAIR(in1) [lindex $argv 0]
set REPAIR(in2) [lindex $argv 1]
set REPAIR(in3) [lindex $argv 2]
set REPAIR(outfilename) [lindex $argv 3]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side top -fill x

# create the quit button
button .but.quit -text Quit -command repair_quit
pack .but.quit -side right

# create the repair button
button .but.repair -text Repair -command repair_repair
pack .but.repair -side right

# create an image and load contents from file 
my_exec_f cp $REPAIR(in1) "/tmp/$REPAIR(name)_1"
my_exec_f cp $REPAIR(in2) "/tmp/$REPAIR(name)_2"
my_exec_f cp $REPAIR(in3) "/tmp/$REPAIR(name)_3"
set REPAIR(im) [image create photo imname -file "/tmp/$REPAIR(name)_2"]

# get image size
set REPAIR(cs) [image height $REPAIR(im)]
set REPAIR(rs) [image width $REPAIR(im)]
set ca_w $REPAIR(rs)
set ca_h $REPAIR(cs)
if {$ca_w > 800} {set ca_w 800}
if {$ca_h > 500} {set ca_h 500}

my_exec genimage $REPAIR(rs) $REPAIR(cs) 1 0 "/tmp/$REPAIR(name)_4"

# create a frame for the image
frame .imframe -borderwidth 10 -width $ca_w -height $ca_h
pack .imframe -side top

# create canvas with the right sizes
set c [my_scanvas .imframe.c -width $ca_w -height $ca_h \
	-scrollregion "0 0 $REPAIR(rs) $REPAIR(cs)"]

pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $REPAIR(im)

# bind actions to mouse events
bind $c <Button> { 
  set REPAIR(x) [$c canvasx %x] 
  set REPAIR(y) [$c canvasy %y]
  my_exec point "/tmp/$REPAIR(name)_4" $REPAIR(x) $REPAIR(y) 0 255 "/tmp/$REPAIR(name)_4"
}

# action associated to repair widget
proc repair_repair {} {
  global REPAIR
  my_exec line "/tmp/$REPAIR(name)_4" "/tmp/$REPAIR(name)_5"
  my_exec dilatball "/tmp/$REPAIR(name)_5" $REPAIR(width) "/tmp/$REPAIR(name)_6"
  my_exec interpolate_color "/tmp/$REPAIR(name)_1" "/tmp/$REPAIR(name)_3" "/tmp/$REPAIR(name)_i"
  my_exec combine_color "/tmp/$REPAIR(name)_2" "/tmp/$REPAIR(name)_i" "/tmp/$REPAIR(name)_6" "/tmp/$REPAIR(name)_o"
  $REPAIR(im) read "/tmp/$REPAIR(name)_o"
}

# action associated to quit widget
proc repair_quit {} {
  global REPAIR
  my_exec mv "/tmp/$REPAIR(name)_o" $REPAIR(outfilename)
  foreach file [glob -nocomplain "/tmp/$REPAIR(name)_?"] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW repair_quit
bind . "q" repair_quit
