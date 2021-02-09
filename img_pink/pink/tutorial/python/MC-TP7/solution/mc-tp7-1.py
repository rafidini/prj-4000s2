# Michel Couprie, 2011
# CeCILL free-software license

# generation of random grains and image splitting

from pink import imview as imview
from pink import cpp as pink
import random
global DEBUG
DEBUG=1

# puts a proportion p of white pixels in image
def randimage(image, p):
    image.fill(0)
    for i in range(image.size.prod()):
        if random.random() <= p:
            image[i] = 255

# puts a proportion p of white pixels in image (except on borders)
def randimage2(image, p, margin):
    image.fill(0)
    rs = image.size[0]
    cs = image.size[1]
    for j in range(margin, cs-margin):
        for i in range(margin, rs-margin):
            if random.random() <= p:
                image[[i,j]] = 255

rad = 4 # radius of particles
RS = 4 # numbers of tiles in a row
CS = 4 # numbers of tiles in a column
rs = 100 # row size for a tile
cs = 100 # column size for a tile
pr = 0.0004 # proportion of particle centers

img = pink.char_image([RS*rs,CS*cs])
randimage2(img, pr, rad+1)
img = pink.dilatball(img, rad)
img.writeimage("wholeimage.pgm")
imview(img)

grain_size = pink.area(img)
lab = pink.labelfgd(img, 8)
nb_grains = pink.minmax(lab)[1]

print("grain_size = " + str(grain_size))
print("nb_grains = " + str(nb_grains))

img = pink.readimage("wholeimage.pgm")
tile = pink.char_image([rs,cs])
for J in range(CS):
    for I in range(RS):
        filename = 'tile%02d.pgm'%(J*CS+I)
        for j in range(cs):
            for i in range(rs):
                tile[[i,j]] = img[[I*rs+i, J*cs+j]]
        tile.writeimage(filename)
