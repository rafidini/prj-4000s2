#!/bin/sh
# $1 : image en ndg
# $2 : image binaire, objet 255, fond 0
# $3 : image resultat
USAGE="Usage: $0 i1.ndg i2.bin out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
inverse     $2           /tmp/surimpinv
min         $1           /tmp/surimpinv   $3
add         $3           $2             /tmp/surimpmax  
pgm2ppm    /tmp/surimpmax $3   $3   $3
rm         /tmp/surimp*
