#!/bin/sh
# calcul des invariants topologiques en 3D et en 26 connexite
USAGE="Usage: $0 in"
if [ $# -ne 1 ]
then
	echo $USAGE
        exit
fi

3dkhalimskize $1 h _invariants_$1.k
3dinvariants _invariants_$1.k
rm -f _invariants_$1.k

