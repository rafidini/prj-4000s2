#!/bin/bash
USAGE="Usage: $0 in radius out"
if [ $# -ne 3 ]
then
	echo $USAGE
        exit
fi

mcgetval2()
{
    tail -n 1 $1|awk '{print $1}'
}

IN=$1
RAD=$2
OUT=$3

genimage $IN 0 _closeballlab3
byte2long _closeballlab3 _closeballlab3
countvalues $IN _closeballlab1
NBVAL=`mcgetval2 "_closeballlab1"`
N=1
while [ $N -le $NBVAL ]
do
#  echo "closeballlab $N"
  selndg $IN $N $N _closeballlab2
  seuil _closeballlab2 1 _closeballlab2
  closeball _closeballlab2 $RAD _closeballlab2
  normalize _closeballlab2 0 $N _closeballlab2
  byte2long _closeballlab2 _closeballlab2
  max _closeballlab3 _closeballlab2 _closeballlab3
  N=$(($N+1))
done		
mv _closeballlab3 $OUT
rm -f _closeballlab?
