#!/bin/sh
USAGE="Usage: $0 in lambda out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
lambdamedialaxis $1 /tmp/lambdaskeleton_f
seuil /tmp/lambdaskeleton_f $2 /tmp/lambdaskeleton_m
skeleton $1 /tmp/lambdaskeleton_f 26 /tmp/lambdaskeleton_m $3
rm -f /tmp/lambdaskeleton_*
