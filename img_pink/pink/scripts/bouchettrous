#!/bin/sh
USAGE="Usage: $0 in connex tailletrous out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi
attribute    $1      $2      0     min    area    /tmp/_$1.a
seuil       /tmp/_$1.a    $3     /tmp/_$1.s
seuil       /tmp/_$1.a    1      /tmp/_$1.s1
sub         /tmp/_$1.s1  /tmp/_$1.s   /tmp/_$1.t
max          $1     /tmp/_$1.t    $4
rm          /tmp/_$1.a
rm          /tmp/_$1.s
rm          /tmp/_$1.s1
rm          /tmp/_$1.t

