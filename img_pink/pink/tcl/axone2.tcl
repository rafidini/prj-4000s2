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
# Axone2
# Michel couprie - sept 2002

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2 $3

# temp. images: 
# _0: grayscale image
# _1: gradient
# _2: current tubule pair
# _3: all tubule pairs
# _31: all tubule pairs (borders)
# _4: surimpression of _31 and original image
# _a: axes of tubule pairs

set PINK "$env(PINK)"

# pour la portabilite LINUX - WINDOWS(MinGW)
# LINUX:
set PORTABLE(tmpdir) "/tmp"
set PORTABLE(labelcommand) label
# WINDOWS:
#set PORTABLE(tmpdir) "\\tmp"
#set PORTABLE(labelcommand) [file join "$PINK" "bin" "label"]

source [file join "$PINK" "tcl" "my_exec.tcl"]
source [file join "$PINK" "tcl" "my_dialog.tcl"]

# set window title
wm title . "Tubule pair detection"
menu .menubar
. config -menu .menubar
foreach m {File Commands Help} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$File      add command -label "Save and quit"     -command axone2_quit
$File      add command -label "Quit without save" -command axone2_cancel
$Commands  add command -label "Reset"             -command axone2_reset
$Commands  add check   -label "Normalize"         -command axone2_normalize \
                       -variable AXONE2(normalized)
$Commands  add command -label "Check"             -command axone2_check
$Commands  add command -label "Rotate"            -command axone2_rotate

if {$argc != 3} { 
  puts stderr "usage: axone2 in rayon name"
  exit
}

# declare globals
global AXONE2
set AXONE2(infilename) [lindex $argv 0]
set AXONE2(rayon) [lindex $argv 1]
set AXONE2(nom_patient) [lindex $argv 2]
# rayon : global radius
# x  : pixel position in row
# y  : pixel position in column
# s  : current smoothing factor
# im : image structure
# rs : row size
# cs : col size
# name : command name
# normalized : boolean
set AXONE2(s) 0.7
set AXONE2(name) "axone2"
set AXONE2(normalized) 1
set AXONE2(r_in_mark) [expr ($AXONE2(rayon) * 8) / 145]
set AXONE2(r_out_mark) [expr ($AXONE2(rayon) * 35) / 145]
set AXONE2(r_open) [expr ($AXONE2(rayon) * 14) / 145]
set AXONE2(check_mode) 0
set AXONE2(wait_point) 0
set AXONE2(selected_tubules) 0

if ![file exists $AXONE2(infilename)] {
  tk_messageBox -type ok -icon error -message "File not found: $AXONE2(infilename)"
  exit
}

proc tmpfile {tmpname} {
  global AXONE2
  global PORTABLE
  return [file join $PORTABLE(tmpdir) "$AXONE2(name)_$tmpname"]
}

proc my_surimp {arg1 arg2 arg3} {
  global AXONE2
  my_exec inverse $arg2 [tmpfile s1]
  my_exec min $arg1 [tmpfile s1] $arg3
  my_exec add $arg3 $arg2 [tmpfile s1]  
  my_exec pgm2ppm [tmpfile s1] $arg3 $arg3 $arg3
}

proc my_bouchetrous {arg1 arg2 arg3} {
  global AXONE2
  global PORTABLE
  my_exec "$PORTABLE(labelcommand)" $arg1 $arg2 min [tmpfile bl]
  my_exec cadre $arg1 [tmpfile bc]
  my_exec seuil [tmpfile bl] 1 [tmpfile bs]
  my_exec geodilat [tmpfile bc] [tmpfile bs] $arg2 -1 [tmpfile br]
  my_exec inverse [tmpfile br] $arg3
}

proc my_skiz {arg1 arg2 arg3} {
  global AXONE2
  global PORTABLE
  my_exec dist $arg1 0 [tmpfile s1]
  my_exec long2byte [tmpfile s1] 2 [tmpfile s1]
  my_exec watershed [tmpfile s1] $arg1 null null $arg2 $arg3
}

my_exec dup $AXONE2(infilename) [tmpfile 0]
my_exec histstretch [tmpfile 0] 0 254 0.05 [tmpfile 0] 
my_exec gradientcd [tmpfile 0] 1 [tmpfile 1]
my_exec genimage $AXONE2(infilename) 0 [tmpfile 2]
my_exec dup [tmpfile 2]  [tmpfile 21] 
my_exec dup [tmpfile 2]  [tmpfile 3] 
my_exec dup [tmpfile 2]  [tmpfile 31] 

# create the rotate and check buttons
frame .buttons
pack .buttons -side bottom
button .buttons.rotate -text "Rotate" -command axone2_rotate
pack .buttons.rotate -side right
button .buttons.check -text "Check" -command axone2_check
pack .buttons.check -side right

# create an image and load contents from file 
set AXONE2(im) [image create photo imname -file [tmpfile 0]]

# get image size
set AXONE2(cs) [image height $AXONE2(im)]
set AXONE2(rs) [image width $AXONE2(im)]

# create a frame for the image
frame .imframe -borderwidth 10 -width $AXONE2(rs) -height $AXONE2(cs)
pack .imframe -side top

# create canvas with the right sizes
set c [canvas .imframe.c -width [expr $AXONE2(rs) - 2] -height [expr $AXONE2(cs) - 2]]
pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $AXONE2(im)

# bind actions to mouse events
bind .imframe.c <ButtonPress-1> { 
  set AXONE2(x) %x 
  set AXONE2(y) %y
  if {$AXONE2(check_mode) == 0} {
    my_exec genimage $AXONE2(rs) $AXONE2(cs) 1 0 [tmpfile 2]
    my_exec point [tmpfile 2] %x %y 0 255 [tmpfile 2]
    my_exec dilatball [tmpfile 2] $AXONE2(r_in_mark) [tmpfile 20]
    my_exec dilatball [tmpfile 2] $AXONE2(r_out_mark) [tmpfile 22]
    my_exec inverse [tmpfile 22] [tmpfile 22]
    my_exec add [tmpfile 22] [tmpfile 20] [tmpfile 2]
    my_exec watershed [tmpfile 1] [tmpfile 2] null null 4 [tmpfile 2]
    my_bouchetrous [tmpfile 2] 4 [tmpfile 23]
    my_exec openball [tmpfile 23] $AXONE2(r_open) [tmpfile 2]
    my_exec border [tmpfile 2] 8 [tmpfile 21]
    my_exec max [tmpfile 3] [tmpfile 2] [tmpfile 3] 
    my_exec max [tmpfile 31] [tmpfile 21] [tmpfile 31] 
    my_surimp [tmpfile 0] [tmpfile 31] [tmpfile 4]

    $AXONE2(im) read [tmpfile 4]
    set AXONE2(selected_tubules) 1
  } else {
    my_skiz [tmpfile a] 8 [tmpfile t]  
    my_exec inverse [tmpfile t] [tmpfile t]
    my_exec selectcomp [tmpfile t] 4 %x %y 0 [tmpfile t]
    my_exec min [tmpfile a] [tmpfile t] [tmpfile t]
    my_exec sub [tmpfile a] [tmpfile t] [tmpfile a]
    my_exec ptend [tmpfile t] 8 [tmpfile t]
    my_skiz [tmpfile t] 8 [tmpfile u]  
    my_exec inverse [tmpfile u] [tmpfile u]
    my_exec selectcomp [tmpfile u] 4 %x %y 0 [tmpfile u]
    my_exec sub [tmpfile t] [tmpfile u] [tmpfile t1]
    my_exec point [tmpfile t1] %x %y 0 255 [tmpfile t]
    my_exec line [tmpfile t] [tmpfile t2]
    my_exec max [tmpfile a] [tmpfile t2] [tmpfile a]
    my_surimp [tmpfile 0] [tmpfile a] [tmpfile 4] 
    $AXONE2(im) read [tmpfile 4]      
  }
}

# bind actions to mouse events
bind .imframe.c <B1-Motion> { 
  set AXONE2(x) %x 
  set AXONE2(y) %y
  if {$AXONE2(check_mode) != 0} {
    my_exec point [tmpfile t1] %x %y 0 255 [tmpfile t]
    my_exec line [tmpfile t] [tmpfile t3]
    my_exec sub [tmpfile a] [tmpfile t2] [tmpfile a]
    my_exec max [tmpfile a] [tmpfile t3] [tmpfile a]
    file copy -force [tmpfile t3] [tmpfile t2]
    my_surimp [tmpfile 0] [tmpfile a] [tmpfile 4] 
    $AXONE2(im) read [tmpfile 4]      
  }
}

bind .imframe.c <Button-3> { 
  set AXONE2(x) %x 
  set AXONE2(y) %y
  if {$AXONE2(check_mode) == 0} {
    my_skiz [tmpfile 3] 8 [tmpfile t]
    my_exec inverse [tmpfile t] [tmpfile t]
    my_exec selectcomp [tmpfile t] 4 %x %y 0 [tmpfile t]
    my_exec sub [tmpfile 3] [tmpfile t] [tmpfile 3]
    my_exec sub [tmpfile 31] [tmpfile t] [tmpfile 31]
    my_surimp [tmpfile 0] [tmpfile 31] [tmpfile 4]
    $AXONE2(im) read [tmpfile 4]
  } else {
    my_skiz [tmpfile a] 8 [tmpfile t]
    my_exec inverse [tmpfile t] [tmpfile t]
    my_exec selectcomp [tmpfile t] 4 %x %y 0 [tmpfile t]
    my_exec sub [tmpfile a] [tmpfile t] [tmpfile a]
    my_surimp [tmpfile 0] [tmpfile a] [tmpfile 4]
    $AXONE2(im) read [tmpfile 4]
  }
}

# action associated to reset menu option
proc axone2_reset {} {
  global AXONE2
  global PORTABLE
  if {$AXONE2(normalized)} {
    my_exec histstretch $AXONE2(infilename) 0 254 0.05 [tmpfile 0]
  } else {
    my_exec dup $AXONE2(infilename) [tmpfile 0] 
  }
  my_exec genimage $AXONE2(rs) $AXONE2(cs) 1 0 [tmpfile 2]
  my_exec dup [tmpfile 2] [tmpfile 3] 
  my_exec dup [tmpfile 2] [tmpfile 31] 
  file delete -force [tmpfile a] 
  file delete -force [tmpfile 4] 
  $AXONE2(im) read [tmpfile 0]
  set AXONE2(check_mode) 0
  set AXONE2(selected_tubules) 0
}

# action associated to normalize menu option
proc axone2_normalize {} {
  global AXONE2
  if {$AXONE2(check_mode) == 0} {
    if {$AXONE2(normalized)} {
      my_exec histstretch [tmpfile 0] 0 254 0.05 [tmpfile 0]
      my_surimp [tmpfile 0] [tmpfile 31] [tmpfile 4]
      $AXONE2(im) read [tmpfile 4]
    } else {
      my_exec dup $AXONE2(infilename) [tmpfile 0]
      my_surimp [tmpfile 0] [tmpfile 31] [tmpfile 4]
      $AXONE2(im) read [tmpfile 4]    
    }
  } else {
    if {$AXONE2(normalized)} {
      my_exec histstretch [tmpfile 0] 0 254 0.05 [tmpfile 0]
      my_surimp [tmpfile 0] [tmpfile a] [tmpfile 5]
      $AXONE2(im) read [tmpfile 5]
    } else {
      my_exec dup $AXONE2(infilename) [tmpfile 0]
      my_surimp [tmpfile 0] [tmpfile a] [tmpfile 5]
      $AXONE2(im) read [tmpfile 5]    
    }
  }
}

# action associated to check menu option
proc axone2_check {} {
  global AXONE2
  if {$AXONE2(selected_tubules) == 0} {
    tk_messageBox -type ok -icon error -message "Please select tubules first"
    return
  }
  my_exec matchrect [tmpfile 3] 4 axis [tmpfile a]
  if {$AXONE2(normalized)} {
    my_exec histstretch $AXONE2(infilename) 0 254 0.05 [tmpfile t]
    my_surimp [tmpfile t] [tmpfile a] [tmpfile 5]
  } else {
    my_surimp $AXONE2(infilename) [tmpfile a] [tmpfile 5]
  }
  $AXONE2(im) read [tmpfile 5]    
  set AXONE2(check_mode) 1
}

# action associated to rotate menu option
proc axone2_rotate {} {
  global AXONE2
  global PORTABLE
  if ![file exists [tmpfile a]] {
    tk_messageBox -type ok -icon error -message "Please check first"
    return
  }
  my_exec "$PORTABLE(labelcommand)" [tmpfile a] 8 max [tmpfile l]
  my_exec long2byte [tmpfile l] [tmpfile l]
  my_exec ptend [tmpfile a] 8 [tmpfile c]
  my_exec min [tmpfile c] [tmpfile l] [tmpfile c]
  if {$AXONE2(normalized)} {
    my_exec histstretch $AXONE2(infilename) 0 254 0.05 [tmpfile t]
    my_exec axones [tmpfile t] [tmpfile c] [tmpfile 5]
  } else {
    my_exec axones $AXONE2(infilename) [tmpfile c] [tmpfile 5]
  }
  my_exec isometry [tmpfile 5] yxz [tmpfile 5] 
  my_exec sym [tmpfile 5] v [tmpfile 5] 
  $AXONE2(im) read [tmpfile 5]    
}

# action associated to cancel menu option
proc axone2_cancel {} {
  global AXONE2
  global PORTABLE
  foreach file [glob -nocomplain [tmpfile "*"]] {
    file delete -force $file
  }
  exit
}

# action associated to quit menu option
proc axone2_quit {} {
  global AXONE2
  global PORTABLE
  set nimage [Dialog_Prompt "Enter image number:" ""]
  if {$nimage == ""} {
    return
  }  
  file copy -force [tmpfile 0] "$AXONE2(nom_patient)_$nimage.pgm"
  set nomres "$AXONE2(nom_patient)_$nimage"
  append nomres "r.pgm"
  file copy -force [tmpfile 5] $nomres
  foreach file [glob -nocomplain [tmpfile "*"]] {
    file delete -force $file
  }
  exit
}
wm protocol . WM_DELETE_WINDOW axone2_quit
bind . "q" axone2_quit
