#! /usr/bin/python

# UjoImro, 2009
# this software is meant to be free

from pink import vint
from pink import cpp_drawcurve2D

"""
Drawcurve module
"""

def drawcurve( I, coords ):
    """
    This function creates a blank image and draws a non-closed curve specified by the 
    'coords'. The coordiantes can be a list of any object with A[k][0..1] way of access. 
    """

    coordsX=vint( len(coords) )
    coordsY=vint( len(coords) )


    for q in range(len(coords)):
        X = int(coords[q][0])
        Y = int(coords[q][1])
        if 0 > X or X >= I.size[0] or 0 > Y or Y >= I.size[1]:
            raise "error: coordinates out of range"

        coordsX[q]=coords[q][0]
        coordsY[q]=coords[q][1]

    return cpp_drawcurve2D( I.size, coordsX, coordsY )
















# LuM end of file
