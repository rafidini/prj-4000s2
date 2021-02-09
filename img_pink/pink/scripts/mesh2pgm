#!/bin/sh
USAGE="Usage: $0 in[.ifs] resolution height"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
meshconvert $1.ifs $2 _0.pgm
enframe _0.pgm 0 10 _0.pgm
dist _0.pgm 0 _d
long2byte _d 2 _d
segmentheight _d 6 $3 m _s
areaselnb _s 6 2 _1
frame _1 _f
max _1 _f _1
inverse _d _i
watershedMeyer _i _1 null null 6 $1.pgm

