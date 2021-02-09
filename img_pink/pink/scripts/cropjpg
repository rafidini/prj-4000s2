#!/bin/sh
USAGE="Usage: $0 in.jpg x y w h out.xxx"
if [ $# -gt 6 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 6 ]
then
	echo $USAGE
        exit
fi
convert     $1 /tmp/_$1.ppm
ppm2pgm     /tmp/_$1.ppm /tmp/_$1.r /tmp/_$1.g /tmp/_$1.b
crop        /tmp/_$1.r  $2  $3  $4  $5 /tmp/_$1.r
crop        /tmp/_$1.g  $2  $3  $4  $5 /tmp/_$1.g
crop        /tmp/_$1.b  $2  $3  $4  $5 /tmp/_$1.b
pgm2ppm     /tmp/_$1.r /tmp/_$1.g /tmp/_$1.b /tmp/_$1.ppm
convert     /tmp/_$1.ppm $6
rm          /tmp/_$1.r /tmp/_$1.g /tmp/_$1.b /tmp/_$1.ppm

