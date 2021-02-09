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
wm title . "Lambda Medial Axis"

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
proc my_read_valf {filename} {
  set input [open $filename]
  set line [gets $input]
  scan $line "%f" v1
  close $input
  return $v1
}


# globals
#   LAMBDAMEDIALAXIS(infilename)
#   LAMBDAMEDIALAXIS(outfilename)
#   LAMBDAMEDIALAXIS(im)
#   LAMBDAMEDIALAXIS(im_rs)
#   LAMBDAMEDIALAXIS(im_cs)
set LAMBDAMEDIALAXIS(combine) 1
set LAMBDAMEDIALAXIS(topo) 0
set LAMBDAMEDIALAXIS(param) 0
set LAMBDAMEDIALAXIS(name) lambdamedialaxis

if {$argc != 2} { 
  puts stderr "usage: lambdamedialaxis.tcl in out"
  exit 
}

proc tmpfile {tmpname} {
  global LAMBDAMEDIALAXIS
  return [file join "/tmp" "$LAMBDAMEDIALAXIS(name)_$tmpname"]
}

# get input image file name as first argument
set LAMBDAMEDIALAXIS(infilename) [lindex $argv 0]

# get output image file name as second argument
set LAMBDAMEDIALAXIS(outfilename) [lindex $argv 1]

# compute the lambda map - store in tmpfile 0
my_exec $PINK/linux/bin/lambdamedialaxis $LAMBDAMEDIALAXIS(infilename) [tmpfile 0]

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the Auto button
button .top.auto -text Auto -command lambdamedialaxis_auto
pack .top.auto -side left

# create the quit button
button .top.quit -text Quit -command lambdamedialaxis_quit
pack .top.quit -side right

# create the combine button
checkbutton .top.combine -text combine -variable LAMBDAMEDIALAXIS(combine) -command lambdamedialaxis_combine
pack .top.combine -side right

# create the topology button
set LAMBDAMEDIALAXIS(topo) 0
checkbutton .top.topo -text topo -variable LAMBDAMEDIALAXIS(topo) -command lambdamedialaxis_topo
pack .top.topo -side right

# create the radius button
label .top.radiuslabel -text "lambda"
scale .top.radius -from 1 -to 100 -length 400 -variable LAMBDAMEDIALAXIS(param) \
  -orient horizontal -tickinterval 10 -showvalue true -command lambdamedialaxis_run
pack .top.radiuslabel .top.radius -side left

# create an image and load contents from file 
set LAMBDAMEDIALAXIS(im) [image create photo imname -file "$LAMBDAMEDIALAXIS(infilename)"]

# get image size
set LAMBDAMEDIALAXIS(im_cs) [image height $LAMBDAMEDIALAXIS(im)]
set LAMBDAMEDIALAXIS(im_rs) [image width $LAMBDAMEDIALAXIS(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $LAMBDAMEDIALAXIS(im_rs) -height $LAMBDAMEDIALAXIS(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $LAMBDAMEDIALAXIS(im_rs) - 2] -height [expr $LAMBDAMEDIALAXIS(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $LAMBDAMEDIALAXIS(im)

# action associated to radius change
proc lambdamedialaxis_run {radius} {
  global LAMBDAMEDIALAXIS
  global PINK
  #my_exec $PINK/linux/bin/seuil [tmpfile 0] $radius [tmpfile 3]
  my_exec $PINK/linux/bin/lambdamedialaxisauto $LAMBDAMEDIALAXIS(infilename) $LAMBDAMEDIALAXIS(param) [tmpfile 3]
  if {$LAMBDAMEDIALAXIS(topo) != 0} {
    my_exec $PINK/linux/bin/skeleton $LAMBDAMEDIALAXIS(infilename) [tmpfile 0] 8 [tmpfile 3] [tmpfile 3]
  }

  my_exec $PINK/linux/bin/distc $LAMBDAMEDIALAXIS(infilename) 3 [tmpfile "_d"]
  my_exec $PINK/linux/bin/normalize [tmpfile 3] 0 1 [tmpfile "_1"]
  my_exec $PINK/linux/bin/mult [tmpfile "_1"] [tmpfile "_d"] [tmpfile "_d1"]
  my_exec $PINK/linux/bin/redt [tmpfile "_d1"] [tmpfile "_rec"] 
  my_exec $PINK/linux/bin/sub $LAMBDAMEDIALAXIS(infilename) [tmpfile "_rec"] [tmpfile "_res"]
  my_exec $PINK/linux/bin/area [tmpfile "_res"] [tmpfile "_res"]
  set res [my_read_val [tmpfile  "_res"]]
  puts "residu = $res"

  if {$LAMBDAMEDIALAXIS(combine) == 0} {
    $LAMBDAMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $LAMBDAMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to auto button
proc lambdamedialaxis_auto {} {
  global LAMBDAMEDIALAXIS
  global PINK
  my_exec $PINK/linux/bin/lambdamedialaxisauto $LAMBDAMEDIALAXIS(infilename) 0 [tmpfile 3] > [tmpfile  "_param"]
  
  set LAMBDAMEDIALAXIS(param) [my_read_valf [tmpfile  "_param"]]
  
  if {$LAMBDAMEDIALAXIS(topo) != 0} {
    my_exec $PINK/linux/bin/skeleton $LAMBDAMEDIALAXIS(infilename) [tmpfile 0] 8 [tmpfile 3] [tmpfile 3]
  }

  my_exec $PINK/linux/bin/distc $LAMBDAMEDIALAXIS(infilename) 3 [tmpfile "_d"]
  my_exec $PINK/linux/bin/normalize [tmpfile 3] 0 1 [tmpfile "_1"]
  my_exec $PINK/linux/bin/mult [tmpfile "_1"] [tmpfile "_d"] [tmpfile "_d1"]
  my_exec $PINK/linux/bin/redt [tmpfile "_d1"] [tmpfile "_rec"] 
  my_exec $PINK/linux/bin/sub $LAMBDAMEDIALAXIS(infilename) [tmpfile "_rec"] [tmpfile "_res"]
  my_exec $PINK/linux/bin/area [tmpfile "_res"] [tmpfile "_res"]
  set res [my_read_val [tmpfile  "_res"]]
  puts "residu = $res"

  if {$LAMBDAMEDIALAXIS(combine) == 0} {
    $LAMBDAMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $LAMBDAMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMEDIALAXIS(im) read [tmpfile 2]
  }
}


# action associated to combine button
proc lambdamedialaxis_combine {} {
  global LAMBDAMEDIALAXIS
  if {$LAMBDAMEDIALAXIS(combine) == 0} {
    $LAMBDAMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $LAMBDAMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to topo button
proc lambdamedialaxis_topo {} {
  global LAMBDAMEDIALAXIS
  global PINK
  #my_exec $PINK/linux/bin/seuil [tmpfile 0] $LAMBDAMEDIALAXIS(param) [tmpfile 3]
  my_exec $PINK/linux/bin/lambdamedialaxisauto $LAMBDAMEDIALAXIS(infilename) $LAMBDAMEDIALAXIS(param) [tmpfile 3]
  if {$LAMBDAMEDIALAXIS(topo) != 0} {
    my_exec $PINK/linux/bin/skeleton $LAMBDAMEDIALAXIS(infilename) [tmpfile 0] 8 [tmpfile 3] [tmpfile 3]
  }
  if {$LAMBDAMEDIALAXIS(combine) == 0} {
    $LAMBDAMEDIALAXIS(im) read [tmpfile 3]
  } else {
    my_exec scale $LAMBDAMEDIALAXIS(infilename) 0.5 [tmpfile 1]
    my_exec scale [tmpfile 3] 0.5 [tmpfile 2]
    my_exec add [tmpfile 1] [tmpfile 2] [tmpfile 2]
    $LAMBDAMEDIALAXIS(im) read [tmpfile 2]
  }
}

# action associated to quit widget
proc lambdamedialaxis_quit {} {
  global LAMBDAMEDIALAXIS
  my_exec cp [tmpfile 3] $LAMBDAMEDIALAXIS(outfilename)
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW lambdamedialaxis_quit
bind . "q" lambdamedialaxis_quit
