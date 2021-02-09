# Copyright ESIEE (2009) 
#
# m.couprie@esiee.fr
#
# This software is an image processing library whose purpose is to be
# used primarily for research and teaching.
#
# This software is governed by the CeCILL  license under French law and
# abiding by the rules of distribution of free software. You can  use, 
# modify and/ or redistribute the software under the terms of the CeCILL
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info". 
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability. 
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or 
# data to be ensured and,  more generally, to use and operate it in the 
# same conditions as regards security. 
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL license and that you accept its terms.
#
             ##################################
             ##################################
             ##           voxedit.tcl        ##
             ##   Cédric Allène - Juin 2003  ##
             ##################################
             ##################################


                 #########################
                ##  PROGRAMME PRINCIPAL  ##
############################################################
############################################################

# The next line is executed by /bin/sh, but not Tcl \
exec vtk $0 $1


# First we include the VTK Tcl packages which will make available
# all of the vtk commands to Tcl
package require vtk
package require vtkinteraction

# Variables globales
global renWin

global IM3D
global long_max
global nb_comp
global mode

global file_dist_pgm
global file_out_pgm
global file_pgm
global dist_pgm
global mask_pgm
global prov_file_pgm
global prov_dist_pgm
global file_vtk


set IM3D(cx)	0
set IM3D(cy)	0
set IM3D(cz)	0
set IM3D(cv)	0
set mode	"Global view"

#Input file
if {$argc != 1} {
  puts stderr "usage: render in"
  exit 
} else {
  set file_in_pgm [lindex $argv 0]
}

proc tmpfile {tmpname} {
  return [file join "/tmp" "voxedit_$tmpname"]
}

#Work files
set file_out_pgm    [tmpfile voxedit_seg.pgm]
set file_dist_pgm   [tmpfile voxedit_dist.pgm]
set prov_file_pgm   [tmpfile prov_file.pgm]
set prov_dist_pgm   [tmpfile prov_dist.pgm]
set mask_pgm        [tmpfile mask.pgm]
set file_vtk        [tmpfile voxedit.vtk]

set file_pgm $file_out_pgm
set dist_pgm $file_dist_pgm

puts ""
puts "Fichier entrée pgm      : $file_in_pgm"
puts "Fichier sortie pgm      : $file_out_pgm"
puts "Fichier distance racine : $file_dist_pgm"
puts "Fichier transitoire vtk : $file_vtk"
puts ""

# Create the GUI:
wm withdraw .
toplevel .top
wm minsize .top 410 520
wm title .top {Skeleton deleter}
frame .top.f1
frame .top.f2
frame .top.f3
frame .top.f4
vtkTkRenderWidget .top.f1.r1 -width 400 -height 400
button .top.f2.btnDelComp -text Delete\ncomponent -command deleteComp -activebackground white -width 10
button .top.f2.btnDelVox -text Delete\nvoxel -command deleteVoxel -activebackground white -width 10
button .top.f3.btnCycle -text "Test cycle" -command testCycle -activebackground white -width 10
button .top.f3.btnView -text "Global view" -command afficherVoxedit -activebackground white -width 10
button .top.f3.btnUndo -text Undo -command undo -activebackground white -width 5
button .top.f4.btnQuit -text Quit -command closeProg -activebackground white -width 5
pack .top.f1.r1 -side left -padx 3 -pady 3 -fill both -expand t
pack .top.f1 -fill both -expand 1
pack .top.f2 -fill x -expand 0
pack .top.f3 -fill x -expand 0
pack .top.f4 -fill x -expand 0
pack .top.f2.btnDelComp -side left
pack .top.f2.btnDelVox -side left
pack .top.f3.btnCycle -side left
pack .top.f3.btnView -side left
pack .top.f3.btnUndo -side left
pack .top.f4.btnQuit -side right


# create the frame to display coordinates
frame .top.f2.rightcol
pack .top.f2.rightcol -side right

# create the label for pixel value
label .top.f2.rightcol.pixval
label .top.f2.rightcol.pixval.xlab -textvariable IM3D(cx) -width 5
label .top.f2.rightcol.pixval.ylab -textvariable IM3D(cy) -width 5
label .top.f2.rightcol.pixval.zlab -textvariable IM3D(cz) -width 5
label .top.f2.rightcol.pixval.vlab -textvariable IM3D(cv) -width 5
label .top.f2.rightcol.pixval.text -text " : "
pack .top.f2.rightcol.pixval.xlab -side left
pack .top.f2.rightcol.pixval.ylab -side left
pack .top.f2.rightcol.pixval.zlab -side left
pack .top.f2.rightcol.pixval.text -side left
pack .top.f2.rightcol.pixval.vlab -side left
pack .top.f2.rightcol.pixval -side top


# create the frame to display number of connexe components
frame .top.f3.rightcol
pack .top.f3.rightcol -side right

# create the label for nbcomp value
label .top.f3.rightcol.nbcomp
label .top.f3.rightcol.nbcomp.text -text "Nb comp : "
label .top.f3.rightcol.nbcomp.nb -textvariable nb_comp -width 5
pack .top.f3.rightcol.nbcomp -side top
pack .top.f3.rightcol.nbcomp.text -side left
pack .top.f3.rightcol.nbcomp.nb -side left


# create the frame to display the current mode
frame .top.f4.leftcol
pack .top.f4.leftcol -side left

# create the label for nbcomp value
label .top.f4.leftcol.mode
label .top.f4.leftcol.mode.text -text "Mode : "
label .top.f4.leftcol.mode.value -textvariable mode -width 12
pack .top.f4.leftcol.mode -side top
pack .top.f4.leftcol.mode.text -side left
pack .top.f4.leftcol.mode.value -side left


# what to do in case of window closing:
wm protocol .top WM_DELETE_WINDOW closeProg
bind . "q" closeProg


#Create working images
exec dup $file_in_pgm $file_out_pgm
#exec skelcurv $file_out_pgm 0 26 $file_out_pgm
#exec /user/allenec/Scripts/dist_racine.script $file_in_pgm $file_dist_pgm
#set long_max [exec pgm2vtk $file_dist_pgm dist $file_vtk]
file copy -force $file_in_pgm $file_dist_pgm
set long_max [exec pgm2vtk $file_dist_pgm dist $file_vtk]

# Cacule le nombre de composantes connexes de l'image
set nb_comp [exec nbcomp $file_out_pgm 26 max]


# Create a cell picker.
vtkCellPicker picker
    picker AddObserver EndPickEvent pick


# Create the Renderer, RenderWindow, and RenderWindowInteractor
set renWin [.top.f1.r1 GetRenderWindow]
vtkRenderer ren1
    $renWin AddRenderer ren1
vtkRenderWindowInteractor iren
    iren SetRenderWindow $renWin
    iren SetPicker picker


#readFile $file_vtk
# Open a data source; create mapper and actor
vtkDataSetReader reader
  reader SetFileName $file_vtk
set file [reader GetOutput]

vtkDataSetMapper dataMapper
  dataMapper SetInput $file
reader Delete

vtkActor dataActor
  dataActor SetMapper dataMapper
  [dataActor GetProperty] SetColor 0 0 1


# Add the actors to the renderer, set the background and size
ren1 AddActor dataActor
ren1 SetBackground 1 1 1


# Get the camera and zoom in closer to the image.
set cam1 [ren1 GetActiveCamera]
$cam1 Zoom 1.4

# Set the user method (bound to key 'u')
iren AddObserver UserEvent {wm deiconify .vtkInteract}
iren Initialize

# Withdraw the default tk window
wm withdraw .




                 #########################
                ##      PROCEDURES       ##
############################################################
############################################################


############################################################
#                           pick                           #
############################################################
# Create a Tcl procedure to show the coordinates of the picked point
proc pick {} {
  global IM3D
  global renWin

    if { [picker GetCellId] < 0 } {
	set IM3D(cv) 0
	set IM3D(cx) 0
	set IM3D(cy) 0
	set IM3D(cz) 0
    } else {
	set IM3D(cv) 1

	#Récupération des coordonnées du point dans le référentiel de la cell:
	set cellCoord [picker GetPCoords]
	set x [expr int([lindex $cellCoord 0])]
	set y [expr int([lindex $cellCoord 1])]
	set z [expr int([lindex $cellCoord 2])]

	#Récupération des coordonnées du point dans le référentiel global:
	set pickPos [picker GetPickPosition]
	set IM3D(cx) [format "%3d" [expr int([lindex $pickPos 0])]] 
	set IM3D(cy) [format "%3d" [expr int([lindex $pickPos 1])]]
	set IM3D(cz) [format "%3d" [expr int([lindex $pickPos 2])]]
	
	#Calcul du point de la matrice correspondant:
	set IM3D(cx) [expr $IM3D(cx) - $x]
	set IM3D(cy) [expr $IM3D(cy) - $y]
	set IM3D(cz) [expr $IM3D(cz) - $z]
	
    }
    $renWin Render
}	


############################################################
#                        testCycle                         #
############################################################
# Create a Tcl procedure to show cycles
proc testCycle {} {
  global renWin
  global mode
  global file_out_pgm
  global prov_file_pgm
  global prov_dist_pgm
  global file_pgm
  global dist_pgm

  set mode "Test Cycle"

#  .top.f3.btnCycle configure -text "Retour" -command afficherVoxedit -activebackground white
  
  # Recherche du squelette ultime
  exec skeleton $file_out_pgm 0 26 $prov_file_pgm

  attributeDist $prov_file_pgm $prov_dist_pgm

  # Attribue l'image de cycles en tant qu'image courante
  set file_pgm $prov_file_pgm
  # Attribue l'image de distances de cycles en tant qu'image de distances courante
  set dist_pgm $prov_dist_pgm
    
  refresh
  $renWin Render
}


############################################################
#                    afficherVoxedit                      #
############################################################
# Create a Tcl procedure to show stumbles
proc afficherVoxedit {} {
  global renWin
  global mode
  global file_dist_pgm
  global file_out_pgm
  global file_pgm
  global dist_pgm

    set mode "Global view"

#    .top.f3.btnCycle configure -text "Test cycle" -command testCycle -activebackground white

    # Supprime image courante si différente de l'image de sortie
    if { $file_pgm != $file_out_pgm} {
      exec rm $file_pgm
    }
    # Supprime image de distances courante si différente de l'image de distances globales
    if { $dist_pgm != $file_dist_pgm } {
      exec rm $dist_pgm
    }

    # Attribue l'image de sortie en image courante
    set file_pgm $file_out_pgm
    # Attribue l'image de distances globale en image de distances courante
    set dist_pgm $file_dist_pgm
    
    refresh
    $renWin Render
}


############################################################
#                      attributeDist                       #
############################################################
# Create a Tcl procedure to attribute initial distances to the modified pgm picture
# (the input binary picture is returned as a distance picture)
proc attributeDist {modified_pgm result_pgm} {
  global file_dist_pgm
  global mask_pgm
  
  # Génére une image de niveau 1
  exec genimage $modified_pgm 1 $mask_pgm

  # Met le fichier modifié à la valeur 1 dans le mask
  exec min $modified_pgm $mask_pgm $mask_pgm

  # Conversion du masque en type long
  exec byte2long $mask_pgm $mask_pgm

  # Récupère la distance à la racine des points des cycles
  exec mult $mask_pgm $file_dist_pgm $result_pgm

  # Supprime l'image de masque
  exec rm $mask_pgm
}


############################################################
#                       deleteVoxel                        #
############################################################
# Create a Tcl procedure to delete a selected point
proc deleteVoxel {} {
  global renWin
  global IM3D
  global file_dist_pgm
  global file_out_pgm
  global file_pgm
  global dist_pgm

    if { $IM3D(cv) == 1 } {
	# Met à 0 le point sélectionné dans l'image courante
	exec point $file_pgm $IM3D(cx) $IM3D(cy) $IM3D(cz) 0 $file_pgm
#	exec point $file_out_pgm $IM3D(cx) $IM3D(cy) $IM3D(cz) 0 $file_out_pgm

	# Met à 0 le point sélectionné dans l'image de distances courante
	exec point $dist_pgm $IM3D(cx) $IM3D(cy) $IM3D(cz) 0 $dist_pgm

	# Met à 0 le point sélectionné dans l'image de sortie si différente
	if { $file_pgm != $file_out_pgm } {
	    exec point $file_out_pgm $IM3D(cx) $IM3D(cy) $IM3D(cz) 0 $file_out_pgm
	 }
	# Met à 0 le point sélectionné dans l'image de distances globale si différente
	if { $dist_pgm != $file_dist_pgm } {
	    exec point $file_dist_pgm $IM3D(cx) $IM3D(cy) $IM3D(cz) 0 $file_dist_pgm
	 }
	
	refresh
    }
    $renWin Render
}


############################################################
#                        deleteComp                        #
############################################################
# Create a Tcl procedure to delete a selected component
proc deleteComp {} {
  global renWin
  global IM3D
  global file_dist_pgm
  global file_out_pgm
  global file_pgm
  global dist_pgm

    if { $IM3D(cv) == 1 } {
	# Détruit la composante connexe sélectionnée dans l'image de sortie
	exec deletecomp $file_pgm 26 $IM3D(cx) $IM3D(cy) $IM3D(cz) $file_pgm
#	exec deletecomp $file_out_pgm 26 $IM3D(cx) $IM3D(cy) $IM3D(cz) $file_out_pgm

	# Détruit la composante connexe sélectionnée dans l'image de distances courante
  	attributeDist $file_pgm $dist_pgm

	# Détruit la composante connexe sélectionnée dans l'image de sortie si différente
	if { $file_pgm != $file_out_pgm } {
	    exec deletecomp $file_out_pgm 26 $IM3D(cx) $IM3D(cy) $IM3D(cz) $file_out_pgm
	 }
	# Détruit la composante connexe sélectionnée dans l'image de distances globale si différente
	if { $dist_pgm != $file_dist_pgm } {
	    attributeDist $file_out_pgm $file_dist_pgm
	 }
	
	refresh
    }
    $renWin Render
}


############################################################
#                          undo                            #
############################################################
# Create a Tcl procedure to undo the last action performed
proc undo {} {
    puts "Undo"
#    $renWin Render
}


############################################################
#                        readFile                          #
############################################################
# Create a Tcl procedure to read a VTK file
proc readFile {} {
  global file_vtk

    # Open a data source; create mapper and actor
    vtkDataSetReader reader
      reader SetFileName $file_vtk
    set file [reader GetOutput]

    vtkDataSetMapper dataMapper
      dataMapper SetInput $file
    reader Delete

    vtkActor dataActor
      dataActor SetMapper dataMapper
      [dataActor GetProperty] SetColor 0 0 1
}


############################################################
#                      cleanScreen                         #
############################################################
# Create a Tcl procedure to refresh the rendered picture
#proc cleanScreen {} {
#  global etat
  
#  if {$etat=="testCycle"} {
#    testCycle
#  } else {
#    refresh
#  }
#}


############################################################
#                        refresh                           #
############################################################
# Create a Tcl procedure to refresh the renderer window
proc refresh {} {
  global renWin
  global IM3D
  global nb_comp
  global long_max
  global file_pgm
  global dist_pgm
  global file_vtk
  
  set cam1 [ren1 GetActiveCamera]
  set camera(ClippingRange) [$cam1 GetClippingRange]
  set camera(FocalPoint)    [$cam1 GetFocalPoint]
  set camera(Position)      [$cam1 GetPosition]
  set camera(ViewUpVector)  [$cam1 GetViewUp]

  dataMapper Delete
  dataActor Delete
  ren1 Delete

  # Convertit l'image au format VTK
  exec pgm2vtk $dist_pgm dist $long_max $file_vtk
  
  # Cacule le nombre de composantes connexes de l'image
  set nb_comp [exec nbcomp $file_pgm 26 max]

  vtkRenderer ren1
    $renWin AddRenderer ren1

  readFile

  ren1 AddActor dataActor
  ren1 SetBackground 1 1 1

  set cam1 [ren1 GetActiveCamera]
    $cam1 Zoom 1.4
    $cam1 SetClippingRange [lindex $camera(ClippingRange) 0] [lindex $camera(ClippingRange) 1]
    $cam1 SetFocalPoint    [lindex $camera(FocalPoint) 0] [lindex $camera(FocalPoint) 1] [lindex $camera(FocalPoint) 2] 
    $cam1 SetPosition      [lindex $camera(Position) 0] [lindex $camera(Position) 1] [lindex $camera(Position) 2] 
    $cam1 SetViewUp        [lindex $camera(ViewUpVector) 0] [lindex $camera(ViewUpVector) 1] [lindex $camera(ViewUpVector) 2] 

  set IM3D(cv) 0
  set IM3D(cx) 0
  set IM3D(cy) 0
  set IM3D(cz) 0
}


############################################################
#                       closeProg                          #
############################################################
# Create a Tcl procedure to close the program
proc closeProg {} {
  global file_out_pgm
  global file_dist_pgm
  global file_pgm
  global dist_pgm
  global file_vtk
    
    # Supprime les images de travail
#    exec rm $file_vtk
#    exec rm $file_dist_pgm
    
    # Supprime image courante si différente de l'image de sortie
    if { $file_pgm != $file_out_pgm} {
      exec rm $file_pgm
    }
    # Supprime image de distances courante si différente de l'image de distances globales
    if { $dist_pgm != $file_dist_pgm } {
      exec rm $dist_pgm
    }
    
    vtkCommand DeleteAllObjects
    exit
}
