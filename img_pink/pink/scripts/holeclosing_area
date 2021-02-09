#!/bin/sh
USAGE="Usage: $0 in connex area out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi
holeclosing $1 $2 -1  /tmp/_holeclosing_area.1
sub /tmp/_holeclosing_area.1 $1 /tmp/_holeclosing_area.2
areaopening /tmp/_holeclosing_area.2 $2 $3 /tmp/_holeclosing_area.3
sub /tmp/_holeclosing_area.2 /tmp/_holeclosing_area.3 /tmp/_holeclosing_area.4
add $1 /tmp/_holeclosing_area.4 $4
rm -f       /tmp/_holeclosing_area.?
