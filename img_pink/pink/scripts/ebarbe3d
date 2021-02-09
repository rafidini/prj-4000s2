#!/bin/sh
USAGE="Usage: $0 in connex k out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi

if [ $2 -ne 26 ]
  then
    echo "bad value for connex: 26 only)"
    exit
fi

IMG_IN=$1
CONNEX=$2
NSTEPS=$3
IMG_OUT=$4

skelpar3d   $IMG_IN 8 $NSTEPS /tmp/_ebarbe2
ptend       /tmp/_ebarbe2 $CONNEX /tmp/_ebarbe3
ptisolated  /tmp/_ebarbe2 $CONNEX /tmp/_ebarbe4
max         /tmp/_ebarbe4 /tmp/_ebarbe3 /tmp/_ebarbe5
sub         $IMG_IN /tmp/_ebarbe2 /tmp/_ebarbe6
max         /tmp/_ebarbe6 /tmp/_ebarbe5 /tmp/_ebarbe7
geodilat    /tmp/_ebarbe5 /tmp/_ebarbe7 $CONNEX $NSTEPS /tmp/_ebarbe8
max         /tmp/_ebarbe2 /tmp/_ebarbe8 $IMG_OUT
rm -f       /tmp/_ebarbe?
