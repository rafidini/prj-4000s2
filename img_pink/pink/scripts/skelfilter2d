#!/bin/sh
USAGE="Usage: $0 in k out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
CONNEX=8
skel_AMK    $1 $2 /tmp/_skelfilter3d2
ptend       /tmp/_skelfilter3d2 $CONNEX /tmp/_skelfilter3d3
ptisolated  /tmp/_skelfilter3d2 $CONNEX /tmp/_skelfilter3d4
max         /tmp/_skelfilter3d4 /tmp/_skelfilter3d3 /tmp/_skelfilter3d5
sub         $1 /tmp/_skelfilter3d2 /tmp/_skelfilter3d6
max         /tmp/_skelfilter3d6 /tmp/_skelfilter3d5 /tmp/_skelfilter3d7
geodilat    /tmp/_skelfilter3d5 /tmp/_skelfilter3d7 $CONNEX $3 /tmp/_skelfilter3d8
max         /tmp/_skelfilter3d2 /tmp/_skelfilter3d8 $3
rm -f       /tmp/_skelfilter3d?
