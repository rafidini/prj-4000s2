#!/bin/sh
USAGE="Usage: $0 in lambda out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
lambdamedialaxis $1 /tmp/skel_tmp_l
seuil /tmp/skel_tmp_l $2 /tmp/skel_tmp_m
skeleton $1 /tmp/skel_tmp_l 8 /tmp/skel_tmp_m $3
rm -f /tmp/skel_tmp_*
