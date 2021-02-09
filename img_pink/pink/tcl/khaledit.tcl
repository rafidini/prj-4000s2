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
exec wish $0 $1 $2 $3 $4

# Khalimsky grid editor
# Michel Couprie - juillet 2005

if {($argc != 1) && ($argc != 3)} { 
  puts stderr "usage: khaledit <width height filename | filename>"
  exit 
}

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

# declare globals
global KHALEDIT

# set globals
set KHALEDIT(name) "khaledit"
set KHALEDIT(pointsize) 15
set KHALEDIT(incrsize) 3
set KHALEDIT(pointgap)  30
set KHALEDIT(margin) 10
set KHALEDIT(revert_mode) 0
# current pixel position
set KHALEDIT(x) 0
set KHALEDIT(y) 0
# argument values
if {$argc == 3} { 
  set KHALEDIT(rs) [lindex $argv 0]
  set KHALEDIT(cs) [lindex $argv 1]
  set KHALEDIT(filename) [lindex $argv 2]
} else {
  set KHALEDIT(filename) [lindex $argv 0]
  # read pgm image header
  set input [open $KHALEDIT(filename)]
  set tag [gets $input]
  set line [gets $input]
  while {[string index $line 0] == "#"} {
    set line [gets $input]
  }
  scan $line "%d %d" KHALEDIT(rs) KHALEDIT(cs)
  close $input
}

# canvas size
set KHALEDIT(ca_rs) [expr ($KHALEDIT(rs)-1) * $KHALEDIT(pointgap) + \
                        $KHALEDIT(pointsize) + $KHALEDIT(margin) * 2]
set KHALEDIT(ca_cs) [expr ($KHALEDIT(cs)-1) * $KHALEDIT(pointgap) + \
                        $KHALEDIT(pointsize) + $KHALEDIT(margin) * 2]
# colors
set KHALEDIT(fgcol) red
set KHALEDIT(bgcol) blue
set KHALEDIT(mkcol) green

# set window title
wm title . "Khaledit"

# ============================================================
# MENU DEFINITION
# ============================================================

menu .menubar
. config -menu .menubar
foreach m {File Actions} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$File      add command -label "Quit"             -command KHALEDIT_quit

$Actions   add command -label "View Princ"       -command KHALEDIT_princ
$Actions   add command -label "View Ess"         -command KHALEDIT_ess
$Actions   add command -label "View Regul"       -command KHALEDIT_regul
$Actions   add command -label "View Critic"      -command KHALEDIT_critic
$Actions   add command -label "View M-Critic"    -command KHALEDIT_mcritic
$Actions   add command -label "View End"         -command KHALEDIT_end

# ============================================================
# BUTTONS
# ============================================================

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit command button
button .top.quit -text "Save/Quit" -command KHALEDIT_quit
pack .top.quit -side right

# create the abort command button
button .top.abort -text Abort -command KHALEDIT_abort
pack .top.abort -side right

# create the revert command button
checkbutton .top.revert -text Revert -variable KHALEDIT(revert_mode)
pack .top.revert -side left

# create a frame for the image
frame .imframe -borderwidth 10 -width $KHALEDIT(ca_rs) -height $KHALEDIT(ca_cs)
pack .imframe -side top

# create canvas with the right sizes
set KHALEDIT(c) [canvas .imframe.c -width [expr $KHALEDIT(ca_rs) - 2] -height [expr $KHALEDIT(ca_cs) - 2]]
pack .imframe.c -fill both -expand true

# ============================================================
# MAIN
# ============================================================

if {$argc == 3} { 
  # init the grid
  for {set j 0} {$j < $KHALEDIT(cs)} {incr j 1} {
    for {set i 0} {$i < $KHALEDIT(rs)} {incr i 1} {
      set x1 [expr $KHALEDIT(margin) + $i * $KHALEDIT(pointgap)]
      set y1 [expr $KHALEDIT(margin) + $j * $KHALEDIT(pointgap)]
      set x2 [expr $KHALEDIT(margin) + $i * $KHALEDIT(pointgap) + $KHALEDIT(pointsize)]
      set y2 [expr $KHALEDIT(margin) + $j * $KHALEDIT(pointgap) + $KHALEDIT(pointsize)]
      if {([expr $i % 2] == 0) && ([expr $j % 2] == 0)} {
	$KHALEDIT(c) create oval $x1 $y1 $x2 $y2 -fill $KHALEDIT(bgcol) -tag {KHALEDIT_point KHALEDIT_bg}
      } else {
	if {([expr $i % 2] == 1)} {
	  incr x1 -$KHALEDIT(incrsize)
          incr x2 $KHALEDIT(incrsize)
	}
	if {([expr $j % 2] == 1)} {
	  incr y1 -$KHALEDIT(incrsize)
	  incr y2 $KHALEDIT(incrsize)
	}
	$KHALEDIT(c) create rectangle $x1 $y1 $x2 $y2 -fill $KHALEDIT(bgcol) -tag {KHALEDIT_point KHALEDIT_bg}
      }
    }
  }
} else {
  # read data
  set tmpfile "/tmp/myreadpgmdata_tmp"
  catch { exec pgmrawmode $KHALEDIT(filename) $tmpfile } result
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message $result
    return 1
  }
  set im [image create photo imname -file $tmpfile]
  set cs $KHALEDIT(cs)
  set rs $KHALEDIT(rs)

  for {set j 0} {$j < $cs} {incr j} {
    for {set i 0} {$i < $rs} {incr i} {
      set data [lindex [$im get $i $j] 0]
      set x1 [expr $KHALEDIT(margin) + $i * $KHALEDIT(pointgap)]
      set y1 [expr $KHALEDIT(margin) + $j * $KHALEDIT(pointgap)]
      set x2 [expr $KHALEDIT(margin) + $i * $KHALEDIT(pointgap) + $KHALEDIT(pointsize)]
      set y2 [expr $KHALEDIT(margin) + $j * $KHALEDIT(pointgap) + $KHALEDIT(pointsize)]

      if {$data == 0} {
	if {([expr $i % 2] == 0) && ([expr $j % 2] == 0)} {
	  $KHALEDIT(c) create oval $x1 $y1 $x2 $y2 -fill $KHALEDIT(bgcol) -tag {KHALEDIT_point KHALEDIT_bg}
	} else {
	  if {([expr $i % 2] == 1)} {
	    incr x1 -$KHALEDIT(incrsize)
            incr x2 $KHALEDIT(incrsize)
	  }
	  if {([expr $j % 2] == 1)} {
	    incr y1 -$KHALEDIT(incrsize)
	    incr y2 $KHALEDIT(incrsize)
	  }
	  $KHALEDIT(c) create rectangle $x1 $y1 $x2 $y2 -fill $KHALEDIT(bgcol) -tag {KHALEDIT_point KHALEDIT_bg}
        }
      } else {
	if {([expr $i % 2] == 0) && ([expr $j % 2] == 0)} {
	  $KHALEDIT(c) create oval $x1 $y1 $x2 $y2 -fill $KHALEDIT(fgcol) -tag {KHALEDIT_point KHALEDIT_fg}
	} else {
	  if {([expr $i % 2] == 1)} {
	    incr x1 -$KHALEDIT(incrsize)
            incr x2 $KHALEDIT(incrsize)
	  }
	  if {([expr $j % 2] == 1)} {
	    incr y1 -$KHALEDIT(incrsize)
	    incr y2 $KHALEDIT(incrsize)
	  }
	  $KHALEDIT(c) create rectangle $x1 $y1 $x2 $y2 -fill $KHALEDIT(fgcol) -tag {KHALEDIT_point KHALEDIT_fg}
        }
      }
    }
  }
  catch { exec rm "-f" $tmpfile } result
}

# bind actions to click events
$KHALEDIT(c) bind KHALEDIT_point <Button> {
  set cur [$KHALEDIT(c) find withtag current]
  set taglist [$KHALEDIT(c) gettags $cur]
  if {[lsearch $taglist KHALEDIT_bg] >= 0} {
    $KHALEDIT(c) itemconfigure $cur -fill $KHALEDIT(fgcol)
    $KHALEDIT(c) dtag $cur KHALEDIT_bg
    $KHALEDIT(c) addtag KHALEDIT_fg withtag current
  } else { 
    if {[lsearch $taglist KHALEDIT_fg] >= 0} {
      $KHALEDIT(c) itemconfigure $cur -fill $KHALEDIT(bgcol)
      $KHALEDIT(c) dtag $cur KHALEDIT_fg
      $KHALEDIT(c) addtag KHALEDIT_bg withtag current
    }
  }
}

# ============================================================
# PROCEDURES
# ============================================================

proc KHALEDIT_save {} {
  global KHALEDIT
  set listout ""
  set list [$KHALEDIT(c) find withtag KHALEDIT_fg]
  set count 0
  foreach i $list {
    set coords [$KHALEDIT(c) coords $i]
    set x [lindex $coords 0]
    set y [lindex $coords 1]
    set x [expr ($x - $KHALEDIT(margin)) / $KHALEDIT(pointgap)]
    set y [expr ($y - $KHALEDIT(margin)) / $KHALEDIT(pointgap)]
    lappend listout [format "%3.0f,%3.0f" $x $y]
    set count [expr $count + 1]
  }
  set fileid [open $KHALEDIT(filename) w 0644]

  puts $fileid "P2"
  puts $fileid "$KHALEDIT(rs) $KHALEDIT(cs)"
  puts $fileid "255"

  if {$KHALEDIT(revert_mode) == 0} {   
    for {set j 0} {$j<$KHALEDIT(cs)} {incr j} {
      for {set i 0} {$i<$KHALEDIT(rs)} {incr i} {
        if {[lsearch $listout [format "%3.0f,%3.0f" $i $j]] != -1} {
  	  puts -nonewline $fileid "255 "
          } else { 
            puts -nonewline $fileid "000 " 
          }
        }
        puts $fileid ""
      }
    } else {
      for {set j 0} {$j<$KHALEDIT(cs)} {incr j} {
        for {set i 0} {$i<$KHALEDIT(rs)} {incr i} {
  	  if {[lsearch $listout [format "%3.0f,%3.0f" $i $j]] != -1} {
  	    puts -nonewline $fileid "000 "
            } else { 
              puts -nonewline $fileid "255 " 
            }
        }
        puts $fileid ""
      }
    }
  close $fileid  
}

# action associated to quit widget
proc KHALEDIT_quit {} {
  KHALEDIT_save
  exit
}

# action associated to abort widget
proc KHALEDIT_abort {} {
  exit
}

# action associated to action menu "Princ"
proc KHALEDIT_princ {} {
  global KHALEDIT

  KHALEDIT_save  
  my_exec mark2d $KHALEDIT(filename) 1 "/tmp/_mark2d"
  my_exec sub $KHALEDIT(filename) "/tmp/_mark2d" "/tmp/_princ"
  my_exec_q khalview.tcl "/tmp/_princ" &
}

# action associated to action menu "Ess"
proc KHALEDIT_ess {} {
  global KHALEDIT

  KHALEDIT_save  
  my_exec mark2d $KHALEDIT(filename) 2 "/tmp/_mark2d"
  my_exec sub $KHALEDIT(filename) "/tmp/_mark2d" "/tmp/_ess"
  my_exec_q khalview.tcl "/tmp/_ess" &
}

# action associated to action menu "Regul"
proc KHALEDIT_regul {} {
  global KHALEDIT

  KHALEDIT_save  
  my_exec mark2d $KHALEDIT(filename) 3 "/tmp/_mark2d"
  my_exec sub $KHALEDIT(filename) "/tmp/_mark2d" "/tmp/_regul"
  my_exec_q khalview.tcl "/tmp/_regul" &
}

# action associated to action menu "Critic"
proc KHALEDIT_critic {} {
  global KHALEDIT

  KHALEDIT_save  
  my_exec mark2d $KHALEDIT(filename) 4 "/tmp/_mark2d"
  my_exec sub $KHALEDIT(filename) "/tmp/_mark2d" "/tmp/_critic"
  my_exec_q khalview.tcl "/tmp/_critic" &
}

# action associated to action menu "M-critic"
proc KHALEDIT_mcritic {} {
  global KHALEDIT

  KHALEDIT_save  
  my_exec mark2d $KHALEDIT(filename) 6 "/tmp/_mark2d"
  my_exec sub $KHALEDIT(filename) "/tmp/_mark2d" "/tmp/_mcritic"
  my_exec_q khalview.tcl "/tmp/_mcritic" &
}

# action associated to action menu "End"
proc KHALEDIT_end {} {
  global KHALEDIT

  KHALEDIT_save  
  my_exec mark2d $KHALEDIT(filename) 5 "/tmp/_mark2d"
  my_exec sub $KHALEDIT(filename) "/tmp/_mark2d" "/tmp/_end"
  my_exec_q khalview.tcl "/tmp/_end" &
}
bind . "q" KHALEDIT_quit
