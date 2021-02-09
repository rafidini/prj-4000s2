# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.

# (C) H. Talbot <talboth@esiee.fr>, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

# histograms 

import sys
from pink import cpp as pink

try:
    import numpy as np
except:
    print("error: could not import numpy, try to install python-numpy")
    raise python_component_missing

pink2npdtype  = {
		'uint8_t':np.dtype('u1'),  # 0 to 255
                'uint16_t':np.dtype('u2'),
                'int16_t':np.dtype('i2'),
                'uint32_t':np.dtype('u4'),
                'int32_t':np.dtype('i4'),
		'float'  :np.dtype('f4'), 
                'double' :np.dtype('f8')
	}

np2pinkdtype  = dict((v,k) for k, v in pink2npdtype.iteritems())

def pink2numpy(image):
    """
    Makes a numpy array from an image. Shape is retained!
    """
    nparray=np.frombuffer(image.get_pixels()[:],pink2npdtype[image.imtype()])
    if (len(image.size) == 3):
        print("Reshaping to %d x %d x %d" % (image.size[2],image.size[1],image.size[0]))
        return(nparray.reshape(image.size[2],image.size[1],image.size[0]))
    elif (len(image.size) == 2):
        print("Reshaping to %d x %d" % (image.size[1],image.size[0]))
        return(nparray.reshape(image.size[1],image.size[0]))
    else:
        sys.stderr.write("Number of dimensions not supported, data was not reshaped\n")
    return nparray

def numpy2pink(array):
    """
    Makes an image from a numpy array
    """
    ## Not elegant
    if (np.size(array.shape) == 2):
        Image= {
        'uint8_t':pink.char_image([array.shape[1],array.shape[0]]), 
        'uint16_t':pink.short_image([array.shape[1],array.shape[0]]), 
        'int32_t':pink.int_image([array.shape[1],array.shape[0]]), 
        'float'  :pink.float_image([array.shape[1],array.shape[0]]), 
        'double' :pink.double_image([array.shape[1],array.shape[0]]), 
        }
    elif (np.size(array.shape) == 3):
        Image= {
        'uint8_t':pink.char_image([array.shape[2],array.shape[1],array.shape[0]]), 
        'uint16_t':pink.short_image([array.shape[2],array.shape[1],array.shape[0]]), 
        'int32_t':pink.int_image([array.shape[2],array.shape[1],array.shape[0]]), 
        'float'  :pink.float_image([array.shape[2],array.shape[1],array.shape[0]]), 
        'double' :pink.double_image([array.shape[2],array.shape[1],array.shape[0]]), 
        }
    else:
        sys.stder.write("Number of dimensions not supported\n")
        return None

    myimg=Image.get(np2pinkdtype[array.dtype])
    # copy data
    myimg.set_pixels(array.ctypes.data,array.nbytes)
    return myimg 
    
# LuM end of file
