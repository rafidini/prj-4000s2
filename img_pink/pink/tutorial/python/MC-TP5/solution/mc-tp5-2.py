# Michel Couprie, 2010
# CeCILL free-software license

# Correction of global bias in image

from pink import imview as imview
from pink import manipulate as manipulate
from pink import cpp as pink
inv=pink.inverse
global DEBUG
DEBUG=1

# bias correction
def correctbias(img, alpha):
    smooth = pink.gaussianfilter(img, alpha/200.)
    mm = pink.minmax(smooth)
    s = smooth - mm[0]
    res1 = img - s
    minmax = pink.minmax(img)
    res = pink.normalize(res1, minmax[0], minmax[1])
    return res

global image
#image = pink.readimage("../images/uo.pgm")
image = pink.readimage("../images/pex2.pgm")

# try gaussian filtering (acts on global 'image' - for use by 'manipulate')
def gaussian(alpha_int):
    alpha = alpha_int / 200.0
    res = pink.gaussianfilter(image, alpha)
    return res

def ss(q): return pink.seuil(image, q)

if debug: 
    manipulate(ss, 0, 255)

#a_int = manipulate(gaussian, 1, 200) # let the user choose parameter value

def cb(q): return correctbias(image, q)

a_int = manipulate(cb, 0, 255)

res = correctbias(image, a_int)

def s2(q): return pink.seuil(res, q)

manipulate(s2, 0, 255)

imview(res)

# LuM end of file
