#!/bin/sh
USAGE="Usage: $0 in1 in2 out"
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

byte2long $1 /tmp/_1
byte2long $2 /tmp/_2
add /tmp/_1 /tmp/_2 /tmp/_3
scale /tmp/_3 0.5 /tmp/_4
long2byte /tmp/_4 0 $3

rm /tmp/_?


