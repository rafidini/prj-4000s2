# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.

# (C) UjoImro <ujoimro@gmail.com>, 2011 
# (C) M. Couprie <coupriem@esiee.fr>, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX


"""
This module contains a function for finding optimal parameters of 
a segmentation by substituting them interactively using a graphical
scalebar. The idea comes from Mathematica's Manipulate function.

Note: part of this file has been generated with Rapyd-TK
"""

from pink import to_photoimage, python_component_missing, to_rgb_photoimage
import pink.windowing

try:
    from Tkinter import *
except: 
    print("error: could not import Tkinter, try to install python-imaging-tk")
    raise python_component_missing

try:
    import tkMessageBox #MC
except: 
    print("error: could not import tkMessageBox, try to install python-imaging-tk")
    raise python_component_missing

class app(Frame):
    parameter_value = -1
    def __init__(self, Master, function_name, minval, maxval):
        self.function_name = function_name
        self.minval = minval
        self.maxval = maxval
        self.size = function_name(minval).size
        
        apply(Frame.__init__,(self,Master))
        self.__Frame2 = Frame(self)
        self.__Frame2.pack(side='top')
        self.canvas = Canvas(self.__Frame2, width=self.size[0], height=self.size[1])
        self.canvas.pack(expand='yes',fill='both',side='top')
        self.__Frame1 = Frame(self)
        self.__Frame1.pack(side='top')
        self.__Frame4 = Frame(self.__Frame1)
        self.__Frame4.pack(side='left')
        self.valuescale = Scale(self.__Frame4,command=self.on_valuescale_command
            ,orient='horizontal', from_=minval, to=maxval, length=max(100, maxval-minval))
        self.valuescale.pack(side='top')
        self.__Frame3 = Frame(self.__Frame1)
        self.__Frame3.pack(side='left')
        self.exitbutton = Button(self.__Frame3
            ,command=self.on_exitbutton_command,text='Exit')
        self.exitbutton.pack(anchor='w',side='top')

    def on_exitbutton_command(self,Event=None):
        if pink.windowing.options.silent:
            self.quit()
        else:
            if tkMessageBox.askokcancel("Quit", "Do you really wish to quit?"):
                self.quit()
          
    
    def on_valuescale_command(self,Event=None):
        self.parameter_value = self.valuescale.get()
        self.tmpimage = self.function_name(self.parameter_value)
        self.tkimage = to_photoimage(self.tmpimage, master=self.__Frame2 )
        self.gui_image = self.canvas.create_image( 1, 1, image=self.tkimage, anchor="nw" )

class app2(Frame):
    parameter_value = -1
    def __init__(self, Master, function_name, image_in, minval, maxval):
        self.function_name = function_name
        self.minval = minval
        self.maxval = maxval
        self.image_in = image_in / 2
        self.tmpimage = image_in
        self.size = image_in.size
        self.surimp = 0
        
        apply(Frame.__init__,(self,Master))
        self.__Frame2 = Frame(self)
        self.__Frame2.pack(side='top')
        self.canvas = Canvas(self.__Frame2, width=self.size[0], height=self.size[1])
        self.canvas.pack(expand='yes',fill='both',side='top')
        self.__Frame1 = Frame(self)
        self.__Frame1.pack(side='top')
        self.__Frame4 = Frame(self.__Frame1)
        self.__Frame4.pack(side='left')
        self.valuescale = Scale(self.__Frame4,command=self.on_valuescale_command
            ,orient='horizontal', from_=minval, to=maxval)
        self.valuescale.pack(side='top')
        self.seuilcheck = Checkbutton(self.__Frame4
            ,command=self.on_seuilcheck_command,text='Superimpose')
        self.seuilcheck.pack(anchor='w',side='top')
        self.__Frame3 = Frame(self.__Frame1)
        self.__Frame3.pack(side='left')
        self.exitbutton = Button(self.__Frame3
            ,command=self.on_exitbutton_command,text='Exit')
        self.exitbutton.pack(anchor='w',side='top')

    def on_exitbutton_command(self,Event=None):
        self.quit()

    def on_seuilcheck_command(self,Event=None):
        if self.surimp == 0:
            self.surimp = 1
            self.parameter_value = self.valuescale.get()
            self.tmpimage = self.function_name(self.parameter_value)
            self.tmpimage = self.tmpimage / 1 + self.image_in
            self.tkimage = to_rgb_photoimage([self.tmpimage, self.image_in, self.image_in], master=self.__Frame2 )
            #self.tkimage = to_photoimage(self.tmpimage, master=self.__Frame2 )
            self.gui_image = self.canvas.create_image( 1, 1, image=self.tkimage, anchor="nw" )            
        else:
            self.surimp = 0
            self.parameter_value = self.valuescale.get()
            self.tmpimage = self.function_name(self.parameter_value)
            #self.tkimage = to_photoimage(self.tmpimage, master=self.__Frame2 )
            self.tkimage = to_rgb_photoimage([self.tmpimage, self.tmpimage, self.tmpimage], master=self.__Frame2 )
            self.gui_image = self.canvas.create_image( 1, 1, image=self.tkimage, anchor="nw" )
    
    def on_valuescale_command(self,Event=None):
        if self.surimp == 0:
            self.parameter_value = self.valuescale.get()
            self.tmpimage = self.function_name(self.parameter_value)
            #self.tkimage = to_photoimage(self.tmpimage, master=self.__Frame2 )
            self.tkimage = to_rgb_photoimage([self.tmpimage, self.tmpimage, self.tmpimage], master=self.__Frame2 )
            self.gui_image = self.canvas.create_image( 1, 1, image=self.tkimage, anchor="nw" )
        else:
            self.parameter_value = self.valuescale.get()
            self.tmpimage = self.function_name(self.parameter_value)
            self.tmpimage = self.tmpimage / 1 + self.image_in
            #self.tkimage = to_photoimage(self.tmpimage, master=self.__Frame2 )
            self.tkimage = to_rgb_photoimage([self.tmpimage, self.image_in, self.image_in], master=self.__Frame2 )
            self.gui_image = self.canvas.create_image( 1, 1, image=self.tkimage, anchor="nw" )

def manipulate(function_name, minval=0, maxval=100, image_in=None):

    top = Toplevel(pink.windowing.root)

    if image_in is None:
        application = app(top, function_name, minval, maxval)
    else:
        application = app2(top, function_name, image_in, minval, maxval)

    top.protocol('WM_DELETE_WINDOW', application.on_exitbutton_command)

    application.pack(expand='yes',fill='both')
    top.mainloop()
    top.withdraw()

    return application.parameter_value

# LuM end of file
