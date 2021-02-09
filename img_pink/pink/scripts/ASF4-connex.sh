#!/bin/sh
# ATTENTION : version valable seulement pour des elements structurants
# symetriques par rapport a l'origine
USAGE="Usage: $0 in n out\n where the size of the ASF is n/2"
if [ $# -gt 3 ]
then
	echo $USAGE
        exit
fi
if [ $# -lt 3 ]
then
	echo $USAGE
        exit
fi

cp $1 $3
FIN_ASF=`expr $2`
echo "Fin $FIN"

I=`expr 1`

while(test $I -le $FIN_ASF) do
#echo "boucle $I"
echo "${PINK}/scripts/GraphClose.sh $3 $I $3"
P=`expr $I % 2`
if( test $P -eq 0)then
    sh ${PINK}/scripts/GraphClose.sh $3 $I $3
    sh ${PINK}/scripts/GraphOpen.sh $3 $I $3
fi
I=`expr $I + 1`
echo "increment $I"
done

