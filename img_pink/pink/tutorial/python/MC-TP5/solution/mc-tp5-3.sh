#!/bin/bash

INPUT="trois003.pgm"
RAD=50

any2complex $INPUT _c
fft _c _f
complex_modulus _f _fm
float2byte _fm 4 _fm

#generate mask _r
genimage 2048 2048 1 0 _0
point _0 1024 1024 0 255 _1
dilatball _1 $RAD _1
drawline _0 0 1024 2047 1024 _2
sub _2 _1 _3
dilatball _3 15 8 _r

#convert and smooth mask
inverse _r _r
normalize _r 0 1 _r
byte2float _r _r
gaussianfilter _r 0.7 _g

#apply mask 
mult _f _g _5

complex_modulus _5 _5m
float2byte _5m 4 _5m

#reverse fft
fft _5 1 _6
complex_modulus _6 _7
float2byte _7 2 _8
