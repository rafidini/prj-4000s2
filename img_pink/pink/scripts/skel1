#!/bin/sh
USAGE="Usage: $0 in seuilangle out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
medialaxis $1 3 /tmp/skel1_tmp_m
seuil /tmp/skel1_tmp_m 1 /tmp/skel1_tmp_ms
distc $1 3 /tmp/skel1_tmp_d
bisector /tmp/skel1_tmp_d /tmp/skel1_tmp_ms /tmp/skel1_tmp_a
seuil /tmp/skel1_tmp_a $2 /tmp/skel1_tmp_i
skeleucl $1 8 /tmp/skel1_tmp_i $3
rm -f /tmp/skel1_tmp_*
