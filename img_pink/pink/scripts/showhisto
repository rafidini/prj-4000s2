#!/bin/sh
USAGE="Usage: $0 image.pgm [mask.pgm]"
if [ $# -gt 2 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 1 ]
then
	echo $USAGE
        exit
fi
if [ $# -eq 1 ]
then
        histo $1 _$1.histo
fi
if [ $# -eq 2 ]
then
        histo $1 $2 _$1.histo
fi
xgraph _$1.histo
rm _$1.histo
