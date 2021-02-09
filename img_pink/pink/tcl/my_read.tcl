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
# reads a pgm image header
#-----------------------------------
proc my_readpgmheader {filename} {
  set input [open $filename]
  set tag [gets $input]
  set line [gets $input]
  while {([string index $line 0] == "#") || ([string length $line] == 0)} {
    set line [gets $input]
  }
  set n [scan $line "%d%d%d" rs cs ds]
  close $input
  if {$n == 1} { return [list $tag $rs] }
  if {$n == 2} { return [list $tag $rs $cs] }
  if {$n == 3} { return [list $tag $rs $cs $ds] }
  tk_messageBox -type ok -icon error -message "incorrect header"
}

# reads a pgm 2d image data
#-----------------------------------
proc my_readpgmdata {filename} {
  set tmpfile "/tmp/myreadpgmdata_tmp"
  catch { exec pgmrawmode $filename $tmpfile } result
  if {$result != ""} {
    tk_messageBox -type ok -icon error -message $result
    return 1
  }
  set im [image create photo imname -file $tmpfile]
  set cs [image height $im]
  set rs [image width $im]
  set data [list]
  for {set j 0} {$j < $cs} {incr j} {
    for {set i 0} {$i < $rs} {incr i} {
      set data [concat $data [lindex [$im get $i $j] 0]]
    }
  }
  catch { exec rm "-f" $tmpfile } result
  return $data
}

# reads a pgm 3d image header
#-----------------------------------
proc my_readpgmheader3 {filename} {
  set input [open $filename]
  set tag [gets $input]
  set line [gets $input]
  while {[string index $line 0] == "#"} {
    set line [gets $input]
  }
  scan $line "%d %d %d" rs cs ds
  close $input
  return [list $tag $rs $cs $ds]
}

# reads a pgm 3d image data
#-----------------------------------
proc my_readpgmdata3 {filename} {
  set h [my_readpgmheader3 $filename]
  set ds [lindex $h 2]
  set data [list]
  set tmpfile "/tmp/myreadpgmdata3_tmp"
  for {set k 0} {$k < $ds} {incr k} {
    catch { exec extractplane $filename $k xy $tmpfile } result
    if {$result != ""} {
      tk_messageBox -type ok -icon error -message $result
      return 1
    }
    catch { exec pgmrawmode $tmpfile } result
    if {$result != ""} {
      tk_messageBox -type ok -icon error -message $result
      return 1
    }
    set d [my_readpgmdata $tmpfile]
    set data [concat $data $d]
  }
  catch { exec rm "-f" $tmpfile } result
  return $data  
}

# reads a text in a text file
#-----------------------------------
proc my_read_text {filename} {
  set input [open $filename]
  set text [read $input]
  close $input
  return $text
}

# reads a 2D points list
#-----------------------------------
proc my_readlistdata {filename} {
  set input [open $filename]
  set line [gets $input]
  scan $line "%c %d" tag n
  set X [list]
  set Y [list]

  for {set i 0} {$i < $n} {incr i} {
    set line [gets $input]
    scan $line "%g %g" x y
    set X [concat $X $x]
    set Y [concat $Y $y]
  }
  close $input
  return [list $X $Y]
}

# reads a 3D points list
#-----------------------------------
proc my_readlistdata3 {filename} {
  set input [open $filename]
  set line [gets $input]
  scan $line "%c %d" tag n
  set X [list]
  set Y [list]
  set Z [list]

  for {set i 0} {$i < $n} {incr i} {
    set line [gets $input]
    scan $line "%g %g %g" x y z
    set X [concat $X $x]
    set Y [concat $Y $y]
    set Z [concat $Z $z]
  }
  close $input
  return [list $X $Y $Z]
}

# reads a 3D points list
#-----------------------------------
proc my_readlistdata_B {filename} {
  set input [open $filename]
  set line [gets $input]
  scan $line "%s %d" tag n
  if {$tag != "B"} {
    puts "bad tag: $tag"
    return 
  }
  set X [list]
  set Y [list]
  set Z [list]

  for {set i 0} {$i < $n} {incr i} {
    set line [gets $input]
    scan $line "%g %g %g" x y z
    set X [concat $X $x]
    set Y [concat $Y $y]
    set Z [concat $Z $z]
  }
  close $input
  return [list $X $Y $Z]
}


# reads a 1D points list
#-----------------------------------
proc my_readlistdata_e {filename} {
  set input [open $filename]
  set line [gets $input]
  scan $line "%s %d" tag n
  if {$tag != "e"} {
    puts "bad tag: $tag"
    return 
  }
  set X [list]

  for {set i 0} {$i < $n} {incr i} {
    set line [gets $input]
    scan $line "%g" x
    set X [concat $X $x]
  }
  close $input
  return $X
}
