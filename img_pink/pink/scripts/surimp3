#!/bin/sh
# $1 : image en ndg
# $2 : image binaire, objet 255, fond 0
# $3 : image binaire, objet 255, fond 0
# $4 : image resultat
USAGE="Usage: $0 i1.ndg i2.bin i3.bin out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi
inverse     $2           /tmp/surimpinv1
inverse     $3           /tmp/surimpinv2
min         $1           /tmp/surimpinv1   /tmp/surimpres
min         /tmp/surimpres   /tmp/surimpinv2   /tmp/surimpres
add         /tmp/surimpres  $2             /tmp/surimpmax1
add         /tmp/surimpres  $3             /tmp/surimpmax2 
pgm2ppm    /tmp/surimpmax1 /tmp/surimpmax2 /tmp/surimpres  $4
#rm         /tmp/surimp*
