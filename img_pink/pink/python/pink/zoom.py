# -*- coding: utf-8 -*-
#
# This software is licensed under the
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
Zoom (shrink or enlarge) an image. 

Note: this function is too rigid (you can't choose the zoom algorithm) 
and should definitely be replaced later

"""

import os

import sage

    
from pink import cpp_py_zoom


def zoom( image, scale=0.0, new_size=[0]):
    """

    """
    
    if scale==0.0 and new_size==[0]:

        raise "error: The zoom scale or the new size must be specified. See the manual."

    elif scale!=0.0 and new_size!=[0]:

        raise "error: Either define the zoom scale or the dimensions of the new image. Not both."

    elif scale!=0.0: # and new_size==[0]

        try:

            fx=fy=fz=float( scale ) # if scale is a vector, than this will fail

            return cpp_py_zoom( image, fx, fy, fz )            

        except TypeError:

            if len( scale )!=len( image.size ):

                raise "error the dimension of the new image is different from the original (one is 2D the other is 3D)"

            else: # not len(scale)!=image.size.size()

                if len(scale)==2:

                    fx,fy,fz=float( scale[0] ), float( scale[1] ), float( 1.0 )

                else: # not len(scale)==2

                    fx,fy,fz=float( scale[0] ), float( scale[1] ), float( scale[2] )

            return cpp_py_zoom( image, fx, fy, fz )

    else: # scale==0 and new_size!=[0]

        size = image.size

        if len( new_size ) != len( size ):

            raise "error the dimension of the new image is different from the original (one is 2D the other is 3D)"

        elif len( new_size )==2:

            fx,fy,fz=float(new_size[0])/float(size[0]), float(new_size[1])/float(size[1]), float(1.0)

            return cpp_py_zoom( image, fx, fy, fz )

        else: # not len( new_size )==2:

            fx,fy,fz=float(new_size[0])/float(size[0]), float(new_size[1])/float(size[1]), float(new_size[2])/float(size[2])

            return cpp_py_zoom( image, fx, fy, fz )

# LuM end of file
