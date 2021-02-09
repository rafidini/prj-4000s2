# UjoImro, 2010
# CeCILL free-software license

# Michel's TP-1

from pink import imview
from pink import manipulate as manipulate
from pink import cpp as pink

debug = True

# exo1_1
# filter the image from noise
def filter(image, radius_opening, radius_closing):
    eroded  = pink.erosball (image,   radius_opening)
    recons1 = pink.geodilat (eroded,  image, 8)
    dilated = pink.dilatball(recons1, radius_closing)
    result  = pink.geoeros  (dilated, recons1, 8)
    return result

cell = pink.readimage("../images/cell.pgm")
filtered = filter(cell,2,2)

print("imview listview test")
imview([cell, filtered])

if debug: imview(filtered)
#filtered.writeimage("cell_filtered.pgm")


# exo1_2
# eliminate the objects touching the border
def remove_objects(image):
    frame = pink.frame( pink.char_image(image.size), 255 )
    border_objects = pink.geodilat( frame, image, 8 )
    result = image - border_objects
    return result

inside = remove_objects(filtered)
if debug: imview(inside)
#inside.writeimage("inside.pgm")


# exo1_3
# fill in the holes
def fill_the_holes(image):
    frame = pink.frame(pink.char_image(image.size), 255)
    inv = pink.inverse(image)
    dilated = pink.geodilat(frame, inv, 8)
    result = pink.inverse(dilated)
    return result

noholes = fill_the_holes(inside)
noholes.writeimage("noholes.pgm")
if debug: imview(noholes)
#imview(noholes)

# exo1_4
# extraction of object with at least one hole
def find_perforated(image):
    noholes = fill_the_holes(image)
    holes = noholes - image
    objects = pink.geodilat(holes, noholes, 8)
    result = pink.min(image, objects)
    return result

perforated = find_perforated(inside)
if debug: imview(perforated)
#perforated.writeimage("perforated.pgm")

# exo1_5
# extraction of the wires from a circuit
def find_the_wires(image, seuil):
    # binarize 
    binary = pink.seuil(image, seuil, 0, 255)
    inv = pink.inverse(binary)

    # eliminate the little objects
    #filtered = filter(inv, 2, 8)
    eros = pink.erosball(inv, 2)
    filtered = pink.geodilat(eros, inv, 8)

    # fill the holes
    filled = fill_the_holes(filtered)

    # detect the joints
    eros = pink.erosball(filled, 6)
    dilat = pink.dilatball(eros, 6)
    joints = pink.geodilat(dilat, filled, 8, 1)

    # extract the wires
    result = filled - joints

    return result


def try_seuil(s):
    return pink.seuil(circuit, s, 0, 255)

circuit = pink.readimage("../images/circuit.pgm")
seuil = manipulate(try_seuil, 0, 255, circuit)
wires = find_the_wires(circuit, seuil)
if debug: imview([circuit, wires])
#wires.writeimage("wires.pgm")

print("calling surimp for package generation")
pink.surimp(circuit, wires, "wires.ppm")

# exo1_6
# extraction of the "corne" from the image 'meb.pgm'
def extract_corne(image, threshold):
    seuil = pink.seuil(image, threshold, 0, 255)
    opened = pink.openball(seuil, 3)
    geodilat = pink.geodilat(opened, seuil, 8)
    diff = seuil - geodilat
    dilated = pink.dilatball(diff, 4)
    opened2 = pink.openball(dilated, 10)
    geodilat2 = pink.geodilat(opened2, dilated, 8)
    closed = pink.closeball(geodilat2, 4)
    border = pink.border(closed, 8)
    return border

meb = pink.readimage("../images/meb.pgm")
cornes = extract_corne(meb, 65)
if debug: imview([cornes, meb])

def fcornes(q): return extract_corne(meb, q)

if debug:
    manipulate(fcornes, 0, 255, meb)
    

#pink.surimp(meb, cornes, "cornes.ppm")




# LuM end of file
