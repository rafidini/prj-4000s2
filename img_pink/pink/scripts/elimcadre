#!/bin/sh
USAGE="Usage: $0 in out"
if [ $# -gt 2 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 2 ]
then
	echo $USAGE
        exit
fi
cadre           $1          _$1.tmp
sub             $1          _$1.tmp   $2
rm              _$1.tmp

