# Michel Couprie, 2010
# CeCILL free-software license

# Segmentation and analysis of corroded joints

from pink import imview, manipulate
from pink import cpp as pink
inv=pink.inverse

global debug
debug=1

# segmentation
def segment(image, height):
    # gradient
    ball = pink.genball(2)
    grad = pink.gradmorph(image,ball)
    if debug:
        grad.writeimage("_grad")
    # obtain markers
    mark1 = pink.segmentheight(inv(grad), 4, height, 0)
    if debug:
        mark1.writeimage("_mark1")
    # watershed
    wshed1 = pink.watershed(inv(grad), mark1, 4)
    if debug:
        wshed1.writeimage("_wshed1")
    # suppress tiny components
    mark2 = pink.segmentarea(wshed1, 4, 100, 0)
    if debug:
        mark2.writeimage("_mark2")
    wshed2 = pink.watershed(inv(grad), mark2, 4)
    if debug:
        wshed2.writeimage("_wshed2")
    return(inv(wshed2))

# detect joints
def analyzejoints(image, seg):
    # extracts thin pores
    ball = pink.genball(3)

    # The following line is the definition of closing, so it can be
    # replaced by a single operator
    #tophat = pink.erosion(pink.dilation(image, ball), ball) - image
    tophat = pink.closing(image, ball) - image

    if debug:
        tophat.writeimage("_tophat")
    thinpores = pink.seuil(tophat, 17)
    if debug:
        thinpores.writeimage("_thinpores")
    # extracts thin pores
    black = inv(pink.seuil(image, 150))
    thickpores = pink.segmentarea(black, 4, 100, 0)
    thickpores = pink.dilation(thickpores, pink.genball(1))
    if debug:
        thickpores.writeimage("_thickpores")
    # extracts joints
    joint = seg - thickpores
    if debug:
        joint.writeimage("_joint")
    jointlen = pink.area(joint)
    print("longueur totale des joints : "+str(jointlen))
    # extracts coroded parts of joints
    corojoint = pink.min(pink.dilation(thinpores, pink.genball(1)), joint)
    if debug:
        corojoint.writeimage("_corojoint")
        pink.surimp(joint, corojoint, "_res")
    corojointlen = pink.area(corojoint)
    print("longueur des joints corrodes : "+str(corojointlen))

    return(corojoint)

joints = pink.readimage("../images/joints.pgm")

def segj(q): return segment(joints, q)

if debug:
    height = manipulate(segj, 0, 15, joints)
else:
    height = 7

seg = segment(joints, height)
imview(seg)

def analj(q): return analyzejoints(joints, segment(joints,q) )

if debug:
    d=manipulate(analj, 0, 100, joints)
else:
    analyzejoints(joints, seg)

pink.surimp(joints, analj(d), "joints_seg.ppm")













# LuM end of file
