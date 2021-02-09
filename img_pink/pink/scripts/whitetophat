#!/bin/sh
USAGE="Usage: $0 in es out"
if [ $# -gt 3 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 3 ]
then
	echo $USAGE
        exit
fi

opening $1 $2 /tmp/_whitetophat_1
sub $1 /tmp/_whitetophat_1 $3

rm /tmp/_whitetophat_1

