#!/bin/sh
USAGE="Usage: $0 in.gif [factor|x rs|y cs] out.gif"
if [ $# -gt 4 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 3 ]
then
	echo $USAGE
        exit
fi
convert     $1 _$1.ppm
ppm2pgm     _$1.ppm _$1.r _$1.g _$1.b
if [ $# -gt 3 ]
then
  zoom        _$1.r  $2 $3 _$1.r
  zoom        _$1.g  $2 $3 _$1.g
  zoom        _$1.b  $2 $3 _$1.b
  pgm2ppm     _$1.r _$1.g _$1.b _$1.ppm
  convert     _$1.ppm $4
fi
if [ $# -lt 4 ]
then
  zoom        _$1.r  $2  _$1.r
  zoom        _$1.g  $2  _$1.g
  zoom        _$1.b  $2  _$1.b
  pgm2ppm     _$1.r _$1.g _$1.b _$1.ppm
  convert     _$1.ppm $3
fi
rm          _$1.r _$1.g _$1.b _$1.ppm

