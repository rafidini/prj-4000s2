# UjoImro, 2012
# ProCarPlan s.r.o.
# CeCILL free-software license

# convert image to graph structure

from pink import imview as imview
from pink import cpp as pink

global DEBUG
DEBUG=1

im = pink.readimage("../images/uo_w.pgm")
imf = pink.frame(im, 255)       # adds a white frame inside the image
imf = pink.frame_around(imf, 0) # adds a black frame outside the image
skelim = pink.skeleton(imf, 0, 8)    # get rid of simple points
imview(skelim)

skel = pink.image2skel( skelim, 8 )
skel.writeskel("_skelpy.skel")

graph = pink.skel2graph( skel, 1 )
graph.writegraph( "_graphpy.graph" )
graph.writeps("_graphpy.ps", 0.3 )
















# LuM end of file
