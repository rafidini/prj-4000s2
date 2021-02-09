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
exec vtk $0 $1 $2 $3

# A call to the VTK decimatePro algorithm - Michel Couprie 2002
package require vtk

# Hide the default . widget
wm withdraw .

# get arguments
if {$argc != 3} { 
  puts stderr "usage: decimate in reduction out"
  exit 
}
set infilename [lindex $argv 0]
set reduction [lindex $argv 1]
set outfilename [lindex $argv 2]

# load geometry
vtkPolyDataReader reader
    reader SetFileName $infilename

# We want to preserve topology (not let any cracks form). This may limit
# the total reduction possible, which we have specified at 90%.
#
vtkDecimate deci
    deci SetInput [reader GetOutput]
    deci SetTargetReduction $reduction
    deci PreserveTopologyOn
#    deci SetDegree 8
#    deci SetMaximumError 100
#    set maxerr [deci GetMaximumError]
#    puts "max. error = $maxerr"
#    set split [deci GetSplitting]
#    puts "splitting = $split"
#    set degree [deci GetDegree]
#    puts "degree = $degree"
#    set fangle [deci GetFeatureAngle]
#    puts "fangle = $fangle"
#    set sangle [deci GetSplitAngle]
#    puts "sangle = $sangle"
    deci SetAspectRatio 5
    set aspect [deci GetAspectRatio]
#    puts "aspect ratio = $aspect"

# save decimated mesh
vtkPolyDataWriter writer
    writer SetFileName $outfilename
    writer SetInput [deci GetOutput]
    writer Write

exit
