#!/bin/sh
# calcul des invariants topologiques en 2D et en 8 connexite
USAGE="Usage: $0 in"
if [ $# -ne 1 ]
then
	echo $USAGE
        exit
fi

2dkhalimskize $1 h _invariants2d_$1.k
2dinvariants _invariants2d_$1.k
rm -f _invariants2d_$1.k

