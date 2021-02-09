#!/bin/sh
USAGE="Usage: $0 in p out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
genimage $1 0 /tmp/saltnpepper_tmp_0
bruite /tmp/saltnpepper_tmp_0 1 255 $2 /tmp/saltnpepper_tmp_1
seuil /tmp/saltnpepper_tmp_1 128 /tmp/saltnpepper_tmp_2
sub $1 /tmp/saltnpepper_tmp_2 /tmp/saltnpepper_tmp_3
add $1 /tmp/saltnpepper_tmp_2 /tmp/saltnpepper_tmp_4
inverse $1 /tmp/saltnpepper_tmp_i
min /tmp/saltnpepper_tmp_4 /tmp/saltnpepper_tmp_i /tmp/saltnpepper_tmp_5
add /tmp/saltnpepper_tmp_3 /tmp/saltnpepper_tmp_5 $3
