#! /usr/bin/python

# UjoImro, 2009
# This software is meant to be free.

import sys
from PyQt4 import QtCore
from PyQt4 import QtGui 
from pink.MICCAI import iSize
import ctypes
import sip


class click(QtGui.QGraphicsView):
    point=[]
    size=[]
    
    def set_size(self, size):
        self.size=[]
        self.size.append(size[0])
        self.size.append(size[1])
    
    def get_point(self, app):
        self.show()
        app.exec_()
        return self.point

    def mousePressEvent(self, event):
        #print event.pos().x(), event.pos().y()
        self.point=[ event.pos().x(), event.pos().y() ]
        self.close()

    def showEvent(self, event):
        self.setGeometry(800, 800, self.size[0], self.size[1])         
        self.setMouseTracking(True)


class _scene(QtGui.QGraphicsScene):
    def mousePressEvent(self, event):
            print "SCENE=", event.pos().x(), event.pos().y()


class ctDisplay(ctypes.Structure):
    pass


def init_display():
    xlib=ctypes.CDLL("libX11.so.6")
    xlib.XOpenDisplay.restype = ctypes.POINTER(ctDisplay)
    xlib.XOpenDisplay.argtypes = [ctypes.c_char_p]
    ct_display=xlib.XOpenDisplay(":0.0")
    display_addr = ctypes.cast( ct_display, ctypes.c_void_p ).value
    pyqt_display=sip.wrapinstance( display_addr,QtGui.Display  )
    return pyqt_display


def shift(val, max):
    if val > max:
        return shift( val - max, max )
    else: 
        return val


def get_a_point( input_image ):    

    def grey_color( color ):
        return QtGui.qRgb( color, color, color )

    app = QtGui.QApplication( init_display(), [])

    grview = click()
    grview.set_size([ input_image.size[1], input_image.size[0] ])
    scene = _scene()
    image = QtGui.QImage( input_image.size[1], input_image.size[0], QtGui.QImage.Format_RGB32 )
    
    translation=[ int( input_image.size[0]/2 ), int( input_image.size[1]/2 ) ]
    
    for q in iSize([ input_image.size[0], input_image.size[1] ]):
        image.setPixel( q[1], q[0], grey_color( 
            input_image[[ 
                shift( val = q[0] + translation[0], max = input_image.size[0] ), 
                shift( val = q[1] + translation[1], max = input_image.size[1] )
              ]] 
          ) 
      )

    scene.setForegroundBrush( QtGui.QBrush(image) )
    grview.setScene( scene )

    result = grview.get_point( app )
    #del app

    return [ result[1], result[0] ]





# LuM end of file
