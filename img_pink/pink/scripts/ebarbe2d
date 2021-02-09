#!/bin/sh
USAGE="Usage: $0 in connex k out"
if [ $# -ne 4 ]
then
	echo $USAGE
        exit
fi

if [ $2 -eq 4 ]
then
	CONNEX=4
	CONNEXBAR=8
else
  if [ $2 -eq 8 ]
  then
	CONNEX=8
	CONNEXBAR=4
  else
        echo "bad value for connex (4,8 only)"
        exit
  fi
fi

hthin       $1 null $CONNEXBAR $3 /tmp/_ebarbe2
ptend       /tmp/_ebarbe2 $CONNEX /tmp/_ebarbe3
ptisolated  /tmp/_ebarbe2 $CONNEX /tmp/_ebarbe4
max         /tmp/_ebarbe4 /tmp/_ebarbe3 /tmp/_ebarbe5
sub         $1 /tmp/_ebarbe2 /tmp/_ebarbe6
max         /tmp/_ebarbe6 /tmp/_ebarbe5 /tmp/_ebarbe7
geodilat    /tmp/_ebarbe5 /tmp/_ebarbe7 $CONNEX $3 /tmp/_ebarbe8
max         /tmp/_ebarbe2 /tmp/_ebarbe8 $4
rm -f       /tmp/_ebarbe?
