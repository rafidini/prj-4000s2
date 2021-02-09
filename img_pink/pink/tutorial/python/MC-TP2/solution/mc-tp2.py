# UjoImro, 2010
# CeCILL free-software license

# Michel's TP-2

from pink import imview
from pink import cpp as pink

debug = True

#sol_outils1
# extraction of thick simple objects

outils = pink.readimage("../images/outils.pgm")
skeleton = pink.skeleton( outils, 0, 8 )
#pink.surimp(outils, skeleton, "skeletonized.ppm")
isolated_points = pink.ptisolated( skeleton, 8 )
#pink.surimp(skeleton, isolated_points, "ptisolated.ppm")
simple_objects = pink.geodilat( isolated_points, outils, 8 )
#pink.surimp(outils, dilated, "dilated.ppm")
eroded = pink.erosball( simple_objects, 10)
thick = pink.geodilat( eroded, outils, 8 )
pink.surimp(outils, thick, "thick.ppm")


# sol_outils2
# extraction of thin simple objects
thin = simple_objects - thick
pink.surimp( outils, thin, "thin.ppm" )


# sol_outils3
# extraction of object with more than one holes
junctions = pink.ptjunction( skeleton, 8 )
holes = pink.geodilat(junctions, outils, 8)
if debug:
    imview([outils, holes])
    pink.surimp( outils, holes, "holes.ppm" )

# sol_outils4
# objects with one hole
single_hole = outils - ( thick + thin + holes )
if debug:
    imview([outils, single_hole])
    pink.surimp( outils, single_hole, "single_hole.ppm" )


# sol_airport
# runway extraction
def extract_runways( image, brightness_threshold=23, beed_filter_radius=3 ):
    seuil = pink.seuil( image, brightness_threshold )
    inv = pink.inverse(seuil)
    asf = pink.asfbin(inv, beed_filter_radius)
    skeleton = pink.skeleton(asf, 0, 8)
    ptcurve = pink.ptcurve(skeleton, 8)
    dilated = pink.geodilat(ptcurve, asf, 8)
    skelcurv = pink.skelcurv(dilated, 0, 8)
    return skelcurv
    
    
airport = pink.readimage("../images/airport.pgm")
runways = extract_runways(airport)

if debug: 
    runways.writeimage("runways.pgm")
    pink.surimp(airport, runways, "surimp.ppm")

if debug:
    imview([airport, runways])

# sol_muscle
# extract the cells of a musle-fiber-cut image
def extract_cells(image, threshold=24):

    # creating the structuring element 
    elem = pink.char_image([3,3])
    elem.fill(1)
    elem.center=[1,1]

    grad = pink.gradmorph( muscle, elem )
    seuil = pink.seuil( grad, threshold )
    frame = pink.frame( pink.char_image(image.size), 255 )
    dilated = pink.geodilat( frame, seuil, 8 )
    skeleton = pink.skeleton( dilated, 0, 8 )
    inv = pink.inverse(skeleton)
    eroded = pink.erosball( inv, 5 )
    inv = pink.inverse(eroded)
    skeleton2 = pink.skeleton( inv, image, 4 )

    return skeleton2

muscle = pink.readimage("../images/muscle.pgm")
cells = extract_cells(muscle)

if debug: 
    imview([muscle, cells])
    pink.surimp(muscle, cells, "cells.pgm")


# sol_fractures
# extract the fractures from an "Write me !!!!!!!!" image
def extract_fractures(image):
    seuil = pink.seuil(image, 123 )
    inv = pink.inverse(seuil)
    skeleton1 = pink.skelcurv( inv, 0, 8 )
    endpoints = pink.ptend( skeleton1, 8 )
    dilated = pink.dilatball( endpoints, 1, 8 )
    sub = skeleton1 - dilated
    add = endpoints + sub
    inv = pink.inverse(add)
    skeleton2 = pink.skeleton(inv, 0, 4)
    inv = pink.inverse(skeleton2)
    dilated = pink.geodilat(endpoints, inv, 4)
    return dilated


eutel = pink.readimage("../images/eutel.pgm")
fractures = extract_fractures(eutel)
#fractures.writeimage("fractures.pgm")
if debug:
    imview([eutel, fractures])
    pink.surimp(eutel, fractures, "fractures.ppm")




# LuM end of file
