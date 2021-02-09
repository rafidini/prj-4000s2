#!/bin/bash
USAGE="Usage: $0 object.pgm medax.pgm"
if [ $# -ne 2 ]
then
	echo $USAGE
        exit
fi

# prints the area of the symmetric difference (a\b u b\a) between object and reconstruction from medax

IN=$1
MA=$2

redt $MA _residual_redt
sub $IN _residual_redt _residual_a
sub _residual_redt $IN _residual_b
max _residual_a _residual_b _residual_a
area _residual_a
rm -f _residual_*
