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
exec tclsh $0

set PINK "$env(PINK)"
source "$PINK/tcl/my_exec.tcl"

set PROBE(name) "probe"

proc tmpfile {tmpname} {
  global PROBE
  return [file join "/tmp" "$PROBE(name)_$tmpname"]
}

proc probefile {f} {
  global PROBE
  my_exec_q seuilhisto $f 0.5 [tmpfile 1]
  my_exec_q encadre [tmpfile 1] 0 [tmpfile 1]
  my_exec_v testprop [tmpfile 1]
}

#set PROBE(filelist) [glob "*.JPG"]
#foreach file $PROBE(filelist) {
#  set name [file rootname $file]
#  exec mv "$name.JPG" "$name.jpg" 
#}

set PROBE(filelist) [glob "*.jpg"]

foreach file $PROBE(filelist) {
  my_exec_q convert $file "$file.ppm"
  my_exec_q rgb2hls "$file.ppm" "/dev/null" "$file.l" "/dev/null"
  probefile "$file.l"
  my_exec_q rm "$file.ppm" "$file.l"
}

catch { exec rm [tmpfile "?"] } mes
