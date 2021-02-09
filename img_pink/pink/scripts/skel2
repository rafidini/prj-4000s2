#!/bin/sh
USAGE="Usage: $0 in seuilR (in [1..infnty[) seuilA (in [0.001..pi]) out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi

medialaxis $1 3 /tmp/skel2_tmp_m
seuil /tmp/skel2_tmp_m 1 /tmp/skel2_tmp_m1
skeleucl $1 8 /tmp/skel2_tmp_m1 /tmp/skel2_tmp_s

seuil /tmp/skel2_tmp_m $2 /tmp/skel2_tmp_ms
distc $1 3 /tmp/skel2_tmp_d
bisector /tmp/skel2_tmp_d /tmp/skel2_tmp_ms /tmp/skel2_tmp_a
seuil /tmp/skel2_tmp_a $3 /tmp/skel2_tmp_i
skeleton /tmp/skel2_tmp_s /tmp/skel2_tmp_d 8 /tmp/skel2_tmp_i $4
rm -f /tmp/skel2_tmp_*