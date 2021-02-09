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

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_dialog.tcl"
source "$PINK/tcl/my_feedback.tcl"

global MAIN
set MAIN(name) main

proc tmpfile {tmpname} {
  global MAIN
  return [file join "/tmp" "$MAIN(name)_$tmpname"]
}

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

$File      add command -label "Quit" -command main_quit -underline 0
$File      add command -label "Open" -command main_open -underline 0

# ============================================================
# MAIN WINDOW DEFINITION
# ============================================================

# create the toplevel frame
frame .top -borderwidth 10 -width 100 -height 40
pack .top -side top -fill x

#===========================================
# PROCEDURES
#===========================================

proc main_quit {} {
  global MAIN
  foreach file [glob -nocomplain [tmpfile *]] {
    exec rm $file
  }
  exit
}

proc main_open {} {
  global MAIN
  set MAIN(filename) [tk_getOpenFile ]
  puts $MAIN(filename)
}

wm protocol . WM_DELETE_WINDOW main_quit
bind . "q" main_quit

