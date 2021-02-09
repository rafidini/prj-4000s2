# UjoImro, 2010
# CeCILL free-software license

# Hugues' TP-2 / Segmentation1 

from pink import cpp as pink


# ex 4.1
numbers = pink.readimage("../images/numbers.pgm")
ball = pink.genball(5)
openn = pink.opening(numbers, ball)
normalized = pink.seuil(numbers - openn, 10, 0, 255)
normalized.writeimage("normalized.pgm")

# ex 5.1-1
cell = pink.readimage("../images/cell.pgm")
ball = pink.genball(1)
dcell = pink.dilation(cell, ball)
ecell = pink.erosion(cell, ball)
pink.normalize(dcell-ecell).writeimage("grad.pgm")

# ex 5.1-2
## HELP ME!!!


#ex 6.1-1
bloodcells = pink.readimage("../images/bloodcells.pgm")
marker = pink.char_image(bloodcells.size)
marker[[50,50]]=255
marker.writeimage("marker.pgm")
ws = pink.watershed(bloodcells, marker, 8)

ws.writeimage("ws.pgm")



# LuM end of file
