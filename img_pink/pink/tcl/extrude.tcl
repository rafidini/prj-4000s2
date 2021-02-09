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
exec tclsh $0 $1 $2 $3

if {$argc != 3} { 
  puts stderr "usage: extrude in2d.pgm nplanes out3d.pgm"
  exit 
}

set EXTRUDE(name) "extrude"
set EXTRUDE(infile) [lindex $argv 0]
set EXTRUDE(nplanes) [lindex $argv 1]
set EXTRUDE(outfile) [lindex $argv 2]

for {set j 0} {$j < $EXTRUDE(nplanes)} {incr j 1} {
  set n [format "%04d" $j]
  set filename_out "$EXTRUDE(name)_$n.pgm"  
  catch { exec cp $EXTRUDE(infile) $filename_out } mes
}
catch { exec catpgm "$EXTRUDE(name)_" 0 [expr $EXTRUDE(nplanes)-1] $EXTRUDE(outfile) } mes
for {set j 0} {$j < $EXTRUDE(nplanes)} {incr j 1} {
  set n [format "%04d" $j]
  set filename_out "$EXTRUDE(name)_$n.pgm"  
  catch { exec rm -f $EXTRUDE(infile) $filename_out } mes
}




