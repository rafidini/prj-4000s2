# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2010
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
#
# Modified Hugues Talbot, Olivia Miraucourt:
# 3D picker.


from pink import python_component_missing
import pink.windowing
from pink.cpp import mcube
from pink.cpp import readimage

try:
    import vtk
except:
    print("error: could not import vtk, try to install python-vtk")
    raise python_component_missing

try:
    from vtk.tk.vtkTkRenderWindowInteractor import vtkTkRenderWindowInteractor
    from vtk.tk.vtkTkRenderWidget import vtkTkRenderWidget
except:
    print("error: could not import vtk.tk, try to install python-tk or tkinter")
    raise python_component_missing

try:
    import Tkinter as tk
    import tkSimpleDialog
    import tkMessageBox
except:
    print("error: could not import Tkinter, try to install python-tk or tkinter")
    raise python_component_missing


# ## these objects are moved in __init__
# root = tk.Tk()
# root.withdraw()

# a global
picker = vtk.vtkCellPicker()
        

# Create a Python function to create the text for the text mapper used
# to display the results of picking.
def annotatePick(object, event):
    global picker, textActor, textMapper
    if picker.GetCellId() < 0:
        textActor.VisibilityOff()
    else:
        selPt = picker.GetSelectionPoint()
        pickPos = picker.GetPickPosition()
        textMapper.SetInput("(%.6f, %.6f, %.6f)"%pickPos)
        textActor.SetPosition(selPt[:2])
        textActor.VisibilityOn()

def feedbackPick(object, event):
    global picker
    selPt = picker.GetSelectionPoint()
    pickPos = picker.GetPickPosition()
    print("pick position= (x=%.4f y=%.4f z=%.4f)" % pickPos)

class render_class:
    def __init__(self, mesh1, mesh2=0):
        self.name = "render"
        self.rs = 600
        self.cs = 600
        self.mesh1 = mesh1
        self.mesh2 = mesh2
        self.ren1 = vtk.vtkRenderer()
        self.ren1.SetViewport([0, 0, 1, 1])
        self.ren1.SetBackground([0.93, 0.93, 0.97])
        self.ren_win = vtk.vtkRenderWindow()
        self.ren_win.AddRenderer(self.ren1)

        

        # load geometry
        ## first object
        self.obj1 = obj(mesh1)        
        self.ren1.AddViewProp(self.obj1.objActor)

        ## optional second object
        if mesh2!=0:
            self.obj2 = obj(mesh2)
            self.obj2.objActor.GetProperty().SetColor([1.0, 0.49, 0.25])
            self.ren1.AddViewProp(self.obj2.objActor)

        self.ren1.ResetCamera()


class obj:
    def __init__(self, mesh):
        self.obj = vtk.vtkPolyDataReader()
        self.obj.SetInputString(mesh)
        self.obj.ReadFromInputStringOn()
        self.normals = vtk.vtkPolyDataNormals()
        self.normals.SetInput(self.obj.GetOutput())
        self.normals.FlipNormalsOn()
        self.objMapper = vtk.vtkPolyDataMapper()
        self.objMapper.SetInput(self.normals.GetOutput())
        self.objActor = vtk.vtkActor()
        self.objActor.SetMapper(self.objMapper)

    
class app:
    def __init__(self, render, root):
        global picker
        self.render = render # the vtk renderer object

        # # then binds rendering window to a tk widget
        self.top = tk.Toplevel(root)
        self.top.title("Multiple Renderer")
        self.vtkw = vtkTkRenderWidget(self.top, width=600, height=600, rw=self.render.ren_win)
        #self.vtkw = vtkTkRenderWindowInteractor(self.top,width=600, height=600, rw=self.render.ren_win)
        self.vtkw.BindTkRenderWidget()
        self.top.protocol('WM_DELETE_WINDOW', self.on_exitbutton_command)

        # HT not sure
        # picker
        picker.AddObserver("EndPickEvent", feedbackPick)
        self.vtkw.SetPicker(picker)
        
        #self.iren=vtkTkRenderWindowInteractor()
        #self.iren.SetRenderWindow(self.ren_win)
        #self.iren.SetPicker(self.picker)

        self.commands = tk.Frame(self.top)
        self.buttons = tk.Frame(self.top)

        self.buttons.pack( fill="both", expand=1, side="top" )
        self.commands.pack( fill="both", expand=1, side="bottom" )
        self.exitbutton = tk.Button(self.buttons, text="Exit", activebackground="white", command=self.on_exitbutton_command)
        self.savebutton = tk.Button(self.buttons, text="Save", activebackground="white", command=self.savepng)        
        self.wirebutton1 = wirebutton(frame=self.buttons, text="Wireframe1", ren_win=self.render.ren_win, obj_actor=self.render.obj1.objActor)

        if render.mesh2!=0:
            self.wirebutton2 = wirebutton(frame=self.buttons, text="Wireframe2", ren_win=self.render.ren_win, obj_actor=self.render.obj2.objActor)

        self.opacity = tk.Scale(self.commands, from_=0.0, to=1.0, res=0.01, orient="horizontal", label="Opacity", command=self.set_opacity)
                
        self.exitbutton.pack(side="right")
        self.savebutton.pack(side="left")
        self.vtkw.pack(side="top", fill="both", expand="yes")
        self.opacity.pack(side="top", anchor="nw", fill="both")
        

    def set_opacity(self, f):
        self.render.obj1.objActor.GetProperty().SetOpacity(float(f))
        self.render.ren_win.Render()

    def mainloop(self):
        self.top.mainloop()
    
    def on_exitbutton_command(self):
        #vtkCommand DeleteAllObjects
        #print("properly exiting")
        if pink.windowing.options.silent:
            self.top.quit()
        else:
            if tkMessageBox.askokcancel("Quit", "Do you really wish to quit?"):
                self.top.quit()

    def savepng(self):
        
        w2i=vtk.vtkWindowToImageFilter()
        writer = vtk.vtkPNGWriter()
        w2i.SetInput(self.render.ren_win)
        writer.SetInput(w2i.GetOutput())
        writer.SetFileName(str(tkSimpleDialog.askstring( "Save a Snapshot", "Specify the filename to save a snapshot:", initialvalue="_sav01.png", parent=self.top)))
        writer.Write()
        



class wirebutton:
    def __init__(self, frame, text, ren_win, obj_actor):
        self.ren_win = ren_win
        self.obj_actor = obj_actor
        self.wireframe = tk.IntVar()
        self.wireframe.set(0)
        self.wiremode = False
        self.button = tk.Checkbutton(frame, text=text, variable=self.wireframe, activebackground="white", command=self.togglewire)        
        self.button.pack(side="left")

    def togglewire(self):
        #if int(self.wireframe.get())==1:
        if not self.wiremode:
            self.obj_actor.GetProperty().SetRepresentationToWireframe()
        else:
            self.obj_actor.GetProperty().SetRepresentationToSurface()
            
        self.wiremode = not self.wiremode
        self.ren_win.Render()



# def renderfiles( filename1, filename2="" ):
#     render_obj = render_class( filename1, filename2 )
#     #renderer( "/home/ujoimro/doc/projects/Tutorial-Pink/MC-TP3/solution/TP3cor/lettre_a.vtk",
#     #                     "/home/ujoimro/doc/projects/Tutorial-Pink/MC-TP3/solution/TP3cor/carotide_seg.vtk"
#     #                     )
#     application = app(render_obj)
#     application.mainloop()

def render( image1=0, image2=0 ):

    ## to be removed later
    if (image1==0):
        image1 = readimage("/Users/talbot/research/Olivia/images/seg_art_008.pgm")
    
    mesh1 = mcube(image1, 0, 0, 0)
    if image2!=0:
        mesh2 = mcube(image2, 0, 0, 0)
    else:
        mesh2 = 0

    top = tk.Toplevel(pink.windowing.root)

    render_obj=render_class(mesh1, mesh2)
    application = app(render_obj, pink.windowing.root)
    top.withdraw()


    application.mainloop()
    application.top.withdraw()
    #mesh2 = 
 
render()





# LuM end of file
