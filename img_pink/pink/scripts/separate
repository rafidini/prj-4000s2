#!/bin/sh
USAGE="Usage: $0 in connex nb out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi
inverse $1 /tmp/_separate_t1
dist /tmp/_separate_t1 0 /tmp/_separate_t1
# finding markers...
long2byte /tmp/_separate_t1 2 /tmp/_separate_t1
heightselnb /tmp/_separate_t1 $2 $3 /tmp/_separate_t2
# watershed...
inverse /tmp/_separate_t1 /tmp/_separate_t1
watershed /tmp/_separate_t1 /tmp/_separate_t2 null null $2 /tmp/_separate_t2
sub $1 /tmp/_separate_t2 $4
rm /tmp/_separate_*
