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
exec tclsh $0 $@

#########################
#
# minctopgm.tcl (linux with minc installed)
#
# converts each minc image file in 'filelist' to the Pink pgm format
# requires minc software
# example: minctopgm.tcl im*.mnc /tmp
#
# Author: Andre Saude dec 2004
#
#########################

#parameters
set l [llength $argv]

if {$l == 0} {
    puts "USAGE: minctopgm.tcl filelist \[output-dir\]"
    puts "\t converts each minc image file in 'filelist' to the Pink pgm format"
    puts "\t requires minc software"
    puts "\t example: minctopgm.tcl im*.mnc /tmp"
}

set outdir [string trim [lindex $argv [expr $l - 1]] "/"]
if [file isdirectory $outdir] {
    set argv [lrange $argv 0 [expr $l - 2]]
} else {
    set outdir "."
}

#doit
foreach i $argv {
    #filenames
    regsub ".mnc" $i ".raw" auxf
    if {[string first ".raw" $auxf] < 0} {set auxf $auxf.raw}
    set auxf $outdir/$auxf

    regsub ".mnc" $i ".pgm" outf
    if {[string first ".pgm" $outf] < 0} {set outf $outf.pgm}
    set outf $outdir/$outf

    #verbose
    puts -nonewline "$i -> $auxf -> $outf \t\t"

    #dimensions
    set inf [exec mincinfo $i]

    set aux [expr [string last xspace $inf] + 10]
    set xspace [string trim [string range $inf $aux [expr $aux + 30]]]
    set aux [expr [string last yspace $inf] + 10]
    set yspace [string trim [string range $inf $aux [expr $aux + 30]]]
    set aux [expr [string last zspace $inf] + 10]
    set zspace [string trim [string range $inf $aux [expr $aux + 30]]]

    #minctoraw
    exec minctoraw -normalize $i > $auxf

    #raw2pgm
    exec raw2pgm $auxf $zspace $yspace $xspace 0 1 0 $outf

    #verbose
    puts "done"
}
