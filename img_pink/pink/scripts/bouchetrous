#!/bin/sh
USAGE="Usage: $0 in connex out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
inverse      $1     /tmp/_bouchetrous.i
cadre        $1     /tmp/_bouchetrous.c
geodilat    /tmp/_bouchetrous.c  /tmp/_bouchetrous.i  $2 -1 /tmp/_bouchetrous.r
inverse     /tmp/_bouchetrous.r  $3
rm -f       /tmp/_bouchetrous.?
