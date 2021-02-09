# UjoImro, 2009
# This software is meant to be free
"""

This is the module that implements the 'show_image' function for the 'pink' images.
These are 'char_image', 'short_image', 'int_image', 'float_image', 'double_image' or 
other image types that have since been implemented.

Note:

It needs 'os' module to function. It should be loaded in '__init__.py'

"""

import os
import sage
from pink import zoom, int_image, char_image, long2byte

def show_char_image( I, scale=0.0, new_size=[0] ): 
    """
    helper function for show_image
    """
    if scale==0.0 and new_size==[0]:
        
        # Generating the filename for the pgm and the png image
        png_file = sage.misc.misc.graphics_filename()
        pgm_file = png_file[ 0:png_file.rfind(".") ] + ".pgm"
        
        # saving the file to 'pgm'. Later it should directly be savable to 'png'
        I.writeimage( pgm_file )
        
        # this is imagemagick's convert function. It will not be necessary when 'char_image' will 
        # directly export 'png'-s
        os.system( "convert " + pgm_file + " " + png_file )
        
        # exporting the html code:
        sage.misc.html.html( "<img src='cell://" + png_file  + "'/>" )

    else: # not zoom==1.0 and new_size==[0]
            
        # Generating the filename for the pgm and the png image
        png_file = sage.misc.misc.graphics_filename()
        pgm_file = png_file[ 0:png_file.rfind(".") ] + ".pgm"
        
        # zooming
        Z = zoom( I, scale=scale, new_size=new_size )

        # saving the file to 'pgm'. Later it should directly be savable to 'png'
        Z.writeimage( pgm_file )

        # this is imagemagick's convert function. It will not be necessary when 'char_image' will 
        # directly export 'png'-s
        os.system( "convert " + pgm_file + " " + png_file )

        # exporting the html code:
        sage.misc.html.html( "<img src='cell://" + png_file  + "'/>" )


def show_image( I, scale=0.0, new_size=[0] ):
    """

    """

    if type(I)==char_image:
        show_char_image( I, scale=scale, new_size=new_size )

    elif type(I)==int_image:
        show_char_image( long2byte(I), scale=scale, new_size=new_size )
        
    else:
        raise "error: Only char_image and int_image is supported"

        




























































# LuM end of file
