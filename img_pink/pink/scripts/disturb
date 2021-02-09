#!/bin/bash
# perturbe la forme


PROGRAMNAME=$0
USAGE="usage: $PROGRAMNAME in.pgm p_eden p_asft out.pgm"
VERBOSE=1
INPUTIMAGE=$1
P_EDEN=$2
P_ASFT=$3
OUTPUTIMAGE=$4

if [ $# -ne 4 ]
then
	echo $USAGE
	exit
fi

addedennoise $INPUTIMAGE $P_EDEN /tmp/_disturb_e
asft /tmp/_disturb_e null null 26 $P_ASFT $OUTPUTIMAGE
rm /tmp/_disturb_e

