#!/bin/sh
USAGE="Usage: $0 in connex out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
cadre           $1          _$1.tmp
geodilat        _$1.tmp     $1        $2    -1     _$1.tmp
sub             $1          _$1.tmp   $3
rm              _$1.tmp

