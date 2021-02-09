#!/bin/sh
USAGE="Usage: $0 in elstruct out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
dilation   $1  $2  /tmp/_$1.dilat
erosion    $1  $2  /tmp/_$1.eros
sub /tmp/_$1.dilat /tmp/_$1.eros $3
rm         /tmp/_$1.dilat
rm         /tmp/_$1.eros
