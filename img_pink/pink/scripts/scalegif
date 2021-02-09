#!/bin/sh
USAGE="Usage: $0 in.gif factor out.gif"
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
convert     $1 /tmp/scalegif.ppm
ppm2pgm     /tmp/scalegif.ppm /tmp/scalegif.r /tmp/scalegif.g /tmp/scalegif.b
scale       /tmp/scalegif.r  $2  /tmp/scalegif.r
scale       /tmp/scalegif.g  $2  /tmp/scalegif.g
scale       /tmp/scalegif.b  $2  /tmp/scalegif.b
pgm2ppm     /tmp/scalegif.r /tmp/scalegif.g /tmp/scalegif.b /tmp/scalegif.ppm
convert     /tmp/scalegif.ppm /tmp/scalegif.gif
gifsicle    /tmp/scalegif.gif -O -o $3
rm          /tmp/scalegif.r /tmp/scalegif.g /tmp/scalegif.b /tmp/scalegif.ppm

