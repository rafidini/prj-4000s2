# Michel Couprie, 2011
# CeCILL free-software license

# make statistics on grains in a 2d image divided into tiles

from pink import imview as imview
from pink import cpp as pink
import random
global DEBUG
DEBUG=1

# returns the maximum value in image
## for this problem, there is a function in pink
# def max1(image):
#     res = image[0]
#     for i in range(image.size.prod()):
#         if image[i] > res:
#             res = image[i]
#     return(res)


# extracts subimage from image
def crop(image, x, y, w, h):
    res = pink.char_image([w,h])
    res = pink.insert_image(res, image, [-x,-y])
    return(res)

RS = 4 # numbers of tiles in a row
CS = 4 # numbers of tiles in a column
rs = 100 # row size for a tile
cs = 100 # column size for a tile

nb_grains = 0
grain_size = 0

for J in range(CS):
    for I in range(RS):
        filename = 'tile%02d.pgm'%(J*CS+I)
        tile = pink.readimage(filename)
        # separate objects into interior and border objects
        frame = pink.frame( pink.char_image(tile.size), 255 )
        border_objects = pink.geodilat( frame, tile, 8 )
        interior_objects = tile - border_objects
        # measure interior objects
        grain_size = grain_size + pink.area(interior_objects)
        lab = pink.labelfgd(interior_objects, 8)
        nb_grains = nb_grains + pink.minmax(lab)[1]
        if DEBUG:
            print("I,J,s,n: " + str(I) + " " + str(J) + " " + str(grain_size) + " " + str(nb_grains))
        # deal with border objects
        if (I < RS-1) and (J < CS-1): 
            filename_r = 'tile%02d.pgm'%(J*CS+I+1)
            tile_r = pink.readimage(filename_r)
            filename_d = 'tile%02d.pgm'%((J+1)*CS+I)
            tile_d = pink.readimage(filename_d)
            filename_rd = 'tile%02d.pgm'%((J+1)*CS+I+1)
            tile_rd = pink.readimage(filename_rd)
            # catenate images and build marker
            marker = pink.char_image([2*rs,2*cs])
            marker = pink.insert_image(marker, border_objects, [0,0])
            tile_4 = pink.insert_image(marker, tile_r, [rs,0])
            tile_4 = pink.insert_image(tile_4, tile_d, [0,cs])
            tile_4 = pink.insert_image(tile_4, tile_rd, [rs,cs])
            # grow marker
            border_grains = pink.geodilat( marker, tile_4, 8 )
            # get images _r _d and _rd with only border grains
            gr_r = crop(border_grains, rs, 0, rs, cs)
            gr_d = crop(border_grains, 0, cs, rs, cs)
            gr_rd = crop(border_grains, rs, cs, rs, cs)
            # remove border grains in original _r _d and _rd images
            tile_r = tile_r - gr_r
            tile_d = tile_d - gr_d
            tile_rd = tile_rd - gr_rd
            # save these images
            tile_r.writeimage(filename_r)
            tile_d.writeimage(filename_d)
            tile_rd.writeimage(filename_rd)
            # measure border objects
            lab = pink.labelfgd(border_grains, 8)
            n = pink.minmax(lab)[1]
            nb_grains = nb_grains + n
            if DEBUG and (n > 0):
                print "grains on border: " + str(n) 
            grain_size = grain_size + pink.area(border_grains)	
        elif (I < RS-1): 
            filename_r = 'tile%02d.pgm'%(J*CS+I+1)
            tile_r = pink.readimage(filename_r)
            # catenate images and build marker
            marker = pink.char_image([2*rs,cs])
            marker = pink.insert_image(marker, border_objects, [0,0])
            tile_2 = pink.insert_image(marker, tile_r, [rs,0])
            # grow marker
            border_grains = pink.geodilat( marker, tile_2, 8 )
            # get image _r with only border grains
            gr_r = crop(border_grains, rs, 0, rs, cs)
            # remove border grains in original _r image
            tile_r = tile_r - gr_r
            # save this image
            tile_r.writeimage(filename_r)
            # measure border objects
            lab = pink.labelfgd(border_grains, 8)
            n = pink.minmax(lab)[1]
            nb_grains = nb_grains + n
            if DEBUG and (n > 0):
                print "grains on border: " + str(n) 
            grain_size = grain_size + pink.area(border_grains)
        elif (J < CS-1): 
            filename_d = 'tile%02d.pgm'%((J+1)*CS+I)
            tile_d = pink.readimage(filename_d)
            # catenate images and build marker
            marker = pink.char_image([rs,2*cs])
            marker = pink.insert_image(marker, border_objects, [0,0])
            tile_2 = pink.insert_image(marker, tile_d, [0,cs])
            # grow marker
            border_grains = pink.geodilat( marker, tile_2, 8 )
            # get image _d with only border grains
            gr_d = crop(border_grains, 0, cs, rs, cs)
            # remove border grains in original _d image
            tile_d = tile_d - gr_d
            # save this image
            tile_d.writeimage(filename_d)
            # measure border objects
            lab = pink.labelfgd(border_grains, 8)
            n = pink.minmax(lab)[1]
            nb_grains = nb_grains + n
            if DEBUG and (n > 0):
                print "grains on border: " + str(n) 
            grain_size = grain_size + pink.area(border_grains)
            
print "grain size = " + str(grain_size)
print "number of grains = " + str(nb_grains)
