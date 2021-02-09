#!/bin/sh
USAGE="Usage: $0 in n out"
if [ $# -gt 3 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 3 ]
then
	echo $USAGE
        exit
fi
dilatballnum $1 $2 _$1.tmp 
geoeros _$1.tmp $1 4 -1 $3 
rm _$1.tmp
