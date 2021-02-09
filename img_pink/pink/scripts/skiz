#!/bin/sh
USAGE="Usage: $0 in connex out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
dist $1 0 /tmp/_skiz1
long2byte /tmp/_skiz1 2 /tmp/_skiz1
watershed /tmp/_skiz1 $1 null null $2 $3
rm /tmp/_skiz1
