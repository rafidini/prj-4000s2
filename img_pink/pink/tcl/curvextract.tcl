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
# Curve extractor
# Michel couprie - octobre 2002

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

# set window title
wm title . "Curve extractor"

set PINK "$env(HOME)/Pink"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_scanvas.tcl"

if {$argc != 2} { 
  puts stderr "usage: curvextract in out"
  exit 
}

# declare globals
global CURVEXTRACT
# infilename  : input file name
# outfilename : output file name
# im : image structure
# rs : row size
# cs : col size
# x1, y1, x2, y2 : coordinates of curve end points
# x0, y0 : coordinates of initial point
set CURVEXTRACT(x1) -1
set CURVEXTRACT(x2) -1
set CURVEXTRACT(alpha) 0.5
set CURVEXTRACT(connex) 4
set CURVEXTRACT(gradient) 1
set CURVEXTRACT(smooth) 0.0
set CURVEXTRACT(visu) 0
set CURVEXTRACT(name) "curvextract"

# get input image file name as first argument
set CURVEXTRACT(infilename) [lindex $argv 0]

# get output image file name as second argument
set CURVEXTRACT(outfilename) [lindex $argv 1]

# create a frame for buttons
frame .but -borderwidth 10
pack .but -side top -fill x

# create a frame for sliders
frame .sli -borderwidth 10
pack .sli -side top -fill x

# create the quit button
button .but.quit -text Quit -command curvextract_quit
pack .but.quit -side right

# create the reset button
button .but.reset -text Reset -command curvextract_reset
pack .but.reset -side right

# create the undo button
button .but.undo -text Undo -command curvextract_undo
pack .but.undo -side right

# create the accu button
button .but.accu -text Accu -command curvextract_accu
pack .but.accu -side right

# create the gradient checkbutton
checkbutton .but.gradient -text Gradient -variable CURVEXTRACT(gradient) -command curvextract_grad
pack .but.gradient -side right

# create the visu checkbutton
checkbutton .but.visu -text Vs -variable CURVEXTRACT(visu) -command curvextract_visu
pack .but.visu -side right

# create the connex buttons
radiobutton .but.connex4 -variable CURVEXTRACT(connex) -text "4" -value 4
pack .but.connex4 -side left
radiobutton .but.connex8 -variable CURVEXTRACT(connex) -text "8" -value 8
pack .but.connex8 -side left

# create the sliders
label .sli.alphaname -text "Alpha"
scale .sli.alpha -from 0.0 -to 1.0 -length 100 -variable CURVEXTRACT(alpha) \
  -orient horizontal -tickinterval 0 -showvalue true -digits 3 -resolution 0.01
pack .sli.alphaname .sli.alpha -side left

label .sli.smoothname -text "Smooth"
scale .sli.smooth -from -4.0 -to 4.0 -length 100 -variable CURVEXTRACT(smooth) \
  -orient horizontal -tickinterval 0 -showvalue true -digits 3 -resolution 0.05 \
  -command curvextract_smooth
pack .sli.smoothname .sli.smooth -side left

# create an image and load contents from file 
# _0 : image originale
# _1 : image affichee
# _c : image contours (im. or. ou gradient)
# _2 : image binaire (contrainte)
# _3 : image ndg (priorite)
# _4 : resultat temporaire
# _9 : resultat accumule
# _9s : resultat accumule anterieur (undo)
# _10 : accumulation des differents contours
my_exec_f cp $CURVEXTRACT(infilename) "/tmp/$CURVEXTRACT(name)_0"
my_exec_f cp $CURVEXTRACT(infilename) "/tmp/$CURVEXTRACT(name)_1"
if {$CURVEXTRACT(gradient) == 1} {
  set s [expr - $CURVEXTRACT(smooth)]
  if {$s < 0} {
    set s [expr ($s / 4) + 1]
  } else {
    set s [expr $s + 1]
  }
  my_exec gradientcd $CURVEXTRACT(infilename) $s "/tmp/$CURVEXTRACT(name)_c"
} else {
  my_exec_f cp $CURVEXTRACT(infilename) "/tmp/$CURVEXTRACT(name)_c"
}
set CURVEXTRACT(im) [image create photo imname -file "/tmp/$CURVEXTRACT(name)_0"]
my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_2"
my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_4"
my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_9"
my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_9s"
my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_10"

# get image size
set CURVEXTRACT(cs) [image height $CURVEXTRACT(im)]
set CURVEXTRACT(rs) [image width $CURVEXTRACT(im)]
set ca_w $CURVEXTRACT(rs)
set ca_h $CURVEXTRACT(cs)
if {$ca_w > 800} {set ca_w 800}
if {$ca_h > 500} {set ca_h 500}

# create a frame for the image
frame .imframe -borderwidth 10 -width $ca_w -height $ca_h
pack .imframe -side top

# create canvas with the right sizes
set c [my_scanvas .imframe.c -width $ca_w -height $ca_h \
	-scrollregion "0 0 $CURVEXTRACT(rs) $CURVEXTRACT(cs)"]

pack .imframe.c -fill both -expand true

# put the image into the canvas
$c create image 1 1 -anchor nw  -image $CURVEXTRACT(im)

# bind actions to mouse events
bind $c <Button-1> { 
  set x [$c canvasx %x] 
  set y [$c canvasy %y]
  if {$CURVEXTRACT(x2) < 0} {
    curvextract_set1 $x $y
    curvextract_visu
  } else {
    curvextract_set2 $x $y
    curvextract_visu
  }
}
bind $c <Button-3> { 
  if {$CURVEXTRACT(x2) < 0} {
  } else {
    curvextract_set2 $CURVEXTRACT(x0) $CURVEXTRACT(y0)
    my_exec squelbin "/tmp/$CURVEXTRACT(name)_9" $CURVEXTRACT(connex) -1 "/tmp/$CURVEXTRACT(name)_9"
    curvextract_visu
  }
}

proc curvextract_set1 {x y} {
  global CURVEXTRACT
  set CURVEXTRACT(x0) $x
  set CURVEXTRACT(y0) $y
  set CURVEXTRACT(x2) $x
  set CURVEXTRACT(y2) $y
  my_exec point "/tmp/$CURVEXTRACT(name)_2" $x $y 0 255 "/tmp/$CURVEXTRACT(name)_2"
  my_exec cp "/tmp/$CURVEXTRACT(name)_2" "/tmp/$CURVEXTRACT(name)_9"
}

proc curvextract_set2 {x y} {
  global CURVEXTRACT
  set t0 "/tmp/$CURVEXTRACT(name)_0"
  set t1 "/tmp/$CURVEXTRACT(name)_1"
  set tc "/tmp/$CURVEXTRACT(name)_c"
  set t2 "/tmp/$CURVEXTRACT(name)_2"
  set t3 "/tmp/$CURVEXTRACT(name)_3"
  set t4 "/tmp/$CURVEXTRACT(name)_4"
  set t5 "/tmp/$CURVEXTRACT(name)_5"
  set t9 "/tmp/$CURVEXTRACT(name)_9"
  set t9s "/tmp/$CURVEXTRACT(name)_9s"
  my_exec sub $t2 $t2 $t2
  my_exec point $t2 $CURVEXTRACT(x2) $CURVEXTRACT(y2) 0 255 $t2
  my_exec point $t2 $x $y 0 255 $t2
  my_exec dist $t2 0 $t3
  my_exec byte2long $tc $t4 
  my_exec scale $t3 [expr 100.0 * (1 - $CURVEXTRACT(alpha))] $t3
  my_exec scale $t4 [expr 100.0 * $CURVEXTRACT(alpha)] $t4
  my_exec addconst $t4 1000000 $t4
  my_exec sub $t4 $t3 $t3
  my_exec genimage $t0 255 $t4
  my_exec cadre $t4 $t5
  my_exec sub $t4 $t5 $t4
  my_exec skeleton $t4 $t3 $CURVEXTRACT(connex) $t2 $t4 
  my_exec cp $t9 $t9s
  my_exec max $t9 $t4 $t9
  set CURVEXTRACT(x1) $CURVEXTRACT(x2)
  set CURVEXTRACT(y1) $CURVEXTRACT(y2)
  set CURVEXTRACT(x2) $x
  set CURVEXTRACT(y2) $y
}

# action: visualize
proc curvextract_visu {} {
  global CURVEXTRACT
  if {$CURVEXTRACT(visu)} {
    if {$CURVEXTRACT(gradient)} {
      my_exec inverse "/tmp/$CURVEXTRACT(name)_c" "/tmp/$CURVEXTRACT(name)_1"
      my_exec surimp "/tmp/$CURVEXTRACT(name)_1" "/tmp/$CURVEXTRACT(name)_9" "/tmp/$CURVEXTRACT(name)_1"
    } else {
      my_exec surimp "/tmp/$CURVEXTRACT(name)_c" "/tmp/$CURVEXTRACT(name)_9" "/tmp/$CURVEXTRACT(name)_1"
      }
  } else {
    my_exec surimp "/tmp/$CURVEXTRACT(name)_0" "/tmp/$CURVEXTRACT(name)_9" "/tmp/$CURVEXTRACT(name)_1"
  }
  $CURVEXTRACT(im) read "/tmp/$CURVEXTRACT(name)_1"
}

# action associated to reset widget
proc curvextract_reset {} {
  global CURVEXTRACT
  set CURVEXTRACT(x1) -1
  set CURVEXTRACT(x2) -1
  my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_9"
  my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_2"
  curvextract_visu
}

# action associated to gradient widget
proc curvextract_grad {} {
  global CURVEXTRACT
  set CURVEXTRACT(x1) -1
  set CURVEXTRACT(x2) -1
  my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_9"
  my_exec_f genimage $CURVEXTRACT(infilename) 0 "/tmp/$CURVEXTRACT(name)_2"
  if {$CURVEXTRACT(gradient)} {
    set s $CURVEXTRACT(smooth)
    if {$s < 0} {
      set s [expr ($s / 4) + 1]
    } else {
      set s [expr $s + 1]
    }
    my_exec_f gradientcd $CURVEXTRACT(infilename) $s "/tmp/$CURVEXTRACT(name)_c"
  } else {
    my_exec_f cp $CURVEXTRACT(infilename) "/tmp/$CURVEXTRACT(name)_c"
  }
  curvextract_visu
}

# action associated to smooth widget
proc curvextract_smooth {s} {
  global CURVEXTRACT
  set s $CURVEXTRACT(smooth)
  if {$s < 0} {
    set s [expr ($s / 4) + 1]
  } else {
    set s [expr $s + 1]
  }
  if {$CURVEXTRACT(gradient)} {
    my_exec_f gradientcd $CURVEXTRACT(infilename) $s "/tmp/$CURVEXTRACT(name)_c"
  } else {
  }
  curvextract_visu
}

# action associated to undo widget
proc curvextract_undo {} {
  global CURVEXTRACT
  if {$CURVEXTRACT(x1) > -1} {
    set CURVEXTRACT(x2) $CURVEXTRACT(x1)
    set CURVEXTRACT(y2) $CURVEXTRACT(y1)
    set CURVEXTRACT(x1) -1
    my_exec cp "/tmp/$CURVEXTRACT(name)_9s" "/tmp/$CURVEXTRACT(name)_9"
    curvextract_visu
  }
}

# action associated to accu widget
proc curvextract_accu {} {
  global CURVEXTRACT
  my_exec max "/tmp/$CURVEXTRACT(name)_9" "/tmp/$CURVEXTRACT(name)_10" "/tmp/$CURVEXTRACT(name)_10"
  curvextract_reset
}

# action associated to quit widget
proc curvextract_quit {} {
  global CURVEXTRACT
  my_exec max "/tmp/$CURVEXTRACT(name)_9" "/tmp/$CURVEXTRACT(name)_10" "/tmp/$CURVEXTRACT(name)_10"
  my_exec cp "/tmp/$CURVEXTRACT(name)_10" $CURVEXTRACT(outfilename)
  foreach file [glob -nocomplain "/tmp/$CURVEXTRACT(name)_?"] {
    catch {exec rm $file} result
  }
  exit
}
wm protocol . WM_DELETE_WINDOW curvextract_quit
bind . "q" curvextract_quit
