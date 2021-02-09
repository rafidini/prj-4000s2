# -*- coding: utf-8 -*-
#
# This software is licensed under the
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
Solution of TP5 exercise 3 conserning fourier transformation
"""

from pink import *
import pink.cpp as pink
inv = pink.inverse

debug = False

# calculating the fourier transform of the image
trois = pink.readimage("../images/trois003.pgm")
complex = pink.any2complex(trois, pink.char_image(trois.size))
fft = pink.fft(complex, 0)
cm = pink.complex_modulus(fft)
cm = pink.float2byte(cm, 4)


#generate mask _r
mask = pink.char_image(fft.size)
mask[[1024,1024]]=255
mask = pink.dilatball(mask, 15, 8)

line = pink.drawline(pink.char_image(fft.size), 0, 1024, 2047, 1024)

mask = pink.dilatball(line, 15, 8) - mask

if debug:
    imview(mask)
    raw_input("press enter to continue")

#convert and smooth mask
mask = pink.normalize(inv(mask))
mask_filt = pink.gaussianfilter(pink.convert2float(mask), 0.7)

if debug:
    imview(mask_filt)
    raw_input("press enter to continue")

#apply mask 
#float_mask = pink.normalize(pink.convert2float(mask_filt))

filtered_fft = pink.mult(fft, mask_filt)

if debug:
    imview(pink.float2byte(pink.complex_modulus(filtered_fft)))
    raw_input("press enter to continue")

#reverse fft
filtered_image = pink.fft(filtered_fft, 1)

if debug:
    imview(pink.float2byte(pink.complex_modulus(filtered_image)))

imview([pink.float2byte(pink.complex_modulus(filtered_image)), trois])


























# LuM end of file
