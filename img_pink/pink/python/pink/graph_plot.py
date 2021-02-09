 #! /usr/bin/python

# UjoImro, 2009
# this software is meant to be free

from sys import path
path.append("/home/ujoimro/doc/projects/Pink/python/")
path.append("/home/ujoimro/doc/projects/Pink/boost/bin.v2/libs/python/build/gcc-4.3/release/threading-multi/")

from matplotlib import use
use("pdf")
import matplotlib.pyplot as plt
from pink import iSize

"""
graph export module, should be written later to show the graph 
directly in the notebook
"""

def export_graph(
    filename, 
    potencial, 
    flow, 
    srcsink,
    potencial_text_size=7,
    flow_text_size=10 
    ):
    
    def draw_arrow(x0,y0,x1,y1):
        ax.annotate("",(x0,y0),(x1,y1), color="blue",
            arrowprops=dict(  
                arrowstyle="fancy,head_width=.3,tail_width=0.1",
                shrinkA=8,
                shrinkB=10,
                fc="w", ec="k",
                connectionstyle="arc3,rad=-0.05",
                fill=True,     
                edgecolor="y",                                 
                linewidth=0.1
                           ),)
    
    def potencial_text(x,y,text):
        ax.text(x+1-0.2, y+1-0.07, text, size=potencial_text_size)
    
    def flow_x(x,y,text):
        ax.text(x+1+0.3, y+1+0.09, text, size=flow_text_size)
        
    def flow_y(x,y,text):
        ax.text(x+1-0.5, y+2-0.6, text, size=flow_text_size)
    
    fig = plt.figure()
    
    ax = fig.add_subplot(111, aspect="equal", frameon=False)
    
    ax.plot([0], [0], ",")
    
    ax.plot([potencial.size[0]+1], [potencial.size[1]+1], ",")
    
    # drawing the nodes
    for q in iSize(potencial.size):
        if srcsink[q]==255:
            ax.add_artist(plt.Circle((q[0]+1,q[1]+1),.25,linewidth=0.1,facecolor=(0.188,0.541,0.690)))
        elif srcsink[q]==1:
            ax.add_artist(plt.Circle((q[0]+1,q[1]+1),.25,linewidth=0.1,facecolor=(0.957,0.408,0.349)))
        else:
            ax.add_artist(plt.Circle((q[0]+1,q[1]+1),.25,linewidth=0.1,facecolor=(0.976,0.975,0.773)))
            
    # drawing the arrows
    for q in iSize(potencial.size):
        draw_arrow(q[0]+2,q[1]+1,    q[0]+1,q[1]+1)
        draw_arrow(q[0]+1,q[1]+2,    q[0]+1,q[1]+1)                              

    for q in iSize(potencial.size):
        potencial_text(q[0],q[1],str(round(potencial[q],3)))    
    
    for q in iSize(potencial.size):
        flow_x(q[0],q[1],str(round(flow[[q[0],q[1],0]],3)))
        flow_y(q[0],q[1],str(round(flow[[q[0],q[1],1]],3)))

    
    # writing the image     
    plt.savefig(filename)
    plt.close()
















# LuM end of file
