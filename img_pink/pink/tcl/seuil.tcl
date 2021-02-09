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
  puts stderr "usage: seuil.tcl in out"
  exit 
}

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_read.tcl"

# globals
#   SEUIL(infilename)
#   SEUIL(outfilename)
#   SEUIL(im)
#   SEUIL(im_rs)
#   SEUIL(im_cs)
#   SEUIL(datatype)
set SEUIL(min) 0
set SEUIL(max) 255
set SEUIL(combine) 0
set SEUIL(inverse) 0
set SEUIL(param) 128
set SEUIL(name) seuil

proc tmpfile {tmpname} {
  global SEUIL
  return [file join "/tmp" "$SEUIL(name)_$tmpname"]
}

# reads a file for a value 
#-----------------------------------
proc my_read_val {filename} {
  set input [open $filename]
  set tag [gets $input]
  set line [gets $input]
  scan $line "%g %g" v1 v2
  close $input
  return $v2
}

# get input image file name as first argument
set SEUIL(infilename) [lindex $argv 0]

# get output image file name as second argument
set SEUIL(outfilename) [lindex $argv 1]

set hdr [my_readpgmheader $SEUIL(infilename)]
set SEUIL(datatype) [lindex $hdr 0]
if {$SEUIL(datatype) == "P5"} {
  # raw byte
  my_exec cp $SEUIL(infilename) [tmpfile 0]
} elseif {$SEUIL(datatype) == "P8"} {
  # raw long
  my_exec max1 $SEUIL(infilename) [tmpfile m]
  set SEUIL(max) [my_read_val [tmpfile m]]
  set SEUIL(param) [expr ($SEUIL(max) - $SEUIL(min)) / 2]
  my_exec long2byte $SEUIL(infilename) 2 [tmpfile 0]
} elseif {$SEUIL(datatype) == "P9"} {
  # raw float
  my_exec min1 $SEUIL(infilename) [tmpfile m]
  set SEUIL(min) [my_read_val [tmpfile m]]
  my_exec max1 $SEUIL(infilename) [tmpfile m]
  set SEUIL(max) [my_read_val [tmpfile m]]
  set SEUIL(param) [expr ($SEUIL(max) - $SEUIL(min)) / 2]
  my_exec float2byte $SEUIL(infilename) 2 [tmpfile 0]
} else {
  puts stderr "$SEUIL(name): bad datatype: $SEUIL(datatype)"
  exit 
}

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit button
button .top.quit -text Quit -command seuil_quit
pack .top.quit -side right

# create the combine button
checkbutton .top.combine -text combine -variable SEUIL(combine) -command seuil_combine
pack .top.combine -side right

# create the inverse button
checkbutton .top.inverse -text inverse -variable SEUIL(inverse) -command seuil_inverse
pack .top.inverse -side right

# create the param button
scale .top.param -from $SEUIL(min) -to $SEUIL(max) -length 200 -variable SEUIL(param) \
  -orient horizontal -tickinterval 0 -showvalue true -command seuil_run
pack .top.param -side left

# create an image and load contents from file 
set SEUIL(im) [image create photo imname -file "[tmpfile 0]"]

# get image size
set SEUIL(im_cs) [image height $SEUIL(im)]
set SEUIL(im_rs) [image width $SEUIL(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $SEUIL(im_rs) -height $SEUIL(im_cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $SEUIL(im_rs) - 2] -height [expr $SEUIL(im_cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $SEUIL(im)

# action associated to param. change
proc seuil_run {param} {
  global SEUIL
  global PINK
  my_exec_f $PINK/linux/bin/seuil $SEUIL(infilename) $param $SEUIL(outfilename)
  if {$SEUIL(inverse) != 0} {
    my_exec $PINK/linux/bin/inverse $SEUIL(outfilename) $SEUIL(outfilename)
  }
  if {$SEUIL(combine) == 0} {
    $SEUIL(im) read $SEUIL(outfilename)
  } else {
      my_exec $PINK/linux/bin/surimp [tmpfile 0] $SEUIL(outfilename) [tmpfile 1]
    $SEUIL(im) read [tmpfile 1]
  }
}

# action associated to combine button
proc seuil_combine {} {
  global SEUIL
  global PINK
  if {$SEUIL(combine) == 1} {
    my_exec $PINK/linux/bin/surimp [tmpfile 0] $SEUIL(outfilename) [tmpfile 1]
    $SEUIL(im) read [tmpfile 1]
  } else {
    $SEUIL(im) read $SEUIL(outfilename)    
  }
}

# action associated to inverse button
proc seuil_inverse {} {
  global SEUIL
  seuil_run $SEUIL(param)
}

# action associated to quit widget
proc seuil_quit {} {
  global SEUIL
  foreach file [glob -nocomplain [tmpfile "?"]] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW seuil_quit
bind . "q" seuil_quit
