#!/bin/sh
# ATTENTION : version valable seulement pour des elements structurants
# symetriques par rapport a l'origine
USAGE="Usage: $0 in n out\n where the size of the opening is n/2"
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

cp $1 /tmp/_e

FIN=`expr $2 / 2`

echo "fin $FIN"
J=0
# attention !! ici _e correspond au dilate et _d au erode

while( test $J -ne $FIN ) do
echo "j $J"
J=`expr $J + 1`
pgm2GA /tmp/_e 1 /tmp/_e
GA2pgm /tmp/_e 1 /tmp/_e
done

PAR=`expr $2 % 2`

if(test $PAR -eq 1 ) then
    echo "Impair"
    pgm2GA /tmp/_e 2 /tmp/_e
    GA2pgm /tmp/_e 1 /tmp/_d
else
    echo "Pair"
    cp /tmp/_e /tmp/_d
fi

J=0
while( test $J -ne $FIN ) do
echo "j $J"
J=`expr $J + 1`
pgm2GA /tmp/_d 2 /tmp/_d
GA2pgm /tmp/_d 2 /tmp/_d
done

cp /tmp/_d $3
rm -f /tmp/_e
rm -f /tmp/_d

exit 0

#pgm2GA ../images/2d/testasft2.pgm 2 /tmp/_d

#erosion        $1  $2  $3
#dilation       $3  $2  $3

