# -*- coding: utf-8 -*-
#
# This software is licensed under the
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2009-2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

"""
pink's 3d slice visualizer
"""

try:
    import Tkinter as tk
except:
    print("error: python-tk or tkinter is not installed")
    raise python_component_missing

try:
    from pink import to_photoimage, to_rgb_photoimage
except:
    print("error: python-imagin-tk is not installed")
    raise python_component_missing


from pink.cpp import extractplane, seuil, char_image, border, inverse
from pink.cpp import max as pink_max
import pink.windowing

class canvas:
    def __init__(self, parent, side, color_x, color_y):
        self.parent = parent
        self.side = side
        self.pressed = False
        self.inside = False
        self.color_x = color_x
        self.color_y = color_y
        self.pos_x = 0
        self.pos_y = 0
        self.size = [ extractplane(self.parent.image1, 0, self.side).size[0], extractplane(self.parent.image1, 0, self.side).size[1] ]        
        self.canvas = tk.Canvas( self.parent.frame, width=self.size[0], height=self.size[1] )
        self.canvas.bind('<Button-1>', self.on_mouse_down )
        self.canvas.bind('<ButtonRelease-1>', self.on_mouse_up )
        self.canvas.bind('<Motion>', self.on_mouse_move )
        self.canvas.bind('<Enter>', self.on_mouse_enter )
        self.canvas.bind('<Leave>', self.on_mouse_leave )
        

    def on_refresh(self, n, draw_line_x, draw_line_y):
        image_to_show = extractplane( self.parent.image1, int(n), self.side )
        if self.parent.seuiling:
            image_to_seuil = image_to_show
        else:
            image_to_seuil = extractplane( self.parent.image2, int(n), self.side )

        self.tkimage = self.parent.to_photoimage(image_to_show, image_to_seuil)
        self.gui_image = self.canvas.create_image( 1, 1, image=self.tkimage, anchor="nw" )
        if draw_line_x:
            self.line_x = self.canvas.create_line( self.pos_x, 0, self.pos_x, self.size[1]-1, fill=self.color_x )
        if draw_line_y:
            self.line_y = self.canvas.create_line( 0, self.pos_y, self.size[0]-1, self.pos_y, fill=self.color_y )

    def on_mouse_down(self, n):
        self.pressed = True
        self.on_mouse_move(n)        

    def on_mouse_up(self, n):
        self.pressed = False        

    def on_mouse_leave(self,n):
        self.inside = False

    def on_mouse_enter(self,n):
        self.inside = True

    def on_mouse_move(self, n):
        if self.pressed and self.inside:
            self.parent.on_mouse_move(self.side, int(n.x), int(n.y))            

    def put_line(self, x, y):
        self.pos_x=x
        self.pos_y=y        
        

class app:
    def __init__(self, master, image1, image2=0):
        if len(image1.size)!=3:
            raise "error: view3d is for 3D images only."

        self.master=master
        master.title("view3d")

        self.seuilmode=False
        self.surimpmode=False
        self.invertmode=False
        
        self.frame = tk.Frame( self.master, bg="", colormap="new")
        #self.frame.bind("<Button-1>", callback)
        self.frame.grid()
        self.image1 = image1
        if image2!=0:
            self.seuiling = False
            self.image2=image2
        else:
            self.seuiling = True

        ### Buttons
        self.exitbutton = tk.Button(self.frame, text="Exit", command=self.frame.quit, width=10 )
        self.exitbutton.grid(row=6, column=3)

        self.canvas_x = canvas(parent=self, side="zy", color_x="blue", color_y="green" )
        self.canvas_x.canvas.grid(row=0, column=2)

        self.canvas_y = canvas(parent=self, side="xz", color_x="red", color_y="blue" )
        self.canvas_y.canvas.grid(row=1, column=1)

        self.canvas_z = canvas(parent=self, side="xy", color_x="red", color_y="green" )
        self.canvas_z.canvas.grid(row=0, column=1)

        self.scale_x = tk.Scale( self.frame, orient=tk.HORIZONTAL, length=self.image1.size[0], from_=0, to=self.image1.size[0]-1, command=self.update )
        self.scale_x.set( self.image1.size[0]/2 )
        self.scale_x.grid(row=2,column=1)

        self.scale_y = tk.Scale( self.frame, orient=tk.HORIZONTAL, length=self.image1.size[1], from_=0, to=self.image1.size[1]-1, command=self.update )
        self.scale_y.set( self.image1.size[1]/2 )
        self.scale_y.grid(row=3,column=1)

        self.scale_z = tk.Scale( self.frame, orient=tk.HORIZONTAL, length=self.image1.size[2], from_=0, to=self.image1.size[2]-1, command=self.update )
        self.scale_z.set( self.image1.size[2]/2 )
        self.scale_z.grid(row=4,column=1)

        if self.seuiling:
            self.scale_seuil_low = tk.Scale( self.frame, label="Seuil", orient=tk.HORIZONTAL, length=256, from_=0, to=255, showvalue=1, command=self.update )
            self.scale_seuil_low.set(128)
            self.scale_seuil_low.grid(row=5,column=1)

        self.mode = tk.IntVar()
        self.mode.set(0)

        self.invertmode = False
        self.invert = tk.Checkbutton( self.frame, text="Invert", width=10, anchor="nw", command=self.on_invert_click )
        self.invert.grid(row=2, column=3)

        self.photob = tk.Radiobutton( self.frame, text="Image", width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=0 )
        self.photob.grid(row=3,column=3)

        self.surimp = tk.Radiobutton( self.frame, text="Surimp", width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=2 )
        self.surimp.grid(row=4, column=3)

        if self.seuiling:
            seuiltext = "Seuil (Threshold)"
        else:
            seuiltext = "Segmentation"

        self.seuil = tk.Radiobutton( self.frame, text=seuiltext, width=10, anchor="nw", command=self.on_mode_change, variable=self.mode, value=1 )
        self.seuil.grid(row=5,column=3)

        self.draw_line_x = tk.IntVar()
        self.checkbutton_x = tk.Checkbutton( self.frame, anchor='se', text="X", command=self.update, variable=self.draw_line_x )
        self.checkbutton_x.configure(selectcolor="red")
        self.checkbutton_x.grid(row=2, column=0)
        self.checkbutton_x.select()

        self.draw_line_y = tk.IntVar()
        self.checkbutton_y = tk.Checkbutton( self.frame, anchor='se', text="Y", command=self.update, variable=self.draw_line_y )
        self.checkbutton_y.configure(selectcolor="green")
        self.checkbutton_y.grid(row=3, column=0)
        self.checkbutton_y.select()

        self.draw_line_z = tk.IntVar()
        self.checkbutton_z = tk.Checkbutton( self.frame, anchor='se', text="Z", command=self.update, variable=self.draw_line_z )
        self.checkbutton_z.configure(selectcolor="blue")
        self.checkbutton_z.grid(row=4, column=0)
        self.checkbutton_z.select()

        #def __del__(self):
        #self.master.destroy()

    def canvas_x_on_mouse_up(self, n):
        print "release me! n =", n

    def update(self, n=0):        
        line_x = self.draw_line_x.get()==1;
        line_y = self.draw_line_y.get()==1;
        line_z = self.draw_line_z.get()==1;

        self.canvas_x.on_refresh(self.scale_x.get(), line_z, line_y )
        self.canvas_y.on_refresh(self.scale_y.get(), line_x, line_z )
        self.canvas_z.on_refresh(self.scale_z.get(), line_x, line_y )
        
        self.canvas_x.put_line( int(self.scale_z.get()), int(self.scale_y.get()))
        self.canvas_y.put_line( int(self.scale_x.get()), int(self.scale_z.get()))
        self.canvas_z.put_line( int(self.scale_x.get()), int(self.scale_y.get()))

    def on_mouse_move(self, side, a, b):
        if side == "zy":
            self.scale_z.set(a)
            self.scale_y.set(b)
        elif side == "xz":
            self.scale_x.set(a)
            self.scale_z.set(b)
        elif side == "xy":
            self.scale_x.set(a)
            self.scale_y.set(b)
        else:
            raise "internal error: bad canvas mode"

        self.update()

    def on_mode_change(self):
        self.seuilmode = self.surimpmode = False

        if self.mode.get()==1:
            self.seuilmode=True
        elif self.mode.get()==2:
            self.surimpmode=True
            
        self.update()

    def on_invert_click(self):
        self.invertmode=not self.invertmode
        self.update()

    def to_photoimage(self, image_to_show, image_to_seuil):
        if self.seuiling:
            seuilvalue = int(self.scale_seuil_low.get())
        else:
            seuilvalue = 128

        if self.seuilmode:
            image = seuil( image_to_seuil, seuilvalue )
            result = to_photoimage(image)
        elif self.surimpmode:
            bord = border(seuil(image_to_seuil, seuilvalue), 8 )
            image = image_to_show
            if self.invertmode:
                image = inverse(image_to_show)
            #inv_bord = inverse(bord)

            image_r = image
            image_g = pink_max( image, bord )
            image_b = pink_max( image, bord )
            result = to_rgb_photoimage([image_r, image_g, image_b ])
        else:
            image = image_to_show
            if self.invertmode:
                image = inverse(image_to_show)
            result = to_photoimage(image)

        return  result 

    def on_exitbutton_command(self):
        self.frame.quit()

## end of class app

def view3d(image1, image2=0):
    top = tk.Toplevel(pink.windowing.root)
    application = app(top, image1, image2)
    top.protocol('WM_DELETE_WINDOW', application.on_exitbutton_command)
    top.mainloop()
    top.withdraw()



    
















































# LuM end of file
