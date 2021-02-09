# Michel Couprie, 2011
# CeCILL free-software license

# crop image (test)

from pink import cpp as pink
from pink import imview as imview

def crop(image, x, y, w, h):
    res = pink.char_image([w,h])
    res = pink.insert_image(res, image, [-x,-y])
    return res

rs = 100 # row size for a tile
cs = 100 # column size for a tile

big = pink.readimage("wholeimage.pgm")
t0 = crop(big, 0, 0, rs, cs)
t1 = crop(big, rs, 0, rs, cs)
t2 = crop(big, 0, cs, rs, cs)
t3 = crop(big, rs, cs, rs, cs)


imview([big, t0, t1, t2, t3])

