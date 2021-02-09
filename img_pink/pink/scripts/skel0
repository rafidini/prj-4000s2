#!/bin/sh
USAGE="Usage: $0 in out"
if [ $# -gt 2 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 2 ]
then
	echo $USAGE
        exit
fi
medialaxis $1 3 /tmp/skel0_tmp_m
seuil /tmp/skel0_tmp_m 1 /tmp/skel0_tmp_ms
distc $1 3 /tmp/skel0_tmp_d
skeleucl $1 8 /tmp/skel0_tmp_ms $2
rm -f /tmp/skel0_tmp_*
