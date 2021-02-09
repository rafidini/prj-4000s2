# Michel Couprie, 2011
# CeCILL free-software license

# illustration of basic binary morphological operators

from pink import imview
from pink import manipulate as manipulate
from pink import cpp as pink

global LANG
LANG = "en"

def message(mes_en, mes_fr):
    if LANG=="fr":
        print mes_fr
    else:
        print mes_en

global IMAGE
IMAGE = pink.readimage("../images/blobs.pgm")

message("\n\nDemonstration of basic morphological operators", "\n\nDemonstration des operateurs morphologiques de base")
message("Check the 'Superimpose' button in the window", "Activer l'option 'Superimpose' dans l'interface")
message("Click 'Exit' to close the window and continue", "Cliquer sur 'Exit' pour passer au suivant")
print("              ----------------")

message("Demonstration of erosions", "demonstration de l'erosion")

message("  - structuring element (se) = 4-distance ball", "  - es = boule de la 4-distance")
def try_erosion4(radius): return pink.erosball(IMAGE, radius, 4)
manipulate(try_erosion4, 0, 40, IMAGE)

message("  - se = 8-distance ball", "  - es = boule de la 8-distance")
def try_erosion8(radius): return pink.erosball(IMAGE, radius, 8)
manipulate(try_erosion8, 0, 40, IMAGE)

message("  - se = Euclidean ball", "  - es = boule euclidienne")
def try_erosion(radius): return pink.erosball(IMAGE, radius)
manipulate(try_erosion, 0, 40, IMAGE)

message("Demonstration of dilations", "demonstration de la dilatation")

message("  - se = 4-distance ball", "  - es = boule de la 4-distance")
def try_dilation4(radius): return pink.dilatball(IMAGE, radius, 4)
manipulate(try_dilation4, 0, 100, IMAGE)

message("  - se = 8-distance ball", "  - es = boule de la 8-distance")
def try_dilation8(radius): return pink.dilatball(IMAGE, radius, 8)
manipulate(try_dilation8, 0, 100, IMAGE)

message("  - se = Euclidean ball", "  - es = boule euclidienne")
def try_dilation(radius): return pink.dilatball(IMAGE, radius)
manipulate(try_dilation, 0, 100, IMAGE)

message("Demonstration of morphological opening", "demonstration de l'ouverture")
def try_opening(radius): return pink.openball(IMAGE, radius)
manipulate(try_opening, 0, 40, IMAGE)

message("Demonstration of morphological closing", "demonstration de la fermeture")
def try_closing(radius): return pink.closeball(IMAGE, radius)
manipulate(try_closing, 0, 100, IMAGE)

global IMAGE2
IMAGE2 = pink.openball(IMAGE, 29)

message("Demonstration of geodesic dilation", "demonstration de la dilatation geodesique")
message("  - se = 4-neighborhood", "  - es = 4-voisinage")
def try_geodilat4(n): return pink.geodilat(IMAGE2, IMAGE, 4, n)
manipulate(try_geodilat4, 0, 250, IMAGE)

message("  - se = 8-neighborhood", "  - es = 8-voisinage")
def try_geodilat8(n): return pink.geodilat(IMAGE2, IMAGE, 8, n)
manipulate(try_geodilat8, 0, 250, IMAGE)

# LuM end of file
