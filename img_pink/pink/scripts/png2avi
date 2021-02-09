#!/bin/sh
USAGE="Usage: $0 prefix rowsize colsize frames_per_sec outfilename"
if [ $# -gt 5 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 5 ]
then
	echo $USAGE
        exit
fi
mencoder mf://$1*.png -mf w=$2:h=$3:fps=$4:type=png -ovc lavc -lavcopts vcodec=mpeg4 -oac copy -o $5
