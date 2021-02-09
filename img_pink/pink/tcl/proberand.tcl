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
  my_exec_q testprop [tmpfile 1]
}

my_exec_q randimage 1000 1000 1 200 _1
my_exec_q seuil _1 193 _2
my_exec_q seuil2 _2 0 1 1 1 _2
my_exec_q genimage 1000 1000 1 0 _3
my_exec_q byte2long _3 _3
my_exec_q bruite _3 1 12000 1 _3
my_exec_q frame _1 _4
my_exec_q seuil2 _4 0 1 1 1 _4
my_exec_q mult _3 _4 _5
my_exec_q mult _2 _5 _6
my_exec_q redt _6 _7
my_exec_q frame _1 _4
my_exec_q max _7 _4 _8
my_exec_q inverse _8 _8

my_exec_q rm [tmpfile "?"]





