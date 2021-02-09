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
#!/bin/sh
# The next line is executed by /bin/sh, but not Tcl \
exec tclsh $0 $@

#########################
# 
# execlist.tcl pinkCommand inputFileList [options]
#
# executes the Pink command 'pinkCommand' for the list of files in 'filelist'
# and write the output for each image in filename'outputSuffix'.pgm
#
# Arguments:
#    pinkCommand:   a Pink executable
#    inputFileList: the list of input files
#    [options]:      optional arguments
#        -a args:         the arguments for the Pink command
#        -o outputSuffix: the suffix of the output files; default: _$pinkCommand_$arg1_$arg2_...
#        -d outdir:       the output directory
#
# example: execlist.tcl seuil im*.pgm -a 12 -o _seuil12 -d /tmp"
#
# Author: Andre Saude dec 2004
#
#########################

#usage
proc printUsage {} {
    puts "\nUSAGE: execlist.tcl pinkCommand inputFileList \[options\]"
    puts "\nExecutes the Pink command 'pinkCommand' for the list of files in 'filelist'"
    puts "and write the output for each image in filename'outputSuffix'.pgm"
    puts "\n\[options\]:      optional arguments"
    puts "\t -a args:         the arguments for the Pink command"
    puts "\t -o outputSuffix: the suffix of the output files; default: _pinkCommand_arg1_arg2_..."
    puts "\t -d outdir:       the output directory"
    puts "\nExample: execlist.tcl seuil im*.pgm -a 12 -o _seuil12 -d /tmp"
    puts "\t executes a threshold at 12 for each file in im*.pgm"
    puts "\t and write the output for each image in filename_seuil12.pgm"
}

#parameters
if {$argc < 2} {
    printUsage
    exit
}

set order [list $argc $argc $argc $argc]
set occurence 0

set ai 0
set oi 0
set di 0
set a $argc
set o $argc
set d $argc

set nooutdir 1
set nooutsuf 1

for {set i 0} {$i < $argc} {incr i} {
    switch -exact -- [lindex $argv $i] {
	-a {
	    if {$a != $argc} {
		puts "ERROR: -a option given twice"
		printUsage
		exit
	    }
	    set a $i
	    set ai $occurence
	    lset order $ai $a
	    incr occurence
	}
	-o {
	    if {$o != $argc} {
		puts "ERROR: -o option given twice"
		printUsage
		exit
	    }
	    set o $i
	    set oi $occurence
	    lset order $oi $o
	    set nooutsuf 0
	    incr occurence
	}
	-d {
	    if {$d != $argc} {
		puts "ERROR: -d option given twice"
		printUsage
		exit
	    }
	    set d $i
	    set di $occurence
	    lset order $di $d
	    set nooutdir 0
	    incr occurence
	}
    }
}

#the parameters, finally
set pinkCommand [lindex $argv 0]
set filelist [lrange $argv 1 [expr [lindex $order 0] - 1]]
set args [lrange $argv [expr $a + 1] [expr [lindex $order [expr $ai + 1]] - 1]]
set outsuf [lrange $argv [expr $o + 1] [expr [lindex $order [expr $oi + 1]] - 1]]
set outdir [lrange $argv [expr $d + 1] [expr [lindex $order [expr $di + 1]] - 1]]

#test them
if {[llength $filelist] < 1} {
    puts "ERROR: no input file given"
    printUsage
    exit
}
if {[llength $outsuf] > 1} {
    puts "ERROR: more than one output suffix given"
    printUsage
    exit
}
if {[llength $outdir] > 1} {
    puts "ERROR: more than one output directory given"
    printUsage
    exit
}

#correct optional parameters
if {!$nooutdir} {
    puts carai
    if {![file isdirectory $outdir]} {
	puts "ERROR: -d option given is not a directory"
	printUsage
	exit
    }
}
if {!$nooutsuf} {
    if {[llength $outsuf] != 1} {
	set nooutsuf 1
    }
}
if {$nooutsuf} {
    set outsuf "_$pinkCommand"
    foreach i $args {
	set outsuf "$outsuf\_$i"
    }
}

#doit
foreach i $filelist {
    #filenames
    set ext [file extension $i]
    set fname [file rootname [file tail $i]]
    if {$nooutdir} {
	set outdir [file dirname $i]
    }
    set outf [file join $outdir $fname$outsuf$ext]
    set inf [file join [file dirname $i] [file tail $i]]

    #verbose
    puts -nonewline "\n$pinkCommand $i $args $outf ... \t\t"

    #exec
     set eargs [concat $inf $args $outf]
     set call "catch { exec $pinkCommand $eargs } result"
     eval $call
     if {$result != ""} {
 	puts "\n\nERROR in execution:"
 	puts "\t$result"
 	puts "\nAborted!"
 	exit
     }

    #verbose
    puts "done"
}
