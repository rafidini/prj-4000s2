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

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_read.tcl"]

# declare globals
global ESEDIT

set ESEDIT(name) "esedit3d"
if {($argc != 2) && ($argc != 4)} { 
  puts stderr "usage: $ESEDIT(name) <rs cs ds|infilename> outfilename"
  exit 
}

# set globals
set ESEDIT(origin_mode) 0
set ESEDIT(origin_set) 0
# current pixel position
set ESEDIT(x) 0
set ESEDIT(y) 0
# get argument values
if {$argc == 4} { 
  set ESEDIT(rs) [lindex $argv 0]
  set ESEDIT(cs) [lindex $argv 1]
  set ESEDIT(ds) [lindex $argv 2]
  set ESEDIT(outfilename) [lindex $argv 3]
} else {
  set ESEDIT(infilename) [lindex $argv 0]
  if {![file exists $ESEDIT(infilename)]} {
    puts "ERROR: file not found: $ESEDIT(infilename)"
  }
  set ESEDIT(outfilename) [lindex $argv 1]
  set h [my_readpgmheader3 $ESEDIT(infilename)]
  set ESEDIT(rs) [lindex $h 1]
  set ESEDIT(cs) [lindex $h 2]
  set ESEDIT(ds) [lindex $h 3]
}
set ESEDIT(ps) [expr $ESEDIT(rs) * $ESEDIT(cs)]

# grid parameters
set ESEDIT(pointsize) 10
set ESEDIT(planegap)  15
set ESEDIT(pointgap)  [expr $ESEDIT(planegap) * $ESEDIT(ds) + $ESEDIT(pointsize)]
set ESEDIT(margin) $ESEDIT(pointsize)
# canvas size
set ESEDIT(ca_rs) [expr ($ESEDIT(rs)-1) * $ESEDIT(pointgap) + \
                        ($ESEDIT(ds)-1) * $ESEDIT(planegap) + \
                        $ESEDIT(pointsize) + $ESEDIT(margin) * 2]
set ESEDIT(ca_cs) [expr ($ESEDIT(cs)-1) * $ESEDIT(pointgap) + \
                        ($ESEDIT(ds)-1) * $ESEDIT(planegap) + \
                        $ESEDIT(pointsize) + $ESEDIT(margin) * 2]
# colors
set ESEDIT(fgcol) red
set ESEDIT(bgcol) blue
set ESEDIT(orcol) green
# file format (pgm, khoros_ascii)
set ESEDIT(fileformat) pgm

# set window title
wm title . "ESEdit"

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit command button
button .top.quit -text Quit -command ESEDIT_quit
pack .top.quit -side right

# create the abort command button
button .top.abort -text Abort -command ESEDIT_abort
pack .top.abort -side right

# create the origin command button
checkbutton .top.origin -text Origin -variable ESEDIT(origin_mode)
pack .top.origin -side left

# create a frame for the image
frame .imframe -borderwidth 10 -width $ESEDIT(ca_rs) -height $ESEDIT(ca_cs)
pack .imframe -side top

# create canvas with the right sizes
set ESEDIT(c) [canvas .imframe.c -width [expr $ESEDIT(ca_rs) - 2] -height [expr $ESEDIT(ca_cs) - 2]]
pack .imframe.c -fill both -expand true

# init the grid
set m $ESEDIT(margin)
set g $ESEDIT(pointgap)
set d $ESEDIT(planegap)
set s $ESEDIT(pointsize)
# horizontal lines
for {set k 0} {$k < $ESEDIT(ds)} {incr k} {
  for {set j 0} {$j < $ESEDIT(cs)} {incr j} {
    set x1 [expr $m + $s/2 + $k * $d]
    set x2 [expr $x1 + ($ESEDIT(rs)-1) * $g]
    set y1 [expr $m + $s/2 + $j * $g + $k * $d]
    set y2 $y1
    $ESEDIT(c) create line $x1 $y1 $x2 $y2
  }
}
# vertical lines
for {set k 0} {$k < $ESEDIT(ds)} {incr k} {
  for {set i 0} {$i < $ESEDIT(rs)} {incr i} {
    set y1 [expr $m + $s/2 + $k * $d]
    set y2 [expr $y1 + ($ESEDIT(cs)-1) * $g]
    set x1 [expr $m + $s/2 + $i * $g + $k * $d]
    set x2 $x1
    $ESEDIT(c) create line $x1 $y1 $x2 $y2
  }
}
# diagonal lines
for {set j 0} {$j < $ESEDIT(cs)} {incr j} {
  for {set i 0} {$i < $ESEDIT(rs)} {incr i} {
    set x1 [expr $m + $s/2 + $i * $g]
    set x2 [expr $x1 + ($ESEDIT(ds)-1) * $d]
    set y1 [expr $m + $s/2 + $j * $g]
    set y2 [expr $y1 + ($ESEDIT(ds)-1) * $d]
    $ESEDIT(c) create line $x1 $y1 $x2 $y2
  }
}
# points
if {$argc == 4} { 
  for {set k 0} {$k < $ESEDIT(ds)} {incr k} {
    for {set j 0} {$j < $ESEDIT(cs)} {incr j} {
      for {set i 0} {$i < $ESEDIT(rs)} {incr i} {
        set x1 [expr $m + $i * $g + $k * $d]
        set y1 [expr $m + $j * $g + $k * $d]
        set x2 [expr $m + $i * $g + $k * $d + $s]
        set y2 [expr $m + $j * $g + $k * $d + $s]
        $ESEDIT(c) create oval $x1 $y1 $x2 $y2 \
                          -fill $ESEDIT(bgcol) \
                          -tag {ESEDIT_point ESEDIT_bg}
      }
    }
  }
} else {
  set imagedata [my_readpgmdata3 $ESEDIT(infilename)]
  for {set k 0} {$k < $ESEDIT(ds)} {incr k} {
    for {set j 0} {$j < $ESEDIT(cs)} {incr j} {
      for {set i 0} {$i < $ESEDIT(rs)} {incr i} {
        set x1 [expr $m + $i * $g + $k * $d]
        set y1 [expr $m + $j * $g + $k * $d]
        set x2 [expr $m + $i * $g + $k * $d + $s]
        set y2 [expr $m + $j * $g + $k * $d + $s]
        puts [lindex $imagedata [expr $k*$ESEDIT(ps)+$j*$ESEDIT(cs)+$i]]
	if {[lindex $imagedata [expr $k*$ESEDIT(ps)+$j*$ESEDIT(cs)+$i]] != 0} {
          $ESEDIT(c) create oval $x1 $y1 $x2 $y2 \
                            -fill $ESEDIT(fgcol) \
                            -tag {ESEDIT_point ESEDIT_fg}
	} else {
          $ESEDIT(c) create oval $x1 $y1 $x2 $y2 \
                            -fill $ESEDIT(bgcol) \
                            -tag {ESEDIT_point ESEDIT_bg}
	}
      }
    }
  }

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
  } else {
    if {[lsearch $taglist ESEDIT_or] >= 0} {
      $ESEDIT(c) itemconfigure $cur -width 1.0 -outline black
      $ESEDIT(c) dtag $cur ESEDIT_or
      set ESEDIT(origin_set) 0
    } else {
      if {$ESEDIT(origin_set) == 0} {
        $ESEDIT(c) itemconfigure $cur -width 2.0 -outline $ESEDIT(orcol)
        $ESEDIT(c) addtag ESEDIT_or withtag current
        set ESEDIT(origin_set) 1
      }
    }
  }
}

# action associated to quit widget
proc ESEDIT_quit {} {
  global ESEDIT
  set m $ESEDIT(margin)
  set g $ESEDIT(pointgap)
  set d $ESEDIT(planegap)
  set list [$ESEDIT(c) find withtag ESEDIT_or]
  set count 0
  foreach i $list {
    set coords [$ESEDIT(c) coords $i]
    #puts "coord($i) = $coords" 
    set xo [lindex $coords 0]
    set yo [lindex $coords 1]
    set zo [expr (int($xo - $m) % $g) / $d]
    set xo [expr int(($xo - $m) / $g)]
    set yo [expr int(($yo - $m) / $g)]
    set count [expr $count + 1]
  }
  if {$count != 1} { 
    tk_messageBox -icon error -type ok -message "missing origin" 
    return
  }  

  set list [$ESEDIT(c) find withtag ESEDIT_fg]
  #puts "$list"
  set listout ""
  foreach i $list {
    set coords [$ESEDIT(c) coords $i]
    #puts "coord($i) = $coords" 
    set x [lindex $coords 0]
    set y [lindex $coords 1]
    set z [expr (int($x - $m) % $g) / $d]
    set x [expr int($x - $m) / $g]
    set y [expr int($y - $m) / $g]
    lappend listout [format "%3.0f,%3.0f,%3.0f" $x $y $z]
  }

  set fileid [open $ESEDIT(outfilename) w 0644]


  if {$ESEDIT(fileformat) == "pgm"} {
    puts $fileid "P2"
    puts $fileid "#origin $xo $yo $zo"
    puts $fileid "$ESEDIT(rs) $ESEDIT(cs) $ESEDIT(ds)"
    puts $fileid "255"

    for {set k 0} {$k < $ESEDIT(ds)} {incr k} {
      for {set j 0} {$j < $ESEDIT(cs)} {incr j} {
        for {set i 0} {$i < $ESEDIT(rs)} {incr i} {
	  if {[lsearch $listout [format "%3.0f,%3.0f,%3.0f" $i $j $k]] != -1} {
	    puts -nonewline $fileid "255 "
          } else { 
            puts -nonewline $fileid "000 " 
          }
        }
        puts $fileid ""
      }
      puts $fileid ""
    }
  }

  if {$ESEDIT(fileformat) == "khoros_ascii"} {
    puts $fileid "#width $ESEDIT(rs)"
    puts $fileid "#height $ESEDIT(cs)"
    puts $fileid "#depth $ESEDIT(ds)"
    puts $fileid "#"
    puts $fileid "#x_hot $xo"
    puts $fileid "#y_hot $yo"
    puts $fileid "#z_hot $zo"

    for {set k 0} {$k < $ESEDIT(ds)} {incr k} {
      for {set j 0} {$j < $ESEDIT(cs)} {incr j} {
        for {set i 0} {$i < $ESEDIT(rs)} {incr i} {
	  if {[lsearch $listout [format "%3.0f,%3.0f,%3.0f" $i $j $k]] != -1} {
	    puts -nonewline $fileid "1 "
          } else { 
            puts -nonewline $fileid "0 " 
          }
        }
        puts $fileid ""
      }
      puts $fileid ""
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
