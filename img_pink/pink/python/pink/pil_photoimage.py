# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009-2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
This module contains the conversion functions for 
Python Tkinter visualisation
"""

from pink import python_component_missing

try:
    from PIL import Image, ImageTk
except:
    print("error: could not import PIL.Image or PIL.ImageTk, try to install python-imaging and python-imaging-tk")
    raise python_component_missing


def to_photoimage(image, master=0):
    size  = [ image.size[0], image.size[1] ]
    if master==0:
        return ImageTk.PhotoImage(Image.frombuffer("L", size, image.get_pixels() , "raw", "L", 0, 1))
    else:
        return ImageTk.PhotoImage(Image.frombuffer("L", size, image.get_pixels() , "raw", "L", 0, 1), master=master)


def to_rgb_photoimage(images, master=0):
    size  = [ images[0].size[0], images[0].size[1] ]
    tkimage_r = Image.frombuffer("L", size, images[0].get_pixels() , "raw", "L", 0, 1)
    tkimage_g = Image.frombuffer("L", size, images[1].get_pixels() , "raw", "L", 0, 1)
    tkimage_b = Image.frombuffer("L", size, images[2].get_pixels() , "raw", "L", 0, 1)
    tk_image_rgb = Image.merge( "RGB", [tkimage_r, tkimage_g, tkimage_b ])
    if master == 0:
        result = ImageTk.PhotoImage(tk_image_rgb.copy())
    else:
        result = ImageTk.PhotoImage(tk_image_rgb.copy(), master=master)
    return result
