#! /bin/bash
# -*- sh -*-

# FILENAMES="ldilateros3d lerosfast3d"
# EXPRS="lerosfast3d"

# for q in $FILENAMES; 
# do
#     for w in $EXPRS;
#     do
#         #echo sed s/$EXPR/$q\_$EXPR/ "< $q.c > $q.c_"
# 	sed s/$w/$q\_$w/ < $q.c > $q.c_
# 	mv $q.c_ $q.c
#     done;
# done;

TARGET=nbvoisc6
#for FILENAME in lcurves lgettree llabeltree lseltopo lskelcurv lskelpar3d ;
#for FILENAME in lfermetrous3d lfiltrestopo lhtkern3d lnbtopo lseltopo lskelcurv lskeletons lskelpar3d lsquelval ltopotypes ;
for FILENAME in l2dcollapse larith lasft lclick lconvol lderiche ldilateros ldiZenzo ldynamique_grimaud leros lfermetrous3d lgeo lgeodesic lhierarchie lhtkern3d lhtkern  llambdakern llpeGA lmedialaxis lMSF lppm2GA lrecalagerigide lrecalagerigide_translateplane mccodimage mcgeo mcgraphe mcimage mckhalimsky2d mckhalimsky3d mcpowell mctopo3d mctopo ;
#for FILENAME in lmedialaxis lMSF lppm2GA lrecalagerigide lrecalagerigide_translateplane mccodimage mcgeo mcgraphe mcimage mckhalimsky2d mckhalimsky3d mcpowell mctopo3d mctopo  ;
do
    sed s/min\(/mcmin\(/g < $FILENAME.c > $FILENAME.c_
#    sed s/mctopo3d_mctopo3d/mctopo3d/g < $FILENAME.c > $FILENAME.c_
    mv $FILENAME.c_ $FILENAME.c
done




# LuM end of file
