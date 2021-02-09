#!/bin/sh
USAGE="Usage: $0 in1 in2 connex out"
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

# deletes from in2 connected components of in2 that are also connected components of in1
# condition : in2 included in in1

geodilat $2 $1 $3 -1 _componentdetect_1
sub _componentdetect_1 $2 _componentdetect_2
geodilat _componentdetect_2 $1 $3 -1 _componentdetect_3
min _componentdetect_3 $2 $4


