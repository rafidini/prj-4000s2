# gradient
gradientcd joints.pgm 1 _1

# obtention de marqueurs (un peu plus que souhaité)
inverse _1 _2
segmentheight _2 8 5 _3

# première lpe (sur-segmentation)
watershed _1 _3 4 _4

# suppression des petites inclusions
inverse _4 _5
segmentarea _5 4 100 _6
watershed _1 _6 4 _7

# extraction de la porosité "mince"
closing joints.pgm $PINK/masks/disque7.pgm _8
sub _8 joints.pgm _9
seuil _9 17 _10

# extraction de la porosité "épaisse"
seuil joints.pgm 150 _11
inverse _11 _11
areaselnb _11 4 1 _12
dilation _12 $PINK/masks/vois4.pgm _12

# joints
sub _7 _12 _13
area _13 joints.dat

# joints corrodes
dilation _10 $PINK/masks/vois4.pgm _14
min _13 _14 _15
area _15 corro.dat

# visualisation 
showsurimp _13 _15&
