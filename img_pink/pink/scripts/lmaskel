#!/bin/sh
USAGE="Usage: $0 in lambda out"
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
lambdamedialaxis $1 /tmp/lmaskel_1
seuil /tmp/lmaskel_1 $2 /tmp/lmaskel_2
skeleton $1 /tmp/lmaskel_1 8 /tmp/lmaskel_2 $3
rm -f /tmp/lmaskel_?

