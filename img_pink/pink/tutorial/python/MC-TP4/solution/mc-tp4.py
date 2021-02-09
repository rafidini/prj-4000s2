# UjoImro, 2010
# CeCILL free-software license

# Michel's TP-4 for 3D images

from pink import view3d, render, seuilmesh
from pink import cpp as pink
inv = pink.inverse

debug = True

# lettre_a = pink.readimage("../images/lettre_a.pgm")
# if debug:
#     view3d(lettre_a)

# ### General skeletonization with toposhrink
# shrunk = pink.toposhrink(lettre_a, pink.distc(lettre_a,0), 26, 1, 1, 1, 1)
# #if debug: # for package generation
# view3d(shrunk)
# # if debug: # for package generation
# render(lettre_a, shrunk)


def fill_the_hole(image, tbmin=1, tbmax=1):
    frame = pink.frame( pink.char_image(image.size), 255)
    dist = pink.dist(image, 0)
    return pink.toposhrink( inv(frame), inv(dist), 26, tbmin, tbmax, 1, 1, image )

# ### Fill in the hole
# frame = pink.frame( pink.char_image(lettre_a.size), 255)
# dist = pink.dist(lettre_a, 0)
# shrunk = pink.toposhrink( inv(frame), inv(dist), 26, 1, 1, 1, 1, lettre_a )
# if debug:
#     render(lettre_a, shrunk)

# ### same with more holes
# tores = pink.readimage("../images/tores.pgm")
# shrunk = fill_the_hole(tores)
# if debug:
#     render(shrunk, tores)

# ### hint: delete the points for which tb==1
# shrunk = fill_the_hole(tores, 0, 10)
# if debug:
#     render(shrunk, tores)



# ### the distance is necessary
# chaise = pink.readimage("../images/chaise.pgm")
# shrunk = fill_the_hole(chaise)
# blank = pink.char_image(chaise.size)
# bad_shrunk = pink.toposhrink( inv(pink.frame(blank,255)), pink.int_image(chaise.size), 26, 0, 10, 1, 1, chaise )

# if debug:
#     render(shrunk, chaise)

### application vertebrae
vertebre = pink.frame(pink.readimage("../images/vertebre.pgm"), 0)
#shrunk = fill_the_hole(vertebre, 0, 10)
#if debug:
#    render(shrunk, vertebre)


# #### REVISION REQUIRED

# ### introduction of the size parameter
# closed1 = pink.holeclosing(vertebre, 26, 0)
# closed2 = pink.holeclosing(vertebre, 26, 10000)
closed3 = pink.holeclosing(vertebre, 26, 6)
#if debug:
render(closed3, vertebre)

# #### end of REVISION REQUIRED

### Greyscale: segmentation

## try a threshold

### homotopic skeleton by shrinking
uo = pink.readimage("../images/uo.pgm")
htkern = pink.htkern(uo, 4)
#htkern.writeimage("image.pgm")
minima = pink.minima(htkern, "4")
#minima.writeimage("image.pgm")

### lambda skeletons
lam = pink.lambdaskel(uo, 4, 15)
minima = pink.minima(lam, "4")

lam = pink.lambdaskel(uo, 4, 24)
minima = pink.minima(lam, "4")
pink.surimp(uo, inv(minima), "surimp.ppm")

### we can also try the gradient with any image


# # ### Greyscale filtering

# ### angiographie
# angio = pink.readimage("../images/angiogra.pgm")
# vois8 = pink.char_image([3,3]).fill(255)
# vois8.center=[1,1]
# rank = pink.rankfilter(angio, vois8, 1, 1, 0.5)
# rank.writeimage("image.pgm")

# ### lennab
# lennab = pink.readimage("../images/lennab.pgm")
# vois25 = pink.char_image([5,5]).fill(255)
# vois25.center=[2,2]
# rank1 = pink.rankfilter(lennab, vois25, 2, 2, 0.5)
# rank2 = pink.rankfilter(lennab, vois8, 2, 2, 0.5)

# ### limits: cobblestones
# paves = pink.readimage("../images/paves.pgm")
# tuf = pink.tuf(paves, 8, 5)
# # (tuf + tlf)*= taf ???????
# taf = pink.taf(paves, 8, 5, 255, 255) ### !!!!!!!!!!!!!!!!!

# tuf.writeimage("tuf.pgm")
# taf.writeimage("taf.pgm")

### greyscale 

# NIVEAUX DE GRIS: restoration de cretes
#-----------------------------------------

# gradient

# amincir

# seuil par hysteresis

# restoration de cretes

### TO BE WRITTEN






# LuM end of file
