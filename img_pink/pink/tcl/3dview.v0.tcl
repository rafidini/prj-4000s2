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
exec wish $0 $1

# set window title
wm title . "3D viewer"

# external command call
#-----------------------------------
proc my_exec {command args} {
  set call "catch \{ exec $command $args \} result"
  eval $call
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message $result
  }
}

# declare globals
global VIEW3D
# infilename  : input file name
# x  : pixel position in row
# y  : pixel position in column
# z  : plane position
# v  : pixel value
# imxy : image structure for xy view
# imzy : image structure for zy view
# imxz : image structure for xz view
# cxy : canvas structure for xy view
# czy : canvas structure for zy view
# cxz : canvas structure for xz view
# rs : row size
# cs : col size
# ds : depth
# name : command name
# xline, yline, zline : booleans 
set VIEW3D(name) "3dview"
set VIEW3D(xline) 1 
set VIEW3D(yline) 1 
set VIEW3D(zline) 1

if {$argc != 1} { 
  puts stderr "usage: $VIEW3D(name) in"
  exit 
}

# get input image file name as first argument
set VIEW3D(infilename) [lindex $argv 0]

# create the toplevel frame
frame .top -borderwidth 10
pack .top -side top -fill x

# create the frame to be displayed on the top right
frame .top.rightcol
pack .top.rightcol -side right

# create the label for pixel value
label .top.rightcol.pixval
label .top.rightcol.pixval.text -text "pixval ="
label .top.rightcol.pixval.vlab -textvariable VIEW3D(v) -width 5
pack .top.rightcol.pixval.text -side left
pack .top.rightcol.pixval.vlab -side left
pack .top.rightcol.pixval -side top

# create the quit button
button .top.rightcol.quit -text Quit -command selcomp_quit
pack .top.rightcol.quit -side top

# create the three images and load contents from files 
my_exec extractplane $VIEW3D(infilename) 0 xy "/tmp/$VIEW3D(name)_xy" 
set VIEW3D(imxy) [image create photo imxyname -file "/tmp/$VIEW3D(name)_xy"]
my_exec extractplane $VIEW3D(infilename) 0 xz "/tmp/$VIEW3D(name)_xz" 
set VIEW3D(imxz) [image create photo imxzname -file "/tmp/$VIEW3D(name)_xz"]
my_exec extractplane $VIEW3D(infilename) 0 zy "/tmp/$VIEW3D(name)_zy" 
set VIEW3D(imzy) [image create photo imzyname -file "/tmp/$VIEW3D(name)_zy"]

# get image size
set VIEW3D(cs) [image height $VIEW3D(imxy)]
set VIEW3D(rs) [image width $VIEW3D(imxy)]
set VIEW3D(ds) [image width $VIEW3D(imzy)]
set VIEW3D(x) [expr $VIEW3D(rs)/2]
set VIEW3D(y) [expr $VIEW3D(cs)/2]
set VIEW3D(z) [expr $VIEW3D(ds)/2]
set VIEW3D(v) [lindex [$VIEW3D(imxy) get $VIEW3D(x) $VIEW3D(y)] 0]

# create the x, y and z command lines
frame .top.xyz
frame .top.xyz.xbutton
frame .top.xyz.ybutton
frame .top.xyz.zbutton
label .top.xyz.xbutton.label -text "X"
label .top.xyz.ybutton.label -text "Y"
label .top.xyz.zbutton.label -text "Z"
scale .top.xyz.xbutton.scale -from 0 -to [expr $VIEW3D(rs)-1] -length 200 -variable VIEW3D(x) \
  -orient horizontal -tickinterval 0 -showvalue true -command view3d_xchange
scale .top.xyz.ybutton.scale -from 0 -to [expr $VIEW3D(cs)-1] -length 200 -variable VIEW3D(y) \
  -orient horizontal -tickinterval 0 -showvalue true -command view3d_ychange
scale .top.xyz.zbutton.scale -from 0 -to [expr $VIEW3D(ds)-1] -length 200 -variable VIEW3D(z) \
  -orient horizontal -tickinterval 0 -showvalue true -command view3d_zchange
checkbutton .top.xyz.xbutton.check -variable VIEW3D(xline) -command view3d_xlinechange
checkbutton .top.xyz.ybutton.check -variable VIEW3D(yline) -command view3d_ylinechange
checkbutton .top.xyz.zbutton.check -variable VIEW3D(zline) -command view3d_zlinechange
.top.xyz.xbutton.check configure -selectcolor red
.top.xyz.ybutton.check configure -selectcolor green
.top.xyz.zbutton.check configure -selectcolor blue
pack .top.xyz.xbutton.check .top.xyz.xbutton.label .top.xyz.xbutton.scale -side left
pack .top.xyz.ybutton.check .top.xyz.ybutton.label .top.xyz.ybutton.scale -side left
pack .top.xyz.zbutton.check .top.xyz.zbutton.label .top.xyz.zbutton.scale -side left
pack .top.xyz.xbutton .top.xyz.ybutton .top.xyz.zbutton -side top
pack .top.xyz -side left

# create frames for the images
frame .one
frame .two
frame .one.imxyframe -borderwidth 10 -width $VIEW3D(rs) -height $VIEW3D(cs)
frame .one.imzyframe -borderwidth 10 -width $VIEW3D(ds) -height $VIEW3D(cs)
frame .two.imxzframe -borderwidth 10 -width $VIEW3D(rs) -height $VIEW3D(ds)
frame .two.filling   -borderwidth 10 -width $VIEW3D(ds) -height $VIEW3D(ds)
pack .one.imxyframe .one.imzyframe -side left
pack .two.imxzframe .two.filling -side left
pack .one .two -side top

# create canvas with the right sizes
set VIEW3D(cxy) [canvas .one.imxyframe.cxy -width $VIEW3D(rs) -height $VIEW3D(cs)]
set VIEW3D(czy) [canvas .one.imzyframe.czy -width $VIEW3D(ds) -height $VIEW3D(cs)]
set VIEW3D(cxz) [canvas .two.imxzframe.cxz -width $VIEW3D(rs) -height $VIEW3D(ds)]
set czz [canvas .two.filling.czz -width [expr $VIEW3D(ds) - 2] -height [expr $VIEW3D(ds) - 2]]
pack .one.imxyframe.cxy -fill both -expand true
pack .one.imzyframe.czy -fill both -expand true
pack .two.imxzframe.cxz -fill both -expand true
pack .two.filling.czz -fill both -expand true

# put the images into the canvas
$VIEW3D(cxy) create image 1 1 -anchor nw -image $VIEW3D(imxy)
$VIEW3D(cxz) create image 1 1 -anchor nw -image $VIEW3D(imxz)
$VIEW3D(czy) create image 1 1 -anchor nw -image $VIEW3D(imzy)

# create the optional lines for x, y and z position
$VIEW3D(cxy) create line 0 0 0 [expr $VIEW3D(cs) - 1] -fill red -tag xline
$VIEW3D(cxz) create line 0 0 0 [expr $VIEW3D(ds) - 1] -fill red -tag xline
$VIEW3D(cxy) create line 0 0 [expr $VIEW3D(rs) - 1] 0 -fill green -tag yline
$VIEW3D(czy) create line 0 0 [expr $VIEW3D(ds) - 1] 0 -fill green -tag yline
$VIEW3D(cxz) create line 0 0 [expr $VIEW3D(rs) - 1] 0 -fill blue -tag zline
$VIEW3D(czy) create line 0 0 0 [expr $VIEW3D(cs) - 1] -fill blue -tag zline

# action associated to quit widget
proc selcomp_quit {} {
  exit
}

# actions associated to x, y and z change
proc view3d_xchange {x} {
  global VIEW3D
  my_exec extractplane $VIEW3D(infilename) $x zy "/tmp/$VIEW3D(name)_zy" 
  $VIEW3D(imzy) read "/tmp/$VIEW3D(name)_zy"
  if {$VIEW3D(xline)} {
    set xline [$VIEW3D(cxy) find withtag xline]
    $VIEW3D(cxy) coords $xline [expr $x+1] 0 [expr $x+1] [expr $VIEW3D(cs)-1]
    set xline [$VIEW3D(cxz) find withtag xline]
    $VIEW3D(cxz) coords $xline [expr $x+1] 0 [expr $x+1] [expr $VIEW3D(ds)-1]
  }
  set VIEW3D(v) [lindex [$VIEW3D(imxy) get $VIEW3D(x) $VIEW3D(y)] 0]
}
proc view3d_ychange {y} {
  global VIEW3D
  my_exec extractplane $VIEW3D(infilename) $y xz "/tmp/$VIEW3D(name)_xz" 
  $VIEW3D(imxz) read "/tmp/$VIEW3D(name)_xz"
  if {$VIEW3D(yline)} {
    set yline [$VIEW3D(cxy) find withtag yline]
    $VIEW3D(cxy) coords $yline 0 [expr $y+1] [expr $VIEW3D(rs)-1] [expr $y+1]
    set yline [$VIEW3D(czy) find withtag yline]
    $VIEW3D(czy) coords $yline 0 [expr $y+1] [expr $VIEW3D(ds)-1] [expr $y+1]
  }
  set VIEW3D(v) [lindex [$VIEW3D(imxy) get $VIEW3D(x) $VIEW3D(y)] 0]
}
proc view3d_zchange {z} {
  global VIEW3D
  my_exec extractplane $VIEW3D(infilename) $z xy "/tmp/$VIEW3D(name)_xy" 
  $VIEW3D(imxy) read "/tmp/$VIEW3D(name)_xy"
  if {$VIEW3D(zline)} {
    set zline [$VIEW3D(cxz) find withtag zline]
    $VIEW3D(cxz) coords $zline 0 [expr $z+1] [expr $VIEW3D(rs)-1] [expr $z+1]
    set zline [$VIEW3D(czy) find withtag zline]
    $VIEW3D(czy) coords $zline [expr $z+1] 0 [expr $z+1] [expr $VIEW3D(cs)-1]
  }
  set VIEW3D(v) [lindex [$VIEW3D(imxy) get $VIEW3D(x) $VIEW3D(y)] 0]
}

# line toggle
proc view3d_xlinechange {} {
  global VIEW3D
  if {$VIEW3D(xline)} {
    set xline [$VIEW3D(cxy) find withtag xline]
    $VIEW3D(cxy) coords $xline [expr $VIEW3D(x)+1] 0 [expr $VIEW3D(x)+1] [expr $VIEW3D(cs)-1]
    set xline [$VIEW3D(cxz) find withtag xline]
    $VIEW3D(cxz) coords $xline [expr $VIEW3D(x)+1] 0 [expr $VIEW3D(x)+1] [expr $VIEW3D(ds)-1]
  } else {
    set xline [$VIEW3D(cxy) find withtag xline]
    $VIEW3D(cxy) coords $xline 0 0 0 [expr $VIEW3D(cs)-1]
    set xline [$VIEW3D(cxz) find withtag xline]
    $VIEW3D(cxz) coords $xline 0 0 0 [expr $VIEW3D(ds)-1]
  }
}
proc view3d_ylinechange {} {
  global VIEW3D
  if {$VIEW3D(yline)} {
    set yline [$VIEW3D(cxy) find withtag yline]
    $VIEW3D(cxy) coords $yline 0 [expr $VIEW3D(y)+1] [expr $VIEW3D(rs)-1] [expr $VIEW3D(y)+1]
    set yline [$VIEW3D(czy) find withtag yline]
    $VIEW3D(czy) coords $yline 0 [expr $VIEW3D(y)+1] [expr $VIEW3D(ds)-1] [expr $VIEW3D(y)+1]
  } else {
    set yline [$VIEW3D(cxy) find withtag yline]
    $VIEW3D(cxy) coords $yline 0 0 [expr $VIEW3D(rs)-1] 0
    set yline [$VIEW3D(czy) find withtag yline]
    $VIEW3D(czy) coords $yline 0 0 [expr $VIEW3D(ds)-1] 0
  }
}
proc view3d_zlinechange {} {
  global VIEW3D
  if {$VIEW3D(zline)} {
    set zline [$VIEW3D(cxz) find withtag zline]
    $VIEW3D(cxz) coords $zline 0 [expr $VIEW3D(z)+1] [expr $VIEW3D(rs)-1] [expr $VIEW3D(z)+1]
    set zline [$VIEW3D(czy) find withtag zline]
    $VIEW3D(czy) coords $zline [expr $VIEW3D(z)+1] 0 [expr $VIEW3D(z)+1] [expr $VIEW3D(cs)-1]
  } else {
    set zline [$VIEW3D(cxz) find withtag zline]
    $VIEW3D(cxz) coords $zline 0 0 [expr $VIEW3D(rs)-1] 0
    set zline [$VIEW3D(czy) find withtag zline]
    $VIEW3D(czy) coords $zline 0 0 0 [expr $VIEW3D(cs)-1]
  }
}
