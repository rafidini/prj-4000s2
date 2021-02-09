#!/bin/sh
USAGE="Usage: $0 in seuilR (in [1..infnty[) seuilA (in [0.001..pi]) out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi
medialaxis $1 3 /tmp/skel3_tmp_m
seuil /tmp/skel3_tmp_m $2 /tmp/skel3_tmp_ms
distc $1 3 /tmp/skel3_tmp_d
bisector /tmp/skel3_tmp_d /tmp/skel3_tmp_ms /tmp/skel3_tmp_a
seuil /tmp/skel3_tmp_a $3 /tmp/skel3_tmp_i
skeleucl $1 26 /tmp/skel3_tmp_i $4
rm -f /tmp/skel3_tmp_*
