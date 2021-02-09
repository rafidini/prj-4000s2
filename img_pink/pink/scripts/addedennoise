#!/bin/bash
# adds Eden noise in percentage of the number of border points
PROGRAMNAME=$0
USAGE="usage: $PROGRAMNAME in.pgm p out.pgm"
VERBOSE=0
INPUTIMAGE=$1
PERCENTAGE=$2
OUTPUTIMAGE=$3

if [ $# -ne 3 ]
then
	echo $USAGE
	exit
fi

return_value()
{
	$1
}

border $INPUTIMAGE 26 _1
area=`return_value "area _1"`
param=`echo "($area*$PERCENTAGE)/100"|bc`
if [ $VERBOSE -eq 1 ]
then
	echo "$PROGRAMNAME: eden param=$param$"
fi
eden $INPUTIMAGE $param 1 1 26 $OUTPUTIMAGE