#!/bin/sh
USAGE="Usage: $0 in.pgm mark.pgm connex out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi

inverse $1 _homrec_tmp1
inverse $2 _homrec_tmp2
skeleton _homrec_tmp1 0 $3 _homrec_tmp2 _homrec_tmp3
inverse _homrec_tmp3 $4
rm -f _homrec_*
