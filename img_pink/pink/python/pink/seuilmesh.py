# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2010
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

try:
    import vtk
except:
    print("error: python-vtk is not installed")
    raise python_component_missing

try:
    from vtk.tk.vtkTkRenderWindowInteractor import vtkTkRenderWindowInteractor
    from vtk.tk.vtkTkRenderWidget import vtkTkRenderWidget
except:
    print("error: python-vtk is not installed")
    raise python_component_missing
    

try:
    import Tkinter as tk
    import tkSimpleDialog
except:
    print("error: python-tk or tkinter is not installed")
    raise python_component_missing

from pink.cpp import mcube

class slide_render_class:
    def __init__(self, image):
        self.name = "slide_render"
        self.rs = 600
        self.cs = 600
        self.image = image
        self.mesh = mcube(image, 128, 0, 0)

        self.ren = vtk.vtkRenderer()
        self.ren.SetViewport([0, 0, 1, 1])
        self.ren.SetBackground([0.93, 0.93, 0.97])
        self.ren_win = vtk.vtkRenderWindow()
        self.ren_win.AddRenderer(self.ren)
        
        # load geometry
        ## the object
        self.obj = obj(self.mesh)
        self.obj.objActor.GetProperty().SetColor([1.0, 0.49, 0.25])
        self.ren.AddViewProp(self.obj.objActor)

        self.ren.ResetCamera()


class obj:
    def __init__(self, mesh):
        self.mesh = mesh
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
    def __init__(self, render):
        self.render = render # the vtk renderer object

        # # then binds rendering window to a tk widget
        self.root = tk.Tk()
        self.root.title("Multiple Renderer")
        self.vtkw = vtkTkRenderWidget(self.root, width=600, height=600, rw=self.render.ren_win)
        self.vtkw.BindTkRenderWidget()
        
        self.commands = tk.Frame(self.root)
        self.buttons = tk.Frame(self.root)

        self.buttons.pack( fill="both", expand=1, side="top" )
        self.commands.pack( fill="both", expand=1, side="bottom" )
        self.quitbutton = tk.Button(self.buttons, text="Exit", activebackground="white", command=self.bye)
        self.savebutton = tk.Button(self.buttons, text="Save", activebackground="white", command=self.savepng)        
        self.wirebutton = wirebutton(frame=self.buttons, text="Wireframe", ren_win=self.render.ren_win, obj_actor=self.render.obj.objActor)

        self.opacity = tk.Scale(self.commands, from_=0.0, to=1.0, res=0.01, orient="horizontal", label="Opacity", command=self.set_opacity)
        self.seuil   = tk.Scale(self.commands, from_=0.0, to=255.0, res=1, orient="horizontal", label="Seuil")
        self.update = tk.Button(self.commands, text="Apply", command=self.set_seuil)
                
        self.quitbutton.pack(side="right")
        self.savebutton.pack(side="left")        
        self.vtkw.pack(side="top", fill="both", expand="yes")
        self.opacity.pack(side="top", anchor="nw", fill="both")
        self.seuil.pack(side="top", anchor="nw", fill="both")
        self.update.pack(side="right", anchor="nw")

    def set_seuil(self):
        f = self.seuil.get()
        #self.render.ren.RemoveViewProp(self.render.obj.objActor)
        #self.render.obj = obj( mcube(self.render.image, int(f), 0, 0))
        self.mesh = mcube(self.render.image, int(f), 0, 0)
        self.render.obj.obj.SetInputString( self.mesh )
        #self.render.ren.AddViewProp(self.render.obj.objActor)
        self.render.ren_win.Render()
        #print "seuil re-render"

    def set_opacity(self, f):
        self.render.obj.objActor.GetProperty().SetOpacity(float(f))
        self.render.ren_win.Render()

    def mainloop(self):
        self.root.mainloop()
    
    def bye(self):
    #vtkCommand DeleteAllObjects
        self.root.quit()


    def savepng(self):
        
        w2i=vtk.vtkWindowToImageFilter()
        writer = vtk.vtkPNGWriter()
        w2i.SetInput(self.render.ren_win)
        writer.SetInput(w2i.GetOutput())
        writer.SetFileName(str(tkSimpleDialog.askstring( "Save a Snapshot", "Specify the filename to save a snapshot:", initialvalue="_sav01.png", parent=self.root)))
        writer.Write()
        



class wirebutton:
    def __init__(self, frame, text, ren_win, obj_actor):
        self.ren_win = ren_win
        self.obj_actor = obj_actor
        self.wireframe = tk.IntVar()
        self.wireframe.set(0)
        self.button = tk.Checkbutton(frame, text=text, variable=self.wireframe, activebackground="white", command=self.togglewire)
        self.button.pack(side="left")

    def togglewire(self):
        if int(self.wireframe.get())==1:
            self.obj_actor.GetProperty().SetRepresentationToWireframe()
        else:
            self.obj_actor.GetProperty().SetRepresentationToSurface()
            
        self.ren_win.Render()







def seuilmesh(image):
    render_obj=slide_render_class(image)
    application = app(render_obj)
    top.protocol('WM_DELETE_WINDOW', application.on_exitbutton_command)        
    application.mainloop()












































# LuM end of file
