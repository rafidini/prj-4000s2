#!/bin/sh
USAGE="Usage: $0 in es out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi
closing $1 $2 /tmp/_blacktophat_1
sub /tmp/_blacktophat_1 $1 $3
rm /tmp/_blacktophat_1

