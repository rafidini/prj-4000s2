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

# Structuring element editor
# Michel Couprie - juin 2002

if {($argc != 1) && ($argc != 3)} { 
  puts stderr "usage: esedit <width height filename | filename>"
  exit 
}

# declare globals
global ESEDIT

set PINK "$env(PINK)"

# set globals
set ESEDIT(pointsize) 15
set ESEDIT(pointgap)  30
set ESEDIT(margin) 10
set ESEDIT(origin_mode) 0
set ESEDIT(origin_set) 0
set ESEDIT(xorigin) -1
set ESEDIT(yorigin) -1
set ESEDIT(revert_mode) 0
# current pixel position
set ESEDIT(x) 0
set ESEDIT(y) 0
# argument values
if {$argc == 3} { 
  set ESEDIT(rs) [lindex $argv 0]
  set ESEDIT(cs) [lindex $argv 1]
  set ESEDIT(filename) [lindex $argv 2]
} else {
  set ESEDIT(filename) [lindex $argv 0]
  # read pgm image header
  set input [open $ESEDIT(filename)]
  set tag [gets $input]
  set line [gets $input]
  while {[string index $line 0] == "#"} {
    set index [string first "origin" $line]
    if {$index != "-1"} {
      scan [string range $line [expr $index + 7] end] "%d %d" ESEDIT(xorigin) ESEDIT(yorigin)
      set ESEDIT(origin_set) 1
    }
    set line [gets $input]
  }
  scan $line "%d %d" ESEDIT(rs) ESEDIT(cs)
  close $input
puts "$ESEDIT(xorigin) $ESEDIT(yorigin)"
}

# canvas size
set ESEDIT(ca_rs) [expr ($ESEDIT(rs)-1) * $ESEDIT(pointgap) + \
                        $ESEDIT(pointsize) + $ESEDIT(margin) * 2]
set ESEDIT(ca_cs) [expr ($ESEDIT(cs)-1) * $ESEDIT(pointgap) + \
                        $ESEDIT(pointsize) + $ESEDIT(margin) * 2]
# colors
set ESEDIT(fgcol) red
set ESEDIT(bgcol) blue
set ESEDIT(orcol) green

# file format (pgm, khoros_ascii)
#set ESEDIT(fileformat) khoros_ascii
set ESEDIT(fileformat) pgm

# set window title
wm title . "ESEdit"

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit command button
button .top.quit -text "Save/Quit" -command ESEDIT_quit
pack .top.quit -side right

# create the abort command button
button .top.abort -text Abort -command ESEDIT_abort
pack .top.abort -side right

# create the origin command button
checkbutton .top.origin -text Origin -variable ESEDIT(origin_mode)
pack .top.origin -side left

# create the revert command button
checkbutton .top.revert -text Revert -variable ESEDIT(revert_mode)
pack .top.revert -side left

# create a frame for the image
frame .imframe -borderwidth 10 -width $ESEDIT(ca_rs) -height $ESEDIT(ca_cs)
pack .imframe -side top

# create canvas with the right sizes
set ESEDIT(c) [canvas .imframe.c -width [expr $ESEDIT(ca_rs) - 2] -height [expr $ESEDIT(ca_cs) - 2]]
pack .imframe.c -fill both -expand true

if {$argc == 3} { 
  # init the grid
  for {set j 0} {$j < $ESEDIT(cs)} {incr j 1} {
    for {set i 0} {$i < $ESEDIT(rs)} {incr i 1} {
      set x1 [expr $ESEDIT(margin) + $i * $ESEDIT(pointgap)]
      set y1 [expr $ESEDIT(margin) + $j * $ESEDIT(pointgap)]
      set x2 [expr $ESEDIT(margin) + $i * $ESEDIT(pointgap) + $ESEDIT(pointsize)]
      set y2 [expr $ESEDIT(margin) + $j * $ESEDIT(pointgap) + $ESEDIT(pointsize)]
      $ESEDIT(c) create oval $x1 $y1 $x2 $y2 -fill $ESEDIT(bgcol) -tag {ESEDIT_point ESEDIT_bg}
    }
  }
} else {
  # read data
  set tmpfile "/tmp/myreadpgmdata_tmp"
  catch { exec $PINK/linux/bin/pgmrawmode $ESEDIT(filename) $tmpfile } result
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message $result
    return 1
  }
  set im [image create photo imname -file $tmpfile]
  set cs $ESEDIT(cs)
  set rs $ESEDIT(rs)

  for {set j 0} {$j < $cs} {incr j} {
    for {set i 0} {$i < $rs} {incr i} {
      set data [lindex [$im get $i $j] 0]
      set x1 [expr $ESEDIT(margin) + $i * $ESEDIT(pointgap)]
      set y1 [expr $ESEDIT(margin) + $j * $ESEDIT(pointgap)]
      set x2 [expr $ESEDIT(margin) + $i * $ESEDIT(pointgap) + $ESEDIT(pointsize)]
      set y2 [expr $ESEDIT(margin) + $j * $ESEDIT(pointgap) + $ESEDIT(pointsize)]
      if {$data == 0} {
        $ESEDIT(c) create oval $x1 $y1 $x2 $y2 -fill $ESEDIT(bgcol) -tag {ESEDIT_point ESEDIT_bg}
      } else {
        $ESEDIT(c) create oval $x1 $y1 $x2 $y2 -fill $ESEDIT(fgcol) -tag {ESEDIT_point ESEDIT_fg}
      }
      if {($ESEDIT(origin_set) != 0) && ($ESEDIT(xorigin) == $i) && ($ESEDIT(yorigin) == $j)} {
	# init origin
	set cur [expr ($j * $rs) + $i + 1]
    	$ESEDIT(c) itemconfigure $cur -width 3.0 -outline $ESEDIT(orcol)
        $ESEDIT(c) addtag ESEDIT_or closest $x1 $y1
      }
    }
  }
  catch { exec rm "-f" $tmpfile } result
}

# bind actions to click events
$ESEDIT(c) bind ESEDIT_point <Button> {
  set cur [$ESEDIT(c) find withtag current]
  set taglist [$ESEDIT(c) gettags $cur]
  if {$ESEDIT(origin_mode) == 0} {   
    if {[lsearch $taglist ESEDIT_bg] >= 0} {
      $ESEDIT(c) itemconfigure $cur -fill $ESEDIT(fgcol)
      $ESEDIT(c) dtag $cur ESEDIT_bg
      $ESEDIT(c) addtag ESEDIT_fg withtag current
    } else { 
      if {[lsearch $taglist ESEDIT_fg] >= 0} {
        $ESEDIT(c) itemconfigure $cur -fill $ESEDIT(bgcol)
        $ESEDIT(c) dtag $cur ESEDIT_fg
        $ESEDIT(c) addtag ESEDIT_bg withtag current
      } else {
        if {[lsearch $taglist ESEDIT_or] >= 0} {
          $ESEDIT(c) itemconfigure $cur -fill $ESEDIT(bgcol)
          $ESEDIT(c) dtag $cur ESEDIT_fg
          $ESEDIT(c) addtag ESEDIT_bg withtag current
          set ESEDIT(origin_set) 0
        }
      }
    }
  } else { # ORIGIN MODE
    if {[lsearch $taglist ESEDIT_or] >= 0} {
      $ESEDIT(c) itemconfigure $cur -width 1.0 -outline black
      $ESEDIT(c) dtag $cur ESEDIT_or
      set ESEDIT(origin_set) 0
    } else {
      if {$ESEDIT(origin_set) == 0} {
        $ESEDIT(c) itemconfigure $cur -width 3.0 -outline $ESEDIT(orcol)
        $ESEDIT(c) addtag ESEDIT_or withtag current
        set ESEDIT(origin_set) 1
      }
    }
  }
}

# action associated to quit widget
proc ESEDIT_quit {} {
  global ESEDIT
  set list [$ESEDIT(c) find withtag ESEDIT_or]
  set count 0
  set listout ""
  foreach i $list {
    set coords [$ESEDIT(c) coords $i]
    #puts "coord($i) = $coords" 
    set xo [lindex $coords 0]
    set yo [lindex $coords 1]
    set xo [expr int(($xo - $ESEDIT(margin)) / $ESEDIT(pointgap))]
    set yo [expr int(($yo - $ESEDIT(margin)) / $ESEDIT(pointgap))]
    set count [expr $count + 1]
  }
  if {$count != 1} { 
    tk_messageBox -icon error -type ok -message "missing origin" 
    return
  }

  set list [$ESEDIT(c) find withtag ESEDIT_fg]
  #puts "$list"
  set count 0
  foreach i $list {
    set coords [$ESEDIT(c) coords $i]
    #puts "coord($i) = $coords" 
    set x [lindex $coords 0]
    set y [lindex $coords 1]
    set x [expr ($x - $ESEDIT(margin)) / $ESEDIT(pointgap)]
    set y [expr ($y - $ESEDIT(margin)) / $ESEDIT(pointgap)]
    lappend listout [format "%3.0f,%3.0f" $x $y]
    set count [expr $count + 1]
  }
  #puts "$count | $listout |"

  set fileid [open $ESEDIT(filename) w 0644]

  if {$ESEDIT(fileformat) == "pgm"} {
    puts $fileid "P2"
    puts $fileid "#origin $xo $yo"
    puts $fileid "$ESEDIT(rs) $ESEDIT(cs)"
    puts $fileid "255"

    if {$ESEDIT(revert_mode) == 0} {   
      for {set j 0} {$j<$ESEDIT(cs)} {incr j} {
        for {set i 0} {$i<$ESEDIT(rs)} {incr i} {
  	  if {[lsearch $listout [format "%3.0f,%3.0f" $i $j]] != -1} {
  	    puts -nonewline $fileid "255 "
            } else { 
              puts -nonewline $fileid "000 " 
            }
        }
        puts $fileid ""
      }
    } else {
      for {set j 0} {$j<$ESEDIT(cs)} {incr j} {
        for {set i 0} {$i<$ESEDIT(rs)} {incr i} {
  	  if {[lsearch $listout [format "%3.0f,%3.0f" $i $j]] != -1} {
  	    puts -nonewline $fileid "000 "
            } else { 
              puts -nonewline $fileid "255 " 
            }
        }
        puts $fileid ""
      }
    }
  }

  if {$ESEDIT(fileformat) == "khoros_ascii"} {
    puts $fileid "#width $ESEDIT(rs)"
    puts $fileid "#height $ESEDIT(cs)"
    puts $fileid "#depth 1"
    puts $fileid "#"
    puts $fileid "#x_hot $xo"
    puts $fileid "#y_hot $yo"
    puts $fileid "#z_hot 0"

    if {$ESEDIT(revert_mode) == 0} {   
      for {set j 0} {$j<$ESEDIT(cs)} {incr j} {
        for {set i 0} {$i<$ESEDIT(rs)} {incr i} {
  	  if {[lsearch $listout [format "%3.0f,%3.0f" $i $j]] != -1} {
	      puts -nonewline $fileid "1 "
            } else { 
              puts -nonewline $fileid "0 " 
            }
        }
        puts $fileid ""
      }
    } else {
      for {set j 0} {$j<$ESEDIT(cs)} {incr j} {
        for {set i 0} {$i<$ESEDIT(rs)} {incr i} {
  	  if {[lsearch $listout [format "%3.0f,%3.0f" $i $j]] != -1} {
	      puts -nonewline $fileid "0 "
            } else { 
              puts -nonewline $fileid "1 " 
            }
        }
        puts $fileid ""
      }
    }
  }

  close $fileid  
  
  exit
}

# action associated to abort widget
proc ESEDIT_abort {} {
  exit
}

bind . "q" ESEDIT_quit
