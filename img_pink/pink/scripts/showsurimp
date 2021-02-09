#!/bin/sh
USAGE="Usage: $0 image_ndg image_bin"
if [ $# -ne 2 ]
then
	echo $USAGE
        exit
fi
# $1 : image en ndg
# $2 : image binaire, objet 255, fond 0
inverse     $2           /tmp/surimpinv
min         $1           /tmp/surimpinv   /tmp/surimpmin
add        /tmp/surimpmin  $2             /tmp/surimpmax  
pgm2ppm    /tmp/surimpmax /tmp/surimpmin   /tmp/surimpmin   /tmp/surimpmin
xv         /tmp/surimpmin
rm         /tmp/surimp*
