#!/bin/sh
USAGE="Usage: $0 in"
if [ $# -gt 1 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 1 ]
then
	echo $USAGE
        exit
fi
convert $1.ppm $1.gif
gifsicle $1.gif -O -o $1.gif
