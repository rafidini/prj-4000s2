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
# writes a point list in a "b" list-type file
#-----------------------------------
proc my_write_b_list {filename pointlist} {
  set output [open $filename w]
  set n [expr [llength $pointlist] / 2]
  puts $output "b $n"
  puts $output $pointlist
  close $output
}

# writes a curve in a text file in the form of list of segments
#-----------------------------------
proc my_write_curve {filename pointlist} {
  set output [open $filename w]
  set n [expr [llength $pointlist] / 2]
  puts $output "l $n"
  for {set k 0} {$k < [expr $n - 1]} {incr k} {
    puts $output "[lindex $pointlist [expr $k*2]] [lindex $pointlist [expr ($k*2)+1]] [lindex $pointlist [expr ($k+1)*2]] [lindex $pointlist [expr (($k+1)*2)+1]]"
  }
  close $output
}

# writes a number list in a "gnuplot" graph file
#-----------------------------------
proc my_write_gp_list {filename list} {
  set output [open $filename w]
  set n [llength $list]
  for {set k 0} {$k < $n} {incr k} {
    puts $output "$k [lindex $list $k]"
  }
  close $output
}

# writes a number list in a pgm 1d image file
#-----------------------------------
proc my_write_pgm1d_list {filename list} {
  set output [open $filename w]
  puts $output "PA"
  set n [llength $list]
  puts $output "$n 1"
  puts $output "255"
  for {set k 0} {$k < $n} {incr k} {
    puts $output "[lindex $list $k]"
  }
  close $output
}

# writes a text in a text file
#-----------------------------------
proc my_write_text {filename text} {
  set output [open $filename w]
  puts $output $text
  close $output
}
