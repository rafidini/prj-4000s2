#!/bin/sh
USAGE="Usage: $0 in connex out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
label        $1      $2     min     /tmp/_$1.l
cadre        $1     /tmp/_$1.c
seuil       /tmp/_$1.l    1     /tmp/_$1.s
geodilat    /tmp/_$1.c /tmp/_$1.s   $2   -1     /tmp/_$1.r
inverse     /tmp/_$1.r    $3
rm          /tmp/_$1.l
rm          /tmp/_$1.s
rm          /tmp/_$1.c
rm          /tmp/_$1.r
