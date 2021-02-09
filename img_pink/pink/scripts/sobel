#!/bin/sh
# Sobel's approximation of gradient modulus
# Only in 2D
# Result is stored as float image
USAGE="Usage: $0 in out"
if [ $# -ne 2 ]
then
	echo $USAGE
        exit
fi
convol $1 $PINK/masks/sobel_h.pgm _sobel_h
convol $1 $PINK/masks/sobel_v.pgm _sobel_v
mult _sobel_h _sobel_h _sobel_h
mult _sobel_v _sobel_v _sobel_v
add _sobel_h _sobel_v _sobel_h
pow _sobel_h 0.5 $2
rm -f _sobel_h _sobel_v


