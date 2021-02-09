#!/bin/sh
# fermeture par reconstruction (dilatation suivie d'une reconstruction geodesique)
USAGE="Usage: $0 in radius out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
dilatball  $1  $2  _$1.dilat
geoeros    _$1.dilat  $1  4  -1  $3
rm         _$1.dilat
