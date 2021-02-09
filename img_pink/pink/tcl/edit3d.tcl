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

# 3D image editor
# Michel Couprie - fevrier 2003

set PINK "$env(PINK)"
source [file join "$PINK" "tcl" "my_read.tcl"]

# declare globals
global EDIT

set EDIT(name) "edit3d"
if {($argc != 2) && ($argc != 4)} { 
  puts stderr "usage: $EDIT(name) <rs cs ds|infilename> outfilename"
  exit 
}

# set globals
# current pixel position
set EDIT(x) 0
set EDIT(y) 0
# get argument values
if {$argc == 4} { 
  set EDIT(rs) [lindex $argv 0]
  set EDIT(cs) [lindex $argv 1]
  set EDIT(ds) [lindex $argv 2]
  set EDIT(outfilename) [lindex $argv 3]
} else {
  set EDIT(infilename) [lindex $argv 0]
  if {![file exists $EDIT(infilename)]} {
    puts "ERROR: file not found: $EDIT(infilename)"
  }
  set EDIT(outfilename) [lindex $argv 1]
  set h [my_readpgmheader3 $EDIT(infilename)]
  set EDIT(rs) [lindex $h 1]
  set EDIT(cs) [lindex $h 2]
  set EDIT(ds) [lindex $h 3]
}
set EDIT(ps) [expr $EDIT(rs) * $EDIT(cs)]

# grid parameters
set EDIT(pointsize) 10
set EDIT(planegap)  15
set EDIT(pointgap)  [expr $EDIT(planegap) * $EDIT(ds) + $EDIT(pointsize)]
set EDIT(margin) $EDIT(pointsize)
# canvas size
set EDIT(ca_rs) [expr ($EDIT(rs)-1) * $EDIT(pointgap) + \
                        ($EDIT(ds)-1) * $EDIT(planegap) + \
                        $EDIT(pointsize) + $EDIT(margin) * 2]
set EDIT(ca_cs) [expr ($EDIT(cs)-1) * $EDIT(pointgap) + \
                        ($EDIT(ds)-1) * $EDIT(planegap) + \
                        $EDIT(pointsize) + $EDIT(margin) * 2]
# colors
set EDIT(fgcol) red
set EDIT(bgcol) blue
set EDIT(mkcol) green

# set window title
wm title . "Edit"

# create a frame for buttons
frame .top -borderwidth 10
pack .top -side top -fill x

# create the quit command button
button .top.quit -text Quit -command EDIT_quit
pack .top.quit -side right

# create a frame for the image
frame .imframe -borderwidth 10 -width $EDIT(ca_rs) -height $EDIT(ca_cs)
pack .imframe -side top

# create canvas with the right sizes
set EDIT(c) [canvas .imframe.c -width [expr $EDIT(ca_rs) - 2] -height [expr $EDIT(ca_cs) - 2]]
pack .imframe.c -fill both -expand true

# init the grid
set m $EDIT(margin)
set g $EDIT(pointgap)
set d $EDIT(planegap)
set s $EDIT(pointsize)
# horizontal lines
for {set k 0} {$k < $EDIT(ds)} {incr k} {
  for {set j 0} {$j < $EDIT(cs)} {incr j} {
    set x1 [expr $m + $s/2 + $k * $d]
    set x2 [expr $x1 + ($EDIT(rs)-1) * $g]
    set y1 [expr $m + $s/2 + $j * $g + $k * $d]
    set y2 $y1
    $EDIT(c) create line $x1 $y1 $x2 $y2
  }
}
# vertical lines
for {set k 0} {$k < $EDIT(ds)} {incr k} {
  for {set i 0} {$i < $EDIT(rs)} {incr i} {
    set y1 [expr $m + $s/2 + $k * $d]
    set y2 [expr $y1 + ($EDIT(cs)-1) * $g]
    set x1 [expr $m + $s/2 + $i * $g + $k * $d]
    set x2 $x1
    $EDIT(c) create line $x1 $y1 $x2 $y2
  }
}
# diagonal lines
for {set j 0} {$j < $EDIT(cs)} {incr j} {
  for {set i 0} {$i < $EDIT(rs)} {incr i} {
    set x1 [expr $m + $s/2 + $i * $g]
    set x2 [expr $x1 + ($EDIT(ds)-1) * $d]
    set y1 [expr $m + $s/2 + $j * $g]
    set y2 [expr $y1 + ($EDIT(ds)-1) * $d]
    $EDIT(c) create line $x1 $y1 $x2 $y2
  }
}
# points
if {$argc == 4} { 
  for {set k 0} {$k < $EDIT(ds)} {incr k} {
    for {set j 0} {$j < $EDIT(cs)} {incr j} {
      for {set i 0} {$i < $EDIT(rs)} {incr i} {
        set x1 [expr $m + $i * $g + $k * $d]
        set y1 [expr $m + $j * $g + $k * $d]
        set x2 [expr $m + $i * $g + $k * $d + $s]
        set y2 [expr $m + $j * $g + $k * $d + $s]
        $EDIT(c) create oval $x1 $y1 $x2 $y2 \
                          -fill $EDIT(bgcol) \
                          -tag {EDIT_point EDIT_bg}
      }
    }
  }
} else {
  set imagedata [my_readpgmdata3 $EDIT(infilename)]
  for {set k 0} {$k < $EDIT(ds)} {incr k} {
    for {set j 0} {$j < $EDIT(cs)} {incr j} {
      for {set i 0} {$i < $EDIT(rs)} {incr i} {
        set x1 [expr $m + $i * $g + $k * $d]
        set y1 [expr $m + $j * $g + $k * $d]
        set x2 [expr $m + $i * $g + $k * $d + $s]
        set y2 [expr $m + $j * $g + $k * $d + $s]
	if {[lindex $imagedata [expr $k*$EDIT(ps)+$j*$EDIT(cs)+$i]] != 0} {
          $EDIT(c) create oval $x1 $y1 $x2 $y2 \
                            -fill $EDIT(fgcol) \
                            -tag {EDIT_point EDIT_fg}
	} else {
          $EDIT(c) create oval $x1 $y1 $x2 $y2 \
                            -fill $EDIT(bgcol) \
                            -tag {EDIT_point EDIT_bg}
	}
      }
    }
  }

}

# bind actions to click events
$EDIT(c) bind EDIT_point <Button-1> {
  set cur [$EDIT(c) find withtag current]
  set taglist [$EDIT(c) gettags $cur]
  if {[lsearch $taglist EDIT_bg] >= 0} {
    $EDIT(c) itemconfigure $cur -fill $EDIT(fgcol)
    $EDIT(c) dtag $cur EDIT_bg
    $EDIT(c) addtag EDIT_fg withtag current
  } else { 
    if {[lsearch $taglist EDIT_fg] >= 0} {
      $EDIT(c) itemconfigure $cur -fill $EDIT(bgcol)
      $EDIT(c) dtag $cur EDIT_fg
      $EDIT(c) addtag EDIT_bg withtag current
    } else { 
      if {[lsearch $taglist EDIT_mk] >= 0} {
        $EDIT(c) itemconfigure $cur -fill $EDIT(bgcol)
        $EDIT(c) dtag $cur EDIT_mk
        $EDIT(c) addtag EDIT_bg withtag current
      }
    }
  }
}

# bind actions to click events
$EDIT(c) bind EDIT_point <Button-3> {
  set cur [$EDIT(c) find withtag current]
  set taglist [$EDIT(c) gettags $cur]
  if {[lsearch $taglist EDIT_bg] >= 0} {
    $EDIT(c) itemconfigure $cur -fill $EDIT(mkcol)
    $EDIT(c) dtag $cur EDIT_bg
    $EDIT(c) addtag EDIT_mk withtag current
  } else { 
    if {[lsearch $taglist EDIT_fg] >= 0} {
      $EDIT(c) itemconfigure $cur -fill $EDIT(mkcol)
      $EDIT(c) dtag $cur EDIT_fg
      $EDIT(c) addtag EDIT_mk withtag current
    }
  }
}

# action associated to quit widget
proc EDIT_quit {} {
  global EDIT
  set m $EDIT(margin)
  set g $EDIT(pointgap)
  set d $EDIT(planegap)

  set list [$EDIT(c) find withtag EDIT_fg]
  #puts "$list"
  set listout ""
  foreach i $list {
    set coords [$EDIT(c) coords $i]
    #puts "coord($i) = $coords" 
    set x [lindex $coords 0]
    set y [lindex $coords 1]
    set z [expr (int($x - $m) % $g) / $d]
    set x [expr int($x - $m) / $g]
    set y [expr int($y - $m) / $g]
    lappend listout [format "%3.0f,%3.0f,%3.0f" $x $y $z]
  }

  set fileid [open $EDIT(outfilename) w 0644]


  puts $fileid "P4"
  puts $fileid "$EDIT(rs) $EDIT(cs) $EDIT(ds)"
  puts $fileid "255"

  for {set k 0} {$k < $EDIT(ds)} {incr k} {
    for {set j 0} {$j < $EDIT(cs)} {incr j} {
      for {set i 0} {$i < $EDIT(rs)} {incr i} {
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

  close $fileid  
  
  exit
}
