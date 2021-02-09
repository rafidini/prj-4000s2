#!/bin/sh
USAGE="Usage: $0 in n out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
dilatballnum $1 $2 _$1.tmp 
erosballnum _$1.tmp $2 $3 
rm _$1.tmp
