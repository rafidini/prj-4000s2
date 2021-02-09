#!/bin/sh
# mesure un critère d'"élongation" pour des objets simplement connexes
# expérimental - ne pas utiliser dans des applications
# implémentation naïve et très inefficace
# M. Couprie 2011
USAGE="Usage: $0 in connex out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi

if [ $2 -ne 8 ]
then
    if [ $2 -ne 26 ]
    then
	echo "bad value for connex (8 or 26 only)"
	exit
    fi
fi

PROGRAMNAME=$0
VERBOSE=1

mcecho()
{
    if [ $VERBOSE -eq 1 ]
    then
	echo "$PROGRAMNAME: $1"
    fi
}

mcrun()
{
    if [ $VERBOSE -eq 1 ]
    then
	echo "$PROGRAMNAME: $1"
    fi
    $1
}

return_value()
{
	$1
}

IMG_IN=$1
CONNEX=$2
IMG_OUT=$3

cp $IMG_IN /tmp/_elong1
genimage /tmp/_elong1 0 /tmp/_elong2
byte2long /tmp/_elong2
fin=0
nstep=0
while [ $fin -eq 0 ]
do 
    nstep=$(($nstep+1))
    mcecho "$nstep"
    cp         /tmp/_elong1 /tmp/_elong1s
    skel_AMK   /tmp/_elong1 1 /tmp/_elong1
    ret=`return_value "$HOME/Pink/linux/bin/equal /tmp/_elong1 /tmp/_elong1s"`
    if [ $ret -eq 1 ]
    then
	fin=1
    else
	ptisolated /tmp/_elong1 $CONNEX /tmp/_elong3
	sub        /tmp/_elong1 /tmp/_elong3 /tmp/_elong1
	byte2long  /tmp/_elong3
	normalize  /tmp/_elong3 0 $nstep /tmp/_elong3
	max        /tmp/_elong3 /tmp/_elong2 /tmp/_elong2
    fi
done

attribute $IMG_IN $CONNEX 0 0 0 /tmp/_elong4           # area
propgeo /tmp/_elong2 $IMG_IN $CONNEX max /tmp/_elong5
long2float /tmp/_elong4
long2float /tmp/_elong5
if [ $CONNEX -eq 8 ]
then
    pow /tmp/_elong5 2 /tmp/_elong5
else
    pow /tmp/_elong5 3 /tmp/_elong5
fi
div /tmp/_elong5 /tmp/_elong4 /tmp/_elong6

cp /tmp/_elong6 $IMG_OUT
#rm -f       /tmp/_elong?
