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
#!/bin/sh
# Run wish from the users PATH \
exec wish $0 $1

# Khalimsky grid editor
# Michel Couprie - juillet 2005

if {$argc != 1} { 
  puts stderr "usage: khalview filename"
  exit 
}

# declare globals
global KHALVIEW

# set globals
set KHALVIEW(name) "khalview"
set KHALVIEW(pointsize) 15
set KHALVIEW(incrsize) 3
set KHALVIEW(pointgap)  30
set KHALVIEW(margin) 10
set KHALVIEW(revert_mode) 0
# current pixel position
set KHALVIEW(x) 0
set KHALVIEW(y) 0
# argument values
set KHALVIEW(filename) [lindex $argv 0]
# read pgm image header
set input [open $KHALVIEW(filename)]
set tag [gets $input]
set line [gets $input]
while {[string index $line 0] == "#"} {
  set line [gets $input]
}
scan $line "%d %d" KHALVIEW(rs) KHALVIEW(cs)
close $input

# canvas size
set KHALVIEW(ca_rs) [expr ($KHALVIEW(rs)-1) * $KHALVIEW(pointgap) + \
                        $KHALVIEW(pointsize) + $KHALVIEW(margin) * 2]
set KHALVIEW(ca_cs) [expr ($KHALVIEW(cs)-1) * $KHALVIEW(pointgap) + \
                        $KHALVIEW(pointsize) + $KHALVIEW(margin) * 2]
# colors
set KHALVIEW(fgcol) red
set KHALVIEW(bgcol) blue
set KHALVIEW(mkcol) green

# set window title
wm title . "Khalview"

# ============================================================
# BUTTONS
# ============================================================

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit command button
button .top.quit -text "Quit" -command KHALVIEW_quit
pack .top.quit -side right

# create a frame for the image
frame .imframe -borderwidth 10 -width $KHALVIEW(ca_rs) -height $KHALVIEW(ca_cs)
pack .imframe -side top

# create canvas with the right sizes
set KHALVIEW(c) [canvas .imframe.c -width [expr $KHALVIEW(ca_rs) - 2] -height [expr $KHALVIEW(ca_cs) - 2]]
pack .imframe.c -fill both -expand true

# ============================================================
# MAIN
# ============================================================

  # read data
  set tmpfile "/tmp/myreadpgmdata_tmp"
  catch { exec pgmrawmode $KHALVIEW(filename) $tmpfile } result
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message $result
    return 1
  }
  set im [image create photo imname -file $tmpfile]
  set cs $KHALVIEW(cs)
  set rs $KHALVIEW(rs)

  for {set j 0} {$j < $cs} {incr j} {
    for {set i 0} {$i < $rs} {incr i} {
      set data [lindex [$im get $i $j] 0]
      set x1 [expr $KHALVIEW(margin) + $i * $KHALVIEW(pointgap)]
      set y1 [expr $KHALVIEW(margin) + $j * $KHALVIEW(pointgap)]
      set x2 [expr $KHALVIEW(margin) + $i * $KHALVIEW(pointgap) + $KHALVIEW(pointsize)]
      set y2 [expr $KHALVIEW(margin) + $j * $KHALVIEW(pointgap) + $KHALVIEW(pointsize)]
      if {$data == 0} {
	set col $KHALVIEW(bgcol)
	set tag "KHALVIEW_bg"
      }
      if {$data == 255} {
	set col $KHALVIEW(fgcol)
	set tag "KHALVIEW_fg"
      }
      if {($data != 0) && $data != 255} {
	set col $KHALVIEW(mkcol)
	set tag "KHALVIEW_mk"
      }
      if {([expr $i % 2] == 0) && ([expr $j % 2] == 0)} {
	$KHALVIEW(c) create oval $x1 $y1 $x2 $y2 -fill $col -tag {KHALVIEW_point $tag}
      } else {
	if {([expr $i % 2] == 1)} {
	  incr x1 -$KHALVIEW(incrsize)
          incr x2 $KHALVIEW(incrsize)
	}
	if {([expr $j % 2] == 1)} {
	  incr y1 -$KHALVIEW(incrsize)
	  incr y2 $KHALVIEW(incrsize)
	}
	$KHALVIEW(c) create rectangle $x1 $y1 $x2 $y2 -fill $col -tag {KHALVIEW_point $tag}
      }
    }
  }
  catch { exec rm "-f" $tmpfile } result

# action associated to quit widget
proc KHALVIEW_quit {} {
  exit
}
bind . "q" KHALVIEW_quit
