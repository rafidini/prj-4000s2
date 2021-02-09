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
exec wish $0 $1 $2 $3 $4

# set window title
wm title . "Flow skeleton"

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
#   2DFLOWSKELETON(infilename)
#   2DFLOWSKELETON(outfilename)
#   2DFLOWSKELETON(mode)
#   2DFLOWSKELETON(im)
#   2DFLOWSKELETON(im_rs)
#   2DFLOWSKELETON(im_cs)
set 2DFLOWSKELETON(combine) 0
set 2DFLOWSKELETON(param) 1
set 2DFLOWSKELETON(maxparam) 100
set 2DFLOWSKELETON(name) 2dflowskeleton

if {($argc != 3) && ($argc != 4)} { 
  puts stderr "usage: 2dflowskeleton.tcl in mode \[maxparam\] out"
  exit 
}

proc tmpfile {tmpname} {
  global 2DFLOWSKELETON
  return [file join "/tmp" "$2DFLOWSKELETON(name)_$tmpname"]
}

# get input image file name as first argument
set 2DFLOWSKELETON(infilename) [lindex $argv 0]

# get mode as second argument
set 2DFLOWSKELETON(mode) [lindex $argv 1]

if {$argc == 3} { 
  # get output image file name as third argument
  set 2DFLOWSKELETON(outfilename) [lindex $argv 2]
}

if {$argc == 4} { 
  set 2DFLOWSKELETON(maxparam) [lindex $argv 2]
  set 2DFLOWSKELETON(outfilename) [lindex $argv 3]
}

my_exec_q $PINK/linux/bin/2dflowskeleton $2DFLOWSKELETON(infilename) $2DFLOWSKELETON(mode) [tmpfile _map]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command 2dflowskeleton_quit
pack .top.quit -side right

# create the combine button
set 2DFLOWSKELETON(combine) 0
checkbutton .top.combine -text combine -variable 2DFLOWSKELETON(combine) -command 2dflowskeleton_combine
pack .top.combine -side right

# create the radius button
scale .top.radius -from 1 -to $2DFLOWSKELETON(maxparam) -length 300 -variable 2DFLOWSKELETON(param) \
  -orient horizontal -tickinterval 0 -showvalue true -command 2dflowskeleton_run
pack .top.radius -side left

# create an image and load contents from file 
set 2DFLOWSKELETON(im) [image create photo imname -file "$2DFLOWSKELETON(infilename)"]

# get image size
set 2DFLOWSKELETON(im_cs) [image height $2DFLOWSKELETON(im)]
set 2DFLOWSKELETON(im_rs) [image width $2DFLOWSKELETON(im)]
my_exec genimage $2DFLOWSKELETON(im_rs) $2DFLOWSKELETON(im_cs) 1 0 [tmpfile v]

# create a frame for the image
frame .imframe -borderwidth 10 -width $2DFLOWSKELETON(im_rs) -height $2DFLOWSKELETON(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $2DFLOWSKELETON(im_rs) - 2] -height [expr $2DFLOWSKELETON(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $2DFLOWSKELETON(im)

# action associated to param change
proc 2dflowskeleton_run {param} {
  global 2DFLOWSKELETON
  global PINK
  my_exec $PINK/linux/bin/seuil [tmpfile _map] $2DFLOWSKELETON(param) [tmpfile 3]
  my_exec $PINK/linux/bin/distc $2DFLOWSKELETON(infilename) 3 [tmpfile "_d"]
  my_exec $PINK/linux/bin/normalize [tmpfile 3] 0 1 [tmpfile "_1"]
  my_exec $PINK/linux/bin/mult [tmpfile "_1"] [tmpfile "_d"] [tmpfile "_d1"]
  my_exec $PINK/linux/bin/redt [tmpfile "_d1"] [tmpfile "_rec"] 
  my_exec $PINK/linux/bin/sub $2DFLOWSKELETON(infilename) [tmpfile "_rec"] [tmpfile "_res"]
  my_exec $PINK/linux/bin/area [tmpfile "_res"] [tmpfile "_res"]
  set res [my_read_val [tmpfile  "_res"]]
  puts "residu = $res"

  if {$2DFLOWSKELETON(combine) == 0} {
    $2DFLOWSKELETON(im) read [tmpfile 3]
  } else {
    my_exec scale $2DFLOWSKELETON(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $2DFLOWSKELETON(im) read [tmpfile 2]
  }
}

# action associated to combine button
proc 2dflowskeleton_combine {} {
  global 2DFLOWSKELETON
  if {$2DFLOWSKELETON(combine) == 0} {
    $2DFLOWSKELETON(im) read [tmpfile 3]
  } else {
    my_exec scale $2DFLOWSKELETON(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $2DFLOWSKELETON(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc 2dflowskeleton_quit {} {
  global 2DFLOWSKELETON
  my_exec cp [tmpfile 3] $2DFLOWSKELETON(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW 2dflowskeleton_quit
bind . "q" 2dflowskeleton_quit



