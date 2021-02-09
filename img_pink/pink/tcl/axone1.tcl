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

# pour la portabilite LINUX - WINDOWS(MinGW)
# LINUX:
set PORTABLE(tmpdir) "/tmp"
set PORTABLE(axone2) "axone2.tcl"
# WINDOWS:
#set PORTABLE(tmpdir) "\\tmp"
#set PORTABLE(axone2) "axone2.bat"

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_exec.tcl"]
source [file join "$PINK" "tcl" "my_scanvas.tcl"]
source [file join "$PINK" "tcl" "my_dialog.tcl"]

# globals
#   AXONE1(im)
#   AXONE1(ca)
#   AXONE1(im_rs)
#   AXONE1(im_cs)
#   AXONE1(y)
set AXONE1(x) -1
set AXONE1(rayon) 56
set AXONE1(name) axone1
if {$argc == 2} {
  set AXONE1(infilename) [lindex $argv 0]
  set AXONE1(nom_patient) [lindex $argv 1]
} else {
  tk_messageBox -type ok -icon error -message "usage: axone1 infilename nom_patient"
  exit 
}

# set window title
wm title . "Axones: $AXONE1(infilename)"

proc tmpfile {tmpname} {
  global AXONE1
  global PORTABLE
  return [file join $PORTABLE(tmpdir) "$AXONE1(name)_$tmpname"]
}

proc my_surimp {arg1 arg2 arg3} {
  global AXONE1
  my_exec inverse $arg2 [tmpfile s1]
  my_exec min $arg1 [tmpfile s1] $arg3
  my_exec add $arg3 $arg2 [tmpfile s1]  
  my_exec pgm2ppm [tmpfile s1] $arg3 $arg3 $arg3
}

# action associated to param. change
proc axone1_rchange {rayon} {
  global AXONE1
  if {$AXONE1(x) >= 0} {
    my_exec dilatball [tmpfile 2] $rayon [tmpfile 3]
    my_exec border [tmpfile 3] 8 [tmpfile 3]
    my_surimp [tmpfile 0] [tmpfile 3] [tmpfile 1]
    $AXONE1(im) read [tmpfile 1]
  }
}

# action associated to coord. change
proc axone1_xychange {} {
  global AXONE1
  my_exec genimage $AXONE1(im_rs) $AXONE1(im_cs) 1 0 [tmpfile 2]
  my_exec point [tmpfile 2] $AXONE1(x) $AXONE1(y) 0 255 [tmpfile 2]
  my_exec dilatball [tmpfile 2] $AXONE1(rayon) [tmpfile 3]
  my_exec border [tmpfile 3] 8 [tmpfile 3]
  my_surimp [tmpfile 0] [tmpfile 3] [tmpfile 1]
  $AXONE1(im) read [tmpfile 1]
}

# action associated to run2 widget
proc axone1_run2 {} {
  global AXONE1
  global PORTABLE
  if {$AXONE1(x) == -1} {
    tk_messageBox -type ok -icon error -message "Please select axone first"
    return
  }
  set r [expr $AXONE1(rayon) * 2]
  set x [expr $AXONE1(x) * 2]
  set y [expr $AXONE1(y) * 2]
  set rs [expr $r * 2]
  set cs $rs
  my_exec crop $AXONE1(infilename) \
               [expr $x - $r] [expr $y - $r] \
               $rs $cs [tmpfile 4]
  my_exec genimage $rs $cs 1 0 [tmpfile 5]
  my_exec point [tmpfile 5] $r $r 0 255 [tmpfile 5]
  my_exec dilatball [tmpfile 5] $r [tmpfile 5]
  my_exec inverse [tmpfile 5] [tmpfile 5]
  my_exec max [tmpfile 4] [tmpfile 5] [tmpfile 4]
  exec $PORTABLE(axone2) [tmpfile 4] $r $AXONE1(nom_patient) &
}

# action associated to quit widget
proc axone1_quit {} {
  global AXONE1
  global PORTABLE
  foreach file [glob -nocomplain [tmpfile "?"]] {
    file delete -force $file
  }
  exit
}

##########################################
# MAIN PROGRAM
##########################################

  my_exec_f zoom $AXONE1(infilename) 0.5 [tmpfile 0]
  my_exec_f dup [tmpfile 0] [tmpfile 1]
  
  wm protocol . WM_DELETE_WINDOW axone1_quit
  bind . "q" axone1_quit
  
  # create a frame for buttons
  frame .top -borderwidth 10
  pack .top -side top -fill x
  
  # create the quit button
  button .top.quit -text Quit -command axone1_quit
  pack .top.quit -side right
  
  # create the run2 button
  button .top.run2 -text "Process" -command axone1_run2
  pack .top.run2 -side right
  
  # create the param button
  scale .top.rayon -from 0 -to 150 -length 150 -variable AXONE1(rayon) \
    -orient horizontal -tickinterval 0 -showvalue true -command axone1_rchange
  pack .top.rayon -side left
  
  # create an image and load contents from file 
  set AXONE1(im) [image create photo imname -file [tmpfile 1]]
  
  # get image size
  set AXONE1(im_cs) [image height $AXONE1(im)]
  set AXONE1(im_rs) [image width $AXONE1(im)]
  set ca_w $AXONE1(im_rs)
  set ca_h $AXONE1(im_cs)
  if {$ca_w > 800} {set ca_w 800}
  if {$ca_h > 500} {set ca_h 500}
  
  # create a frame for the image
  frame .imframe -borderwidth 10 -width $ca_w -height $ca_h
  pack .imframe -side top
  
  # create canvas with the right sizes
  set AXONE1(ca) [my_scanvas .imframe.c -width $ca_w -height $ca_h \
  	-scrollregion "0 0 $AXONE1(im_rs) $AXONE1(im_cs)"]
  pack .imframe.c -fill both -expand true
  
  # put the image into the canvas
  $AXONE1(ca) create image 1 1 -anchor nw  -image $AXONE1(im)
  
  # bind actions to mouse events
  bind $AXONE1(ca) <Button> { 
    # Map from view coordinates to canvas coordinates
    set AXONE1(x) [$AXONE1(ca) canvasx %x] 
    set AXONE1(y) [$AXONE1(ca) canvasy %y]
    axone1_xychange
  }
  
  # bind actions to keyboard events
  bind . <KeyPress> {
    if {$AXONE1(x) == -1} {
      tk_messageBox -type ok -icon error -message "Please select axone first"
      return
    }
    if {"%K" == "Left"}  { set AXONE1(x) [expr $AXONE1(x) - 1] }
    if {"%K" == "Right"} { set AXONE1(x) [expr $AXONE1(x) + 1] }
    if {"%K" == "Up"}    { set AXONE1(y) [expr $AXONE1(y) - 1] }
    if {"%K" == "Down"}  { set AXONE1(y) [expr $AXONE1(y) + 1] }
    axone1_xychange  
  }

