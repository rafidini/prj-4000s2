#!/bin/sh
USAGE="Usage: $0 in rad connex out"
if [ $# -gt 4 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 4 ]
then
	echo $USAGE
        exit
fi
openball $1 $2 _$1_op.tmp
sub $1 _$1_op.tmp _$1_res.tmp
skeleton $1 4 $3 _$1_op.tmp _$1_sk.tmp
min _$1_res.tmp _$1_sk.tmp _$1_mark.tmp
geodilat _$1_mark.tmp _$1_res.tmp $3 -1 _$1_rec.tmp
max _$1_op.tmp _$1_rec.tmp $4
#rm _$1_*.tmp
