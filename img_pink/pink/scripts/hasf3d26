#!/bin/sh
USAGE="Usage: $0 in f n out"
# f : threshold for filtering medial axes
# n : number of alternate filter steps
if [ $# -gt 4 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 4 ]
then
	echo $USAGE
        exit
fi
medialaxis  $1 26 0 /tmp/_hasf3d_1
seuil       /tmp/_hasf3d_1 $2 /tmp/_hasf3d_1
inverse     $1 /tmp/_hasf3d_2
medialaxis  /tmp/_hasf3d_2 26 0 /tmp/_hasf3d_2
seuil       /tmp/_hasf3d_2 $2 /tmp/_hasf3d_2
asft        $1 /tmp/_hasf3d_1 /tmp/_hasf3d_2 26 $3 $4


