# Michel Couprie, 2011
# CeCILL free-software license

# Correction of a radial luminance bias

from pink import imview, manipulate
from pink import cpp as pink
global debug
debug=True

# identification of the bias parameter (assuming it is linear)
def find_bias(image, xc, yc):
    rs = image.size[0]
    cs = image.size[1]
    circle = pink.char_image(image.size)
    rad = 20 # below, circles are too small
    Y = []
    while True:
        if debug:
            print rad
        if (xc + rad >= rs) or (xc - rad < 0):
            break
        if (yc + rad >= cs) or (yc - rad < 0):
            break
        circle = pink.drawball(circle, rad, xc, yc, 0)
        circle = pink.border(circle, 8)
        av = pink.average(image, circle)
        Y = Y + [av]
        rad = rad + 1
        
    X = range(len(Y))
    res = pink.identifyline(X, Y)
    # output : coefficient a of the equation of the line y = ax+b
    # matching the data 
    return(res[0])

# correction of the bias
def correct_bias(image, xc, yc, alpha):
    result = pink.char_image([0,0])
    result.copy(image)
    rs = result.size[0]
    cs = result.size[1]
    for y in range(cs):
        for x in range(rs):
            R = pow((xc-x)*(xc-x) + (yc-y)*(yc-y), 0.5)
            T = (1.0 * result[[x,y]]) - (alpha * R)
            if T > 255:
                T = 255
            if T < 0:
                T = 0
            result[[x,y]] = int(round(T))
    return(result)

img = pink.readimage("../images/mortier_2d.pgm")
xc = 144 # the center is supposed to be known -
yc = 138 # otherwise we have to analyse the 3D image to "guess" it
alpha = find_bias(img, xc, yc) 
res = correct_bias(img, xc, yc, alpha)

def sm(q): return pink.seuil(img,q)
def rm(q): return pink.seuil(res,q)

if debug:
    imview([img, res])
    manipulate(sm, 0, 255)
    manipulate(rm, 0, 255)

imview(res)

# LuM end of file
