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
#!tclsh

if {$argc != 4} { 
  puts stderr "usage: selcomp in n m out"
  exit 
}

set HASF(infilename) [lindex $argv 0]
set HASF(n) [lindex $argv 1]
set HASF(m) [lindex $argv 2]
set HASF(outfilename) [lindex $argv 3]

catch {exec cp $HASF(infilename) "/tmp/HASF_1"} result
for {set x 0} {$x<$HASF(m)} {incr x} {
  puts "step $x: shrink"
  catch {exec shrink "/tmp/HASF_1" $HASF(n) "/tmp/HASF_1"} result
  catch {exec inverse "/tmp/HASF_1" "/tmp/HASF_1"} result
  puts "step $x: grow"
  catch {exec shrinkc "/tmp/HASF_1" $HASF(n) "/tmp/HASF_1"} result
  catch {exec inverse "/tmp/HASF_1" "/tmp/HASF_1"} result
}
catch {exec cp "/tmp/HASF_1" $HASF(outfilename)} result
catch {exec rm "/tmp/HASF_1"} result

