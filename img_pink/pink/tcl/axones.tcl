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
wm title . "Main window"

# pour la portabilite LINUX - WINDOWS(MinGW)
# LINUX:
set PORTABLE(axone1) "axone1.tcl"
# WINDOWS:
#set PORTABLE(axone1) "axone1.bat"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_dialog.tcl"
source "$PINK/tcl/my_feedback.tcl"

global MAIN
set MAIN(name) main

# ============================================================
# MENU DEFINITION
# ============================================================

set modifier Alt
menu .menubar -tearoff 0
. config -menu .menubar
foreach m {File Help} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$File      add command -label "Open" -command main_open -underline 0
$File      add command -label "Quit" -command main_quit -underline 0

# ============================================================
# MAIN WINDOW DEFINITION
# ============================================================

# create the toplevel frame
frame .top
pack .top -side top -fill x

# create the image
set SEG2D(im) [image create photo imxyname -file "axones.gif"]

# get image size
set SEG2D(cs) [image height $SEG2D(im)]
set SEG2D(rs) [image width $SEG2D(im)]

# create frame for the image
frame .top.imframe -borderwidth 10 -width $SEG2D(rs) -height $SEG2D(cs)
pack .top.imframe -side top

# create canvas with the right size
set SEG2D(can) [canvas .top.imframe.can -width $SEG2D(rs) -height $SEG2D(cs)]
pack .top.imframe.can -fill both -expand true

# put the images into the canvas
$SEG2D(can) create image 1 1 -anchor nw -image $SEG2D(im)

#===========================================
# PROCEDURES
#===========================================

proc main_quit {} {
  exit
}

proc main_open {} {
  global MAIN
  global PINK
  global PORTABLE
  set MAIN(filename) [tk_getOpenFile ]
  set filename [file tail $MAIN(filename)]
  set pathname [file dirname $MAIN(filename)]
  set patientname [string range $filename 0 3]
  cd $pathname
  exec $PORTABLE(axone1) $MAIN(filename) $patientname &
}

wm protocol . WM_DELETE_WINDOW main_quit
bind . "q" main_quit



