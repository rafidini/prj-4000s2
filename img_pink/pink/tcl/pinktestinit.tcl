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
exec tclsh $0 $1

# ============================================================
# ============================================================
# pinktest.tcl
# executes regression tests for Pink
# Michel Couprie - July 2009
# ============================================================
# ============================================================

if {($argc != 0) && ($argc != 1)} { 
    puts stderr "usage: pinktest \[operator\]"
  exit 
}

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

set PINKTEST(name)     "pinktest"
set PINKTEST(testtag)  "%TEST"
set PINKTEST(testdir)  [file join "$env(HOME)" "Pinktest"]
set PINKTEST(comdir)   [file join "$PINK" "src" "com"]
if {($argc == 1)} { 
  set PINKTEST(operator)     [lindex $argv 0]
  set PINKTEST(filelist) [glob "$PINKTEST(comdir)/$PINKTEST(operator).c*"]
} else {
  set PINKTEST(operator)     ""
  set PINKTEST(filelist) [glob "$PINKTEST(comdir)/*.c*"]
}

proc tmpfile {tmpname} {
  global PINKTEST
  return [file join "/tmp" "$PINKTEST(name)_$tmpname"]
}

my_exec_q rm -f [tmpfile file1] [tmpfile file2]

foreach file $PINKTEST(filelist) {
  my_exec_q grep -h --color=never $PINKTEST(testtag) $file >> [tmpfile file1]
}

set sedcom1 "s|%TEST||g"
set sedcom2 "s|%IMAGES|$PINKTEST(testdir)/images|g"
set sedcom3 "s|%RESULTS|$PINKTEST(testdir)/results|g"

my_exec_q sed -e $sedcom1 -e $sedcom2 -e $sedcom3 [tmpfile file1] > [tmpfile file2]

set PINKTEST(input) [open [tmpfile file2]]

while {[gets $PINKTEST(input) comexec] >= 0} {
#  puts "$comexec"
  set res [my_exec_q $comexec]
  if {$res != ""} {
    puts "EXECUTION ERROR/WARNING: $comexec"
    puts $res
  }
  set tag "results/"
  set ltag [string length $tag]
  set ind [string first $tag $comexec]
  set filename [string range $comexec [expr $ind+$ltag] end]
  set comcp "cp -f $PINKTEST(testdir)/results/$filename $PINKTEST(testdir)/results_prev/$filename"
  puts $comcp
  my_exec_q $comcp
}

close $PINKTEST(input)
