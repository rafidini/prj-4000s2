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
# 3D Segmenter
# Michel couprie - aout 2002

# the current segmentation is stored in the file: /tmp/$SEG3D(name)_t
# the current displayed image is stored in the file: /tmp/$SEG3D(name)_d

#sh
# The next line is executed by /bin/sh, but not Tcl \
exec wish $0 $1 $2

if {$argc != 2} { 
  puts stderr "usage: 3dsegment.tcl gray bin"
  exit 
}

wm title . "3D segmenter"

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"
source "$PINK/tcl/my_dialog.tcl"
source "$PINK/tcl/my_feedback.tcl"

# ============================================================
# GLOBAL VARIABLES
# ============================================================

global SEG3D
# infilename  : input file name
# outfilename : output file name
# x  : pixel position in row
# y  : pixel position in column
# z  : plane position
# v  : pixel value
# tx  : transient pixel position in row
# ty  : transient pixel position in column
# tz  : transient plane position
# tv  : transient pixel value
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
# connex : adjacency relation (6, 18, 26)
# quit : boolean
# saved : boolean
# changed : boolean
# surimp : boolean
# surimplight : boolean
set SEG3D(name) "3dsegment"
append SEG3D(name) [clock seconds]
set SEG3D(xline)       1
set SEG3D(yline)       1
set SEG3D(zline)       1
set SEG3D(connex)      26
set SEG3D(quit_auxwin) 0
set SEG3D(saved)       1
set SEG3D(changed)     0
set SEG3D(surimp)      0
set SEG3D(surimplight) 0
set SEG3D(par_seuil1)  128
set SEG3D(par_seuil2min) 128
set SEG3D(par_seuil2max) 256
set SEG3D(par_param)   0
set SEG3D(infilename) [lindex $argv 0]
set SEG3D(outfilename) [lindex $argv 1]
if ![file exists $SEG3D(infilename)] {
  puts stderr "cannot find input file: $SEG3D(infilename)"
  exit   
}
my_exec_f cp $SEG3D(infilename) "/tmp/$SEG3D(name)_d" 
if [file exists $SEG3D(outfilename)] {
  my_exec cp $SEG3D(outfilename) "/tmp/$SEG3D(name)_t" 
}

# ============================================================
# MENU DEFINITION
# ============================================================

set modifier Alt
menu .menubar -tearoff 0
. config -menu .menubar
foreach m {File Analyze Segment Filter Edit Region Visualize Help} {
  set $m [menu .menubar.m$m]
  .menubar add cascade -label $m -menu .menubar.m$m
}

$File      add command -label "Quit"             -command segment3d_quit -underline 0
$File      add command -label "Save segmentation" -command segment3d_saveseg
$File      add command -label "Load segmentation" -command segment3d_loadseg
$File      add command -label "Save mesh"        -command segment3d_savemesh
$File      add command -label "Save parameters"  -command segment3d_savepars
$File      add command -label "Load parameters"  -command segment3d_loadpars

$Analyze   add command -label "Histogram"        -command segment3d_histogram
$Analyze   add command -label "ROI Histogram"    -command segment3d_roihistogram

$Segment   add command -label "Threshold"        -command segment3d_threshold
$Segment   add command -label "Double Threshold" -command segment3d_threshold2

$Filter    add command -label "Component filter" -command segment3d_compfilter
$Filter    add command -label "Cavity filter"    -command segment3d_cavfilter
$Filter    add command -label "Erosion"          -command \
                         {segment3d_one_par_operator erosion "Ball radius" 1}
$Filter    add command -label "Dilation"          -command \
                         {segment3d_one_par_operator dilation "Ball radius" 1}
$Filter    add command -label "Opening"          -command \
                         {segment3d_one_par_operator opening "Ball radius" 1}
$Filter    add command -label "Closing"          -command \
                         {segment3d_one_par_operator closing "Ball radius" 1}
$Filter    add command -label "Alternate Filter"          -command \
                         {segment3d_one_par_operator asf "Ball radius" 1}
$Edit      add command -label "Revert image"     -command segment3d_revertima
$Edit      add command -label "Revert segmentation" -command segment3d_revertseg
$Edit      add command -label "Cancel segmentation" -command segment3d_cancelseg
$Edit      add command -label "Erase ball"       -command \
                         {segment3d_one_par_operator eraseball "Ball radius" 1}
$Edit      add command -label "Erase xy disc"    -command \
                         {segment3d_one_par_operator erasexydisc "Disc radius" 1}
$Edit      add command -label "Cut by xy plane"  -command segment3d_cutxy
$Edit      add command -label "Cut by yz plane"  -command segment3d_cutyz
$Edit      add command -label "Cut by xz plane"  -command segment3d_cutxz
$Region    add command -label "Select"           -command segment3d_select
$Region    add command -label "Separate"         -command \
                    {segment3d_one_par_operator separate "Component number" 2}
$Region    add command -label "Connect"          -command segment3d_connect
$Visualize add check   -label "Surimp"           -command segment3d_surimp \
                       -variable SEG3D(surimp) 
$Visualize add check   -label "Light"           -command segment3d_surimp \
                       -variable SEG3D(surimplight) 
$Visualize add command -label "Render"           -command segment3d_render

# ============================================================
# MAIN WINDOW DEFINITION
# ============================================================

# create the toplevel frame
frame .top -borderwidth 10
pack .top -side top -fill x

# create the three images and load contents from files 
my_exec_f extractplane "/tmp/$SEG3D(name)_d" 0 xy "/tmp/$SEG3D(name)_xy" 
set SEG3D(imxy) [image create photo imxyname -file "/tmp/$SEG3D(name)_xy"]
my_exec_f extractplane "/tmp/$SEG3D(name)_d" 0 xz "/tmp/$SEG3D(name)_xz" 
set SEG3D(imxz) [image create photo imxzname -file "/tmp/$SEG3D(name)_xz"]
my_exec_f extractplane "/tmp/$SEG3D(name)_d" 0 zy "/tmp/$SEG3D(name)_zy" 
set SEG3D(imzy) [image create photo imzyname -file "/tmp/$SEG3D(name)_zy"]

# get image size
set SEG3D(cs) [image height $SEG3D(imxy)]
set SEG3D(rs) [image width $SEG3D(imxy)]
set SEG3D(ds) [image width $SEG3D(imzy)]
set SEG3D(x) [expr $SEG3D(rs)/2]
set SEG3D(y) [expr $SEG3D(cs)/2]
set SEG3D(z) [expr $SEG3D(ds)/2]
set SEG3D(v) [lindex [$SEG3D(imxy) get $SEG3D(x) $SEG3D(y)] 0]
set SEG3D(tx) [expr $SEG3D(rs)/2]
set SEG3D(ty) [expr $SEG3D(cs)/2]
set SEG3D(tz) [expr $SEG3D(ds)/2]
set SEG3D(tv) [lindex [$SEG3D(imxy) get $SEG3D(x) $SEG3D(y)] 0]

# create frames for the images
frame .top.one
frame .top.two
frame .top.one.imxyframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(cs)
frame .top.one.imzyframe -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(cs)
frame .top.two.imxzframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(ds)
frame .top.two.filling   -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(ds)
pack .top.one.imxyframe .top.one.imzyframe -side left
pack .top.two.imxzframe .top.two.filling -side left
pack .top.one .top.two -side top

# create canvas with the right sizes
set SEG3D(cxy) [canvas .top.one.imxyframe.cxy -width $SEG3D(rs) -height $SEG3D(cs)]
set SEG3D(czy) [canvas .top.one.imzyframe.czy -width $SEG3D(ds) -height $SEG3D(cs)]
set SEG3D(cxz) [canvas .top.two.imxzframe.cxz -width $SEG3D(rs) -height $SEG3D(ds)]
set czz [canvas .top.two.filling.czz -width [expr $SEG3D(ds) - 2] -height [expr $SEG3D(ds) - 2]]
pack .top.one.imxyframe.cxy -fill both -expand true
pack .top.one.imzyframe.czy -fill both -expand true
pack .top.two.imxzframe.cxz -fill both -expand true
pack .top.two.filling.czz -fill both -expand true

# put the images into the canvas
$SEG3D(cxy) create image 1 1 -anchor nw -image $SEG3D(imxy)
$SEG3D(cxz) create image 1 1 -anchor nw -image $SEG3D(imxz)
$SEG3D(czy) create image 1 1 -anchor nw -image $SEG3D(imzy)

# create the optional lines for x, y and z position
$SEG3D(cxy) create line 0 0 0 [expr $SEG3D(cs) - 1] -fill red -tag xline
$SEG3D(cxz) create line 0 0 0 [expr $SEG3D(ds) - 1] -fill red -tag xline
$SEG3D(cxy) create line 0 0 [expr $SEG3D(rs) - 1] 0 -fill green -tag yline
$SEG3D(czy) create line 0 0 [expr $SEG3D(ds) - 1] 0 -fill green -tag yline
$SEG3D(cxz) create line 0 0 [expr $SEG3D(rs) - 1] 0 -fill blue -tag zline
$SEG3D(czy) create line 0 0 0 [expr $SEG3D(cs) - 1] -fill blue -tag zline

# create the x, y and z command lines
frame .top.xyz
frame .top.xyz.xbutton
frame .top.xyz.ybutton
frame .top.xyz.zbutton
label .top.xyz.xbutton.label -text "X"
label .top.xyz.ybutton.label -text "Y"
label .top.xyz.zbutton.label -text "Z"
scale .top.xyz.xbutton.scale -from 0 -to [expr $SEG3D(rs)-1] -length 200 -variable SEG3D(x) \
  -orient horizontal -tickinterval 0 -showvalue true -command segment3d_xchange
scale .top.xyz.ybutton.scale -from 0 -to [expr $SEG3D(cs)-1] -length 200 -variable SEG3D(y) \
  -orient horizontal -tickinterval 0 -showvalue true -command segment3d_ychange
scale .top.xyz.zbutton.scale -from 0 -to [expr $SEG3D(ds)-1] -length 200 -variable SEG3D(z) \
  -orient horizontal -tickinterval 0 -showvalue true -command segment3d_zchange
checkbutton .top.xyz.xbutton.check -variable SEG3D(xline) -command segment3d_xlinechange
checkbutton .top.xyz.ybutton.check -variable SEG3D(yline) -command segment3d_ylinechange
checkbutton .top.xyz.zbutton.check -variable SEG3D(zline) -command segment3d_zlinechange
.top.xyz.xbutton.check configure -selectcolor red
.top.xyz.ybutton.check configure -selectcolor green
.top.xyz.zbutton.check configure -selectcolor blue
pack .top.xyz.xbutton.check .top.xyz.xbutton.label .top.xyz.xbutton.scale -side left
pack .top.xyz.ybutton.check .top.xyz.ybutton.label .top.xyz.ybutton.scale -side left
pack .top.xyz.zbutton.check .top.xyz.zbutton.label .top.xyz.zbutton.scale -side left
pack .top.xyz.xbutton .top.xyz.ybutton .top.xyz.zbutton -side top
pack .top.xyz -side left

# create the frame to be displayed on the top right
frame .top.rightcol
pack .top.rightcol -side right

# create the label for pixel value
label .top.rightcol.pixval
label .top.rightcol.pixval.xlab -textvariable SEG3D(tx) -width 5
label .top.rightcol.pixval.ylab -textvariable SEG3D(ty) -width 5
label .top.rightcol.pixval.zlab -textvariable SEG3D(tz) -width 5
label .top.rightcol.pixval.vlab -textvariable SEG3D(tv) -width 5
label .top.rightcol.pixval.text -text " : "
pack .top.rightcol.pixval.xlab -side left
pack .top.rightcol.pixval.ylab -side left
pack .top.rightcol.pixval.zlab -side left
pack .top.rightcol.pixval.text -side left
pack .top.rightcol.pixval.vlab -side left
pack .top.rightcol.pixval -side top

# bind actions to mouse events
bind $SEG3D(cxy) <Button-1> { 
  set SEG3D(x) %x 
  set SEG3D(y) %y
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
}
bind $SEG3D(czy) <Button-1> { 
  set SEG3D(z) %x 
  set SEG3D(y) %y
  segment3d_zchange $SEG3D(z)
  segment3d_ychange $SEG3D(y)
}
bind $SEG3D(cxz) <Button-1> { 
  set SEG3D(x) %x 
  set SEG3D(z) %y
  segment3d_xchange $SEG3D(x)
  segment3d_zchange $SEG3D(z)
}
bind $SEG3D(cxy) <Motion> { 
  if {(%x < $SEG3D(rs)) && (%y < $SEG3D(cs))} {
    set SEG3D(tx) %x 
    set SEG3D(ty) %y
    set SEG3D(tz) $SEG3D(z)
    set SEG3D(tv) [lindex [$SEG3D(imxy) get %x %y] 0]
  }
}
bind $SEG3D(czy) <Motion> { 
  if {(%x < $SEG3D(ds)) && (%y < $SEG3D(cs))} {
    set SEG3D(tz) %x 
    set SEG3D(ty) %y
    set SEG3D(tx) $SEG3D(x)
    set SEG3D(tv) [lindex [$SEG3D(imzy) get %x %y] 0]
  }
}
bind $SEG3D(cxz) <Motion> { 
  if {(%x < $SEG3D(rs)) && (%y < $SEG3D(ds))} {
    set SEG3D(tx) %x 
    set SEG3D(tz) %y
    set SEG3D(ty) $SEG3D(y)
    set SEG3D(tv) [lindex [$SEG3D(imxz) get %x %y] 0]
  }
}
bind $SEG3D(cxy) <Button-3> { 
  set SEG3D(x) %x 
  set SEG3D(y) %y
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    my_exec genimage $SEG3D(rs) $SEG3D(cs) $SEG3D(ds) 0 "/tmp/$SEG3D(name)_t"
  }
  my_exec point "/tmp/$SEG3D(name)_t" $SEG3D(x) $SEG3D(y) $SEG3D(z) 255 "/tmp/$SEG3D(name)_t"
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  set SEG3D(changed) 1
}
bind $SEG3D(czy) <Button-3> { 
  set SEG3D(z) %x 
  set SEG3D(y) %y
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    my_exec genimage $SEG3D(rs) $SEG3D(cs) $SEG3D(ds) 0 "/tmp/$SEG3D(name)_t"
  }
  my_exec point "/tmp/$SEG3D(name)_t" $SEG3D(x) $SEG3D(y) $SEG3D(z) 255 "/tmp/$SEG3D(name)_t"
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  set SEG3D(changed) 1
}
bind $SEG3D(cxz) <Button-3> { 
  set SEG3D(x) %x 
  set SEG3D(z) %y
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    my_exec genimage $SEG3D(rs) $SEG3D(cs) $SEG3D(ds) 0 "/tmp/$SEG3D(name)_t"
  }
  my_exec point "/tmp/$SEG3D(name)_t" $SEG3D(x) $SEG3D(y) $SEG3D(z) 255 "/tmp/$SEG3D(name)_t"
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  set SEG3D(changed) 1
}

#===========================================
# PROCEDURES
#===========================================

# action associated to quit menu 
#-----------------------------------
proc segment3d_quit {} {
  global SEG3D
  if {$SEG3D(saved) == 0} {
    set choice [tk_messageBox -type yesnocancel -default yes -icon question\
      -message "Save changes ?"]
    if {$choice == "yes"} {
      if [file exists "/tmp/$SEG3D(name)_t"] {
        my_exec mv "/tmp/$SEG3D(name)_t" $SEG3D(outfilename)
      }
    }
    if {$choice == "cancel"} {
      return
    }
  }
  foreach file [glob -nocomplain "/tmp/$SEG3D(name)_*"] {
    exec rm $file
  }
  exit
}
wm protocol . WM_DELETE_WINDOW segment3d_quit
bind . "q" segment3d_quit

# actions associated to save and load menus 
#-----------------------------------
proc segment3d_saveseg {} {
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    tk_messageBox -type ok -icon error -message "Nothing to save"
    return
  }
  my_exec cp "/tmp/$SEG3D(name)_t" $SEG3D(outfilename)
  set SEG3D(saved) 1
}

proc segment3d_loadseg {} {
  global SEG3D
  if {$SEG3D(saved) == 0} {
    set choice [tk_messageBox -type yesnocancel -default yes -icon question\
      -message "Save changes before loading ?"]
    if {$choice == "yes"} {
      if [file exists "/tmp/$SEG3D(name)_t"] {
        my_exec mv "/tmp/$SEG3D(name)_t" $SEG3D(outfilename)
      }
    }
  }
  set filename [Dialog_Prompt "Enter Filename" ""]
  if {$filename == ""} {
    return
  }
  . config -cursor watch
  update 
  toplevel .waitmess -borderwidth 10
  entry .waitmess.entry
  pack .waitmess.entry -side top
  update
  my_feedback "loading..."
  my_exec cp $filename "/tmp/$SEG3D(name)_t"
  set SEG3D(saved) 0
  set SEG3D(changed) 0
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  . config -cursor left_ptr
  destroy .waitmess
}

proc segment3d_savemesh {} {
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_v"] {
    tk_messageBox -type ok -icon error -message "Nothing to save"
    return
  }
  my_exec cp "/tmp/$SEG3D(name)_v" "$SEG3D(outfilename).vtk"
}

proc segment3d_savepars {} {
  global SEG3D
  if [catch {open "$SEG3D(infilename).par" w} fd] {
    tk_messageBox -type ok -icon error -message "Cannot open $SEG3D(infilename).par for writing"
    return
  }
  foreach var {par_seuil1 par_seuil2min par_seuil2max} {
    puts $fd "$var $SEG3D($var)"
  }
  close $fd
}

proc segment3d_loadpars {} {
  global SEG3D
  if [catch {open "$SEG3D(infilename).par" r} fd] {
    tk_messageBox -type ok -icon error -message "cannot open $SEG3D(infilename).par for reading"
    return
  }
  while {[gets $fd line] >= 0} {
    set var [lindex $line 0]
    set val [lindex $line 1]
    set SEG3D($var) $val
  }
  close $fd
}

# actions associated to x, y and z change
#-----------------------------------
proc segment3d_xchange {x} {
  global SEG3D
  my_exec_f extractplane "/tmp/$SEG3D(name)_d" $x zy "/tmp/$SEG3D(name)_zy" 
  if {$SEG3D(surimp)} {
    my_exec_f extractplane "/tmp/$SEG3D(name)_t" $x zy "/tmp/$SEG3D(name)_tzy" 
    my_exec border "/tmp/$SEG3D(name)_tzy" 8 "/tmp/$SEG3D(name)__1"
    if {$SEG3D(surimplight)} {
      my_exec add "/tmp/$SEG3D(name)_zy" "/tmp/$SEG3D(name)__1" "/tmp/$SEG3D(name)__2"
    } else {
      my_exec sub "/tmp/$SEG3D(name)_zy" "/tmp/$SEG3D(name)__1" "/tmp/$SEG3D(name)__2"
    }
    my_exec pgm2ppm "/tmp/$SEG3D(name)_zy" "/tmp/$SEG3D(name)__2" "/tmp/$SEG3D(name)__2" "/tmp/$SEG3D(name)_zy"
  }
  $SEG3D(imzy) read "/tmp/$SEG3D(name)_zy"
  if {$SEG3D(xline)} {
    set xline [$SEG3D(cxy) find withtag xline]
    $SEG3D(cxy) coords $xline [expr $x+1] 0 [expr $x+1] [expr $SEG3D(cs)-1]
    set xline [$SEG3D(cxz) find withtag xline]
    $SEG3D(cxz) coords $xline [expr $x+1] 0 [expr $x+1] [expr $SEG3D(ds)-1]
  }
  set SEG3D(v) [lindex [$SEG3D(imxy) get $SEG3D(x) $SEG3D(y)] 0]
}
proc segment3d_ychange {y} {
  global SEG3D
  my_exec_f extractplane "/tmp/$SEG3D(name)_d" $y xz "/tmp/$SEG3D(name)_xz" 
  if {$SEG3D(surimp)} {
    my_exec_f extractplane "/tmp/$SEG3D(name)_t" $y xz "/tmp/$SEG3D(name)_txz" 
    my_exec border "/tmp/$SEG3D(name)_txz" 8 "/tmp/$SEG3D(name)__1"
    if {$SEG3D(surimplight)} {
      my_exec add "/tmp/$SEG3D(name)_xz" "/tmp/$SEG3D(name)__1" "/tmp/$SEG3D(name)__2"
    } else {
      my_exec sub "/tmp/$SEG3D(name)_xz" "/tmp/$SEG3D(name)__1" "/tmp/$SEG3D(name)__2"
    }
    my_exec pgm2ppm "/tmp/$SEG3D(name)_xz" "/tmp/$SEG3D(name)__2" "/tmp/$SEG3D(name)__2" "/tmp/$SEG3D(name)_xz"
  }
  $SEG3D(imxz) read "/tmp/$SEG3D(name)_xz"
  if {$SEG3D(yline)} {
    set yline [$SEG3D(cxy) find withtag yline]
    $SEG3D(cxy) coords $yline 0 [expr $y+1] [expr $SEG3D(rs)-1] [expr $y+1]
    set yline [$SEG3D(czy) find withtag yline]
    $SEG3D(czy) coords $yline 0 [expr $y+1] [expr $SEG3D(ds)-1] [expr $y+1]
  }
  set SEG3D(v) [lindex [$SEG3D(imxy) get $SEG3D(x) $SEG3D(y)] 0]
}
proc segment3d_zchange {z} {
  global SEG3D
  my_exec_f extractplane "/tmp/$SEG3D(name)_d" $z xy "/tmp/$SEG3D(name)_xy" 
  if {$SEG3D(surimp)} {
    my_exec_f extractplane "/tmp/$SEG3D(name)_t" $z xy "/tmp/$SEG3D(name)_txy" 
    my_exec border "/tmp/$SEG3D(name)_txy" 8 "/tmp/$SEG3D(name)__1"
    if {$SEG3D(surimplight)} {
      my_exec add "/tmp/$SEG3D(name)_xy" "/tmp/$SEG3D(name)__1" "/tmp/$SEG3D(name)__2"
    } else {
      my_exec sub "/tmp/$SEG3D(name)_xy" "/tmp/$SEG3D(name)__1" "/tmp/$SEG3D(name)__2"
    }
    my_exec pgm2ppm "/tmp/$SEG3D(name)_xy" "/tmp/$SEG3D(name)__2" "/tmp/$SEG3D(name)__2" "/tmp/$SEG3D(name)_xy"
  }
  $SEG3D(imxy) read "/tmp/$SEG3D(name)_xy"
  if {$SEG3D(zline)} {
    set zline [$SEG3D(cxz) find withtag zline]
    $SEG3D(cxz) coords $zline 0 [expr $z+1] [expr $SEG3D(rs)-1] [expr $z+1]
    set zline [$SEG3D(czy) find withtag zline]
    $SEG3D(czy) coords $zline [expr $z+1] 0 [expr $z+1] [expr $SEG3D(cs)-1]
  }
  set SEG3D(v) [lindex [$SEG3D(imxy) get $SEG3D(x) $SEG3D(y)] 0]
}

# line toggle
#-----------------------------------
proc segment3d_xlinechange {} {
  global SEG3D
  if {$SEG3D(xline)} {
    set xline [$SEG3D(cxy) find withtag xline]
    $SEG3D(cxy) coords $xline [expr $SEG3D(x)+1] 0 [expr $SEG3D(x)+1] [expr $SEG3D(cs)-1]
    set xline [$SEG3D(cxz) find withtag xline]
    $SEG3D(cxz) coords $xline [expr $SEG3D(x)+1] 0 [expr $SEG3D(x)+1] [expr $SEG3D(ds)-1]
  } else {
    set xline [$SEG3D(cxy) find withtag xline]
    $SEG3D(cxy) coords $xline 0 0 0 [expr $SEG3D(cs)-1]
    set xline [$SEG3D(cxz) find withtag xline]
    $SEG3D(cxz) coords $xline 0 0 0 [expr $SEG3D(ds)-1]
  }
}
proc segment3d_ylinechange {} {
  global SEG3D
  if {$SEG3D(yline)} {
    set yline [$SEG3D(cxy) find withtag yline]
    $SEG3D(cxy) coords $yline 0 [expr $SEG3D(y)+1] [expr $SEG3D(rs)-1] [expr $SEG3D(y)+1]
    set yline [$SEG3D(czy) find withtag yline]
    $SEG3D(czy) coords $yline 0 [expr $SEG3D(y)+1] [expr $SEG3D(ds)-1] [expr $SEG3D(y)+1]
  } else {
    set yline [$SEG3D(cxy) find withtag yline]
    $SEG3D(cxy) coords $yline 0 0 [expr $SEG3D(rs)-1] 0
    set yline [$SEG3D(czy) find withtag yline]
    $SEG3D(czy) coords $yline 0 0 [expr $SEG3D(ds)-1] 0
  }
}
proc segment3d_zlinechange {} {
  global SEG3D
  if {$SEG3D(zline)} {
    set zline [$SEG3D(cxz) find withtag zline]
    $SEG3D(cxz) coords $zline 0 [expr $SEG3D(z)+1] [expr $SEG3D(rs)-1] [expr $SEG3D(z)+1]
    set zline [$SEG3D(czy) find withtag zline]
    $SEG3D(czy) coords $zline [expr $SEG3D(z)+1] 0 [expr $SEG3D(z)+1] [expr $SEG3D(cs)-1]
  } else {
    set zline [$SEG3D(cxz) find withtag zline]
    $SEG3D(cxz) coords $zline 0 0 [expr $SEG3D(rs)-1] 0
    set zline [$SEG3D(czy) find withtag zline]
    $SEG3D(czy) coords $zline 0 0 0 [expr $SEG3D(cs)-1]
  }
}

# thresholding procedure
#-----------------------------------
proc segment3d_threshold {} {
  global SEG3D
  # create toplevel window for the thresholded image
  toplevel .threshold -borderwidth 10
  frame .threshold.one
  frame .threshold.two
  frame .threshold.one.txyframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(cs)
  frame .threshold.one.tzyframe -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(cs)
  frame .threshold.two.txzframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(ds)
  frame .threshold.two.filling  -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(ds)
  pack .threshold.one.txyframe .threshold.one.tzyframe -side left
  pack .threshold.two.txzframe .threshold.two.filling -side left
  pack .threshold.one .threshold.two -side top

  set ctxy [canvas .threshold.one.txyframe.ctxy -width $SEG3D(rs) -height $SEG3D(cs)]
  set ctzy [canvas .threshold.one.tzyframe.ctzy -width $SEG3D(ds) -height $SEG3D(cs)]
  set ctxz [canvas .threshold.two.txzframe.ctxz -width $SEG3D(rs) -height $SEG3D(ds)]
  set ctzz [canvas .threshold.two.filling.ctzz -width $SEG3D(ds) -height $SEG3D(ds)]

  pack .threshold.one.txyframe.ctxy -fill both -expand true
  pack .threshold.one.tzyframe.ctzy -fill both -expand true
  pack .threshold.two.txzframe.ctxz -fill both -expand true
  pack .threshold.two.filling.ctzz -fill both -expand true

  set SEG3D(seuil) $SEG3D(par_seuil1)
  frame .threshold.command
  scale .threshold.command.scale -from 0 -to 256 -length 200 \
    -variable SEG3D(seuil) -orient horizontal -tickinterval 0 -showvalue true \
    -command threshold_change
  button .threshold.command.button1 -text Done -command "set SEG3D(quit_auxwin) 1"
  button .threshold.command.button2 -text Cancel -command "set SEG3D(quit_auxwin) 2"
  pack .threshold.command.scale -side left
  pack .threshold.command.button1 -side right
  pack .threshold.command.button2 -side right
  pack .threshold.command -side top -fill x

  # operate the threshold 
  my_exec seuil "/tmp/$SEG3D(name)_d" $SEG3D(seuil) "/tmp/$SEG3D(name)_t" 

  # extract the 3 views
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 

  set SEG3D(imtxy) [image create photo timxyname -file "/tmp/$SEG3D(name)_txy"]
  set SEG3D(imtzy) [image create photo timzyname -file "/tmp/$SEG3D(name)_tzy"]
  set SEG3D(imtxz) [image create photo timxzname -file "/tmp/$SEG3D(name)_txz"]
  $ctxy create image 1 1 -anchor nw -image $SEG3D(imtxy)
  $ctzy create image 1 1 -anchor nw -image $SEG3D(imtzy)
  $ctxz create image 1 1 -anchor nw -image $SEG3D(imtxz)
  set oldx $SEG3D(x)
  set oldy $SEG3D(y)
  set oldz $SEG3D(z)
  while {$SEG3D(quit_auxwin) == 0} {
    vwait SEG3D
    if {$oldx != $SEG3D(x)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy"
      $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"
      set oldx $SEG3D(x)
    }
    if {$oldy != $SEG3D(y)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz"
      $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"
      set oldy $SEG3D(y)
    }
    if {$oldz != $SEG3D(z)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy"
      $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"
      set oldz $SEG3D(z)
    }
  }
  if {$SEG3D(quit_auxwin) == 1} { set SEG3D(saved) 0 }
  set SEG3D(quit_auxwin) 0
  set SEG3D(par_seuil1) $SEG3D(seuil)
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  destroy .threshold
}

proc threshold_change {t} {
  global SEG3D
  my_exec seuil "/tmp/$SEG3D(name)_d" $t "/tmp/$SEG3D(name)_t" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

# double thresholding window
#-----------------------------------
proc segment3d_threshold2 {} {
  global SEG3D
  # create toplevel window for the thresholded image
  toplevel .threshold2 -borderwidth 10
  frame .threshold2.one
  frame .threshold2.two
  frame .threshold2.one.txyframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(cs)
  frame .threshold2.one.tzyframe -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(cs)
  frame .threshold2.two.txzframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(ds)
  frame .threshold2.two.filling  -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(ds)
  pack .threshold2.one.txyframe .threshold2.one.tzyframe -side left
  pack .threshold2.two.txzframe .threshold2.two.filling -side left
  pack .threshold2.one .threshold2.two -side top

  set ctxy [canvas .threshold2.one.txyframe.ctxy -width $SEG3D(rs) -height $SEG3D(cs)]
  set ctzy [canvas .threshold2.one.tzyframe.ctzy -width $SEG3D(ds) -height $SEG3D(cs)]
  set ctxz [canvas .threshold2.two.txzframe.ctxz -width $SEG3D(rs) -height $SEG3D(ds)]
  set ctzz [canvas .threshold2.two.filling.ctzz -width $SEG3D(ds) -height $SEG3D(ds)]

  pack .threshold2.one.txyframe.ctxy -fill both -expand true
  pack .threshold2.one.tzyframe.ctzy -fill both -expand true
  pack .threshold2.two.txzframe.ctxz -fill both -expand true
  pack .threshold2.two.filling.ctzz -fill both -expand true

  set SEG3D(seuilmin) $SEG3D(par_seuil2min)
  set SEG3D(seuilmax) $SEG3D(par_seuil2max)
  frame .threshold2.command
  frame .threshold2.command.scales
  scale .threshold2.command.scales.scalemin -from 0 -to 256 -length 200 \
    -variable SEG3D(seuilmin) -orient horizontal -tickinterval 0 -showvalue true \
    -command threshold2_change
  scale .threshold2.command.scales.scalemax -from 0 -to 256 -length 200 \
    -variable SEG3D(seuilmax) -orient horizontal -tickinterval 0 -showvalue true \
    -command threshold2_change
  pack .threshold2.command.scales.scalemin .threshold2.command.scales.scalemax \
    -side top
  button .threshold2.command.button1 -text Done -command "set SEG3D(quit_auxwin) 1"
  button .threshold2.command.button2 -text Cancel -command "set SEG3D(quit_auxwin) 2"
  pack .threshold2.command.scales -side left
  pack .threshold2.command.button1 -side right
  pack .threshold2.command.button2 -side right
  pack .threshold2.command -side top -fill x

  # operate the double threshold
  my_exec seuil2 "/tmp/$SEG3D(name)_d" $SEG3D(seuilmin) $SEG3D(seuilmax) 0 0 "/tmp/$SEG3D(name)_t" 
  my_exec seuil "/tmp/$SEG3D(name)_t" 1 "/tmp/$SEG3D(name)_t" 

  # extract the 3 views
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 

  set SEG3D(imtxy) [image create photo timxyname -file "/tmp/$SEG3D(name)_txy"]
  set SEG3D(imtzy) [image create photo timzyname -file "/tmp/$SEG3D(name)_tzy"]
  set SEG3D(imtxz) [image create photo timxzname -file "/tmp/$SEG3D(name)_txz"]
  $ctxy create image 1 1 -anchor nw -image $SEG3D(imtxy)
  $ctzy create image 1 1 -anchor nw -image $SEG3D(imtzy)
  $ctxz create image 1 1 -anchor nw -image $SEG3D(imtxz)
  set oldx $SEG3D(x)
  set oldy $SEG3D(y)
  set oldz $SEG3D(z)
  while {$SEG3D(quit_auxwin) == 0} {
    vwait SEG3D
    if {$oldx != $SEG3D(x)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy"
      $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"
      set oldx $SEG3D(x)
    }
    if {$oldy != $SEG3D(y)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz"
      $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"
      set oldy $SEG3D(y)
    }
    if {$oldz != $SEG3D(z)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy"
      $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"
      set oldz $SEG3D(z)
    }
  }
  if {$SEG3D(quit_auxwin) == 1} { set SEG3D(saved) 0 }
  set SEG3D(quit_auxwin) 0
  set SEG3D(par_seuil2min) $SEG3D(seuilmin)
  set SEG3D(par_seuil2max) $SEG3D(seuilmax)
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  destroy .threshold2
}

proc threshold2_change {t} {
  global SEG3D
  if {$SEG3D(seuilmin) > $SEG3D(seuilmax)} {
    set SEG3D(seuilmin) $SEG3D(seuilmax)
  }
  my_exec seuil "/tmp/$SEG3D(name)_d" $SEG3D(seuilmin) "/tmp/$SEG3D(name)_t" 
  my_exec seuil "/tmp/$SEG3D(name)_d" $SEG3D(seuilmax) "/tmp/$SEG3D(name)_u" 
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_u" "/tmp/$SEG3D(name)_t" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

# histogram
#-----------------------------------
proc segment3d_histogram {} {
  global SEG3D
  my_exec histopgm "/tmp/$SEG3D(name)_d" "/tmp/$SEG3D(name)_h"
  exec xv "/tmp/$SEG3D(name)_h" &
}

# histogram on ROI
#-----------------------------------
proc segment3d_roihistogram {} {
  global SEG3D
  my_exec histopgm "/tmp/$SEG3D(name)_d" "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_h"
  exec xv "/tmp/$SEG3D(name)_h" &
}

# component filter
#-----------------------------------
proc segment3d_compfilter {} {
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    tk_messageBox -type ok -icon error\
      -message "Segmented image needed\nUse a segmentation command first" 
    return
  }
  # create toplevel window
  toplevel .compfilter -borderwidth 10
  frame .compfilter.one
  frame .compfilter.two
  frame .compfilter.one.txyframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(cs)
  frame .compfilter.one.tzyframe -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(cs)
  frame .compfilter.two.txzframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(ds)
  frame .compfilter.two.filling  -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(ds)
  pack .compfilter.one.txyframe .compfilter.one.tzyframe -side left
  pack .compfilter.two.txzframe .compfilter.two.filling -side left
  pack .compfilter.one .compfilter.two -side top

  set ctxy [canvas .compfilter.one.txyframe.ctxy -width $SEG3D(rs) -height $SEG3D(cs)]
  set ctzy [canvas .compfilter.one.tzyframe.ctzy -width $SEG3D(ds) -height $SEG3D(cs)]
  set ctxz [canvas .compfilter.two.txzframe.ctxz -width $SEG3D(rs) -height $SEG3D(ds)]
  set ctzz [canvas .compfilter.two.filling.ctzz -width $SEG3D(ds) -height $SEG3D(ds)]

  pack .compfilter.one.txyframe.ctxy -fill both -expand true
  pack .compfilter.one.tzyframe.ctzy -fill both -expand true
  pack .compfilter.two.txzframe.ctxz -fill both -expand true
  pack .compfilter.two.filling.ctzz -fill both -expand true

  set SEG3D(seuil) 0
  frame .compfilter.command
  frame .compfilter.command.dialog
  set SEG3D(criterion) "minsize"
  menubutton .compfilter.command.dialog.mb -text "Component min size ..." \
    -menu .compfilter.command.dialog.mb.menu -relief raised
  set m [menu .compfilter.command.dialog.mb.menu -tearoff 0]
  $m add command -label "Component min size" -command {
    .compfilter.command.dialog.mb config -text "Component min size ..."
    set SEG3D(criterion) "minsize"
  }
  $m add command -label "Component number" -command {
    .compfilter.command.dialog.mb config -text "Component number ..."
    set SEG3D(criterion) "number"
  }
  entry .compfilter.command.dialog.entry -textvariable SEG3D(seuil) \
    -relief sunken -width 10
  bind  .compfilter.command.dialog.entry <Return> compfilter_change
  button .compfilter.command.dialog.run -text Run -command compfilter_change
  button .compfilter.command.button1 -text Done -command "set SEG3D(quit_auxwin) 1"
  button .compfilter.command.button2 -text Cancel -command "set SEG3D(quit_auxwin) 2"
  pack .compfilter.command.dialog.mb .compfilter.command.dialog.entry \
       .compfilter.command.dialog.run -side left
  pack .compfilter.command.dialog -side left
  pack .compfilter.command.button1 -side right
  pack .compfilter.command.button2 -side right
  pack .compfilter.command -side top -fill x

  # extract the 3 views
  my_exec cp "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 

  set SEG3D(imtxy) [image create photo timxyname -file "/tmp/$SEG3D(name)_txy"]
  set SEG3D(imtzy) [image create photo timzyname -file "/tmp/$SEG3D(name)_tzy"]
  set SEG3D(imtxz) [image create photo timxzname -file "/tmp/$SEG3D(name)_txz"]
  $ctxy create image 1 1 -anchor nw -image $SEG3D(imtxy)
  $ctzy create image 1 1 -anchor nw -image $SEG3D(imtzy)
  $ctxz create image 1 1 -anchor nw -image $SEG3D(imtxz)
  set oldx $SEG3D(x)
  set oldy $SEG3D(y)
  set oldz $SEG3D(z)
  while {$SEG3D(quit_auxwin) == 0} {
    vwait SEG3D
    if {$oldx != $SEG3D(x)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy"
      $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"
      set oldx $SEG3D(x)
    }
    if {$oldy != $SEG3D(y)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz"
      $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"
      set oldy $SEG3D(y)
    }
    if {$oldz != $SEG3D(z)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy"
      $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"
      set oldz $SEG3D(z)
    }
  }
  if {$SEG3D(quit_auxwin) == 1} {
    my_exec mv "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t" 
    set SEG3D(saved) 0
  }
  set SEG3D(quit_auxwin) 0
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  destroy .compfilter
}

proc compfilter_change {} {
  global SEG3D
  if {$SEG3D(criterion) == "minsize"} {
    my_exec areaopening "/tmp/$SEG3D(name)_t" $SEG3D(connex) $SEG3D(seuil) "/tmp/$SEG3D(name)_t1"
  }
  if {$SEG3D(criterion) == "number"} {
    if {$SEG3D(seuil) <= 0} {
      tk_messageBox -type ok -icon error -message "Enter a positive value"
      return
    }
    .compfilter config -cursor watch
    . config -cursor watch
    update 
    toplevel .waitmess -borderwidth 10
    entry .waitmess.entry
    pack .waitmess.entry -side top
    update
    my_feedback "deleting..."
    my_exec areaselnb "/tmp/$SEG3D(name)_t" $SEG3D(connex) $SEG3D(seuil) "/tmp/$SEG3D(name)_t1"
    .compfilter config -cursor left_ptr
    . config -cursor left_ptr
    destroy .waitmess
  }
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

# cavity filter (dual of component filter)
#-----------------------------------
proc segment3d_cavfilter {} {
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    tk_messageBox -type ok -icon error\
      -message "Segmented image needed\nUse a segmentation command first" 
    return
  }
  my_exec inverse "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t" 
  segment3d_compfilter    
  my_exec inverse "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t" 
}

# single parameter operator
#-----------------------------------
proc segment3d_one_par_operator {procname paramname pardefault} {
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    tk_messageBox -type ok -icon error\
      -message "Segmented image needed\nUse a segmentation command first" 
    return
  }
  # create toplevel window
  toplevel .$procname -borderwidth 10
  frame .$procname.one
  frame .$procname.two
  frame .$procname.one.txyframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(cs)
  frame .$procname.one.tzyframe -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(cs)
  frame .$procname.two.txzframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(ds)
  frame .$procname.two.filling  -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(ds)
  pack .$procname.one.txyframe .$procname.one.tzyframe -side left
  pack .$procname.two.txzframe .$procname.two.filling -side left
  pack .$procname.one .$procname.two -side top

  set ctxy [canvas .$procname.one.txyframe.ctxy -width $SEG3D(rs) -height $SEG3D(cs)]
  set ctzy [canvas .$procname.one.tzyframe.ctzy -width $SEG3D(ds) -height $SEG3D(cs)]
  set ctxz [canvas .$procname.two.txzframe.ctxz -width $SEG3D(rs) -height $SEG3D(ds)]
  set ctzz [canvas .$procname.two.filling.ctzz -width $SEG3D(ds) -height $SEG3D(ds)]

  pack .$procname.one.txyframe.ctxy -fill both -expand true
  pack .$procname.one.tzyframe.ctzy -fill both -expand true
  pack .$procname.two.txzframe.ctxz -fill both -expand true
  pack .$procname.two.filling.ctzz -fill both -expand true

  set SEG3D(param) $pardefault
  frame .$procname.command
  frame .$procname.command.dialog
  set SEG3D(criterion) "minsize"
  label .$procname.command.dialog.label -text $paramname
  entry .$procname.command.dialog.entry -textvariable SEG3D(param) \
    -relief sunken -width 10
  bind  .$procname.command.dialog.entry <Return> $procname
  button .$procname.command.dialog.run -text Run -command $procname
  button .$procname.command.button1 -text Done -command "set SEG3D(quit_auxwin) 1"
  button .$procname.command.button2 -text Cancel -command "set SEG3D(quit_auxwin) 2"
  pack .$procname.command.dialog.label .$procname.command.dialog.entry \
       .$procname.command.dialog.run -side left
  pack .$procname.command.dialog -side left
  pack .$procname.command.button1 -side right
  pack .$procname.command.button2 -side right
  pack .$procname.command -side top -fill x

  # extract the 3 views
  my_exec cp "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 

  set SEG3D(imtxy) [image create photo timxyname -file "/tmp/$SEG3D(name)_txy"]
  set SEG3D(imtzy) [image create photo timzyname -file "/tmp/$SEG3D(name)_tzy"]
  set SEG3D(imtxz) [image create photo timxzname -file "/tmp/$SEG3D(name)_txz"]
  $ctxy create image 1 1 -anchor nw -image $SEG3D(imtxy)
  $ctzy create image 1 1 -anchor nw -image $SEG3D(imtzy)
  $ctxz create image 1 1 -anchor nw -image $SEG3D(imtxz)
  set oldx $SEG3D(x)
  set oldy $SEG3D(y)
  set oldz $SEG3D(z)
  while {$SEG3D(quit_auxwin) == 0} {
    vwait SEG3D
    if {$oldx != $SEG3D(x)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy"
      $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"
      set oldx $SEG3D(x)
    }
    if {$oldy != $SEG3D(y)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz"
      $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"
      set oldy $SEG3D(y)
    }
    if {$oldz != $SEG3D(z)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy"
      $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"
      set oldz $SEG3D(z)
    }
  }
  if {$SEG3D(quit_auxwin) == 1} {
    my_exec mv "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t" 
    set SEG3D(saved) 0
  }
  set SEG3D(quit_auxwin) 0
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  destroy .$procname
}

proc erosion {} {
  global SEG3D
  .erosion config -cursor watch
  . config -cursor watch
  update 
  my_exec erosball "/tmp/$SEG3D(name)_t" $SEG3D(param) "/tmp/$SEG3D(name)_t1"
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  .erosion config -cursor left_ptr
  . config -cursor left_ptr
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

proc dilation {} {
  global SEG3D
  .dilation config -cursor watch
  . config -cursor watch
  update 
  my_exec dilatball "/tmp/$SEG3D(name)_t" $SEG3D(param) "/tmp/$SEG3D(name)_t1"
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  .dilation config -cursor left_ptr
  . config -cursor left_ptr
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

proc closing {} {
  global SEG3D
  .closing config -cursor watch
  . config -cursor watch
  update 
  my_exec dilatball "/tmp/$SEG3D(name)_t" $SEG3D(param) "/tmp/$SEG3D(name)_t1"
  my_exec erosball "/tmp/$SEG3D(name)_t1" $SEG3D(param) "/tmp/$SEG3D(name)_t1"
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  .closing config -cursor left_ptr
  . config -cursor left_ptr
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

proc opening {} {
  global SEG3D
  .opening config -cursor watch
  . config -cursor watch
  update 
  my_exec erosball "/tmp/$SEG3D(name)_t" $SEG3D(param) "/tmp/$SEG3D(name)_t1"
  my_exec dilatball "/tmp/$SEG3D(name)_t1" $SEG3D(param) "/tmp/$SEG3D(name)_t1"
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  .opening config -cursor left_ptr
  . config -cursor left_ptr
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

proc asf {} {
  global SEG3D
  .asf config -cursor watch
  . config -cursor watch
  update 
  my_exec asfbin "/tmp/$SEG3D(name)_t" $SEG3D(param) "/tmp/$SEG3D(name)_t1"
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  .asf config -cursor left_ptr
  . config -cursor left_ptr
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

proc eraseball {} {
  global SEG3D
  .eraseball config -cursor watch
  . config -cursor watch
  update 
  my_exec genimage $SEG3D(rs) $SEG3D(cs) $SEG3D(ds) 0 "/tmp/$SEG3D(name)_t1"
  my_exec point "/tmp/$SEG3D(name)_t1" $SEG3D(x) $SEG3D(y) $SEG3D(z) 255 "/tmp/$SEG3D(name)_t1"
  my_exec dilatball "/tmp/$SEG3D(name)_t1" $SEG3D(param) "/tmp/$SEG3D(name)_t1"
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t1"
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  .eraseball config -cursor left_ptr
  . config -cursor left_ptr
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

proc erasexydisc {} {
  global SEG3D
  .erasexydisc config -cursor watch
  . config -cursor watch
  update 
  my_exec genimage $SEG3D(rs) $SEG3D(cs) 1 0 "/tmp/$SEG3D(name)_t1d"
  my_exec point "/tmp/$SEG3D(name)_t1d" $SEG3D(x) $SEG3D(y) 0 255 "/tmp/$SEG3D(name)_t1d"
  my_exec dilatball "/tmp/$SEG3D(name)_t1d" $SEG3D(param) "/tmp/$SEG3D(name)_t1d"
  my_exec genimage $SEG3D(rs) $SEG3D(cs) $SEG3D(ds) 0 "/tmp/$SEG3D(name)_t1"
  my_exec insert "/tmp/$SEG3D(name)_t1d" "/tmp/$SEG3D(name)_t1" 0 0 $SEG3D(z) "/tmp/$SEG3D(name)_t1"
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t1"
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  .erasexydisc config -cursor left_ptr
  . config -cursor left_ptr
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  if {$SEG3D(surimp)} {
    segment3d_xchange $SEG3D(x)
    segment3d_ychange $SEG3D(y)
    segment3d_zchange $SEG3D(z)
  }
}

proc separate {} {
  global SEG3D
  .separate config -cursor watch
  . config -cursor watch
  update 
  toplevel .waitmess -borderwidth 10
  entry .waitmess.entry
  pack .waitmess.entry -side top
  update
  my_feedback "calculating distance..."
  my_exec inverse "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1"
  my_exec dist "/tmp/$SEG3D(name)_t1" 0 "/tmp/$SEG3D(name)_t1"
  my_feedback "finding markers..."
  my_exec long2byte "/tmp/$SEG3D(name)_t1" 2 "/tmp/$SEG3D(name)_t1"
  my_exec heightselnb "/tmp/$SEG3D(name)_t1" $SEG3D(connex) $SEG3D(param) "/tmp/$SEG3D(name)_t2"
  my_feedback "watershed..."
  my_exec inverse "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t1"
  my_exec watershed "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t2" null null $SEG3D(connex) "/tmp/$SEG3D(name)_t2"
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t2" "/tmp/$SEG3D(name)_t1"
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 
  my_exec extractplane "/tmp/$SEG3D(name)_t1" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  destroy .waitmess
  .separate config -cursor left_ptr
  . config -cursor left_ptr
  $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"    
  $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"    
  $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"    
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
}

proc segment3d_cutxy {} {
  global SEG3D
  set z [Dialog_Prompt "Enter z" 0]
  if {$z == ""} {
    return
  }
  . config -cursor watch
  update 
  toplevel .waitmess -borderwidth 10
  entry .waitmess.entry
  pack .waitmess.entry -side top
  update
  my_feedback "cutting..."
  my_exec genplane $SEG3D(rs) $SEG3D(cs) $SEG3D(ds) z $z "/tmp/$SEG3D(name)_t1"
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t"
  set SEG3D(saved) 0
  set SEG3D(changed) 1
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  . config -cursor left_ptr
  destroy .waitmess
}

proc segment3d_cutyz {} {
  global SEG3D
  set x [Dialog_Prompt "Enter x" 0]
  if {$x == ""} {
    return
  }
  . config -cursor watch
  update 
  toplevel .waitmess -borderwidth 10
  entry .waitmess.entry
  pack .waitmess.entry -side top
  update
  my_feedback "cutting..."
  my_exec genplane $SEG3D(rs) $SEG3D(cs) $SEG3D(ds) x $x "/tmp/$SEG3D(name)_t1"
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t"
  set SEG3D(saved) 0
  set SEG3D(changed) 1
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  . config -cursor left_ptr
  destroy .waitmess
}

proc segment3d_cutxz {} {
  global SEG3D
  set y [Dialog_Prompt "Enter y" 0]
  if {$y == ""} {
    return
  }
  . config -cursor watch
  update 
  toplevel .waitmess -borderwidth 10
  entry .waitmess.entry
  pack .waitmess.entry -side top
  update
  my_feedback "cutting..."
  my_exec genplane $SEG3D(rs) $SEG3D(cs) $SEG3D(ds) y $y "/tmp/$SEG3D(name)_t1"
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t"
  set SEG3D(saved) 0
  set SEG3D(changed) 1
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  . config -cursor left_ptr
  destroy .waitmess
}

# revert image
#-----------------------------------
proc segment3d_revertima {} {
  global SEG3D
  my_exec inverse "/tmp/$SEG3D(name)_d" "/tmp/$SEG3D(name)_d"
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
}

# revert segmentation
#-----------------------------------
proc segment3d_revertseg {} {
  global SEG3D
  my_exec inverse "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t"
  set SEG3D(changed) 1
}

# cancel segmentation
#-----------------------------------
proc segment3d_cancelseg {} {
  global SEG3D
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t"
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  set SEG3D(changed) 1
}

# connected component selection or deletion
#-----------------------------------
proc segment3d_select {} {
# images intermediaires:
# _t: segmentation de base (restauree par l'action Reset)
# _t1: sub _t selection courante (image binaire)
# _t2: intermediaire de calcul
# _t3: image affichee (composee avec parties selectionnees en gris)
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    tk_messageBox -type ok -icon error\
      -message "Segmented image needed\nUse a segmentation command first" 
    return
  }
  # create toplevel window
  toplevel .select -borderwidth 10
  frame .select.one
  frame .select.two
  frame .select.one.txyframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(cs)
  frame .select.one.tzyframe -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(cs)
  frame .select.two.txzframe -borderwidth 10 -width $SEG3D(rs) -height $SEG3D(ds)
  frame .select.two.filling  -borderwidth 10 -width $SEG3D(ds) -height $SEG3D(ds)
  pack .select.one.txyframe .select.one.tzyframe -side left
  pack .select.two.txzframe .select.two.filling -side left
  pack .select.one .select.two -side top

  set ctxy [canvas .select.one.txyframe.ctxy -width $SEG3D(rs) -height $SEG3D(cs)]
  set ctzy [canvas .select.one.tzyframe.ctzy -width $SEG3D(ds) -height $SEG3D(cs)]
  set ctxz [canvas .select.two.txzframe.ctxz -width $SEG3D(rs) -height $SEG3D(ds)]
  set ctzz [canvas .select.two.filling.ctzz -width $SEG3D(ds) -height $SEG3D(ds)]

  pack .select.one.txyframe.ctxy -fill both -expand true
  pack .select.one.tzyframe.ctzy -fill both -expand true
  pack .select.two.txzframe.ctxz -fill both -expand true
  pack .select.two.filling.ctzz -fill both -expand true

  frame .select.command
  button .select.command.reset -text Reset -command select_reset
  button .select.command.inverse -text Inverse -command select_inverse
  radiobutton .select.command.connex6 -variable SEG3D(connex) -text "6" -value 6
  radiobutton .select.command.connex18 -variable SEG3D(connex) -text "18" -value 18
  radiobutton .select.command.connex26 -variable SEG3D(connex) -text "26" -value 26
  radiobutton .select.command.connex60 -variable SEG3D(connex) -text "6xy" -value 60
  radiobutton .select.command.connex260 -variable SEG3D(connex) -text "26xy" -value 260
  button .select.command.done -text Done -command "set SEG3D(quit_auxwin) 1"
  button .select.command.cancel -text Cancel -command "set SEG3D(quit_auxwin) 2"
  pack .select.command.reset .select.command.inverse .select.command.connex6\
       .select.command.connex18 .select.command.connex26\
       .select.command.connex60 .select.command.connex260 -side left
  pack .select.command.done -side right
  pack .select.command.cancel -side right
  pack .select.command -side top -fill x

  # extract the 3 views
  my_exec cp "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" 
  my_exec cp "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t3" 
  my_exec extractplane "/tmp/$SEG3D(name)_t3" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t3" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy" 
  my_exec extractplane "/tmp/$SEG3D(name)_t3" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz" 

  set SEG3D(imtxy) [image create photo timxyname -file "/tmp/$SEG3D(name)_txy"]
  set SEG3D(imtzy) [image create photo timzyname -file "/tmp/$SEG3D(name)_tzy"]
  set SEG3D(imtxz) [image create photo timxzname -file "/tmp/$SEG3D(name)_txz"]
  $ctxy create image 1 1 -anchor nw -image $SEG3D(imtxy)
  $ctzy create image 1 1 -anchor nw -image $SEG3D(imtzy)
  $ctxz create image 1 1 -anchor nw -image $SEG3D(imtxz)

  # bind actions to mouse events
  bind $ctxy <Button> { 
    set SEG3D(x) %x 
    set SEG3D(y) %y
    select_update
  }
  bind $ctzy <Button> { 
    set SEG3D(z) %x 
    set SEG3D(y) %y
    select_update
  }
  bind $ctxz <Button> { 
    set SEG3D(x) %x 
    set SEG3D(z) %y
    select_update
  }

  set oldx $SEG3D(x)
  set oldy $SEG3D(y)
  set oldz $SEG3D(z)
  while {$SEG3D(quit_auxwin) == 0} {
    vwait SEG3D
    if {($oldx != $SEG3D(x)) || $SEG3D(changed)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t3" $SEG3D(x) zy "/tmp/$SEG3D(name)_tzy"
      $SEG3D(imtzy) read "/tmp/$SEG3D(name)_tzy"
      set oldx $SEG3D(x)
    }
    if {($oldy != $SEG3D(y)) || $SEG3D(changed)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t3" $SEG3D(y) xz "/tmp/$SEG3D(name)_txz"
      $SEG3D(imtxz) read "/tmp/$SEG3D(name)_txz"
      set oldy $SEG3D(y)
    }
    if {($oldz != $SEG3D(z)) || $SEG3D(changed)} {
      my_exec extractplane "/tmp/$SEG3D(name)_t3" $SEG3D(z) xy "/tmp/$SEG3D(name)_txy"
      $SEG3D(imtxy) read "/tmp/$SEG3D(name)_txy"
      set oldz $SEG3D(z)
    }
    set SEG3D(changed) 0
  }
  if {$SEG3D(quit_auxwin) == 1} {
    my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t" 
    set SEG3D(saved) 0
  }
  set SEG3D(quit_auxwin) 0
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  destroy .select
}

proc select_update {} {
  global SEG3D
  .select config -cursor watch
  . config -cursor watch
  update 
  toplevel .waitmess -borderwidth 10
  entry .waitmess.entry
  pack .waitmess.entry -side top
  update
  my_feedback "selecting..."
  my_exec selectcomp "/tmp/$SEG3D(name)_t1" $SEG3D(connex) $SEG3D(x) $SEG3D(y) $SEG3D(z) "/tmp/$SEG3D(name)_t2" 
  my_feedback "displaying..."
  my_exec sub "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t2" "/tmp/$SEG3D(name)_t1" 
  my_exec scale "/tmp/$SEG3D(name)_t2" 0.5 "/tmp/$SEG3D(name)_t2" 
  my_exec sub "/tmp/$SEG3D(name)_t3" "/tmp/$SEG3D(name)_t2" "/tmp/$SEG3D(name)_t3" 
  my_exec rm -f "/tmp/$SEG3D(name)_t2"
  set SEG3D(changed) 1
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  .select config -cursor left_ptr
  . config -cursor left_ptr
  destroy .waitmess
}

proc select_reset {} {
  global SEG3D
  my_exec cp "/tmp/$SEG3D(name)_t"  "/tmp/$SEG3D(name)_t1" 
  my_exec cp "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t3" 
  set SEG3D(changed) 1
}

proc select_inverse {} {
  global SEG3D
  my_exec scale "/tmp/$SEG3D(name)_t1" 0.5 "/tmp/$SEG3D(name)_t2" 
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t2" "/tmp/$SEG3D(name)_t3" 
  my_exec sub "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_t1" "/tmp/$SEG3D(name)_t1" 
  my_exec rm -f "/tmp/$SEG3D(name)_t2"
  set SEG3D(changed) 1
}

# rendering (needs VTK)
#-----------------------------------
proc segment3d_render {} {
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    tk_messageBox -type ok -icon error -message "Segmented image needed\nUse a segmentation command first"
    return
  }
  . config -cursor watch
  update 
  toplevel .waitmess -borderwidth 10
  entry .waitmess.entry
  pack .waitmess.entry -side top
  update
  my_feedback "marching cubes..."
  my_exec mcube "/tmp/$SEG3D(name)_t" 0 0 0 vtk "/tmp/$SEG3D(name)_v"
  my_feedback "decimating..."
  my_exec decimate.tcl "/tmp/$SEG3D(name)_v" 0.9 "/tmp/$SEG3D(name)_v"
  exec render0.tcl "/tmp/$SEG3D(name)_v" &
  . config -cursor left_ptr
  destroy .waitmess
}

# connect
#-----------------------------------
proc segment3d_connect {} {
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    tk_messageBox -type ok -icon error -message "Segmented image needed\nUse a segmentation command first"
    return
  }
  . config -cursor watch
  update 
  toplevel .waitmess -borderwidth 10
  entry .waitmess.entry
  pack .waitmess.entry -side top
  update
  my_feedback "making bounding box..."
  my_exec boxmin "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_c"
  my_exec dilatball "/tmp/$SEG3D(name)_c" 5 "/tmp/$SEG3D(name)_c"
  my_exec cadre "/tmp/$SEG3D(name)_t" "/tmp/$SEG3D(name)_p"
  my_exec sub "/tmp/$SEG3D(name)_c" "/tmp/$SEG3D(name)_p" "/tmp/$SEG3D(name)_c"
  my_feedback "computing priority function..."
  my_exec byte2long "/tmp/$SEG3D(name)_d" "/tmp/$SEG3D(name)_p"
  my_feedback "shrinking bounding box..."
  my_exec squelubp "/tmp/$SEG3D(name)_c" "/tmp/$SEG3D(name)_p" $SEG3D(connex) "/tmp/$SEG3D(name)_t"  "/tmp/$SEG3D(name)_t" 
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
  . config -cursor left_ptr
  destroy .waitmess
}

# superimposes segmentation contours to the grayscale image
#-----------------------------------
proc segment3d_surimp {} {
  global SEG3D
  if ![file exists "/tmp/$SEG3D(name)_t"] {
    tk_messageBox -type ok -icon error -message "Segmented image needed\nUse a segmentation command first"
    set SEG3D(surimp) 0
    return
  }
  segment3d_xchange $SEG3D(x)
  segment3d_ychange $SEG3D(y)
  segment3d_zchange $SEG3D(z)
}
