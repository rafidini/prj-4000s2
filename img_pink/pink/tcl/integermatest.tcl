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
wm title . "Integer Medial Axis (Test)"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]

# reads a file
#-----------------------------------
proc my_read_val {filename} {
  set input [open $filename]
  set tag [gets $input]
  set line [gets $input]
  scan $line "%g" v1
  close $input
  return $v1
}

# globals
set INTEGERMATEST(infilename) [lindex $argv 0]
set INTEGERMATEST(outfilename) [lindex $argv 1]
#   INTEGERMATEST(im)
#   INTEGERMATEST(im_rs)
#   INTEGERMATEST(im_cs)
set INTEGERMATEST(combine) 0
set INTEGERMATEST(param) 0
set INTEGERMATEST(angle) 0
set INTEGERMATEST(name) integermatest

if {$argc != 2} { 
  puts stderr "usage: integermatest.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global INTEGERMATEST
  return [file join "/tmp" "$INTEGERMATEST(name)_$tmpname"]
}

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command integermatest_quit
pack .top.quit -side right

# create the combine button
set INTEGERMATEST(combine) 0
checkbutton .top.combine -text combine -variable INTEGERMATEST(combine) -command integermatest_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from 1 -to 100 -length 400 -variable INTEGERMATEST(param) \
  -orient horizontal -tickinterval 10 -showvalue true -command integermatest_run
pack .top.radius -side left

# create the angle button
scale .top.angle -from 0 -to 359 -length 300 -variable INTEGERMATEST(angle) \
  -orient horizontal -tickinterval 45 -showvalue true -command rotation_run
pack .top.angle -side left

# create an image and load contents from file 
set INTEGERMATEST(im) [image create photo imname -file "$INTEGERMATEST(infilename)"]

# get image size
set INTEGERMATEST(im_cs) [image height $INTEGERMATEST(im)]
set INTEGERMATEST(im_rs) [image width $INTEGERMATEST(im)]

puts "size : $INTEGERMATEST(im_rs) $INTEGERMATEST(im_cs)"

# create a frame for the image
frame .imframe -borderwidth 10 -width $INTEGERMATEST(im_rs) -height $INTEGERMATEST(im_cs)
pack .imframe -side top

puts "frame created"

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $INTEGERMATEST(im_rs) - 2] -height [expr $INTEGERMATEST(im_cs) - 2]]
pack .imframe.c -fill both -expand true

puts "canvas created"

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $INTEGERMATEST(im)

puts "image created"

# action associated to radius change
proc integermatest_run {radius} {
  global INTEGERMATEST
  global PINK
  my_exec $PINK/linux/bin/rotate $INTEGERMATEST(infilename) $INTEGERMATEST(angle) [expr $INTEGERMATEST(im_rs)/2] [expr $INTEGERMATEST(im_cs)/2] [tmpfile 0]
  my_exec $PINK/linux/bin/seuil [tmpfile 0] 128 [tmpfile 0]
  my_exec $PINK/linux/bin/areaclosing [tmpfile 0] 4 100 [tmpfile 0]
  my_exec $PINK/linux/bin/integermedialaxis [tmpfile 0] $radius [tmpfile 3]

  my_exec $PINK/linux/bin/distc $INTEGERMATEST(infilename) 3 [tmpfile "_d"]
  my_exec $PINK/linux/bin/normalize [tmpfile 3] 0 1 [tmpfile "_1"]
  my_exec $PINK/linux/bin/mult [tmpfile "_1"] [tmpfile "_d"] [tmpfile "_d1"]
  my_exec $PINK/linux/bin/redt [tmpfile "_d1"] [tmpfile "_rec"] 
  my_exec $PINK/linux/bin/sub $INTEGERMATEST(infilename) [tmpfile "_rec"] [tmpfile "_res"]
  my_exec $PINK/linux/bin/area [tmpfile "_res"] [tmpfile "_res"]
  set res [my_read_val [tmpfile  "_res"]]
  puts "residu = $res"

  if {$INTEGERMATEST(combine) == 0} {
    $INTEGERMATEST(im) read [tmpfile 3]
  } else {
    my_exec scale [tmpfile 0] 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $INTEGERMATEST(im) read [tmpfile 2]
  }
}

# action associated to angle change
proc rotation_run {angle} {
  global INTEGERMATEST
  global PINK
  my_exec $PINK/linux/bin/rotate $INTEGERMATEST(infilename) $angle [expr $INTEGERMATEST(im_rs)/2] [expr $INTEGERMATEST(im_cs)/2] [tmpfile 0]
  my_exec $PINK/linux/bin/seuil [tmpfile 0] 128 [tmpfile 0]
  my_exec $PINK/linux/bin/areaclosing [tmpfile 0] 4 100 [tmpfile 0]
  my_exec $PINK/linux/bin/integermedialaxis [tmpfile 0] $INTEGERMATEST(param) [tmpfile 3]
  if {$INTEGERMATEST(combine) == 0} {
    $INTEGERMATEST(im) read [tmpfile 3]
  } else {
    my_exec scale [tmpfile 0] 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $INTEGERMATEST(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc integermatest_combine {} {
  global INTEGERMATEST
  if {$INTEGERMATEST(combine) == 0} {
    $INTEGERMATEST(im) read [tmpfile 3]
  } else {
    my_exec scale $INTEGERMATEST(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $INTEGERMATEST(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc integermatest_quit {} {
  global INTEGERMATEST
  my_exec cp [tmpfile 3] $INTEGERMATEST(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW integermatest_quit
bind . "q" integermatest_quit
