#!/bin/sh
USAGE="Usage: $0 in n out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
addconst $1 $[255-$2] _$1.1
seuil2 _$1.1 $[255-$2] 255 0 255 _$1.1
seuil2 $1 $[$2+1] 255 0 255 _$1.2
inverse _$1.2 _$1.2
addconst _$1.2 $[$2+1] _$1.2
min _$1.1 _$1.2 $3
rm _$1.1
rm _$1.2

