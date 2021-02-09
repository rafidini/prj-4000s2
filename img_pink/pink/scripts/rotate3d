#!/bin/csh
#-------------------------------------------------------
# rotation d'une image 3D
# Michel Couprie - 2008
#-------------------------------------------------------
if ($# > 4) then
	echo "Usage: $0 in theta nbplanes out"
        exit
endif
if ($# < 4) then
	echo "Usage: $0 in theta nbplanes out"
        exit
endif

explode $1 _rotate3d_tmp

foreach file (_rotate3d_tmp*)
  rotate $file $2 _res$file
end

catpgm _res_rotate3d_tmp 0 $3 $4

rm -f _rotate3d_tmp*
rm -f _res_rotate3d_tmp*
