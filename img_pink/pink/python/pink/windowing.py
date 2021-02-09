# -*- coding: utf-8 -*-
#
# This software is licensed under 
# CeCILL FREE SOFTWARE LICENSE AGREEMENT

# This software comes in hope that it will be useful but 
# without any warranty to the extent permitted by applicable law.
  
# (C) UjoImro <ujoimro@gmail.com>, 2011
# Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX

#### NOTE: windowing has to be handled globally
#### this is apparently a deficit of Tkinter
#### code concerning Tkinter dialogs

class options:
    silent=True
    debug=False

class windowing:
    def __init__(self):
        self.options=options()
        try:
            from Tkinter import Tk, Toplevel, wantobjects
            wantobjects = False

            self.root=Tk()
            self.root.withdraw()
            # self.top=Toplevel(self.root)
            # self.top.withdraw()
            # self.top.protocol('WM_DELETE_WINDOW', self.top.quit)
        except:
            print("Warning: could not initialize windowing, try to install python-tk or tkinter")
            pass
        
    root=False
    top=False
    options=False


windowing = windowing()








# LuM end of file
