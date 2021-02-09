# Michel Couprie, 2011
# CeCILL free-software license

# Segmentation of cells

from pink import imview as imview
from pink import manipulate as manipulate
from pink import cpp as pink
inv=pink.inverse
global debug
debug=1

# segmentation
def segment(image, height):
    # obtain markers
    mark1 = pink.segmentheight(image, 4, height, 0)
    if debug:
        mark1.writeimage("_mark1")

    # watershed
    wshed1 = pink.watershed(image, mark1, 4)
    if debug:
        wshed1.writeimage("_wshed1")
    return inv(wshed1)

global im
im = pink.readimage("../images/uo.pgm")

print "try different parameters (best one is 20)"
def try_segment(h): return segment(im, h)
manipulate(try_segment, 0, 50, im)

seg = segment(im, 20)
seg.writeimage("../images/uo_w.pgm")

# LuM end of file
