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
erosballnum $1 $2 _$1.tmp 
dilatballnum _$1.tmp $2 $3 
rm _$1.tmp
