# # UjoImro, 2011
# # CeCILL free-software license

""" This function is not part of standart Pink You must compile the
    development version. Decommend the code for testing.
"""

from pink import *
import pink.cpp as pink


def filter_image(image, iterations, tau, theta):
    blank = pink.char_image(image.size)
    lambda_image = pink.float_image(image.size)
    lambda_image.fill(1.) # we set up the lambda image to 1
    seuil = pink.seuil(image, 200)

    fseuil = pink.normalize(pink.convert2float(seuil))
    fimage = pink.normalize(pink.convert2float(image))
    
    gg = pink.measure(fimage)

    return  pink.ungerflow(blank, gg, lambda_image, fimage, iterations, tau, theta)




dora = pink.readimage("dora.pgm")
filtdora = filter_image( pink.medianfilter(dora, pink.genball(2)), 5000, 0.132, 0.0006)

#flamingos = pink.readimage("flamingos.pgm")
#filtflag = filter_image(flamingos, 2000, 0.132, 0.0006)


#imview([flamingos, filtflag])
imview([dora, filtdora])
dora.writeimage("dora.pgm")
pink.float2byte(filtdora).writeimage("filtdora.pgm")

# LuM end of file
