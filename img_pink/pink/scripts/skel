#!/bin/sh
USAGE="Usage: $0 in out"
if [ $# -ne 2 ]
then
	echo $USAGE
        exit
fi
medialaxis $1 3 /tmp/skel_tmp_m
seuil /tmp/skel_tmp_m 1 /tmp/skel_tmp_ms
distc $1 3 /tmp/skel_tmp_d
bisector /tmp/skel_tmp_d /tmp/skel_tmp_ms /tmp/skel_tmp_a
seuil /tmp/skel_tmp_a 0.7 /tmp/skel_tmp_i
skeleucl $1 8 /tmp/skel_tmp_i /tmp/skel_tmp_e
skelend /tmp/skel_tmp_e 8 $2
rm -f /tmp/skel_tmp_*
