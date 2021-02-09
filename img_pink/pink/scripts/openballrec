#!/bin/sh
# ouverture par reconstruction (erosion suivie d'une reconstruction geodesique) par une boule euclidienne
USAGE="Usage: $0 in r connex out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi
erosball     $1  $2  _$1.eros
geodilat    _$1.eros  $1 $3 -1 $4
rm         _$1.eros
