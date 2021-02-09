# UjoImro,HT, 2010-212
# CeCILL free-software license

# Hugues' TP-1

from pink import Imview
from pink import cpp as pink

#ex 3.1-1
#help(pink.geoeros)

#ex 3.1-2
I=pink.readimage("../images/cells.pgm")

viewer1=Imview(I)

eroded = pink.erosball( I, 1, 0 )
viewer1.show(eroded,"eroded") # spacebar to switch from one image to the next

#ex 3.1-3
recons1 = pink.geodilat(eroded, I, 8, 10)
viewer1.show(recons1,"recons1")
#recons1.writeimage("recons1.pgm")


#ex 3.1-4
# we start from the already partially denoised result
dilated = pink.dilatball(recons1, 1, 0)
recons2 = pink.geoeros(dilated, recons1, 8, 10)

# we save this result
#recons2.writeimage("recons2.pgm")

#ex 3.1-5

# a procedure to do everything as a function
def filter_noise(image):
    eroded  = pink.erosball( image, 1, 0 )
    recons1 = pink.geodilat(eroded, image, 8, 10)
    dilated = pink.dilatball(recons1, 2, 0)
    result  = pink.geoeros(dilated, recons1, 8, 10)
    return result

cells_filt = filter_noise(I)
# save the result
cells_filt.writeimage("cells_filt.pgm")

###
#ex 3.2-2
def remove_touching( image ):
    frame  = pink.frame(image, 0)
    seeds  = image - frame
    #seeds.writeimage("seeds.pgm")
    result = image - pink.geodilat(seeds, image, 8, -1)
    return result
    
cells_notouch = remove_touching(cells_filt)
viewer1.show(cells_notouch,"cells_notouch")
#cells_nohole.writeimage("cells_nohole.pgm")


#ex 3.3-2
inv = pink.inverse(cells_notouch)
holes = remove_touching(inv)

cells_filled = pink.max(cells_notouch, holes)
viewer1.show(cells_filled,"cells_filled")
#cells_filled.writeimage("cells_filled.pgm")


#ex 3.4-2
objects = pink.geodilat( holes, cells_filled, 8, -1 )
cells_final = pink.min( cells_filled, objects )
viewer1.show(cells_final,"cells_final")
#cells_final.writeimage("cells_final.pgm")

#ex 4
## filtering and inverting
circuit = pink.readimage("../images/circuit.pgm")
inv = pink.inverse(circuit)
inv = pink.seuil(inv, 180, 0 ,255)
inv = filter_noise(inv)
viewer2=Imview(inv)
#inv.writeimage("circ.pgm")

# Filtering out the strips to get the round ends
round = pink.openball(inv, 5,0)
# plus one round of geodesic dilation to get the shape back
roundok = pink.geodilat(round, inv, 8, 1) # note the 1 here
viewer2.show(roundok, "roundok")

# get the strips by difference
strips = inv - roundok
viewer2.show(strips, "strips")


## dilation
structuring_element = pink.char_image([11,1])
structuring_element.fill(255)
structuring_element.writeimage("se.pgm")
structuring_element.center=[5,0]
dilated = pink.dilation( inv, structuring_element )
#dilated.writeimage("dil.pgm")
viewer2.show(dilated,"dilated")













# LuM end of file
