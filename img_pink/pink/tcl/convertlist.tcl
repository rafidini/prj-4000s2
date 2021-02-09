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
# convertlist.tcl (linux)
#
# converts each image file in 'list' to the format 'outformat' using convert
# example: convertlist.tcl im*.pgm png /tmp
#
# Author: Andre Saude dec 2004
#
#########################

#parameters
if {$argc < 2} {
    puts "USAGE: convertlist.tcl filelist outformat \[output-dir\]"
    puts "\t converts each image file in 'list' to the format 'outformat'"
    puts "\t example: convertlist.tcl im*.pgm png /tmp"
}

set outdir [string trim [lindex $argv [expr $argc - 1]] "/"]

if [file isdirectory $outdir] {
    set argv [lrange $argv 0 [expr $argc - 2]]
    set argc [llength $argv]
    set nooutdir 0
} else {
    set nooutdir 1
}
set outformat [lindex $argv [expr $argc - 1]]
set argv [lrange $argv 0 [expr $argc - 2]]

#doit
foreach i $argv {
    #filenames
    set ext [file extension $i]
    set fname [string trim [file tail $i] .$ext]
    if {$nooutdir} {
	set outdir [file dirname $i]
    }
    set outf [file join $outdir $fname.$outformat]

    #verbose
    puts -nonewline "$i -> $outf \t\t"

    #seuil
    exec convert $i $outf

    #verbose
    puts "done"
}
