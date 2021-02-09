#!/bin/sh
USAGE="Usage: $0 in s out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
lambdamedialaxis $1 /tmp/lambdaskeleton_f 
skelcurv2 $1 /tmp/lambdaskeleton_f 26 $2 $3
#rm -f /tmp/lambdaskeleton_*
