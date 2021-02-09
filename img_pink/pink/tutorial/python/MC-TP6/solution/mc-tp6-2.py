# Michel Couprie, 2011
# CeCILL free-software license

# convert image to graph structure
# use of external operators acting on files

from pink import imview as imview
from pink import cpp as pink
import os
global DEBUG
DEBUG=1

def run(cmd):
    res = os.system(cmd)
    if res != 0:
        print "error while executing: "+cmd

global im
im = pink.readimage("../images/uo_w.pgm")
imf = pink.frame(im, 255)       # adds a white frame inside the image
imf = pink.frame_around(imf, 0) # adds a black frame outside the image
s = pink.skeleton(imf, 0, 8)    # get rid of simple points
imview(s)

s.writeimage("_s")
run("pink.pgm2skel _s 8 _s.skel")    # conversion into "skelcurv" format
run("pink.skel2graph _s.skel 1 _s.graph") # conversion into "graph" format
run("pink.graph2ps _s.graph 0.3 _s.ps") # postcript file generation
run("pink.gv _s.ps")                 # show ps file

# LuM end of file
